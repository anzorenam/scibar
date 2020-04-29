//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTTrackingAction_h
#define SciCRTTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class SciCRTTrackingAction : public G4UserTrackingAction {

  public:

    SciCRTTrackingAction() { };
    virtual ~SciCRTTrackingAction() { };

    virtual void PreUserTrackingAction(const G4Track*);

};

#endif
