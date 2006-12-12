// $Id: TG4PhysicsConstructorMuon.h,v 1.5 2006/01/13 16:59:38 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsConstructorMuon
/// \brief Constructor of electromagnetic physics.
///
/// According to ExN04MuonPhysics.hh, GEANT4 tag Name: geant4-06-00
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_CONSTRUCTOR_MUON_H
#define TG4_PHYSICS_CONSTRUCTOR_MUON_H

#include "TG4VPhysicsConstructor.h"

#include <globals.hh>

class G4MultipleScattering;
class G4MuBremsstrahlung;
class G4MuPairProduction;
class G4MuIonisation;
class G4MuonMinusCaptureAtRest;
class G4hIonisation;

class TG4PhysicsConstructorMuon: public TG4VPhysicsConstructor
{
  public:
    TG4PhysicsConstructorMuon(const G4String& name = "Muon");
    TG4PhysicsConstructorMuon(G4int verboseLevel,
                              const G4String& name = "Muon");
    virtual ~TG4PhysicsConstructorMuon();

  protected:
    // methods
          // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

    // data members
          // mu+ physics
   G4MultipleScattering*   fMuPlusMultipleScattering;  //mu+ msc
   G4MuBremsstrahlung*     fMuPlusBremsstrahlung ;     //mu+ Bremsstrahlung
   G4MuPairProduction*     fMuPlusPairProduction;      //mu+ pair production
   G4MuIonisation*         fMuPlusIonisation;          //mu+ ionisation

          // mu- physics
   G4MultipleScattering*   fMuMinusMultipleScattering; //mu- msc
   G4MuBremsstrahlung*     fMuMinusBremsstrahlung ;    //mu- Bremsstrahlung
   G4MuPairProduction*     fMuMinusPairProduction;     //mu- pair production
   G4MuIonisation*         fMuMinusIonisation;         //mu- ionisation
   G4MuonMinusCaptureAtRest* fMuMinusCaptureAtRest;    //mu- capture

          // tau+ physics
   G4MultipleScattering*   fTauPlusMultipleScattering; //tau+ msc
   G4hIonisation*          fTauPlusIonisation;         //tau+ ionisation

          // tau+ physics
   G4MultipleScattering*   fTauMinusMultipleScattering;//tau- msc
   G4hIonisation*          fTauMinusIonisation;        //tau- ionisation
    
  private:
    TG4PhysicsConstructorMuon(const TG4PhysicsConstructorMuon& right);
    TG4PhysicsConstructorMuon& operator=(const TG4PhysicsConstructorMuon& right);

    // methods
    void ConstructProcessForMuonPlus();    
    void ConstructProcessForMuonMinus();    
    void ConstructProcessForTauPlus();    
    void ConstructProcessForTauMinus();    
};

#endif //TG4_PHYSICS_CONSTRUCTOR_MUON_H

