// $Id: TG4PhysicsConstructorIon.cxx,v 1.3 2003/12/18 13:27:46 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorIon
// ------------------------------
// See the class description in the header file.
// According to ExN04IonPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp 
// GEANT4 tag Name: geant4-06-00
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorIon.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include "G4IonConstructor.hh"

//_____________________________________________________________________________
TG4PhysicsConstructorIon::TG4PhysicsConstructorIon(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fSetEM(true),
    fSetHadron(true) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorIon::TG4PhysicsConstructorIon(G4int verboseLevel,
                                                   G4bool setEM,
						   G4bool setHadron,
						   const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fSetEM(setEM),
    fSetHadron(setHadron) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorIon::TG4PhysicsConstructorIon(
                                     const TG4PhysicsConstructorIon& right)
{
//
  TG4Globals::Exception("TG4PhysicsConstructorIon is protected from copying.");
}

//_____________________________________________________________________________
TG4PhysicsConstructorIon::~TG4PhysicsConstructorIon() {
//
}

//
// operators
//

//_____________________________________________________________________________
TG4PhysicsConstructorIon& 
TG4PhysicsConstructorIon::operator=(const TG4PhysicsConstructorIon &right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception(
    "TG4PhysicsConstructorIon is protected from assigning.");

  return *this;
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
  pManager->AddDiscreteProcess(&fElasticProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fElasticProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fElasticProcess, kPHElastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForDeuteron()
{
/// Construct electromagnetic processes for deuteron.

  // add process
  G4ProcessManager* pManager = G4Deuteron::Deuteron()->GetProcessManager();
  pManager->AddDiscreteProcess(&fElasticProcess);

  fDeuteronModel = new G4LEDeuteronInelastic();
  fDeuteronProcess.RegisterMe(fDeuteronModel);
  pManager->AddDiscreteProcess(&fDeuteronProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fDeuteronProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fDeuteronProcess, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForTriton()
{
/// Construct electromagnetic processes for triton.

  // add process
  G4ProcessManager* pManager = G4Triton::Triton()->GetProcessManager();
  pManager->AddDiscreteProcess(&fElasticProcess);

  fTritonModel = new G4LETritonInelastic();
  fTritonProcess.RegisterMe(fTritonModel);
  pManager->AddDiscreteProcess(&fTritonProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fTritonProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fTritonProcess, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForAlpha()
{
/// Construct electromagnetic processes for alpha.

  // add process
  G4ProcessManager* pManager = G4Alpha::Alpha()->GetProcessManager();
  pManager->AddDiscreteProcess(&fElasticProcess);

  fAlphaModel = new G4LEAlphaInelastic();
  fAlphaProcess.RegisterMe(fAlphaModel);
  pManager->AddDiscreteProcess(&fAlphaProcess);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fAlphaProcess, kHADR); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fAlphaProcess, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructHadProcessForHe3()
{
/// Construct electromagnetic processes for He3.

  // add process
  G4ProcessManager* pManager = G4He3::He3()->GetProcessManager();
  pManager->AddDiscreteProcess(&fElasticProcess);
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForGenericIon()
{
/// Construct electromagnetic processes for generic ion.

  // add process
  G4ProcessManager* pManager = G4GenericIon::GenericIon()->GetProcessManager();
  pManager->AddProcess(&fIonMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fIonIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fIonMultipleScattering, kMULS); 
  controlMap->Add(&fIonIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fIonMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fIonIonisation, kPEnergyLoss); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForDeuteron()
{
/// Construct electromagnetic processes for deuteron.

  // add process
  G4ProcessManager* pManager = G4Deuteron::Deuteron()->GetProcessManager();
  pManager->AddProcess(&fDeuteronMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fDeuteronIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fDeuteronIonisation, kLOSS); 
  controlMap->Add(&fDeuteronMultipleScattering, kMULS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fDeuteronIonisation, kPEnergyLoss); 
  mcMap->Add(&fDeuteronMultipleScattering, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForTriton()
{
/// Construct electromagnetic processes for triton.

  // add process
  G4ProcessManager* pManager = G4Triton::Triton()->GetProcessManager();
  pManager->AddProcess(&fTritonMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fTritonIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fTritonMultipleScattering, kMULS); 
  controlMap->Add(&fTritonIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fTritonMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fTritonIonisation, kPEnergyLoss); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForAlpha()
{
/// Construct electromagnetic processes for alpha.

  // add process
  G4ProcessManager* pManager = G4Alpha::Alpha()->GetProcessManager();
  pManager->AddProcess(&fAlphaMultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fAlphaIonisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fAlphaMultipleScattering, kMULS); 
  controlMap->Add(&fAlphaIonisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fAlphaMultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fAlphaIonisation, kPEnergyLoss); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorIon::ConstructEMProcessForHe3()
{
/// Construct electromagnetic processes for He3.

  // add process
  G4ProcessManager* pManager = G4He3::He3()->GetProcessManager();
  pManager->AddProcess(&fHe3MultipleScattering, -1, 1, 1);
  pManager->AddProcess(&fHe3Ionisation,         -1, 2, 2);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(&fHe3MultipleScattering, kMULS); 
  controlMap->Add(&fHe3Ionisation, kLOSS); 

  // map to TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(&fHe3MultipleScattering, kPMultipleScattering); 
  mcMap->Add(&fHe3Ionisation, kPEnergyLoss); 
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
    fElasticProcess.RegisterMe(fElasticModel);

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
