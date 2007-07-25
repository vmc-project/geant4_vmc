// $Id: TG4ParticlesManager.cxx,v 1.11 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4ParticlesManager
// -------------------------
// See the class description in the header file.

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
    fUserIonMap()
    
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

/*
//_____________________________________________________________________________
G4int TG4ParticlesManager::GetPDGIonEncoding(G4int Z, G4int A, G4int iso) const
{
/// Use ENDF-6 mapping for ions = 10000*z+10*a+iso
/// and add 10 000 000.

  return 10000000 + 10000*Z + 10*A + iso; 
}  
*/  
//_____________________________________________________________________________
void TG4ParticlesManager::AddIonToPdgDatabase(const G4String& name,
                              G4ParticleDefinition* particleDefinition)
{
/// Add the ion in TDatabasePDG and the maps
/// and return its generated PDG code.

  // Check if ion
  if ( particleDefinition->GetParticleType() != "nucleus" ) {
    
      TG4Globals::Exception(
        "TG4ParticlesManager", "AddIonToPdgDatabase", 
        "Added particle is not ion."); 
  }    

  // Return if Ion was already added
  G4int pdgEncoding = particleDefinition->GetPDGEncoding();
  TParticlePDG* particlePDG 
    = TDatabasePDG::Instance()->GetParticle(pdgEncoding);
  if ( particlePDG )  return;
  
  // Get particle data
  G4String g4Name = particleDefinition->GetParticleName();
  G4int pdgQ = G4int(particleDefinition->GetPDGCharge()/eplus);
      // !! here we do not save dynamic charge but the static one

  if (VerboseLevel() > 1) {
     G4cout << "Adding ion to TDatabasePDG " << G4endl;
     G4cout << "   name:   " << g4Name << G4endl;
     G4cout << "   g4name: " << name << G4endl;
     G4cout << "   PDG:    " << pdgEncoding << G4endl;
     G4cout << "   pdgQ:   " << pdgQ << G4endl;
  }               

  // Add ion to TDatabasePDG
  TDatabasePDG::Instance()
    ->AddParticle(name, g4Name, 
                  particleDefinition->GetPDGMass()/TG4G3Units::Energy(), 
                  particleDefinition->GetPDGStable(), 
                  particleDefinition->GetPDGWidth()/TG4G3Units::Energy(), 
                  pdgQ*3, "Ion", pdgEncoding);                     
}


//
// public methods
//

//_____________________________________________________________________________
void TG4ParticlesManager::DefineParticles()
{
/// Add special particles with standard PDG = 0 to TDatabasePDG
/// and map them to G4 particles objects.

  const Int_t kspe=50000000;
  TDatabasePDG *pdgDB = TDatabasePDG::Instance();

  // optical phothon
  if ( !pdgDB->GetParticle(kspe+50) )
    pdgDB->AddParticle("Cherenkov", "Cherenkov", 0, kFALSE,
                       0,0,"Special",kspe+50);
  fParticleNameMap.Add("opticalphoton","Cherenkov");

  // feedback phothon
  if ( !pdgDB->GetParticle(kspe+51) )
    pdgDB->AddParticle("FeedbackPhoton","FeedbackPhoton", 0, kFALSE,
                       0, 0, "Special", kspe+51);
  fParticleNameMap.AddInverse("opticalphoton","FeedbackPhoton");

  // generic ion
  // This particle should not appear in tracking (as it is commented 
  // in class G4GenericIon), but as it does, we map it anyway
  if ( !pdgDB->GetParticle(kspe+60) )
    pdgDB->AddParticle("GenericIon", "GenericIon",  0.938272, kTRUE,
                       0, 1, "Special", kspe+60);
  fParticleNameMap.Add("GenericIon","GenericIon");
  
  // Light ions
  // Get PDG codes from Geant4 the rest as in TGeant3
  const Double_t kGeV=0.9314943228;
  const Double_t kHslash = 1.0545726663e-27;
  const Double_t kErgGeV = 1/1.6021773349e-3;
  const Double_t kHshGeV = kHslash*kErgGeV;
  const Double_t kYearsToSec = 3600*24*365.25;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle;

  particle = particleTable->FindParticle("deuteron");
  if ( particle && ! particleTable->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("Deuteron","Deuteron",2*kGeV+8.071e-3, kTRUE,
		       0, 3, "Ion", particle->GetPDGEncoding());
  }                       
                       
  particle = particleTable->FindParticle("triton");
  if ( particle && ! particleTable->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("Triton","Triton",3*kGeV+14.931e-3,kFALSE,
		     kHshGeV/(12.33*kYearsToSec), 3, "Ion", 
                     particle->GetPDGEncoding());
  }                       
                       
  particle = particleTable->FindParticle("alpha");
  if ( particle && ! particleTable->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("Alpha","Alpha",4*kGeV+2.424e-3, kTRUE,
		       kHshGeV/(12.33*kYearsToSec), 6, "Ion",
                       particle->GetPDGEncoding());
  }                       

  particle = particleTable->FindParticle("He3");
  if ( particle && ! particleTable->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("HE3", "HE3", 3*kGeV+14.931e-3, kFALSE,
		       0, 6, "Ion", particle->GetPDGEncoding());
  }                       

  // geantino
  fParticleNameMap.Add("geantino", "Rootino");
  fParticleNameMap.Add("chargedgeantino", "Rootino");
  
  if (VerboseLevel() > 1) {
    fParticleNameMap.PrintAll();
  }  
}  
 
