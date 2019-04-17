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

class TVirtualMCApplication;
class TVirtualMCSensitiveDetector;

/// \ingroup digits_hits
/// \brief Sensitive detector class for calling a user defined stepping
/// function.
///
/// This class adds the integer identifier data member to G4VSensitiveDetector
/// and keeps the medium ID of the associated volume.
/// It also takes care of setting the step status (kBoundary, kNormalStep)
/// and passing G4Step to TG4StepManager and for calling a user defined
/// stepping function either via a user MC application stepping function
/// or a user defined VMC sensitive detector (new).
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SensitiveDetector : public G4VSensitiveDetector
{
 public:
  TG4SensitiveDetector(G4String sdName, G4int mediumID);
  TG4SensitiveDetector(
    TVirtualMCSensitiveDetector* userSD, G4int mediumID, G4bool exclusiveSD);
  virtual ~TG4SensitiveDetector();

  // methods
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual G4bool ProcessHitsOnBoundary(G4Step* step);
  virtual void ProcessHitsOnTrackStart();
  // Was user process hits

  // static get method
  static G4int GetTotalNofSensitiveDetectors();

  // get methods
  G4int GetID() const;
  G4int GetMediumID() const;
  TVirtualMCSensitiveDetector* GetUserSD() const;

 protected:
  void UserProcessHits();

  // data members
  /// Cached pointer to thread-local step manager
  TG4StepManager* fStepManager;
  /// Cached pointer to thread-local VMC application
  TVirtualMCApplication* fMCApplication;
  /// User sensitive detector
  TVirtualMCSensitiveDetector* fUserSD;

 private:
  /// Not implemented
  TG4SensitiveDetector();
  /// Not implemented
  TG4SensitiveDetector(const TG4SensitiveDetector& right);
  /// Not implemented
  TG4SensitiveDetector& operator=(const TG4SensitiveDetector& right);

  // data members
  static G4ThreadLocal G4int fgSDCounter; ///< sensitive detector counter

  // data members
  G4int fID;       ///< sensitive detector ID
  G4int fMediumID; ///< medium ID
  /// map logical volume -> volume id
  std::map<G4LogicalVolume*, G4int> fLVToVolIdMap;
};

// inline methods

inline G4int TG4SensitiveDetector::GetTotalNofSensitiveDetectors()
{
  /// Returns the total number of sensitive detectors.
  return fgSDCounter;
}

inline G4int TG4SensitiveDetector::GetID() const
{
  /// Returns sensitive detector ID.
  return fID;
}

inline G4int TG4SensitiveDetector::GetMediumID() const
{
  /// Returns medium ID
  return fMediumID;
}

inline TVirtualMCSensitiveDetector* TG4SensitiveDetector::GetUserSD() const
{
  /// Returns user SD
  return fUserSD;
}

#endif // TG4_SENSITIVE_DETECTOR_H
