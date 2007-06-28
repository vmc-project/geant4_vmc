// $Id: TG4PhysicsManager.cxx,v 1.16 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsManager
// -----------------------
// See the class description in the header file.

#include "TG4PhysicsManager.h"
#include "TG4ModularPhysicsList.h"
#include "TG4ParticlesManager.h"
#include "TG4G3PhysicsManager.h"
#include "TG4StateManager.h"
#include "TG4GeometryServices.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4G3Cut.h"
#include "TG4G3Control.h"
#include "TG4G3Units.h"
#include "TG4Limits.h"

#include <G4ParticleDefinition.hh>
#include <G4VUserPhysicsList.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4VProcess.hh>

#include <TDatabasePDG.h>
#include <TVirtualMCApplication.h>

TG4PhysicsManager* TG4PhysicsManager::fgInstance = 0;

//_____________________________________________________________________________
TG4PhysicsManager::TG4PhysicsManager(G4VUserPhysicsList* physicsList)
  : TG4Verbose("physicsManager"),
    fMessenger(this),
    fParticlesManager(0),
    fG3PhysicsManager(0),
    fPhysicsList(physicsList),
    fProcessMCMap(),
    fProcessControlMap()
{ 
//
  if (fgInstance) {
    TG4Globals::Exception(
      "TG4PhysicsManager", "TG4PhysicsManager",
      "Cannot create two instances of singleton.");
  }
      
  fgInstance = this;  
  
  // create particles manager
  fParticlesManager = new TG4ParticlesManager();
      
  // create G3 physics manager
  fG3PhysicsManager = new TG4G3PhysicsManager();

  // fill process name map
  // FillProcessMap();
}

