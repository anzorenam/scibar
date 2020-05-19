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

SciCRTPhotonDetSD::SciCRTPhotonDetSD(G4String name)
  : G4VSensitiveDetector(name), fPhotonDetHitCollection(0)
{
  collectionName.insert("PhotonDetHitCollection");
}

SciCRTPhotonDetSD::~SciCRTPhotonDetSD() { }

void SciCRTPhotonDetSD::Initialize(G4HCofThisEvent* HCE)
{
  fPhotonDetHitCollection =
       new SciCRTPhotonDetHitsCollection(SensitiveDetectorName,collectionName[0]);
  //Almacena la coleccion del evento y ID
  static G4int HCID = -1;
  if (HCID<0) HCID = GetCollectionID(0);
  HCE->AddHitsCollection( HCID, fPhotonDetHitCollection );
}

G4bool SciCRTPhotonDetSD::ProcessHits(G4Step* , G4TouchableHistory* )
{
  return false;
}

G4bool SciCRTPhotonDetSD::ProcessHits_constStep(const G4Step* aStep,
                                             G4TouchableHistory* )
//Produce un hit y usa el postStepPoint; PostStepPoint ya que el hit es generado
//manualmente cuando hay photon hits the detector
{
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();

  // necesita conocer si este es un optical photon
  if(theTrack->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  // descubrir informacion sobre los hits
  G4StepPoint* thePostPoint=aStep->GetPostStepPoint();

  SciCRTUserTrackInformation* trackInformation
      = (SciCRTUserTrackInformation*)theTrack->GetUserInformation();

  G4TouchableHistory* theTouchable
      = (G4TouchableHistory*)(thePostPoint->GetTouchable());

  G4double kcon=6.62607015*(1.0/1.602176634)*0.299792458*(1e-3);
  G4ThreeVector photonExit=trackInformation->GetExitPosition();
  G4ThreeVector photonArrive=thePostPoint->GetPosition();
  G4double arrivalTime=theTrack->GetGlobalTime();
  G4double pen=theTrack->GetKineticEnergy();
  G4double waveLen=kcon/pen;
  photonArrive = theTouchable->GetHistory()->
                                GetTopTransform().TransformPoint(photonArrive);

  // crea el hit y lo añade a la coleccion
  fPhotonDetHitCollection->
            insert(new SciCRTPhotonDetHit(photonExit,photonArrive,arrivalTime,waveLen));

  return true;
}
