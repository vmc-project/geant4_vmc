#ifndef TG4_STEP_MANAGER_H
#define TG4_STEP_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4StepManager.h
/// \brief Definition of the TG4StepManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <Rtypes.h>

#include "TG4StepStatus.h"

#include <G4GFlashSpot.hh>
#include <G4Step.hh>
#include <G4SteppingManager.hh>
#include <G4ThreeVector.hh>
#include <G4TransportationManager.hh>
#include <globals.hh>

#include <TArrayI.h>
#include <TMCProcess.h>
#include <TString.h>

class TG4Limits;
class TG4TrackManager;
class TG4SteppingAction;

class G4Track;
class G4SteppingManager;
class G4VPhysicalVolume;
class G4VTouchable;

class TLorentzVector;
class TVector3;
struct TMCParticleStatus;

/// \ingroup digits_hits
/// \brief Geant4 implementation of the TVirtualMC interface methods
/// for access to Geant4 at step level.
///
/// The public methods that do not implement TVirtualMC methods
/// are commented as G4 specific
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4StepManager
{
 public:
  TG4StepManager(const TString& userGeometry);
  virtual ~TG4StepManager();

  // static access method
  static TG4StepManager* Instance();

  // methods
  void LateInitialize();
  void StopTrack();
  void InterruptTrack();
  void StopEvent();
  void StopRun();

  // set methods
  void SetStep(G4Step* step, TG4StepStatus status);             // G4 specific
  void SetStep(G4Track* track, TG4StepStatus status);           // G4 specific
  void SetStep(G4GFlashSpot* gflashSpot, TG4StepStatus status); // G4 specific
  void SetSteppingManager(G4SteppingManager* manager);          // G4 specific
  void SetMaxStep(Double_t step);
  void SetMaxStepBack(); // G4 specific
  void SetMaxNStep(Int_t maxNofSteps);
  void SetCollectTracks(Bool_t collectTracks);
  void ForceDecayTime(Float_t pdg);
  void SetInitialVMCTrackStatus(TMCParticleStatus* status);

  // get methods
  G4Track* GetTrack() const;                 // G4 specific
  G4Step* GetStep() const;                   // G4 specific
  TG4StepStatus GetStepStatus() const;       // G4 specific
  TG4Limits* GetLimitsModifiedOnFly() const; // G4 specific
  Bool_t IsCollectTracks() const;

  // tracking volume(s)
  G4VPhysicalVolume* GetCurrentPhysicalVolume() const; // G4 specific
  TG4Limits* GetCurrentLimits() const;                 // G4 specific
  Int_t CurrentVolID(Int_t& copyNo) const;
  Int_t CurrentVolOffID(Int_t off, Int_t& copyNo) const;
  const char* CurrentVolName() const;
  const char* CurrentVolOffName(Int_t off) const;
  const char* CurrentVolPath();
  Bool_t CurrentBoundaryNormal(Double_t& x, Double_t& y, Double_t& z) const;
  Int_t CurrentMaterial(
    Float_t& a, Float_t& z, Float_t& dens, Float_t& radl, Float_t& absl) const;
  Int_t CurrentMedium() const;
  void Gmtod(Double_t* xm, Double_t* xd, Int_t iflag);
  void Gmtod(Float_t* xm, Float_t* xd, Int_t iflag);
  void Gdtom(Double_t* xd, Double_t* xm, Int_t iflag);
  void Gdtom(Float_t* xd, Float_t* xm, Int_t iflag);
  Double_t MaxStep() const;
  Int_t GetMaxNStep() const;

  // tracking particle
  // dynamic properties
  void TrackPosition(TLorentzVector& position) const;
  void TrackPosition(Double_t& x, Double_t& y, Double_t& z) const;
  void TrackPosition(Float_t& x, Float_t& y, Float_t& z) const;
  void TrackMomentum(TLorentzVector& momentum) const;
  void TrackMomentum(
    Double_t& px, Double_t& py, Double_t& pz, Double_t& etot) const;
  void TrackMomentum(
    Float_t& px, Float_t& py, Float_t& pz, Float_t& etot) const;
  Double_t TrackStep() const;
  Double_t TrackLength() const;
  Double_t TrackTime() const;
  Double_t Edep() const;
  Double_t NIELEdep() const;
  Int_t StepNumber() const;
  Double_t TrackWeight() const;
  void TrackPolarization(Double_t& polX, Double_t& polY, Double_t& polZ) const;
  void TrackPolarization(TVector3& pol) const;
  // static properties
  Int_t TrackPid() const;
  Double_t TrackCharge() const;
  Double_t TrackMass() const;
  Double_t Etot() const;

  // track status
  Bool_t IsTrackInside() const;
  Bool_t IsTrackEntering() const;
  Bool_t IsTrackExiting() const;
  Bool_t IsTrackOut() const;
  Bool_t IsTrackDisappeared() const;
  Bool_t IsTrackStop() const;
  Bool_t IsTrackAlive() const;
  Bool_t IsNewTrack() const;

  // secondaries
  Int_t NSecondaries() const;
  void GetSecondary(Int_t index, Int_t& particleId, TLorentzVector& position,
    TLorentzVector& momentum);
  TMCProcess ProdProcess(Int_t isec) const;
  Int_t StepProcesses(TArrayI& proc) const;

 private:
  /// Not implemented
  TG4StepManager(const TG4StepManager& right);
  /// Not implemented
  TG4StepManager& operator=(const TG4StepManager& right);

  // methods
  void CheckTrack() const;
  void CheckStep(const G4String& method) const;
  void CheckGflashSpot(const G4String& method) const;
  void CheckSteppingManager() const;
  void SetTLorentzVector(
    G4ThreeVector xyz, G4double t, TLorentzVector& lv) const;
  const G4VTouchable* GetCurrentTouchable() const;
  G4VPhysicalVolume* GetCurrentOffPhysicalVolume(
    G4int off, G4bool warn = false) const;

  // static data members
  static G4ThreadLocal TG4StepManager* fgInstance; ///< this instance

  //
  // data members

  /// current track
  G4Track* fTrack;

  /// current step
  G4Step* fStep;

  /// current Gflash spot
  G4GFlashSpot* fGflashSpot;

  /// \brief step status
  /// \details that decides whether track properties will be returned from
  /// PreStepPoint or PostStepPoint
  TG4StepStatus fStepStatus;

  /// \brief limits which step limit was modified during tracking
  TG4Limits* fLimitsModifiedOnFly;

  /// G4SteppingManager
  G4SteppingManager* fSteppingManager;

  /// buffer for current volume name or path
  mutable G4String fNameBuffer;

  /// volume copy number offset
  G4int fCopyNoOffset;

  /// division copy number offset
  G4int fDivisionCopyNoOffset;

  /// Cached pointer to thread-local track manager
  TG4TrackManager* fTrackManager;

  /// The initial status of a VMC track when it was popped from the VMC stack
  TMCParticleStatus* fInitialVMCTrackStatus;
};

