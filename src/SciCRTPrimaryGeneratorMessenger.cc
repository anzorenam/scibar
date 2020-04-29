/// \file optical/SciCRT/src/SciCRTPrimaryGeneratorMessenger.cc
/// \brief Implementation of the SciCRTPrimaryGeneratorMessenger class
//
//
#include "G4UIdirectory.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"

#include "SciCRTPrimaryGeneratorAction.hh"
#include "SciCRTPrimaryGeneratorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPrimaryGeneratorMessenger::
                   SciCRTPrimaryGeneratorMessenger(SciCRTPrimaryGeneratorAction* gun)
  : fAction(gun)
{
  fGunDir = new G4UIdirectory("/SciCRT/gun/");
  fGunDir->SetGuidance("SciCRTPrimaryGenerator control");

  fSetPolarizationCmd =
                 new G4UIcmdWithADoubleAndUnit("/SciCRT/gun/optPhotonPolar",this);
  fSetPolarizationCmd->SetGuidance("Set linear polarization");
  fSetPolarizationCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fSetPolarizationCmd->SetParameterName("angle",true);
  fSetPolarizationCmd->SetUnitCategory("Angle");
  fSetPolarizationCmd->SetDefaultValue(0.);
  fSetPolarizationCmd->AvailableForStates(G4State_Idle);
 
  fSetDecayTimeConstantCmd =
           new G4UIcmdWithADoubleAndUnit("/SciCRT/gun/setDecayTimeConstant",this);
  fSetDecayTimeConstantCmd->SetGuidance("Set the decay time constant");
  fSetDecayTimeConstantCmd->SetGuidance("for the starting time of each photon");
  fSetDecayTimeConstantCmd->SetParameterName("time_const",false);
  fSetDecayTimeConstantCmd->SetUnitCategory("Time");
  fSetDecayTimeConstantCmd->SetRange("time_const>=0");
  fSetDecayTimeConstantCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPrimaryGeneratorMessenger::~SciCRTPrimaryGeneratorMessenger()
{
  delete fGunDir;
  delete fSetPolarizationCmd;
  delete fSetDecayTimeConstantCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTPrimaryGeneratorMessenger::
                           SetNewValue(G4UIcommand * command,G4String val)
{
  if ( command == fSetPolarizationCmd )
     fAction->
          SetOptPhotonPolar(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  else if ( command == fSetDecayTimeConstantCmd )
     fAction->
       SetDecayTimeConstant(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
}
