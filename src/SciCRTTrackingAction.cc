/// \file optical/SciCRT/src/SciCRTTrackingAction.cc
/// \brief Implementation of the SciCRTTrackingAction class
//
//
#include "globals.hh"
#include "G4RunManager.hh"

#include "SciCRTTrajectory.hh"

#include "SciCRTUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"

#include "SciCRTTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new SciCRTTrajectory(aTrack));

  SciCRTUserTrackInformation* trackInformation = new SciCRTUserTrackInformation();

  if (aTrack->GetMomentumDirection().z()>0.0) {
     trackInformation->AddStatusFlag(right);
  } else {
     trackInformation->AddStatusFlag(left);
  }

  G4String PVName = aTrack->GetVolume()->GetName();

  if (PVName == "SciCRTFiber" || PVName == "Clad1" || PVName == "Clad2")
     trackInformation->AddStatusFlag(InsideOfFiber);

  fpTrackingManager->SetUserTrackInformation(trackInformation);
}