// inline methods

inline TG4StepManager* TG4StepManager::Instance()
{
  /// Return this instance.
  return fgInstance;
}

inline void TG4StepManager::SetStep(G4Step* step, TG4StepStatus status)
{
  /// Set current step and step status.
  fTrack = step->GetTrack();
  fStep = step;
  fStepStatus = status;
  fGflashSpot = 0;
}

inline void TG4StepManager::SetStep(G4Track* track, TG4StepStatus status)
{
  /// Set current track and step status.
  fTrack = track;
  fStep = 0;
  fStepStatus = status;
  fGflashSpot = 0;
}

inline void TG4StepManager::SetStep(
  G4GFlashSpot* gflashSpot, TG4StepStatus status)
{
  /// Set current track and step status.
  fTrack =
    const_cast<G4Track*>(gflashSpot->GetOriginatorTrack()->GetPrimaryTrack());
  fStep = 0;
  fStepStatus = status;
  fGflashSpot = gflashSpot;
}

inline void TG4StepManager::SetSteppingManager(G4SteppingManager* manager)
{
  /// Set G4 stepping manger.
  fSteppingManager = manager;

  /// Set navigator !!!
  // G4cout << "SetNavigator:"
  //       << G4TransportationManager::GetTransportationManager()
  //		   ->GetNavigatorForTracking() << G4endl;
  fSteppingManager->SetNavigator(
    G4TransportationManager::GetTransportationManager()
      ->GetNavigatorForTracking());
}

inline G4Track* TG4StepManager::GetTrack() const
{
  /// Return current track manger.
  return fTrack;
}

inline G4Step* TG4StepManager::GetStep() const
{
  /// Return current step.
  return fStep;
}

inline TG4StepStatus TG4StepManager::GetStepStatus() const
{
  /// Return current step status.
  return fStepStatus;
}

inline TG4Limits* TG4StepManager::GetLimitsModifiedOnFly() const
{
  /// Return limits that has been modified on fly
  return fLimitsModifiedOnFly;
}

#endif // TG4_STEP_MANAGER_H
