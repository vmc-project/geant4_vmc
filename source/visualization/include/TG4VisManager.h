#ifndef TG4_VIS_MANAGER_H
#define TG4_VIS_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VisManager.h
/// \brief Definition of the TG4VisManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4G3Attribute.h"

#include <G4VisManager.hh>
#include <vector>

#include <Rtypes.h>

/// \ingroup visualization
/// \class TG4VisManager
/// \brief Visualization manager class
///
/// G4VisManager provides Geant4 implementation of
/// the TVirtualMC interface methods for visualization.
///
/// \author: I. Hrivnacova, IPN, Orsay; A. Gheata

class TG4VisManager
{
 public:
  /// The vector of G4 logical volumes
  typedef std::vector<G4LogicalVolume*> LogicalVolumesVector;

  /// The vector of G4 physical volumes
  typedef std::vector<G4VPhysicalVolume*> PhysicalVolumesVector;

 public:
  TG4VisManager(G4int verboseLevel = 0);
  // Controls initial verbose level of VisManager and VisMessenger.
  // Can be changed by /vis/set/verbose.
  ~TG4VisManager();

  //----------------------
  // functions for drawing
  //----------------------
  void DrawOneSpec(const char* name); // not implemented

  // see TG4VisManager.cxx for detailed description of Gsatt(), Gdraw()
  void Gsatt(const char* name, const char* att, Int_t val);
  void Gdraw(const char* name, Float_t theta, Float_t phi, Float_t psi,
    Float_t u0, Float_t v0, Float_t ul, Float_t vl);
  void SetColors();

 private:
  /// Not implemented
  TG4VisManager(const TG4VisManager& right);
  /// Not implemented
  TG4VisManager& operator=(const TG4VisManager& right);

  // methods
  //--------
  G4bool NeedSetColours();
  void SetColourFlag(G4bool value);

  // methods used by Gsatt(), Gdraw()
  //---------------------------------

  G4bool Contains(
    const LogicalVolumesVector& lvVector, const G4LogicalVolume* lv) const;
  G4bool Contains(
    const PhysicalVolumesVector& pvVector, const G4VPhysicalVolume* pv) const;

  // Get the logical volume list corresponding to NAME
  //         Either a logical or physical volume name can be supplied
  // Clones of G3VOLUME_NUMBER will be atached to the list
  LogicalVolumesVector GetLVList(G4String name);

  // Get the physical volume list corresponding to NAME
  PhysicalVolumesVector GetPVList(G4String name);

  // Case insensitive string comparison
  G4bool CaseInsensitiveEqual(const G4String string1, const G4String string2);

  // Return true if the vis. attributes pointer corresponding to the
  //        selected volume is shared by others. In this case, duplication
  //  of those is mandatory
  G4bool IsSharedVisAttributes(const G4LogicalVolume* pLV);

  // Set an attribute to a specific volume
  void SetG4Attribute(
    G4LogicalVolume* const lv, const TG4G3Attribute att, const G4int val);
  // Set an attribute to the tree coresponding to a volume
  void SetAtt4Daughters(
    G4LogicalVolume* const lv, const TG4G3Attribute att, const G4int val);

  // data members
  //------------
  G4bool fColourFlag;  ///< colour flag
  G4int fVerboseLevel; ///< verbose level
};

// inline methods

inline G4bool TG4VisManager::NeedSetColours()
{
  /// Return colour flag
  return fColourFlag;
}

inline void TG4VisManager::SetColourFlag(G4bool value)
{
  /// Set colour flag
  fColourFlag = value;
}

#endif // TG4_VIS_MANAGER_H
