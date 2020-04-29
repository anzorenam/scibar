//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef SciCRTMaterials_h
#define SciCRTMaterials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

class SciCRTMaterials
{
  public:

    virtual ~SciCRTMaterials();
 
    static SciCRTMaterials* GetInstance();

    G4Material* GetMaterial(const G4String);
 
  private:
 
    SciCRTMaterials();

    void CreateMaterials();

  private:

    static SciCRTMaterials* fInstance;

    G4NistManager*     fNistMan;

    G4Material*        fAir;
    G4Material*        fAl;
    G4Material*        fGlass;

    G4Material*        fPMMA;
    G4Material*        fPethylene;
    G4Material*        fFPethylene;
    G4Material*        fPolystyrene;
    G4Material*        fSilicone;
    G4Material*        fCoating;

};

#endif /*SciCRTMaterials_h*/
