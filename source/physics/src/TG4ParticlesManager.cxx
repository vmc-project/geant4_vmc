// $Id: TG4ParticlesManager.cxx,v 1.7 2006/09/13 06:26:12 brun Exp $
// Category: physics
//
// Class TG4ParticlesManager
// -------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4ParticlesManager.h"
#include "TG4G3Units.h"

#include <G4ParticleDefinition.hh>
#include <G4DynamicParticle.hh>
#include <G4ParticleTable.hh>

#include <TDatabasePDG.h>
#include <TParticle.h>
#include <TClonesArray.h>

TG4ParticlesManager* TG4ParticlesManager::fgInstance = 0;

//_____________________________________________________________________________
TG4ParticlesManager::TG4ParticlesManager()
  : TG4Verbose("particlesManager"),
    fParticleNameMap(),
    fParticlePDGMap()
    
{ 
//
  if (fgInstance) {
    TG4Globals::Exception(
      "TG4ParticlesManager", "TG4ParticlesManager",
      "Cannot create two instances of singleton.");
  }
      
  fgInstance = this;  
}

//_____________________________________________________________________________
TG4ParticlesManager::~TG4ParticlesManager() {
//
}

//
// private methods
//

//_____________________________________________________________________________
G4int TG4ParticlesManager::GetPDGEncoding(G4ParticleDefinition* particle) const
{
/// Return the PDG code of particle;
/// if standard PDG code is not defined the TDatabasePDG
/// is used.

  // get PDG encoding from G4 particle definition
  G4int pdgEncoding = particle->GetPDGEncoding();

  if (pdgEncoding == 0) {
    // get PDG encoding from TDatabasePDG
  
    // get particle name from the name map
    G4String g4name = particle->GetParticleName();
    G4String tname = fParticleNameMap.GetSecond(g4name);
    if (tname == "Undefined") {
      TG4Globals::Exception(
        "TG4ParticlesManager", "GetPDGEncoding",
        "Particle " + TString(g4name) + " was not found in the name map.");
    }  
  
    // get particle from TDatabasePDG
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();
    TParticlePDG* particle = pdgDB->GetParticle(tname);
    if (!particle) {
      TG4Globals::Exception(
        "TG4ParticlesManager", "GetPDGEncoding",
        "Particle " +  TString(tname) + " was not found in TDatabasePDG.");
    }  
    
    // get PDG encoding
    pdgEncoding = particle->PdgCode();
  }
    
  return pdgEncoding;  
}  
     
//_____________________________________________________________________________
G4int TG4ParticlesManager::GetPDGEncoding(G4String particleName) const
{
/// Return the PDG code of particle sepcified by name.                      \n
/// If particle was not built in Geant4 physics, return -1 

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition* particle = 0;  
  particle = particleTable->FindParticle(particleName);
  if (!particle) return -1;

  return GetPDGEncoding(particle);
}  
  
//_____________________________________________________________________________
G4int TG4ParticlesManager::GetPDGIonEncoding(G4int Z, G4int A, G4int iso) const
{
/// Use ENDF-6 mapping for ions = 10000*z+10*a+iso
/// and add 10 000 000.

  return 10000000 + 10000*Z + 10*A + iso;
}  
  
//_____________________________________________________________________________
void  TG4ParticlesManager::AddParticlesToPdgDatabase() const
{
/// Taken from TGeant3
///
/// Use ENDF-6 mapping for ions = 10000*z+10*a+iso
/// and add 1 000 000
/// and numbers above 5 000 000 for special applications

  const Int_t kion=10000000;
  const Int_t kspe=50000000;

  const Double_t kGeV=0.9314943228;
  const Double_t kHslash = 1.0545726663e-27;
  const Double_t kErgGeV = 1/1.6021773349e-3;
  const Double_t kHshGeV = kHslash*kErgGeV;
  const Double_t kYearsToSec = 3600*24*365.25;

  TDatabasePDG *pdgDB = TDatabasePDG::Instance();

  pdgDB->AddParticle("Deuteron","Deuteron",2*kGeV+8.071e-3,kTRUE,
                     0,3,"Ion",kion+10020);
                     
  pdgDB->AddParticle("Triton","Triton",3*kGeV+14.931e-3,kFALSE,
                     kHshGeV/(12.33*kYearsToSec),3,"Ion",kion+10030);

  pdgDB->AddParticle("Alpha","Alpha",4*kGeV+2.424e-3,kTRUE,
                     kHshGeV/(12.33*kYearsToSec),6,"Ion",kion+20040);

  pdgDB->AddParticle("HE3","HE3",3*kGeV+14.931e-3,kFALSE,
                     0,6,"Ion",kion+20030);

  pdgDB->AddParticle("Cherenkov","Cherenkov",0,kFALSE,
                     0,0,"Special",kspe+50);

  pdgDB->AddParticle("FeedbackPhoton","FeedbackPhoton",0,kFALSE,
                     0,0,"Special",kspe+51);
}                     

