/// \file optical/SciCRT/include/SciCRTEventAction.hh
/// \brief Definition of the SciCRTEventAction class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTEventAction_h
#define SciCRTEventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"

class SciCRTRunAction;
class SciCRTEventActionMessenger;

class SciCRTEventAction : public G4UserEventAction
{
  public:

    SciCRTEventAction(SciCRTRunAction*);
    virtual ~SciCRTEventAction();

  public:

    virtual void   BeginOfEventAction(const G4Event*);
    virtual void     EndOfEventAction(const G4Event*);

    G4int GetEventNo();
    void SetEventVerbose(G4int);

  private:

    SciCRTRunAction* fRunAction;
    SciCRTEventActionMessenger* fEventMessenger;

    G4int fVerboseLevel;

    G4int fMPPCCollID;
    G4int fScintCollID;
};

#endif