//_____________________________________________________________________________
void TG4ParticlesManager::AddIon(const G4String& name, G4int Z, G4int A, G4int Q, 
                                 G4double excEnergy)
{
/// Add the ion with specified characteristics.

  if ( VerboseLevel() > 1 ) {
    G4cout << "Adding ion with Z, A, Q, excEnergy [keV]: "
           << Z << " " << A << " " << Q << " " << excEnergy/keV << G4endl;
  }         

  // Get G4 ion particle definition
  // (Ion is created if it does not yet exist)
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->GetIon(Z, A, excEnergy);
  
  if ( !particleDefinition ) {
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
  AddIonToPdgDatabase(name, particleDefinition);
  
  // Add ion to the map to be able to retrieve later its charge
  fUserIonMap[name] 
    = new TG4UserIon(name, particleDefinition->GetPDGEncoding(), Q);
}

//_____________________________________________________________________________
G4int TG4ParticlesManager::GetPDGEncoding(G4ParticleDefinition* particle)
{
/// Return the PDG code of particle;
/// if standard PDG code is not defined the TDatabasePDG
/// is used.

  // Get PDG encoding from G4 particle definition
  G4int pdgEncoding = particle->GetPDGEncoding();
  if ( pdgEncoding ) {
    // Add particle to TDatabasePDG
    if ( ! TDatabasePDG::Instance()->GetParticle(pdgEncoding) )
       AddIonToPdgDatabase(particle->GetParticleName(), particle); 
    return pdgEncoding;
  }     
  
  // Get PDG encoding from TDatabasePDG if not defined in Geant4
  
  // get particle name from the name map
  G4String g4name = particle->GetParticleName();
  G4String tname = fParticleNameMap.GetSecond(g4name);
  if (tname == "Undefined") {
    particle->DumpTable();
    TG4Globals::Exception(
      "TG4ParticlesManager", "GetPDGEncoding",
      "Particle " + TString(g4name) + " was not found in the name map.");
  }  
  
  // get particle from TDatabasePDG
  TDatabasePDG* pdgDB = TDatabasePDG::Instance();
  TParticlePDG* tparticle = pdgDB->GetParticle(tname);
  if ( !tparticle ) {
    TG4Globals::Exception(
      "TG4ParticlesManager", "GetPDGEncoding",
      "Particle " +  TString(tname) + " was not found in TDatabasePDG.");
  }  
  
  // get PDG encoding
  return tparticle->PdgCode();
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
    TString text = "pdgEncoding= ";
    text +=  pdgEncoding;
    TG4Globals::Warning(
      "TG4ParticlesManager", "GetParticleDefinition",
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

//_____________________________________________________________________________
TG4UserIon*  TG4ParticlesManager::GetUserIon(
                              const G4String& ionName, G4bool warn) const
{
/// Return the user defined ion properties     

   UserIonMap::const_iterator it = fUserIonMap.find(ionName);
   if ( it != fUserIonMap.end() )  return it->second;

   if ( warn) {
     TG4Globals::Warning(
       "TG4ParticlesManager", "GetUserIon",
       "User ion with name=" + ionName + " not defined.");
   }    
   return 0;
}     
                                                          