//_____________________________________________________________________________
void  TG4ParticlesManager::MapParticles()
{
/// Map G4 particle names to TDatabasePDG names
/// (the map is built only for particles that have not
///  defined standard PDG encoding)
  
  fParticleNameMap.Add("deuteron","Deuteron");
  fParticleNameMap.Add("triton",  "Triton");
  fParticleNameMap.Add("alpha",   "Alpha");
  fParticleNameMap.Add("He3",     "HE3");
  fParticleNameMap.Add("opticalphoton","Cherenkov");
  // fParticleNameMap.Add("???","FeedbackPhoton");
  fParticleNameMap.Add("geantino", "Rootino");
  fParticleNameMap.Add("chargedgeantino", "Rootino");
  
  // map G4 particle names to TDatabasePDG encodings
  // (if present in built Geant4 physics)
  
  G4int pdg = GetPDGEncoding("deuteron");
  if (pdg > 0 ) fParticlePDGMap.Add("deuteron", pdg);
  
  pdg = GetPDGEncoding("triton");
  if (pdg > 0 ) fParticlePDGMap.Add("triton", pdg);
  
  pdg = GetPDGEncoding("alpha");
  if (pdg > 0 ) fParticlePDGMap.Add("alpha", pdg);
  
  pdg = GetPDGEncoding("He3");
  if (pdg > 0 ) fParticlePDGMap.Add("He3", pdg);

  pdg = GetPDGEncoding("opticalphoton");
  if (pdg > 0 ) fParticlePDGMap.Add("opticalphoton", pdg);
 
  // fParticlePDGMap.Add("???","FeedbackPhoton");
  
  pdg = GetPDGEncoding("geantino");
  if (pdg > 0 ) fParticlePDGMap.Add("geantino", pdg);
  
  pdg = GetPDGEncoding("chargedgeantino");
  if (pdg > 0 ) fParticlePDGMap.Add("chargedgeantino", pdg);

  if (VerboseLevel() > 1) {
    G4cout << "Particle maps have been filled." << G4endl;
  }  
  if (VerboseLevel() > 1) {
    fParticleNameMap.PrintAll();
    fParticlePDGMap.PrintAll();
  }  
}    

//_____________________________________________________________________________
G4int TG4ParticlesManager::AddIonToPdgDatabase(
                              const G4String& name,
                              G4ParticleDefinition* particleDefinition,
                              G4int Q)
{
/// Add the ion in TDatabasePDG and the maps
/// and return its generated PDG code.

  // Check if ion
  if (particleDefinition->GetPDGEncoding() != 0 || 
      particleDefinition->GetParticleType() != "nucleus") {
    
      TG4Globals::Exception(
        "TG4ParticlesManager", "AddIonToPdgDatabase", 
        "Added particle is not ion."); 
  }    

  TParticlePDG* particlePDG 
    = TDatabasePDG::Instance()->GetParticle(name);

  if (particlePDG) {
     // Ion was already added
     return particlePDG->PdgCode();     
  }
  else {
    // Define PDG encoding
    G4int a = particleDefinition->GetBaryonNumber();
    G4int z = G4int(particleDefinition->GetPDGCharge()/eplus);
    G4int pdg = GetPDGIonEncoding(z, a, 0);
    
    // Find isomer number which is not yet used
    while (TDatabasePDG::Instance()->GetParticle(pdg) &&
           pdg < GetPDGIonEncoding(z, a+1, 0)-1 ) 
      pdg++;

    if (TDatabasePDG::Instance()->GetParticle(pdg)) {
      //G4String text = "TG4ParticlesManager::AddIonToPdgDatabase: \n";
      //text = text + "    All isomer numbers are already used."; 
      // TG4Globals::Exception(text);
      return pdg-1;
    }
            
    // Define unique ion name
    G4String uniqueIonName 
      = UniqueIonName(particleDefinition->GetParticleName(), Q);

    if (VerboseLevel() > 1) {
       G4cout << "Adding ion to TDatabasePDG " << G4endl;
       G4cout << "   Unique ion name: " << uniqueIonName << G4endl;
       G4cout << "   PDG:             " << pdg << G4endl;
    }               

    // Add ion to TDatabasePDG
    TDatabasePDG::Instance()
      ->AddParticle(name, uniqueIonName, 
                    particleDefinition->GetPDGMass()/TG4G3Units::Energy(), 
                    particleDefinition->GetPDGStable(), 
                    particleDefinition->GetPDGWidth()/TG4G3Units::Energy(), 
                    Q*3, "Ion", pdg);                     
                    
    // Add ion to PDG map
    fParticlePDGMap.Add(uniqueIonName, pdg);  

    // Add ion to name map
    // if name in PDG is different from unique name
    if (name != uniqueIonName)
       fParticleNameMap.Add(uniqueIonName, name); 
    return pdg;    
  }                
}


