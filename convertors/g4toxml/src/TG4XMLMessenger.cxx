// $Id: TG4XMLMessenger.cxx,v 1.1 2003/07/22 06:46:58 brun Exp $
//
// Author: I. Hrivnacova
//
// Class TG4XMLMessenger
// ---------------------
// See the class description in the header file.

#include "TG4XMLMessenger.h"
#include "TG4VXMLGeometryGenerator.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

G4UIdirectory* TG4XMLMessenger::fgDirectory = 0;
G4int  TG4XMLMessenger::fgCounter = 0;

//_____________________________________________________________________________
TG4XMLMessenger::TG4XMLMessenger(TG4VXMLGeometryGenerator* geometryGenerator,
                                 const G4String& xmlFormat)
  : fXMLGeometryGenerator(geometryGenerator)
{
//
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
  fgCounter--;
  if (fgCounter==0) {
    delete fgDirectory;
    fgDirectory = 0;
  }  
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
