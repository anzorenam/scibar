/// \file optical/SciCRT/src/SciCRTEventActionMessenger.cc
/// \brief Implementation of the SciCRTEventActionMessenger class
//
//
#include "globals.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "SciCRTEventAction.hh"
#include "SciCRTEventActionMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTEventActionMessenger::SciCRTEventActionMessenger(SciCRTEventAction* eventaction)
  : fEventAction(eventaction)
{
  fSetVerboseCmd = new G4UIcmdWithAnInteger("/event/setverbose",this);
  fSetVerboseCmd->SetGuidance("Set verbose level ." );
  fSetVerboseCmd->SetParameterName("level",true);
  fSetVerboseCmd->SetDefaultValue(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTEventActionMessenger::~SciCRTEventActionMessenger()
{
  delete fSetVerboseCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTEventActionMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
  if (command == fSetVerboseCmd)
    fEventAction->SetEventVerbose(fSetVerboseCmd->GetNewIntValue(newValue));
}
