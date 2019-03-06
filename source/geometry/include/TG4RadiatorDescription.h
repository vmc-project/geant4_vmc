#ifndef TG4_RADIATOR_DESCRIPTION_H
#define TG4_RADIATOR_DESCRIPTION_H

//-------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4RadiatorDescription.h
/// \brief Definition of the TG4RadiatorDescription class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>

#include <vector>
#include <tuple>

/// \ingroup geometry
/// \brief The radiator description
///
/// The class defines the radiator parameters which are used
/// by transition radiation processes.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4RadiatorDescription
{
  public:
    using Component = std::tuple<G4String, G4double, G4double>;

  public:
    TG4RadiatorDescription(const G4String& volumeName = "");
    virtual ~TG4RadiatorDescription();

    // methods
    void PrintParameters() const;

    // set methods
    void SetVolumeName(const G4String& name);
    void SetXtrModel(const G4String& name);
    void SetFoilNumber(G4int foilNumber);
    void SetLayer(const G4String& materialName, G4double thickness, G4double fluctuation);
    void SetStrawTube(const G4String& materialName, G4double wallThickness, G4double gasThickness);

    // get methods
    G4String  GetVolumeName() const;
    G4String  GetXtrModel() const;
    G4int     GetFoilNumber() const;
    Component GetLayer(G4int i) const;
    Component GetStrawTube() const;

  private:
    /// The name of associated volume
    G4String  fVolumeName;

    /// The transition radiation process model
    G4String  fXtrModel;

    /// The number of foils
    G4int   fFoilNumber;

    /// The layer parameters
    std::vector<Component> fLayers;

    /// The straw tube parameters
    Component fStrawTube;
 };

// inline functions

/// Set the name of associated volume
inline void TG4RadiatorDescription::SetVolumeName(const G4String& name)
{  fVolumeName = name; }

/// Set the transition radiation process model
inline void  TG4RadiatorDescription::SetXtrModel(const G4String& name)
{ fXtrModel = name; }

/// Set the number of foils
inline void TG4RadiatorDescription::SetFoilNumber(G4int foilNumber)
{  fFoilNumber = foilNumber; }

/// Return the name of associated volume
inline G4String  TG4RadiatorDescription::GetVolumeName() const
{ return fVolumeName; }

/// Return the transition radiation process model
inline G4String  TG4RadiatorDescription::GetXtrModel() const
{ return fXtrModel; }

/// Return the number of foils
inline G4int  TG4RadiatorDescription::GetFoilNumber() const
{  return fFoilNumber; }

/// Return the straw tube parameters
inline TG4RadiatorDescription::Component TG4RadiatorDescription::GetStrawTube() const
{ return fStrawTube; }

#endif //TG4_RADIATOR_DESCRIPTION_H

