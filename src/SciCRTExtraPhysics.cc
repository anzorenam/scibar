/// \file optical/SciCRT/src/SciCRTExtraPhysics.cc
/// \brief Implementation of the SciCRTExtraPhysics class
//
//
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4UserSpecialCuts.hh"
#include "G4StepLimiter.hh"
#include "SciCRTExtraPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTExtraPhysics::SciCRTExtraPhysics()
    : G4VPhysicsConstructor("Extra") { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SciCRTExtraPhysics::~SciCRTExtraPhysics() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTExtraPhysics::ConstructParticle() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SciCRTExtraPhysics::ConstructProcess()
{
    G4cout << "SciCRTExtraPhysics:: Add Extra Physics Processes"
              << G4endl;

    aParticleIterator->reset();

    while ((*aParticleIterator)()) {
        G4ParticleDefinition* particle = aParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        G4double charge = particle->GetPDGCharge();

        if (!pmanager) {
            std::ostringstream o;
            o << "Particle " << particleName << "without a Process Manager";
            G4Exception("SciCRTExtraPhysics::ConstructProcess()","",
                         FatalException,o.str().c_str());
        }

        if (particleName == "opticalphoton") break;

        if (charge != 0.0) {
           // All charged particles should have a step limiter
           // to make sure that the steps do not get too long.
           pmanager->AddDiscreteProcess(new G4StepLimiter());
           pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
        } else if (particleName == "neutron") {
          // time cuts for ONLY neutrons:
          pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
        } else {
          // Energy cuts for all other neutral particles
          pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
        }
    }
}
