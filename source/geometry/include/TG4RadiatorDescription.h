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
    TG4RadiatorDescription(const G4String& volumeName = "");
    virtual ~TG4RadiatorDescription();

    // methods
    void PrintParameters() const;
    
    // set methods
    void SetVolumeName(const G4String& name);
    void SetXtrModel(const G4String& name);
    void SetFoilMaterialName(const G4String& name);
    void SetGasMaterialName(const G4String& name);
    void SetStrawTubeMaterialName(const G4String& name);
    void SetFoilThickness(G4double thickness);
    void SetGasThickness(G4double thickness);
    void SetFoilNumber(G4int foilNumber);

    // get methods
    G4String  GetVolumeName() const;
    G4String  GetXtrModel() const;
    G4String  GetFoilMaterialName() const;
    G4String  GetGasMaterialName() const;
    G4String  GetStrawTubeMaterialName() const;
    G4double  GetFoilThickness() const;
    G4double  GetGasThickness() const;
    G4int     GetFoilNumber() const;

  private:
    /// The name of associated volume
    G4String  fVolumeName;
    
    /// The transition radiation process model
    G4String  fXtrModel;

    /// The name of foil material
    G4String  fFoilMaterialName;

    /// The name of gas material
    G4String  fGasMaterialName;

    /// The name of straw tube material
    G4String  fStrawTubeMaterialName; 

    /// The foil thickness
    G4double  fFoilThickness;

    /// The gas thickness
    G4double  fGasThickness;

    /// The number of foils
    G4int     fFoilNumber;
 };

// inline functions

/// Set the name of associated volume
inline void TG4RadiatorDescription::SetVolumeName(const G4String& name)
{  fVolumeName = name; }

/// Set the transition radiation process model
inline void  TG4RadiatorDescription::SetXtrModel(const G4String& name)
{ fXtrModel = name; }

/// Set the name of foil material
inline void TG4RadiatorDescription::SetFoilMaterialName(const G4String& name)
{  fFoilMaterialName = name; }

/// Set the name of gas material
inline void TG4RadiatorDescription::SetGasMaterialName(const G4String& name)
{  fGasMaterialName = name; }

/// Set the name of straw tube material
inline void TG4RadiatorDescription::SetStrawTubeMaterialName(const G4String& name)
{  fStrawTubeMaterialName = name; }

/// Set the foil thickness
inline void TG4RadiatorDescription::SetFoilThickness(G4double thickness)
{  fFoilThickness = thickness; }

/// Set the gas thickness
inline void TG4RadiatorDescription::SetGasThickness(G4double thickness)
{  fGasThickness = thickness; }

/// Set the number of foils
inline void TG4RadiatorDescription::SetFoilNumber(G4int foilNumber)
{  fFoilNumber = foilNumber; }

/// Return the name of associated volume
inline G4String  TG4RadiatorDescription::GetVolumeName() const
{ return fVolumeName; }

/// Return the transition radiation process model
inline G4String  TG4RadiatorDescription::GetXtrModel() const
{ return fXtrModel; }

/// Return the name of foil material
inline G4String  TG4RadiatorDescription::GetFoilMaterialName() const
{  return fFoilMaterialName; }

/// Return the name of gas material
inline G4String  TG4RadiatorDescription::GetGasMaterialName() const
{  return fGasMaterialName; }

/// Return the name of straw tube material
inline G4String  TG4RadiatorDescription::GetStrawTubeMaterialName() const
{  return fStrawTubeMaterialName; }

/// Return the foil thickness
inline G4double  TG4RadiatorDescription::GetFoilThickness() const
{  return fFoilThickness; }

/// Return the gas thickness
inline G4double  TG4RadiatorDescription::GetGasThickness() const
{  return fGasThickness; }

/// Return the number of foils
inline G4int  TG4RadiatorDescription::GetFoilNumber() const
{  return fFoilNumber; }



#endif //TG4_FIELD_PARAMETERS_H

