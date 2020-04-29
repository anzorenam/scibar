
/// \file optical/SciCRT/src/SciCRTUserTrackInformation.cc
/// \brief Implementation of the SciCRTUserTrackInformation class
//
//
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "SciCRTUserTrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTUserTrackInformation::SciCRTUserTrackInformation ()
{
   fStatus = undefined;
   fExitPosition = G4ThreeVector(0.,0.,0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTUserTrackInformation::~SciCRTUserTrackInformation () { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SciCRTUserTrackInformation::AddStatusFlag(TrackStatus s)
// Intenta añadir una bandera  y regresa si es exitoso o no
// no añade una no definida o que tiene conflicto con otra bandera
// regresa true si añade una bandera exitosa
{
   switch (s) {

      case left:
      case right:

        // Allow the user to set left or right
        // only if the track is undefined
        if (fStatus == undefined) return fStatus |= s;

        return false;
 
      case EscapedFromSide:
      case EscapedFromReadOut:

        // Allow the user to set escaped flag
        // only if the photon hasn't exited the fiber yet

        if ((fStatus == undefined) || (fStatus & OutsideOfFiber)) return false;

        return fStatus |= s;
 
      case ReflectedAtMirror:
      case ReflectedAtReadOut:
      case murderee:

        return fStatus |= s;

      case InsideOfFiber:
 
        return ( fStatus =
         (fStatus & ~(EscapedFromSide + EscapedFromReadOut + OutsideOfFiber)) | s);

      case OutsideOfFiber:

        return ( fStatus = (fStatus & ~InsideOfFiber) | s );
 
      default:
 
        return false;
   }
}
