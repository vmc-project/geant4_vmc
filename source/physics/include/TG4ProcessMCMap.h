#ifndef TG4_PROCESS_MC_MAP_H
#define TG4_PROCESS_MC_MAP_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMCMap.h
/// \brief Definition of the TG4ProcessMCMap class
///
/// \author I. Hrivnacova; IPN Orsay

#include <globals.hh>
#include <map>

#include <Rtypes.h>
#include <TMCProcess.h>

class G4VProcess;

/// \ingroup physics
/// \brief Maps G4 process names to TMCProcess code
///
/// Singleton map container for associated pairs
/// of G4 process name and TMCProcess code.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ProcessMCMap
{
 public:
  /// The map of TMCProcess to strings
  typedef std::map<G4String, TMCProcess, std::less<G4String> > Map;

  /// The iterator for the map of TMCProcess to strings
  typedef Map::iterator MapIterator;

  /// The constant iterator for the map of TMCProcess to strings
  typedef Map::const_iterator MapConstIterator;

 public:
  TG4ProcessMCMap();
  virtual ~TG4ProcessMCMap();

  // static access method
  static TG4ProcessMCMap* Instance();

  // methods
  G4bool Add(G4VProcess* process, TMCProcess second);
  G4bool Add(G4String processName, TMCProcess second);
  void PrintAll() const;
  void Clear();
  void SetIsBiasing(G4bool value);

  // get methods
  TMCProcess GetMCProcess(const G4VProcess* process) const;
  TMCProcess GetMCProcess(const G4String& processName) const;
  G4String GetMCProcessName(const G4VProcess* process) const;
  G4String GetMCProcessName(const G4String& processName) const;

 private:
  /// Not implemented
  TG4ProcessMCMap(const TG4ProcessMCMap& right);
  /// Not implemented
  TG4ProcessMCMap& operator=(const TG4ProcessMCMap& right);

  // methods
  G4bool IsDefined(const G4String& processName);

  // static data members
  // MT COMMON
  static TG4ProcessMCMap* fgInstance; ///< this instance

  // data members
  Map fMap; ///< map container
  G4bool fIsBiasing; ///< info whether biasing is activated
};

// inline methods

inline TG4ProcessMCMap* TG4ProcessMCMap::Instance()
{
  /// Return this instance
  return fgInstance;
}

inline void TG4ProcessMCMap::SetIsBiasing(G4bool value)
{
  /// Set info whether biasing is activated
  fIsBiasing = value;
}

#endif // TG4_PROCESS_MC_MAP_H
