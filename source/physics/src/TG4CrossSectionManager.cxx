//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CrossSectionManager.cxx
/// \brief Implementation of the TG4CrossSectionManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4CrossSectionManager.h"
#include "TG4Globals.h"
#include "TG4RegionsManager.h"

#include <G4NistManager.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <G4HadronicProcessStore.hh>

#include <TH1.h>
#include <TObjArray.h>

// Moved after Root includes to avoid shadowed variables
// generated from short units names
#include <G4SystemOfUnits.hh>

#include <iomanip>

const G4String TG4CrossSectionManager::fgkDefaultParticleName = "proton";
const G4String TG4CrossSectionManager::fgkDefaultElementName  = "Al";
const G4double TG4CrossSectionManager::fgkDefaultMinKinEnergy = 0.1*MeV;
const G4double TG4CrossSectionManager::fgkDefaultMaxKinEnergy = 1*TeV;
const G4double TG4CrossSectionManager::fgkDefaultMinMomentum = 1*MeV;
const G4double TG4CrossSectionManager::fgkDefaultMaxMomentum = 100*TeV;
const G4int    TG4CrossSectionManager::fgkDefaultNofBinsE = 700;
const G4int    TG4CrossSectionManager::fgkDefaultNofBinsP = 800;
const G4double TG4CrossSectionManager::fgkDefaultKinEnergy = 1*MeV;

//_____________________________________________________________________________
TG4CrossSectionManager::TG4CrossSectionManager()
  : TG4Verbose("crossSectionManager"),
    fMessenger(this),
    fHistograms(0),
    fParticleName(fgkDefaultParticleName),
    fElementName(fgkDefaultElementName),
    fMinKinEnergy(fgkDefaultMinKinEnergy),
    fMaxKinEnergy(fgkDefaultMaxKinEnergy),
    fMinMomentum(fgkDefaultMinMomentum),
    fMaxMomentum(fgkDefaultMaxMomentum),
    fNofBinsE(fgkDefaultNofBinsE),
    fNofBinsP(fgkDefaultNofBinsP),
    fLabel(),
    fKinEnergy(fgkDefaultKinEnergy),
    fIsInitialised(false),
    fMakeHistograms(false)
{
/// Default constructor
}

//_____________________________________________________________________________
TG4CrossSectionManager::~TG4CrossSectionManager()
{
/// Destructor

  // fHistograms->Delete();
  delete fHistograms;
}

//
// private methods
//

//_____________________________________________________________________________
const G4ParticleDefinition* TG4CrossSectionManager::GetParticle() const
{
/// Return the particle for selected particle name

  const G4ParticleDefinition* particle
    = G4ParticleTable::GetParticleTable()->FindParticle(fParticleName);

  if ( ! particle ) {
    TString text = "Particle \"";
    text += fParticleName.data();
    text += "\" not found.";
    TG4Globals::Warning("TG4CrossSectionManager", "GetParticle", text);
    return 0;
  }

  return particle;
}

//_____________________________________________________________________________
const G4Element* TG4CrossSectionManager::GetElement() const
{
/// Return the element for selected element name

  const G4Element* element
    = G4NistManager::Instance()->FindOrBuildElement(fElementName);

  if ( ! element ) {
    TString text = "Element \"";
    text += fElementName.data();
    text += "\" not found.";
    G4cout << "element:" << fElementName << G4endl;
    TG4Globals::Warning("TG4CrossSectionManager", "GetElement", text);
    return 0;
  }

  return element;
}

//_____________________________________________________________________________
G4double TG4CrossSectionManager::GetMomentum() const
{
/// Return the momentum for selected particle and kinetic energy

  const G4ParticleDefinition* particle = GetParticle();

  G4double mass = particle->GetPDGMass();
  G4double etot = mass + fKinEnergy;

  return std::sqrt(etot*etot - mass*mass);
}

