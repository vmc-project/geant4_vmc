//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessMCMap.cxx
/// \brief Implementation of the TG4ProcessMCMap class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ProcessMCMap.h"
#include "TG4G3PhysicsManager.h"
#include "TG4Globals.h"

#include "globals.hh"
#include <G4VProcess.hh>
#include <iomanip>

TG4ProcessMCMap* TG4ProcessMCMap::fgInstance = 0;

//_____________________________________________________________________________
TG4ProcessMCMap::TG4ProcessMCMap() : fMap(), fIsBiasing(false)
{
  /// Default constructor

  if (fgInstance) {
    TG4Globals::Exception("TG4ProcessMCMap", "TG4ProcessMCMap",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4ProcessMCMap::~TG4ProcessMCMap()
{
  /// Destructor

  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
G4bool TG4ProcessMCMap::IsDefined(const G4String& processName)
{
  /// Return true if the first is already in the map.

  if (fMap.find(processName) == fMap.end())
    return false;
  else
    return true;
}

//
// public methods
//

//_____________________________________________________________________________
G4bool TG4ProcessMCMap::Add(G4VProcess* process, TMCProcess mcProcess)
{
  /// Add the pair to the map.

  if (!process) return false;

  return Add(process->GetProcessName(), mcProcess);
}

//_____________________________________________________________________________
G4bool TG4ProcessMCMap::Add(G4String processName, TMCProcess mcProcess)
{
  /// Add the pair to the map.

  if (!IsDefined(processName)) {
    // insert into map
    // only in case it is not yet here
    fMap[processName] = mcProcess;
    if (fIsBiasing) {
      // when biasing then process name is wrapped
      fMap[G4String("biasWrapper(") + processName + G4String(")")] = mcProcess;
    }
    return true;
  }
  return false;
}

//_____________________________________________________________________________
void TG4ProcessMCMap::PrintAll() const
{
  /// Dump the whole map.

  if (fMap.size()) {
    G4cout << "Dump of TG4ProcessMCMap - " << fMap.size()
           << " entries:" << G4endl;
    G4int counter = 0;
    for (MapConstIterator i = fMap.begin(); i != fMap.end(); i++) {
      G4String processName = (*i).first;
      TMCProcess mcProcess = (*i).second;
      G4cout << "Map element " << std::setw(3) << counter++ << "   "
             << processName << "   " << TMCProcessName[mcProcess] << G4endl;
    }
  }
}

//_____________________________________________________________________________
void TG4ProcessMCMap::Clear()
{
  /// Clear the map.

  fMap.clear();
}

//_____________________________________________________________________________
TMCProcess TG4ProcessMCMap::GetMCProcess(const G4VProcess* process) const
{
  /// Return TMCProcess code for the process with a given name.

  if (!process) return kPNoProcess;

  return GetMCProcess(process->GetProcessName());
}

//_____________________________________________________________________________
TMCProcess TG4ProcessMCMap::GetMCProcess(const G4String& processName) const
{
  /// Return TMCProcess code for the process with a given name.

  MapConstIterator i = fMap.find(processName);
  if (i == fMap.end())
    return kPNoProcess;
  else
    return (*i).second;
}

//_____________________________________________________________________________
G4String TG4ProcessMCMap::GetMCProcessName(const G4VProcess* process) const
{
  /// Return TMCProcess code for the process with a given name.

  if (!process) return TMCProcessName[kPNoProcess];

  return GetMCProcessName(process->GetProcessName());
}

//_____________________________________________________________________________
G4String TG4ProcessMCMap::GetMCProcessName(const G4String& processName) const
{
  /// Return TMCProcess code for the process with a given name.

  return TMCProcessName[GetMCProcess(processName)];
}
