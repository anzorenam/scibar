//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPhysicsList_h
#define SciCRTPhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class G4VPhysicsConstructor;
class SciCRTPhysicsListMessenger;

class SciCRTStepMax;
class SciCRTOpticalPhysics;

class SciCRTPhysicsList: public G4VModularPhysicsList
{
  public:

    SciCRTPhysicsList(G4String);
    virtual ~SciCRTPhysicsList();

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

    void SetStepMax(G4double);
    SciCRTStepMax* GetStepMaxProcess();
    void AddStepMax();

    /// Remove specific physics from physics list.
    void RemoveFromPhysicsList(const G4String&);

    /// Make sure that the physics list is empty.
    void ClearPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // Turn on or off the absorption process
    void SetAbsorption(G4bool);

    void SetNbOfPhotonsCerenkov(G4int);

    void SetVerbose(G4int);

private:

    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;

    SciCRTStepMax* fStepMaxProcess;

    SciCRTOpticalPhysics* fOpticalPhysics;

    SciCRTPhysicsListMessenger* fMessenger;

    G4bool fAbsorptionOn;
    
    G4VMPLData::G4PhysConstVectorData* fPhysicsVector;

};

#endif
