//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2022 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMap.cxx
/// \brief Implementation of the TG4ProcessMap class
///
/// \author I. Hrivnacova; IJCLab, Orsay

#include "TG4ProcessMap.h"
#include "TG4G3PhysicsManager.h"
#include "TG4Globals.h"

#include <G4VProcess.hh>

#include <iomanip>

TG4ProcessMap* TG4ProcessMap::fgInstance = 0;

//_____________________________________________________________________________
TG4ProcessMap::TG4ProcessMap() : fMap()
{
  /// Default constructor

  if (fgInstance) {
    TG4Globals::Exception("TG4ProcessMap", "TG4ProcessMap",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4ProcessMap::~TG4ProcessMap()
{
  /// Destructor

  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
G4bool TG4ProcessMap::IsDefined(G4int subType)
{
  /// Return true if the first is already in the map.

  if (fMap.find(subType) == fMap.end())
    return false;
  else
    return true;
}

//
// public methods
//

//_____________________________________________________________________________
G4bool TG4ProcessMap::Add(
  G4int subType, TMCProcess mcProcess, TG4G3Control g3Control)
{
  /// Add the pair to the map.

  if (!IsDefined(subType)) {
    // insert into map
    // only in case it is not yet here
    fMap[subType] = std::pair(mcProcess, g3Control);
    return true;
  }
  return false;
}

//_____________________________________________________________________________
void TG4ProcessMap::PrintAll() const
{
  /// Dump the whole map.

  if (fMap.empty()) return;

  G4cout << "Dump of TG4ProcessMap - " << fMap.size()
         << " entries:" << G4endl;
  G4int counter = 0;
  for (auto [subType, codes] : fMap) {
    // TO DO: get process sub-type name
    G4cout << "Map element " << std::setw(3) << counter++ << "   "
           << subType << "   " << TMCProcessName[codes.first] << "   "
           << TG4G3ControlVector::GetControlName(codes.second) << G4endl;
  }
}

//_____________________________________________________________________________
void TG4ProcessMap::Clear()
{
  /// Clear the map.

  fMap.clear();
}

//_____________________________________________________________________________
std::pair<TMCProcess, TG4G3Control> 
TG4ProcessMap::GetCodes(const G4VProcess* process) const
{
  /// Return the pair of TMCProcess  and G3 control codes for the process
  /// with a given name.

  if (!process) return { kPNoProcess, kNoG3Controls };

  auto i = fMap.find(process->GetProcessSubType());
  if (i == fMap.end()) {
    G4String text = "Unknown process code for ";
    text += process->GetProcessName();
    TG4Globals::Warning("TG4ProcessMap", "GetCodes", text);
    return { kPNoProcess, kNoG3Controls };
  }
  else {
    return (*i).second;
  }
}


//_____________________________________________________________________________
TMCProcess TG4ProcessMap::GetMCProcess(const G4VProcess* process) const
{
  /// Return TMCProcess code for the process with a given name.

  return GetCodes(process).first;
}

//_____________________________________________________________________________
TG4G3Control TG4ProcessMap::GetControl(const G4VProcess* process) const
{
  /// Return G3 control code for the process with a given name.

  return GetCodes(process).second;
}

//_____________________________________________________________________________
G4String TG4ProcessMap::GetMCProcessName(const G4VProcess* process) const
{
  /// Return TMCProcess code for the process with a given name.

  if (!process) return TMCProcessName[kPNoProcess];

  return TMCProcessName[GetMCProcess(process)];
}

//_____________________________________________________________________________
G4String TG4ProcessMap::GetControlName(const G4VProcess* process) const
{
  /// Return G3 control code for the process with a given name.

  if (!process) return TG4G3ControlVector::GetControlName(kNoG3Controls);

  return TG4G3ControlVector::GetControlName(GetControl(process));
}

