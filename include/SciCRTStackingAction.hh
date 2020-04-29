
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTStackingAction_h
#define SciCRTStackingAction_h 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class SciCRTStackingAction : public G4UserStackingAction
{
  public:

    SciCRTStackingAction();
    virtual ~SciCRTStackingAction();

  public:

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* );
    virtual void NewStage();
    virtual void PrepareNewEvent();

  private:

    G4int fPhotonCounter;


};

#endif
