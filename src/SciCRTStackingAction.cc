#include "SciCRTStackingAction.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"

#include "SciCRTAnalysis.hh"
#include <iomanip>

SciCRTStackingAction::SciCRTStackingAction() : fPhotonCounter(0) { }

SciCRTStackingAction::~SciCRTStackingAction() {}

G4ClassificationOfNewTrack
      SciCRTStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
  G4ParticleDefinition* particleType=aTrack->GetDefinition();
  if (aTrack->GetParentID()==0){
   return fUrgent;}

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {
    if  (aTrack->GetVolume()->GetName()=="SciCRTFiber" || aTrack->GetVolume()->GetName()=="Scintillator"){
     fPhotonCounter++;
     return fUrgent;
    }
  } else {
  }
  return fUrgent;
}

void SciCRTStackingAction::NewStage(){
   G4AnalysisManager* analysisManager=G4AnalysisManager::Instance();
   analysisManager->FillNtupleIColumn(1,3,fPhotonCounter);
}

void SciCRTStackingAction::PrepareNewEvent() {fPhotonCounter=0;}
