// $Id: TG4UniformMagneticField.cxx,v 1.3 2004/11/10 11:39:28 brun Exp $
// Category: geometry
//
// Class TG4UniformMagneticField
// -----------------------------
// See the class description in the header file.
// According to:
// Id: ExN02MagneticField.cc,v 1.1 1999/01/07 16:05:49 gunter Exp 
// GEANT4 tag Name: geant4-00-01
//
// Author: I. Hrivnacova

#include "TG4UniformMagneticField.h"

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>

//  Constructors

//_____________________________________________________________________________
TG4UniformMagneticField::TG4UniformMagneticField()
  : G4UniformMagField(G4ThreeVector()) 
{
/// Default constructor

  GetGlobalFieldManager()->SetDetectorField(this);
  GetGlobalFieldManager()->CreateChordFinder(this);
}

//_____________________________________________________________________________
TG4UniformMagneticField::TG4UniformMagneticField(G4ThreeVector fieldVector)
  : G4UniformMagField(fieldVector)
{    
/// Constructor from given fieldVector

  GetGlobalFieldManager()->SetDetectorField(this);
  GetGlobalFieldManager()->CreateChordFinder(this);
}

//_____________________________________________________________________________
TG4UniformMagneticField::TG4UniformMagneticField(const TG4UniformMagneticField& right)
  : G4UniformMagField(right)
{
/// Copy constructor

  GetGlobalFieldManager()->SetDetectorField(this);
  GetGlobalFieldManager()->CreateChordFinder(this);
}

//_____________________________________________________________________________
TG4UniformMagneticField::~TG4UniformMagneticField() 
{
/// Destructor
}

//
// operators
//

//_____________________________________________________________________________
TG4UniformMagneticField& 
TG4UniformMagneticField::operator=(const TG4UniformMagneticField& right)
{                                  
  // check assignement to self
  if (this == &right) return *this;

  // base class assignement
  G4UniformMagField::operator=(right);
  
  return *this;
}  

//
// public methods
//

//_____________________________________________________________________________
void TG4UniformMagneticField::SetFieldValue(G4double fieldValue)
{
/// Set the value of the global field to fieldValue along Z.

  G4UniformMagField::SetFieldValue(G4ThreeVector(0,0,fieldValue));
}

//_____________________________________________________________________________
void TG4UniformMagneticField::SetFieldValue(G4ThreeVector fieldVector)
{
/// Set the value of the global field.

  // Find the Field Manager for the global field
  G4FieldManager* fieldMgr= GetGlobalFieldManager();
    
  if(fieldVector!=G4ThreeVector(0.,0.,0.)) { 
    G4UniformMagField::SetFieldValue(fieldVector); 
    fieldMgr->SetDetectorField(this);
  } 
  else {
    // If the new field's value is Zero, then it is best to
    //  insure that it is not used for propagation.
    G4MagneticField* magField = 0;
    fieldMgr->SetDetectorField(magField);
  }
}

//_____________________________________________________________________________
G4FieldManager*  TG4UniformMagneticField::GetGlobalFieldManager() const
{
/// Utility method

  return G4TransportationManager::GetTransportationManager()
           ->GetFieldManager();
}
    
