//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTExtraPhysics_h
#define SciCRTExtraPhysics_h 1

#include "globals.hh"

#include "G4VPhysicsConstructor.hh"

class SciCRTExtraPhysics : public G4VPhysicsConstructor
{
  public:

    SciCRTExtraPhysics();
    virtual ~SciCRTExtraPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

};

#endif
