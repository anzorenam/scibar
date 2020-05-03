
/// \file optical/SciCRT/include/SciCRTDetectorConstruction.hh
/// \brief Definition of the SciCRTDetectorConstruction class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTDetectorConstruction_h
#define SciCRTDetectorConstruction_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4Tubs;
class G4EllipticalTube;
class G4LogicalVolume;
class G4VPhysicalVolume;
class SciCRTMaterials;
class G4Material;
class SciCRTDetectorMessenger;
class SciCRTPhotonDetSD;
class SciCRTScintSD;

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

class SciCRTDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    SciCRTDetectorConstruction();
    virtual ~SciCRTDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    G4VPhysicalVolume* ConstructDetector();

    virtual void ConstructSDandField();

    // Set Material Commands for World and SciCRTfiber
    void SetWorldMaterial         (G4String);
    void SetSciCRTFiberMaterial      (G4String);
    void SetCoupleMaterial        (G4String);

    void SetPhotonDetGeometry     (G4String);
    void SetNumberOfCladding      (G4int);        // Maximum 2 claddings

    void SetSciCRTLength             (G4double);     // Total length of SciCRT fiber
    void SetSciCRTRadius             (G4double);
    void SetClad1Radius           (G4double);
    void SetClad2Radius           (G4double);
    void SetPhotonDetHalfLength   (G4double);
    void SetGap                   (G4double);
    void SetPhotonDetAlignment    (G4double);
    // Set the ratio of x and y (x/y) radius of the ellipse
    void SetXYRatio               (G4double);
    // Set the Roughness in between each layer
    void SetSurfaceRoughness      (G4double);
    // Set the reflectivity of the mirror
    void SetMirrorReflectivity    (G4double);
    // Set the polish of the mirror
    void SetMirrorPolish          (G4double);
    // Set the reflectivity of the mirror
    void SetPhotonDetReflectivity (G4double);
    // Set the polish of the mirror
    void SetPhotonDetPolish       (G4double);

    void SetMirror                (G4bool);

    void SetBarLength             (G4double);
    void SetBarBase               (G4double);
    void SetHoleRadius            (G4double);
    void SetCoatingThickness      (G4double);
    void SetCoatingRadius         (G4double);

    G4double GetSciCRTFiberLength();
    G4double GetSciCRTFiberEnd();
    G4double GetSciCRTFiberRMax();
    G4double GetSurfaceRoughness();
    G4bool   IsPerfectFiber();

    G4double GetBarLength();
    G4double GetBarBase();
    G4double GetHoleRadius();
    G4double GetHoleLength();
    G4double GetFiberRadius();

    G4double GetCoatingThickness();
    G4double GetCoatingRadius();

    // StringToRotationMatrix() converts a string "X90,Y45" into a
    // G4RotationMatrix.
    // This is an active rotation, in that the object is first rotated
    // around the parent's X axis by 90 degrees, then the object is
    // further rotated around the parent's Y axis by 45 degrees.
    // The return value points to a G4RotationMatrix on the heap, so
    // it is persistent. Angles are in degrees, can have decimals,
    // and can be negative. Axes are X, Y, Z.

    static G4RotationMatrix StringToRotationMatrix(G4String rotation);
    G4Material* FindMaterial(G4String);

  private:

    SciCRTMaterials* fMaterials;

    G4LogicalVolume* fLogicHole;
    G4LogicalVolume* fLogicWorld;

    G4VPhysicalVolume* fPhysiWorld;
    G4VPhysicalVolume* fPhysiHole;

    G4double           fWorldSizeX;
    G4double           fWorldSizeY;
    G4double           fWorldSizeZ;

    G4double           fSciCRTfiberRX;
    G4double           fSciCRTfiberRY;
    G4double           fSciCRTfiberZ;

    G4double           fClad1RX;
    G4double           fClad1RY;
    G4double           fClad1Z;

    G4double           fClad2RX;
    G4double           fClad2RY;
    G4double           fClad2Z;

    G4double           fClrfiberHalfL;
    G4double           fClrfiberZ;

    G4double           fCoupleRX;
    G4double           fCoupleRY;
    G4double           fCoupleZ;

    G4double           fMirrorRmax;
    G4double           fMirrorZ;
    G4bool             fMirrorToggle;

    G4String           fMPPCShape;
    G4double           fMPPCHalfL;
    G4double           fMPPCZ;
    G4double           fMPPCDist;
    G4double           fMPPCTheta;

    G4double fSciCRTfiberOrigin;
    G4double fCoupleOrigin;
    G4double fMirrorOrigin;
    G4double fMPPCOriginX;
    G4double fMPPCOriginZ;

    G4int fNumOfCladLayers;

    G4double fMirrorPolish;
    G4double fMirrorReflectivity;
    G4double fMPPCPolish;
    G4double fMPPCReflectivity;
    G4double fExtrusionPolish;
    G4double fSurfaceRoughness;
    G4double fXYRatio;

    G4double fBarLength;
    G4double fBarBase;
    G4double fHoleRadius;
    G4double fHoleLength;
    G4double fCoatingThickness;
    G4double fCoatingRadius;

    G4double xx;
	  G4double yy;
	  G4double zz;

  private:

     void ConstructFiber(G4double pEn[],G4double rMi[],G4double eMi[],G4double rMPPC[],G4double eMPPC[],G4int Ne);
     void UpdateGeometryParameters();
     SciCRTDetectorMessenger* fDetectorMessenger;
     G4Cache<SciCRTPhotonDetSD*> fmppcSD;
     G4Cache<SciCRTScintSD*> fScint_SD;

};

#endif
