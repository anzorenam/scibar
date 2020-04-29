//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTEventActionMessenger_h
#define SciCRTEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class EventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class SciCRTEventActionMessenger: public G4UImessenger
{
  public:

    SciCRTEventActionMessenger(SciCRTEventAction*);
    virtual ~SciCRTEventActionMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    SciCRTEventAction* fEventAction;

    G4UIcmdWithAnInteger* fSetVerboseCmd;
};

#endif
