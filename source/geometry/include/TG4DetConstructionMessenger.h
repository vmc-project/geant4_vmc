// $Id: TG4DetConstructionMessenger.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4DetConstructionMessenger
// ---------------------------------
// Messenger class that defines commands for TG4DetConstruction.

#ifndef TG4_DET_CONSTRUCTION_MESSENGER_H
#define TG4_DET_CONSTRUCTION_MESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4DetConstruction;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class TG4DetConstructionMessenger: public G4UImessenger
{
  public:
    TG4DetConstructionMessenger(TG4DetConstruction* modulesComposition);
    // --> protected
    // TG4DetConstructionMessenger();
    // TG4DetConstructionMessenger(const TG4DetConstructionMessenger& right);
    virtual ~TG4DetConstructionMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  protected:
    TG4DetConstructionMessenger();
    TG4DetConstructionMessenger(const TG4DetConstructionMessenger& right);

    // operators
    TG4DetConstructionMessenger& operator=(
                                const TG4DetConstructionMessenger &right);
             
  private:
    TG4DetConstruction*  fDetConstruction; //associated class
    G4UIdirectory*       fDirectory;       //command directory
    
    // commands data members
    G4UIcmdWithAString*         fFieldTypeCmd;        //command: fieldType
    G4UIcmdWithADoubleAndUnit*  fUniformFieldValueCmd;//command: uniformFieldValue
    G4UIcmdWithABool*           fSetReadGeometryCmd;  //command: readGeometry   
    G4UIcmdWithABool*           fSetWriteGeometryCmd; //command: writeGeometry    
    G4UIcmdWithoutParameter*    fPrintMaterialsCmd;   //command: printMatrials     
    G4UIcmdWithAString*         fGenerateXMLCmd;      //command: generateXML
};

#endif //TG4_DET_CONSTRUCTION_MESSENGER_H

