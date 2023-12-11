//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RunConfiguration.cxx
/// \brief Implementation of the TG4RunConfiguration class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4RunConfiguration.h"
#include "TG4ComposedPhysicsList.h"
#include "TG4DetConstruction.h"
#include "TG4EmPhysicsList.h"
#include "TG4EventAction.h"
#include "TG4ExtraPhysicsList.h"
#include "TG4Globals.h"
#include "TG4HadronPhysicsList.h"
#include "TG4PrimaryGeneratorAction.h"
#include "TG4RunAction.h"
#include "TG4SpecialPhysicsList.h"
#include "TG4SpecialStackingAction.h"
#include "TG4SteppingAction.h"
#include "TG4TrackingAction.h"
#include "TG4VUserRegionConstruction.h"

#include <G4UImessenger.hh>
//#include <G4PhysListFactory.hh>

#ifdef USE_VGM
#include "TG4VGMMessenger.h"
#include <XmlVGM/AGDDExporter.h>
#include <XmlVGM/GDMLExporter.h>
#endif

//_____________________________________________________________________________
TG4RunConfiguration::TG4RunConfiguration(const TString& userGeometry,
  const TString& physicsList, const TString& specialProcess,
  Bool_t specialStacking, Bool_t mtApplication)
  : fUserGeometry(userGeometry),
    fPhysicsListSelection(physicsList),
    fSpecialProcessSelection(),
    fSpecialStacking(specialStacking),
    fMTApplication(mtApplication),
    fSpecialControls(false),
    fSpecialCuts(false),
    fSpecialCutsOld(false),
    fAGDDMessenger(0),
    fGDMLMessenger(0),
    fParameters()
{
  /// Standard constructor

  if (userGeometry != "geomVMCtoGeant4" && userGeometry != "geomVMCtoRoot" &&
      userGeometry != "geomRoot" && userGeometry != "geomRootToGeant4" &&
      userGeometry != "geomVMC+RootToGeant4" && userGeometry != "geomGeant4") {

    TG4Globals::Exception("TG4RunConfiguration", "TG4RunConfiguration",
      "User geometry " + userGeometry + " not recognized." +
        TG4Globals::Endl() + "Available options:" + TG4Globals::Endl() +
        "geomVMCtoGeant4 geomVMCtoRoot geomRoot geomRootToGeant4 "
        "geomVMC+RootToGeant4 geomGeant4");
  }

  G4int itoken = 0;
  G4String token;
  do {
    token = TG4Globals::GetToken(itoken++, physicsList);

    if (!TG4EmPhysicsList::IsAvailableSelection(token) &&
        !TG4HadronPhysicsList::IsAvailableSelection(token) &&
        !TG4ExtraPhysicsList::IsAvailableSelection(token)) {

      TG4Globals::Exception("TG4RunConfiguration", "TG4RunConfiguration",
        "Physics list selection " + physicsList + " not recognized." +
          TG4Globals::Endl() + "Available options:" + TG4Globals::Endl() +
          "EMonly, EMonly+Extra, Hadron_EM, Hadron_EM+Extra" +
          TG4Globals::Endl() + "  where EMonly = " +
          TString(TG4EmPhysicsList::AvailableSelections()) +
          TG4Globals::Endl() + "        Hadron = " +
          TString(TG4HadronPhysicsList::AvailableHadronSelections()) +
          TG4Globals::Endl() + "        EM = " +
          TString(TG4HadronPhysicsList::AvailableEMSelections()) +
          TG4Globals::Endl() + "        Extra = " +
          TString(TG4ExtraPhysicsList::AvailableSelections()) +
          TG4Globals::Endl() +
          "  The Extra selections are cumulative, while Hadron selections are "
          "exlusive." +
          TG4Globals::Endl());
    }
  } while (token != "");

  G4String g4SpecialProcess(specialProcess.Data());

  if (G4StrUtil::contains(g4SpecialProcess, "specialControls")) {
    fSpecialControls = true;
    // remove "specialControls" from the string passsed to special physics list
    g4SpecialProcess.erase(g4SpecialProcess.find("specialControls"), 15);
    if (g4SpecialProcess.find("++") != std::string::npos)
      g4SpecialProcess.erase(g4SpecialProcess.find("++"), 1);
  }
  fSpecialProcessSelection = g4SpecialProcess;

  if (G4StrUtil::contains(g4SpecialProcess, "specialCuts")) {
    fSpecialCuts = true;
  }

  if (!TG4SpecialPhysicsList::IsAvailableSelection(g4SpecialProcess)) {

    TG4Globals::Exception("TG4RunConfiguration", "TG4RunConfiguration",
      "Special process selection " + specialProcess + " is not recognized." +
        TG4Globals::Endl() + "Available options:" + TG4Globals::Endl() +
        TString(TG4SpecialPhysicsList::AvailableSelections()));
  }

#ifdef USE_VGM
  // instantiate XML messengers
  fAGDDMessenger = new TG4VGMMessenger("AGDD", userGeometry.Data());
  fGDMLMessenger = new TG4VGMMessenger("GDML", userGeometry.Data());
#endif
}

