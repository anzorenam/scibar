
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTSteppingActionMessenger_h
#define SciCRTSteppingActionMessenger_h 1

#include "G4UImessenger.hh"

class SciCRTSteppingAction;

class G4UIdirectory;
class G4UIcmdWithAnInteger;

class SciCRTSteppingActionMessenger : public G4UImessenger
{
  public:

    SciCRTSteppingActionMessenger(SciCRTSteppingAction* );
    virtual ~SciCRTSteppingActionMessenger();

    virtual void SetNewValue(G4UIcommand* ,G4String );

  private:

    SciCRTSteppingAction* fSteppingAction;

    G4UIdirectory*     fSteppingDir;
 
    G4UIcmdWithAnInteger* fSetBounceLimitCmd;

};

#endif
