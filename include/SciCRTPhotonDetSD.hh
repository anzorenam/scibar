//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPhotonDetSD_h
#define SciCRTPhotonDetSD_h 1

#include "SciCRTPhotonDetHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SciCRTPhotonDetSD : public G4VSensitiveDetector
{
  public:

    SciCRTPhotonDetSD(G4String );
    virtual ~SciCRTPhotonDetSD();

    virtual void Initialize(G4HCofThisEvent* );

    virtual G4bool ProcessHits(G4Step* , G4TouchableHistory* );
    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step* ,
                                 G4TouchableHistory* );

  private:

    SciCRTPhotonDetHitsCollection* fPhotonDetHitCollection;
};

#endif
