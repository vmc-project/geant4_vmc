// $Id: TG4DetConstructionMessenger.cxx,v 1.7 2004/11/10 11:39:28 brun Exp $
// Category: geometry
//
// Class TG4DetConstructionMessenger
// ------------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4DetConstructionMessenger.h"
#include "TG4DetConstruction.h"
#include "TG4MagneticFieldType.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

//_____________________________________________________________________________
TG4DetConstructionMessenger::TG4DetConstructionMessenger(
                                   TG4DetConstruction* detConstruction)
  : fDetConstruction(detConstruction),
    fDirectory(0)
    
{
//
  fDirectory = new G4UIdirectory("/mcDet/");
  fDirectory->SetGuidance("Detector construction control commands.");

  fFieldTypeCmd = new G4UIcmdWithAString("/mcDet/fieldType", this);
  G4String guidance =   "Select type of magnetic field:\n";
  guidance = guidance + "  MCApplication:  field defined by VMC application (default)\n";
  guidance = guidance + "  Uniform:        uniform magnetic field\n";
  guidance = guidance + "  None:           no magnetic field";
  fFieldTypeCmd->SetGuidance(guidance);
  fFieldTypeCmd->SetParameterName("FieldType", true);
  fFieldTypeCmd->SetCandidates("MCApplication Uniform None");   
  fFieldTypeCmd->SetDefaultValue("MCApplication");
  fFieldTypeCmd->AvailableForStates(G4State_PreInit);

  fSeparatorCmd = new G4UIcmdWithAString("/mcDet/volNameSeparator", this);
  guidance =   "Override the default value of the volume name separator:\n";
  guidance = guidance + "  in roottog4 (in g3tog4 this is not yet available) \n";
  fSeparatorCmd->SetGuidance(guidance);
  fSeparatorCmd->SetParameterName("VolNameSeparator", true);
  fSeparatorCmd->AvailableForStates(G4State_PreInit);

  fUniformFieldValueCmd 
    = new G4UIcmdWithADoubleAndUnit("/mcDet/uniformFieldValue", this);
  fUniformFieldValueCmd
    ->SetGuidance("Define uniform magnetic field in Z direction.");
  fUniformFieldValueCmd
    ->SetGuidance("(Uniform magnetic field type has to be selected first.)");
  fUniformFieldValueCmd->SetParameterName("UniformFieldValue", false, false);
  fUniformFieldValueCmd->SetDefaultUnit("tesla");
  fUniformFieldValueCmd->SetUnitCategory("Magnetic flux density");
  fUniformFieldValueCmd->AvailableForStates(G4State_Idle);  
  
  fSetReadGeometryCmd 
    = new G4UIcmdWithABool("/mcDet/readGeometry", this);
  fSetReadGeometryCmd->SetGuidance("Read geometry from g3calls.dat files");
  fSetReadGeometryCmd->SetParameterName("readGeometry", false);
  fSetReadGeometryCmd->AvailableForStates(G4State_PreInit);  
 
  fSetWriteGeometryCmd 
    = new G4UIcmdWithABool("/mcDet/writeGeometry", this);
  fSetWriteGeometryCmd->SetGuidance("Write geometry to g3calls.dat file");
  fSetWriteGeometryCmd->SetParameterName("writeGeometry", false);
  fSetWriteGeometryCmd->AvailableForStates(G4State_PreInit);   

  fPrintMaterialsCmd 
    = new G4UIcmdWithoutParameter("/mcDet/printMaterials", this);
  fPrintMaterialsCmd->SetGuidance("Prints all materials.");
  fPrintMaterialsCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);   

 #ifdef USE_VGM
 fUseVGMCmd 
    = new G4UIcmdWithABool("/mcDet/useVGM", this);
  fUseVGMCmd->SetGuidance("Use VGM for Root->Geant4 geometry conversion");
  fUseVGMCmd->SetParameterName("writeGeometry", false);
  fUseVGMCmd->AvailableForStates(G4State_PreInit);   
#endif

}

//_____________________________________________________________________________
TG4DetConstructionMessenger::TG4DetConstructionMessenger() {
//
}

//_____________________________________________________________________________
TG4DetConstructionMessenger::TG4DetConstructionMessenger(
                                const TG4DetConstructionMessenger& right)
{
//
  TG4Globals::Exception(
    "TG4DetConstructionMessenger is protected from copying.");
}

//_____________________________________________________________________________
TG4DetConstructionMessenger::~TG4DetConstructionMessenger() {
//
  delete fDirectory;
  delete fFieldTypeCmd;
  delete fSeparatorCmd;
  delete fUniformFieldValueCmd;
  delete fSetReadGeometryCmd;
  delete fSetWriteGeometryCmd;
  delete fPrintMaterialsCmd;
#ifdef USE_VGM
  delete fUseVGMCmd;
#endif
}

//
// operators
//

//_____________________________________________________________________________
TG4DetConstructionMessenger& 
TG4DetConstructionMessenger::operator=(
                                const TG4DetConstructionMessenger& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
     "TG4DetConstructionMessenger is protected from assigning.");
    
  return *this;  
}    
          
//
// public methods
//
  
//_____________________________________________________________________________
void TG4DetConstructionMessenger::SetNewValue(G4UIcommand* command, 
                                               G4String newValues)
{
/// Apply command to the associated object.

  if( command == fFieldTypeCmd ) { 
    if (newValues == "MCApplication") 
      fDetConstruction->SetFieldType(kMCApplicationField); 
    if (newValues == "Uniform") 
      fDetConstruction->SetFieldType(kUniformField); 
    if (newValues == "None") 
      fDetConstruction->SetFieldType(kNoField); 
  }
  else if( command == fSeparatorCmd ) { 
    char separator = newValues(0);
    TG4GeometryServices::Instance()->SetSeparator(separator);
  }
  if (command == fUniformFieldValueCmd) {  
    fDetConstruction
      ->SetUniformFieldValue(fUniformFieldValueCmd->GetNewDoubleValue(newValues)); 
  }
  else if (command == fSetReadGeometryCmd) {
    fDetConstruction->SetReadGeometry(
                         fSetReadGeometryCmd->GetNewBoolValue(newValues));
  }  
  else if (command == fSetWriteGeometryCmd) {
    fDetConstruction->SetWriteGeometry(
                         fSetWriteGeometryCmd->GetNewBoolValue(newValues));
  }    
  else if (command == fPrintMaterialsCmd) {
    fDetConstruction->PrintMaterials();
  }    
#ifdef USE_VGM
  else if (command == fUseVGMCmd) {
    fDetConstruction->SetUseVGM(
                         fUseVGMCmd->GetNewBoolValue(newValues));
  }    
#endif
}
