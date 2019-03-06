//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ParticlesManager.cxx
/// \brief Implementation of the TG4ParticlesManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ParticlesManager.h"
#include "TG4UserIon.h"
#include "TG4UserParticle.h"
#include "TG4G3Units.h"

#include <G4ParticleDefinition.hh>
#include <G4DynamicParticle.hh>
#include <G4ParticleTable.hh>
#include <G4IonTable.hh>
#include <G4Monopole.hh>
#include <G4AutoLock.hh>
#include <G4Version.hh>

#include <TDatabasePDG.h>
#include <TParticle.h>
#include <TClonesArray.h>

#include <G4VDecayChannel.hh>
#include <G4PhaseSpaceDecayChannel.hh>
#include <G4DecayTable.hh>
#include "TG4StateManager.h"
#include <TVirtualMCApplication.h>
#include <TParticle.h>

// Moved after Root includes to avoid shadowed variables
// generated from short units names
#include <G4SystemOfUnits.hh>

#ifdef G4MULTITHREADED
namespace {
  //Mutex to lock master application when merging data
  G4Mutex addParticleMutex = G4MUTEX_INITIALIZER;
}
#endif

TG4ParticlesManager* TG4ParticlesManager::fgInstance = 0;

//_____________________________________________________________________________
TG4ParticlesManager::TG4ParticlesManager()
  : TG4Verbose("particlesManager"),
    fParticleNameMap(),
    fUserIonMap(),
    fUserParticles(),
    fParticlesChecker()

