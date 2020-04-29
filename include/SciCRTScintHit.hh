#ifndef SciCRTScintHit_h
#define SciCRTScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class SciCRTScintHit : public G4VHit
{
  public:
 
    SciCRTScintHit();
    SciCRTScintHit(G4VPhysicalVolume* pVol);
    virtual ~SciCRTScintHit();
    SciCRTScintHit(const SciCRTScintHit &right);
    const SciCRTScintHit& operator=(const SciCRTScintHit &right);
    G4int operator==(const SciCRTScintHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

  private:
    G4double fEdep;
    G4ThreeVector fPos;
    const G4VPhysicalVolume* fPhysVol;

};

typedef G4THitsCollection<SciCRTScintHit> SciCRTScintHitsCollection;

extern G4ThreadLocal G4Allocator<SciCRTScintHit>* SciCRTScintHitAllocator;

inline void* SciCRTScintHit::operator new(size_t)
{
  if(!SciCRTScintHitAllocator)
      SciCRTScintHitAllocator = new G4Allocator<SciCRTScintHit>;
  return (void *) SciCRTScintHitAllocator->MallocSingle();
}

inline void SciCRTScintHit::operator delete(void *aHit)
{
  SciCRTScintHitAllocator->FreeSingle((SciCRTScintHit*) aHit);
}

#endif
