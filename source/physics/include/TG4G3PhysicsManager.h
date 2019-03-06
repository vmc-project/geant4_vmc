#ifndef TG4_G3_PHYSICS_MANAGER_H
#define TG4_G3_PHYSICS_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3PhysicsManager.h
/// \brief Definition of the TG4G3PhysicsManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Globals.h"
#include "TG4G3Defaults.h"
#include "TG4G3Cut.h"
#include "TG4G3Control.h"
#include "TG4G3ParticleWSP.h"

#include <Rtypes.h>

#include <globals.hh>

class TG4G3CutVector;
class TG4G3ControlVector;

class G4ParticleDefinition;
class G4VProcess;

/// \ingroup physics
/// \brief Provides a Geant3 way control to Geant4 physics.
///
/// The G3 cuts and process controls are
/// stored in fCutVector and fControlVector vectors.
/// These special cuts/controls are activated
/// by registering their physics constructors
/// (TG4PhysicsConstructorSpecialCuts, TG4PhysicsConstructorSpecialControl)
/// to the physics list.
///
/// \author I. Hrivnacova; IPN Orsay

class TG4G3PhysicsManager
{
  public:
    TG4G3PhysicsManager();
    virtual ~TG4G3PhysicsManager();

    // static access method
    static TG4G3PhysicsManager* Instance();

    // methods
    void Lock();
    void CheckLock();
    G4bool CheckCutWithTheVector(
             G4String name, G4double value, TG4G3Cut& cut);
    G4bool CheckControlWithTheVector(
             G4String name, G4double value,
             TG4G3Control& control, TG4G3ControlValue& controlValue);

    // set methods
    void SetCut(TG4G3Cut cut, G4double cutValue);
    void SetProcess(TG4G3Control control, TG4G3ControlValue controlValue);
    void SetG3DefaultCuts();
    void SetG3DefaultControls();

    // get methods
    G4bool IsSpecialCuts() const;
    G4bool IsSpecialControls() const;
    G4bool IsGlobalSpecialControls() const;
    TG4G3CutVector*     GetCutVector() const;
    TG4G3ControlVector* GetControlVector() const;
    TG4boolVector*      GetIsCutVector() const;
    TG4boolVector*      GetIsControlVector() const;
          // conversions
    TG4G3ParticleWSP  GetG3ParticleWSP(G4ParticleDefinition* particle) const;
    G4String GetG3ParticleWSPName(G4int particleWSP) const;

  private:
    /// Not implemented
    TG4G3PhysicsManager(const TG4G3PhysicsManager& right);
    /// Not implemented
    TG4G3PhysicsManager& operator=(const TG4G3PhysicsManager& right);

    // set methods
    void SwitchIsCutVector(TG4G3Cut cut);
    void SwitchIsControlVector(TG4G3Control control);

    // static data members
    static TG4G3PhysicsManager*  fgInstance; ///< this instance

    //
    // data members

    /// TG4G3CutVector
    TG4G3CutVector*      fCutVector;

    /// TG4G3ControlVector
    TG4G3ControlVector*  fControlVector;

    /// vector of booleans which cuts are set
    TG4boolVector*       fIsCutVector;

    /// vector of booleans which controls are set
    TG4boolVector*       fIsControlVector;

    /// G3 default cuts/controls
    TG4G3Defaults        fG3Defaults;

    /// if true: cut/control vectors cannot be modified
    G4bool               fLock;
};

// inline methods

inline TG4G3PhysicsManager* TG4G3PhysicsManager::Instance() {
  /// Return this instance
  return fgInstance;
}

inline void TG4G3PhysicsManager::Lock()  {
  /// Lock - the cut/control vectors cannot be modified
  /// after calling this function
  fLock = true;
}

inline TG4G3CutVector* TG4G3PhysicsManager::GetCutVector() const {
  /// Return the vector of cut values
  return fCutVector;
}

inline TG4G3ControlVector* TG4G3PhysicsManager::GetControlVector() const {
  /// Return the vector of control values
  return fControlVector;
}

inline TG4boolVector* TG4G3PhysicsManager::GetIsCutVector() const {
  /// Return the vector of boolean info which cuts are set
  return fIsCutVector;
}

inline TG4boolVector* TG4G3PhysicsManager::GetIsControlVector() const {
  /// Return the vector of boolean info which controls are set
  return fIsControlVector;
}

#endif //TG4_PHYSICS_MANAGER_H