{
/// Default constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4ParticlesManager", "TG4ParticlesManager",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4ParticlesManager::~TG4ParticlesManager()
{
/// Destructor

  fgInstance = 0;
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
void TG4ParticlesManager::AddParticleToPdgDatabase(const G4String& name,
                              G4ParticleDefinition* particleDefinition)
{
/// Add the particle definition in TDatabasePDG

  // Return if particle was already added
  G4int pdgEncoding = particleDefinition->GetPDGEncoding();
  TParticlePDG* particlePDG
    = TDatabasePDG::Instance()->GetParticle(pdgEncoding);
  if ( particlePDG )  return;

  // Get particle data
  G4String g4Name = particleDefinition->GetParticleName();
  G4int pdgQ = G4int(particleDefinition->GetPDGCharge()/eplus);
      // !! here we do not save dynamic charge but the static one
  G4String g4Type = particleDefinition->GetParticleType();
  G4String rootType = g4Type;
  if ( g4Type == "nucleus" ||  g4Type == "anti_nucleus") rootType="Ion";

  if (VerboseLevel() > 1) {
     G4cout << "Adding particle to TDatabasePDG " << G4endl;
     G4cout << "   name:   " << g4Name << G4endl;
     G4cout << "   g4name: " << name << G4endl;
     G4cout << "   PDG:    " << pdgEncoding << G4endl;
     G4cout << "   pdgQ:   " << pdgQ << G4endl;
     G4cout << "   type:   " << rootType << G4endl;
  }

  // Add particle to TDatabasePDG
#ifdef G4MULTITHREADED
  G4AutoLock lm(&addParticleMutex);
#endif
  TDatabasePDG::Instance()
    ->AddParticle(name, g4Name,
                  particleDefinition->GetPDGMass()/TG4G3Units::Energy(),
                  particleDefinition->GetPDGStable(),
                  particleDefinition->GetPDGWidth()/TG4G3Units::Energy(),
                  pdgQ*3, rootType, pdgEncoding);
#ifdef G4MULTITHREADED
  lm.unlock();
#endif
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
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("Deuteron","Deuteron",2*kGeV+8.071e-3, kTRUE,
		       0, 3, "Ion", particle->GetPDGEncoding());
  }

  particle = particleTable->FindParticle("triton");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("Triton","Triton",3*kGeV+14.931e-3,kFALSE,
		     kHshGeV/(12.33*kYearsToSec), 3, "Ion",
                     particle->GetPDGEncoding());
  }

  particle = particleTable->FindParticle("alpha");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("Alpha","Alpha",4*kGeV+2.424e-3, kTRUE,
		       kHshGeV/(12.33*kYearsToSec), 6, "Ion",
                       particle->GetPDGEncoding());
  }

  particle = particleTable->FindParticle("He3");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("HE3", "HE3", 3*kGeV+14.931e-3, kFALSE,
		       0, 6, "Ion", particle->GetPDGEncoding());
  }

  // Light anti-ions
  // Get parameters from Geant4

  particle = particleTable->FindParticle("anti_deuteron");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("AntiDeuteron", "AntiDeuteron", 1.875613, kTRUE,
		       0, -3, "Ion", particle->GetPDGEncoding());
  }

  particle = particleTable->FindParticle("anti_triton");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("AntiTriton", "AntiTriton", 2.808921, kTRUE,
		       0, -3, "Ion", particle->GetPDGEncoding());
  }

  particle = particleTable->FindParticle("anti_alpha");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("AntiAlpha","AntiAlpha", 3.727379, kTRUE,
		       0, -6, "Ion", particle->GetPDGEncoding());
  }

  particle = particleTable->FindParticle("anti_He3");
  if ( particle && ! pdgDB->GetParticle(particle->GetPDGEncoding()) ) {
    pdgDB->AddParticle("AntiHE3", "AntiHE3", 2.808391, kTRUE,
		       0, -6, "Ion", particle->GetPDGEncoding());
  }

  // monopole
  particle = particleTable->FindParticle("monopole");
  if ( particle ) {
    if ( !pdgDB->GetParticle(60000000) ) {
      G4cout << "Adding monnopole in TDatabase with mass " << particle->GetPDGMass()/GeV << G4endl;
      pdgDB->AddParticle("Monopole","Monopole",
                         particle->GetPDGMass()/GeV, kTRUE,
                         particle->GetPDGCharge()/eplus*3.,
                         static_cast<G4Monopole*>(particle)->MagneticCharge()/eplus*3.,
                         "Special", 60000000);
      fParticleNameMap.Add("monopole","monopole");
    } else {
      TG4Globals::Warning(
        "TG4ParticlesManager", "DefineParticles",
        "Cannot add monopole with PDG=60000000 in TDatabasePDG.");
    }
  }

  // geantino
  fParticleNameMap.Add("geantino", "Rootino");
  fParticleNameMap.Add("chargedgeantino", "ChargedRootino");
      // ChargedRootino does not exist in Root particle database
      // but the user can just change the title of Rootino to ChargedRootino
      // to get it interpreted as chargedgeantino:
      // TParticlePDG* rootino = pdgTable->GetParticle("Rootino");
      // if (rootino) rootino->SetTitle("ChargedRootino");

  if (VerboseLevel() > 1) {
    fParticleNameMap.PrintAll();
  }
}

