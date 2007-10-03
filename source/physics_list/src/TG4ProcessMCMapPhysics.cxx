// $Id: TG4ProcessMCMapPhysics.cxx,v 1.8 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4ProcessMCMapPhysics
// -----------------------------
// See the class description in the header file.

#include "TG4ProcessMCMapPhysics.h"
#include "TG4ProcessMCMap.h"
#include "TG4Globals.h"

#include <TVirtualMCDecayer.h>
#include <TVirtualMC.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>

//_____________________________________________________________________________
TG4ProcessMCMapPhysics::TG4ProcessMCMapPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name)
{
//
  FillMap();
}

//_____________________________________________________________________________
TG4ProcessMCMapPhysics::TG4ProcessMCMapPhysics(G4int verboseLevel,
                                               const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel)
{
//
  FillMap();
}

//_____________________________________________________________________________
TG4ProcessMCMapPhysics::~TG4ProcessMCMapPhysics() 
{
//
}

//
// private methods
//

//_____________________________________________________________________________
void TG4ProcessMCMapPhysics::FillMap()
{
/// Fill the process codes map with known G4 process names

  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();

  mcMap->Add("msc", kPMultipleScattering); 

  mcMap->Add("eIoni", kPEnergyLoss); 
  mcMap->Add("muIoni", kPEnergyLoss); 
  mcMap->Add("hIoni", kPEnergyLoss); 
  mcMap->Add("ionIoni", kPEnergyLoss); 

  mcMap->Add("Decay", kPDecay); 

  mcMap->Add("compt", kPCompton); 
  mcMap->Add("phot", kPPhotoelectric); 
  
  mcMap->Add("eBrem", kPBrem); 
  mcMap->Add("muBrems", kPBrem); 

  mcMap->Add("annihil", kPAnnihilation); 

  mcMap->Add("conv", kPPair); 
  mcMap->Add("muPairProd", kPPair); 

  mcMap->Add("HadronCapture", kPNCapture);
  mcMap->Add("HadronFission", kPNuclearFission);
  
  mcMap->Add("PiMinusAbsorptionAtRest", kPNuclearAbsorption); 
  mcMap->Add("PionMinusAbsorptionAtRest", kPNuclearAbsorption); 
  mcMap->Add("KaonMinusAbsorption", kPNuclearAbsorption); 
  mcMap->Add("KaonMinusAbsorptionAtRest", kPNuclearAbsorption); 
  mcMap->Add("CHIPSNuclearAbsorptionAtRest", kPNuclearAbsorption); 
  
  mcMap->Add("AntiProtonAnnihilationAtRest", kPPbarAnnihilation); 
  mcMap->Add("AntiNeutronAnnihilationAtRest", kPPbarAnnihilation);
         /// \todo Define kPAnnihilation to cover both

  mcMap->Add("HadronElastic", kPHElastic); 
  mcMap->Add("hElastic", kPHElastic); 
  mcMap->Add("CHIPSElasticScattering", kPHElastic); 
  
  mcMap->Add("PionMinusInelastic", kPHInhelastic); 
  mcMap->Add("PionPlusInelastic", kPHInhelastic); 
  mcMap->Add("KaonPlusInelastic", kPHInhelastic); 
  mcMap->Add("KaonMinusInelastic", kPHInhelastic); 
  mcMap->Add("KaonZeroLInelastic", kPHInhelastic); 
  mcMap->Add("KaonZeroSInelastic", kPHInhelastic); 
  mcMap->Add("LambdaInelastic", kPHInhelastic); 
  mcMap->Add("ProtonInelastic", kPHInhelastic); 
  mcMap->Add("AntiProtonInelastic", kPHInhelastic); 
  mcMap->Add("NeutronInelastic", kPHInhelastic); 
  mcMap->Add("AntiNeutronInelastic", kPHInhelastic); 
  mcMap->Add("LambdaInelastic", kPHInhelastic); 
  mcMap->Add("AntiLambdaInelastic", kPHInhelastic); 
  mcMap->Add("SigmaMinusInelastic", kPHInhelastic); 
  mcMap->Add("AntiSigmaMinusInelastic", kPHInhelastic); 
  mcMap->Add("SigmaPlusInelastic", kPHInhelastic); 
  mcMap->Add("AntiSigmaPlusInelastic", kPHInhelastic); 
  mcMap->Add("XiMinusInelastic", kPHInhelastic); 
  mcMap->Add("AntiXiMinusInelastic", kPHInhelastic); 
  mcMap->Add("XiZeroInelastic", kPHInhelastic); 
  mcMap->Add("AntiXiZeroInelastic", kPHInhelastic); 
  mcMap->Add("OmegaMinusInelastic", kPHInhelastic); 
  mcMap->Add("AntiOmegaMinusInelastic", kPHInhelastic); 
  mcMap->Add("DeuteronInelastic", kPHInhelastic); 
  mcMap->Add("dInelastic", kPHInhelastic); 
  mcMap->Add("TritonInelastic", kPHInhelastic); 
  mcMap->Add("tInelastic", kPHInhelastic); 
  mcMap->Add("AlphaInelastic", kPHInhelastic); 
  mcMap->Add("alphaInelastic", kPHInhelastic); 
  mcMap->Add("He3Inelastic", kPHInhelastic); 
  mcMap->Add("ionInelastic", kPHInhelastic); 
  mcMap->Add("PhotonInelastic", kPHInhelastic); 
         /// \todo Define kPPhotonInelastic

  mcMap->Add("nKiller", kPHadronic); 

  mcMap->Add("muMinusCaptureAtRest", kPMuonNuclear); 
  mcMap->Add("PositronNuclear", kPMuonNuclear); 
  mcMap->Add("ElectroNuclear", kPMuonNuclear); 
             /// \todo Add kPPositronNuclear, kPElectronNuclear
  
  mcMap->Add("Cerenkov", kPCerenkov);
  mcMap->Add("Scintillation", kPNoProcess);
  mcMap->Add("OpAbsorption", kPLightAbsorption);
  mcMap->Add("OpRayleigh", kPRayleigh);
  mcMap->Add("OpBoundary", kPLightScattering);
             /// \todo Add kPScintillation

  mcMap->Add("Transportation", kPTransportation);
  
  mcMap->Add("StepLimiter", kStepMax);
  mcMap->Add("specialCutForChargedHadron", kPStop); 
  mcMap->Add("specialCutForElectron", kPStop); 
  mcMap->Add("specialCutForEplus", kPStop); 
  mcMap->Add("specialCutForGamma", kPStop); 
  mcMap->Add("specialCutForMuon", kPStop); 
  mcMap->Add("specialCutForNeutralHadron", kPStop); 
  mcMap->Add("specialCutForForOther", kPStop); 
  mcMap->Add("specialCutForElectron", kPStop); 
  mcMap->Add("specialCutForElectron", kPStop); 
  mcMap->Add("specialControls", kPNull); 
  mcMap->Add("stackPopper", kPNull); 
             /// \todo Add kPUserDefined
  
}  
//
// protected methods
//

//_____________________________________________________________________________
void TG4ProcessMCMapPhysics::ConstructParticle()
{
/// Instantiate particles - nothing to be done here
}

//_____________________________________________________________________________
void TG4ProcessMCMapPhysics::ConstructProcess()
{
/// Loop over all particles and their processes and check if
/// the process is present in the map

  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  G4bool success = true;

  theParticleIterator->reset();
  while ((*theParticleIterator)())
  {
    G4ProcessVector* processVector 
      = theParticleIterator->value()->GetProcessManager()->GetProcessList();
    for (G4int i=0; i<processVector->length(); i++) {
    
      G4String processName = (*processVector)[i]->GetProcessName();

      if ( mcMap->GetMCProcess(processName) == kPNoProcess &&
           processName != "Scintillation" ) {
        G4String text = "Unknown process code for ";
        text += processName;
        TG4Globals::Warning(
          "TG4ProcessMCMapPhysics", "ConstructProcess", text);
        success = false;  
      }
    }              
  }
    
  if ( VerboseLevel() > 0 ) {
    if ( success )    
      G4cout << "### Processes mapped to VMC codes ok." << G4endl;
    else 
      G4cout << "### Processes mapped to VMC codes: some proceesses unknown." 
             << G4endl;
  }    
}
