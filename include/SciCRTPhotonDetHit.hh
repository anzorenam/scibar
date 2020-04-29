
/// \file optical/SciCRT/include/SciCRTPhotonDetHit.hh
/// \brief Definition of the SciCRTPhotonDetHit class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPhotonDetHit_h
#define SciCRTPhotonDetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class G4VTouchable;

//--------------------------------------------------
// SciCRTPhotonDetHit Class
//--------------------------------------------------

class SciCRTPhotonDetHit : public G4VHit
{
  public:

    SciCRTPhotonDetHit();
    SciCRTPhotonDetHit(G4ThreeVector pExit, G4ThreeVector pArrive, G4double pTime);
    virtual ~SciCRTPhotonDetHit();

    SciCRTPhotonDetHit(const SciCRTPhotonDetHit &right);
    const SciCRTPhotonDetHit& operator=(const SciCRTPhotonDetHit& right);

    G4int operator==(const SciCRTPhotonDetHit& right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    inline void SetArrivalPos(G4ThreeVector xyz) { fPosArrive = xyz; }
    inline G4ThreeVector GetArrivalPos() { return fPosArrive; }

    inline void SetExitPos(G4ThreeVector xyz) { fPosExit = xyz; }
    inline G4ThreeVector GetExitPos() { return fPosExit; }

    inline void SetArrivalTime(G4double t) { fArrivalTime = t; }
    inline G4double GetArrivalTime() { return fArrivalTime; }
 
  private:

    // the arrival time of the photon
    G4double      fArrivalTime;
    // where the photon hit the detector (detector's coordinate)
    G4ThreeVector fPosArrive;
    // where the photon exited the fiber (world's coordinate)
    G4ThreeVector fPosExit;

};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<SciCRTPhotonDetHit> SciCRTPhotonDetHitsCollection;

extern G4ThreadLocal G4Allocator<SciCRTPhotonDetHit>* SciCRTPhotonDetHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* SciCRTPhotonDetHit::operator new(size_t)
{
  if(!SciCRTPhotonDetHitAllocator)
      SciCRTPhotonDetHitAllocator = new G4Allocator<SciCRTPhotonDetHit>;
  return (void *) SciCRTPhotonDetHitAllocator->MallocSingle();
}

inline void SciCRTPhotonDetHit::operator delete(void *aHit)
{
  SciCRTPhotonDetHitAllocator->FreeSingle((SciCRTPhotonDetHit*) aHit);
}

#endif
