//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MediumMap.cxx
/// \brief Implementation of the TG4MediumMap.cxx class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>

#include <iomanip>

//_____________________________________________________________________________
TG4MediumMap::TG4MediumMap()
  : fIdMap(),
    fLVMap(),
    fMaterialMap()
{
/// Default constructor
}

//_____________________________________________________________________________
TG4MediumMap::~TG4MediumMap()
{
/// Destructor

  // Delete medium objects
  std::map<G4int, TG4Medium*>::const_iterator it;
  for ( it = fIdMap.begin(); it != fIdMap.end(); ++it ) delete it->second;
}

//
// public methods
//


//_____________________________________________________________________________
TG4Medium* TG4MediumMap::AddMedium(G4int mediumID, G4bool warn)
{
/// Create a new medium with given mediumID and name if it is not yet
/// present in the map, and add it to the map.

  TG4Medium* medium = GetMedium(mediumID, false);

  if ( medium ) {
    if ( warn) {
      TString text = "Medium with given ID=";
      text += mediumID;
      text += " already exists.";
      TG4Globals::Warning(
        "TG4MediumMap", "AddMedium", text);
    }
    return medium;
  }

  medium = new TG4Medium(mediumID);
  fIdMap[mediumID] = medium;
  return medium;
}

//_____________________________________________________________________________
void TG4MediumMap::MapMedium(G4LogicalVolume* lv, G4int mediumID)
{
/// Map the medium with the given \em mediumID to the given logical volume
/// \em lv

  TG4Medium* medium = GetMedium(mediumID);

  if ( !medium ) {
    TString text = "Medium with given ID=";
    text += mediumID;
    text += " not defined.";
    TG4Globals::Warning(
      "TG4MediumMap", "MapMedium", text);
    return;
  }

  fLVMap[lv]= medium;

  if ( fMaterialMap.find(lv->GetMaterial()) == fMaterialMap.end() ) {
    fMaterialMap[lv->GetMaterial()]= medium;
  }
}

//_____________________________________________________________________________
void TG4MediumMap::MapMedium(const G4String& lvName, G4int mediumID)
{
/// Map the medium with the given \em mediumID to the logical volume
/// with given name \em lvName

  G4LogicalVolume* lv
    = TG4GeometryServices::Instance()->FindLogicalVolume(lvName);

  if (!lv ) {
    TG4Globals::Warning(
      "TG4MediumMap", "MapMedium",
      "Logical volume " + TString(lvName) + " not defined.");
    return;
  }

  MapMedium(lv, mediumID);
}

//_____________________________________________________________________________
void TG4MediumMap::Print() const
{
/// Print the whole map.

  if (fIdMap.size()) {
    G4cout << "Dump of TG4MediumMap - " << fIdMap.size() << " entries:" << G4endl;
    G4int counter = 0;
    std::map<G4int, TG4Medium*>::const_iterator it;
    for ( it = fIdMap.begin(); it != fIdMap.end(); ++it ) {
      G4cout << "Map element " << std::setw(3) << counter++ << "   ";
      it->second->Print();
      G4cout << G4endl;
    }
  }
}

//_____________________________________________________________________________
G4int TG4MediumMap::GetNofMedia() const
{
/// Return the number of media registered in the map

  return fIdMap.size();
}

//_____________________________________________________________________________
TG4Medium* TG4MediumMap::GetMedium(G4int mediumID, G4bool warn) const
{
/// Return medium with the given mediumID

  std::map<G4int, TG4Medium*>::const_iterator it
    = fIdMap.find(mediumID);

  if ( it == fIdMap.end()) {
    if (warn) {
      TString text = "Medium with given ID=";
      text += mediumID;
      text += " not defined.";
      TG4Globals::Warning(
        "TG4MediumMap", "GetMedium", text);
    }
    return 0;
  }

  return it->second;
}

//_____________________________________________________________________________
TG4Medium*  TG4MediumMap::GetMedium(const G4String& name, G4bool warn) const
{
/// Return medium with the given name

  std::map<G4int, TG4Medium*>::const_iterator it;
  for ( it = fIdMap.begin(); it != fIdMap.end(); ++it )
    if ( it->second->GetName() == name ) return it->second;

  // Give warning if not found
  if ( warn ) {
    TG4Globals::Warning(
      "TG4MediumMap", "GetMedium",
      "Medium " + TString(name) + " is not found.");
  }
  return 0;
}

//_____________________________________________________________________________
TG4Medium*  TG4MediumMap::GetMedium(G4LogicalVolume* lv, G4bool warn) const
{
/// Return medium for the given lv

  std::map<G4LogicalVolume*, TG4Medium*>::const_iterator it
    = fLVMap.find(lv);

  if (it == fLVMap.end()) {
    if ( warn ) {
      TG4Globals::Warning(
        "TG4MediumMap", "GetMedium",
        "Medium for LV " + TString(lv->GetName()) + " is not found.");
    }
    return 0;
  }

  return it->second;
}

//_____________________________________________________________________________
TG4Medium*  TG4MediumMap::GetMedium(const G4Material* material, G4bool warn) const
{
/// Return medium for the given material

  std::map<const G4Material*, TG4Medium*>::const_iterator it
    = fMaterialMap.find(material);

  if ( it == fMaterialMap.end() ) {
    if ( warn ) {
      TG4Globals::Warning(
        "TG4MediumMap", "GetMedium",
        "Medium for material " + TString(material->GetName()) + " is not found.");
    }
    return 0;
  }

  return it->second;
}