//_____________________________________________________________________________
G4String TG4ParticlesManager::UniqueIonName(const G4String& g4IonName, 
                                            G4int Q) const
{
/// Construct unique name by adding [Q] to G4 ion name.                      \n
/// Eg. Al[0.] -> Al[0.][13]

  G4String newName = g4IonName;
  newName = newName + "[";
  TG4Globals::AppendNumberToString(newName, Q);
  newName = newName + "]";
  
  return newName;  
}

//_____________________________________________________________________________
G4String TG4ParticlesManager::CutUniqueIonName(const G4String& uniqueIonName) const
{
/// Construct G4 ion name from unique name by cutting [Q].                    \n
/// Eg. Al[0.][13] -> Al[0.] 

  G4String newName(uniqueIonName);  
  return newName(0, newName.find_first_of(']')+1);
}

//
// public methods
//

//_____________________________________________________________________________
void TG4ParticlesManager::DefineParticles()
{
/// Add particles with standard PDG = 0 to TDatabasePDG
/// and map them to G4 particles objects.

  AddParticlesToPdgDatabase();
  MapParticles();
}  
 
//_____________________________________________________________________________
void TG4ParticlesManager::AddIon(const G4String& name, G4int Z, G4int A, G4int Q, 
                                 G4double excEnergy)
{
/// Add the ion with specified characteristics.

  // Get G4 ion particle definition
  // (Ion is created if it does not yet exist)
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->GetIon(Z, A, excEnergy);
  
  if (!particleDefinition) {
    TString text = "Z, A, excEnergy [keV]: ";
    text += Z;
    text += " ";
    text += A;
    text += " ";
    text += excEnergy/keV;
    text += ".";
    TG4Globals::Exception(
      "TG4ParticlesManager", "AddIon",
      text +  TG4Globals::Endl() + 
      "G4ParticleTable::FindParticle() failed.");
  }        
  
  // Add ion to TDatabasePDG
  AddIonToPdgDatabase(name, particleDefinition, Q);
}

//_____________________________________________________________________________
G4int TG4ParticlesManager::GetPDGEncodingFast(G4ParticleDefinition* particle,
                                              G4int Q)
{
/// Return the PDG code of particle;
/// if standard PDG code is not defined the preregistred
/// fParticlePDGMap is used.

  // get PDG encoding from G4 particle definition
  G4int pdgEncoding = particle->GetPDGEncoding();
  
  if (pdgEncoding) return pdgEncoding;

  // use fParticlePDGMap 
  // if standard/ENDF-6 PDG code is not defined
  G4String name = particle->GetParticleName();
  pdgEncoding = fParticlePDGMap.GetSecond(name, false);

  if (pdgEncoding) return pdgEncoding;

  // if a nucles
  if (particle->GetParticleType() == "nucleus") {

     // Check if nucleus has already PDG defined
     // in fParticlePDGMap 
     G4String uniqueIonName 
       = UniqueIonName(particle->GetParticleName(), Q);
     pdgEncoding = fParticlePDGMap.GetSecond(uniqueIonName, false);

     if (pdgEncoding) return pdgEncoding;

     // Add nucleus in database 
     // (It has not yet been defind)
     pdgEncoding = AddIonToPdgDatabase(uniqueIonName, particle, Q);
     if (pdgEncoding) return pdgEncoding;
  }  

  if (pdgEncoding == 0 && 
      particle->GetParticleName() != "geantino" && 
      particle->GetParticleName() != "chargedgeantino" ) {
    // unknown particle
    TG4Globals::Warning(
      "TG4ParticlesManager", "GetPDGEncodingFast",
      TString(particle->GetParticleName()) + " is not defined.");
  }      

  return pdgEncoding;  
}  
     

