#ifndef TG4_RUN_CONFIGURATION_H
#define TG4_RUN_CONFIGURATION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RunConfiguration.h
/// \brief Definition of the TG4RunConfiguration class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <Rtypes.h>
#include <TString.h>

class TG4DetConstruction;
class TG4TrackingAction;
class TG4SteppingAction;
class TG4SpecialPhysicsList;
class TG4VUserRegionConstruction;

class G4VUserDetectorConstruction;
class G4VUserPrimaryGeneratorAction;
class G4VUserPhysicsList;
class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4RunManager;
class G4UImessenger;

/// \ingroup run
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
///
/// The second argument in the constructor selects physics list:
/// - emStandard         - standard em physics (default)
/// - emStandard+optical - standard em physics + optical physics
/// - XYZ                - selected hadron physics list ( XYZ = LHEP, QGSP, ...)
/// - XYZ+optical        - selected hadron physics list + optical physics
///
/// The third argument activates the special processes in the TG4SpecialPhysicsList,
/// which implement VMC features:
/// - stepLimiter       - step limiter (default) 
/// - specialCuts       - VMC cuts
/// - specialControls   - VMC controls for activation/inactivation selected processes
/// - stackPopper       - stackPopper process
/// When more than one options are selected, they should be separated with '+'
/// character: eg. stepLimit+specialCuts.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RunConfiguration 
{
  public:
    TG4RunConfiguration(const TString& userGeometry,
                        const TString& physicsList = "emStandard",
                        const TString& specialProcess = "stepLimiter",
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
    
    virtual TG4VUserRegionConstruction*  CreateUserRegionConstruction();

    // get methods
    TString  GetUserGeometry() const;
    TString  GetPhysicsListSelection() const;
    Bool_t   IsSpecialStacking() const;
    Bool_t   IsSpecialControls() const;
    Bool_t   IsSpecialCuts() const;

  protected:
    // data members
    TString        fUserGeometry;           ///< way of building geometry
    TString        fPhysicsListSelection;   ///< physics list selection
    TString        fSpecialProcessSelection;///< special process selection
    Bool_t         fSpecialStacking;        ///< option for special stacking
    Bool_t         fSpecialControls;        ///< option for special controls
    Bool_t         fSpecialCuts;            ///< option for special cuts
    G4UImessenger* fAGDDMessenger;          //!< XML messenger
    G4UImessenger* fGDMLMessenger;          //!< XML messenger

  private:
    /// Not implemented
    TG4RunConfiguration();
    /// Not implemented
    TG4RunConfiguration(const TG4RunConfiguration& right);
    /// Not implemented
    TG4RunConfiguration& operator=(const TG4RunConfiguration& right);
};

// inline functions

inline TString TG4RunConfiguration::GetPhysicsListSelection() const {
  /// Return physics list selection
  return fPhysicsListSelection;
}  

#endif //TG4V_RUN_CONFIGURATION_H

