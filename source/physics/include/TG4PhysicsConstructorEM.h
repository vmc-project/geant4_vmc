// $Id: TG4PhysicsConstructorEM.h,v 1.6 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics
/// \class TG4PhysicsConstructorEM
/// \brief Constructor of electromagnetic physics.
///
/// According to ExN04EMPhysics, GEANT4 tag Name: geant4-06-00
///
/// \author I. Hrivnacova; IPN Orsay

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
    TG4PhysicsConstructorEM(const TG4PhysicsConstructorEM& right);
    TG4PhysicsConstructorEM& operator=(const TG4PhysicsConstructorEM& right);

    // methods
    void ConstructProcessForGamma();    
    void ConstructProcessForElectron();    
    void ConstructProcessForPositron();    
};

#endif //TG4_PHYSICS_CONSTRUCTOR_H

