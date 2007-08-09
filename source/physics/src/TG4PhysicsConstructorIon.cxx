// $Id: TG4PhysicsConstructorIon.cxx,v 1.7 2007/05/31 10:24:33 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics
// Class TG4PhysicsConstructorIon
// ------------------------------
// See the class description in the header file.
// According to ExN04IonPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00

#include "TG4PhysicsConstructorIon.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include "G4IonConstructor.hh"

#include <G4HadronElasticProcess.hh>
#include <G4LElastic.hh>
#include <G4DeuteronInelasticProcess.hh>
#include <G4LEDeuteronInelastic.hh>
#include <G4TritonInelasticProcess.hh>
#include <G4LETritonInelastic.hh>
#include <G4AlphaInelasticProcess.hh>
#include <G4LEAlphaInelastic.hh>
#include <G4hMultipleScattering.hh>
#include <G4ionIonisation.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorIon::TG4PhysicsConstructorIon(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fElasticProcess(0),
    fElasticModel(0),
    fIonMultipleScattering(0),
    fIonIonisation(0),
    fDeuteronMultipleScattering(0),
    fDeuteronIonisation(0),
    fDeuteronProcess(0),
    fDeuteronModel(0),
    fTritonMultipleScattering(0),
    fTritonIonisation(0),
    fTritonProcess(0),
    fTritonModel(0),
    fAlphaMultipleScattering(0),
    fAlphaIonisation(0),
    fAlphaProcess(0),
    fAlphaModel(0),
    fHe3MultipleScattering(0),
    fHe3Ionisation(0),
    fSetEM(true),
    fSetHadron(true) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorIon::TG4PhysicsConstructorIon(G4int verboseLevel,
                                                   G4bool setEM,
                                                   G4bool setHadron,
                                                   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fElasticProcess(0),
    fElasticModel(0),
    fIonMultipleScattering(0),
    fIonIonisation(0),
    fDeuteronMultipleScattering(0),
    fDeuteronIonisation(0),
    fDeuteronProcess(0),
    fDeuteronModel(0),
    fTritonMultipleScattering(0),
    fTritonIonisation(0),
    fTritonProcess(0),
    fTritonModel(0),
    fAlphaMultipleScattering(0),
    fAlphaIonisation(0),
    fAlphaProcess(0),
    fAlphaModel(0),
    fHe3MultipleScattering(0),
    fHe3Ionisation(0),
    fSetEM(setEM),
    fSetHadron(setHadron) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorIon::~TG4PhysicsConstructorIon() 
{
//
  delete fElasticProcess;
  delete fElasticModel;
  delete fIonMultipleScattering;
  delete fIonIonisation;
  delete fDeuteronMultipleScattering;
  delete fDeuteronIonisation;
  delete fDeuteronProcess;
  delete fDeuteronModel;
  delete fTritonMultipleScattering;
  delete fTritonIonisation;
  delete fTritonProcess;
  delete fTritonModel;
  delete fAlphaMultipleScattering;
  delete fAlphaIonisation;
  delete fAlphaProcess;
  delete fAlphaModel;
  delete fHe3MultipleScattering;
  delete fHe3Ionisation;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForGenericIon()
{
/// Construct electromagnetic processes for generic ion.

  // add process
  G4ProcessManager* pManager = G4GenericIon::GenericIon()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fElasticProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fElasticProcess, kPHElastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForDeuteron()
{
/// Construct electromagnetic processes for deuteron.

  // add process
  G4ProcessManager* pManager = G4Deuteron::Deuteron()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fDeuteronModel = new G4LEDeuteronInelastic();
  fDeuteronProcess = new G4DeuteronInelasticProcess();
  fDeuteronProcess->RegisterMe(fDeuteronModel);
  pManager->AddDiscreteProcess(fDeuteronProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fDeuteronProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fDeuteronProcess, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForTriton()
{
/// Construct electromagnetic processes for triton.

  // add process
  G4ProcessManager* pManager = G4Triton::Triton()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fTritonModel = new G4LETritonInelastic();
  fTritonProcess = new G4TritonInelasticProcess();
  fTritonProcess->RegisterMe(fTritonModel);
  pManager->AddDiscreteProcess(fTritonProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fTritonProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fTritonProcess, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForAlpha()
{
/// Construct electromagnetic processes for alpha.

  // add process
  G4ProcessManager* pManager = G4Alpha::Alpha()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fAlphaModel = new G4LEAlphaInelastic();
  fAlphaProcess = new G4AlphaInelasticProcess();
  fAlphaProcess->RegisterMe(fAlphaModel);
  pManager->AddDiscreteProcess(fAlphaProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAlphaProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAlphaProcess, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForHe3()
{
/// Construct electromagnetic processes for He3.

  // add process
  G4ProcessManager* pManager = G4He3::He3()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForGenericIon()
{
/// Construct electromagnetic processes for generic ion.

  // create process
  fIonMultipleScattering = new G4hMultipleScattering();
  fIonIonisation = new G4ionIonisation();

  // add process
  G4ProcessManager* pManager = G4GenericIon::GenericIon()->GetProcessManager();
  pManager->AddProcess(fIonMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fIonIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fIonMultipleScattering, kMULS); 
  controlMap->Add(fIonIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fIonMultipleScattering, kPMultipleScattering); 
  mcMap->Add(fIonIonisation, kPEnergyLoss); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForDeuteron()
{
/// Construct electromagnetic processes for deuteron.

  // create process
  fDeuteronMultipleScattering = new G4hMultipleScattering();
  fDeuteronIonisation = new G4ionIonisation();

  // add process
  G4ProcessManager* pManager = G4Deuteron::Deuteron()->GetProcessManager();
  pManager->AddProcess(fDeuteronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fDeuteronIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fDeuteronIonisation, kLOSS); 
  controlMap->Add(fDeuteronMultipleScattering, kMULS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fDeuteronIonisation, kPEnergyLoss); 
  mcMap->Add(fDeuteronMultipleScattering, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForTriton()
{
/// Construct electromagnetic processes for triton.

  // create process
  fTritonMultipleScattering = new G4hMultipleScattering();
  fTritonIonisation = new G4ionIonisation();

  // add process
  G4ProcessManager* pManager = G4Triton::Triton()->GetProcessManager();
  pManager->AddProcess(fTritonMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fTritonIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fTritonMultipleScattering, kMULS); 
  controlMap->Add(fTritonIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fTritonMultipleScattering, kPMultipleScattering); 
  mcMap->Add(fTritonIonisation, kPEnergyLoss); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForAlpha()
{
/// Construct electromagnetic processes for alpha.

  // create process
  fAlphaMultipleScattering = new G4hMultipleScattering();
  fAlphaIonisation = new G4ionIonisation();

  // add process
  G4ProcessManager* pManager = G4Alpha::Alpha()->GetProcessManager();
  pManager->AddProcess(fAlphaMultipleScattering, -1, 1, 1);
  pManager->AddProcess(fAlphaIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAlphaMultipleScattering, kMULS); 
  controlMap->Add(fAlphaIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAlphaMultipleScattering, kPMultipleScattering); 
  mcMap->Add(fAlphaIonisation, kPEnergyLoss); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForHe3()
{
/// Construct electromagnetic processes for He3.

  // create process
  fHe3MultipleScattering = new G4hMultipleScattering();
  fHe3Ionisation = new G4ionIonisation();

  // add process
  G4ProcessManager* pManager = G4He3::He3()->GetProcessManager();
  pManager->AddProcess(fHe3MultipleScattering, -1, 1, 1);
  pManager->AddProcess(fHe3Ionisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fHe3MultipleScattering, kMULS); 
  controlMap->Add(fHe3Ionisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fHe3MultipleScattering, kPMultipleScattering); 
  mcMap->Add(fHe3Ionisation, kPEnergyLoss); 
}

//
// protected methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructParticle()
{
/// Instantiate particles.

  //  Construct light ions
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();  
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructProcess()
{
/// Construct processes for ions.

  if (fSetHadron) {
    // Elastic Process
    fElasticModel = new G4LElastic();
    fElasticProcess = new G4HadronElasticProcess();
    fElasticProcess->RegisterMe(fElasticModel);

    // Hadron processes
    ConstructHadProcessForGenericIon();
    ConstructHadProcessForDeuteron();
    ConstructHadProcessForTriton();
    ConstructHadProcessForAlpha();
    ConstructHadProcessForHe3();
  }  

  if (fSetEM) {
    // EM processes
    ConstructEMProcessForGenericIon();
    ConstructEMProcessForDeuteron();
    ConstructEMProcessForTriton();
    ConstructEMProcessForAlpha();
    ConstructEMProcessForHe3();
  }  

  if (VerboseLevel() > 0) {
    if (fSetEM)     G4cout << "### EM physics for ions constructed." << G4endl;
    if (fSetHadron) G4cout << "### Hadron physics for ions constructed." << G4endl;
  }  
}
