//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4TransitionRadiationPhysics.cxx
/// \brief Implementation of the TG4TransitionRadiationPhysics class
///
/// According to TG4TransitionRadiationPhysics from Geant4
/// extended/electromagnetic/TestEm10 example.
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4TransitionRadiationPhysics.h"
// #include "TG4TransitionRadiationMessenger.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4RadiatorDescription.h"

#include <G4LogicalVolumeStore.hh>
#include <G4VXTRenergyLoss.hh>
#include <G4ProcessManager.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>

#include <G4RegularXTRadiator.hh>
#include <G4TransparentRegXTRadiator.hh>
#include <G4GammaXTRadiator.hh>
#include <G4StrawTubeXTRadiator.hh>
#include <G4XTRGammaRadModel.hh>
#include <G4XTRRegularRadModel.hh>
#include <G4XTRTransparentRegRadModel.hh>
#include <G4AutoDelete.hh>

//_____________________________________________________________________________
G4ThreadLocal
std::vector<G4VXTRenergyLoss*>* TG4TransitionRadiationPhysics::fXtrProcesses = 0;

//_____________________________________________________________________________
TG4TransitionRadiationPhysics::TG4TransitionRadiationPhysics(const G4String& name)
  : TG4VPhysicsConstructor(name)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4TransitionRadiationPhysics::TG4TransitionRadiationPhysics(G4int theVerboseLevel,
                                             const G4String& name)
  : TG4VPhysicsConstructor(name, theVerboseLevel)
{
/// Standard constructor
}