//_____________________________________________________________________________
void TG4CrossSectionManager::CreateHistograms()
{
/// Create the cross sections histograms.

  if ( fIsInitialised ) return;

  fHistograms = new TObjArray();

  TString title0(fLabel.data());
  title0 += ": ";
  title0 += fParticleName.data();
  title0 += " - ";
  title0 += fElementName.data();
  title0 += " : ";

  TString title;
  title = title0 + "Elastic cross section (barn) as a functions of log10(p/GeV)";
  fHistograms->Add(new TH1D("h1", title, fNofBinsP,
                   std::log10(fMinMomentum/GeV), std::log10(fMaxMomentum/GeV)));

  title = title0 + "Elastic cross section (barn) as a functions of log10(E/MeV)";
  fHistograms->Add(new TH1D("h2", title, fNofBinsE,
                   std::log10(fMinKinEnergy/GeV), std::log10(fMaxKinEnergy/GeV)));

  title = title0 + "Inelastic cross section (barn) as a functions of log10(p/GeV)";
  fHistograms->Add(new TH1D("h3", title, fNofBinsP,
                   std::log10(fMinMomentum/GeV), std::log10(fMaxMomentum/GeV)));

  title = title0 + "Inelastic cross section (barn) as a functions of log10(E/MeV)";
  fHistograms->Add(new TH1D("h4", title, fNofBinsE,
                   std::log10(fMinKinEnergy/GeV), std::log10(fMaxKinEnergy/GeV)));

  if ( fParticleName == "neutron" ) {
    title = title0 + "Capture cross section (barn) as a functions of log10(E/MeV)";
    fHistograms->Add(new TH1D("h5", title, fNofBinsE,
                     std::log10(fMinKinEnergy/GeV), std::log10(fMaxKinEnergy/GeV)));

    title = title0 + "Fission cross section (barn) as a functions of log10(E/MeV)";
    fHistograms->Add(new TH1D("h6", title, fNofBinsE,
                     std::log10(fMinKinEnergy/GeV), std::log10(fMaxKinEnergy/GeV)));
  }
/*
  title = title0 + "Charge exchange cross section (barn) as a functions of log10(E/MeV)";
  fHistograms->Add(new TH1D("h7", title, fNofBinsE,
                   std::log10(fMinKinEnergy/GeV), std::log10(fMaxKinEnergy/GeV)));
*/

  fIsInitialised = true;
}

//_____________________________________________________________________________
void TG4CrossSectionManager::FillHistograms()
{
/// Fill the cross sections histograms. This method has to be called when the physics
/// list is fully initialised, this is true for sure in the EndOfRunAction.

  if ( ! fIsInitialised ) CreateHistograms();

  if ( VerboseLevel() > 0 ) {
    G4cout << "TG4CrossSectionManager: Filling histograms is started." << G4endl;
  }

  const G4Element* elm = GetElement();
  const G4ParticleDefinition* particle = GetParticle();

  if ( VerboseLevel() > 0 ) {
    G4cout << "### Fill Cross Sections for " << fParticleName
	   << " on " << fElementName
	   << G4endl;
    G4cout << "-------------------------------------------------------------"
	   << G4endl;
    G4cout << "    N     E(MeV)    Elastic(barn)   Inelastic(barn)";
    //if ( particle == neutron ) G4cout << "  Capture(barn)     Fission(barn)";
    if ( fParticleName == "neutron" ) G4cout << "  Capture(barn)     Fission(barn)";
    G4cout << "    ChargeExchange(barn)" << G4endl;
    G4cout << "-------------------------------------------------------------"
	   << G4endl;
  }

  G4int prec = G4cout.precision();
  G4cout.precision(7);

  G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
  G4double mass = particle->GetPDGMass();

  // Build histograms

  G4double p1 = std::log10(fMinMomentum/GeV);
  G4double p2 = std::log10(fMaxMomentum/GeV);
  G4double e1 = std::log10(fMinKinEnergy/MeV);
  G4double e2 = std::log10(fMaxKinEnergy/MeV);
  G4double de = (e2 - e1)/G4double(fNofBinsE);
  G4double dp = (p2 - p1)/G4double(fNofBinsP);

  G4double x  = e1 - de*0.5;
  G4double e, p, xs;
  for ( G4int i=0; i<fNofBinsE; ++i ) {
    x += de;
    e  = std::pow(10.,x)*MeV;

    if ( VerboseLevel() > 0 ) {
      G4cout << std::setw(5) << i << std::setw(12) << e;
    }

    xs = store->GetElasticCrossSectionPerAtom(particle,e,elm);
    if ( VerboseLevel() > 0 ) {
      G4cout << std::setw(14) << xs/barn;
    }
    ((TH1D*)fHistograms->At(1))->Fill(x, xs/barn);

    xs = store->GetInelasticCrossSectionPerAtom(particle,e,elm);
    if ( VerboseLevel() > 0 ) {
      G4cout << " " << std::setw(17) << xs/barn;
    }
    ((TH1D*)fHistograms->At(3))->Fill(x, xs/barn);

    //if ( particle == neutron ) {
    if ( fParticleName == "neutron" ) {
      xs = store->GetCaptureCrossSectionPerAtom(particle,e,elm);
      if ( VerboseLevel() > 0 ) {
        G4cout << " " << std::setw(17) << xs/barn;
      }
      ((TH1D*)fHistograms->At(4))->Fill(x, xs/barn);

      xs = store->GetFissionCrossSectionPerAtom(particle,e,elm);
      if ( VerboseLevel() > 0 ) {
        G4cout << " " << std::setw(17) << xs/barn;
      }
      ((TH1D*)fHistograms->At(5))->Fill(x, xs/barn);
    }

    xs = store->GetChargeExchangeCrossSectionPerAtom(particle,e,elm);
    if ( VerboseLevel() > 0 ) {
      G4cout << " " << std::setw(17) << xs/barn;
    }
    //((TH1D*)fHistograms->At(6))->Fill(x, xs/barn);

    if ( VerboseLevel() > 0 ) {
      G4cout << "  " << x << G4endl;
    }
  }

  x = p1 - dp*0.5;
  for ( G4int i=0; i<fNofBinsP; ++i ) {
    x += dp;
    p  = std::pow(10.,x)*GeV;
    e  = std::sqrt(p*p + mass*mass) - mass;
    xs = store->GetElasticCrossSectionPerAtom(particle,e,elm);
    ((TH1D*)fHistograms->At(0))->Fill(x, xs/barn);
    xs = store->GetInelasticCrossSectionPerAtom(particle,e,elm);
    ((TH1D*)fHistograms->At(2))->Fill(x, xs/barn);
  }
  if ( VerboseLevel() > 0 ) {
    G4cout << "---------------------------------------------------------"
	   << G4endl;
  }
  G4cout.precision(prec);
  //fHistograms->Write();
}

