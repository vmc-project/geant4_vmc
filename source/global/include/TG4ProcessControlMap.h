#ifndef TG4_PROCESS_CONTROL_MAP_H
#define TG4_PROCESS_CONTROL_MAP_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ProcessControlMap.h
/// \brief Definition of the TG4ProcessControlMap class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <map>
#include <globals.hh>

#include "TG4G3Control.h"

class G4VProcess;

/// \ingroup global
/// \brief Maps G4 process names to TG4G3Control
///
/// Singleton map container for associated pairs G4 process name 
/// and TG4G3Control.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4ProcessControlMap
{
  public:
    /// The map of TG4G3Controls to strings 
    typedef std::map<G4String, TG4G3Control, std::less<G4String> >  Map;

    /// The iterator for the map of TG4G3Controls to strings 
    typedef Map::iterator  MapIterator;

    /// The constant iterator for the map of TG4G3Controls to strings 
    typedef Map::const_iterator MapConstIterator;

  public:
    TG4ProcessControlMap();
    virtual ~TG4ProcessControlMap();

    // static access method
    static TG4ProcessControlMap* Instance();

    // methods
    G4bool Add(G4VProcess* process,  TG4G3Control second);  
    G4bool Add(G4String processName, TG4G3Control second);  
    void PrintAll() const;
    void Clear();

    // get methods
    TG4G3Control    GetControl(const G4VProcess* process);
    TG4G3Control    GetControl(const G4String& processName);
    const G4String& GetControlName(const G4VProcess* process);
    const G4String& GetControlName(const G4String& processName);

  private:
    /// Not implemented
    TG4ProcessControlMap(const TG4ProcessControlMap& right);
    /// Not implemented
    TG4ProcessControlMap& operator=(const TG4ProcessControlMap& right);
  
    // methods
    G4bool IsDefined(const G4String& processName);

    // static data members
    static TG4ProcessControlMap*  fgInstance; ///< this instance

    // data members
    Map  fMap; ///< map container
};

// inline methods

inline TG4ProcessControlMap* TG4ProcessControlMap::Instance() { 
  /// Return this instance
  return fgInstance; 
}

#endif //TG4_PROCESS_CONTROL_MAP_H
