//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ProcessControlMap.cxx
/// \brief Implementation of the TG4ProcessControlMap class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4ProcessControlMap.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#include "globals.hh"
#include "iomanip"
#include <G4VProcess.hh>

TG4ProcessControlMap* TG4ProcessControlMap::fgInstance = 0;

//_____________________________________________________________________________
TG4ProcessControlMap::TG4ProcessControlMap() : fMap(), fIsBiasing(false)
{
  /// Default constructor

  if (fgInstance) {
    TG4Globals::Exception("TG4ProcessControlMap", "TG4ProcessControlMap",
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4ProcessControlMap::~TG4ProcessControlMap()
{
  /// Destructor

  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
G4bool TG4ProcessControlMap::IsDefined(const G4String& processName)
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
G4bool TG4ProcessControlMap::Add(G4VProcess* process, TG4G3Control control)
{
  /// Add the pair to the map.

  if (!process) return false;

  return Add(process->GetProcessName(), control);
}

//_____________________________________________________________________________
G4bool TG4ProcessControlMap::Add(G4String processName, TG4G3Control control)
{
  /// Add the pair to the map.

  if (!IsDefined(processName)) {
    // insert into map
    // only in case it is not yet here
    fMap[processName] = control;
    if (fIsBiasing) {
      // when biasing then process name is wrapped
      fMap[G4String("biasWrapper(") + processName + G4String(")")] = control;
    }
    return true;
  }
  return false;
}

//_____________________________________________________________________________
void TG4ProcessControlMap::PrintAll() const
{
  /// Dump the whole map.

  if (fMap.size()) {
    G4cout << "Dump of TG4ProcessControlMap - " << fMap.size()
           << " entries:" << G4endl;
    G4int counter = 0;
    for (MapConstIterator i = fMap.begin(); i != fMap.end(); i++) {
      G4String processName = (*i).first;
      TG4G3Control control = (*i).second;
      G4cout << "Map element " << std::setw(3) << counter++ << "   "
             << processName << "   "
             << TG4G3ControlVector::GetControlName(control) << G4endl;
    }
  }
}

//_____________________________________________________________________________
void TG4ProcessControlMap::Clear()
{
  /// Clear the map.

  fMap.clear();
}

//_____________________________________________________________________________
TG4G3Control TG4ProcessControlMap::GetControl(const G4VProcess* process)
{
  /// Return the G3 process control for the process with a given name.

  if (!process) return kNoG3Controls;

  return GetControl(process->GetProcessName());
}

//_____________________________________________________________________________
TG4G3Control TG4ProcessControlMap::GetControl(const G4String& processName)
{
  /// Return the G3 process control for the process with a given name.

  MapIterator i = fMap.find(processName);
  if (i == fMap.end())
    return kNoG3Controls;
  else
    return (*i).second;
}

//_____________________________________________________________________________
const G4String& TG4ProcessControlMap::GetControlName(const G4VProcess* process)
{
  /// Return the G3 process control name for the process with a given name.

  if (!process) return TG4G3ControlVector::GetControlName(kNoG3Controls);

  return GetControlName(process->GetProcessName());
}

//_____________________________________________________________________________
const G4String& TG4ProcessControlMap::GetControlName(
  const G4String& processName)
{
  /// Return the G3 process control name for the process with a given name.

  return TG4G3ControlVector::GetControlName(GetControl(processName));
}
