// $Id: TG4ProcessControlMap.h,v 1.5 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup global
/// \class TG4ProcessControlMap
/// \brief Maps G4 process names to TG4G3Control
///
/// Singleton map container for associated pairs G4 process name 
/// and TG4G3Control.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_PROCESS_CONTROL_MAP_H
#define TG4_PROCESS_CONTROL_MAP_H

#include <map>
#include <globals.hh>

#include "TG4G3Control.h"

class G4VProcess;

class TG4ProcessControlMap
{
  typedef std::map<G4String, TG4G3Control, std::less<G4String> >  Map;
  typedef Map::iterator       MapIterator;
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
    TG4ProcessControlMap(const TG4ProcessControlMap& right);
    TG4ProcessControlMap& operator=(const TG4ProcessControlMap& right);
  
    // methods
    G4bool IsDefined(const G4String& processName);

    // static data members
    static TG4ProcessControlMap*  fgInstance; //this instance

    // data members
    Map  fMap; //map container
};

// inline methods

inline TG4ProcessControlMap* TG4ProcessControlMap::Instance() { 
  /// Return this instance
  return fgInstance; 
}

#endif //TG4_PROCESS_CONTROL_MAP_H
