// $Id: TG4DetConstructionMessenger.h,v 1.7 2005/09/01 10:04:32 brun Exp $
/// \ingroup geometry
//
/// \class TG4DetConstructionMessenger
/// \brief Messenger class that defines commands for TG4DetConstruction.
///
/// Author: I. Hrivnacova

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
    G4UIcmdWithoutParameter*    fPrintVolumesCmd;     //command: printVolumes    
};

#endif //TG4_DET_CONSTRUCTION_MESSENGER_H

