// $Id: TG4PhysicsConstructorHadron.h,v 1.6 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorHadron
/// \brief Constructor of hadron physics.
///
/// According to ExN04HadronPhysics.hh, GEANT4 tag Name: geant4-06-00
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_HADRON_H
#define TG4_PHYSICS_CONSTRUCTOR_HADRON_H

#include "TG4VPhysicsConstructor.h"

#include <vector>
#include <globals.hh>

#include <G4QGSModel.hh>
#include <G4QGSParticipants.hh>

class G4MultipleScattering;
class G4hIonisation;

class G4HadronElasticProcess;
class G4HadronFissionProcess;
class G4HadronCaptureProcess;

class G4PionPlusInelasticProcess;
class G4PionMinusInelasticProcess;
class G4KaonPlusInelasticProcess;
class G4KaonZeroSInelasticProcess;
class G4KaonZeroLInelasticProcess;
class G4KaonMinusInelasticProcess;
class G4ProtonInelasticProcess;
class G4AntiProtonInelasticProcess;
class G4NeutronInelasticProcess;
class G4AntiNeutronInelasticProcess;
class G4LambdaInelasticProcess;
class G4AntiLambdaInelasticProcess;
class G4SigmaPlusInelasticProcess;
class G4SigmaMinusInelasticProcess;
class G4AntiSigmaPlusInelasticProcess;
class G4AntiSigmaMinusInelasticProcess;
class G4XiZeroInelasticProcess;
class G4XiMinusInelasticProcess;
class G4AntiXiZeroInelasticProcess;
class G4AntiXiMinusInelasticProcess;
class G4DeuteronInelasticProcess;
class G4TritonInelasticProcess;
class G4AlphaInelasticProcess;
class G4OmegaMinusInelasticProcess;
class G4AntiOmegaMinusInelasticProcess;

// Low-energy Models
class G4LElastic;   
class G4LFission;
class G4LCapture;

class G4LEPionPlusInelastic;
class G4LEPionMinusInelastic;
class G4LEKaonPlusInelastic;
class G4LEKaonZeroSInelastic;
class G4LEKaonZeroLInelastic;
class G4LEKaonMinusInelastic;
class G4LEProtonInelastic;
class G4LEAntiProtonInelastic;
class G4LENeutronInelastic;
class G4LEAntiNeutronInelastic;
class G4LELambdaInelastic;
class G4LEAntiLambdaInelastic;
class G4LESigmaPlusInelastic;
class G4LESigmaMinusInelastic;
class G4LEAntiSigmaPlusInelastic;
class G4LEAntiSigmaMinusInelastic;
class G4LEXiZeroInelastic;
class G4LEXiMinusInelastic;
class G4LEAntiXiZeroInelastic;
class G4LEAntiXiMinusInelastic;
class G4LEDeuteronInelastic;
class G4LETritonInelastic;
class G4LEAlphaInelastic;
class G4LEOmegaMinusInelastic;
class G4LEAntiOmegaMinusInelastic;

// High-energy Models

class G4HEPionPlusInelastic;
class G4HEPionMinusInelastic;
class G4HEKaonPlusInelastic;
class G4HEKaonZeroInelastic;
class G4HEKaonZeroInelastic;
class G4HEKaonMinusInelastic;
class G4HEProtonInelastic;
class G4HEAntiProtonInelastic;
class G4HENeutronInelastic;
class G4HEAntiNeutronInelastic;
class G4HELambdaInelastic;
class G4HEAntiLambdaInelastic;
class G4HESigmaPlusInelastic;
class G4HESigmaMinusInelastic;
class G4HEAntiSigmaPlusInelastic;
class G4HEAntiSigmaMinusInelastic;
class G4HEXiZeroInelastic;
class G4HEXiMinusInelastic;
class G4HEAntiXiZeroInelastic;
class G4HEAntiXiMinusInelastic;
class G4HEOmegaMinusInelastic;
class G4HEAntiOmegaMinusInelastic;

// Stopping processes
class G4AntiProtonAnnihilationAtRest;
class G4AntiNeutronAnnihilationAtRest;

#ifdef TRIUMF_STOP_PIMINUS
class G4PionMinusAbsorptionAtRest;
#else
class G4PiMinusAbsorptionAtRest;
#endif
#ifdef TRIUMF_STOP_KMINUS
class G4KaonMinusAbsorption;
#else
class G4KaonMinusAbsorptionAtRest;
#endif

// quark gluon string model with chips afterburner.
class G4TheoFSGenerator;
class G4ExcitationHandler;
class G4PreCompoundModel;
class G4GeneratorPrecompoundInterface;
class G4QGSMFragmentation;
class G4ExcitedStringDecay;

