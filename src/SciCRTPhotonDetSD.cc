/// \file optical/SciCRT/src/SciCRTPhotonDetSD.cc
/// \brief Implementation of the SciCRTPhotonDetSD class
//
//
#include "SciCRTPhotonDetSD.hh"
#include "SciCRTPhotonDetHit.hh"
#include "SciCRTUserTrackInformation.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPhotonDetSD::SciCRTPhotonDetSD(G4String name)
  : G4VSensitiveDetector(name), fPhotonDetHitCollection(0)
{
  collectionName.insert("PhotonDetHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPhotonDetSD::~SciCRTPhotonDetSD() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTPhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
  fPhotonDetHitCollection =
       new SciCRTPhotonDetHitsCollection(SensitiveDetectorName,collectionName[0]);
  //Almacena la coleccion del evento y ID
  static G4int HCID = -1;
  if (HCID<0) HCID = GetCollectionID(0);
  HCE->AddHitsCollection( HCID, fPhotonDetHitCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SciCRTPhotonDetSD::ProcessHits(G4Step* , G4TouchableHistory* )
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SciCRTPhotonDetSD::ProcessHits_constStep(const G4Step* aStep,
                                             G4TouchableHistory* )
//Produce un hit y usa el postStepPoint; PostStepPoint ya que el hit es generado
//manualmente cuando hay photon hits the detector
{
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();
  //G4double edep = aStep->GetTotalEnergyDeposit();
  //if(edep!=0){G4cout << "Energia depositada= " << G4BestUnit(edep,"Energy")<< G4endl;}
  
  
  // necesita conocer si este es un optical photon
  if(theTrack->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  // descubrir informacion sobre los hits
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
 
  SciCRTUserTrackInformation* trackInformation
      = (SciCRTUserTrackInformation*)theTrack->GetUserInformation();
 
  G4TouchableHistory* theTouchable
      = (G4TouchableHistory*)(thePostPoint->GetTouchable());
 
  G4ThreeVector photonExit   = trackInformation -> GetExitPosition();
  G4ThreeVector photonArrive = thePostPoint -> GetPosition();
  G4double      arrivalTime  = theTrack -> GetGlobalTime();

  //convienrte las coordenadas globales de los fotones que llegan en 
  // coordenadas locales del detector
  photonArrive = theTouchable->GetHistory()->
                                GetTopTransform().TransformPoint(photonArrive);

  // crea el hit y lo añade a la coleccion
  fPhotonDetHitCollection->
            insert(new SciCRTPhotonDetHit(photonExit, photonArrive, arrivalTime));

  return true;
}
