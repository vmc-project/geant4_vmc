// $Id: TG4PhysicsConstructorHadron.cxx,v 1.5 2006/01/13 16:59:39 brun Exp $
// Category: physics
//
// Class TG4PhysicsConstructorHadron
// ---------------------------------
// See the class description in the header file.
// According to ExN04HadronPhysics.cc,v 1.1.2.1 2001/06/28 19:07:37 gunter Exp
// GEANT4 tag Name: geant4-06-00
//
// Author: I. Hrivnacova

#include "TG4PhysicsConstructorHadron.h"
#include "TG4ProcessControlMap.h"
#include "TG4ProcessMCMap.h"
#include "TG4Globals.h"

#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4MesonConstructor.hh>
#include <G4BaryonConstructor.hh>
#include <G4ShortLivedConstructor.hh>

#include <G4MultipleScattering.hh>
#include <G4hIonisation.hh>

#include <G4HadronElasticProcess.hh>
#include <G4HadronFissionProcess.hh>
#include <G4HadronCaptureProcess.hh>

#include <G4PionPlusInelasticProcess.hh>
#include <G4PionMinusInelasticProcess.hh>
#include <G4KaonPlusInelasticProcess.hh>
#include <G4KaonZeroSInelasticProcess.hh>
#include <G4KaonZeroLInelasticProcess.hh>
#include <G4KaonMinusInelasticProcess.hh>
#include <G4ProtonInelasticProcess.hh>
#include <G4AntiProtonInelasticProcess.hh>
#include <G4NeutronInelasticProcess.hh>
#include <G4AntiNeutronInelasticProcess.hh>
#include <G4LambdaInelasticProcess.hh>
#include <G4AntiLambdaInelasticProcess.hh>
#include <G4SigmaPlusInelasticProcess.hh>
#include <G4SigmaMinusInelasticProcess.hh>
#include <G4AntiSigmaPlusInelasticProcess.hh>
#include <G4AntiSigmaMinusInelasticProcess.hh>
#include <G4XiZeroInelasticProcess.hh>
#include <G4XiMinusInelasticProcess.hh>
#include <G4AntiXiZeroInelasticProcess.hh>
#include <G4AntiXiMinusInelasticProcess.hh>
#include <G4DeuteronInelasticProcess.hh>
#include <G4TritonInelasticProcess.hh>
#include <G4AlphaInelasticProcess.hh>
#include <G4OmegaMinusInelasticProcess.hh>
#include <G4AntiOmegaMinusInelasticProcess.hh>

// Low-energy Models
#include <G4LElastic.hh>   
#include <G4LFission.hh>
#include <G4LCapture.hh>

#include <G4LEPionPlusInelastic.hh>
#include <G4LEPionMinusInelastic.hh>
#include <G4LEKaonPlusInelastic.hh>
#include <G4LEKaonZeroSInelastic.hh>
#include <G4LEKaonZeroLInelastic.hh>
#include <G4LEKaonMinusInelastic.hh>
#include <G4LEProtonInelastic.hh>
#include <G4LEAntiProtonInelastic.hh>
#include <G4LENeutronInelastic.hh>
#include <G4LEAntiNeutronInelastic.hh>
#include <G4LELambdaInelastic.hh>
#include <G4LEAntiLambdaInelastic.hh>
#include <G4LESigmaPlusInelastic.hh>
#include <G4LESigmaMinusInelastic.hh>
#include <G4LEAntiSigmaPlusInelastic.hh>
#include <G4LEAntiSigmaMinusInelastic.hh>
#include <G4LEXiZeroInelastic.hh>
#include <G4LEXiMinusInelastic.hh>
#include <G4LEAntiXiZeroInelastic.hh>
#include <G4LEAntiXiMinusInelastic.hh>
#include <G4LEDeuteronInelastic.hh>
#include <G4LETritonInelastic.hh>
#include <G4LEAlphaInelastic.hh>
#include <G4LEOmegaMinusInelastic.hh>
#include <G4LEAntiOmegaMinusInelastic.hh>

// High-energy Models

#include <G4HEPionPlusInelastic.hh>
#include <G4HEPionMinusInelastic.hh>
#include <G4HEKaonPlusInelastic.hh>
#include <G4HEKaonZeroInelastic.hh>
#include <G4HEKaonZeroInelastic.hh>
#include <G4HEKaonMinusInelastic.hh>
#include <G4HEProtonInelastic.hh>
#include <G4HEAntiProtonInelastic.hh>
#include <G4HENeutronInelastic.hh>
#include <G4HEAntiNeutronInelastic.hh>
#include <G4HELambdaInelastic.hh>
#include <G4HEAntiLambdaInelastic.hh>
#include <G4HESigmaPlusInelastic.hh>
#include <G4HESigmaMinusInelastic.hh>
#include <G4HEAntiSigmaPlusInelastic.hh>
#include <G4HEAntiSigmaMinusInelastic.hh>
#include <G4HEXiZeroInelastic.hh>
#include <G4HEXiMinusInelastic.hh>
#include <G4HEAntiXiZeroInelastic.hh>
#include <G4HEAntiXiMinusInelastic.hh>
#include <G4HEOmegaMinusInelastic.hh>
#include <G4HEAntiOmegaMinusInelastic.hh>

// Stopping processes
#include <G4AntiProtonAnnihilationAtRest.hh>
#include <G4AntiNeutronAnnihilationAtRest.hh>

#ifdef TRIUMF_STOP_PIMINUS
#include <G4PionMinusAbsorptionAtRest.hh>
#else
#include <G4PiMinusAbsorptionAtRest.hh>
#endif
#ifdef TRIUMF_STOP_KMINUS
#include <G4KaonMinusAbsorption.hh>
#else
#include <G4KaonMinusAbsorptionAtRest.hh>
#endif

// quark gluon string model with chips afterburner.
#include <G4TheoFSGenerator.hh>
#include <G4ExcitationHandler.hh>
#include <G4PreCompoundModel.hh>
#include <G4GeneratorPrecompoundInterface.hh>
#include <G4QGSModel.hh>
#include <G4QGSParticipants.hh>
#include <G4QGSMFragmentation.hh>
#include <G4ExcitedStringDecay.hh>