class TG4PhysicsConstructorHadron: public TG4VPhysicsConstructor
{
  typedef std::vector<G4VProcess*>  ProcessVector;

  public:
    TG4PhysicsConstructorHadron(const G4String& name = "Hadron");
    TG4PhysicsConstructorHadron(G4int verboseLevel, 
                                G4bool setEM, G4bool setHadron,
                                const G4String& name = "Hadron");
    virtual ~TG4PhysicsConstructorHadron();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
         // Elastic Process
    G4HadronElasticProcess*  fElasticProcess;            //hadron elastic process
    G4LElastic*              fElasticModel;              //elastic model
  
         // Pi + 
    G4PionPlusInelasticProcess* fPionPlusInelastic;      //pi+ inel process
    G4LEPionPlusInelastic*      fLEPionPlusModel;        //pi+ LE inel model
    G4MultipleScattering*       fPionPlusMult;           //pi+ msc
    G4hIonisation*              fPionPlusIonisation;     //pi+ ionisation

         // Pi -
    G4PionMinusInelasticProcess*  fPionMinusInelastic;   //pi- inel process
    G4LEPionMinusInelastic*       fLEPionMinusModel;     //pi- LE inel model
    G4MultipleScattering*         fPionMinusMult;        //pi- msc
    G4hIonisation*                fPionMinusIonisation;  //pi- ionisation
#ifdef TRIUMF_STOP_PIMINUS
    G4PionMinusAbsorptionAtRest*  fPionMinusAbsorption;  //pi- absorption
#else
    G4PiMinusAbsorptionAtRest*    fPionMinusAbsorption;  //pi- absorption
#endif

         // Pi+ and Pi-
   
    G4TheoFSGenerator*                fTheoModel;    // theo model
    G4ExcitationHandler*              fHandler;      // excitation handler
    G4PreCompoundModel*               fPreEquilib;   // precompound model
    G4GeneratorPrecompoundInterface*  fCascade;      // cascade
    G4QGSModel< G4QGSParticipants >*  fStringModel;  // string model
    G4QGSMFragmentation*              fFragmentation;// fragmentation
    G4ExcitedStringDecay *            fStringDecay;  // string decay

         // K + 
    G4KaonPlusInelasticProcess*  fKaonPlusInelastic;     //kaon+ inel process
    G4LEKaonPlusInelastic*       fLEKaonPlusModel;       //kaon+ LE inel model
    G4HEKaonPlusInelastic*       fHEKaonPlusModel;       //kaon+ HE inel model
    G4MultipleScattering*        fKaonPlusMult;          //kaon+ msc
    G4hIonisation*               fKaonPlusIonisation;    //kaon+ ionisation

         // K -
    G4KaonMinusInelasticProcess*  fKaonMinusInelastic;   //kaon- inel process
    G4LEKaonMinusInelastic*       fLEKaonMinusModel;     //kaon- LE inel model
    G4HEKaonMinusInelastic*       fHEKaonMinusModel;     //kaon- HE inel model
    G4MultipleScattering*         fKaonMinusMult;        //kaon- msc
    G4hIonisation*                fKaonMinusIonisation;  //kaon- ionisation
#ifdef TRIUMF_STOP_KMINUS
    G4KaonMinusAbsorption*        fKaonMinusAbsorption;  //kaon- absorption
#else
    G4PiMinusAbsorptionAtRest*    fKaonMinusAbsorption;  //kaon- absorption
#endif

        // K0L
    G4KaonZeroLInelasticProcess*  fKaonZeroLInelastic;   //kaon0 inel process
    G4LEKaonZeroLInelastic*       fLEKaonZeroLModel;     //kaon0 LE inel model
    G4HEKaonZeroInelastic*        fHEKaonZeroLModel;     //kaon0 HE inel model

        // K0S
    G4KaonZeroSInelasticProcess*  fKaonZeroSInelastic;   //kaon0S inel process
    G4LEKaonZeroSInelastic*       fLEKaonZeroSModel;     //kaon0S LE inel model
    G4HEKaonZeroInelastic*        fHEKaonZeroSModel;     //kaon0S HE inel mode

        // Proton
    G4ProtonInelasticProcess*  fProtonInelastic;         //p inel process
    G4LEProtonInelastic*       fLEProtonModel;           //p LE inel model
    G4HEProtonInelastic*       fHEProtonModel;           //p HE inel model
    G4MultipleScattering*      fProtonMult;              //p msc
    G4hIonisation*             fProtonIonisation;        //p ionisation
 
        // anti-proton
    G4AntiProtonInelasticProcess*    fAntiProtonInelastic; //p_bar inel process
    G4LEAntiProtonInelastic*         fLEAntiProtonModel;   //p_bar LE inel model
    G4HEAntiProtonInelastic*         fHEAntiProtonModel;   //p_bar HE inel model
    G4MultipleScattering*            fAntiProtonMult;      //p_bar msc
    G4hIonisation*                   fAntiProtonIonisation;//p_bar ionisation
    G4AntiProtonAnnihilationAtRest*  fAntiProtonAnnihilation;//p_bar annihilation
    
