#ifndef TG4_RUN_MANAGER_H
#define TG4_RUN_MANAGER_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4RunManager.h
/// \brief Definition of the TG4RunManager.h class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4RunMessenger.h"

#include <globals.hh>

#include <Rtypes.h>

class TG4RunConfiguration;
class TG4SpecialControlsV2;
class TG4RegionsManager;

class G4RunManager;
class G4UIExecutive;

class TApplication;

/// \ingroup run
/// \brief Geant4 implementation of the TVirtualMC interface methods                    
/// for access to Geant4 at run level.
///
/// It provides also methods for switching between Geant4 and
/// Root UIs.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RunManager : public TG4Verbose
{
  public:
    TG4RunManager(TG4RunConfiguration* configuration, 
                  int argc = 0, char** argv = 0);
     virtual ~TG4RunManager();

    // static access method
    static TG4RunManager* Instance();

    // methods
    void Initialize();
    void LateInitialize();
    void ProcessEvent();
    Bool_t ProcessRun(G4int nofEvents);

    // get methods
    Int_t   CurrentEvent() const;
    Bool_t  SecondariesAreOrdered() const;

    //
    // methods for Geant4 only 
    //
    void StartGeantUI();
    void StartRootUI();
    void ProcessGeantMacro(G4String macroName);
    void ProcessRootMacro(G4String macroName);
    void ProcessGeantCommand(G4String command);
    void ProcessRootCommand(G4String command);
    void UseG3Defaults();   
    void UseRootRandom(G4bool useRootRandom);   

  private:
    /// Not implemented
    TG4RunManager();
    /// Not implemented
    TG4RunManager(const TG4RunManager& right);
    /// Not implemented
    TG4RunManager& operator=(const TG4RunManager& right);

    // methods
    void ConfigureRunManager();
    void CloneRootNavigatorForWorker();
    void FilterARGV(const G4String& option);
    void CreateGeantUI();
    void CreateRootUI();
    void CreateUIs();
    void SetRandomSeed();
    
    // static data members

    /// master instance
    static TG4RunManager*  fgMasterInstance; 
    
    /// thread local instance
    static G4ThreadLocal TG4RunManager*  fgInstance; ///< this instance
    
    // data members    
    G4RunManager*         fRunManager;       ///< G4RunManager
    TG4RunMessenger       fMessenger;        ///< messenger
    TG4RunConfiguration*  fRunConfiguration; ///< TG4RunConfiguration
    TG4RegionsManager*    fRegionsManager;   ///< regions manager
    G4UIExecutive*        fGeantUISession; ///< G4 UI 
    TApplication*         fRootUISession;    ///< Root UI 
    G4bool                fRootUIOwner;      ///< ownership of Root UI
    G4int                 fARGC;             ///< argc 
    char**                fARGV;             ///< argv
    G4bool                fUseRootRandom;    ///< the option to use Root random number seed
};

// inline methods

inline TG4RunManager* TG4RunManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}
    
inline void TG4RunManager::UseRootRandom(G4bool useRootRandom) {
  /// Set the option whether to use Root random number seed 
  fUseRootRandom = useRootRandom;
}   

#endif //TG4_RUN_MANAGER_H

