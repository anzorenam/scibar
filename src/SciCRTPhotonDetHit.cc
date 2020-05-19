#include "SciCRTPhotonDetHit.hh"

G4ThreadLocal G4Allocator<SciCRTPhotonDetHit>* SciCRTPhotonDetHitAllocator=0;

SciCRTPhotonDetHit::SciCRTPhotonDetHit()
{
  fwaveLen=0.;
  fArrivalTime=0.;
  fPosArrive=G4ThreeVector(0., 0., 0.);
  fPosExit=G4ThreeVector(0., 0., 0.);
}

SciCRTPhotonDetHit::SciCRTPhotonDetHit(G4ThreeVector pExit,
                                       G4ThreeVector pArrive,
                                       G4double pTime,
                                       G4double pLen)
{
  fwaveLen=pLen;
  fPosExit=pExit;
  fPosArrive=pArrive;
  fArrivalTime=pTime;
}

SciCRTPhotonDetHit::~SciCRTPhotonDetHit() { }

SciCRTPhotonDetHit::SciCRTPhotonDetHit(const SciCRTPhotonDetHit &right)
  : G4VHit()
{
  *this = right;
}

const SciCRTPhotonDetHit& SciCRTPhotonDetHit::operator=(const SciCRTPhotonDetHit &right)
{

  fPosExit=right.fPosExit;
  fPosArrive=right.fPosArrive;
  fArrivalTime=right.fArrivalTime;
  fwaveLen=right.fwaveLen;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTPhotonDetHit::operator==(const SciCRTPhotonDetHit& right) const
{
  return fPosExit     == right.fPosExit    &&
         fPosArrive   == right.fPosArrive  &&
         fArrivalTime == right.fArrivalTime;
         fwaveLen == right.fwaveLen;
}