       // neutron
    G4NeutronInelasticProcess*  fNeutronInelastic;       //n inel process
    G4LENeutronInelastic*       fLENeutronModel;         //n LE inel model
    G4HENeutronInelastic*       fHENeutronModel;         //n HE inel model
    G4HadronFissionProcess*     fNeutronFission;         //n fission
    G4LFission*                 fNeutronFissionModel;    //n fission model
    G4HadronCaptureProcess*     fNeutronCapture;         //n capture
    G4LCapture*                 fNeutronCaptureModel;    //n capture model

       // anti-neutron
    G4AntiNeutronInelasticProcess*    fAntiNeutronInelastic;//n_bar inel process
    G4LEAntiNeutronInelastic*         fLEAntiNeutronModel;  //n_bar LE inel model
    G4HEAntiNeutronInelastic*         fHEAntiNeutronModel;  //n_bar HE inel model
    G4AntiNeutronAnnihilationAtRest*  fAntiNeutronAnnihilation;//n_bar ionisation
     
       // Lambda
    G4LambdaInelasticProcess*  fLambdaInelastic;         //lambda inel process
    G4LELambdaInelastic*       fLELambdaModel;           //lambda LE inel model
    G4HELambdaInelastic*       fHELambdaModel;           //lambda HE inel model
  
       // AntiLambda
    G4AntiLambdaInelasticProcess*  fAntiLambdaInelastic; //lambda_bar inel process
    G4LEAntiLambdaInelastic*       fLEAntiLambdaModel;   //lambda_bar LE inel model
    G4HEAntiLambdaInelastic*       fHEAntiLambdaModel;   //lambda_bar HE inel model
  
       // SigmaMinus
    G4SigmaMinusInelasticProcess*  fSigmaMinusInelastic; //sigma- inel process
    G4LESigmaMinusInelastic*       fLESigmaMinusModel;   //sigma- LE inel model
    G4HESigmaMinusInelastic*       fHESigmaMinusModel;   //sigma- HE inel model
    G4MultipleScattering*          fSigmaMinusMult;      //sigma- msc
    G4hIonisation*                 fSigmaMinusIonisation;//sigma- ionisation
  
       // AntiSigmaMinus
    G4AntiSigmaMinusInelasticProcess*  fAntiSigmaMinusInelastic; //sigma-_bar inel process
    G4LEAntiSigmaMinusInelastic*       fLEAntiSigmaMinusModel;   //sigma-_bar LE inel model
    G4HEAntiSigmaMinusInelastic*       fHEAntiSigmaMinusModel;   //sigma-_bar HE inel model
    G4MultipleScattering*              fAntiSigmaMinusMult;      //sigma-_bar msc
    G4hIonisation*                     fAntiSigmaMinusIonisation;//sigma-_bar ionisation
   
       // SigmaPlus
    G4SigmaPlusInelasticProcess*  fSigmaPlusInelastic;   //sigma+ inel process
    G4LESigmaPlusInelastic*       fLESigmaPlusModel;     //sigma+ LE inel model
    G4HESigmaPlusInelastic*       fHESigmaPlusModel;     //sigma+ HE inel model
    G4MultipleScattering*         fSigmaPlusMult;        //sigma+ msc
    G4hIonisation*                fSigmaPlusIonisation;  //sigma+ ionisation
  
       // AntiSigmaPlus
    G4AntiSigmaPlusInelasticProcess*  fAntiSigmaPlusInelastic;  //sigma+_bar inel process
    G4LEAntiSigmaPlusInelastic*       fLEAntiSigmaPlusModel;    //sigma+_bar LE inel model
    G4HEAntiSigmaPlusInelastic*       fHEAntiSigmaPlusModel;    //sigma+_bar HE inel model
    G4MultipleScattering*             fAntiSigmaPlusMult;       //sigma+_bar msc
    G4hIonisation*                    fAntiSigmaPlusIonisation; //sigma+_bar ionisation
  
      // XiZero
    G4XiZeroInelasticProcess*  fXiZeroInelastic;        //xi0 inel process
    G4LEXiZeroInelastic*       fLEXiZeroModel;          //xi0 LE inel model
    G4HEXiZeroInelastic*       fHEXiZeroModel;          //xi0 HE inel model
  
      // AntiXiZero
    G4AntiXiZeroInelasticProcess*  fAntiXiZeroInelastic;//xi0_bar inel process
    G4LEAntiXiZeroInelastic*       fLEAntiXiZeroModel;  //xi0_bar LE inel model
    G4HEAntiXiZeroInelastic*       fHEAntiXiZeroModel;  //xi0_bar HE inel model
  
