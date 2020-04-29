
/// \file optical/SciCRT/include/SciCRTSteppingVerbose.hh
/// \brief Definition of the SciCRTSteppingVerbose class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTSteppingVerbose_h
#define SciCRTSteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class SciCRTSteppingVerbose : public G4SteppingVerbose
{
  public:

    SciCRTSteppingVerbose();
    virtual ~SciCRTSteppingVerbose();

    virtual void StepInfo();
    virtual void TrackingStarted();
};

#endif
