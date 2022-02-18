#ifndef TG4_PROCESS_MAP_H
#define TG4_PROCESS_MAP_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2022 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMap.h
/// \brief Definition of the TG4ProcessMap class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4G3Control.h"

#include <globals.hh>

#include <TMCProcess.h>

#include <map>

class G4VProcess;

/// \ingroup physics
/// \brief Maps G4 process sub types to TMCProcess and TG4G3Control codes
///
/// Singleton map container for associated pairs
/// of G4 process sub types and TMCProcess and TG4G3Control code.
///
/// \author I. Hrivnacova; IJClab Orsay

class TG4ProcessMap
{
 public:
  TG4ProcessMap();
  TG4ProcessMap(const TG4ProcessMap& right) = delete;
  TG4ProcessMap& operator=(const TG4ProcessMap& right) = delete;
  virtual ~TG4ProcessMap();

  // static access method
  static TG4ProcessMap* Instance();

  // methods
  G4bool Add(G4int subType, TMCProcess mcProcess, TG4G3Control g3Control);
  void PrintAll() const;
  void Clear();

  // get methods
  std::pair<TMCProcess, TG4G3Control> GetCodes(const G4VProcess* process) const;
  TMCProcess GetMCProcess(const G4VProcess* process) const;
  TG4G3Control GetControl(const G4VProcess* process) const;
  G4String GetMCProcessName(const G4VProcess* process) const;
  G4String GetControlName(const G4VProcess* process) const;

 private:
  // methods
  G4bool IsDefined(G4int subType);

  // static data members
  static TG4ProcessMap* fgInstance; ///< this instance

  // data members
  std::map<G4int, std::pair<TMCProcess, TG4G3Control>> fMap; ///< map container
};

// inline methods

inline TG4ProcessMap* TG4ProcessMap::Instance()
{
  /// Return this instance
  return fgInstance;
}

#endif // TG4_PROCESS_MAP_H
