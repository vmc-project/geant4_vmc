//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Medium.cxx
/// \brief Implementation of the TG4Medium class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4Globals.h"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4UserLimits.hh>

const G4String TG4Medium::fgkUndefinedName = "UndefinedMediumName";
const G4int    TG4Medium::fgkDefaultIfield = 1;

//_____________________________________________________________________________
TG4Medium::TG4Medium(G4int id)
  : fID(id),
    fName(fgkUndefinedName),
    fMaterial(0),
    fLimits(0),
    fIfield(fgkDefaultIfield)
{
/// Standard constructor from given id
}

//_____________________________________________________________________________
TG4Medium::~TG4Medium()
{
/// Destructor
}

//_____________________________________________________________________________
void TG4Medium::Print() const
{
/// Print medium characteristics

  G4cout << "ID = " << fID
         << "  name = "<< fName
         << "  limits = " << fLimits;

  TG4Limits* limits =  dynamic_cast<TG4Limits*> (fLimits);
  if (limits) limits->Print();
}

//_____________________________________________________________________________
void TG4Medium::SetName(const G4String& name)
{
/// Set the given name;
/// give a warning if medium name has been already defined

  if ( fName != fgkUndefinedName &&  fName != name ) {
    TG4Globals::Warning(
      "TG4Medium", "SetName",
      "Reseting name " + TString(fName) + " to " + TString(name));
  }
  fName = name;
}

//_____________________________________________________________________________
void TG4Medium::SetMaterial(G4Material* material)
{
/// Set the given material;
/// give a warning if medium material has been already defined

  if ( fMaterial && fMaterial != material ) {
    G4cout << fMaterial->GetName() << " " << material->GetName() << G4endl;
    TG4Globals::Warning(
      "TG4Medium", "SetMaterial",
      "Reseting material " + TString(fMaterial->GetName()) +
      " to " + TString(material->GetName()));
  }
  fMaterial = material;
}

//_____________________________________________________________________________
void TG4Medium::SetLimits(G4UserLimits* limits)
{
/// Set the given limits;

  // if ( fLimits && fLimits != limits ) {
  //  TG4Globals::Warning(
  //    "TG4Medium", "SetLimits", "Reseting medium limits");
  //}
  fLimits = limits;
}
