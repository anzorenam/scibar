/// \file optical/SciCRT/src/SciCRTStackingAction.cc
/// \brief Implementation of the SciCRTStackingAction class
//
//
#include "SciCRTStackingAction.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"

#include "SciCRTAnalysis.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTStackingAction::SciCRTStackingAction() : fPhotonCounter(0) { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTStackingAction::~SciCRTStackingAction() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
      SciCRTStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  G4ParticleDefinition* particleType = aTrack->GetDefinition();
  // keep primary particle
  if (aTrack->GetParentID() == 0){
   return fUrgent;}

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {
    if(aTrack->GetParentID()>0){
      //if(aTrack->GetVolume()->GetName()== "SciCRTFiber"){
        fPhotonCounter++;
        return fUrgent;
    //  }
    }
  }

  return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTStackingAction::NewStage() {
//   G4cout << "Numero de optical photons producidos en este evento: "
//          << fPhotonCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTStackingAction::PrepareNewEvent() { fPhotonCounter = 0; }
