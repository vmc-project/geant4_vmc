// $Id: TG4XmlVGMMessenger.cxx,v 1.2 2004/04/26 17:05:04 brun Exp $
//
// Author: I. Hrivnacova
//
// Class TG4XmlVGMMessenger
// -------------------------
// See the class description in the header file.

#ifdef USE_VGM

#include "TG4XmlVGMMessenger.h"
#include "TG4GeometryServices.h"

#include <Geant4GM/volumes/Factory.h>
#include <XmlVGM/AGDDExporter.h>
#include <XmlVGM/GDMLExporter.h>

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

G4UIdirectory* TG4XmlVGMMessenger::fgDirectory = 0;
G4int  TG4XmlVGMMessenger::fgCounter = 0;

//_____________________________________________________________________________
TG4XmlVGMMessenger::TG4XmlVGMMessenger(const G4String& xmlFormat)
  : fG4Factory(new Geant4GM::Factory()),
    fXmlVGMExporter(0)
{
//
  if (xmlFormat == "AGDD")
    fXmlVGMExporter = new XmlVGM::AGDDExporter(fG4Factory);
  if (xmlFormat == "GDML")
    fXmlVGMExporter = new XmlVGM::GDMLExporter(fG4Factory);

  if (!fgDirectory) {
    fgDirectory = new G4UIdirectory("/xml/");
    fgDirectory->SetGuidance("XML geometry generator control commands.");
  }  

  G4String cmdName("/xml/generate");
  cmdName = cmdName + xmlFormat;
  fGenerateXMLCmd = new G4UIcmdWithAString(cmdName, this);
  fGenerateXMLCmd->SetGuidance("Generate geometry XML file");
  fGenerateXMLCmd->SetGuidance("starting from a logical volume specified by name;");
  fGenerateXMLCmd->SetGuidance("if no name is given - the whole world is processed.");
  fGenerateXMLCmd->SetParameterName("lvName", true);
  fGenerateXMLCmd->SetDefaultValue("");
  fGenerateXMLCmd->AvailableForStates(G4State_Idle); 

  fgCounter++; 
}


//_____________________________________________________________________________
TG4XmlVGMMessenger::TG4XmlVGMMessenger() {
//
}

//_____________________________________________________________________________
TG4XmlVGMMessenger::TG4XmlVGMMessenger(const TG4XmlVGMMessenger& right)
{
//
  G4cerr << "    TG4XmlVGMMessenger is protected from copying." << G4endl;
  G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
  exit(1);
}

//_____________________________________________________________________________
TG4XmlVGMMessenger::~TG4XmlVGMMessenger() {
//

  delete fG4Factory;
  delete fXmlVGMExporter;

  fgCounter--;
  if (fgCounter==0) {
    delete fgDirectory;
    fgDirectory = 0;
  }  
  delete fGenerateXMLCmd;
}

// operators

//_____________________________________________________________________________
TG4XmlVGMMessenger& 
TG4XmlVGMMessenger::operator=(const TG4XmlVGMMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  G4cerr << "    TG4XmlVGMMessenger is protected from assigning." << G4endl;
  G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
  exit(1);
    
  return *this;  
}    

// public methods
  
//_____________________________________________________________________________
void TG4XmlVGMMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
// Applies command to the associated object.
// ---

  if (!fG4Factory->Top()) {
     // Import geometry in VGM
    // fG4Factory->SetDebug(1);
    fG4Factory->Import(TG4GeometryServices::Instance()->GetWorld());
  }  

  if (command == fGenerateXMLCmd) {    
    if (newValues == "") 
      fXmlVGMExporter->GenerateXMLGeometry();
    else 
      fXmlVGMExporter->GenerateXMLGeometry(newValues);
  }	
}

#endif //USE_VGM
