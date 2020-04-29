
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTRunAction_h
#define SciCRTRunAction_h 1

#include "globals.hh"

#include "G4UserRunAction.hh"

class G4Run;

class SciCRTRunActionMessenger;

class SciCRTRunAction : public G4UserRunAction
{
  public:

    SciCRTRunAction();
    virtual ~SciCRTRunAction();

  public:

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void  SetRndmFreq(G4int val) { fSaveRndm = val; }
    G4int GetRndmFreq()          { return fSaveRndm; }

    inline void SetAutoSeed (const G4bool val) { fAutoSeed = val; }

  private:
 
    SciCRTRunActionMessenger* fRunMessenger;

    G4int fSaveRndm;
    G4bool fAutoSeed;

};

#endif
