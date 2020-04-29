
/// \file optical/SciCRT/src/SciCRTActionInitialization.cc
/// \brief Implementation of the SciCRTActionInitialization class

#include "SciCRTActionInitialization.hh"
#include "SciCRTDetectorConstruction.hh"

#include "SciCRTPrimaryGeneratorAction.hh"

#include "SciCRTRunAction.hh"
#include "SciCRTEventAction.hh"
#include "SciCRTTrackingAction.hh"
#include "SciCRTSteppingAction.hh"
#include "SciCRTStackingAction.hh"
#include "SciCRTSteppingVerbose.hh"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTActionInitialization::SciCRTActionInitialization(SciCRTDetectorConstruction* det)
 : G4VUserActionInitialization(), fDetector(det)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTActionInitialization::~SciCRTActionInitialization()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTActionInitialization::BuildForMaster() const
{
  SetUserAction(new SciCRTRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTActionInitialization::Build() const
{
  SetUserAction(new SciCRTPrimaryGeneratorAction(fDetector));

  SciCRTRunAction* runAction = new SciCRTRunAction();
  SciCRTEventAction* eventAction = new SciCRTEventAction(runAction);

  SetUserAction(runAction);
  SetUserAction(eventAction);
  SetUserAction(new SciCRTTrackingAction());
  SetUserAction(new SciCRTSteppingAction(fDetector,eventAction));
  SetUserAction(new SciCRTStackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose* SciCRTActionInitialization::InitializeSteppingVerbose() const
{
  return new SciCRTSteppingVerbose();
}
