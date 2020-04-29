/// \file optical/SciCRT/src/SciCRTDetectorConstruction.cc
/// \brief Implementation of the SciCRTDetectorConstruction class
//
//
#include "G4ios.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "SciCRTDetectorConstruction.hh"
#include "SciCRTDetectorMessenger.hh"
#include "SciCRTMaterials.hh"
#include "SciCRTPhotonDetSD.hh"
#include "SciCRTScintSD.hh"
#include "G4UserLimits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTDetectorConstruction::SciCRTDetectorConstruction()
  : fMaterials(NULL), fLogicHole(NULL), fLogicWorld(NULL),
    fPhysiWorld(NULL), fPhysiHole(NULL)
{
  fDetectorMessenger = new SciCRTDetectorMessenger(this);
  //no se puede cambiar este parametro
  fNumOfCladLayers = 2;//
  //idealmente liso, es mejor este parametro
  //reflection according to Fresnel's equation p.21 de G.Ros
  fSurfaceRoughness = 1;
  //este valor es constante
  fMirrorToggle = true;
  fMirrorPolish = 1.;

  //ya lo sustitui
  fMirrorReflectivity = 1.;

  fMPPCPolish = 1.;
  //es de esperarse porque tiene que absorberse ya que es el photocath
  fMPPCReflectivity = 0.;
  //por recomendacion de medina 0.5 ya que no esta perfectamente pulido
  fExtrusionPolish = 0.5;

  //este parametro es cte ya que la fibra es redonda
  fXYRatio = 1.0;
  //longitud de la fibra
  fSciCRTfiberZ     = 1.6*m;//
  fSciCRTfiberRY  = 0.75*mm;//
  //
  fSciCRTfiberOrigin =10*cm;//
  fMPPCShape = "Circle";
  fMPPCHalfL = fSciCRTfiberRY;
  fMPPCDist  = 0.00*mm;
  fMPPCTheta = 0.0*deg;
  fMPPCZ     = 0.05*mm;
  fClrfiberZ  = fMPPCZ + 10.*nm;
  //digamos espesor de la pintura
  fMirrorZ    = 0.1*mm;
  fBarLength        = 3.*m;//
  fBarBase          = 1.3*cm;//
  fHoleRadius       = 0.9*mm;
  fHoleLength       = fBarLength;
  fCoatingThickness = 0.25*mm;
  //por el momento no lo vamos a utilizar por eso definimos el valor en cero
  fCoatingRadius    = 0*mm;//
  xx =8.75*cm;
  yy=-0.65*cm;
  zz=-10*cm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTDetectorConstruction::~SciCRTDetectorConstruction()
{
  if (fDetectorMessenger) delete fDetectorMessenger;
  if (fMaterials)         delete fMaterials;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* SciCRTDetectorConstruction::Construct()
{
  if (fPhysiWorld) {
     G4GeometryManager::GetInstance()->OpenGeometry();
     G4PhysicalVolumeStore::GetInstance()->Clean();
     G4LogicalVolumeStore::GetInstance()->Clean();
     G4SolidStore::GetInstance()->Clean();
     G4LogicalSkinSurface::CleanSurfaceTable();
     G4LogicalBorderSurface::CleanSurfaceTable();
  }

  fMaterials = SciCRTMaterials::GetInstance();
  UpdateGeometryParameters();
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* SciCRTDetectorConstruction::ConstructDetector()
{
  //--------------------------------------------------
  // El universo de aire
  //--------------------------------------------------

  G4VSolid* solidWorld =new G4Box("World", 12.5*cm, 200*cm, 200.0*cm);
  fLogicWorld = new G4LogicalVolume(solidWorld,FindMaterial("G4_AIR"),"World");
  fPhysiWorld = new G4PVPlacement(0,G4ThreeVector(),fLogicWorld,"World",0,false,0);

  //-------------------------------------------------
  //experimento para la tarjeta
  //-------------------------------------------------
  //G4VSolid* solid_env =new G4Box("Env", 2.0*cm, 81.9*cm, 180.0*cm);
  //G4LogicalVolume* logic_env = new G4LogicalVolume(solid_env,FindMaterial("G4_AIR"),"Env");
  //new G4PVPlacement(0,G4ThreeVector(xx,81.9*cm,zz),logic_env,"Env",fLogicWorld,false,0);

  //--------------------------------------------------
  // Extrusion
  //--------------------------------------------------

  G4VSolid* solidExtrusion =new G4Box("Extrusion",GetBarBase()-0.05*cm,GetBarBase()/2,GetBarLength()/2);
  G4LogicalVolume* logicExtrusion =new G4LogicalVolume(solidExtrusion,FindMaterial("Coating"),"Extrusion");
 //medina aconseja un 0.5 en rugosidad y con estas caracteristicas de la superficie
  G4OpticalSurface* TiO2Surface = new G4OpticalSurface("TiO2Surface",glisur,ground,dielectric_metal,fExtrusionPolish);
  G4MaterialPropertiesTable* TiO2SurfaceProperty =new G4MaterialPropertiesTable();
  G4double p_TiO2[] = {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
                        2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
                        2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
                        2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
                        2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
                        2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
                        2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
                        3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
                        3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
                        3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};
  const G4int nbins = sizeof(p_TiO2)/sizeof(G4double);
  G4double refl_TiO2[] = {0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,
                          0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,
                          0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,
                          0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,
                          0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93,0.93};
  assert(sizeof(refl_TiO2) == sizeof(p_TiO2));
  G4double effi_TiO2[] = {0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0};
  assert(sizeof(effi_TiO2) == sizeof(p_TiO2));

  TiO2SurfaceProperty -> AddProperty("REFLECTIVITY",p_TiO2,refl_TiO2,nbins);
  TiO2SurfaceProperty -> AddProperty("EFFICIENCY",p_TiO2,effi_TiO2,nbins);
  TiO2Surface -> SetMaterialPropertiesTable(TiO2SurfaceProperty);
  new G4PVPlacement(0,G4ThreeVector(xx,yy,zz),logicExtrusion,"Extrusion",fLogicWorld,false,0);
  new G4LogicalSkinSurface("TiO2Surface",logicExtrusion,TiO2Surface);
  //-------------------------------------------------
  //********Experimento para la tarjeta**************
  //-------------------------------------------------
  //G4VSolid* solidExtrusion2 =new G4Box("Extr",GetBarBase()-0.05*cm,GetBarBase()/2,GetBarLength()/2);
  //G4LogicalVolume* logicExtrusion2 =new G4LogicalVolume(solidExtrusion2,FindMaterial("Coating"),"Extr");
  //(xx,yy+20*cm,zz)
  //new G4PVPlacement(0,G4ThreeVector(),logicExtrusion2,"Extrusion",logic_env,false,0);
  //new G4LogicalSkinSurface("TiO2Surface",logicExtrusion2,TiO2Surface);
  //new G4PVReplica("Extr",logicExtrusion2,logic_env,kYAxis,126,1.3*cm,0);

  //--------------------------------------------------
  // Scintillator
  //--------------------------------------------------
  G4VSolid* solidScintillator = new G4Box("Scintillator",
                                GetBarBase()-0.05*cm-GetCoatingThickness()-GetCoatingRadius(),
                                GetBarBase()/2-GetCoatingThickness()-GetCoatingRadius(),
                                GetBarLength()/2);
  G4LogicalVolume* logicScintillator =new G4LogicalVolume(solidScintillator,FindMaterial("Polystyrene"),"Scintillator");
  new G4PVPlacement(0,G4ThreeVector(),logicScintillator,"Scintillator",logicExtrusion,false,0);
  //---------------------------------------------------
  //***********Experimento para la tarjeta*************
  //---------------------------------------------------
  //G4VSolid* solidScintillator2 = new G4Box("Scint",
    //                            GetBarBase()-0.05*cm-GetCoatingThickness()-GetCoatingRadius(),
      //                          GetBarBase()/2-GetCoatingThickness()-GetCoatingRadius(),
        //                        GetBarLength()/2);
  //G4LogicalVolume* logicScintillator2 =new G4LogicalVolume(solidScintillator2,FindMaterial("Polystyrene"),"Scint");
  //new G4PVPlacement(0,G4ThreeVector(),logicScintillator2,"Scint",logicExtrusion2,false,0);


  // el tamaño de la fibra es cte igual que el hole
  G4VSolid* solidHole = new G4Tubs("Hole",0.0*cm,0.9*mm,fClad2Z+0.75*mm,0.*deg,360.*deg);
  fLogicHole = new G4LogicalVolume(solidHole,FindMaterial("G4_AIR"),"Hole");
  fPhysiHole = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,fSciCRTfiberOrigin+0.75*mm),fLogicHole,"Hole",logicScintillator,false,0);
  //--------------------------------------------------
  //*******Experimento con la tarjeta
  //--------------------------------------------------
  //G4VSolid* solidHole2 = new G4Tubs("Hole",0.0*cm,0.9*mm,fClad2Z+0.75*mm,0.*deg,360.*deg);
  //G4LogicalVolume* fLogicHole2 = new G4LogicalVolume(solidHole2,FindMaterial("G4_AIR"),"Hole");
  //new G4PVPlacement(0,G4ThreeVector(0.0,0.0,fSciCRTfiberOrigin+0.75*mm),fLogicHole2,"Hole",logicScintillator2,false,0);


  //--------------------------------------------------
  // Fiber- funcion siguiente
  //--------------------------------------------------

  ConstructFiber();

  //--------------------------------------------------
  // Fin de la construccion
  //--------------------------------------------------
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::ConstructFiber()
{
 if (!(fLogicHole) || !(fPhysiHole) ) {
     std::ostringstream o;
     o << "The Fiber Hole has not been constructed";
     G4Exception("SciCRTDetectorConstruction::ConstructFiber","",
                  FatalException,o.str().c_str());}

  // apuntadores a los volumenes creados donde va la fibra
  G4LogicalVolume* logicPlacement = fLogicHole;
  G4VPhysicalVolume* physiPlacement = fPhysiHole;

  //--------------------------------------------------
  // Construccion de la fibra
  //--------------------------------------------------
  // propiedades de la superficie de contorno
  G4OpticalSurface* opSurface = NULL;
  //reflection according to Fresnel's equation p.21 de G.Ros
  if (fSurfaceRoughness < 1.){
     opSurface = new G4OpticalSurface("RoughSurface",          // Surface Name
                                      glisur,                  // SetModel
                                      ground,                  // SetFinish
                                      dielectric_dielectric,   // SetType
                                      fSurfaceRoughness);}      // SetPolish

  G4LogicalVolume   *logicClad1, *logicClad2;
  G4VPhysicalVolume *physiClad1, *physiClad2;
  //REVESTIMIENTO EXTERIOR
  G4VSolid* solidClad2;
    solidClad2 = new G4Tubs("Clad2",0.,fClad2RX,fClad2Z,0.0*rad,twopi*rad);
  logicClad2  = new G4LogicalVolume(solidClad2,FindMaterial("FPethylene"),"Clad2");
  physiClad2 = new G4PVPlacement(0,G4ThreeVector(),logicClad2,"Clad2",logicPlacement,false,0);
  //no se usa
  if (opSurface) {
    new G4LogicalBorderSurface("surfaceClad2Out",physiClad2,physiPlacement,opSurface);
    new G4LogicalBorderSurface("surfaceClad2In",physiPlacement,physiClad2,opSurface);}

  //REVESTIMIENTO INTERNO
  G4VSolid* solidClad1;
    solidClad1 = new G4Tubs("Clad1",0.,fClad1RX,fClad1Z,0.0*rad,twopi*rad);
  logicClad1 = new G4LogicalVolume(solidClad1,FindMaterial("PMMA"),"Clad1");
  physiClad1 = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,0.0),logicClad1,"Clad1",logicClad2,false,0);

  //no se usa
  if (opSurface) {new G4LogicalBorderSurface("surfaceClad1Out",physiClad1,physiClad2,opSurface);
    new G4LogicalBorderSurface("surfaceClad1In",physiClad2,physiClad1,opSurface);}

  //NUCLEO
  G4VSolid* solidSciCRTfiber;
    solidSciCRTfiber =new G4Tubs("SciCRTFiber",0.,fSciCRTfiberRX,fSciCRTfiberZ,0.0*rad,twopi*rad);


  G4LogicalVolume*   logicSciCRTfiber =new G4LogicalVolume(solidSciCRTfiber,FindMaterial("Pethylene"),"SciCRTFiber");
  //aqui se puede juagr un poco con la simulacion *****
  //DBL_MAX viene de una clase que proporciona cross sections y stopping powers
  //y nos ayuda a acceder a metodos

  logicSciCRTfiber->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,0.01*ms));
  G4VPhysicalVolume* physiSciCRTfiber = new G4PVPlacement(0,
                                       G4ThreeVector(0.0,0.0,0.0),
                                       logicSciCRTfiber,
                                       "SciCRTFiber",
                                       logicClad1,
                                       false,
                                       0);

  // no se usa
  if (opSurface) {
    new G4LogicalBorderSurface("surfaceSciCRTOut",physiSciCRTfiber,physiClad1,opSurface);
    new G4LogicalBorderSurface("surfaceSciCRTIn",physiClad1,physiSciCRTfiber,opSurface);}
  //--------------------------------------------------
  // Mirror for reflection at one of the end
  //--------------------------------------------------

  // Place the mirror only if the user wants the mirror
  if (fMirrorToggle){G4VSolid* solidMirror = new G4Box("Mirror",fMirrorRmax,fMirrorRmax,fMirrorZ);
     G4LogicalVolume* logicMirror = new G4LogicalVolume(solidMirror,FindMaterial("G4_Al"),"Mirror");
     G4OpticalSurface* mirrorSurface = new G4OpticalSurface("MirrorSurface",glisur,ground,dielectric_metal,fMirrorPolish);
     G4MaterialPropertiesTable* mirrorSurfaceProperty =new G4MaterialPropertiesTable();
     G4double p_mirror[] = {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
                            2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
                            2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
                            2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
                            2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
                            2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
                            2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
                            3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
                            3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
                            3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};
     const G4int nbins = sizeof(p_mirror)/sizeof(G4double);
     G4double refl_mirror[] = {0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,
                              0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,
                              0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,
                              0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,
                              0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46,0.46};
     assert(sizeof(refl_mirror) == sizeof(p_mirror));
     G4double effi_mirror[] = {0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,0,0};
     assert(sizeof(effi_mirror) == sizeof(effi_mirror));
     mirrorSurfaceProperty->AddProperty("REFLECTIVITY",p_mirror,refl_mirror,nbins);
     mirrorSurfaceProperty->AddProperty("EFFICIENCY",p_mirror,effi_mirror,nbins);
     mirrorSurface -> SetMaterialPropertiesTable(mirrorSurfaceProperty);
     new G4PVPlacement(0,G4ThreeVector(xx,yy,fMirrorOrigin),logicMirror,"Mirror",fLogicWorld,false,0);
     new G4LogicalSkinSurface("MirrorSurface",logicMirror,mirrorSurface);
  }

  //--------------------------------------------------
  // PhotonDet
  //--------------------------------------------------
  // Purpose: Preventing direct dielectric to metal contact

  // Check for valid placement of PhotonDet
  //verdaderamente que esten alineados
  if (fMPPCTheta > std::atan(fMPPCDist / fMPPCHalfL)) {
     fMPPCTheta = 0;
     fMPPCOriginX  = std::sin(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
     fMPPCOriginZ  = -fCoupleZ+std::cos(fMPPCTheta)*(fMPPCDist+fClrfiberZ);
     G4cerr << "Invalid alignment.  Alignment Reset to 0" << G4endl;
  }

  G4VSolid* solidClrfiber= new G4Tubs("ClearFiber",0.,fClrfiberHalfL,0.3175*mm,0.0*rad,twopi*rad);
  G4LogicalVolume*   logicClrfiber =new G4LogicalVolume(solidClrfiber,FindMaterial("Glass"),"ClearFiber");
  //estas coordenadas estan bien porque esta en funcion del logicCouple no del mundo
  new G4PVPlacement(new G4RotationMatrix(CLHEP::HepRotationY(-fMPPCTheta)),
                    G4ThreeVector(0.0,0.0,1.6*m+1*mm-0.6825*mm),//aqui
                    logicClrfiber,
                    "ClearFiber",
                    logicPlacement,
                    false,
                    0);

  //--------------------------------------------------
  // PhotonDet (Sensitive Detector) fotocatodo
  //--------------------------------------------------

  // Physical Construction
  G4VSolid* solidPhotonDet=new G4Tubs("PhotonDet",0.,fMPPCHalfL,0.15875*mm,0.0*rad,twopi*rad);
  G4LogicalVolume*   logicPhotonDet =new G4LogicalVolume(solidPhotonDet,FindMaterial("Al"),"PhotonDet_LV");
  new G4PVPlacement(0,G4ThreeVector(0.0,0.0,0.15875*mm),logicPhotonDet,"PhotonDet",logicClrfiber,false,0);
  G4OpticalSurface* photonDetSurface = new G4OpticalSurface("PhotonDetSurface",glisur,polished,dielectric_metal);
  G4MaterialPropertiesTable* photonDetSurfaceProperty =new G4MaterialPropertiesTable();
  G4double p_mppc[] = {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};
  const G4int nbins = sizeof(p_mppc)/sizeof(G4double);
  G4double refl_mppc[] = {0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0};
  assert(sizeof(refl_mppc) == sizeof(p_mppc));
  G4double effi_mppc[] = {0.023,0.028,0.034,0.039,0.047,0.054,0.063,0.068,0.076,0.082,
                          0.091,0.10,0.12,0.14,0.15,0.16,0.168,0.17,0.18,0.188,
                          0.19,0.196,0.2,0.2,0.2,0.2,0.23,0.23,0.23,0.23,
                          0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,0.24,
                          0.24,0.24,0.25,0.25,0.25,0.25,0.25,0.24,0.24,0.24};
  assert(sizeof(effi_mppc) == sizeof(p_mppc));
  photonDetSurfaceProperty->AddProperty("REFLECTIVITY",p_mppc,refl_mppc,nbins);
  photonDetSurfaceProperty->AddProperty("EFFICIENCY",p_mppc,effi_mppc,nbins);
  photonDetSurface->SetMaterialPropertiesTable(photonDetSurfaceProperty);
  new G4LogicalSkinSurface("PhotonDetSurface",logicPhotonDet,photonDetSurface);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SciCRTDetectorConstruction::ConstructSDandField()
{
  if (!fmppcSD.Get()) {
     G4String mppcSDName = "SciCRT/PhotonDet";
     SciCRTPhotonDetSD* mppcSD = new SciCRTPhotonDetSD(mppcSDName);
     fmppcSD.Put(mppcSD);}
  SetSensitiveDetector("PhotonDet_LV", fmppcSD.Get(), true);

  if (!fScint_SD.Get()){
	 G4String scintSDName = "SciCRT/Scintillator";
	 SciCRTScintSD* scintSD= new SciCRTScintSD(scintSDName);
	 fScint_SD.Put(scintSD);}
  SetSensitiveDetector("Scintillator", fScint_SD.Get(),true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::UpdateGeometryParameters()
{
  fSciCRTfiberRX  = fXYRatio * fSciCRTfiberRY;

  fClad1RX = fSciCRTfiberRX + 0.03*fSciCRTfiberRX;
  fClad1RY = fSciCRTfiberRY + 0.03*fSciCRTfiberRY;
  fClad1Z  = fSciCRTfiberZ;

  fClad2RX = fClad1RX + 0.03*fSciCRTfiberRX;
  fClad2RY = fClad1RY + 0.03*fSciCRTfiberRY;
  fClad2Z  = fSciCRTfiberZ;
  //a estos tres parametros les quite el 1cm de tolerancia que tenian
  //les dejo el nombre porque no tengo mucho tiempo pero no seria
  //world sino PMT ok
  fWorldSizeX = fClad2RX   + fMPPCDist + fMPPCHalfL;
  fWorldSizeY = fClad2RY   + fMPPCDist + fMPPCHalfL;
  fWorldSizeZ = fSciCRTfiberZ + fMPPCDist + fMPPCHalfL;

  //esto seria el pegamento entre la fibra y el PMT
  fCoupleRX = fWorldSizeX;
  fCoupleRY = fWorldSizeY;
  fCoupleZ  = (fWorldSizeZ - fSciCRTfiberZ) / 2;

  fClrfiberHalfL = fMPPCHalfL;

  fMirrorRmax = fClad2RY;
 //como esta en funcion del mundo le quito el fSciCRTfiberOrigin
  fCoupleOrigin = fSciCRTfiberZ + fCoupleZ;
  //como esta en funcion del mundo le quito el fSciCRTfiberOrigin
  fMirrorOrigin = -fSciCRTfiberZ-fMirrorZ;

  fMPPCOriginX  = std::sin(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
  fMPPCOriginZ  = -fCoupleZ + std::cos(fMPPCTheta) * (fMPPCDist + fClrfiberZ);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4RotationMatrix
            SciCRTDetectorConstruction::StringToRotationMatrix(G4String rotation)
{
  // We apply successive rotations OF THE OBJECT around the FIXED
  // axes of the parent's local coordinates; rotations are applied
  // left-to-right (rotation="r1,r2,r3" => r1 then r2 then r3).

  G4RotationMatrix rot;

  unsigned int place = 0;

  while (place < rotation.size()){
        G4double angle;
        char* p;

        const G4String tmpstring=rotation.substr(place+1);

        angle = strtod(tmpstring.c_str(),&p) * deg;

        if (!p || (*p != (char)',' && *p != (char)'\0')) {
           G4cerr << "Invalid rotation specification: " <<
                                                  rotation.c_str() << G4endl;
           return rot;}

        G4RotationMatrix thisRotation;

        switch(rotation.substr(place,1).c_str()[0]){
              case 'X': case 'x':
                thisRotation = G4RotationMatrix(CLHEP::HepRotationX(angle));
                break;
              case 'Y': case 'y':
                thisRotation = G4RotationMatrix(CLHEP::HepRotationY(angle));
                break;
              case 'Z': case 'z':
                thisRotation = G4RotationMatrix(CLHEP::HepRotationZ(angle));
                break;
              default:
                G4cerr << " Invalid rotation specification: "
                       << rotation << G4endl;
                return rot;}

       rot = thisRotation * rot;
       place = rotation.find(',',place);
       if (place > rotation.size()) break;
       ++place;
  }

  return rot;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetPhotonDetGeometry (G4String shape)
// Set the Geometry of the PhotonDet detector
// Pre:  shape must be either "Circle" and "Square"
{
  if (shape == "Circle" || shape == "Square" ) fMPPCShape = shape;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetNumberOfCladding(G4int num)
// Set the number of claddings
// Pre: 0 <= num <= 2
{
  fNumOfCladLayers = num;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetSciCRTLength (G4double length)
// Set the TOTAL length of the SciCRT fiber
{
  fSciCRTfiberZ = length;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetSciCRTRadius (G4double radius)
// Set the Y radius of SciCRT fiber
{
  fSciCRTfiberRY = radius;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetClad1Radius (G4double radius)
// Set the Y radius of Cladding 1
{
  fClad1RY = radius;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetClad2Radius (G4double radius)
// Set the Y radius of Cladding 2
{
  fClad2RY = radius;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetPhotonDetHalfLength(G4double halfL)
// Set the half length of the PhotonDet detector
// The half length will be the radius if PhotonDet is circular
{
  fMPPCHalfL = halfL;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetGap (G4double gap)
// Set the distance between fiber end and PhotonDet
{
  fMPPCDist = gap;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetPhotonDetAlignment(G4double theta)
// Set the Aligment of PhotonDet with respect to the z axis
// If theta is 0 deg, then the detector is perfectly aligned
// PhotonDet will be deviated by theta from z axis
// facing towards the center of the fiber
{
  fMPPCTheta = theta;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetSurfaceRoughness(G4double roughness)
// Set the Surface Roughness between Cladding 1 and SciCRT fiber
// Pre: 0 < roughness <= 1
{
  fSurfaceRoughness = roughness;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetMirrorPolish(G4double polish)
// Set the Polish of the mirror, polish of 1 is a perfect mirror surface
// Pre: 0 < polish <= 1
{
  fMirrorPolish = polish;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetMirrorReflectivity(G4double reflectivity)
// Set the Reflectivity of the mirror, reflectivity of 1 is a perfect mirror
// Pre: 0 < reflectivity <= 1
{
  fMirrorReflectivity = reflectivity;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetPhotonDetPolish(G4double polish)
// Set the Polish of the PhotonDet, polish of 1 is a perfect mirror surface
// Pre: 0 < polish <= 1
{
  fMPPCPolish = polish;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetPhotonDetReflectivity(G4double reflectivity)
// Set the Reflectivity of the PhotonDet, reflectivity of 1 is a perfect mirror
// Pre: 0 < reflectivity <= 1
{
  fMPPCReflectivity = reflectivity;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetMirror(G4bool flag)
// Toggle to place the mirror or not at one end (-z end) of the fiber
// True means place the mirror, false means otherwise
{
  fMirrorToggle = flag;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetXYRatio(G4double r)
// Set the ratio of the x and y radius of the ellipse (x/y)
// a ratio of 1 would produce a circle
{
  fXYRatio = r;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetBarLength (G4double length)
// Set the length of the scintillator bar
{
  fBarLength = length;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetBarBase (G4double side)
// Set the side of the scintillator bar
{
  fBarBase = side;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetHoleRadius (G4double radius)
// Set the radius of the fiber hole
{
  fHoleRadius = radius;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetCoatingThickness (G4double thick)
// Set thickness of the coating on the bars
{
  fCoatingThickness = thick;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorConstruction::SetCoatingRadius (G4double radius)
// Set inner radius of the corner bar coating
{
  fCoatingRadius = radius;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetSciCRTFiberLength() { return fSciCRTfiberZ; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetBarLength() { return fBarLength; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetBarBase() { return fBarBase; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetHoleRadius() { return fHoleRadius; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetHoleLength() { return fHoleLength; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetFiberRadius() { return GetSciCRTFiberRMax(); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetCoatingThickness()
                                                   { return fCoatingThickness; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetCoatingRadius() { return fCoatingRadius; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetSciCRTFiberEnd()
{
  return fSciCRTfiberOrigin + fSciCRTfiberZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetSciCRTFiberRMax()
{
  if (fNumOfCladLayers == 2) return fClad2RY;
  if (fNumOfCladLayers == 1) return fClad1RY;
  return fSciCRTfiberRY;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SciCRTDetectorConstruction::GetSurfaceRoughness()
{
  return fSurfaceRoughness;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Return True if the fiber construction is ideal
G4bool SciCRTDetectorConstruction::IsPerfectFiber(){
  return     fSurfaceRoughness == 1. && fXYRatio == 1.
             && (!fMirrorToggle    ||
             (fMirrorPolish    == 1. && fMirrorReflectivity == 1.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* SciCRTDetectorConstruction::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}
