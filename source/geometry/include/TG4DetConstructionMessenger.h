#ifndef TG4_DET_CONSTRUCTION_MESSENGER_H
#define TG4_DET_CONSTRUCTION_MESSENGER_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4DetConstructionMessenger.h
/// \brief Definition of the TG4DetConstructionMessenger class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4GeometryManager;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

/// \ingroup geometry
/// \brief Messenger class that defines commands for TG4DetConstruction.
///
/// Implements commands:
/// - /mcDet/updateMagField
/// - /mcDet/volNameSeparator [char]  - for geomVMCtoGeant4 only
/// - /mcDet/printMaterials 
/// - /mcDet/printMaterialsProperties 
/// - /mcDet/printMedia
/// - /mcDet/printVolumes
/// - /mcDet/printCuts cutName
/// - /mcDet/printControls controlName
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4DetConstructionMessenger: public G4UImessenger
{
  public:
    TG4DetConstructionMessenger(TG4GeometryManager* geometryManager);
    virtual ~TG4DetConstructionMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  private:
    /// Not implemented
    TG4DetConstructionMessenger();
    /// Not implemented
    TG4DetConstructionMessenger(const TG4DetConstructionMessenger& right);
    /// Not implemented
    TG4DetConstructionMessenger& operator=(
                                const TG4DetConstructionMessenger &right);

    // \data members
    TG4GeometryManager*  fGeometryManager; ///< associated class
    G4UIdirectory*       fDirectory;       ///< command directory
    
    //
    // commands data members
    
    /// command: updateMagField     
    G4UIcmdWithoutParameter*    fUpdateMagFieldCmd;
    
    /// command: volumeNameSeparator
    G4UIcmdWithAString*         fSeparatorCmd;
    
    /// command: printMatrials     
    G4UIcmdWithoutParameter*    fPrintMaterialsCmd;
    
    /// command: printMaterialsProperties     
    G4UIcmdWithoutParameter*    fPrintMaterialsPropertiesCmd;
    
    /// command: printMedia     
    G4UIcmdWithoutParameter*    fPrintMediaCmd;
    
    /// command: printVolumes  
    G4UIcmdWithoutParameter*    fPrintVolumesCmd;  

    /// command: printCuts  
    G4UIcmdWithAString*         fPrintCutsCmd;  

    /// command: printControls  
    G4UIcmdWithAString*         fPrintControlsCmd;  
};

#endif //TG4_DET_CONSTRUCTION_MESSENGER_H

