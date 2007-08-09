// $Id: TG4PhysicsConstructorIon.h,v 1.7 2007/05/31 10:24:32 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorIon
/// \brief Constructor of physics for ions.
///
/// According to ExN04IonPhysics.hh, GEANT4 tag Name: geant4-06-00
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_PHYSICS_CONSTRUCTOR_ION_H
#define TG4_PHYSICS_CONSTRUCTOR_ION_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class G4HadronElasticProcess;
class G4LElastic;
class G4DeuteronInelasticProcess;
class G4LEDeuteronInelastic;
class G4TritonInelasticProcess;
class G4LETritonInelastic;
class G4AlphaInelasticProcess;
class G4LEAlphaInelastic;
class G4ionIonisation;
class G4hMultipleScattering;

class TG4PhysicsConstructorIon: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorIon(const G4String& name = "Ion");
    TG4PhysicsConstructorIon(G4int verboseLevel, 
                             G4bool setEM, G4bool setHadron,
                             const G4String& name = "Ion");
    virtual ~TG4PhysicsConstructorIon();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
          // Elastic Process
   G4HadronElasticProcess* fElasticProcess;         //elastic process
   G4LElastic*             fElasticModel;           //elastic model

          // Generic Ion physics
   G4hMultipleScattering*  fIonMultipleScattering;  //ion multiple scattering
   G4ionIonisation*        fIonIonisation;          //ion ionisation

          // Deuteron physics
   G4hMultipleScattering*       fDeuteronMultipleScattering; //D msc
   G4ionIonisation*             fDeuteronIonisation;//D ionisation
   G4DeuteronInelasticProcess*  fDeuteronProcess;   //D inelastic process
   G4LEDeuteronInelastic*       fDeuteronModel;     //D LE inelastic model

          // Triton physics
   G4hMultipleScattering*       fTritonMultipleScattering; //T msc
   G4ionIonisation*             fTritonIonisation;  //T ionisation
   G4TritonInelasticProcess*    fTritonProcess;     //T inelastic process
   G4LETritonInelastic*         fTritonModel;       //T LE inelastic model
  
         // Alpha physics
   G4hMultipleScattering*       fAlphaMultipleScattering; //alpha msc
   G4ionIonisation*             fAlphaIonisation;   //alpha ionisation
   G4AlphaInelasticProcess*     fAlphaProcess;      //alpha inelastic process
   G4LEAlphaInelastic*          fAlphaModel;        //alpha LE inelastic model

        // He3 physics
   G4hMultipleScattering*       fHe3MultipleScattering; //He3 msc
   G4ionIonisation*             fHe3Ionisation;     //He3 ionisation
    
  private:
    TG4PhysicsConstructorIon(const TG4PhysicsConstructorIon& right);
    TG4PhysicsConstructorIon& operator=(const TG4PhysicsConstructorIon& right);

    // Hadron processes
    void ConstructHadProcessForGenericIon();    
    void ConstructHadProcessForDeuteron();    
    void ConstructHadProcessForTriton();    
    void ConstructHadProcessForAlpha();    
    void ConstructHadProcessForHe3();    

    // EM processes
    void ConstructEMProcessForGenericIon();    
    void ConstructEMProcessForDeuteron();    
    void ConstructEMProcessForTriton();    
    void ConstructEMProcessForAlpha();    
    void ConstructEMProcessForHe3();    

    // data members
    G4bool  fSetEM;    //if true - EM processes are constructed
    G4bool  fSetHadron;//if true - hadron processes are constructed
};

#endif //TG4_PHYSICS_CONSTRUCTOR_ION_H