//
// public methods
//

//_____________________________________________________________________________
TObjArray* TG4CrossSectionManager::MakeHistograms()
{
/// This call first triggers creating and filling the array of histograms
/// which is then returned. This method has to be called when the physics
/// list is fully initialised, this is true for sure in the EndOfRunAction.

  CreateHistograms();
  FillHistograms();

  return fHistograms;
}

//_____________________________________________________________________________
G4double TG4CrossSectionManager::GetCrossSection(
                                    TG4CrossSectionType type) const
{
/// Return the value of the selected cross section type for the
/// particle and element selected via set methods

  if ( type == kCapture || type == kFission ) {
    if ( fParticleName != "neutron" ) return 0;
  }

  const G4Element* elm = GetElement();
  const G4ParticleDefinition* particle = GetParticle();

  G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
  switch ( type ) {
    case kElastic:
      return store->GetElasticCrossSectionPerAtom(particle,fKinEnergy,elm);
    case kInelastic:
      return store->GetInelasticCrossSectionPerAtom(particle,fKinEnergy,elm);
    case kCapture:
      return store->GetCaptureCrossSectionPerAtom(particle,fKinEnergy,elm);
    case kFission:
      return store->GetFissionCrossSectionPerAtom(particle,fKinEnergy,elm);
    case kChargeExchange:
      return store->GetChargeExchangeCrossSectionPerAtom(particle,fKinEnergy,elm);
    default:
      return 0;
  }
}

//_____________________________________________________________________________
void TG4CrossSectionManager::PrintCrossSections() const
{
/// Print all cross sections for the particle and element selected via set methods

  G4cout << G4endl
         << "For \"" << fParticleName << " : " << fElementName << "\"  "
         << "  momentum:  " << GetMomentum() / GeV << " GeV"
         << "  kinEnergy: " << fKinEnergy / GeV << " GeV"
         <<  G4endl;

  for (G4int i=0; i<kNoCrossSectionType; ++i ) {
    if ( ( i== kCapture ||  i== kFission ) && fParticleName != "neutron" ) continue;
    G4cout << "  "
           << std::setw(15) << std::left << TG4CrossSectionTypeName(i)
           << " cross section:  "
           << GetCrossSection(GetCrossSectionType(i))/barn << " barn" << G4endl;
  }
}

//_____________________________________________________________________________
void TG4CrossSectionManager::PrintCrossSection(
                                  TG4CrossSectionType type) const
{
/// Print the value of the selected cross section type for the
/// particle and element selected via set methods

  G4cout << "For \"" << fParticleName << " : " << fElementName << "\"  "
         << "  momentum:  " << GetMomentum() / GeV << " GeV"
         << "  kinEnergy: " << fKinEnergy / GeV << " GeV"
         <<  G4endl;
  G4cout << "  "
         << TG4CrossSectionTypeName(type)
         << " cross section:  "
         << GetCrossSection(type)/barn << " barn" << G4endl;
}


//_____________________________________________________________________________
void TG4CrossSectionManager::SetMomentum(G4double momentum)
{
/// Recalculate the current kinetic energy for given momentum and selected particle

  const G4ParticleDefinition* particle = GetParticle();

  G4double mass = particle->GetPDGMass();
  fKinEnergy = std::sqrt(momentum*momentum + mass*mass) - mass;
}
