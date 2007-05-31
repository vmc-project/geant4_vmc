// $Id: TG4DetConstructionMessenger.h,v 1.9 2007/03/22 09:03:12 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4DetConstructionMessenger
/// \brief Messenger class that defines commands for TG4DetConstruction.
///
/// Implements commands:
/// - /mcDet/fieldType [MCApplication|Uniform|None]
/// - /mcDet/volNameSeparator [char]  - for geomVMCtoGeant4 only
/// - /mcDet/uniformFieldValue 
/// - /mcDet/printMaterials 
/// - /mcDet/printMedia
/// - /mcDet/printVolumes
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_DET_CONSTRUCTION_MESSENGER_H
#define TG4_DET_CONSTRUCTION_MESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4GeometryManager;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class TG4DetConstructionMessenger: public G4UImessenger
{
  public:
    TG4DetConstructionMessenger(TG4GeometryManager* geometryManager);
    virtual ~TG4DetConstructionMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  private:
    TG4DetConstructionMessenger();
    TG4DetConstructionMessenger(const TG4DetConstructionMessenger& right);
    TG4DetConstructionMessenger& operator=(
                                const TG4DetConstructionMessenger &right);

    TG4GeometryManager*  fGeometryManager; //associated class
    G4UIdirectory*       fDirectory;       //command directory
    
    // commands data members
    G4UIcmdWithAString*         fFieldTypeCmd;        //command: fieldType
    G4UIcmdWithAString*         fSeparatorCmd;        //command: volumeNameSeparator
    G4UIcmdWithADoubleAndUnit*  fUniformFieldValueCmd;//command: uniformFieldValue
    G4UIcmdWithoutParameter*    fPrintMaterialsCmd;   //command: printMatrials     
    G4UIcmdWithoutParameter*    fPrintMediaCmd;       //command: printMedia     
    G4UIcmdWithoutParameter*    fPrintVolumesCmd;     //command: printVolumes    
};

#endif //TG4_DET_CONSTRUCTION_MESSENGER_H

