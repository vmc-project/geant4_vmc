// $Id: TG4RunConfiguration.h,v 1.9 2007/02/06 11:06:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup run
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
/// In constructor user has to specify the geometry input
/// and select geometry navigation via the following options:
/// - geomVMCtoGeant4   - geometry defined via VMC, G4 native navigation
/// - geomVMCtoRoot     - geometry defined via VMC, Root navigation
/// - geomRoot          - geometry defined via Root, Root navigation
/// - geomRootToGeant4  - geometry defined via Root, G4 native navigation
/// - geomGeant4        - geometry defined via Geant4, G4 native navigation

/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_RUN_CONFIGURATION_H
#define TG4_RUN_CONFIGURATION_H

#include "TG4PhysicsListOptions.h"

#include <Rtypes.h>
#include <TString.h>

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
    TG4RunConfiguration(const TString& userGeometry,
                        Bool_t specialStacking = false);
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
    TString  GetUserGeometry() const;
    Bool_t   IsSpecialStacking() const;

  protected:
    // data members
    TString                fUserGeometry;     //  way of building geometry
    Bool_t                 fSpecialStacking;  //  option to include special stacking
    G4VUserPhysicsList*    fPhysicsList;      //  user physics list
    TG4PhysicsListOptions  fPhysicsListOptions;// physics list options
    G4UImessenger*         fAGDDMessenger;    //! XML messenger
    G4UImessenger*         fGDMLMessenger;    //! XML messenger

  private:
    TG4RunConfiguration();
    TG4RunConfiguration(const TG4RunConfiguration& right);
    TG4RunConfiguration& operator=(const TG4RunConfiguration& right);
};

#endif //TG4V_RUN_CONFIGURATION_H

