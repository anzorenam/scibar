#include "SciCRTAnalysis.hh"
#include "SciCRTEventAction.hh"
#include "SciCRTRunAction.hh"
#include "SciCRTEventActionMessenger.hh"
#include "SciCRTPhotonDetHit.hh"
#include "SciCRTScintHit.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "SciCRTTrajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include <iomanip>

// Purpose: Acumula informacion estadistica sobre hits en el PhotonDet

SciCRTEventAction::SciCRTEventAction(SciCRTRunAction* runaction)
 : fRunAction(runaction), fVerboseLevel(0)
{
  fMPPCCollID=0;
  //inicializo
  fScintCollID=0;
  fEventMessenger = new SciCRTEventActionMessenger(this);
}

SciCRTEventAction::~SciCRTEventAction()
{
  delete fEventMessenger;
}

void SciCRTEventAction::BeginOfEventAction(const G4Event* evt)
{
 G4int evtNb = evt->GetEventID();
 if(fVerboseLevel>0)
    G4cout << "<<< Evento  " << evtNb << " iniciado." << G4endl;
}

#include "G4Threading.hh"
void SciCRTEventAction::EndOfEventAction(const G4Event* evt)
{
  if (fVerboseLevel>0)
     G4cout << "<<< Evento  " << evt->GetEventID() << " finalizado." << G4endl;

  if (fRunAction->GetRndmFreq()==2)
    {
     std::ostringstream os;
     os<<"endOfEvent_"<<G4Threading::G4GetThreadId()<<".rndm";
     G4Random::saveEngineStatus(os.str().c_str());
    }

  // Get Hits from the detector if any
  G4SDManager * SDman=G4SDManager::GetSDMpointer();
  G4String colName="PhotonDetHitCollection";
  fMPPCCollID=SDman->GetCollectionID(colName);

  fScintCollID=SDman->GetCollectionID("scintCollection");
  G4HCofThisEvent* HCE=evt->GetHCofThisEvent();
  SciCRTPhotonDetHitsCollection* mppcHC=0;
  //se declara en el hh
  SciCRTScintHitsCollection* scintHC=0;

  // get analysis manager
  G4AnalysisManager* analysisManager=G4AnalysisManager::Instance();

  // Get the hit collections
  if (HCE)
  {
    if(fMPPCCollID>=0) mppcHC=(SciCRTPhotonDetHitsCollection*)(HCE->GetHC(fMPPCCollID));
	  if(fScintCollID>=0) scintHC=(SciCRTScintHitsCollection*)(HCE->GetHC(fScintCollID));
  }

  G4int evento=evt->GetEventID();
  G4double energy=evt->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();

  if (mppcHC)
  {
	 G4int n_hit=mppcHC->entries();
     if (n_hit!=0){
       // fill histograms solo cuando se usa g4root, de otra manera se llena la tuple
       analysisManager->FillNtupleIColumn(1,0,evento);
       analysisManager->FillNtupleDColumn(1,1,energy);
       analysisManager->FillNtupleIColumn(1,4,n_hit);
	     //analysisManager->AddNtupleRow(1);
	     G4cout << "Numero de evento: "<< evento << G4endl;
       //Dato importante Marcos
       G4double tiempo,wavelen;
       for(int i=0;i<n_hit;i++){
		     tiempo=(*mppcHC)[i]->GetArrivalTime();
         wavelen=(*mppcHC)[i]->GetWaveLen();
		     // fill ntuple
         analysisManager->FillNtupleIColumn(2,0,evento);
		     analysisManager->FillNtupleIColumn(2,1,i);
		     analysisManager->FillNtupleDColumn(2,2,tiempo);
         analysisManager->FillNtupleDColumn(2,3,wavelen/nm);
		     analysisManager->AddNtupleRow(2);
		   }
	   }
  }
  //Hits in scintillator
  //le voy a quitar la condicion de verbose
  if(scintHC){
		int n_hitt=scintHC->entries();
		G4double edep;
		G4double TotE;
		for(int i=0; i<n_hitt; i++){ //gather info on hits in scintillator
			edep=(*scintHC)[i]->GetEdep();
      if ((*scintHC)[i]->GetPos().y() < 0.0){
			//suma de la edep
			  TotE+=edep;
        }
		 }
		if(TotE==0.){
			G4cout<<"No hay hits en el centellador en este evento."<<G4endl;
		}
		G4double energy_out=TotE/MeV;
		analysisManager->FillNtupleDColumn(1,2,energy_out);
	  analysisManager->AddNtupleRow(1);
	 }
}

G4int SciCRTEventAction::GetEventNo()
{
  return fpEventManager->GetConstCurrentEvent()->GetEventID();
}

void SciCRTEventAction::SetEventVerbose(G4int level)
{
  fVerboseLevel=level;
}
