// $Id: SciCRTSteppingAction.hh 69561 2013-05-08 12:25:56Z gcosmo $
//
/// \file optical/SciCRT/include/SciCRTSteppingAction.hh
/// \brief Definition of the SciCRTSteppingAction class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTSteppingAction_h
#define SciCRTSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class SciCRTDetectorConstruction;
class SciCRTSteppingActionMessenger;

class G4Track;
class G4StepPoint;

class G4OpBoundaryProcess;

class SciCRTSteppingAction : public G4UserSteppingAction
{
  public:

    SciCRTSteppingAction(SciCRTDetectorConstruction*);
    virtual ~SciCRTSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

    // Set the bounce limit, 0 for no limit
    void  SetBounceLimit(G4int);

    G4int GetNumberOfBounces();
    G4int GetNumberOfClad1Bounces();
    G4int GetNumberOfClad2Bounces();
    G4int GetNumberOfSciCRTBounces();
    // return number of successful events and reset the counter
    G4int ResetSuccessCounter();

  private:

    // Artificially kill the photon after it has bounced more than this number
    G4int fBounceLimit;
    // number of photons that reach the end
    G4int fCounterEnd;
    // number of photons that didn't make it to the end
    G4int fCounterMid;
    // total number of bounces that a photon been through
    G4int fCounterBounce;
    // number of bounces that a photon been through within the fibre
    G4int fCounterSciCRTBounce;
    // number of bounces that a photon been through from Cladding 1 to 2
    G4int fCounterClad1Bounce;
    // number of bounces that a photon been through from Cladding 2 to World
    G4int fCounterClad2Bounce;

    // initial gamma of the photon
    G4double fInitGamma;
    // initial theta of the photon
    G4double fInitTheta;

    G4OpBoundaryProcess* fOpProcess;

    // maximum number of save states
    static G4int fMaxRndmSave;

    SciCRTDetectorConstruction* fDetector;

    SciCRTSteppingActionMessenger* fSteppingMessenger;

    inline void ResetCounters()
    {
      fCounterBounce = fCounterSciCRTBounce =
      fCounterClad1Bounce = fCounterClad2Bounce = 0;
      fInitGamma = fInitTheta = -1;
    }

    // save the random status into a sub-directory
    // Pre: subDir must be empty or ended with "/"
    inline void saveRandomStatus(G4String subDir);

};

#endif
