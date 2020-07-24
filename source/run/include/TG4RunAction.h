#ifndef TG4_RUN_ACTION_H
#define TG4_RUN_ACTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RunAction.h
/// \brief Definition of the TG4RunAction class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4CrossSectionManager.h"
#include "TG4RunActionMessenger.h"
#include "TG4Verbose.h"

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4Timer;
// in order to avoid the odd dependency for the
// times system function this declaration must be the first
class G4Run;

/// \ingroup run
/// \brief Actions at the beginning and the end of run.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RunAction : public G4UserRunAction, public TG4Verbose
{
 public:
  TG4RunAction();
  virtual ~TG4RunAction();

  // methods
  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);

  // set methods
  void SetSaveRandomStatus(G4bool saveRandomStatus);
  void SetReadRandomStatus(G4bool readRandomStatus);
  void SetRandomStatusFile(G4String RandomStatusFile);
  void SetThresholdWarningEnergy(G4double value);
  void SetThresholdImportantEnergy(G4double value);
  void SetNumberOfThresholdTrials(G4int value);

 private:
  /// Not implemented
  TG4RunAction(const TG4RunAction& right);
  /// Not implemented
  TG4RunAction& operator=(const TG4RunAction& right);

  // methods
  void ChangeLooperParameters(const G4ParticleDefinition* particleDefinition);
  void PrintLooperParameters() const;

  // static data members
  /// default name of the random engine status file to be read in
  static const G4String fgkDefaultRandomStatusFile;

  // data members
  TG4RunActionMessenger fMessenger;            ///< messenger
  TG4CrossSectionManager fCrossSectionManager; ///< cross section manager
  G4Timer* fTimer;                             ///< G4Timer
  G4int fRunID;                                ///< run ID
  G4bool fSaveRandomStatus;   ///< control for saving random engine status
  G4bool fReadRandomStatus;   ///< control for reading random engine status
  G4String fRandomStatusFile; ///< random engine status file name

  /// Energy threshold for warnings about killing looping tracks
  G4double fThresholdWarningEnergy;

  /// Important energy threshold:
  /// it enables tracks above its value to survive a chosen number of ‘tracking’
  /// steps
  G4double fThresholdImportantEnergy;

  /// Number of trials to propagate a looping track
  G4int fNumberOfThresholdTrials;
};

inline void TG4RunAction::SetSaveRandomStatus(G4bool saveRandomStatus)
{
  /// Set option for saving random engine status
  fSaveRandomStatus = saveRandomStatus;
}

inline void TG4RunAction::SetReadRandomStatus(G4bool readRandomStatus)
{
  /// Set option for reading random engine status
  fReadRandomStatus = readRandomStatus;
}

inline void TG4RunAction::SetRandomStatusFile(G4String RandomStatusFile)
{
  /// Set random engine status file name
  fRandomStatusFile = RandomStatusFile;
}

inline void TG4RunAction::SetThresholdWarningEnergy(G4double value)
{
  fThresholdWarningEnergy = value;
}

inline void TG4RunAction::SetThresholdImportantEnergy(G4double value)
{
  fThresholdImportantEnergy = value;
}

inline void TG4RunAction::SetNumberOfThresholdTrials(G4int value)
{
  fNumberOfThresholdTrials = value;
}

#endif // TG4_RUN_ACTION_H
