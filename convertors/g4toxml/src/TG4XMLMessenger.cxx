// $Id: TG4XMLMessenger.cxx,v 1.1 2003/01/29 11:27:38 brun Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4XMLMessenger
// ---------------------
// See the class description in the header file.

#include "TG4XMLMessenger.h"
#include "TG4XMLGeometryGenerator.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

//_____________________________________________________________________________
TG4XMLMessenger::TG4XMLMessenger(TG4XMLGeometryGenerator* geometryGenerator)
  : fXMLGeometryGenerator(geometryGenerator)
{
//
  fDirectory = new G4UIdirectory("/xml/");
  fDirectory->SetGuidance("XML geometry generator control commands.");

  fGenerateXMLCmd = new G4UIcmdWithAString("/xml/generateXML", this);
  fGenerateXMLCmd->SetGuidance("Generate geometry XML file");
  fGenerateXMLCmd->SetGuidance("starting from a logical volume specified by name;");
  fGenerateXMLCmd->SetGuidance("if no name is given - the whole world is processed.");
  fGenerateXMLCmd->SetParameterName("lvName", true);
  fGenerateXMLCmd->SetDefaultValue("");
  fGenerateXMLCmd->AvailableForStates(G4State_Idle);   
}

//_____________________________________________________________________________
TG4XMLMessenger::TG4XMLMessenger() {
//
}

//_____________________________________________________________________________
TG4XMLMessenger::TG4XMLMessenger(const TG4XMLMessenger& right)
{
//
  G4cerr << "    TG4XMLMessenger is protected from copying." << G4endl;
  G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
  exit(1);
}

//_____________________________________________________________________________
TG4XMLMessenger::~TG4XMLMessenger() {
//
  delete fDirectory;
  delete fGenerateXMLCmd;
}

// operators

//_____________________________________________________________________________
TG4XMLMessenger& 
TG4XMLMessenger::operator=(const TG4XMLMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  G4cerr << "    TG4XMLMessenger is protected from assigning." << G4endl;
  G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
  exit(1);
    
  return *this;  
}    
          
// public methods
  
//_____________________________________________________________________________
void TG4XMLMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
// Applies command to the associated object.
// ---

  if (command == fGenerateXMLCmd) {    
    if (newValues == "") 
     fXMLGeometryGenerator->GenerateXMLGeometry();
    else 
     fXMLGeometryGenerator->GenerateXMLGeometry(newValues);
  }    
}
