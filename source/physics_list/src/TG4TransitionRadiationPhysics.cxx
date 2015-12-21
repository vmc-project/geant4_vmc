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

//_____________________________________________________________________________
G4ThreadLocal 
std::vector<G4VXTRenergyLoss*> TG4TransitionRadiationPhysics::fXtrProcesses;

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
/// Desctructor
}

//
// private methods
//

//_____________________________________________________________________________
G4VXTRenergyLoss* 
TG4TransitionRadiationPhysics::CreateXTRProcess(TG4RadiatorDescription* radiatorDescription)
{
/// Create XTR process for the given radiatorDescription

  G4String xtrModel = radiatorDescription->GetXtrModel();
  G4String volumeName = radiatorDescription->GetVolumeName();
  G4String foilMaterialName = radiatorDescription->GetFoilMaterialName();
  G4String gasMaterialName = radiatorDescription->GetGasMaterialName();
  G4String strawTubeMaterialName = radiatorDescription->GetStrawTubeMaterialName();

  G4LogicalVolume* logicalVolume
    = TG4GeometryServices::Instance()->FindLogicalVolume(volumeName, true);
  if ( ! logicalVolume ) {
   TString text = "Volume ";
   text +=  volumeName.data();
   text += " does not exist";
   TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess", text);
   return 0;
  }

  G4Material* foilMaterial = G4Material::GetMaterial(foilMaterialName);
  if ( ! foilMaterial ) {
   TString text = "Foil material ";
   text +=  foilMaterialName.data();
   text += " does not exist";
   TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess", text);
   return 0;
  }

  G4Material* gasMaterial = G4Material::GetMaterial(gasMaterialName);
  if ( ! gasMaterial ) {
   TString text = "Gas material ";
   text +=  foilMaterialName.data();
   text += " does not exist";
   TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess", text);
   return 0;
  }

  G4Material* strawTubeMaterial = 0;
  if ( strawTubeMaterialName.size() ) {
    strawTubeMaterial = G4Material::GetMaterial(strawTubeMaterialName);
    if ( ! strawTubeMaterial ) {
      TString text = "Straw tube material ";
      text +=  strawTubeMaterialName.data();
      text += " does not exist";
      TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess", text);
      return 0;
    }
  }

  // Print materials used in radiator
  G4cout << "Foil material: " << G4endl;
  G4cout << *foilMaterial << G4endl;

  G4cout << "Gas material: " << G4endl;
  G4cout << *gasMaterial << G4endl;

  // Get remaining parameters
  G4double foilThickness = radiatorDescription->GetFoilThickness();
  G4double gasThickness = radiatorDescription->GetGasThickness();
  G4int foilNumber = radiatorDescription->GetFoilNumber();

  if ( xtrModel == "gammaR" ) {      
    return new G4GammaXTRadiator(
                 logicalVolume, 100., 100.,
                 foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                 "GammaXTRadiator");
  }
  else if ( xtrModel == "gammaM" ) {
    return new G4XTRGammaRadModel(
                 logicalVolume, 100., 100.,
                 foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                 "GammaXTRadiator");
  }
  else if ( xtrModel == "strawR" ) {
    return new G4StrawTubeXTRadiator(
                 logicalVolume,
                 foilMaterial, gasMaterial, 0.53, 3.14159, strawTubeMaterial,
                 true, "StrawXTRadiator");
  }
  else if ( xtrModel == "regR" ) {      
    return new G4RegularXTRadiator(
                 logicalVolume,
                 foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                 "RegularXTRadiator");            
  }
  else if ( xtrModel == "transpR" ) {
    return new G4TransparentRegXTRadiator(
                 logicalVolume,
                 foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                 "RegularXTRadiator");
  }
  else if ( xtrModel == "regM" ) {
    return new G4XTRRegularRadModel(
                 logicalVolume,
                 foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber,
                 "RegularXTRadiator");
  } 
  else {     
    TString text = "XTR model <";
    text +=  xtrModel.data();
    text += " is not defined.";
    TG4Globals::Warning("TG4TransitionRadiationPhysics", "CreateXTRProcess", text);
    return 0;
  }
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

  for (G4int i=0; i<G4int(radiators.size()); ++i) {

    // create XTR process
    G4VXTRenergyLoss*  xtrProcess = CreateXTRProcess(radiators[i]);
    if ( ! xtrProcess ) continue;

    if (VerboseLevel() > 1) {
      G4cout << "Constructed XTR process " << radiators[i]->GetXtrModel() 
             << " in radiator " << radiators[i]->GetVolumeName() << G4endl;
    }  

    fXtrProcesses.push_back(xtrProcess);
    xtrProcess->SetVerboseLevel(VerboseLevel()); 
      // CHECK if better to decrement this

    // set process to e-, e+
    G4Electron::Electron()->GetProcessManager()->AddDiscreteProcess(xtrProcess);
    G4Positron::Positron()->GetProcessManager()->AddDiscreteProcess(xtrProcess);
  }

  if (VerboseLevel() > 0) {
    G4cout << "### Transition radiation physics constructed." << G4endl;
  }
}

//_____________________________________________________________________________