//_____________________________________________________________________________
void TG4ParticlesManager::AddParticle(Int_t pdg, const TString& name,
                 TMCParticleType mcType,
                 Double_t mass, Double_t charge, Double_t lifetime,
                 const TString& pType, Double_t width,
                 Int_t iSpin, Int_t iParity, Int_t iConjugation,
                 Int_t iIsospin, Int_t iIsospinZ, Int_t gParity,
                 Int_t lepton, Int_t baryon,
                 Bool_t stable, Bool_t shortlived,
                 const TString& subType,
                 Int_t antiEncoding, Double_t magMoment,
                 Double_t excitation)
{
/// Add the user defined particle with specified characteristics.

  if ( VerboseLevel() > 1 ) {
    G4cout << "Adding particle with:  "             << G4endl
           << "  pdg =          " << pdg            << G4endl
           << "  name =         " << name           << G4endl
           << "  mcType =       " << mcType         << G4endl
           << "  mass [GeV} =   " << mass           << G4endl
           << "  charge [e] =   " << charge         << G4endl
           << "  lifetime [s] = " << lifetime       << G4endl
           << "  pType =        " << pType.Data()   << G4endl
           << "  width =        " << width          << G4endl
           << "  iSpin =        " << iSpin          << G4endl
           << "  iParity =      " << iParity        << G4endl
           << "  iConjugation=  " << iConjugation   << G4endl
           << "  iIsospin =     " << iIsospin       << G4endl
           << "  iIsospinZ =    " << iIsospinZ      << G4endl
           << "  gParity =      " << gParity        << G4endl
           << "  lepton =       " << lepton         << G4endl
           << "  baryon =       " << baryon         << G4endl
           << "  stable =       " << stable         << G4endl
           << "  shortlived =   " << shortlived     << G4endl
           << "  subType =      " << subType.Data() << G4endl
           << "  antiEncoding = " << antiEncoding   << G4endl
           << "  magMoment =    " << magMoment      << G4endl
           << "  excitation [GeV] = " << excitation << G4endl;
  }

  // Instantiate a new user particle
  // with the first available user type
  //
  fUserParticles.push_back(
    new TG4UserParticle(
           name.Data(), mcType,
           mass * TG4G3Units::Energy(), width * TG4G3Units::Energy(),
           charge* TG4G3Units::Charge(),
           iSpin, iParity, iConjugation, iIsospin, iIsospinZ, gParity,
           pType.Data(), lepton, baryon, pdg, stable,
           lifetime * TG4G3Units::Time(), 0,
           shortlived, subType.Data(), antiEncoding, magMoment,
           excitation * TG4G3Units::Energy()));

  // Add particle to TDatabasePDG
  Int_t anti = -1;
  if ( antiEncoding != 0 ) anti = antiEncoding;

  TDatabasePDG *pdgDB = TDatabasePDG::Instance();
  if ( !pdgDB->GetParticle(pdg) ) {
    pdgDB->AddParticle(name.Data(), name.Data(), mass, stable,
                       width, charge*3, pType.Data(), pdg, anti);
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
#if G4VERSION_NUMBER >= 1000
  G4ParticleDefinition* particleDefinition
    = G4IonTable::GetIonTable()->GetIon(Z, A, excEnergy);
#else
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->GetIon(Z, A, excEnergy);
#endif

  if ( ! particleDefinition ) {
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
#if G4VERSION_NUMBER >= 1000
      "G4IonTable::GetIon() failed.");
#else
      "G4ParticleTable::GetIon() failed.");
#endif
  }

  // Add ion to TDatabasePDG
  AddParticleToPdgDatabase(name, particleDefinition);

  // Add ion to the map to be able to retrieve later its charge
  fUserIonMap[name]
    = new TG4UserIon(name, particleDefinition->GetPDGEncoding(), Q);
}