//_____________________________________________________________________________
TG4TransitionRadiationPhysics::~TG4TransitionRadiationPhysics()
{
/// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
G4bool
TG4TransitionRadiationPhysics::CreateXTRProcess(TG4RadiatorDescription* radiatorDescription)
{
/// Create XTR process for the given radiatorDescription

  G4String xtrModel = radiatorDescription->GetXtrModel();

  auto foilLayer = radiatorDescription->GetLayer(0);
  auto gasLayer = radiatorDescription->GetLayer(1);
  auto strawTube = radiatorDescription->GetStrawTube();

  if (  xtrModel == "strawR" && ( ! std::get<0>(strawTube).size() ) ) {
    TString text = "Straw tube parameters are not defined.";
    TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess",
      TString("Straw tube parameters are not defined.")
      + TG4Globals::Endl()
      + TString("The XTR  process is not created."));
    return false;
  }

  G4Material* foilMaterial = G4Material::GetMaterial(std::get<0>(foilLayer));
  if ( ! foilMaterial ) {
   TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess",
     TString("Foil material ") + std::get<0>(foilLayer).data() + TString(" does not exist.")
      + TG4Globals::Endl()
      + TString("The XTR  process is not created."));
   return false;
  }

  G4Material* gasMaterial = G4Material::GetMaterial(std::get<0>(gasLayer));
  if ( ! gasMaterial ) {
   TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess",
     TString("Gas material ") + std::get<0>(gasLayer).data() + TString(" does not exist.")
      + TG4Globals::Endl()
      + TString("The XTR  process is not created."));
   return false;
  }

  G4Material* strawTubeMaterial = 0;
  if ( std::get<0>(strawTube).size() ) {
    strawTubeMaterial = G4Material::GetMaterial(std::get<0>(strawTube));
    if ( ! strawTubeMaterial ) {
      TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess",
        TString("Straw tube material ") + std::get<0>(strawTube).data() + TString(" does not exist.")
        + TG4Globals::Endl()
        + TString("The XTR  process is not created."));
      return false;
    }
  }

  // Get remaining parameters
  G4int foilNumber = radiatorDescription->GetFoilNumber();
  G4double foilThickness, foilFluctuation;
  G4double gasThickness, gasFluctuation;
  std::tie(std::ignore, foilThickness, foilFluctuation) = foilLayer;
  std::tie(std::ignore, gasThickness, gasFluctuation) = gasLayer;

  // Check if fluctuation parameters are defined when gamma models are selected
  if ( ( xtrModel == "gammaR" || xtrModel == "gammaM" )  &&
       ( foilFluctuation*gasFluctuation == 0. ) ) {
      TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess",
        TString("The gamma-distributed thickness parameter must be defined for both radiator layers")
          + TG4Globals::Endl()
          + TString("when gammaR or gammaM model is selected.")
          + TG4Globals::Endl()
          + TString("The XTR  process is not created."));
      return false;
  }

  G4bool isXtrProcess = false;
  G4String volumeName = radiatorDescription->GetVolumeName();
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  for (G4int i=0; i<G4int(lvStore->size()); ++i) {
    G4LogicalVolume* logicalVolume = (*lvStore)[i];

    if (logicalVolume->GetName() != volumeName) continue;

    G4VXTRenergyLoss* xtrProcess = 0;
    if ( xtrModel == "gammaR" ) {
      xtrProcess = new G4GammaXTRadiator(
                   logicalVolume, foilFluctuation, gasFluctuation,
                   foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                   "GammaXTRadiator");
    }
    else if ( xtrModel == "gammaM" ) {
      xtrProcess = new G4XTRGammaRadModel(
                   logicalVolume, foilFluctuation, gasFluctuation,
                   foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                   "GammaXTRadiator");
    }
    else if ( xtrModel == "strawR" ) {
      xtrProcess = new G4StrawTubeXTRadiator(
                   logicalVolume,
                   foilMaterial, gasMaterial,
                   std::get<1>(strawTube), std::get<2>(strawTube), strawTubeMaterial,
                   true, "StrawXTRadiator");
    }
    else if ( xtrModel == "regR" ) {
      xtrProcess = new G4RegularXTRadiator(
                   logicalVolume,
                   foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                   "RegularXTRadiator");
    }
    else if ( xtrModel == "transpR" ) {
      xtrProcess = new G4TransparentRegXTRadiator(
                   logicalVolume,
                   foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                   "RegularXTRadiator");
    }
    else if ( xtrModel == "regM" ) {
      xtrProcess = new G4XTRRegularRadModel(
                   logicalVolume,
                   foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                   "RegularXTRadiator");
    }
    else {
      TString text = "XTR model <";
      text +=  xtrModel.data();
      text += " is not defined.";
      TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess", text);
    }

    if ( xtrProcess ) {
      fXtrProcesses->push_back(xtrProcess);
      xtrProcess->SetVerboseLevel(VerboseLevel());
          // CHECK if better to decrement this

      // set process to e-, e+
      G4Electron::Electron()->GetProcessManager()->AddDiscreteProcess(xtrProcess);
      G4Positron::Positron()->GetProcessManager()->AddDiscreteProcess(xtrProcess);

      isXtrProcess = true;
    }
  }

  if ( ! isXtrProcess ) {
   TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess",
     TString("Volume ") + volumeName.data() + TString(" does not exist.")
      + TG4Globals::Endl()
      + TString("The XTR  process is not created."));
  }

  return isXtrProcess;
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4TransitionRadiationPhysics::ConstructParticle()
{
/// No particles instatiated

}

//_____________________________________________________________________________
void TG4TransitionRadiationPhysics::ConstructProcess()
{
  const std::vector<TG4RadiatorDescription*> radiators
    = TG4GeometryManager::Instance()->GetRadiators();

  // nothing to be done if no radiators are defined
  if ( ! radiators.size() ) return;

  if ( ! fXtrProcesses ) {
    fXtrProcesses = new std::vector<G4VXTRenergyLoss*>();
    G4AutoDelete::Register(fXtrProcesses);
  }

  for (G4int i=0; i<G4int(radiators.size()); ++i) {

    // create XTR process
    G4bool  isXtrProcess = CreateXTRProcess(radiators[i]);
    if ( ! isXtrProcess ) continue;

    if (VerboseLevel() > 1) {
      G4cout << "Constructed XTR process " << radiators[i]->GetXtrModel()
             << " in radiator " << radiators[i]->GetVolumeName() << G4endl;
    }
  }

  if (VerboseLevel() > 0) {
    G4cout << "### Transition radiation physics constructed." << G4endl;
  }
}

//_____________________________________________________________________________

