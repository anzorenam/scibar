#ifndef SciCRTActionInitialization_h
#define SciCRTActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class SciCRTDetectorConstruction;

class B4DetectorConstruction;

class G4GeneralParticleSource;

/// Action initialization class.
///

class SciCRTActionInitialization : public G4VUserActionInitialization
{
  public:
    SciCRTActionInitialization(SciCRTDetectorConstruction*);
    virtual ~SciCRTActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
 
    virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;

  private:
    SciCRTDetectorConstruction* fDetector;
};

#endif
