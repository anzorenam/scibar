
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPrimaryGeneratorMessenger_h
#define SciCRTPrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

class SciCRTPrimaryGeneratorAction;

class SciCRTPrimaryGeneratorMessenger : public G4UImessenger
{
  public:

    SciCRTPrimaryGeneratorMessenger(SciCRTPrimaryGeneratorAction*);
    virtual ~SciCRTPrimaryGeneratorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    SciCRTPrimaryGeneratorAction*   fAction;

    G4UIdirectory*               fGunDir;

    G4UIcmdWithADoubleAndUnit*   fSetPolarizationCmd;
    G4UIcmdWithADoubleAndUnit*   fSetDecayTimeConstantCmd;
};

#endif
