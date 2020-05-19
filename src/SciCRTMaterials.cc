#include "SciCRTMaterials.hh"
#include "G4SystemOfUnits.hh"
SciCRTMaterials* SciCRTMaterials::fInstance = 0;

SciCRTMaterials::SciCRTMaterials()
{
  fNistMan=G4NistManager::Instance();
  fNistMan->SetVerbose(2);
  CreateMaterials();
}

SciCRTMaterials::~SciCRTMaterials()
{
  delete fPMMA;
  delete fPethylene;
  delete fFPethylene;
  delete fPolystyrene;
  delete fSilicone;
}

SciCRTMaterials* SciCRTMaterials::GetInstance()
{
  if(fInstance==0)
    {
      fInstance=new SciCRTMaterials();
    }
  return fInstance;
}

G4Material* SciCRTMaterials::GetMaterial(const G4String material)
{
  G4Material* mat=fNistMan->FindOrBuildMaterial(material);

  if (!mat) mat=G4Material::GetMaterial(material);
  if (!mat) {
     std::ostringstream o;
     o << "Material " << material << " not found!";
     G4Exception("SciCRTMaterials::GetMaterial","",
                 FatalException,o.str().c_str());
  }
  return mat;
}

void SciCRTMaterials::CreateMaterials()
{
  G4double density;
  G4double a,nz;
  G4int ncomponents;
  G4double fractionmass;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;
  G4double z;  // atomic number

  //***Elements
  G4Element* fC;
  G4Element* fH;
  fH=new G4Element("H","H",z=1.,a=1.01*g/mole);
  fC=new G4Element("C","C",z=6.,a=12.01*g/mole);

  // Materials Definitions
  // =====================

  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Galactic");

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  fAir=fNistMan->FindOrBuildMaterial("G4_AIR");

  //--------------------------------------------------
  // Glass
  //--------------------------------------------------

  fGlass=new G4Material("Glass",density=1.032*g/cm3,2);
  fGlass->AddElement(fC,91.533*perCent);
  fGlass->AddElement(fH,8.467*perCent);

  //--------------------------------------------------
  // SciCRTfiber PMMA
  //--------------------------------------------------

  elements.push_back("C"); natoms.push_back(5);
  elements.push_back("H"); natoms.push_back(8);
  elements.push_back("O"); natoms.push_back(2);
  density=1.190*g/cm3;
  fPMMA=fNistMan->ConstructNewMaterial("PMMA",elements,natoms,density);
  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Cladding (polyethylene)
  //--------------------------------------------------

  elements.push_back("C"); natoms.push_back(8);
  elements.push_back("H"); natoms.push_back(8);
  //density de  1.200*g/cm3 a 1.05;
  density=1.05*g/cm3;
  fPethylene=fNistMan->ConstructNewMaterial("Pethylene",elements,natoms,density);
  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Double Cladding (fluorinated polyethylene)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(4);
  density=1.430*g/cm3;
  fFPethylene=fNistMan->ConstructNewMaterial("FPethylene",elements,natoms,density);
  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Polystyrene
  //--------------------------------------------------

  elements.push_back("C"); natoms.push_back(8);
  elements.push_back("H"); natoms.push_back(8);
  //density = 1.050*g/cm3;
  //de medina-tanco
  density=1.080*g/cm3;
  fPolystyrene=fNistMan->ConstructNewMaterial("Polystyrene",elements,natoms,density);
  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Silicone (Template for Optical Grease)
  //--------------------------------------------------

  elements.push_back("C"); natoms.push_back(2);
  elements.push_back("H"); natoms.push_back(6);
  density=1.060*g/cm3;
  fSilicone=fNistMan->ConstructNewMaterial("Silicone",elements,natoms,density);
  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Aluminium
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Al");
  fAl = new G4Material("Al",nz=13.,a=26.98*g/mole,density=2.7*g/cm3);

  //--------------------------------------------------
  // TiO2
  //--------------------------------------------------

  elements.push_back("Ti"); natoms.push_back(1);
  elements.push_back("O"); natoms.push_back(2);
  density=4.26*g/cm3;
  G4Material* TiO2=fNistMan->ConstructNewMaterial("TiO2",elements,natoms,density);
  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  //--------------------------------------------------

  density=1.52*g/cm3;
  fCoating=new G4Material("Coating",density,ncomponents=2);
  fCoating->AddMaterial(TiO2,fractionmass=15*perCent);
  fCoating->AddMaterial(fPolystyrene,fractionmass=85*perCent);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //

  const G4int Ne=400;
  G4double pEn[Ne],rIndA[Ne],rIndFb[Ne],rIndC1[Ne],rIndC2[Ne];
  G4double emiPS[Ne],emiFb[Ne],absFb[Ne],absWLS[Ne],rIndG[Ne];
  G4double absGl[Ne],rIndSi[Ne],absSi[Ne],rIndPS[Ne],absPS[Ne];

  G4String filename="optical/scibar_optics0.csv";
  std::ifstream data_file;
  data_file.open(filename);
  if(!data_file) {
    G4cout << "File open error "<< G4endl;
  }
  else{
    for(int k=0;k<15;k++){
      for(int j=0;j<Ne;j++){
          if(k==0){
            data_file>>pEn[j];
          } else if(k==1){
            data_file>>rIndFb[j];
          } else if(k==2){
            data_file>>rIndC1[j];
          } else if(k==3){
            data_file>>rIndC2[j];
          } else if(k==4){
            data_file>>emiPS[j];
          } else if(k==5){
            data_file>>emiFb[j];
          } else if(k==6){
            data_file>>absFb[j];
          } else if(k==7){
            data_file>>absWLS[j];
          } else if(k==8){
            rIndA[j]=1.0;
          } else if(k==9){
            rIndG[j]=1.49;
          } else if(k==10){
            absGl[j]=420.*cm;
          } else if(k==11){
            rIndSi[j]=1.46;
          } else if(k==12){
            absSi[j]=20.0*m;
          } else if(k==13){
            rIndPS[j]=1.58;
          } else if(k==14){
            absPS[j]=250.*cm;
          }
      }
    }
  }
  data_file.close();
  for(int j=0;j<Ne;j++){
    pEn[j]*=eV;
    absFb[j]*=m;
    absWLS[j]*=mm;
  }

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  G4MaterialPropertiesTable* mpt=new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX",pEn,rIndA,Ne);
  fAir->SetMaterialPropertiesTable(mpt);

  //--------------------------------------------------
  //  Polyethylene(nucleo)
  //--------------------------------------------------

  G4MaterialPropertiesTable* mptSciCRTfiber=new G4MaterialPropertiesTable();
  mptSciCRTfiber->AddProperty("RINDEX",pEn,rIndFb,Ne);
  mptSciCRTfiber->AddProperty("ABSLENGTH",pEn,absFb,Ne);
  mptSciCRTfiber->AddProperty("WLSABSLENGTH",pEn,absWLS,Ne);
  mptSciCRTfiber->AddProperty("WLSCOMPONENT",pEn,emiFb,Ne);
  //PENDIENTE de 0.5ns a 10ns por knoll y Super-IFR ...
  mptSciCRTfiber->AddConstProperty("WLSTIMECONSTANT", 10.*ns);
  fPethylene->SetMaterialPropertiesTable(mptSciCRTfiber);

  //--------------------------------------------------
  //  PMMA primer revestimiento-interno
  //--------------------------------------------------

  G4MaterialPropertiesTable* mptClad1=new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX",pEn,rIndC1,Ne);
  mptClad1->AddProperty("ABSLENGTH",pEn,absFb,Ne); // falta
  fPMMA->SetMaterialPropertiesTable(mptClad1);

  //--------------------------------------------------
  // Fluorinated Polyethylene segundo revestimeinto-externo
  //--------------------------------------------------

  G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX",pEn,rIndC2,Ne);
  mptClad2->AddProperty("ABSLENGTH",pEn,absFb,Ne);
  fFPethylene->SetMaterialPropertiesTable(mptClad2);

  //--------------------------------------------------
  // Glass
  //este material lo saque del proyecto LXe
  //--------------------------------------------------
  //Por lo tanto los parametros son los recomendados

  G4MaterialPropertiesTable *glass_mt = new G4MaterialPropertiesTable();
  glass_mt->AddProperty("ABSLENGTH",pEn,absGl,Ne);
  glass_mt->AddProperty("RINDEX",pEn,rIndG,Ne);
  fGlass->SetMaterialPropertiesTable(glass_mt);

  //--------------------------------------------------
  // Silicone
  //--------------------------------------------------

  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->AddProperty("RINDEX",pEn,rIndSi,Ne);
  mptSilicone->AddProperty("ABSLENGTH",pEn,absSi,Ne);
  fSilicone->SetMaterialPropertiesTable(mptSilicone);

  //--------------------------------------------------
  //  Polystyrene, plástico centellador
  //--------------------------------------------------

  G4MaterialPropertiesTable* mptPolystyrene=new G4MaterialPropertiesTable();
  mptPolystyrene->AddProperty("RINDEX",pEn,rIndPS,Ne);
  mptPolystyrene->AddProperty("ABSLENGTH",pEn,absPS,Ne);
  mptPolystyrene->AddProperty("FASTCOMPONENT",pEn,emiPS,Ne);
  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD",500./MeV);
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
  mptPolystyrene->AddConstProperty("FASTTIMECONSTANT",3.6*ns);
  fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);
  fPolystyrene->GetIonisation()->SetBirksConstant(0.208*mm/MeV);

}
