// $Id: TG4SensitiveDetector.h,v 1.4 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup digits_hits
/// \class TG4SensitiveDetector
/// \brief Sensitive detector common to all logical volumes
///
/// This class adds integer identifier data member to G4VSensitiveDetector.
/// It also takes care of setting step status (kBoundary, kNormalStep)
/// and passing G4Step to TG4StepManager and for calling a user application
/// stepping function.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_SENSITIVE_DETECTOR_H
#define TG4_SENSITIVE_DETECTOR_H

#include <G4VSensitiveDetector.hh>
#include <globals.hh>

class TG4StepManager;

class TG4SensitiveDetector : public G4VSensitiveDetector
{
  public:
    TG4SensitiveDetector(G4String sdName);
    TG4SensitiveDetector(G4String sdName, G4int id);
    virtual ~TG4SensitiveDetector();

    // methods
    virtual void UserProcessHits(const G4Track* track,const G4Step* step);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual G4bool ProcessHitsOnBoundary(G4Step* step);
 
    // static get method
    static G4int GetTotalNofSensitiveDetectors();

    // get methods
    G4int GetID() const;
    
  private:          
    TG4SensitiveDetector(); 
    TG4SensitiveDetector(const TG4SensitiveDetector& right);
    TG4SensitiveDetector& operator=(const TG4SensitiveDetector &right);

    // data members
    static G4int  fgSDCounter; //sensitive detector counter

    // data members
    G4int  fID;                    //sensitive detector ID
    TG4StepManager*  fStepManager; //TG4StepManager
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

#endif //TG4V_SENSITIVE_DETECTOR_H


