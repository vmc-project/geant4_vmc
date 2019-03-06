//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CrossSectionMessenger.cxx
/// \brief Implementation of the TG4CrossSectionMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4CrossSectionMessenger.h"
#include "TG4CrossSectionManager.h"
#include "TG4Globals.h"
#include "TG4UICmdWithAComplexString.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

//_____________________________________________________________________________
TG4CrossSectionMessenger::TG4CrossSectionMessenger(
                             TG4CrossSectionManager* crossSectionManager)
  : G4UImessenger(),
    fCrossSectionManager(crossSectionManager),
    fDirectory(0),
    fMakeHistogramsCmd(0),
    fParticleCmd(0),
    fElementCmd(0),
    fNofBinsECmd(0),
    fNofBinsPCmd(0),
    fMinKinECmd(0),
    fMaxKinECmd(0),
    fKinECmd(0),
    fMinMomentumCmd(0),
    fMaxMomentumCmd(0),
    fMomentumCmd(0),
    fLabelCmd(0),
    fPrintCmd(0)
{
/// Standard constructor

  fDirectory = new G4UIdirectory("/mcCrossSection/");
  fDirectory->SetGuidance("Cross section manager commands.");

  fMakeHistogramsCmd = new G4UIcmdWithABool("/mcCrossSection/makeHistograms", this);
  fMakeHistogramsCmd->SetGuidance("Activate creating histograms with cross sections");
  fMakeHistogramsCmd->SetParameterName("MakeHistograms", true);
  fMakeHistogramsCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fParticleCmd = new G4UIcmdWithAString("/mcCrossSection/setParticle", this);
  fParticleCmd->SetGuidance("Set particle name");
  fParticleCmd->SetParameterName("particleName", true);
  fParticleCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fElementCmd = new G4UIcmdWithAString("/mcCrossSection/setElement", this);
  fElementCmd->SetGuidance("Set chemical element name");
  fElementCmd->SetParameterName("particleName", true);
  fElementCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fNofBinsECmd = new G4UIcmdWithAnInteger("/mcCrossSection/setNofBinsE", this);
  fNofBinsECmd->SetGuidance("Set number of bins in E");
  fNofBinsECmd->SetParameterName("nofBinsE", false);
  fNofBinsECmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fNofBinsPCmd = new G4UIcmdWithAnInteger("/mcCrossSection/setNofBinsP", this);
  fNofBinsPCmd->SetGuidance("Set number of bins in P");
  fNofBinsPCmd->SetParameterName("nofBinsE", false);
  fNofBinsPCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fMinKinECmd = new G4UIcmdWithADoubleAndUnit("/mcCrossSection/setMinKinE", this);
  fMinKinECmd->SetGuidance("Set minimum kinetic energy");
  fMinKinECmd->SetParameterName("minKinE", false);
  fMinKinECmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fMaxKinECmd = new G4UIcmdWithADoubleAndUnit("/mcCrossSection/setMaxKinE", this);
  fMaxKinECmd->SetGuidance("Set minimum kinetic energy");
  fMaxKinECmd->SetParameterName("minKinE", false);
  fMaxKinECmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fKinECmd = new G4UIcmdWithADoubleAndUnit("/mcCrossSection/setKinE", this);
  fKinECmd->SetGuidance("Set current kinetic energy");
  fKinECmd->SetParameterName("minKinE", false);
  fKinECmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fMinMomentumCmd = new G4UIcmdWithADoubleAndUnit("/mcCrossSection/setMinMomentum", this);
  fMinMomentumCmd->SetGuidance("Set minimum momentum");
  fMinMomentumCmd->SetParameterName("minMomentum", false);
  fMinMomentumCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fMaxMomentumCmd = new G4UIcmdWithADoubleAndUnit("/mcCrossSection/setMaxMomentum", this);
  fMaxMomentumCmd->SetGuidance("Set maximum momentum");
  fMaxMomentumCmd->SetParameterName("maxMomentum", false);
  fMaxMomentumCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fMomentumCmd = new G4UIcmdWithADoubleAndUnit("/mcCrossSection/setMomentum", this);
  fMomentumCmd->SetGuidance("Set current momentum");
  fMomentumCmd->SetParameterName("Momentum", false);
  fMomentumCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fLabelCmd = new G4UIcmdWithAString("/mcCrossSection/setLabel", this);
  fLabelCmd->SetGuidance("Set label which will be put at the beginning of histograms title");
  fLabelCmd->SetParameterName("Label", true);
  fLabelCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);

  fPrintCmd = new G4UIcmdWithAString("/mcCrossSection/printCrossSection", this);
  fPrintCmd->SetGuidance("Print selected cross section for current kinetic energy");
  fPrintCmd->SetParameterName("crossSectionType", false);
  G4String candidates("All ");
  for ( G4int i=0; i<kNoCrossSectionType; i++ ) {
    candidates += TG4CrossSectionTypeName(i);
    candidates += G4String(" ");
  }
  fPrintCmd->SetCandidates(candidates);
  fPrintCmd->AvailableForStates(G4State_Idle);
}

//_____________________________________________________________________________
TG4CrossSectionMessenger::~TG4CrossSectionMessenger()
{
/// Destructor

  delete fDirectory;
  delete fMakeHistogramsCmd;
  delete fParticleCmd;
  delete fElementCmd;
  delete fNofBinsECmd;
  delete fNofBinsPCmd;
  delete fMinKinECmd;
  delete fMaxKinECmd;
  delete fKinECmd;
  delete fMinMomentumCmd;
  delete fMaxMomentumCmd;
  delete fLabelCmd;
  delete fMomentumCmd;
  delete fPrintCmd;
}

//
// public methods
//

//_____________________________________________________________________________
void TG4CrossSectionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
/// Apply command to the associated object.

  if ( command == fMakeHistogramsCmd ) {
    fCrossSectionManager->SetMakeHistograms(fMakeHistogramsCmd->GetNewBoolValue(newValue));
  }
  else if ( command == fParticleCmd ) {
    fCrossSectionManager->SetParticleName(newValue);
  }
  else if (command == fElementCmd) {
    fCrossSectionManager->SetElementName(newValue);
  }
  else if (command == fNofBinsECmd) {
    fCrossSectionManager->SetNumberOfBinsE(fNofBinsECmd->GetNewIntValue(newValue));
  }
  else if (command == fNofBinsPCmd) {
    fCrossSectionManager->SetNumberOfBinsP(fNofBinsPCmd->GetNewIntValue(newValue));
  }
  else if (command == fMinKinECmd) {
    fCrossSectionManager->SetMinKinEnergy(fMinKinECmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMaxKinECmd) {
    fCrossSectionManager->SetMaxKinEnergy(fMaxKinECmd->GetNewDoubleValue(newValue));
  }
  else if (command == fKinECmd) {
    fCrossSectionManager->SetKinEnergy(fKinECmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMinMomentumCmd) {
    fCrossSectionManager->SetMinMomentum(fMinMomentumCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMaxMomentumCmd) {
    fCrossSectionManager->SetMaxMomentum(fMaxMomentumCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fMomentumCmd) {
    fCrossSectionManager->SetMomentum(fMomentumCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fLabelCmd) {
    fCrossSectionManager->SetLabel(newValue);
  }
  else if (command == fPrintCmd) {
    if ( newValue == "All" )
      fCrossSectionManager->PrintCrossSections();
    else
      fCrossSectionManager->PrintCrossSection(GetCrossSectionType(newValue));
  }
}
