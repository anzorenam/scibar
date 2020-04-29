//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTRunActionMessenger_h
#define SciCRTRunActionMessenger_h 1

#include "globals.hh"

#include "G4UImessenger.hh"

class SciCRTRunAction;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class SciCRTRunActionMessenger : public G4UImessenger
{
  public:

    SciCRTRunActionMessenger(SciCRTRunAction* );
    virtual ~SciCRTRunActionMessenger();

    virtual void SetNewValue(G4UIcommand* ,G4String );

  private:

    SciCRTRunAction*              fRunAction;

    G4UIdirectory*             fRndmDir;
    G4UIcmdWithAnInteger*      fRndmSaveCmd;
    G4UIcmdWithAString*        fRndmReadCmd;
    G4UIcmdWithABool*          fSetAutoSeedCmd;

};

#endif
