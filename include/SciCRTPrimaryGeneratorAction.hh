//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTPrimaryGeneratorAction_h
#define SciCRTPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4AffineTransform.hh"

class G4GeneralParticleSource;

class G4Event;
class G4PhysicsTable;

class SciCRTDetectorConstruction;
class SciCRTPrimaryGeneratorMessenger;

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
    G4GeneralParticleSource*   fParticleGun;
    SciCRTPrimaryGeneratorMessenger* fGunMessenger;

    static G4bool fFirst;

    G4double fTimeConstant;

};

#endif
