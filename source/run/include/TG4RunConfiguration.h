// $Id: TG4RunConfiguration.h,v 1.6 2006/04/12 10:37:24 brun Exp $
/// \ingroup run
//
/// \class TG4RunConfiguration
/// \brief Takes care of creating Geant4 user action classes using VMC
///
/// This class takes care of creating all Geant4 user defined mandatory
/// and action classes that will be initialized and managed by Geant4
/// kernel (G4RunManager).
///
/// The class can be extended in a user application by inheritence;
/// this gives a user possibility to extend each Geant4 user defined class.
///
/// Author: I. Hrivnacova

#ifndef TG4_RUN_CONFIGURATION_H
#define TG4_RUN_CONFIGURATION_H

#include "TG4PhysicsListOptions.h"

#include <Rtypes.h>

class TG4DetConstruction;
class TG4TrackingAction;
class TG4SteppingAction;

class G4VUserDetectorConstruction;
class G4VUserPrimaryGeneratorAction;
class G4VUserPhysicsList;
class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4RunManager;
class G4UImessenger;

class TG4RunConfiguration 
{
  public:
    TG4RunConfiguration(Bool_t specialStacking = false);
    
    // --> protected
    // TG4RunConfiguration(const TG4RunConfiguration& right);
    virtual ~TG4RunConfiguration();

    // methods 
    //
    virtual G4VUserDetectorConstruction*   CreateDetectorConstruction();
    virtual G4VUserPhysicsList*            CreatePhysicsList();
    virtual G4VUserPrimaryGeneratorAction* CreatePrimaryGenerator();

    virtual G4UserRunAction*      CreateRunAction();  
    virtual G4UserEventAction*    CreateEventAction(); 
    virtual TG4TrackingAction*    CreateTrackingAction();
    virtual TG4SteppingAction*    CreateSteppingAction();
    virtual G4UserStackingAction* CreateStackingAction(); 

    // set methods
    virtual void SetPhysicsListOptions(const TG4PhysicsListOptions& options);

    // get methods
    G4VUserPhysicsList*  GetPhysicsList() const;
    Bool_t IsSpecialStacking() const;

  protected:
    TG4RunConfiguration(const TG4RunConfiguration& right);

    // operators
    TG4RunConfiguration& operator=(const TG4RunConfiguration& right);

    // data members
    Bool_t                 fSpecialStacking;
    G4VUserPhysicsList*    fPhysicsList;
    TG4PhysicsListOptions  fPhysicsListOptions;
    G4UImessenger*         fAGDDMessenger;    //! XML messenger
    G4UImessenger*         fGDMLMessenger;    //! XML messenger
};

#endif //TG4V_RUN_CONFIGURATION_H

