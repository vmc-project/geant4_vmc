//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessControlMapPhysics.cxx
/// \brief Implementation of the TG4ProcessControlMapPhysics class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ProcessControlMapPhysics.h"
#include "TG4Globals.h"
#include "TG4ProcessControlMap.h"

#include <TVirtualMC.h>
#include <TVirtualMCDecayer.h>

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Version.hh>

//_____________________________________________________________________________
TG4ProcessControlMapPhysics::TG4ProcessControlMapPhysics(
  G4bool isBiasing, const G4String& name)
  : TG4VPhysicsConstructor(name)
{
  /// Standard constructor

  FillMap(isBiasing);
}

//_____________________________________________________________________________
TG4ProcessControlMapPhysics::TG4ProcessControlMapPhysics(
  G4bool isBiasing, G4int theVerboseLevel, const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel)
{
  /// Standard constructor

  FillMap(isBiasing);
}

//_____________________________________________________________________________
TG4ProcessControlMapPhysics::~TG4ProcessControlMapPhysics()
{
  /// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4ProcessControlMapPhysics::FillMap(G4bool isBiasing)
{
  /// Fill the process control map with known G4 process names

  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->SetIsBiasing(isBiasing);

  controlMap->Add("msc", kMULS);
  controlMap->Add("muMsc", kMULS);
  controlMap->Add("CoulombScat", kMULS);
  controlMap->Add("eCoulombScat", kMULS);
  controlMap->Add("nuclearStopping", kMULS);

  controlMap->Add("eIoni", kG3LOSS);
  controlMap->Add("muIoni", kG3LOSS);
  controlMap->Add("hIoni", kG3LOSS);
  controlMap->Add("ionIoni", kG3LOSS);
  controlMap->Add("mplIoni", kG3LOSS);

  controlMap->Add("Decay", kDCAY);
  controlMap->Add("RadioactiveDecay", kDCAY);

  controlMap->Add("compt", kCOMP);
  controlMap->Add("phot", kPHOT);

  controlMap->Add("eBrem", kBREM);
  controlMap->Add("muBrems", kBREM);
  controlMap->Add("hBrems", kBREM);

  controlMap->Add("annihil", kANNI);
  controlMap->Add("AnnihiToMuPair", kANNI);
  controlMap->Add("ee2hadr", kANNI);

  controlMap->Add("conv", kPAIR);
  controlMap->Add("ePairProd", kPAIR);
  controlMap->Add("muPairProd", kPAIR);
  controlMap->Add("hPairProd", kPAIR);
  controlMap->Add("GammaToMuPair", kPAIR);

  controlMap->Add("nCapture", kHADR);
  controlMap->Add("HadronCapture", kHADR);
  controlMap->Add("nFission", kHADR);
  controlMap->Add("HadronFission", kHADR);

  controlMap->Add("PiMinusAbsorptionAtRest", kHADR);
  controlMap->Add("PiMinusAbsorptionBertini", kHADR);
  controlMap->Add("PionMinusAbsorptionAtRest", kHADR);
  controlMap->Add("KaonMinusAbsorption", kHADR);
  controlMap->Add("KaonMinusAbsorptionAtRest", kHADR);
  controlMap->Add("CHIPSNuclearCaptureAtRest", kHADR);
  controlMap->Add("FTFNuclearCaptureAtRest", kHADR);
  controlMap->Add("hFritiofCaptureAtRest", kHADR);
  controlMap->Add("hBertiniCaptureAtRest", kHADR);

  controlMap->Add("AntiProtonAnnihilationAtRest", kHADR);
  controlMap->Add("AntiNeutronAnnihilationAtRest", kHADR);

  controlMap->Add("hadElastic", kHADR);
  controlMap->Add("HadronElastic", kHADR);
  controlMap->Add("hElastic", kHADR);
  controlMap->Add("hElasticLHEP", kHADR);
  controlMap->Add("ionElastic", kHADR);
  controlMap->Add("CHIPSElasticScattering", kHADR);

  controlMap->Add("inelastic", kHADR);
  controlMap->Add("PionMinusInelastic", kHADR);
  controlMap->Add("pi-Inelastic", kHADR);
  controlMap->Add("PionPlusInelastic", kHADR);
  controlMap->Add("pi+Inelastic", kHADR);
  controlMap->Add("KaonPlusInelastic", kHADR);
  controlMap->Add("kaon+Inelastic", kHADR);
  controlMap->Add("KaonMinusInelastic", kHADR);
  controlMap->Add("kaon-Inelastic", kHADR);
  controlMap->Add("KaonZeroLInelastic", kHADR);
  controlMap->Add("kaon0LInelastic", kHADR);
  controlMap->Add("KaonZeroSInelastic", kHADR);
  controlMap->Add("kaon0SInelastic", kHADR);
  controlMap->Add("LambdaInelastic", kHADR);
  controlMap->Add("lambdaInelastic", kHADR);
  controlMap->Add("anti-lambdaInelastic", kHADR);
  controlMap->Add("ProtonInelastic", kHADR);
  controlMap->Add("protonInelastic", kHADR);
  controlMap->Add("AntiProtonInelastic", kHADR);
  controlMap->Add("anti_protonInelastic", kHADR);
  controlMap->Add("NeutronInelastic", kHADR);
  controlMap->Add("neutronInelastic", kHADR);
  controlMap->Add("AntiNeutronInelastic", kHADR);
  controlMap->Add("anti_neutronInelastic", kHADR);
  controlMap->Add("LambdaInelastic", kHADR);
  controlMap->Add("AntiLambdaInelastic", kHADR);
  controlMap->Add("anti_lambdaInelastic", kHADR);
  controlMap->Add("SigmaMinusInelastic", kHADR);
  controlMap->Add("sigma-Inelastic", kHADR);
  controlMap->Add("AntiSigmaMinusInelastic", kHADR);
  controlMap->Add("anti_sigma-Inelastic", kHADR);
  controlMap->Add("SigmaPlusInelastic", kHADR);
  controlMap->Add("sigma+Inelastic", kHADR);
  controlMap->Add("AntiSigmaPlusInelastic", kHADR);
  controlMap->Add("anti_sigma+Inelastic", kHADR);
  controlMap->Add("sigma0Inelastic", kHADR);
  controlMap->Add("XiMinusInelastic", kHADR);
  controlMap->Add("xi-Inelastic", kHADR);
  controlMap->Add("AntiXiMinusInelastic", kHADR);
  controlMap->Add("anti_xi-Inelastic", kHADR);
  controlMap->Add("XiZeroInelastic", kHADR);
  controlMap->Add("xi0Inelastic", kHADR);
  controlMap->Add("AntiXiZeroInelastic", kHADR);
  controlMap->Add("anti_xi0Inelastic", kHADR);
  controlMap->Add("OmegaMinusInelastic", kHADR);
  controlMap->Add("omega-Inelastic", kHADR);
  controlMap->Add("AntiOmegaMinusInelastic", kHADR);
  controlMap->Add("anti_omega-Inelastic", kHADR);
  controlMap->Add("DeuteronInelastic", kHADR);
  controlMap->Add("dInelastic", kHADR);
  controlMap->Add("AntiDeuteronInelastic", kHADR);
  controlMap->Add("anti_deuteronInelastic", kHADR);
  controlMap->Add("TritonInelastic", kHADR);
  controlMap->Add("tInelastic", kHADR);
  controlMap->Add("AntiTritonInelastic", kHADR);
  controlMap->Add("AntiTritonInelasticProcess", kHADR);
  controlMap->Add("anti_tritonInelastic", kHADR);
  controlMap->Add("AlphaInelastic", kHADR);
  controlMap->Add("alphaInelastic", kHADR);
  controlMap->Add("AntiAlphaInelasticProcess", kHADR);
  controlMap->Add("anti_alphaInelastic", kHADR);
  controlMap->Add("He3Inelastic", kHADR);
  controlMap->Add("AntiHe3InelasticProcess", kHADR);
  controlMap->Add("anti_He3Inelastic", kHADR);
  controlMap->Add("ionInelastic", kHADR);
  controlMap->Add("CHIPS_LowEnergyIonIonInelastic", kHADR);
  controlMap->Add("CHIPS_IonIonElasticScattering", kHADR);
  controlMap->Add("PhotonInelastic", kHADR);
  controlMap->Add("hInelastic", kHADR);
  controlMap->Add("CHIPS_Inelastic", kHADR);

  controlMap->Add("nKiller", kHADR);

  controlMap->Add("muNucl", kMUNU);
  controlMap->Add("muonNuclear", kMUNU);
  controlMap->Add("muMinusCaptureAtRest", kMUNU);
  controlMap->Add("PositronNuclear", kNoG3Controls);
  controlMap->Add("positronNuclear", kNoG3Controls);
  controlMap->Add("ElectroNuclear", kNoG3Controls);
  controlMap->Add("electronNuclear", kNoG3Controls);
  controlMap->Add("photoNuclear", kNoG3Controls);
  controlMap->Add("photonNuclear", kNoG3Controls);

  controlMap->Add("Cerenkov", kCKOV);
  controlMap->Add("Scintillation", kNoG3Controls);
  controlMap->Add("OpAbsorption", kLABS);
  controlMap->Add("OpRayleigh", kRAYL);
  controlMap->Add("Rayl", kRAYL);
  controlMap->Add("OpBoundary", kLABS);
  controlMap->Add("OpMieHG", kNoG3Controls);
  controlMap->Add("OpWLS", kNoG3Controls);
  controlMap->Add("GammaXTRadiator", kNoG3Controls);
  controlMap->Add("StrawXTRadiator", kNoG3Controls);
  controlMap->Add("RegularXTRadiator", kNoG3Controls);

  controlMap->Add("SynRad", kSYNC);
  controlMap->Add("CHIPS_SynchrotronRadiation", kSYNC);

  controlMap->Add("biasWrapper(0)", kNoG3Controls);
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4ProcessControlMapPhysics::ConstructParticle()
{
  /// Instantiate particles - nothing to be done here
}

//_____________________________________________________________________________
void TG4ProcessControlMapPhysics::ConstructProcess()
{
  /// Loop over all particles and their processes and check if
  /// the process is present in the map

  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  G4bool success = true;

  auto aParticleIterator = GetParticleIterator();
  aParticleIterator->reset();
  while ((*aParticleIterator)()) {
    // skip iteration if particle does not have a process manager
    if (!aParticleIterator->value()->GetProcessManager()) continue;

    G4ProcessVector* processVector =
      aParticleIterator->value()->GetProcessManager()->GetProcessList();

    for (G4int i = 0; i < processVector->length(); i++) {

      G4String processName = (*processVector)[i]->GetProcessName();

      if (controlMap->GetControl(processName) == kNoG3Controls &&
          processName != "Transportation" &&
          processName != "MonopoleTransportation" &&
          processName != "PositronNuclear" &&
          processName != "positronNuclear" && processName != "ElectroNuclear" &&
          processName != "electronNuclear" && processName != "photoNuclear" &&
          processName != "photonNuclear" && processName != "Scintillation" &&
          processName != "OpMieHG" && processName != "OpWLS" &&
          processName != "MinEkineCuts" && processName != "G4MinEkineCuts" &&
          processName != "MaxTimeCuts" && processName != "stackPopper" &&
          processName != "StepLimiter" && processName != "GammaXTRadiator" &&
          processName != "StrawXTRadiator" &&
          processName != "RegularXTRadiator" &&
          processName != "G4MaxTimeCuts" && processName != "biasWrapper(0)") {

        G4String text = "Unknown process control for ";
        text += processName;
        TG4Globals::Warning(
          "TG4ProcessControlMapPhysics", "ConstructProcess", text);
        success = false;
      }
    }
  }

  if (VerboseLevel() > 0) {
    if (success)
      G4cout << "### Processes mapped to VMC controls ok." << G4endl;
    else
      G4cout << "### Processes mapped to VMC controls: some proceess unknown."
             << G4endl;
  }
}
