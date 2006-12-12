// $Id: TG4NameMap.cxx,v 1.5 2006/09/13 06:26:11 brun Exp $
// Category: global
//
// Class TG4NameMap
// ----------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

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
//
}

//_____________________________________________________________________________
TG4NameMap::~TG4NameMap() {
//
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
    G4cout << "Dump of TG4NameMap - " << fMap.size() << " entries:" << G4endl;
    G4int counter = 0;
    for (MapConstIterator i=fMap.begin(); i != fMap.end(); i++) {
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
