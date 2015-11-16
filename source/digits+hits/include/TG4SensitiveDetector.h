#ifndef TG4_SENSITIVE_DETECTOR_H
#define TG4_SENSITIVE_DETECTOR_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SensitiveDetector.h
/// \brief Definition of the TG4SensitiveDetector class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4VSensitiveDetector.hh>
#include <globals.hh>

class TG4StepManager;

/// \ingroup digits_hits
/// \brief Sensitive detector common to all logical volumes
///
/// This class adds integer identifier data member to G4VSensitiveDetector
/// and keeps the medium ID of the associated volume.  
/// It also takes care of setting step status (kBoundary, kNormalStep)
/// and passing G4Step to TG4StepManager and for calling a user application
/// stepping function.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SensitiveDetector : public G4VSensitiveDetector
{
  public:
    TG4SensitiveDetector(G4String sdName, G4int mediumID);
    virtual ~TG4SensitiveDetector();

    // methods
    virtual void UserProcessHits(const G4Track* track,const G4Step* step);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual G4bool ProcessHitsOnBoundary(G4Step* step);
 
    // static get method
    static G4int GetTotalNofSensitiveDetectors();
    
    // get methods
    G4int GetID() const;
    G4int GetMediumID() const;
    
  protected:
    // methods
    TG4StepManager*  GetStepManager() const;

  private:
    /// Not implemented
    TG4SensitiveDetector(); 
    /// Not implemented
    TG4SensitiveDetector(const TG4SensitiveDetector& right);
    /// Not implemented
    TG4SensitiveDetector& operator=(const TG4SensitiveDetector &right);

    // data members
    static G4ThreadLocal G4int fgSDCounter; ///< sensitive detector counter

    // data members
    G4int            fID;          ///< sensitive detector ID
    G4int            fMediumID;    ///< medium ID
    TG4StepManager*  fStepManager; ///< the TG4StepManager instance
};

// inline methods

inline G4int TG4SensitiveDetector::GetTotalNofSensitiveDetectors() { 
  /// Returns the total number of sensitive detectors.
  return fgSDCounter; 
}

inline G4int TG4SensitiveDetector::GetID() const { 
  /// Returns sensitive detector ID.
  return fID; 
}

inline G4int TG4SensitiveDetector::GetMediumID() const {
  /// Returns medium ID
  return fMediumID;
}  

inline TG4StepManager*  TG4SensitiveDetector::GetStepManager() const {
  /// Returns the TG4StepManager instance
  return fStepManager;
}

#endif //TG4V_SENSITIVE_DETECTOR_H


