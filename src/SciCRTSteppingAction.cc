/// \file optical/SciCRT/src/SciCRTSteppingAction.cc
/// \brief Implementation of the SciCRTSteppingAction class
//
//
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"

#include "SciCRTSteppingAction.hh"
#include "SciCRTDetectorConstruction.hh"
#include "SciCRTEventAction.hh"
#include "SciCRTSteppingActionMessenger.hh"
#include "SciCRTPhotonDetSD.hh"

#include "G4ParticleTypes.hh"
#include "SciCRTUserTrackInformation.hh"
#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"

#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>

// Purpose: salvar informacion usada en la traza

static const G4ThreeVector ZHat = G4ThreeVector(0.0,0.0,1.0);
G4int SciCRTSteppingAction::fMaxRndmSave = 10000;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTSteppingAction::SciCRTSteppingAction(SciCRTDetectorConstruction* detector, SciCRTEventAction* event)
  : fDetector(detector),fEventAction(event)
{
  fSteppingMessenger = new SciCRTSteppingActionMessenger(this);

  fCounterEnd = 0;
  fCounterMid = 0;
  fBounceLimit = 100000;
  fOpProcess = NULL;
  ResetCounters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTSteppingAction::~SciCRTSteppingAction()
{
  delete fSteppingMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  SciCRTSteppingAction::SetBounceLimit(G4int i)   {fBounceLimit = i;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTSteppingAction::GetNumberOfBounces()      {return fCounterBounce;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTSteppingAction::GetNumberOfClad1Bounces() {return fCounterClad1Bounce;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTSteppingAction::GetNumberOfClad2Bounces() {return fCounterClad2Bounce;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTSteppingAction::GetNumberOfSciCRTBounces()   {return fCounterSciCRTBounce;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SciCRTSteppingAction::ResetSuccessCounter()     {
      G4int temp = fCounterEnd; fCounterEnd = 0; return temp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SciCRTSteppingAction::saveRandomStatus(G4String subDir)
// save the random status into a sub-directory
// Pre: subDir must be empty or ended with "/"
{

    // don't save if the maximum amount has been reached
    if (SciCRTSteppingAction::fMaxRndmSave == 0) return;

    G4RunManager* theRunManager = G4RunManager::GetRunManager();
    G4String randomNumberStatusDir = theRunManager->GetRandomNumberStoreDir();

    G4String fileIn  = randomNumberStatusDir + "currentEvent.rndm";

    std::ostringstream os;

    os << "run" << theRunManager->GetCurrentRun()->GetRunID() << "evt"
       << theRunManager->GetCurrentEvent()->GetEventID() << ".rndm" << '\0';

    G4String fileOut = randomNumberStatusDir + subDir + os.str();

    G4String copCmd = "/control/shell cp "+fileIn+" "+fileOut;
    G4UImanager::GetUIpointer()->ApplyCommand(copCmd);

    SciCRTSteppingAction::fMaxRndmSave--;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTSteppingAction::UserSteppingAction(const G4Step* theStep)
{
  G4double edep=theStep->GetTotalEnergyDeposit();
  if (edep <= 0.) return;

  fEventAction->AddEdep(edep);

  G4Track* theTrack = theStep->GetTrack();
  SciCRTUserTrackInformation* trackInformation
      = (SciCRTUserTrackInformation*)theTrack->GetUserInformation();

  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

  G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4String thePrePVname  = " ";
  G4String thePostPVname = " ";

  if (thePostPV) {
     thePrePVname  = thePrePV->GetName();
     thePostPVname = thePostPV->GetName();
  }

  //Recording data for start
  if (theTrack->GetParentID()==0) {
     //This is a primary track
     if ( theTrack->GetCurrentStepNumber() == 1 ) {
//        G4double x  = theTrack->GetVertexPosition().x();
//        G4double y  = theTrack->GetVertexPosition().y();
//        G4double z  = theTrack->GetVertexPosition().z();
//        G4double pz = theTrack->GetVertexMomentumDirection().z();
//        G4double fInitTheta =
//                         theTrack->GetVertexMomentumDirection().angle(ZHat);
     }
  }

  // Retrieve the status of the photon
  G4OpBoundaryProcessStatus theStatus = Undefined;

  G4ProcessManager* OpManager =
                      G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (OpManager) {
     G4int MAXofPostStepLoops =
              OpManager->GetPostStepProcessVector()->entries();
     G4ProcessVector* fPostStepDoItVector =
              OpManager->GetPostStepProcessVector(typeDoIt);

     for ( G4int i=0; i<MAXofPostStepLoops; i++) {
         G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
         fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
         if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
     }
  }

  // Find the skewness of the ray at first change of boundary
  if ( fInitGamma == -1 &&
       (theStatus == TotalInternalReflection
        || theStatus == FresnelReflection
        || theStatus == FresnelRefraction)
        && trackInformation->isStatus(InsideOfFiber) ) {

        G4double px = theTrack->GetVertexMomentumDirection().x();
        G4double py = theTrack->GetVertexMomentumDirection().y();
        G4double x  = theTrack->GetPosition().x();
        G4double y  = theTrack->GetPosition().y();

        fInitGamma = x * px + y * py;

        fInitGamma =
                 fInitGamma / std::sqrt(px*px + py*py) / std::sqrt(x*x + y*y);

        fInitGamma = std::acos(fInitGamma*rad);

        if ( fInitGamma / deg > 90.0)  { fInitGamma = 180 * deg - fInitGamma;}
  }
  // Record Photons that missed the photon detector but escaped from readout
  if ( !thePostPV && trackInformation->isStatus(EscapedFromReadOut) ) {
     //UpdateHistogramSuccess(thePostPoint,theTrack);
     ResetCounters();

     return;
  }

  // Assumed photons are originated at the fiber OR
  // the fiber is the first material the photon hits
  switch (theStatus) {

     // Exiting the fiber
     case FresnelRefraction:
     case SameMaterial:

       G4bool isFiber;
       isFiber = thePostPVname == "SciCRTFiber"
                     || thePostPVname == "Clad1"
                     || thePostPVname == "Clad2";

       if ( isFiber ) {

           if (trackInformation->isStatus(OutsideOfFiber))
                               trackInformation->AddStatusFlag(InsideOfFiber);

       // Set the Exit flag when the photon refracted out of the fiber
       } else if (trackInformation->isStatus(InsideOfFiber)) {

           // EscapedFromReadOut if the z position is the same as fiber's end
           if (theTrack->GetPosition().z() == fDetector->GetSciCRTFiberEnd())
           {
              trackInformation->AddStatusFlag(EscapedFromReadOut);
              fCounterEnd++;
           }
           else // Escaped from side
           {
              trackInformation->AddStatusFlag(EscapedFromSide);
              trackInformation->SetExitPosition(theTrack->GetPosition());

//              UpdateHistogramEscape(thePostPoint,theTrack);

              fCounterMid++;
              ResetCounters();
           }

           trackInformation->AddStatusFlag(OutsideOfFiber);
           trackInformation->SetExitPosition(theTrack->GetPosition());

       }

       return;

     // Internal Reflections
     case TotalInternalReflection:

       // Kill the track if it's number of bounces exceeded the limit
       if (fBounceLimit > 0 && fCounterBounce >= fBounceLimit)
       {
          theTrack->SetTrackStatus(fStopAndKill);
          trackInformation->AddStatusFlag(murderee);
          ResetCounters();
          G4cout << "\n Bounce Limit Exceeded" << G4endl;
          return;
       }

     case FresnelReflection:

       fCounterBounce++;

       if ( thePrePVname == "SciCRTFiber") fCounterSciCRTBounce++;

       else if ( thePrePVname == "Clad1") fCounterClad1Bounce++;

       else if ( thePrePVname == "Clad2") fCounterClad2Bounce++;

       // Determine if the photon has reflected off the read-out end
       if (theTrack->GetPosition().z() == fDetector->GetSciCRTFiberEnd())
       {
          if (!trackInformation->isStatus(ReflectedAtReadOut) &&
              trackInformation->isStatus(InsideOfFiber))
          {
             trackInformation->AddStatusFlag(ReflectedAtReadOut);

             if (fDetector->IsPerfectFiber() &&
                 theStatus == TotalInternalReflection)
             {
                theTrack->SetTrackStatus(fStopAndKill);
                trackInformation->AddStatusFlag(murderee);
//                UpdateHistogramReflect(thePostPoint,theTrack);
                     ResetCounters();
                return;
             }
          }
       }
       return;

     // Reflection of the mirror
     case LambertianReflection:
     case LobeReflection:
     case SpikeReflection:

       // Check if it hits the mirror
       if ( thePostPVname == "Mirror" )
          trackInformation->AddStatusFlag(ReflectedAtMirror);

       return;

     // Detected by a detector
     case Detection:

       // Check if the photon hits the detector and process the hit if it does
       if ( thePostPVname == "PhotonDet" ) {

          G4SDManager* SDman = G4SDManager::GetSDMpointer();
          G4String SDname="SciCRT/PhotonDet";
          SciCRTPhotonDetSD* mppcSD =
                        (SciCRTPhotonDetSD*)SDman->FindSensitiveDetector(SDname);

          if (mppcSD) mppcSD->ProcessHits_constStep(theStep,NULL);

          // Record Photons that escaped at the end
          if (trackInformation->isStatus(EscapedFromReadOut))
          //                    UpdateHistogramSuccess(thePostPoint,theTrack);

          // Stop Tracking when it hits the detector's surface
          ResetCounters();
          theTrack->SetTrackStatus(fStopAndKill);

          return;
       }

       break;

     default: break;

  }

  // Check for absorbed photons
  if (theTrack->GetTrackStatus() != fAlive  &&
      trackInformation->isStatus(InsideOfFiber))
  {
//     UpdateHistogramAbsorb(thePostPoint,theTrack);
     ResetCounters();
     return;
  }

}
