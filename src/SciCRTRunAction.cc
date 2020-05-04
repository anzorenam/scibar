#include "SciCRTRunAction.hh"
#include "SciCRTRunActionMessenger.hh"

#include "SciCRTAnalysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"

#include "SciCRTDetectorConstruction.hh"
#include "SciCRTSteppingAction.hh"

#include <ctime>

SciCRTRunAction::SciCRTRunAction()
  : fSaveRndm(0), fAutoSeed(false)
{
  fRunMessenger = new SciCRTRunActionMessenger(this);

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstNtupleId(1);
  analysisManager->CreateNtuple("edep","photons");
  analysisManager->CreateNtupleIColumn("evento");
  analysisManager->CreateNtupleDColumn("energy");
  analysisManager->CreateNtupleIColumn("num_pmt");
  analysisManager->CreateNtupleIColumn("num_all");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("time", "time");
  analysisManager->CreateNtupleIColumn("evento");
  analysisManager->CreateNtupleIColumn("ID");
  analysisManager->CreateNtupleDColumn("Time");
  analysisManager->FinishNtuple();

}


SciCRTRunAction::~SciCRTRunAction()
{
  delete fRunMessenger;
  delete G4AnalysisManager::Instance();
}

void SciCRTRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  char* long_ID=getenv("LONGZ");

  if (fAutoSeed) {
     G4cout << "*******************" << G4endl;
     G4cout << "*** AUTOSEED ON ***" << G4endl;
     G4cout << "*******************" << G4endl;
     long seeds[2];
     time_t systime = time(NULL);
     seeds[0] = (long) systime;
     seeds[1] = (long) (systime*G4UniformRand());
     G4Random::setTheSeeds(seeds);
     G4Random::showEngineStatus();
  } else {
     G4Random::showEngineStatus();
  }

  if (fSaveRndm > 0) G4Random::saveEngineStatus("BeginOfRun.rndm");

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4String lID;
  lID=G4String(long_ID);
  G4String fileName = "scibar"+lID;
  analysisManager->OpenFile(fileName);
}

void SciCRTRunAction::EndOfRunAction(const G4Run* )
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  if (fSaveRndm == 1)
  {
     G4Random::showEngineStatus();
     G4Random::saveEngineStatus("endOfRun.rndm");
  }
  analysisManager->Write();
  analysisManager->CloseFile();

}