//_____________________________________________________________________________
TParticle* TG4ParticlesManager::GetParticle(const TClonesArray* particles, 
                                            G4int index) const
{
/// Retrive particle with given index from TClonesArray 
/// and check type.

#ifdef MCDEBUG
  TObject* particleTObject = particles->UncheckedAt(index);      
  TParticle* particle
    = dynamic_cast<TParticle*>(particleTObject);

  // check particle type
  if (!particle) {
    TG4Globals::Exception(
      "TG4ParticlesManager", "GetParticle", "Unknown particle type");
  }    
  
  return particle;          
#else
  return (TParticle*)particles->UncheckedAt(index);      
#endif  
}     


//_____________________________________________________________________________
G4ParticleDefinition* TG4ParticlesManager::GetParticleDefinition(
                               const TParticle* particle, G4bool warn) const
{
/// Return G4 particle definition for given TParticle

  // get particle definition from G4ParticleTable
  G4int pdgEncoding = particle->GetPdgCode();
  G4ParticleTable* particleTable 
    = G4ParticleTable::GetParticleTable();                
  G4ParticleDefinition* particleDefinition = 0;    
  if (pdgEncoding != 0) 
    particleDefinition = particleTable->FindParticle(pdgEncoding);

  if (!particleDefinition) {
    G4String rootName = particle->GetName();
    G4String g4Name = fParticleNameMap.GetFirst(rootName);
    particleDefinition = particleTable->FindParticle(g4Name);
  }    
  
  if (particleDefinition==0 && warn) {
    G4String text = "pdgEncoding= ";
    text +=  pdgEncoding;
    TG4Globals::Warning(
      "TG4ParticlesManager", "GetParticleDefinition",
      "G4ParticleTable::FindParticle() for particle with " + text + " failed.");
  }        
  
  return particleDefinition;
}

//_____________________________________________________________________________
G4ParticleDefinition* TG4ParticlesManager::GetIonParticleDefinition(
                               const TParticle* particle, G4bool warn) const
{
/// Return G4 particle definition for given ion specified by TParticle.

  // Find ion by name
  G4String ionName = CutUniqueIonName(particle->GetTitle());
            // Use GetTitle() rather that GetName() as the title is always
            // set equal to UniqueIonName
  
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(ionName);
  
  if (particleDefinition==0 && warn) {
    TString text = "pdgEncoding=";
    text += particle->GetPdgCode();
    TG4Globals::Warning(
      "TG4ParticlesManager", "GetIonDefinition:",
      "G4ParticleTable::FindParticle() for particle with " + text + " failed.");
  }        
  
  return particleDefinition;
}


//_____________________________________________________________________________
G4DynamicParticle* TG4ParticlesManager::CreateDynamicParticle(
                                   const TParticle* particle) const
{ 
/// Create G4DynamicParticle.

  // get particle properties
  G4ParticleDefinition* particleDefinition 
    = GetParticleDefinition(particle);    
  if (!particleDefinition) return 0;  
        
  G4ThreeVector momentum = GetParticleMomentum(particle);

  // create G4DynamicParticle
  G4DynamicParticle* dynamicParticle 
    = new G4DynamicParticle(particleDefinition, momentum);
  
  return dynamicParticle;
}


//_____________________________________________________________________________
G4ThreeVector TG4ParticlesManager::GetParticlePosition(
                                   const TParticle* particle) const 
{
/// Return particle vertex position.

  G4ThreeVector position 
     = G4ThreeVector(particle->Vx()*TG4G3Units::Length(),
                     particle->Vy()*TG4G3Units::Length(),
                     particle->Vz()*TG4G3Units::Length());
  return position;
}                       
                        
                        
//_____________________________________________________________________________
G4ThreeVector TG4ParticlesManager::GetParticleMomentum(
                                   const TParticle* particle) const
{
/// Return particle momentum.
  G4ThreeVector momentum 
     = G4ThreeVector(particle->Px()*TG4G3Units::Energy(),
                     particle->Py()*TG4G3Units::Energy(),
                     particle->Pz()*TG4G3Units::Energy());
  return momentum;
}
