#ifndef TG4_SD_SERVICES_H
#define TG4_SD_SERVICES_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file  TG4SDServices.h
/// \brief Definition of the  TG4SDServices class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>

#include <Rtypes.h>

#include <map>

class TG4SensitiveDetector;

class G4LogicalVolume;
class G4VSensitiveDetector;

/// \ingroup digits_hits
/// \brief Sensitive detectors services
///
/// The class provides service methods for accessing to Geant4 geometry,
/// namely using TVirtualMC volumes identifiers
/// (implemented via TG4SensitiveDetector instances).
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SDServices
{
  public:
    TG4SDServices();
    virtual ~TG4SDServices();

    // static methods
    static TG4SDServices* Instance();

    // methods
    //void MapVolumes();
    void PrintStatistics(G4bool open, G4bool close) const;
    void PrintSensitiveVolumes() const;

    // set methods
    void SetIsStopRun(G4bool stopRun);

    // get methods
    G4int  GetMediumId(G4LogicalVolume* volume) const;
    G4bool GetIsStopRun() const;
    TG4SensitiveDetector* GetSensitiveDetector(G4VSensitiveDetector* sd) const;  

  private:
    /// Not implemented
    TG4SDServices(const TG4SDServices& right);
    /// Not implemented
    TG4SDServices& operator=(const TG4SDServices& right);

    // static data members
    static TG4SDServices* fgInstance; ///< this instance

    G4bool  fIsStopRun; ///< info about run stopping by user  
};

// inline methods

inline TG4SDServices* TG4SDServices::Instance() { 
  /// Returns this instance.
  return fgInstance; 
}

inline void TG4SDServices::SetIsStopRun(G4bool isStopRun) { 
  /// Sets flag for notifying about stopping run by a user.
  fIsStopRun = isStopRun; 
}

inline G4bool TG4SDServices::GetIsStopRun() const { 
  /// Returns flag for notifying about stopping run by a user.
  return fIsStopRun; 
}

#endif //TG4_SD_SERVICES_H

