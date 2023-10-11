#ifndef TG4_INT_MAP_H
#define TG4_INT_MAP_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4IntMap.h
/// \brief Definition of the TG4IntMap class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>
#include <map>

/// \ingroup global
/// \brief The map container for integer numbers associated with names.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4IntMap
{
 public:
  /// The map of integers to strings
  typedef std::map<G4String, G4int, std::less<G4String> > Map;

  /// The iterator for the map of integers to strings
  typedef Map::iterator MapIterator;

  /// The constant iterator for the map of integers to strings
  typedef Map::const_iterator MapConstIterator;

 public:
  TG4IntMap();
  ~TG4IntMap();

  // methods
  G4bool Add(const G4String& first, G4int second);
  G4int GetSecond(const G4String& name, G4bool warn = true);
  G4int GetSize() const;
  void PrintAll() const;
  void Clear();

 protected:
 private:
  /// Not implemented
  TG4IntMap(const TG4IntMap& right);
  /// Not implemented
  TG4IntMap& operator=(const TG4IntMap& right);

  // methods
  G4bool IsDefined(const G4String& first);

  // data members
  Map fMap; ///< map container
};

#endif // TG4_NAME_MAP_H