//_____________________________________________________________________________
TG4PhysicsConstructorHadron::TG4PhysicsConstructorHadron(const G4String& name)
  : TG4VPhysicsConstructor(name),
    fElasticProcess(0),
    fElasticModel(0),
    fPionPlusInelastic(0),
    fLEPionPlusModel(0),
    fPionPlusMult(0),
    fPionPlusIonisation(0),
    fPionMinusInelastic(0),
    fLEPionMinusModel(0),
    fPionMinusMult(0),         
    fPionMinusIonisation(0),
    fPionMinusAbsorption(0),
    fTheoModel(0), 
    fHandler(0),
    fPreEquilib(0),
    fCascade(0),           
    fStringModel(0),
    fFragmentation(0),
    fStringDecay(0),  
    fKaonPlusInelastic(0),
    fLEKaonPlusModel(0),
    fHEKaonPlusModel(0),
    fKaonPlusMult(0),
    fKaonPlusIonisation(0),
    fKaonMinusInelastic(0),
    fLEKaonMinusModel(0),  
    fHEKaonMinusModel(0),  
    fKaonMinusMult(0),        
    fKaonMinusIonisation(0),
    fKaonMinusAbsorption(0),
    fKaonZeroLInelastic(0),
    fLEKaonZeroLModel(0),  
    fHEKaonZeroLModel(0),  
    fKaonZeroSInelastic(0),
    fLEKaonZeroSModel(0),  
    fHEKaonZeroSModel(0),  
    fProtonInelastic(0),    
    fLEProtonModel(0),         
    fHEProtonModel(0),         
    fProtonMult(0),         
    fProtonIonisation(0),   
    fAntiProtonInelastic(0),
    fLEAntiProtonModel(0),  
    fHEAntiProtonModel(0),  
    fAntiProtonMult(0),         
    fAntiProtonIonisation(0),
    fAntiProtonAnnihilation(0),
    fNeutronInelastic(0),    
    fLENeutronModel(0),          
    fHENeutronModel(0),          
    fNeutronFission(0),          
    fNeutronFissionModel(0),
    fNeutronCapture(0),          
    fNeutronCaptureModel(0),
    fAntiNeutronInelastic(0),
    fLEAntiNeutronModel(0),
    fHEAntiNeutronModel(0),
    fAntiNeutronAnnihilation(0),
    fLambdaInelastic(0),
    fLELambdaModel(0),
    fHELambdaModel(0),
    fAntiLambdaInelastic(0),
    fLEAntiLambdaModel(0),  
    fHEAntiLambdaModel(0),  
    fSigmaMinusInelastic(0),
    fLESigmaMinusModel(0),  
    fHESigmaMinusModel(0),  
    fSigmaMinusMult(0),         
    fSigmaMinusIonisation(0),
    fAntiSigmaMinusInelastic(0),
    fLEAntiSigmaMinusModel(0),  
    fHEAntiSigmaMinusModel(0),  
    fAntiSigmaMinusMult(0),     
    fAntiSigmaMinusIonisation(0),
    fSigmaPlusInelastic(0),
    fLESigmaPlusModel(0),  
    fHESigmaPlusModel(0),  
    fSigmaPlusMult(0),        
    fSigmaPlusIonisation(0),
    fAntiSigmaPlusInelastic(0),
    fLEAntiSigmaPlusModel(0),  
    fHEAntiSigmaPlusModel(0),  
    fAntiSigmaPlusMult(0),     
    fAntiSigmaPlusIonisation(0),
    fXiZeroInelastic(0),
    fLEXiZeroModel(0),
    fHEXiZeroModel(0),
    fAntiXiZeroInelastic(0),
    fLEAntiXiZeroModel(0),  
    fHEAntiXiZeroModel(0),  
    fXiMinusInelastic(0),
    fLEXiMinusModel(0),
    fHEXiMinusModel(0),
    fXiMinusMult(0),
    fXiMinusIonisation(0),
    fAntiXiMinusInelastic(0),
    fLEAntiXiMinusModel(0),  
    fHEAntiXiMinusModel(0),  
    fAntiXiMinusMult(0),     
    fAntiXiMinusIonisation(0),
    fOmegaMinusInelastic(0),  
    fLEOmegaMinusModel(0),    
    fHEOmegaMinusModel(0),    
    fOmegaMinusMult(0),           
    fOmegaMinusIonisation(0), 
    fAntiOmegaMinusInelastic(0),
    fLEAntiOmegaMinusModel(0),  
    fHEAntiOmegaMinusModel(0),  
    fAntiOmegaMinusMult(0),     
    fAntiOmegaMinusIonisation(0), 
    fOtherProcesses(),
    fSetEM(true),
    fSetHadron(true) 
{
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorHadron::TG4PhysicsConstructorHadron(G4int verboseLevel,
                                                         G4bool setEM, 
                                                         G4bool setHadron,
                                                         const G4String& name)
  : TG4VPhysicsConstructor(name, verboseLevel),
    fElasticProcess(0),
    fElasticModel(0),
    fPionPlusInelastic(0),
    fLEPionPlusModel(0),
    fPionPlusMult(0),
    fPionPlusIonisation(0),
    fPionMinusInelastic(0),
    fLEPionMinusModel(0),
    fPionMinusMult(0),         
    fPionMinusIonisation(0),
    fPionMinusAbsorption(0),
    fTheoModel(0), 
    fHandler(0),
    fPreEquilib(0),
    fCascade(0),           
    fStringModel(0),
    fFragmentation(0),
    fStringDecay(0),  
    fKaonPlusInelastic(0),
    fLEKaonPlusModel(0),
    fHEKaonPlusModel(0),
    fKaonPlusMult(0),
    fKaonPlusIonisation(0),
    fKaonMinusInelastic(0),
    fLEKaonMinusModel(0),  
    fHEKaonMinusModel(0),  
    fKaonMinusMult(0),        
    fKaonMinusIonisation(0),
    fKaonMinusAbsorption(0),
    fKaonZeroLInelastic(0),
    fLEKaonZeroLModel(0),  
    fHEKaonZeroLModel(0),  
    fKaonZeroSInelastic(0),
    fLEKaonZeroSModel(0),  
    fHEKaonZeroSModel(0),  
    fProtonInelastic(0),    
    fLEProtonModel(0),         
    fHEProtonModel(0),         
    fProtonMult(0),         
    fProtonIonisation(0),   
    fAntiProtonInelastic(0),
    fLEAntiProtonModel(0),  
    fHEAntiProtonModel(0),  
    fAntiProtonMult(0),         
    fAntiProtonIonisation(0),
    fAntiProtonAnnihilation(0),
    fNeutronInelastic(0),    
    fLENeutronModel(0),          
    fHENeutronModel(0),          
    fNeutronFission(0),          
    fNeutronFissionModel(0),
    fNeutronCapture(0),          
    fNeutronCaptureModel(0),
    fAntiNeutronInelastic(0),
    fLEAntiNeutronModel(0),
    fHEAntiNeutronModel(0),
    fAntiNeutronAnnihilation(0),
    fLambdaInelastic(0),
    fLELambdaModel(0),
    fHELambdaModel(0),
    fAntiLambdaInelastic(0),
    fLEAntiLambdaModel(0),  
    fHEAntiLambdaModel(0),  
    fSigmaMinusInelastic(0),
    fLESigmaMinusModel(0),  
    fHESigmaMinusModel(0),  
    fSigmaMinusMult(0),         
    fSigmaMinusIonisation(0),
    fAntiSigmaMinusInelastic(0),
    fLEAntiSigmaMinusModel(0),  
    fHEAntiSigmaMinusModel(0),  
    fAntiSigmaMinusMult(0),     
    fAntiSigmaMinusIonisation(0),
    fSigmaPlusInelastic(0),
    fLESigmaPlusModel(0),  
    fHESigmaPlusModel(0),  
    fSigmaPlusMult(0),        
    fSigmaPlusIonisation(0),
    fAntiSigmaPlusInelastic(0),
    fLEAntiSigmaPlusModel(0),  
    fHEAntiSigmaPlusModel(0),  
    fAntiSigmaPlusMult(0),     
    fAntiSigmaPlusIonisation(0),
    fXiZeroInelastic(0),
    fLEXiZeroModel(0),
    fHEXiZeroModel(0),
    fAntiXiZeroInelastic(0),
    fLEAntiXiZeroModel(0),  
    fHEAntiXiZeroModel(0),  
    fXiMinusInelastic(0),
    fLEXiMinusModel(0),
    fHEXiMinusModel(0),
    fXiMinusMult(0),
    fXiMinusIonisation(0),
    fAntiXiMinusInelastic(0),
    fLEAntiXiMinusModel(0),  
    fHEAntiXiMinusModel(0),  
    fAntiXiMinusMult(0),     
    fAntiXiMinusIonisation(0),
    fOmegaMinusInelastic(0),  
    fLEOmegaMinusModel(0),    
    fHEOmegaMinusModel(0),    
    fOmegaMinusMult(0),           
    fOmegaMinusIonisation(0), 
    fAntiOmegaMinusInelastic(0),
    fLEAntiOmegaMinusModel(0),  
    fHEAntiOmegaMinusModel(0),  
    fAntiOmegaMinusMult(0),     
    fAntiOmegaMinusIonisation(0),
    fOtherProcesses(),
    fSetEM(setEM),
    fSetHadron(setHadron) {
//
}

//_____________________________________________________________________________
TG4PhysicsConstructorHadron::~TG4PhysicsConstructorHadron() 
{
//
  delete fElasticProcess;
  delete fElasticModel;
  delete fPionPlusInelastic;
  delete fLEPionPlusModel;
  delete fPionPlusMult;
  delete fPionPlusIonisation;
  delete fPionMinusInelastic;
  delete fLEPionMinusModel;
  delete fPionMinusMult;         
  delete fPionMinusIonisation;
  delete fPionMinusAbsorption;
  delete fTheoModel; 
  delete fHandler;
  delete fPreEquilib;
  delete fCascade;           
  delete fStringModel;
  delete fFragmentation;
  delete fStringDecay;  
  delete fKaonPlusInelastic;
  delete fLEKaonPlusModel;
  delete fHEKaonPlusModel;
  delete fKaonPlusMult;
  delete fKaonPlusIonisation;
  delete fKaonMinusInelastic;
  delete fLEKaonMinusModel;  
  delete fHEKaonMinusModel;  
  delete fKaonMinusMult;        
  delete fKaonMinusIonisation;
  delete fKaonMinusAbsorption;
  delete fKaonZeroLInelastic;
  delete fLEKaonZeroLModel;  
  delete fHEKaonZeroLModel;  
  delete fKaonZeroSInelastic;
  delete fLEKaonZeroSModel;  
  delete fHEKaonZeroSModel;  
  delete fProtonInelastic;    
  delete fLEProtonModel;         
  delete fHEProtonModel;         
  delete fProtonMult;         
  delete fProtonIonisation;   
  delete fAntiProtonInelastic;
  delete fLEAntiProtonModel;  
  delete fHEAntiProtonModel;  
  delete fAntiProtonMult;         
  delete fAntiProtonIonisation;
  delete fAntiProtonAnnihilation;
  delete fNeutronInelastic;    
  delete fLENeutronModel;          
  delete fHENeutronModel;          
  delete fNeutronFission;          
  delete fNeutronFissionModel;
  delete fNeutronCapture;          
  delete fNeutronCaptureModel;
  delete fAntiNeutronInelastic;
  delete fLEAntiNeutronModel;
  delete fHEAntiNeutronModel;
  delete fAntiNeutronAnnihilation;
  delete fLambdaInelastic;
  delete fLELambdaModel;
  delete fHELambdaModel;
  delete fAntiLambdaInelastic;
  delete fLEAntiLambdaModel;  
  delete fHEAntiLambdaModel;  
  delete fSigmaMinusInelastic;
  delete fLESigmaMinusModel;  
  delete fHESigmaMinusModel;  
  delete fSigmaMinusMult;         
  delete fSigmaMinusIonisation;
  delete fAntiSigmaMinusInelastic;
  delete fLEAntiSigmaMinusModel;  
  delete fHEAntiSigmaMinusModel;  
  delete fAntiSigmaMinusMult;     
  delete fAntiSigmaMinusIonisation;
  delete fSigmaPlusInelastic;
  delete fLESigmaPlusModel;  
  delete fHESigmaPlusModel;  
  delete fSigmaPlusMult;        
  delete fSigmaPlusIonisation;
  delete fAntiSigmaPlusInelastic;
  delete fLEAntiSigmaPlusModel;  
  delete fHEAntiSigmaPlusModel;  
  delete fAntiSigmaPlusMult;     
  delete fAntiSigmaPlusIonisation;
  delete fXiZeroInelastic;
  delete fLEXiZeroModel;
  delete fHEXiZeroModel;
  delete fAntiXiZeroInelastic;
  delete fLEAntiXiZeroModel;  
  delete fHEAntiXiZeroModel;  
  delete fXiMinusInelastic;
  delete fLEXiMinusModel;
  delete fHEXiMinusModel;
  delete fXiMinusMult;
  delete fXiMinusIonisation;
  delete fAntiXiMinusInelastic;
  delete fLEAntiXiMinusModel;  
  delete fHEAntiXiMinusModel;  
  delete fAntiXiMinusMult;     
  delete fAntiXiMinusIonisation;
  delete fOmegaMinusInelastic;  
  delete fLEOmegaMinusModel;    
  delete fHEOmegaMinusModel;    
  delete fOmegaMinusMult;           
  delete fOmegaMinusIonisation; 
  delete fAntiOmegaMinusInelastic;
  delete fLEAntiOmegaMinusModel;  
  delete fHEAntiOmegaMinusModel;  
  delete fAntiOmegaMinusMult;     
  delete fAntiOmegaMinusIonisation;

  for (G4int i=0; i<G4int(fOtherProcesses.size()); i++) 
    delete fOtherProcesses[i];
}

//
// private methods
//

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForPionPlus()
{
/// Construct processes for pi+.

  // create process
  fPionPlusMult = new G4MultipleScattering();
  fPionPlusIonisation = new G4hIonisation();

  // add process
  G4ProcessManager* pManager = G4PionPlus::PionPlus()->GetProcessManager();

  pManager->AddProcess(fPionPlusIonisation, ordInActive, 2, 2);
  pManager->AddProcess(fPionPlusMult);

  // set ordering
  pManager->SetProcessOrdering(fPionPlusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fPionPlusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fPionPlusIonisation, kLOSS); 
  controlMap->Add(fPionPlusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fPionPlusIonisation, kPEnergyLoss); 
  mcMap->Add(fPionPlusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForPionMinus()
{
/// Construct processes for pi-.

  // create process
  fPionMinusMult = new G4MultipleScattering();
  fPionMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager = G4PionMinus::PionMinus()->GetProcessManager();

  pManager->AddProcess(fPionMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fPionMinusMult);
  pManager->SetProcessOrdering(fPionMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fPionMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fPionMinusIonisation, kLOSS); 
  controlMap->Add(fPionMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fPionMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fPionMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForKaonPlus()
{
/// Construct processes for K+.

  // create process
  fKaonPlusMult = new G4MultipleScattering();
  fKaonPlusIonisation = new G4hIonisation();

  // add process
  G4ProcessManager* pManager = G4KaonPlus::KaonPlus()->GetProcessManager();

  pManager->AddProcess(fKaonPlusIonisation, ordInActive, 2, 2);
  pManager->AddProcess(fKaonPlusMult);

  // set ordering
  pManager->SetProcessOrdering(fKaonPlusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fKaonPlusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fKaonPlusIonisation, kLOSS); 
  controlMap->Add(fKaonPlusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fKaonPlusIonisation, kPEnergyLoss); 
  mcMap->Add(fKaonPlusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForKaonMinus()
{
/// Construct processes for K-.

  // create process
  fKaonMinusMult = new G4MultipleScattering();
  fKaonMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager = G4KaonMinus::KaonMinus()->GetProcessManager();

  pManager->AddProcess(fKaonMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fKaonMinusMult);
  pManager->SetProcessOrdering(fKaonMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fKaonMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fKaonMinusIonisation, kLOSS); 
  controlMap->Add(fKaonMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fKaonMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fKaonMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForProton()
{
/// Construct processes for proton.

  // create process
  fProtonMult = new G4MultipleScattering();
  fProtonIonisation = new G4hIonisation();

  // add process
  G4ProcessManager* pManager = G4Proton::Proton()->GetProcessManager();

  pManager->AddProcess(fProtonIonisation, ordInActive, 2, 2);
  pManager->AddProcess(fProtonMult);

  // set ordering
  pManager->SetProcessOrdering(fProtonMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fProtonMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fProtonIonisation, kLOSS); 
  controlMap->Add(fProtonMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fProtonIonisation, kPEnergyLoss); 
  mcMap->Add(fProtonMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForAntiProton()
{
/// Construct processes for anti-proton.

  // create process
  fAntiProtonMult = new G4MultipleScattering();
  fAntiProtonIonisation = new G4hIonisation();
  fAntiProtonAnnihilation = new G4AntiProtonAnnihilationAtRest();

  // add process & set ordering
  G4ProcessManager* pManager = G4AntiProton::AntiProton()->GetProcessManager();

  pManager->AddProcess(fAntiProtonIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fAntiProtonMult);
  pManager->SetProcessOrdering(fAntiProtonMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fAntiProtonMult, idxPostStep, 1);

  pManager->AddRestProcess(fAntiProtonAnnihilation);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiProtonIonisation, kLOSS); 
  controlMap->Add(fAntiProtonMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiProtonIonisation, kPEnergyLoss); 
  mcMap->Add(fAntiProtonMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForSigmaMinus()
{
/// Construct processes for Sigma-.

  // create process
  fSigmaMinusMult = new G4MultipleScattering();
  fSigmaMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager = G4SigmaMinus::SigmaMinus()->GetProcessManager();

  pManager->AddProcess(fSigmaMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fSigmaMinusMult);
  pManager->SetProcessOrdering(fSigmaMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fSigmaMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fSigmaMinusIonisation, kLOSS); 
  controlMap->Add(fSigmaMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fSigmaMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fSigmaMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForAntiSigmaMinus()
{
/// Construct processes for anti-Sigma-.

  // create process
  fAntiSigmaMinusMult = new G4MultipleScattering();
  fAntiSigmaMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiSigmaMinus::AntiSigmaMinus()->GetProcessManager();

  pManager->AddProcess(fAntiSigmaMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fAntiSigmaMinusMult);
  pManager->SetProcessOrdering(fAntiSigmaMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fAntiSigmaMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiSigmaMinusIonisation, kLOSS); 
  controlMap->Add(fAntiSigmaMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiSigmaMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fAntiSigmaMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForSigmaPlus()
{
/// Construct processes for Sigma+.

  // create process
  fSigmaPlusMult = new G4MultipleScattering();
  fSigmaPlusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager = G4SigmaPlus::SigmaPlus()->GetProcessManager();

  pManager->AddProcess(fSigmaPlusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fSigmaPlusMult);
  pManager->SetProcessOrdering(fSigmaPlusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fSigmaPlusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fSigmaPlusIonisation, kLOSS); 
  controlMap->Add(fSigmaPlusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fSigmaPlusIonisation, kPEnergyLoss); 
  mcMap->Add(fSigmaPlusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForAntiSigmaPlus()
{
/// Construct processes for anti-Sigma+.

  // create process
  fAntiSigmaPlusMult = new G4MultipleScattering();
  fAntiSigmaPlusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiSigmaPlus::AntiSigmaPlus()->GetProcessManager();

  pManager->AddProcess(fAntiSigmaPlusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fAntiSigmaPlusMult);
  pManager->SetProcessOrdering(fAntiSigmaPlusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fAntiSigmaPlusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiSigmaPlusIonisation, kLOSS); 
  controlMap->Add(fAntiSigmaPlusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiSigmaPlusIonisation, kPEnergyLoss); 
  mcMap->Add(fAntiSigmaPlusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForXiMinus()
{
/// Construct processes for Xi-.

  // create process
  fXiMinusMult = new G4MultipleScattering();
  fXiMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager = G4XiMinus::XiMinus()->GetProcessManager();

  pManager->AddProcess(fXiMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fXiMinusMult);
  pManager->SetProcessOrdering(fXiMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fXiMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fXiMinusIonisation, kLOSS); 
  controlMap->Add(fXiMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fXiMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fXiMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForAntiXiMinus()
{
/// Construct processes for anti-Xi-.

  // create process
  fAntiXiMinusMult = new G4MultipleScattering();
  fAntiXiMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiXiMinus::AntiXiMinus()->GetProcessManager();

  pManager->AddProcess(fAntiXiMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fAntiXiMinusMult);
  pManager->SetProcessOrdering(fAntiXiMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fAntiXiMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiXiMinusIonisation, kLOSS); 
  controlMap->Add(fAntiXiMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiXiMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fAntiXiMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForOmegaMinus()
{
/// Construct processes for Omega-.

  // create process
  fOmegaMinusMult = new G4MultipleScattering();
  fOmegaMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager = G4OmegaMinus::OmegaMinus()->GetProcessManager();

  pManager->AddProcess(fOmegaMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fOmegaMinusMult);
  pManager->SetProcessOrdering(fOmegaMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fOmegaMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fOmegaMinusIonisation, kLOSS); 
  controlMap->Add(fOmegaMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fOmegaMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fOmegaMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForAntiOmegaMinus()
{
/// Construct processes for pi+.

  // create process
  fAntiOmegaMinusMult = new G4MultipleScattering();
  fAntiOmegaMinusIonisation = new G4hIonisation();

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiOmegaMinus::AntiOmegaMinus()->GetProcessManager();

  pManager->AddProcess(fAntiOmegaMinusIonisation, ordInActive, 2, 2);

  pManager->AddProcess(fAntiOmegaMinusMult);
  pManager->SetProcessOrdering(fAntiOmegaMinusMult, idxAlongStep, 1);
  pManager->SetProcessOrdering(fAntiOmegaMinusMult, idxPostStep, 1);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiOmegaMinusIonisation, kLOSS); 
  controlMap->Add(fAntiOmegaMinusMult, kMULS); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiOmegaMinusIonisation, kPEnergyLoss); 
  mcMap->Add(fAntiOmegaMinusMult, kPMultipleScattering); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructEMProcessForOther()
{
/// Construct processes for other hadrons.

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pManager = particle->GetProcessManager();
    G4int nofAlongStepProcesses 
      = pManager->GetAlongStepProcessVector()->length();

    if ( !particle->IsShortLived()       &&
         particle->GetPDGCharge() != 0.0 &&
         nofAlongStepProcesses == 1      && 
         particle->GetParticleName() != "chargedgeantino") {
        
      // create processes
      G4VProcess* aMultipleScattering = new G4MultipleScattering();
      G4VProcess* anIonisation = new G4hIonisation();

      // add processes
      pManager->AddProcess(anIonisation, ordInActive, 2, 2);
      pManager->AddProcess(aMultipleScattering);

      // set ordering
      pManager->SetProcessOrdering(aMultipleScattering, idxAlongStep, 1);
      pManager->SetProcessOrdering(aMultipleScattering, idxPostStep, 1);

      // map to G3 controls
      TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
      controlMap->Add(anIonisation, kLOSS); 
      controlMap->Add(aMultipleScattering, kMULS); 

      // map to TMCProcess codes
      TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
      mcMap->Add(anIonisation, kPEnergyLoss); 
      mcMap->Add(aMultipleScattering, kPMultipleScattering); 
      
      // keep for deleting 
      fOtherProcesses.push_back(aMultipleScattering);
      fOtherProcesses.push_back(anIonisation);
    }
  }    
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForPionPlus()
{
/// Construct processes for pi+.

  // theo model
  fPreEquilib = new G4PreCompoundModel(fHandler);
  fCascade->SetDeExcitation(fPreEquilib);  
  fTheoModel->SetTransport(fCascade);
  fTheoModel->SetHighEnergyGenerator(fStringModel);
  fStringDecay = new G4ExcitedStringDecay(fFragmentation);
  fStringModel->SetFragmentationModel(fStringDecay);
  fTheoModel->SetMinEnergy(15*GeV);
  fTheoModel->SetMaxEnergy(100*TeV);

  // PionPlus
  // add process
  G4ProcessManager* pManager = G4PionPlus::PionPlus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEPionPlusModel = new G4LEPionPlusInelastic();
  fPionPlusInelastic = new G4PionPlusInelasticProcess();
  fPionPlusInelastic->RegisterMe(fLEPionPlusModel);
  fPionPlusInelastic->RegisterMe(fTheoModel);
  pManager->AddDiscreteProcess(fPionPlusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fElasticProcess, kHADR); 
  controlMap->Add(fPionPlusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fElasticProcess, kPHElastic); 
  mcMap->Add(fPionPlusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForPionMinus()
{
/// Construct processes for pi-.

  // add process & set ordering
  G4ProcessManager* pManager = G4PionMinus::PionMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEPionMinusModel = new G4LEPionMinusInelastic();
  fPionMinusInelastic = new G4PionMinusInelasticProcess();
  fPionMinusInelastic->RegisterMe(fLEPionMinusModel);
  fPionMinusInelastic->RegisterMe(fTheoModel);
  pManager->AddDiscreteProcess(fPionMinusInelastic);

#ifdef TRIUMF_STOP_PIMINUS
  fPionMinusAbsorption = new G4PionMinusAbsorptionAtRest(); 
#else
  fPionMinusAbsorption = new G4PiMinusAbsorptionAtRest();
#endif
  pManager->AddRestProcess(fPionMinusAbsorption, ordDefault);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fPionMinusInelastic, kHADR); 
  controlMap->Add(fPionMinusAbsorption, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fPionMinusInelastic, kPHInhelastic); 
  mcMap->Add(fPionMinusAbsorption, kPNuclearAbsorption); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForKaonPlus()
{
/// Construct processes for K+.

  // add process
  G4ProcessManager* pManager = G4KaonPlus::KaonPlus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEKaonPlusModel = new G4LEKaonPlusInelastic();
  fHEKaonPlusModel = new G4HEKaonPlusInelastic();
  fKaonPlusInelastic = new G4KaonPlusInelasticProcess();
  fKaonPlusInelastic->RegisterMe(fLEKaonPlusModel);
  fKaonPlusInelastic->RegisterMe(fHEKaonPlusModel);
  pManager->AddDiscreteProcess(fKaonPlusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fKaonPlusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fKaonPlusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForKaonMinus()
{
/// Construct processes for K-.

  // add process & set ordering
  G4ProcessManager* pManager = G4KaonMinus::KaonMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEKaonMinusModel = new G4LEKaonMinusInelastic();
  fHEKaonMinusModel = new G4HEKaonMinusInelastic();
  fKaonMinusInelastic = new G4KaonMinusInelasticProcess();
  fKaonMinusInelastic->RegisterMe(fLEKaonMinusModel);
  fKaonMinusInelastic->RegisterMe(fHEKaonMinusModel);
  pManager->AddDiscreteProcess(fKaonMinusInelastic);

#ifdef TRIUMF_STOP_KMINUS
  fKaonMinusAbsorption = new G4KaonMinusAbsorption();
#else
  fKaonMinusAbsorption = new G4PiMinusAbsorptionAtRest();
#endif
  pManager->AddRestProcess(fKaonMinusAbsorption, ordDefault);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fKaonMinusInelastic, kHADR); 
  controlMap->Add(fKaonMinusAbsorption, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fKaonMinusInelastic, kPHInhelastic); 
  mcMap->Add(fKaonMinusAbsorption, kPNuclearAbsorption); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForKaonZeroLong()
{
/// Construct processes for K0L.

  // add process
  G4ProcessManager* pManager 
    = G4KaonZeroLong::KaonZeroLong()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEKaonZeroLModel = new G4LEKaonZeroLInelastic();
  fHEKaonZeroLModel = new G4HEKaonZeroInelastic();
  fKaonZeroLInelastic = new G4KaonZeroLInelasticProcess();
  fKaonZeroLInelastic->RegisterMe(fLEKaonZeroLModel);
  fKaonZeroLInelastic->RegisterMe(fHEKaonZeroLModel);
  pManager->AddDiscreteProcess(fKaonZeroLInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fKaonZeroLInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fKaonZeroLInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForKaonZeroShort()
{
/// Construct processes for K0S.

  // add process
  G4ProcessManager* pManager 
    = G4KaonZeroShort::KaonZeroShort()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEKaonZeroSModel = new G4LEKaonZeroSInelastic();
  fHEKaonZeroSModel = new G4HEKaonZeroInelastic();
  fKaonZeroSInelastic = new G4KaonZeroSInelasticProcess();
  fKaonZeroSInelastic->RegisterMe(fLEKaonZeroSModel);
  fKaonZeroSInelastic->RegisterMe(fHEKaonZeroSModel);
  pManager->AddDiscreteProcess(fKaonZeroSInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fKaonZeroSInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fKaonZeroSInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForProton()
{
/// Construct processes for proton.

  // add process
  G4ProcessManager* pManager = G4Proton::Proton()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEProtonModel = new G4LEProtonInelastic();
  fHEProtonModel = new G4HEProtonInelastic();
  fProtonInelastic = new G4ProtonInelasticProcess();
  fProtonInelastic->RegisterMe(fLEProtonModel);
  fProtonInelastic->RegisterMe(fHEProtonModel);
  pManager->AddDiscreteProcess(fProtonInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fProtonInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fProtonInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiProton()
{
/// Construct processes for anti-proton.

  // add process & set ordering
  G4ProcessManager* pManager = G4AntiProton::AntiProton()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiProtonModel = new G4LEAntiProtonInelastic();
  fHEAntiProtonModel = new G4HEAntiProtonInelastic();
  fAntiProtonInelastic = new G4AntiProtonInelasticProcess();
  fAntiProtonInelastic->RegisterMe(fLEAntiProtonModel);
  fAntiProtonInelastic->RegisterMe(fHEAntiProtonModel);
  pManager->AddDiscreteProcess(fAntiProtonInelastic);

  fAntiProtonAnnihilation = new G4AntiProtonAnnihilationAtRest();
  pManager->AddRestProcess(fAntiProtonAnnihilation);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiProtonInelastic, kHADR); 
  controlMap->Add(fAntiProtonAnnihilation, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiProtonInelastic, kPHInhelastic); 
  mcMap->Add(fAntiProtonAnnihilation, kPPbarAnnihilation); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForNeutron()
{
/// Construct processes for neutron.

  // add process
  G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLENeutronModel = new G4LENeutronInelastic();
  fHENeutronModel = new G4HENeutronInelastic();
  fNeutronInelastic = new G4NeutronInelasticProcess();
  fNeutronInelastic->RegisterMe(fLENeutronModel);
  fNeutronInelastic->RegisterMe(fHENeutronModel);
  pManager->AddDiscreteProcess(fNeutronInelastic);

  fNeutronFissionModel = new G4LFission();
  fNeutronFission = new G4HadronFissionProcess();
  fNeutronFission->RegisterMe(fNeutronFissionModel);
  pManager->AddDiscreteProcess(fNeutronFission);

  fNeutronCaptureModel = new G4LCapture();
  fNeutronCapture = new G4HadronCaptureProcess();
  fNeutronCapture->RegisterMe(fNeutronCaptureModel);
  pManager->AddDiscreteProcess(fNeutronCapture);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fNeutronInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fNeutronInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiNeutron()
{
/// Construct processes for anti-neutron.

  // add process
  G4ProcessManager* pManager = G4AntiNeutron::AntiNeutron()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiNeutronModel = new G4LEAntiNeutronInelastic();
  fHEAntiNeutronModel = new G4HEAntiNeutronInelastic();
  fAntiNeutronInelastic = new G4AntiNeutronInelasticProcess();
  fAntiNeutronInelastic->RegisterMe(fLEAntiNeutronModel);
  fAntiNeutronInelastic->RegisterMe(fHEAntiNeutronModel);
  pManager->AddDiscreteProcess(fAntiNeutronInelastic);

  fAntiNeutronAnnihilation = new G4AntiNeutronAnnihilationAtRest();
  pManager->AddRestProcess(fAntiNeutronAnnihilation);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiNeutronInelastic, kHADR); 
  controlMap->Add(fAntiNeutronAnnihilation, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiNeutronInelastic, kPHInhelastic); 
  mcMap->Add(fAntiNeutronAnnihilation, kPNoProcess);   
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForLambda()
{
/// Construct processes for Lambda.

  // add process
  G4ProcessManager* pManager = G4Lambda::Lambda()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLELambdaModel = new G4LELambdaInelastic();
  fHELambdaModel = new G4HELambdaInelastic();
  fLambdaInelastic = new G4LambdaInelasticProcess();
  fLambdaInelastic->RegisterMe(fLELambdaModel);
  fLambdaInelastic->RegisterMe(fHELambdaModel);
  pManager->AddDiscreteProcess(fLambdaInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fLambdaInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fLambdaInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiLambda()
{
/// Construct processes for anti-Lambda.

  // add process
  G4ProcessManager* pManager = G4AntiLambda::AntiLambda()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiLambdaModel = new G4LEAntiLambdaInelastic();
  fHEAntiLambdaModel = new G4HEAntiLambdaInelastic();
  fAntiLambdaInelastic = new G4AntiLambdaInelasticProcess();
  fAntiLambdaInelastic->RegisterMe(fLEAntiLambdaModel);
  fAntiLambdaInelastic->RegisterMe(fHEAntiLambdaModel);
  pManager->AddDiscreteProcess(fAntiLambdaInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiLambdaInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiLambdaInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForSigmaMinus()
{
/// Construct processes for Sigma-.

  // add process & set ordering
  G4ProcessManager* pManager = G4SigmaMinus::SigmaMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLESigmaMinusModel = new G4LESigmaMinusInelastic();
  fHESigmaMinusModel = new G4HESigmaMinusInelastic();
  fSigmaMinusInelastic = new G4SigmaMinusInelasticProcess();
  fSigmaMinusInelastic->RegisterMe(fLESigmaMinusModel);
  fSigmaMinusInelastic->RegisterMe(fHESigmaMinusModel);
  pManager->AddDiscreteProcess(fSigmaMinusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fSigmaMinusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fSigmaMinusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiSigmaMinus()
{
/// Construct processes for anti-Sigma-.

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiSigmaMinus::AntiSigmaMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiSigmaMinusModel = new G4LEAntiSigmaMinusInelastic();
  fHEAntiSigmaMinusModel = new G4HEAntiSigmaMinusInelastic();
  fAntiSigmaMinusInelastic = new G4AntiSigmaMinusInelasticProcess();
  fAntiSigmaMinusInelastic->RegisterMe(fLEAntiSigmaMinusModel);
  fAntiSigmaMinusInelastic->RegisterMe(fHEAntiSigmaMinusModel);
  pManager->AddDiscreteProcess(fAntiSigmaMinusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiSigmaMinusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiSigmaMinusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForSigmaPlus()
{
/// Construct processes for Sigma+.

  // add process & set ordering
  G4ProcessManager* pManager = G4SigmaPlus::SigmaPlus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLESigmaPlusModel = new G4LESigmaPlusInelastic();
  fHESigmaPlusModel = new G4HESigmaPlusInelastic();
  fSigmaPlusInelastic = new G4SigmaPlusInelasticProcess();
  fSigmaPlusInelastic->RegisterMe(fLESigmaPlusModel);
  fSigmaPlusInelastic->RegisterMe(fHESigmaPlusModel);
  pManager->AddDiscreteProcess(fSigmaPlusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fSigmaPlusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fSigmaPlusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiSigmaPlus()
{
/// Construct processes for anti-Sigma+.

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiSigmaPlus::AntiSigmaPlus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiSigmaPlusModel = new G4LEAntiSigmaPlusInelastic();
  fHEAntiSigmaPlusModel = new G4HEAntiSigmaPlusInelastic();
  fAntiSigmaPlusInelastic = new G4AntiSigmaPlusInelasticProcess();
  fAntiSigmaPlusInelastic->RegisterMe(fLEAntiSigmaPlusModel);
  fAntiSigmaPlusInelastic->RegisterMe(fHEAntiSigmaPlusModel);
  pManager->AddDiscreteProcess(fAntiSigmaPlusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiSigmaPlusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiSigmaPlusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForXiMinus()
{
/// Construct processes for Xi-.

  // add process & set ordering
  G4ProcessManager* pManager = G4XiMinus::XiMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEXiMinusModel = new G4LEXiMinusInelastic();
  fHEXiMinusModel = new G4HEXiMinusInelastic();
  fXiMinusInelastic = new G4XiMinusInelasticProcess(); 
  fXiMinusInelastic->RegisterMe(fLEXiMinusModel);
  fXiMinusInelastic->RegisterMe(fHEXiMinusModel);
  pManager->AddDiscreteProcess(fXiMinusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fXiMinusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fXiMinusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiXiMinus()
{
/// Construct processes for anti-Xi-.

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiXiMinus::AntiXiMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiXiMinusModel = new G4LEAntiXiMinusInelastic();
  fHEAntiXiMinusModel = new G4HEAntiXiMinusInelastic();
  fAntiXiMinusInelastic = new G4AntiXiMinusInelasticProcess();
  fAntiXiMinusInelastic->RegisterMe(fLEAntiXiMinusModel);
  fAntiXiMinusInelastic->RegisterMe(fHEAntiXiMinusModel);
  pManager->AddDiscreteProcess(fAntiXiMinusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiXiMinusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiXiMinusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForXiZero()
{
/// Construct processes for Xi0.

  // add process
  G4ProcessManager* pManager = G4XiZero::XiZero()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEXiZeroModel = new G4LEXiZeroInelastic();
  fHEXiZeroModel = new G4HEXiZeroInelastic();
  fXiZeroInelastic = new G4XiZeroInelasticProcess();
  fXiZeroInelastic->RegisterMe(fLEXiZeroModel);
  fXiZeroInelastic->RegisterMe(fHEXiZeroModel);
  pManager->AddDiscreteProcess(fXiZeroInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fXiZeroInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fXiZeroInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiXiZero()
{
/// Construct processes for anti-Xi0.

  // add process
  G4ProcessManager* pManager = G4AntiXiZero::AntiXiZero()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiXiZeroModel = new G4LEAntiXiZeroInelastic();
  fHEAntiXiZeroModel = new G4HEAntiXiZeroInelastic();
  fAntiXiZeroInelastic = new G4AntiXiZeroInelasticProcess();
  fAntiXiZeroInelastic->RegisterMe(fLEAntiXiZeroModel);
  fAntiXiZeroInelastic->RegisterMe(fHEAntiXiZeroModel);
  pManager->AddDiscreteProcess(fAntiXiZeroInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiXiZeroInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiXiZeroInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForOmegaMinus()
{
/// Construct processes for Omega-.

  // add process & set ordering
  G4ProcessManager* pManager = G4OmegaMinus::OmegaMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEOmegaMinusModel = new G4LEOmegaMinusInelastic();
  fHEOmegaMinusModel = new G4HEOmegaMinusInelastic();
  fOmegaMinusInelastic = new G4OmegaMinusInelasticProcess();
  fOmegaMinusInelastic->RegisterMe(fLEOmegaMinusModel);
  fOmegaMinusInelastic->RegisterMe(fHEOmegaMinusModel);
  pManager->AddDiscreteProcess(fOmegaMinusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fOmegaMinusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fOmegaMinusInelastic, kPHInhelastic); 
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructHadProcessForAntiOmegaMinus()
{
/// Construct processes for pi+.

  // add process & set ordering
  G4ProcessManager* pManager 
    = G4AntiOmegaMinus::AntiOmegaMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(fElasticProcess);

  fLEAntiOmegaMinusModel = new G4LEAntiOmegaMinusInelastic();
  fHEAntiOmegaMinusModel = new G4HEAntiOmegaMinusInelastic();
  fAntiOmegaMinusInelastic = new G4AntiOmegaMinusInelasticProcess();
  fAntiOmegaMinusInelastic->RegisterMe(fLEAntiOmegaMinusModel);
  fAntiOmegaMinusInelastic->RegisterMe(fHEAntiOmegaMinusModel);
  pManager->AddDiscreteProcess(fAntiOmegaMinusInelastic);

  // map to G3 controls
  TG4ProcessControlMap* controlMap = TG4ProcessControlMap::Instance();
  controlMap->Add(fAntiOmegaMinusInelastic, kHADR); 

  // map to G3 TMCProcess codes
  TG4ProcessMCMap* mcMap = TG4ProcessMCMap::Instance();
  mcMap->Add(fAntiOmegaMinusInelastic, kPHInhelastic); 
}


// protected methods

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructParticle()
{
/// Construct all hadrons.

  //  Construct all mesons
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  //  Construct all barions
  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  //  Construct  resonances and quarks
  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

//_____________________________________________________________________________
void TG4PhysicsConstructorHadron::ConstructProcess()
{
/// Construct all hadronic processes.

  G4cout << G4endl;
  G4cout << "###" << G4endl;
  G4cout << "You are using the Geant4 VMC hadronics physics" << G4endl;
  G4cout << "equivalent to ExN04HadronPhysics from Geant4 (version 6.0)" << G4endl; 
  G4cout << "Note that this hadronic physics list is not optimized for any particular usage." << G4endl;
  G4cout << "If you wish to have a starting point tailored for a particular area of work," << G4endl;
  G4cout << "please use one of the available physics lists by use-case." << G4endl;
  G4cout << " More information can also be found from the Geant4 HyperNews." << G4endl;
  G4cout << "###" << G4endl;
  G4cout << G4endl;

  if (fSetHadron) {

    // Elastic process
    fElasticModel = new G4LElastic();
    fElasticProcess = new G4HadronElasticProcess();
    fElasticProcess->RegisterMe(fElasticModel);
    
    fTheoModel = new G4TheoFSGenerator();
    fHandler = new G4ExcitationHandler();
    fCascade = new G4GeneratorPrecompoundInterface();
    fStringModel = new G4QGSModel< G4QGSParticipants >();
    fFragmentation = new G4QGSMFragmentation();
  
    // Hadron processes
    ConstructHadProcessForPionPlus();
    ConstructHadProcessForPionMinus();
    ConstructHadProcessForKaonPlus();
    ConstructHadProcessForKaonMinus();
    ConstructHadProcessForKaonZeroLong();
    ConstructHadProcessForKaonZeroShort();
    ConstructHadProcessForProton();
    ConstructHadProcessForAntiProton();
    ConstructHadProcessForNeutron();
    ConstructHadProcessForAntiNeutron();
    ConstructHadProcessForLambda();
    ConstructHadProcessForAntiLambda();
    ConstructHadProcessForSigmaMinus();
    ConstructHadProcessForAntiSigmaMinus();
    ConstructHadProcessForSigmaPlus();
    ConstructHadProcessForAntiSigmaPlus();
    ConstructHadProcessForXiMinus();
    ConstructHadProcessForAntiXiMinus();
    ConstructHadProcessForXiZero();
    ConstructHadProcessForAntiXiZero();
    ConstructHadProcessForOmegaMinus();
    ConstructHadProcessForAntiOmegaMinus();
  }  

  if (fSetEM) {
    // EM processes
    ConstructEMProcessForPionPlus();
    ConstructEMProcessForPionMinus();
    ConstructEMProcessForKaonPlus();
    ConstructEMProcessForKaonMinus();
    ConstructEMProcessForProton();
    ConstructEMProcessForAntiProton();
    ConstructEMProcessForSigmaMinus();
    ConstructEMProcessForAntiSigmaMinus();
    ConstructEMProcessForSigmaPlus();
    ConstructEMProcessForAntiSigmaPlus();
    ConstructEMProcessForXiMinus();
    ConstructEMProcessForAntiXiMinus();
    ConstructEMProcessForOmegaMinus();
    ConstructEMProcessForAntiOmegaMinus();
    ConstructEMProcessForOther();
  }  

  if (VerboseLevel() > 0) {
     if (fSetEM)     G4cout << "### EM physics for hadrons constructed." << G4endl;
     if (fSetHadron) G4cout << "### Hadron physics for hadrons constructed." << G4endl;
  }   
}

