//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPhysicsListMessenger_h
#define SciCRTPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"

class SciCRTPhysicsList;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

/// Provide control of the physics list and cut parameters

class SciCRTPhysicsListMessenger : public G4UImessenger
{
  public:

    SciCRTPhysicsListMessenger(SciCRTPhysicsList* );
    virtual ~SciCRTPhysicsListMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    SciCRTPhysicsList* fPhysicsList;

    G4UIdirectory* fDirectory;
    G4UIdirectory* fDecayDirectory;

    G4UIcmdWithABool* fSetAbsorptionCMD;

    G4UIcmdWithAnInteger* fVerboseCmd;
    G4UIcmdWithAnInteger* fCerenkovCmd;

    G4UIcmdWithADoubleAndUnit* fGammaCutCMD;
    G4UIcmdWithADoubleAndUnit* fElectCutCMD;
    G4UIcmdWithADoubleAndUnit* fPosCutCMD;
    G4UIcmdWithADoubleAndUnit* fAllCutCMD;
    G4UIcmdWithADoubleAndUnit* fStepMaxCMD;

    G4UIcmdWithAString*        fRemovePhysicsCMD;
    G4UIcmdWithoutParameter*   fClearPhysicsCMD;

    G4UIcmdWithoutParameter*   fListCMD;

    G4UIcmdWithoutParameter* fPienuCMD;
    G4UIcmdWithoutParameter* fPimunuCMD;

};

#endif
