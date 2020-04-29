

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTTrajectoryPoint_h_seen
#define SciCRTTrajectoryPoint_h_seen 1

#include "globals.hh"

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"

#include "G4StepStatus.hh"

class G4Track;
class G4Step;
class G4VProcess;

class SciCRTTrajectoryPoint : public G4TrajectoryPoint {

//--------
  public: // without description
//--------

// Constructor/Destructor

    SciCRTTrajectoryPoint();
    SciCRTTrajectoryPoint(const G4Track* );
    SciCRTTrajectoryPoint(const G4Step* );
    SciCRTTrajectoryPoint(const SciCRTTrajectoryPoint &right);
    virtual ~SciCRTTrajectoryPoint();

// Operators

    inline void *operator new(size_t);
    inline void operator delete(void *aTrajectoryPoint);
    inline int operator==(const SciCRTTrajectoryPoint& right) const
    { return (this==&right); };

// Get/Set functions

    inline G4double GetTime() const { return fTime; };
    inline const G4ThreeVector GetMomentum() const { return fMomentum; };
    inline G4StepStatus GetStepStatus() const { return fStepStatus; };
    inline G4String GetVolumeName() const { return fVolumeName; };

// Get method for HEPRep style attributes

   virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
   virtual std::vector<G4AttValue>* CreateAttValues() const;

//---------
  private:
//---------

// Member data

    G4double fTime;
    G4ThreeVector fMomentum;
    G4StepStatus fStepStatus;
    G4String fVolumeName;

};

extern G4ThreadLocal G4Allocator<SciCRTTrajectoryPoint>* SciCRTTrajPointAllocator;

inline void* SciCRTTrajectoryPoint::operator new(size_t)
{
    if(!SciCRTTrajPointAllocator)
      SciCRTTrajPointAllocator = new G4Allocator<SciCRTTrajectoryPoint>;
    return (void *) SciCRTTrajPointAllocator->MallocSingle();
}

inline void SciCRTTrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
    SciCRTTrajPointAllocator->FreeSingle(
        (SciCRTTrajectoryPoint *) aTrajectoryPoint);
}

#endif