//_____________________________________________________________________________
Bool_t TG4ParticlesManager::SetDecayMode(Int_t pdg,
                                         Float_t bratio[6], Int_t mode[6][3])
{
/// Set a user phase space decay for a particle

  // Check input pdg
  if ( pdg == 0 ) {
    TG4Globals::Exception(
       "TG4ParticlesManager", "SetDecayMode",
       "Cannot define decay mode for particle with PDG=0");
  }

  G4ParticleTable* particleTable= G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle(pdg);

  if ( ! particleDefinition ) {
     TString pdgs;
     pdgs += pdg;
     TG4Globals::Exception(
       "TG4ParticlesManager", "SetDecayMode",
       "Particle PDG=" + pdgs + " was not found in G4ParticleTable.");
  }

  G4String particleName=  particleDefinition->GetParticleName();
  G4DecayTable *decayTable =  new G4DecayTable();

  // Loop over decay channels
  //
  for (Int_t kz = 0; kz < 6; ++kz) {

    // Do not fill empty channels
    if ( bratio[kz] == 0.) break;

    // Fill names of daughters
    G4int nofDaughters = 0;
    std::vector<G4String> daughtersNames(3);
    for ( G4int i=0; i<3; i++ ) {
      daughtersNames[i] = "";
      if ( mode[kz][i] != 0 ) {
        G4ParticleDefinition* daughter = particleTable->FindParticle(mode[kz][i]);
        if ( ! daughter ) {
          TString pdgs;
          pdgs += mode[kz][i];
          TG4Globals::Exception(
            "TG4ParticlesManager", "SetDecayMode",
            "Daughter particle PDG=" + pdgs + " was not found in G4ParticleTable.");
        }
        daughtersNames[i] = daughter->GetParticleName();
        ++nofDaughters;
      }
    }

    // Define decay channel
    G4VDecayChannel* decayChannel
      = new G4PhaseSpaceDecayChannel(
              particleName, bratio[kz]/100.0, nofDaughters,
              daughtersNames[0], daughtersNames[1], daughtersNames[2]);
    decayTable->Insert(decayChannel);
  }

  particleDefinition->SetDecayTable(decayTable);

  if ( VerboseLevel() > 1 )
    particleDefinition->DumpTable();

  return true;
}

//_____________________________________________________________________________
void TG4ParticlesManager::SetUserDecay(Int_t pdg)
{
/// Force the decay of particle with given PDG to be done with user
/// defined decay or external decayer
/// Set the decay table to zero.
/// Do not delete the existing table as it may be used somewhere else.

  // Check input pdg
  if ( pdg == 0 ) {
    TG4Globals::Exception(
       "TG4ParticlesManager", "SetUserDecay",
       "Cannot set user decay mode for particle with PDG=0");
  }

  G4ParticleTable* particleTable= G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle(pdg);

  if ( ! particleDefinition ) {
     TString pdgs;
     pdgs += pdg;
     TG4Globals::Exception(
       "TG4ParticlesManager", "SetUserDecay",
       "Particle PDG=" + pdgs + " was not found in G4ParticleTable.");
  }

  particleDefinition->SetDecayTable(0);
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
       AddParticleToPdgDatabase(particle->GetParticleName(), particle);
    return pdgEncoding;
  }

  // Get PDG encoding from TDatabasePDG if not defined in Geant4

  // get particle name from the name map
  G4String g4name = particle->GetParticleName();
  G4String tname = fParticleNameMap.GetSecond(g4name);
  if ( tname == "ChargedRootino" ) tname = "Rootino";
          // special treatment for Rootino
          // user can reset the particle title to ChargedRootino to interpret
          // Rootino as chargedgeantino

  if ( tname == "Undefined") {
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
    if ( rootName == "Rootino") rootName = particle->GetTitle();
           // special treatment for Rootino
           // user can reset the particle title to ChargedRootino to interpret
           // Rootino as chargedgeantino
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

  // set polarization
  G4ThreeVector polarization = GetParticlePolarization(particle);
  dynamicParticle
    ->SetPolarization(polarization.x(), polarization.y(), polarization.z());

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
G4ThreeVector TG4ParticlesManager::GetParticlePolarization(
                                   const TParticle* particle) const
{
/// Return particle polarization.

  TVector3 polarization;
  particle->GetPolarisation(polarization);

  return G4ThreeVector(polarization.X(), polarization.Y(), polarization.Z());
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

//_____________________________________________________________________________
G4int  TG4ParticlesManager::GetNofUserParticles() const
{
/// Return the number of user defined particles

  return G4int(fUserParticles.size());
}

//_____________________________________________________________________________
TG4UserParticle*  TG4ParticlesManager::GetUserParticle(G4int index) const
{
/// Return the index-th user particle

  if ( index < 0 || index >= G4int(fUserParticles.size()) ) {
    TG4Globals::Exception(
       "TG4ParticlesManager", "GetUserParticle",
       "Index out of limits");
  }

  return fUserParticles[index];
}

