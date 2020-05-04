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
 :fRunAction(runaction),fVerboseLevel(0),fTotalEdep(0.)
{
  fMPPCCollID=0;
  //inicializo
  fScintCollID=0;
  fEventMessenger=new SciCRTEventActionMessenger(this);
}

SciCRTEventAction::~SciCRTEventAction()
{
  delete fEventMessenger;
}

void SciCRTEventAction::BeginOfEventAction(const G4Event* evt)
{
 fTotalEdep=0.;
 G4int evtNb=evt->GetEventID();
 if(fVerboseLevel>0)
    G4cout << "<<< Evento  " << evtNb << " iniciado." << G4endl;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4String colName="PhotonDetHitCollection";
  fMPPCCollID=SDman->GetCollectionID(colName);

  //get hit
  fScintCollID = SDman->GetCollectionID("scintCollection");
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  SciCRTPhotonDetHitsCollection* mppcHC=0;
  //se declara en el hh
  SciCRTScintHitsCollection* scintHC=0;

  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Get the hit collections
  if (HCE)
  {
    if(fMPPCCollID>=0) mppcHC=(SciCRTPhotonDetHitsCollection*)(HCE->GetHC(fMPPCCollID));
	  if(fScintCollID>=0) scintHC=(SciCRTScintHitsCollection*)(HCE->GetHC(fScintCollID));
  }

  G4int evento=evt->GetEventID();
  G4double Kconv=6.62607015*(1.0/1.602176634)*0.299792458*(1e-3);
  G4double energy=Kconv*(1.0/(evt->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy()));
  // Get hit information about photons that reached the detector in this event
  if (mppcHC)
  {
	 G4int n_hit = mppcHC->entries();
     if (n_hit!=0){
       analysisManager->FillNtupleIColumn(1,0,evento);
       analysisManager->FillNtupleDColumn(1,1,energy);
       analysisManager->FillNtupleIColumn(1,2,n_hit);
       analysisManager->AddNtupleRow(1);
       G4double tiempo;
       for(int i=0; i<n_hit;i++){
		     tiempo=(*mppcHC)[i]->GetArrivalTime();
         analysisManager->FillNtupleIColumn(2,0,evento);
		     analysisManager->FillNtupleIColumn(2,1,i);
		     analysisManager->FillNtupleDColumn(2,2,tiempo);
		     analysisManager->AddNtupleRow(2);
		   }
	   }
  }

//  if (fTotalEdep > 0.) {
//    G4double TotEdep=Kconv*(1.0/GetEdep());
//    analysisManager->FillNtupleIColumn(1,0,evento);
//    analysisManager->FillNtupleDColumn(1,2,energy);
//    analysisManager->FillNtupleDColumn(1,1,TotEdep);
//	  analysisManager->AddNtupleRow(1);
// }

}

G4int SciCRTEventAction::GetEventNo()
{
  return fpEventManager->GetConstCurrentEvent()->GetEventID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SciCRTEventAction::SetEventVerbose(G4int level)
{
  fVerboseLevel = level;
}
