#ifndef TG4_REGIONS_MESSENGER_H
#define TG4_REGIONS_MESSENGER_H 

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RegionsMessenger.h
/// \brief Definition of the TG4RegionsMessenger class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4RegionsManager;

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

/// \ingroup run
/// \brief Messenger class that defines commands for TG4RegionsManager
///
/// Implements commands:
/// - /mcRegions/dump [lvName]
/// - /mcRegions/setRangePrecision value
/// - /mcRegions/check [true|false]
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RegionsMessenger: public G4UImessenger
{
  public:
    TG4RegionsMessenger(TG4RegionsManager* runManager);
    virtual ~TG4RegionsMessenger();
   
    // methods 
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  private:
    /// Not implemented
    TG4RegionsMessenger();  
    /// Not implemented
    TG4RegionsMessenger(const TG4RegionsMessenger& right);
    /// Not implemented
    TG4RegionsMessenger& operator=(const TG4RegionsMessenger& right);

    // data members
    TG4RegionsManager*     fRegionsManager; ///< associated class   
    G4UIdirectory*         fDirectory;      ///< command directory
    G4UIcmdWithAString*    fDumpRegionCmd;  ///< command: /mcRegions/dump [lvName]
    G4UIcmdWithAnInteger*  fSetRangePrecisionCmd; ///< command: /mcRegions/setRangePrecision value
    G4UIcmdWithABool*      fSetCheckCmd;    ///< command: /mcRegions/check [true|false]
};

#endif //TG4_RUN_MESSENGER_H