      // XiMinus
    G4XiMinusInelasticProcess*  fXiMinusInelastic;      //xi- inel process
    G4LEXiMinusInelastic*       fLEXiMinusModel;        //xi- LE inel model
    G4HEXiMinusInelastic*       fHEXiMinusModel;        //xi- HE inel model
    G4MultipleScattering*       fXiMinusMult;           //xi- msc
    G4hIonisation*              fXiMinusIonisation;     //xi- ionisation

      // AntiXiMinus
    G4AntiXiMinusInelasticProcess*  fAntiXiMinusInelastic; //xi-_bar inel process
    G4LEAntiXiMinusInelastic*       fLEAntiXiMinusModel;   //xi-_bar LE inel model
    G4HEAntiXiMinusInelastic*       fHEAntiXiMinusModel;   //xi-_bar HE inel model
    G4MultipleScattering*           fAntiXiMinusMult;      //xi-_bar msc
    G4hIonisation*                  fAntiXiMinusIonisation;//xi-_bar ionisation
  
      // OmegaMinus
    G4OmegaMinusInelasticProcess*  fOmegaMinusInelastic;   //omega- inel process
    G4LEOmegaMinusInelastic*       fLEOmegaMinusModel;     //omega- LE inel model
    G4HEOmegaMinusInelastic*       fHEOmegaMinusModel;     //omega- HE inel model
    G4MultipleScattering*          fOmegaMinusMult;        //omega- msc
    G4hIonisation*                 fOmegaMinusIonisation;  //omega- ionisation
   
      // AntiOmegaMinus
    G4AntiOmegaMinusInelasticProcess*  fAntiOmegaMinusInelastic; //omega-_bar inel process
    G4LEAntiOmegaMinusInelastic*       fLEAntiOmegaMinusModel;   //omega-_bar LE inel model
    G4HEAntiOmegaMinusInelastic*       fHEAntiOmegaMinusModel;   //omega-_bar HE inel model
    G4MultipleScattering*              fAntiOmegaMinusMult;      //omega-_bar msc
    G4hIonisation*                     fAntiOmegaMinusIonisation;//omega-_bar ionisation
    
      // Other
    ProcessVector  fOtherProcesses; //other process
    

  private:
    TG4PhysicsConstructorHadron(const TG4PhysicsConstructorHadron& right);
    TG4PhysicsConstructorHadron& operator=(
                                const TG4PhysicsConstructorHadron& right);
  
    // methods

    // EM processes
    void ConstructEMProcessForPionPlus();
    void ConstructEMProcessForPionMinus();
    void ConstructEMProcessForKaonPlus();
    void ConstructEMProcessForKaonMinus();
    void ConstructEMProcessForProton();
    void ConstructEMProcessForAntiProton();
    void ConstructEMProcessForSigmaMinus();
    void ConstructEMProcessForAntiSigmaMinus();
    void ConstructEMProcessForSigmaPlus();
    void ConstructEMProcessForAntiSigmaPlus();
    void ConstructEMProcessForXiMinus();
    void ConstructEMProcessForAntiXiMinus();
    void ConstructEMProcessForOmegaMinus();
    void ConstructEMProcessForAntiOmegaMinus();
    void ConstructEMProcessForOther();

    // hadron processes
    void ConstructHadProcessForPionPlus();
    void ConstructHadProcessForPionMinus();
    void ConstructHadProcessForKaonPlus();
    void ConstructHadProcessForKaonMinus();
    void ConstructHadProcessForKaonZeroLong();
    void ConstructHadProcessForKaonZeroShort();
    void ConstructHadProcessForProton();
    void ConstructHadProcessForAntiProton();
    void ConstructHadProcessForNeutron();
    void ConstructHadProcessForAntiNeutron();
    void ConstructHadProcessForLambda();
    void ConstructHadProcessForAntiLambda();
    void ConstructHadProcessForSigmaMinus();
    void ConstructHadProcessForAntiSigmaMinus();
    void ConstructHadProcessForSigmaPlus();
    void ConstructHadProcessForAntiSigmaPlus();
    void ConstructHadProcessForXiMinus();
    void ConstructHadProcessForAntiXiMinus();
    void ConstructHadProcessForXiZero();
    void ConstructHadProcessForAntiXiZero();
    void ConstructHadProcessForOmegaMinus();
    void ConstructHadProcessForAntiOmegaMinus();
    void ConstructHadProcessForOther();

    // data members
    G4bool  fSetEM;    //if true - EM processes are constructed
    G4bool  fSetHadron;//if true - hadron processes are constructed
};

#endif //TG4_PHYSICS_CONSTRUCTOR_HADRON_H

