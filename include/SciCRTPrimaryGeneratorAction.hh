//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPrimaryGeneratorAction_h
#define SciCRTPrimaryGeneratorAction_h 1

#include <fstream>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4AffineTransform.hh"

class G4ParticleGun;

class G4Event;
class G4PhysicsTable;

class SciCRTDetectorConstruction;

class SciCRTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    SciCRTPrimaryGeneratorAction(SciCRTDetectorConstruction*);
    virtual ~SciCRTPrimaryGeneratorAction();

  public:

    virtual void GeneratePrimaries(G4Event*);

    void BuildEmissionSpectrum();

    void SetOptPhotonPolar(G4double);

    void SetDecayTimeConstant(G4double);

  protected:

    G4PhysicsTable* fIntegralTable;

  private:

    void SetOptPhotonPolar();
    void SetOptPhotonTime();

    SciCRTDetectorConstruction*   fDetector;
    G4ParticleGun*   fParticleGun;

    static G4bool fFirst;

    G4double fTimeConstant;
    G4String fileName;
    std::ifstream inFile;

};

#endif
