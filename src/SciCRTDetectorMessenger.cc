
/// \file optical/SciCRT/src/SciCRTDetectorMessenger.cc
/// \brief Implementation of the SciCRTDetectorMessenger class
//
//
#include "SciCRTDetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTDetectorMessenger::SciCRTDetectorMessenger(SciCRTDetectorConstruction * det)
  : fDetector(det)
{
  fDetDir = new G4UIdirectory("/SciCRT/");
  fDetDir->SetGuidance(" Geometry Setup ");

  fSetPhotonDetGeometryCmd =
                      new G4UIcmdWithAString("/SciCRT/setPhotonDetGeometry",this);
  fSetPhotonDetGeometryCmd->
                  SetGuidance("Select the geometry of the PhotonDet detector");
  fSetPhotonDetGeometryCmd->SetGuidance("Only Accepts 'Circle' and 'Square'");
  fSetPhotonDetGeometryCmd->SetCandidates("Circle Square");
  fSetPhotonDetGeometryCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetPhotonDetGeometryCmd->SetToBeBroadcasted(false);
 
  fSetNumOfCladLayersCmd =
                        new G4UIcmdWithAnInteger("/SciCRT/setNumOfLayers", this);
  fSetNumOfCladLayersCmd->SetGuidance("Select the number of cladding layers");
  fSetNumOfCladLayersCmd->SetGuidance("Maximum number is 2");
  fSetNumOfCladLayersCmd->SetParameterName("numberOfLayers",false);
  fSetNumOfCladLayersCmd->SetRange("numberOfLayers>=0 && numberOfLayers<=2");
  fSetNumOfCladLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetNumOfCladLayersCmd->SetToBeBroadcasted(false);

  fSetSurfaceRoughnessCmd =
                      new G4UIcmdWithADouble("/SciCRT/setSurfaceRoughness", this);
  fSetSurfaceRoughnessCmd->
                  SetGuidance("Set the roughness between Clad1 and SciCRT Fiber");
  fSetSurfaceRoughnessCmd->SetParameterName("roughness",false);
  fSetSurfaceRoughnessCmd->SetRange("roughness>0 && roughness<=1");
  fSetSurfaceRoughnessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSurfaceRoughnessCmd->SetToBeBroadcasted(false);

  fSetXYRatioCmd = new G4UIcmdWithADouble("/SciCRT/setXYRatio", this);
  fSetXYRatioCmd->SetGuidance("Set the ratio between x and y axis (x/y)");
  fSetXYRatioCmd->SetParameterName("ratio",false);
  fSetXYRatioCmd->SetRange("ratio>0 && ratio<=1");
  fSetXYRatioCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetXYRatioCmd->SetToBeBroadcasted(false);

  fSetMirrorPolishCmd = new G4UIcmdWithADouble("/SciCRT/setMirrorPolish", this);
  fSetMirrorPolishCmd->SetGuidance("Set the polish of the mirror");
  fSetMirrorPolishCmd->SetParameterName("polish",false);
  fSetMirrorPolishCmd->SetRange("polish>0 && polish<=1");
  fSetMirrorPolishCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetMirrorPolishCmd->SetToBeBroadcasted(false);

  fSetMirrorReflectivityCmd =
                    new G4UIcmdWithADouble("/SciCRT/setMirrorReflectivity", this);
  fSetMirrorReflectivityCmd->SetGuidance("Set the reflectivity of the mirror");
  fSetMirrorReflectivityCmd->SetParameterName("reflectivity",false);
  fSetMirrorReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  fSetMirrorReflectivityCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetMirrorReflectivityCmd->SetToBeBroadcasted(false);

  fSetPhotonDetPolishCmd =
                       new G4UIcmdWithADouble("/SciCRT/setPhotonDetPolish", this);
  fSetPhotonDetPolishCmd->SetGuidance("Set the polish of the mirror");
  fSetPhotonDetPolishCmd->SetParameterName("polish",false);
  fSetPhotonDetPolishCmd->SetRange("polish>0 && polish<=1");
  fSetPhotonDetPolishCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetPhotonDetPolishCmd->SetToBeBroadcasted(false);

  fSetPhotonDetReflectivityCmd =
                 new G4UIcmdWithADouble("/SciCRT/setPhotonDetReflectivity", this);
  fSetPhotonDetReflectivityCmd->
                             SetGuidance("Set the reflectivity of the mirror");
  fSetPhotonDetReflectivityCmd->SetParameterName("reflectivity",false);
  fSetPhotonDetReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  fSetPhotonDetReflectivityCmd->AvailableForStates(G4State_PreInit);
  fSetPhotonDetReflectivityCmd->SetToBeBroadcasted(false);

  fSetSciCRTLengthCmd = new G4UIcmdWithADoubleAndUnit("/SciCRT/setSciCRTLength",this);
  fSetSciCRTLengthCmd->SetGuidance("Set the half length of the SciCRT fiber");
  fSetSciCRTLengthCmd->SetParameterName("length",false);
  fSetSciCRTLengthCmd->SetRange("length>0.");
  fSetSciCRTLengthCmd->SetUnitCategory("Length");
  fSetSciCRTLengthCmd->SetDefaultUnit("mm");
  fSetSciCRTLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSciCRTLengthCmd->SetToBeBroadcasted(false);

  fSetSciCRTRadiusCmd = new G4UIcmdWithADoubleAndUnit("/SciCRT/setSciCRTRadius",this);
  fSetSciCRTRadiusCmd->SetGuidance("Set the radius of the SciCRT fiber");
  fSetSciCRTRadiusCmd->SetParameterName("radius",false);
  fSetSciCRTRadiusCmd->SetRange("radius>0.");
  fSetSciCRTRadiusCmd->SetUnitCategory("Length");
  fSetSciCRTRadiusCmd->SetDefaultUnit("mm");
  fSetSciCRTRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetSciCRTRadiusCmd->SetToBeBroadcasted(false);

  fSetClad1RadiusCmd =
                   new G4UIcmdWithADoubleAndUnit("/SciCRT/setClad1Radius",this);
  fSetClad1RadiusCmd->SetGuidance("Set the radius of Cladding 1");
  fSetClad1RadiusCmd->SetParameterName("radius",false);
  fSetClad1RadiusCmd->SetRange("radius>0.");
  fSetClad1RadiusCmd->SetUnitCategory("Length");
  fSetClad1RadiusCmd->SetDefaultUnit("mm");
  fSetClad1RadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetClad1RadiusCmd->SetToBeBroadcasted(false);

  fSetClad2RadiusCmd =
                   new G4UIcmdWithADoubleAndUnit("/SciCRT/setClad2Radius",this);
  fSetClad2RadiusCmd->SetGuidance("Set the radius of Cladding 2");
  fSetClad2RadiusCmd->SetParameterName("radius",false);
  fSetClad2RadiusCmd->SetRange("radius>0.");
  fSetClad2RadiusCmd->SetUnitCategory("Length");
  fSetClad2RadiusCmd->SetDefaultUnit("mm");
  fSetClad2RadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetClad2RadiusCmd->SetToBeBroadcasted(false);

  fSetPhotonDetHalfLengthCmd =
             new G4UIcmdWithADoubleAndUnit("/SciCRT/setPhotonDetHalfLength",this);
  fSetPhotonDetHalfLengthCmd->
                      SetGuidance("Set the half length of PhotonDet detector");
  fSetPhotonDetHalfLengthCmd->SetParameterName("halfL",false);
  fSetPhotonDetHalfLengthCmd->SetRange("halfL>0.");
  fSetPhotonDetHalfLengthCmd->SetUnitCategory("Length");
  fSetPhotonDetHalfLengthCmd->SetDefaultUnit("mm");
  fSetPhotonDetHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetPhotonDetHalfLengthCmd->SetToBeBroadcasted(false);

  fSetGapCmd = new G4UIcmdWithADoubleAndUnit("/SciCRT/setGap",this);
  fSetGapCmd->SetGuidance("Set the distance between PhotonDet and fiber end");
  fSetGapCmd->SetParameterName("theta",false);
  fSetGapCmd->SetUnitCategory("Length");
  fSetGapCmd->SetDefaultUnit("mm");
  fSetGapCmd->SetRange("theta>=0.");
  fSetGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetGapCmd->SetToBeBroadcasted(false);

  fSetPhotonDetAlignmentCmd =
                       new G4UIcmdWithADoubleAndUnit("/SciCRT/setAlignment",this);
  fSetPhotonDetAlignmentCmd->
                     SetGuidance("Set the deviation of PhotonDet from z axis");
  fSetPhotonDetAlignmentCmd->SetParameterName("theta",false);
  fSetPhotonDetAlignmentCmd->SetUnitCategory("Angle");
  fSetPhotonDetAlignmentCmd->SetDefaultUnit("deg");
  fSetPhotonDetAlignmentCmd->SetRange("theta>-90. && theta<90.");
  fSetPhotonDetAlignmentCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetPhotonDetAlignmentCmd->SetToBeBroadcasted(false);

  fSetMirrorCmd = new G4UIcmdWithABool("/SciCRT/setMirror", this);
  fSetMirrorCmd->SetGuidance("Place a mirror at the end of the fiber");
  fSetMirrorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetMirrorCmd->SetToBeBroadcasted(false);

  fSetBarLengthCmd = new G4UIcmdWithADoubleAndUnit("/SciCRT/setBarLength",this);
  fSetBarLengthCmd->SetGuidance("Set the length of the scintillator bar");
  fSetBarLengthCmd->SetParameterName("length",false);
  fSetBarLengthCmd->SetRange("length>0.");
  fSetBarLengthCmd->SetUnitCategory("Length");
  fSetBarLengthCmd->SetDefaultUnit("mm");
  fSetBarLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetBarLengthCmd->SetToBeBroadcasted(false);

  fSetBarBaseCmd = new G4UIcmdWithADoubleAndUnit("/SciCRT/setBarBase",this);
  fSetBarBaseCmd->SetGuidance("Set the side length of the scintillator bar");
  fSetBarBaseCmd->SetParameterName("length",false);
  fSetBarBaseCmd->SetRange("length>0.");
  fSetBarBaseCmd->SetUnitCategory("Length");
  fSetBarBaseCmd->SetDefaultUnit("mm");
  fSetBarBaseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetBarBaseCmd->SetToBeBroadcasted(false);

  fSetHoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/SciCRT/setHoleRadius",this);
  fSetHoleRadiusCmd->SetGuidance("Set the radius of the fiber hole");
  fSetHoleRadiusCmd->SetParameterName("radius",false);
  fSetHoleRadiusCmd->SetRange("radius>0.");
  fSetHoleRadiusCmd->SetUnitCategory("Length");
  fSetHoleRadiusCmd->SetDefaultUnit("mm");
  fSetHoleRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetHoleRadiusCmd->SetToBeBroadcasted(false);

  fSetCoatingThicknessCmd =
               new G4UIcmdWithADoubleAndUnit("/SciCRT/setCoatingThickness",this);
  fSetCoatingThicknessCmd->
                   SetGuidance("Set thickness of the coating on the bars");
  fSetCoatingThicknessCmd->SetParameterName("thick",false);
  fSetCoatingThicknessCmd->SetUnitCategory("Length");
  fSetCoatingThicknessCmd->SetDefaultUnit("mm");
  fSetCoatingThicknessCmd->SetRange("thick>=0.");
  fSetCoatingThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetCoatingThicknessCmd->SetToBeBroadcasted(false);

  fSetCoatingRadiusCmd =
                new G4UIcmdWithADoubleAndUnit("/SciCRT/setCoatingRadius",this);
  fSetCoatingRadiusCmd->
                    SetGuidance("Set inner radius of the corner bar coating");
  fSetCoatingRadiusCmd->SetParameterName("cradius",false);
  fSetCoatingRadiusCmd->SetUnitCategory("Length");
  fSetCoatingRadiusCmd->SetDefaultUnit("mm");
  fSetCoatingRadiusCmd->SetRange("cradius>=0.");
  fSetCoatingRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSetCoatingRadiusCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTDetectorMessenger::~SciCRTDetectorMessenger()
{
  delete fDetDir;

  delete fSetPhotonDetGeometryCmd;
  delete fSetNumOfCladLayersCmd;
  delete fSetSciCRTLengthCmd;
  delete fSetSciCRTRadiusCmd;
  delete fSetClad1RadiusCmd;
  delete fSetClad2RadiusCmd;
  delete fSetPhotonDetHalfLengthCmd;
  delete fSetGapCmd;
  delete fSetPhotonDetAlignmentCmd;
  delete fSetSurfaceRoughnessCmd;
  delete fSetMirrorPolishCmd;
  delete fSetMirrorReflectivityCmd;
  delete fSetXYRatioCmd;
  delete fSetMirrorCmd;
  delete fSetBarLengthCmd;
  delete fSetBarBaseCmd;
  delete fSetHoleRadiusCmd;
  delete fSetCoatingThicknessCmd;
  delete fSetCoatingRadiusCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTDetectorMessenger::SetNewValue(G4UIcommand* command,G4String val)
{
  if( command == fSetPhotonDetGeometryCmd ) {
 
    fDetector->SetPhotonDetGeometry(val);
  }
  else if( command == fSetNumOfCladLayersCmd ) {

    fDetector->SetNumberOfCladding(G4UIcmdWithAnInteger::GetNewIntValue(val));
  }
  else if( command == fSetSurfaceRoughnessCmd ) {

    fDetector->SetSurfaceRoughness(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == fSetXYRatioCmd ) {
 
    fDetector->SetXYRatio(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == fSetMirrorPolishCmd ) {

    fDetector->SetMirrorPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == fSetMirrorReflectivityCmd ) {
 
    fDetector->
             SetMirrorReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == fSetPhotonDetPolishCmd ) {
 
    fDetector->SetPhotonDetPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == fSetPhotonDetReflectivityCmd ) {
 
    fDetector->
          SetPhotonDetReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if( command == fSetSciCRTLengthCmd ) {
 
    fDetector->SetSciCRTLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetSciCRTRadiusCmd ) {
 
    fDetector->SetSciCRTRadius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetClad1RadiusCmd ) {
 
    fDetector->
             SetClad1Radius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetClad2RadiusCmd ) {
 
    fDetector->
             SetClad2Radius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetPhotonDetHalfLengthCmd ) {
 
    fDetector->
     SetPhotonDetHalfLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetGapCmd ) {
 
   fDetector->SetGap(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetPhotonDetAlignmentCmd ) {
 
   fDetector->
      SetPhotonDetAlignment(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if( command == fSetMirrorCmd ) {

   fDetector->SetMirror(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == fSetBarLengthCmd ) {

   fDetector->SetBarLength(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == fSetBarBaseCmd ) {

   fDetector->SetBarBase(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == fSetHoleRadiusCmd ) {

   fDetector->SetHoleRadius(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == fSetCoatingThicknessCmd ) {

   fDetector->SetCoatingThickness(G4UIcmdWithABool::GetNewBoolValue(val));
  }
  else if( command == fSetCoatingRadiusCmd ) {

   fDetector->SetCoatingRadius(G4UIcmdWithABool::GetNewBoolValue(val));
  }
}
