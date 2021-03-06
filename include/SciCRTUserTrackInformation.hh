

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTUserTrackInformation_h
#define SciCRTUserTrackInformation_h 1

#include "G4VUserTrackInformation.hh"

#include "G4ThreeVector.hh"

enum TrackStatus { undefined=0,
                   left=1, right=2, defined = 3,
                   EscapedFromSide=4, EscapedFromReadOut=8,
                   ReflectedAtMirror=16, ReflectedAtReadOut=32,
                   murderee=64, InsideOfFiber=128, OutsideOfFiber=256};

/*TrackStatus:
  undefined:
  left:                   track is going -z
  right:                  track is going +z
  defined:                left or right flag is on (Can't be Set)
  EscapedFromSide:        photon escaped through the side of the fiber
  EscapedFromReadOut:     photon escaped through the read-out end of the fiber
  ReflectedAtMirror:      photon has been reflected by the mirror at far end
  ReflectedAtReadOut:     photon has been reflected at the read-out end
  murderee                photon is artificially killed
  InsideOfFiber           Flag is on if the photon is inside of fiber
  OutsideOfFiber          Flag is on if the photon is outside of fiber
*/

class SciCRTUserTrackInformation : public G4VUserTrackInformation
{

  public:

    SciCRTUserTrackInformation();
    virtual ~SciCRTUserTrackInformation();
 
    const G4ThreeVector& GetExitPosition() const { return fExitPosition; }
    void SetExitPosition (const G4ThreeVector& pos) { fExitPosition = pos; }

    // Try adding a status flag and return if it is successful or not
    // Cannot Add Undefine or a flag that conflicts with another flag
    // Return true if the addition of flag is successful, false otherwise
    G4bool AddStatusFlag(TrackStatus s);

    // Check if a certain flag is on
    G4bool isStatus(TrackStatus s)
       { return s == undefined ? !(fStatus &= defined) : fStatus & s; }

  private:

    G4int fStatus;
    G4ThreeVector fExitPosition;

};

#endif
