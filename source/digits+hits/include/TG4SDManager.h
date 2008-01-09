#ifndef TG4_SD_MANAGER_H
#define TG4_SD_MANAGER_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4SDManager.h
/// \brief Definition of the TG4SDManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>

#include <Rtypes.h>

class TG4SDServices;
class TG4SDConstruction;

/// \ingroup digits_hits
/// \brief Geant4 implementation of the TVirtualMC interface methods                    
/// for access to Geant4 geometry related with sensitive detectors.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SDManager
{
  public:
    TG4SDManager();
    virtual ~TG4SDManager();

    // static methods
    static TG4SDManager* Instance();

    // methods
    void Initialize();
    
    // TVirtualMC methods
    Int_t VolId(const Text_t* volName) const;                
    const char* VolName(Int_t id) const;
    Int_t NofVolumes() const; 
    Int_t NofVolDaughters(const char* volName) const;
    const char*  VolDaughterName(const char* volName, Int_t i) const;
    Int_t        VolDaughterCopyNo(const char* volName, Int_t i) const;
    Int_t VolId2Mate(Int_t volumeId) const;

    // get methods
    TG4SDConstruction* GetSDConstruction() const;

  private:
    /// Not implemented
    TG4SDManager(const TG4SDManager& right);
    /// Not implemented
    TG4SDManager& operator=(const TG4SDManager& right);

    // static data members
    static TG4SDManager* fgInstance; ///< this instance
    
    //
    // data members
    
    /// sensitive detectors construction 
    TG4SDConstruction*  fSDConstruction;
    
    /// services related with sensitive detectors    
    TG4SDServices*      fSDServices;
                                         

};

// inline methods

inline TG4SDManager* TG4SDManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline TG4SDConstruction* TG4SDManager::GetSDConstruction() const { 
  /// Return sensitive detctor construction
  return fSDConstruction; 
}

#endif //TG4_SD_MANAGER_H

