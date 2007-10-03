// $Id: TG4ComposedPhysicsMessenger.h,v 1.1 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup physics_list
/// \class TG4ComposedPhysicsMessenger
/// \brief Messenger class that defines commands for Geant4 VMC composed 
///        physics list and related classes
///
/// Implements commands:
/// - /mcPhysics/rangeCutForElectron 
/// - /mcPhysics/rangeCutForPositron
/// - /mcPhysics/rangeCutForGamma
/// - /mcPhysics/rangeCuts
/// - /mcPhysics/printProcessMCMap
/// - /mcPhysics/printProcessControlMap
/// - /mcPhysics/printVolumeLimits [volName]
/// - /mcPhysics/printGlobalCuts
/// - /mcPhysics/printGlobalControls
///
/// \author I. Hrivnacova; IPN Orsay

#ifndef TG4_COMPOSED_PHYSICS_MESSENGER_H
#define TG4_COMPOSED_PHYSICS_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4ComposedPhysicsList;

class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;

class TG4ComposedPhysicsMessenger : public G4UImessenger
{
  public:
    TG4ComposedPhysicsMessenger(TG4ComposedPhysicsList* physicsList); 
    virtual ~TG4ComposedPhysicsMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    TG4ComposedPhysicsMessenger();  
    TG4ComposedPhysicsMessenger(const TG4ComposedPhysicsMessenger& right);
    TG4ComposedPhysicsMessenger& operator=(const TG4ComposedPhysicsMessenger& right);

    // data members
    TG4ComposedPhysicsList*     fPhysicsList;        //associated class
    G4UIdirectory*              fDirectory;          //command directory
    G4UIcmdWithADoubleAndUnit*  fRangeGammaCutCmd;   //rangeCutForGamma command
    G4UIcmdWithADoubleAndUnit*  fRangeElectronCutCmd;//rangeCutForElectron command
    G4UIcmdWithADoubleAndUnit*  fRangePositronCutCmd;//rangeCutForPositron command
    G4UIcmdWithADoubleAndUnit*  fRangeAllCutCmd;     //rangeCuts command
    
    G4UIcmdWithoutParameter*  fPrintAllProcessesCmd; //printAllProcess command  
    G4UIcmdWithoutParameter*  fDumpAllProcessesCmd;  //dumpAllProcess command                                                  
    G4UIcmdWithoutParameter*  fPrintProcessMCMapCmd; //printProcessMCMap command
    G4UIcmdWithoutParameter*  fPrintProcessControlMapCmd;// printProcessControlsMap command
    G4UIcmdWithAString*       fPrintVolumeLimitsCmd; //printVolumeLimits command
    G4UIcmdWithoutParameter*  fPrintGlobalCutsCmd;   //printGeneralCuts command
    G4UIcmdWithoutParameter*  fPrintGlobalControlsCmd; //printGeneralControls command
};     

#endif //TG4_COMPOSED_PHYSICS_MESSENGER_H
