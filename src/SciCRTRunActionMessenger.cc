/// \file optical/SciCRT/src/SciCRTRunActionMessenger.cc
/// \brief Implementation of the SciCRTRunActionMessenger class
//
//
#include "globals.hh"
#include "Randomize.hh"

#include "G4UImanager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "SciCRTRunAction.hh"
#include "SciCRTRunActionMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTRunActionMessenger::SciCRTRunActionMessenger(SciCRTRunAction* runaction)
  : fRunAction (runaction)
{
  fRndmDir = new G4UIdirectory("/rndm/");
  fRndmDir->SetGuidance("Rndm status control.");
 
  fRndmSaveCmd = new G4UIcmdWithAnInteger("/rndm/save",this);
  fRndmSaveCmd->
           SetGuidance("set frequency to save rndm status on external files.");
  fRndmSaveCmd->SetGuidance("freq = 0 not saved");
  fRndmSaveCmd->SetGuidance("freq > 0 saved on: beginOfRun.rndm");
  fRndmSaveCmd->SetGuidance("freq = 1 saved on:   endOfRun.rndm");
  fRndmSaveCmd->SetGuidance("freq = 2 saved on: endOfEvent.rndm");    
  fRndmSaveCmd->SetParameterName("frequency",false);
  fRndmSaveCmd->SetRange("frequency>=0 && frequency<=2");
  fRndmSaveCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  fRndmReadCmd = new G4UIcmdWithAString("/rndm/read",this);
  fRndmReadCmd->SetGuidance("get rndm status from an external file.");
  fRndmReadCmd->SetParameterName("fileName",true);
  fRndmReadCmd->SetDefaultValue ("beginOfRun.rndm");
  fRndmReadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetAutoSeedCmd = new G4UIcmdWithABool("/rndm/autoSeed",this);
  fSetAutoSeedCmd->SetGuidance("Switch on/off time-based random seeds");
  fSetAutoSeedCmd->SetGuidance(" true: run seeds determined by system time");
  fSetAutoSeedCmd->SetGuidance("false: use command 'random/resetEngineFrom'");
  fSetAutoSeedCmd->SetGuidance("Default = false");
  fSetAutoSeedCmd->SetParameterName("autoSeed", false);
  fSetAutoSeedCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTRunActionMessenger::~SciCRTRunActionMessenger()
{
  delete fRndmDir; delete fRndmSaveCmd;
  delete fRndmReadCmd; delete fSetAutoSeedCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == fRndmSaveCmd)
      fRunAction->SetRndmFreq(fRndmSaveCmd->GetNewIntValue(newValue));

  if (command == fRndmReadCmd)
  {  G4cout << "\n---> rndm status restored from file: " << newValue << G4endl;
     G4Random::restoreEngineStatus(newValue);
     G4Random::showEngineStatus();
  }

  if(command == fSetAutoSeedCmd)
      fRunAction->SetAutoSeed(fSetAutoSeedCmd->GetNewBoolValue(newValue));
}
