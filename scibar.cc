#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "SciCRTPhysicsList.hh"
#include "SciCRTDetectorConstruction.hh"
#include "SciCRTActionInitialization.hh"

int main(int argc,char** argv)
{
  G4int seed;
  if (argc >2) seed=atoi(argv[argc-1]);
  auto *randomEngine = new CLHEP::RanecuEngine();
  G4Random::setTheEngine(randomEngine);
  G4Random::setTheSeed(seed);
  G4MTRunManager *runManager=new G4MTRunManager;

  G4int nThreads=G4Threading::G4GetNumberOfCores();
  runManager->SetNumberOfThreads(nThreads);
  G4String physName="QGSP_BERT_HP";
  SciCRTDetectorConstruction* detector=new SciCRTDetectorConstruction();
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new SciCRTPhysicsList(physName));
  runManager->SetUserInitialization(new SciCRTActionInitialization(detector));
  runManager->Initialize();
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  G4int optmax = argc;
  if (argc > 2) {optmax=optmax-1;}
  if (optind < optmax){
    G4String command = "/control/execute ";
    for ( ; optind < optmax; optind++){
      G4String macroFilename = argv[optind];
      UImanager->ApplyCommand(command+macroFilename);
     }
  }

  delete runManager;
  return 0;
}