//_____________________________________________________________________________
TG4RunConfiguration::~TG4RunConfiguration()
{
  /// Destructor

  delete fAGDDMessenger;
  delete fGDMLMessenger;
}

//
// public methods
//

//_____________________________________________________________________________
G4VUserDetectorConstruction* TG4RunConfiguration::CreateDetectorConstruction()
{
  /// Create and return Geant4 VMC default detector construction

  if (fUserGeometry == "Root") return 0;

  return new TG4DetConstruction();
}

//_____________________________________________________________________________
G4VUserPhysicsList* TG4RunConfiguration::CreatePhysicsList()
{
  /// Create default Geant4 VMC physics list

  TG4ComposedPhysicsList* builder = new TG4ComposedPhysicsList();

  // Decompose physics list selection
  G4int itoken = 0;
  G4String emSelection;
  G4String hadronSelection;
  G4String extraSelection;
  G4String token;
  G4bool isValid = true;
  while (
    (token = TG4Globals::GetToken(itoken++, fPhysicsListSelection)) != "") {
    // The first token must be either EM or Hadronic physics list
    if (itoken == 1) {
      if (TG4EmPhysicsList::IsAvailableSelection(token)) {
        emSelection = token;
      }
      else if (TG4HadronPhysicsList::IsAvailableSelection(token)) {
        hadronSelection = token;
      }
      else {
        isValid = false;
      }
    }
    // The next tokens are Extra physics selections
    else {
      if (TG4ExtraPhysicsList::IsAvailableSelection(token)) {
        extraSelection += token;
        extraSelection += " ";
      }
      else {
        isValid = false;
      }
    }
  }

  if (!isValid) {
    TG4Globals::Exception("TG4RunConfiguration", "TG4RunConfiguration",
      "Physics list selection " + fPhysicsListSelection + " is not valid." +
        TG4Globals::Endl() +
        "The EMonly selections cannot be combined with Hadron selections." +
        TG4Globals::Endl());
  }

  if (emSelection != "") {
    G4cout << "Adding EMPhysicsList " << emSelection << G4endl;
    builder->AddPhysicsList(new TG4EmPhysicsList(emSelection));
  }

  if (hadronSelection != "") {
    G4cout << "Adding HadronPhysicsList " << hadronSelection << G4endl;
    builder->AddPhysicsList(new TG4HadronPhysicsList(hadronSelection));
  }

  if (extraSelection != "") {
    G4cout << "Adding ExtraPhysicsList " << extraSelection << G4endl;
    builder->AddPhysicsList(
      new TG4ExtraPhysicsList(extraSelection, fParameters));
  }

  // add option here
  G4cout << "Adding SpecialPhysicsList " << fSpecialProcessSelection.Data()
         << G4endl;
  builder->AddPhysicsList(
    new TG4SpecialPhysicsList(fSpecialProcessSelection.Data()));

  return builder;
}

