/// \file optical/SciCRT/src/SciCRTPhotonDetHit.cc
/// \brief Implementation of the SciCRTPhotonDetHit class
//
//
#include "SciCRTPhotonDetHit.hh"

G4ThreadLocal G4Allocator<SciCRTPhotonDetHit>* SciCRTPhotonDetHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPhotonDetHit::SciCRTPhotonDetHit()
{
  fArrivalTime = 0.;
  fPosArrive   = G4ThreeVector(0., 0., 0.);
  fPosExit     = G4ThreeVector(0., 0., 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPhotonDetHit::SciCRTPhotonDetHit(G4ThreeVector pExit,
                                 G4ThreeVector pArrive,
                                 G4double pTime)
{
  fPosExit     = pExit;
  fPosArrive   = pArrive;
  fArrivalTime = pTime;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPhotonDetHit::~SciCRTPhotonDetHit() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTPhotonDetHit::SciCRTPhotonDetHit(const SciCRTPhotonDetHit &right)
  : G4VHit()
{
  *this = right;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SciCRTPhotonDetHit& SciCRTPhotonDetHit::operator=(const SciCRTPhotonDetHit &right)
{
  fPosExit     = right.fPosExit;
  fPosArrive   = right.fPosArrive;
  fArrivalTime = right.fArrivalTime;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTPhotonDetHit::operator==(const SciCRTPhotonDetHit& right) const
{
  return fPosExit     == right.fPosExit    &&
         fPosArrive   == right.fPosArrive  &&
         fArrivalTime == right.fArrivalTime;  
}
