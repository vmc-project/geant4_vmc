// $Id: TG4PhysicsConstructorIon.h,v 1.5 2006/01/13 16:59:38 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorIon
/// \brief Constructor of physics for ions.
///
/// According to ExN04IonPhysics.hh, GEANT4 tag Name: geant4-06-00
///
/// Author: I. Hrivnacova

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
class G4hIonisation;
class G4MultipleScattering;

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
   G4MultipleScattering*   fIonMultipleScattering;  //ion multiple scattering
   G4hIonisation*          fIonIonisation;          //ion ionisation

          // Deuteron physics
   G4MultipleScattering*        fDeuteronMultipleScattering; //D msc
   G4hIonisation*               fDeuteronIonisation;//D ionisation
   G4DeuteronInelasticProcess*  fDeuteronProcess;   //D inelastic process
   G4LEDeuteronInelastic*       fDeuteronModel;     //D LE inelastic model

          // Triton physics
   G4MultipleScattering*        fTritonMultipleScattering; //T msc
   G4hIonisation*               fTritonIonisation;  //T ionisation
   G4TritonInelasticProcess*    fTritonProcess;     //T inelastic process
   G4LETritonInelastic*         fTritonModel;       //T LE inelastic model
  
         // Alpha physics
   G4MultipleScattering*        fAlphaMultipleScattering; //alpha msc
   G4hIonisation*               fAlphaIonisation;   //alpha ionisation
   G4AlphaInelasticProcess*     fAlphaProcess;      //alpha inelastic process
   G4LEAlphaInelastic*          fAlphaModel;        //alpha LE inelastic model

        // He3 physics
   G4MultipleScattering*        fHe3MultipleScattering; //He3 msc
   G4hIonisation*               fHe3Ionisation;     //He3 ionisation
    
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

