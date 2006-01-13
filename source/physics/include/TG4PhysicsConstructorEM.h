// $Id: TG4PhysicsConstructorEM.h,v 1.4 2005/09/01 10:04:33 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorEM
/// \brief Constructor of electromagnetic physics.
///
/// According to ExN04EMPhysics, GEANT4 tag Name: geant4-06-00
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_CONSTRUCTOR_EM_H
#define TG4_PHYSICS_CONSTRUCTOR_EM_H

#include "TG4VPhysicsConstructor.h"
#include <globals.hh>

class G4PhotoElectricEffect;
class G4ComptonScattering;
class G4GammaConversion;
class G4MultipleScattering;
class G4eIonisation;
class G4eBremsstrahlung;
class G4eplusAnnihilation;

class TG4PhysicsConstructorEM: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorEM(const G4String& name = "EM"); 
    TG4PhysicsConstructorEM(G4int verboseLevel, 
                            const G4String& name = "EM");
    virtual ~TG4PhysicsConstructorEM();

    // methods
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  protected:
    // data members
            // Gamma physics
    G4PhotoElectricEffect* fPhotoEffect;               //gamma photoeffect
    G4ComptonScattering*   fComptonEffect;             //Compton scattering
    G4GammaConversion*     fPairProduction;            //gamma pair production
     
            // Electron physics
    G4MultipleScattering*  fElectronMultipleScattering;//e- multiple scattering
    G4eIonisation*         fElectronIonisation;        //e- ionization 
    G4eBremsstrahlung*     fElectronBremsStrahlung;    //e- Bremsstrahlung
  
            //Positron physics
    G4MultipleScattering*  fPositronMultipleScattering;//e+ multiple scattering
    G4eIonisation*         fPositronIonisation;        //e+ ionisation
    G4eBremsstrahlung*     fPositronBremsStrahlung;    //e+ Bremsstrahlung
    G4eplusAnnihilation*   fAnnihilation;              //e+ annihilation
    
  private:
    // methods
    void ConstructProcessForGamma();    
    void ConstructProcessForElectron();    
    void ConstructProcessForPositron();    
};

#endif //TG4_PHYSICS_CONSTRUCTOR_H

