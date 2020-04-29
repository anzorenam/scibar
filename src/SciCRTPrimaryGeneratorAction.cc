#include "G4ios.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicsTable.hh"
#include "Randomize.hh"
#include "G4Threading.hh"

#include "SciCRTPrimaryGeneratorAction.hh"
#include "SciCRTDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoLock.hh"

namespace {
  G4Mutex gen_mutex = G4MUTEX_INITIALIZER;
}

G4bool SciCRTPrimaryGeneratorAction::fFirst = false;
SciCRTPrimaryGeneratorAction::
                         SciCRTPrimaryGeneratorAction(SciCRTDetectorConstruction* dc)
{
  fDetector = dc;
  fIntegralTable = NULL;

  char* long_ID=getenv("LONGZ");
  G4double longz=(float) atoi(G4String(long_ID));

  fParticleGun = new G4ParticleGun(1);
  G4ParticleTable* particleTable=G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle=particleTable->FindParticle("opticalphoton");
  G4ThreeVector momentumDir=G4ThreeVector(0,0,1);
  G4ThreeVector par_position=G4ThreeVector(8.75*cm,-0.65*cm,longz*cm);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(2.91*eV);
  fParticleGun->SetParticlePosition(par_position);
  fParticleGun->SetParticleMomentumDirection(momentumDir);

  fTimeConstant = 0.;
}

SciCRTPrimaryGeneratorAction::~SciCRTPrimaryGeneratorAction()
{
  delete fParticleGun;
  if (fIntegralTable) {
     fIntegralTable->clearAndDestroy();
     delete fIntegralTable;
  }
}

void SciCRTPrimaryGeneratorAction::SetDecayTimeConstant(G4double time)
{
  fTimeConstant = time;
}

void SciCRTPrimaryGeneratorAction::BuildEmissionSpectrum()
{
   if (fIntegralTable) return;
   const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
   G4int numOfMaterials = G4Material::GetNumberOfMaterials();
   if(!fIntegralTable)fIntegralTable = new G4PhysicsTable(numOfMaterials);
   for (G4int i=0 ; i < numOfMaterials; i++) {
       G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector =
                                           new G4PhysicsOrderedFreeVector();
       G4Material* aMaterial = (*theMaterialTable)[i];
       G4MaterialPropertiesTable* aMaterialPropertiesTable =
                                    aMaterial->GetMaterialPropertiesTable();
       if (aMaterialPropertiesTable) {
          G4MaterialPropertyVector* theSciCRTVector =
                      aMaterialPropertiesTable->GetProperty("SciCRTCOMPONENT");
          if (theSciCRTVector) {
             G4double currentIN = (*theSciCRTVector)[0];
             if (currentIN >= 0.0) {
                G4double currentPM = theSciCRTVector->Energy(0);
                G4double currentCII = 0.0;
                aPhysicsOrderedFreeVector->
                                     InsertValues(currentPM , currentCII);
                G4double prevPM  = currentPM;
                G4double prevCII = currentCII;
                G4double prevIN  = currentIN;
                for (size_t j = 1;
                     j < theSciCRTVector->GetVectorLength();
                     j++)
                {
                  currentPM = theSciCRTVector->Energy(j);
                  currentIN = (*theSciCRTVector)[j];
                  currentCII = 0.5 * (prevIN + currentIN);
                  currentCII = prevCII + (currentPM - prevPM) * currentCII;
                  aPhysicsOrderedFreeVector->
                                     InsertValues(currentPM, currentCII);
                  prevPM  = currentPM;
                  prevCII = currentCII;
                  prevIN  = currentIN;
                }
             }
          }
       }
       fIntegralTable->insertAt(i,aPhysicsOrderedFreeVector);
   }
}

void SciCRTPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (!fFirst) {
     fFirst = true;
     BuildEmissionSpectrum();
  }

#ifdef use_sampledEnergy
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
  G4double sampledEnergy = 3*eV;
  for (size_t j=0 ; j<theMaterialTable->size() ; j++) {
      G4Material* fMaterial = (*theMaterialTable)[j];
      if (fMaterial->GetName() == "PMMA" ) {
         G4MaterialPropertiesTable* aMaterialPropertiesTable =
                                      fMaterial->GetMaterialPropertiesTable();
         const G4MaterialPropertyVector* SciCRTIntensity =
                   aMaterialPropertiesTable->GetProperty("SciCRTCOMPONENT");
         if (SciCRTIntensity) {
            G4int MaterialIndex = fMaterial->GetIndex();
            G4PhysicsOrderedFreeVector* SciCRTIntegral =
              (G4PhysicsOrderedFreeVector*)((*fIntegralTable)(MaterialIndex));
            G4double CIImax = SciCRTIntegral->GetMaxValue();
            G4double CIIvalue = G4UniformRand()*CIImax;
            sampledEnergy = SciCRTIntegral->GetEnergy(CIIvalue);
         }
      }
  }
#endif
  G4AutoLock l(&gen_mutex);
  if(fParticleGun->GetParticleDefinition()->GetParticleName()=="opticalphoton"){
    SetOptPhotonPolar();
    SetOptPhotonTime();
  }
  char* long_ID=getenv("LONGZ");
  G4double longz=(float) atoi(G4String(long_ID));

  G4double z=2.0*G4UniformRand()-1.0;
  G4double randz;
  if(z>=0){
    randz=1.0;
  }else if(z<0){
    randz=-1.0;
  }

  G4ParticleTable* particleTable=G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle=particleTable->FindParticle("opticalphoton");
  G4ThreeVector momentumDir=G4ThreeVector(0,0,randz);
  G4ThreeVector par_position=G4ThreeVector(8.75*cm,-0.65*cm,longz*cm);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(2.91*eV);
  fParticleGun->SetParticlePosition(par_position);
  fParticleGun->SetParticleMomentumDirection(momentumDir);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

void SciCRTPrimaryGeneratorAction::SetOptPhotonPolar()
{
  G4double angle = G4UniformRand() * 360.0*deg;
  SetOptPhotonPolar(angle);
}

void SciCRTPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
  if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
  {
     G4cout << "-> warning from SciCRTPrimaryGeneratorAction::SetOptPhotonPolar()"
            << ":  the ParticleGun is not an opticalphoton" << G4endl;
     return;
  }
  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2       = product*product;
  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
  fParticleGun->SetParticlePolarization(polar);

}

void SciCRTPrimaryGeneratorAction::SetOptPhotonTime()
{
   G4double time = -std::log(G4UniformRand())*fTimeConstant;
   fParticleGun->SetParticleTime(time);
}
