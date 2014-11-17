//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4NameMap.cxx
/// \brief Implementation of the TG4NameMap class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4NameMap.h"
#include "TG4Globals.h"

#include "iomanip"
#include "globals.hh"

G4String TG4NameMap::fgUndefined = "Undefined";

//_____________________________________________________________________________
TG4NameMap::TG4NameMap() 
  : fMap(),
    fInverseMap(),
    fSecond(fgUndefined) 
{
/// Default constructor
}

//_____________________________________________________________________________
TG4NameMap::~TG4NameMap() 
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
G4bool TG4NameMap::Add(const G4String& first, const G4String& second)
{  
/// Add names pair to the map.
/// fSecond is not used in this add method.

  if (GetSecond(first) == fgUndefined) {
    // insert into map 
    // only in case it is not yet here
    fMap[first] = second;
    fInverseMap[second] = first;
    return true;
  }
  return false;
}

//_____________________________________________________________________________
G4bool TG4NameMap::AddInverse(const G4String& first, const G4String& second)
{  
/// Add names pair only to the inverse map.

  if (GetFirst(second) == fgUndefined) {
    // insert into map 
    // only in case it is not yet here
    fInverseMap[second] = first;
    return true;
  }
  return false;
}

//_____________________________________________________________________________
G4bool TG4NameMap::AddName(const G4String& name)
{  
/// Add name to the map.

  if (GetSecond(name) == fgUndefined) {
    // insert into map 
    // only in case it is not yet here
    fMap[name] = fSecond;
    fInverseMap[fSecond] = name;
    return true;
  }
  return false;
}

//_____________________________________________________________________________
const G4String& TG4NameMap::GetFirst(const G4String& second) const
{
/// Get first name associated with given second name.

  MapConstIterator i = fInverseMap.find(second);
  if (i == fInverseMap.end()) 
    return fgUndefined;
  else                 
    return (*i).second;
}

//_____________________________________________________________________________
const G4String& TG4NameMap::GetSecond(const G4String& first) const
{
/// Get second name associated with given first name.

  MapConstIterator i = fMap.find(first);
  if (i == fMap.end()) 
    return fgUndefined;
  else                 
    return (*i).second;
}

//_____________________________________________________________________________
void TG4NameMap::PrintAll() const
{
/// Dump the whole map.

  if (fMap.size()) {
    G4cout << "Dump of map - " << fMap.size() << " entries:" << G4endl;
    G4int counter = 0;
    for (MapConstIterator i=fMap.begin(); i != fMap.end(); i++) {
      const G4String& first  = (*i).first;
      const G4String& second = (*i).second;
      G4cout << "Map element " << std::setw(3) << counter++ << "   " 
             << first << "   " << second << G4endl;
    }
  }

  if (fInverseMap.size()) {
    G4cout << "Dump of inverse map - " << fInverseMap.size() << " entries:" << G4endl;
    G4int counter = 0;
    for (MapConstIterator i=fInverseMap.begin(); i != fInverseMap.end(); i++) {
      const G4String& first  = (*i).first;
      const G4String& second = (*i).second;
      G4cout << "Map element " << std::setw(3) << counter++ << "   " 
             << first << "   " << second << G4endl;
    }
  }
}

//_____________________________________________________________________________
void TG4NameMap::Clear() 
{
/// Clear the map.

  fMap.clear();
  fInverseMap.clear();
  fSecond = fgUndefined;
}  
