// $Id: TG4DetConstruction.cxx,v 1.9 2005/05/17 13:43:57 brun Exp $
// Category: geometry
//
// Class TG4DetConstruction
// ---------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4DetConstruction.h"
#include "TG4GeometryManager.h"
#include "TG4GeometryServices.h"
#include "TG4MagneticField.h"
#include "TG4UniformMagneticField.h"
#include "TG4LVTree.h"
#include "TG4Globals.h"

#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>

#ifdef USE_VGM
#include <XmlVGM/AGDDExporter.h>
#include <XmlVGM/GDMLExporter.h>
#endif

#include <TVirtualMCApplication.h>

//_____________________________________________________________________________
TG4DetConstruction::TG4DetConstruction()
  : TG4Verbose("detConstruction"),
    fMessenger(this),
#ifdef USE_VGM
    fAGDDMessenger("AGDD"),
    fGDMLMessenger("GDML"),
#endif    
    fMagneticFieldType(kMCApplicationField), 
    fMagneticField(0), 
    fReadGeometry(false),
    fWriteGeometry(false) {
//

  // instantiate LVtree browser
  TG4LVTree::Instance();
}

//_____________________________________________________________________________
TG4DetConstruction::TG4DetConstruction(const TG4DetConstruction& right)
  : TG4Verbose("detConstruction"),
    fMessenger(this),
#ifdef USE_VGM
    fAGDDMessenger("AGDD"),
    fGDMLMessenger("GDML"),
#endif    
    fMagneticFieldType(kMCApplicationField), 
    fMagneticField(0), 
    fReadGeometry(false),
    fWriteGeometry(false)
{
//
  TG4Globals::Exception("TG4DetConstruction is protected from copying.");  
}

//_____________________________________________________________________________
TG4DetConstruction::~TG4DetConstruction() {
//   
  delete fMagneticField;
 
  // delete LVtree browser
  delete TG4LVTree::Instance();
}

//
// operators
//

//_____________________________________________________________________________
TG4DetConstruction& 
TG4DetConstruction::operator=(const TG4DetConstruction& right)
{
  // check assignement to self
  if (this == &right) return *this;
  
  TG4Globals::Exception("TG4DetConstruction is protected from assigning.");  

  return *this;  
}    
          
//
// private methods
//

//_____________________________________________________________________________
void TG4DetConstruction::CreateMagneticField()
{
/// Create standard magnetic field (defined by TVirtualMCApplication).

  switch (fMagneticFieldType) {
  
    case kMCApplicationField:
      fMagneticField = new TG4MagneticField();
      G4cout << "kMCApplicationField" << G4endl;
      break;

    case kUniformField:
      fMagneticField = new TG4UniformMagneticField();
      G4cout << "kUniformField" << G4endl;
      break;
      
    case kNoField:
      G4cout << "kNoField" << G4endl;
      ;;
  }  
}

//
// public methods
//

//_____________________________________________________________________________
G4VPhysicalVolume* TG4DetConstruction::Construct()
{ 
/// Construct geometry using the VMC application.
// --

  // create magnetic field
  CreateMagneticField();  

  // get geometry manager
  TG4GeometryManager* pGeometryManager = TG4GeometryManager::Instance();

  if (fReadGeometry) {

    // read geometry from g3calls.dat file using g3tog4 methods       
    pGeometryManager->SetWriteGeometry(false);
    pGeometryManager->ReadG3Geometry("g3calls.dat");
  }
  else {	            
    if (fWriteGeometry)  {
      // set geometry output stream for this module
      pGeometryManager->SetWriteGeometry(fWriteGeometry);
      pGeometryManager->OpenOutFile("g3calls.dat");
    }  
        
    // VMC application construct geometry 
    TVirtualMCApplication::Instance()->ConstructGeometry();   

    // close output file      
    if (fWriteGeometry) pGeometryManager->CloseOutFile();
  }  
  
  if (pGeometryManager->IsVMCGeometry()) {
  
    // construct G4 geometry
    pGeometryManager->CreateG4Geometry();

    // reset TG4GeometryManager 
    pGeometryManager->ClearG3Tables();
  }  

  // VMC application construct geometry for optical processes
  TVirtualMCApplication::Instance()->ConstructOpGeometry();   

  return TG4GeometryServices::Instance()->GetWorld();      
}

//_____________________________________________________________________________
void TG4DetConstruction::SetFieldType(TG4MagneticFieldType fieldType)
{
/// Select from available magnetic field types:
/// field defined by TVirtualMCApplication, uniform magnetic field
/// or no magnetic field.                                                    \n
/// Applicable only when no field has been yet created (PreInit).

  if (fMagneticField) {
     G4String text = "TG4DetConstruction::SetFieldType :\n";
     text = text + "    The magnetic field already exists.";
     text = text + "    Selection was ignored.";
     TG4Globals::Warning(text);
  }   

  fMagneticFieldType = fieldType;
}  

//_____________________________________________________________________________
void TG4DetConstruction::SetUniformFieldValue(G4double fieldValue)
{
/// Set uniform magnetic field to specified value.
  
  if (!fMagneticField) {
     G4String text = "TG4DetConstruction::SetUniformMagField: \n";
     text = text + "    Magnetic field i not defined.";
     TG4Globals::Exception(text);
  }   

  // Check field type 
  TG4UniformMagneticField* uniformField 
    =dynamic_cast<TG4UniformMagneticField*>(fMagneticField);

  if (!uniformField) {
     G4String text = "TG4DetConstruction::SetUniformMagField: \n";
     text = text + "    Defined magnetic field is not uniform.";
     TG4Globals::Exception(text);
  }   

  // Set value
  uniformField->SetFieldValue(fieldValue);
}