//_____________________________________________________________________________
G4VUserPrimaryGeneratorAction* TG4RunConfiguration::CreatePrimaryGenerator()
{
  /// Create and return Geant4 VMC default primary generator

  return new TG4PrimaryGeneratorAction();
}

//_____________________________________________________________________________
G4UserRunAction* TG4RunConfiguration::CreateRunAction()
{
  /// Create and return Geant4 VMC default run action

  return new TG4RunAction();
}

//_____________________________________________________________________________
G4UserEventAction* TG4RunConfiguration::CreateEventAction()
{
  /// Create and return Geant4 VMC default event action

  return new TG4EventAction();
}

//_____________________________________________________________________________
TG4TrackingAction* TG4RunConfiguration::CreateTrackingAction()
{
  /// Create and return Geant4 VMC default tracking action

  return new TG4TrackingAction();
}

//_____________________________________________________________________________
TG4SteppingAction* TG4RunConfiguration::CreateSteppingAction()
{
  /// Create and return Geant4 VMC default stepping action

  return new TG4SteppingAction();
}

//_____________________________________________________________________________
G4UserStackingAction* TG4RunConfiguration::CreateStackingAction()
{
  /// Create and return Geant4 VMC special stacking action

  if (fSpecialStacking) return new TG4SpecialStackingAction();

  return 0;
}

//_____________________________________________________________________________
TG4VUserRegionConstruction* TG4RunConfiguration::CreateUserRegionConstruction()
{
  /// No region construction is defined by default

  return 0;
}

//_____________________________________________________________________________
TG4VUserPostDetConstruction*
TG4RunConfiguration::CreateUserPostDetConstruction()
{
  /// No user post detector construction is defined by default

  return 0;
}

//_____________________________________________________________________________
TG4VUserFastSimulation* TG4RunConfiguration::CreateUserFastSimulation()
{
  /// No user fast simulation is defined by default

  return 0;
}

//_____________________________________________________________________________
void TG4RunConfiguration::SetMTApplication(Bool_t mtApplication)
{
  /// Select running application in MT mode, if available.

  fMTApplication = mtApplication;
}

//_____________________________________________________________________________
void TG4RunConfiguration::SetParameter(const TString& name, Double_t value)
{
  /// Set a special paremeter which can be then used later.
  /// Actually used for monopole properties:
  /// monopoleMass, monopoleElCharge, monopoleMagCharge

  std::map<TString, Double_t>::iterator it = fParameters.find(name);

  if (it != fParameters.end()) {
    it->second = value;
  }
  else {
    fParameters[name] = value;
  }
}

//_____________________________________________________________________________
void TG4RunConfiguration::SetSpecialCutsOld()
{
  /// Activate usage of old regions manager
  /// that sets production thresholds by ranges

  G4cout
    << "### Special cuts old activated: production cuts will be set by ranges."
    << G4endl;

  fSpecialCutsOld = true;
}

//_____________________________________________________________________________
TString TG4RunConfiguration::GetUserGeometry() const
{
  /// Return the way user geometry is built

  // strip out "geom"from the name
  TString userGeometry = fUserGeometry;
  userGeometry = userGeometry.Remove(0, 4);

  return userGeometry;
}

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsSpecialStacking() const
{
  /// Return true if special stacking is activated

  return fSpecialStacking;
}

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsSpecialControls() const
{
  /// Return true if special controls are activated

  return fSpecialControls;
}

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsSpecialCuts() const
{
  /// Return true if special cuts are activated

  return fSpecialCuts;
}

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsSpecialCutsOld() const
{
  /// Return true if special cuts old are activated

  return fSpecialCutsOld;
}

//_____________________________________________________________________________
Bool_t TG4RunConfiguration::IsMTApplication() const
{
  /// Return true if running in multi-threading mode is activated

  return fMTApplication;
}