//_____________________________________________________________________________
TG4PhysicsManager::~TG4PhysicsManager() {
//
  delete fParticlesManager;
  delete fG3PhysicsManager;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4PhysicsManager::FillProcessMap()
{
/// Fill fProcessMCMap.
/// The default G4 process names are used in the map.
/// Not used - the map is filled in physics constructors
/// only with processes that are really built.

  // multiple scattering
  fProcessMCMap.Add("msc",  kPMultipleScattering);
  fProcessMCMap.Add("Imsc", kPMultipleScattering);
    
  // continuous energy loss
  // !! including delta rays
  fProcessMCMap.Add("eIoni",  kPEnergyLoss);
  fProcessMCMap.Add("IeIoni", kPEnergyLoss);
  fProcessMCMap.Add("LowEnergyIoni", kPEnergyLoss);
  fProcessMCMap.Add("hIoni",  kPEnergyLoss);
  fProcessMCMap.Add("IhIoni", kPEnergyLoss);
  fProcessMCMap.Add("hLowEIoni", kPEnergyLoss);
  fProcessMCMap.Add("MuIoni", kPEnergyLoss);
  fProcessMCMap.Add("IMuIonisation", kPEnergyLoss);
  fProcessMCMap.Add("ionIoni",  kPEnergyLoss);
  fProcessMCMap.Add("ionLowEIoni",  kPEnergyLoss);
  fProcessMCMap.Add("PAIonisation",  kPEnergyLoss);
  
  // bending in mag. field
  // kPMagneticFieldL

  // particle decay
  fProcessMCMap.Add("Decay", kPDecay);
  
  // photon pair production or
  // muon direct pair production
  fProcessMCMap.Add("conv", kPPair);
  fProcessMCMap.Add("LowEnConversion", kPPair);
  fProcessMCMap.Add("MuPairProd", kPPair);
  fProcessMCMap.Add("IMuPairProduction", kPPair);

  // Compton scattering
  fProcessMCMap.Add("compt", kPCompton);
  fProcessMCMap.Add("LowEnCompton", kPCompton);
  fProcessMCMap.Add("polarCompt", kPCompton);

  // photoelectric effect
  fProcessMCMap.Add("phot", kPPhotoelectric);
  fProcessMCMap.Add("LowEnPhotoElec", kPPhotoelectric);

  // bremsstrahlung
  fProcessMCMap.Add("eBrem", kPBrem);
  fProcessMCMap.Add("IeBrem", kPBrem);
  fProcessMCMap.Add("MuBrems", kPBrem);
  fProcessMCMap.Add("IMuBremsstrahlung", kPBrem);
  fProcessMCMap.Add("LowEnBrem", kPBrem);

  // delta-ray production
  // kPDeltaRay
  // has to be distinguished from kPEnergyLoss on flight
  
  // positron annihilation
  fProcessMCMap.Add("annihil", kPAnnihilation);
  fProcessMCMap.Add("Iannihil", kPAnnihilation);

  // hadronic interaction
  // kPHadronic

  // nuclear evaporation
  // kPEvaporation
  
  // nuclear fission
  // kPNuclearFission

  // nuclear absorption
  fProcessMCMap.Add("PionMinusAbsorptionAtRest", kPNuclearAbsorption);
  fProcessMCMap.Add("PiMinusAbsorptionAtRest", kPNuclearAbsorption);
  fProcessMCMap.Add("KaonMinusAbsorption", kPNuclearAbsorption);         
  fProcessMCMap.Add("KaonMinusAbsorptionAtRest", kPNuclearAbsorption);         
  
  // antiproton annihilation
  fProcessMCMap.Add("AntiProtonAnnihilationAtRest", kPPbarAnnihilation);
  // fProcessMCMap.Add("AntiNeutronAnnihilationAtRest", not defined);

  // neutron capture    
  fProcessMCMap.Add("NeutronCaptureAtRest", kPNCapture);
  // fProcessMCMap.Add("LCapture", hadron capture not defined);

  // hadronic elastic incoherent scattering
  fProcessMCMap.Add("LElastic", kPHElastic);

  // hadronic inelastic scattering
  fProcessMCMap.Add("inelastic", kPHInhelastic);

  // muon nuclear interaction
  fProcessMCMap.Add("MuNucl", kPMuonNuclear);

  // exceeded time of flight cut
  // kPTOFlimit
  
  // nuclear photofission
  // kPPhotoFission

  // Rayleigh scattering
  fProcessMCMap.Add("Rayleigh Scattering", kPRayleigh);

  // no mechanism is active, usually at the entrance of a new volume
  fProcessMCMap.Add("Transportation", kPTransportation);

  // particle has fallen below energy threshold and tracking stops
  // kPStop
  
  // Cerenkov photon absorption
  fProcessMCMap.Add("Absorption", kPLightAbsorption);

  // Cerenkov photon reflection/refraction
  // kPLightScattering, kPLightReflection, kPLightRefraction
  // has to be inquired from the G4OpBoundary process

  // synchrotron radiation
  fProcessMCMap.Add("SynchrotronRadiation", kPSynchrotron);
}  

//_____________________________________________________________________________
void TG4PhysicsManager::GstparCut(G4int itmed, TG4G3Cut par, G4double parval)
{
/// Set special tracking medium parameter. 
/// It is applied to all logical volumes that use the specified 
/// tracking medium.

  // get medium from the map
  TG4Medium* medium 
    = TG4GeometryServices::Instance()
      ->GetMediumMap()->GetMedium(itmed);
  if ( !medium ) {
    //TG4GeometryServices::Instance()->GetMediumMap()->Print();
    TString text = "mediumId=";
    text += itmed;
    TG4Globals::Warning(
      "TG4PhysicsManager", "GstparCut", 
      "Medium with " + text + " not found."); 
    return;   
  }  

  // get/create user limits
  TG4Limits* limits 
    = TG4GeometryServices::Instance()
      ->GetLimits(medium->GetLimits(),
                  *fG3PhysicsManager->GetCutVector(),
                  *fG3PhysicsManager->GetControlVector());
    
  if (!limits) {
    limits = new TG4Limits(*fG3PhysicsManager->GetCutVector(),
                           *fG3PhysicsManager->GetControlVector());
    if (VerboseLevel() > 1) {
      G4cout << "TG4PhysicsManager::GstparCut: new TG4Limits() for medium " 
             << itmed << " has been created." << G4endl;  
    }             
  }           

  // add units
  if (par == kTOFMAX) parval *= TG4G3Units::Time();
  else                parval *= TG4G3Units::Energy();

  // set new limits object to medium
  medium->SetLimits(limits);

  // set parameter
  limits->SetG3Cut(par, parval);
}


//_____________________________________________________________________________
void TG4PhysicsManager::GstparControl(G4int itmed, TG4G3Control par, 
                                      TG4G3ControlValue parval)
{
/// Set special tracking medium parameter. 
/// It is applied to all logical volumes that use the specified 
/// tracking medium.

  // get medium from the map
  TG4Medium* medium 
    = TG4GeometryServices::Instance()
      ->GetMediumMap()->GetMedium(itmed);
  if ( !medium ) {
    //TG4GeometryServices::Instance()->GetMediumMap()->Print();
    TString text = "mediumId=";
    text += itmed;
    TG4Globals::Warning(
      "TG4PhysicsManager", "GstparControl", 
      "Medium with " + text + " not found."); 
    return;  
  }  

  // get/create user limits
  TG4Limits* limits 
    = TG4GeometryServices::Instance()
      ->GetLimits(medium->GetLimits(),
                  *fG3PhysicsManager->GetCutVector(),
                  *fG3PhysicsManager->GetControlVector());

  if (!limits) {
    limits = new TG4Limits(*fG3PhysicsManager->GetCutVector(),
                           *fG3PhysicsManager->GetControlVector());

    if (VerboseLevel() > 1) {
      G4cout << "TG4PhysicsManager::GstparControl: new TG4Limits() for medium" 
             << itmed << " has been created." << G4endl;  
    }             
  }                           
    
  // set new limits object to medium
  medium->SetLimits(limits);
  
  // set parameter
  limits->SetG3Control(par, parval);
}

//_____________________________________________________________________________
G4ParticleDefinition* 
TG4PhysicsManager::GetParticleDefinition(G4int pdgEncoding) const
{
/// Return G4 particle definition for given PDG encoding.

  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(pdgEncoding);
    
  if (!particleDefinition) {
    TString text = "PDG="; 
    text += pdgEncoding;
    TG4Globals::Warning(
      "TG4ParticlesManager", "GetParticleDefinition",
      "Particle with " + text + " not found.");
  }  
    
  return particleDefinition;    
}

//
// public methods
//

//_____________________________________________________________________________
void  TG4PhysicsManager::Gstpar(Int_t itmed, const char *param, Float_t parval) 
{ 
/// Passe the tracking medium parameter to TG4Limits.
/// The tracking medium parameter is set only in case
/// its value is different from the "global" physics setup.
/// (If: CheckCut/ControlWithG3Defaults is used checking
///  is performed with respect to G3 default values.)
/// When any cut/control parameter is set in limits
/// the physics manager is locked and the physics setup
/// cannot be changed.                                                       \n
/// Applying this TG4Limits to particles and physical
/// processes is still in development.
///
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  To change the value of cut  or mechanism "CHPAR"
///  to a new value PARVAL  for tracking medium ITMED
///  The  data   structure  JTMED   contains  the   standard  tracking
///  parameters (CUTS and flags to control the physics processes)  which
///  are used  by default  for all  tracking media.   It is  possible to
///  redefine individually  with GSTPAR  any of  these parameters  for a
///  given tracking medium. 
///  - ITMED     tracking medium number 
///  - CHPAR     is a character string (variable name) 
///  - PARVAL    must be given as a floating point.

  if (VerboseLevel() > 1) {
    G4cout << "TG4PhysicsManager::Gstpar " 
           << param << "  " << parval << G4endl;
  }           

  G4String name = TG4GeometryServices::Instance()->CutName(param); 
  TG4G3Cut cut;
  if (fG3PhysicsManager->CheckCutWithTheVector(name, parval, cut)) {
      GstparCut(itmed, cut, parval);
      fG3PhysicsManager->Lock();
  }  
  else {
    TG4G3Control control;
    TG4G3ControlValue controlValue; 
    if (fG3PhysicsManager
         ->CheckControlWithTheVector(name, parval, control, controlValue)) {
      GstparControl(itmed, control, controlValue);
      fG3PhysicsManager->Lock();
    } 
    else if (cut==kNoG3Cuts && control==kNoG3Controls) { 
      TG4Globals::Warning(
        "TG4PhysicsManager", "Gstpar",
        TString(name) + " parameter is not yet implemented.");
    }        
  }   
} 
 
//_____________________________________________________________________________
Bool_t TG4PhysicsManager::SetCut(const char* cutName, Float_t cutValue)
{
/// Set the specified cut.

  fG3PhysicsManager->CheckLock();
  TG4G3Cut g3Cut = TG4G3CutVector::GetCut(cutName);

  if (g3Cut == kNoG3Cuts) {
    TG4Globals::Warning(
      "TG4PhysicsManager", "SetCut",
      "Parameter " + TString(cutName) + " is not implemented.");
    return false;
  }  
  
  // add units
  if (g3Cut == kTOFMAX) cutValue *= TG4G3Units::Time();
  else                  cutValue *= TG4G3Units::Energy();

  fG3PhysicsManager->SetCut(g3Cut, cutValue); 
  
  return true;   
}  
  
//_____________________________________________________________________________
Bool_t TG4PhysicsManager::SetProcess(const char* controlName, Int_t value)
{
/// Set the specified process control.

  fG3PhysicsManager->CheckLock();
  TG4G3Control control = TG4G3ControlVector::GetControl(controlName);
  
  if (control != kNoG3Controls) {
    TG4G3ControlValue controlValue 
      = TG4G3ControlVector::GetControlValue(value, control);
    fG3PhysicsManager->SetProcess(control, controlValue);
    
    return true;
  }  
  else {   
    TG4Globals::Warning(
      "TG4PhysicsManager", "SetProcess",
      "Parameter " + TString(controlName) + " is not implemented.");
    return false;
  }  
}  

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::DefineParticle(Int_t pdg, const char* name, 
                           TMCParticleType /*type*/, Double_t /*mass*/, 
                           Double_t /*charge*/, Double_t /*lifetime*/)
{
/// Only check if particle with specified pdg is available in Geant4;
/// if not give exception.

   // Check if particle is available in Geant4
  G4ParticleTable* particleTable 
    = G4ParticleTable::GetParticleTable();                        
  G4ParticleDefinition* particleDefinition = 0;      
  if (pdg != 0) 
     particleDefinition = particleTable->FindParticle(pdg);
  else {
     G4String particleName(name);
     if (name == "Rootino")        
         particleDefinition = particleTable->FindParticle("geantino");
  }        
  
  if (particleDefinition) { 
     if (VerboseLevel() > 0) {    
       G4cout << "TG4PhysicsManager::DefineParticle (PDG = " 
              << pdg << ", " << name << "): " << G4endl;
       G4cout << "   This particle is in Geant4 defined as " 
              <<  particleDefinition->GetParticleName() << G4endl;
     }              
     return true;              
  }            
  else { 
     TString text = "Particle with PDG=";
     text += pdg;
     text += ", name=" + TString(name) + " does not exist in Geant4.";
     TG4Globals::Exception(
       "TG4PhysicsManager", "DefineParticle",
       text + TG4Globals::Endl() +
       "Ask " + TString(TG4Globals::Help()) + 
       " to include this particle in Geant4 VMC.");
     return false;                  
  }
}

//_____________________________________________________________________________
Bool_t TG4PhysicsManager::DefineIon(const char* name, Int_t Z, Int_t A,  
                                    Int_t Q, Double_t excEnergy, Double_t /*mass*/)
{
/// Keep user defined ion properties in order to be able to use
/// them later as a primary particle.

  // add unit
  excEnergy *= TG4G3Units::Energy();
  
  fParticlesManager->AddIon(name, Z, A, Q, excEnergy);
    
  return true;
}                           

//_____________________________________________________________________________
Float_t TG4PhysicsManager::Xsec(char* /*ch*/, Float_t /*p1*/, 
                                Int_t /*i1*/, Int_t /*i2*/)
{
/// Not yet implemented -> give exception.

  TG4Globals::Exception(
    "TG4PhysicsManager", "Xsec", "Not yet implemented.");

  return 0.;
}    
  
//_____________________________________________________________________________
Int_t TG4PhysicsManager::IdFromPDG(Int_t pdgID) const
{
/// G4 does not use the integer particle identifiers
/// Id <-> PDG is identity.

  return pdgID;
}  

//_____________________________________________________________________________
Int_t TG4PhysicsManager::PDGFromId(Int_t mcID) const
{
/// G4 does not use integer particle identifiers
/// Id <-> PDG is identity.

  return mcID;
}  

//_____________________________________________________________________________
TString  TG4PhysicsManager::ParticleName(Int_t pdg) const
{
/// Return name of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);
  
  if (particle)
    return TString(particle->GetParticleName());
  else
    return TString();
}          

//_____________________________________________________________________________
Double_t  TG4PhysicsManager::ParticleMass(Int_t pdg) const          
{
/// Return mass of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);
  
  if (particle)
    return particle->GetPDGMass()/TG4G3Units::Energy();
  else
    return 0.;
}          

//_____________________________________________________________________________
Double_t  TG4PhysicsManager::ParticleCharge(Int_t pdg) const          
{
/// Return charge (in e units) of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);
  
  if (particle)
    return particle->GetPDGCharge()/TG4G3Units::Charge();
  else
    return 0.;
}          

//_____________________________________________________________________________
Double_t  TG4PhysicsManager::ParticleLifeTime(Int_t pdg) const  
{
/// Return life time of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);
  
  if (particle)
    return particle->GetPDGLifeTime();
  else
    return 0.;
}          

//_____________________________________________________________________________
TMCParticleType TG4PhysicsManager::ParticleMCType(Int_t pdg) const
{
/// Return VMC type of G4 particle specified by pdg.

  G4ParticleDefinition* particle = GetParticleDefinition(pdg);
  
  if (particle) {
    TG4Globals::Warning(
      "TG4PhysicsManager", "ParticleMCType", "Not yet implemented.");
    return kPTUndefined;
  }  
  else
    return kPTUndefined;
}          

//_____________________________________________________________________________
void  TG4PhysicsManager::DefineParticles()
{
/// Add particles with standard PDG = 0 
/// and user defined particles to TDatabasePDG
/// and maps them to G4 particles objects.

  fParticlesManager->DefineParticles();

  TG4StateManager::Instance()->SetNewState(kAddParticles);
  TVirtualMCApplication::Instance()->AddParticles();
  TG4StateManager::Instance()->SetNewState(kNotInApplication);
}    

//_____________________________________________________________________________
void TG4PhysicsManager::SetProcessActivation()
{
/// (In)Activate built processes according
/// to the setup in TG4G3PhysicsManager::fControlVector.

  TG4ModularPhysicsList* tg4PhysicsList 
    = dynamic_cast<TG4ModularPhysicsList*>(fPhysicsList);

  if (tg4PhysicsList)
    tg4PhysicsList->SetProcessActivation();
}       


//_____________________________________________________________________________
TMCProcess TG4PhysicsManager::GetMCProcess(const G4VProcess* process)
{
/// Return the TMCProcess code of the specified G4 process.
 
  if (!process) return kPNoProcess;

  return fProcessMCMap.GetMCProcess(process);
}

//_____________________________________________________________________________
TMCProcess TG4PhysicsManager::GetOpBoundaryStatus(const G4VProcess* process)
{
/// Return the TMCProcess code according to the OpBoundary process
/// status.
 
#ifdef MCDEBUG
  const G4OpBoundaryProcess* opBoundary
    = dynamic_cast<const G4OpBoundaryProcess*>(process);
    
  if (!opBoundary) {
    TG4Globals::Exception(
      "TG4PhysicsManager", "GetOpBoundaryStatus", "Wrong process type.");
    return kPNoProcess;
  }
#else
  const G4OpBoundaryProcess* opBoundary = (const G4OpBoundaryProcess*)process;
#endif  

  switch (opBoundary->GetStatus()) {
    // reflection
    case FresnelReflection: 
    case TotalInternalReflection:
    case LambertianReflection: 
    case LobeReflection:
    case SpikeReflection: 
    case BackScattering:
       return kPLightReflection;
       ;;

    // refraction
    case FresnelRefraction: 
       return kPLightRefraction;
       ;;

    // absorption
    case Absorption:
       return kPLightAbsorption;
       ;;
    case Detection: 
       return kPLightDetection;
       ;;
       
    case NotAtBoundary:
    case SameMaterial:
    case StepTooSmall:
    case NoRINDEX:
    case Undefined:
      return kPNoProcess;
      ;;
  }
  
  // should not happen
  return kPNoProcess;
}

