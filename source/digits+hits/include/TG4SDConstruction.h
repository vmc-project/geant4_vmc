#ifndef TG4_SD_CONSTRUCTION_H
#define TG4_SD_CONSTRUCTION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SDConstruction.h
/// \brief Definition of the TG4SDConstruction class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4SDMessenger.h"
#include "TG4Verbose.h"

#include <set>

class G4LogicalVolume;

/// \ingroup digits_hits
/// \class TG4SDConstruction
/// \brief Sensitive detector construction
///
/// Class for construction of user sensitive detectors.
/// Constructed sensitive detectors inherit from
/// TG4VSensitiveDetector (see TG4VSensitiveDetector.h description);
/// all cloned logical volumes (which a single G3 volume correspond to)
/// share the same sensitive detector instance.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4SDConstruction : public TG4Verbose

{
 public:
  TG4SDConstruction();
  virtual ~TG4SDConstruction();

  // static methods
  static const G4String& GetDefaultSVLabel();

  // methods
  virtual void Construct();

  // set methods
  void AddSelection(const G4String& selection);
  void SetSelectionFromTGeo(G4bool value);
  void SetSensitiveVolumeLabel(const G4String& label);
  void SetIsGflash(G4bool isGflash);

 private:
  // methods
  G4int CreateSD(G4LogicalVolume* lv) const;
  void FillSDSelectionFromTGeo();

  TG4SDMessenger fMessenger; ///< messenger

  /// default value of the sensitive volumes label
  static const G4String fgkDefaultSVLabel;

  /// the flag to activate retrieving sensitive volumes from TGeo
  G4bool fSelectionFromTGeo;

  /// the label used to flag sensitive volumes in TGeo
  G4String fSVLabel;

  /// the set of volumes names which are selected as sensitive
  std::set<G4String> fSelection;

  /// the flag to acivate creating Gflash sensitive detectors
  G4bool fIsGflash;
};

// inline functions

inline const G4String& TG4SDConstruction::GetDefaultSVLabel()
{
  /// Get the default value of the sensitive volumes label
  /// (the string which is used to select sensitive volumes in TGeoGeometry)
  return fgkDefaultSVLabel;
}

inline void TG4SDConstruction::SetSelectionFromTGeo(G4bool value)
{
  /// Set option to get the SD selection from TGeo
  fSelectionFromTGeo = value;
}

inline void TG4SDConstruction::SetSensitiveVolumeLabel(const G4String& label)
{
  /// Set the sensitive volumes label
  /// (the string which is used to select sensitive volumes in TGeoGeometry)
  fSVLabel = label;
}

inline void TG4SDConstruction::SetIsGflash(G4bool isGflash)
{
  /// Set the flag to acivate creating Gflash sensitive detectors
  fIsGflash = isGflash;
}

#endif // TG4_SD_CONSTRUCTION_H
