//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4OpGeometryManager.cxx
/// \brief Implementation of the TG4OpGeometryManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4OpGeometryManager.h"
#include "TG4G3ControlVector.h"
#include "TG4G3CutVector.h"
#include "TG4G3Units.h"
#include "TG4GeometryServices.h"
#include "TG4Globals.h"
#include "TG4Limits.h"
#include "TG4Medium.h"
#include "TG4MediumMap.h"

#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4EllipticalTube.hh>
#include <G4Hype.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4OpticalSurface.hh>
#include <G4Para.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4ReflectedSolid.hh>
#include <G4Sphere.hh>
#include <G4SystemOfUnits.hh>
#include <G4Torus.hh>
#include <G4Transform3D.hh>
#include <G4Trap.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4TwistedTrap.hh>

#include <Riostream.h>
#include <TArrayD.h>
#include <TString.h>

//_____________________________________________________________________________
TG4OpGeometryManager::TG4OpGeometryManager()
  : TG4Verbose("opGeometryManager"), fGeometryServices(0), fOpSurfaceMap(0)
{
  /// Default constructor

  fGeometryServices = TG4GeometryServices::Instance();
  if (!fGeometryServices) {
    TG4Globals::Exception("TG4OpGeometryManager", "TG4OpGeometryManager",
      "TG4GeometryServices have to be defined first");
  }
  fOpSurfaceMap = fGeometryServices->GetOpSurfaceMap();
}

//_____________________________________________________________________________
TG4OpGeometryManager::~TG4OpGeometryManager()
{
  /// Destructor
}

//
// private methods
//

//_____________________________________________________________________________
Double_t TG4OpGeometryManager::AddUnit(
  const G4String& propertyName, Double_t value) const
{
  /// Add unit to a given property value and print a warning
  /// if the property is not known to the VMC interface.
  /// The properties are ordered as they appear in
  /// Geant4 Book For Applcation Developers (Geant4 v11.0)

  // clang-format off
  if (propertyName == "ABSLENGTH" ||
      propertyName == "RAYLEIGH" ||
      propertyName == "WLSABSLENGTH" ||
      propertyName == "WLSABSLENGTH2" ||
      propertyName == "MIEHG") {
    return value * TG4G3Units::Length();
  }

  if (propertyName == "BIRKS_CONSTANT") {

    return value * TG4G3Units::Length() * TG4G3Units::InverseEnergy();
  }

  if (propertyName == "ALPHASCINTILLATIONYIELD" ||
      propertyName == "DEUTERONSCINTILLATIONYIELD" ||
      propertyName == "ELECTRONSCINTILLATIONYIELD" ||
      propertyName == "IONSCINTILLATIONYIELD" ||
      propertyName == "PROTONSCINTILLATIONYIELD" ||
      propertyName == "SCINTILLATIONYIELD" ||
      propertyName == "TRITONSCINTILLATIONYIELD") {

    return value * TG4G3Units::InverseEnergy();
  }

  if (propertyName == "SCINTILLATIONRISETIME1" ||
      propertyName == "SCINTILLATIONRISETIME2" ||
      propertyName == "SCINTILLATIONRISETIME3" ||
      propertyName == "SCINTILLATIONTIMECONSTANT1" ||
      propertyName == "SCINTILLATIONTIMECONSTANT2" ||
      propertyName == "SCINTILLATIONTIMECONSTANT3" ||
      propertyName == "WLSTIMECONSTANT" ||
      propertyName == "WLSTIMECONSTANT2")  {

    return value * TG4G3Units::Time();
  }

  if (propertyName == "ISOTHERMAL_COMPRESSIBILITY" ) {
    return value * std::pow(TG4G3Units::Length(), 3) * TG4G3Units::InverseEnergy();
  }

  if (propertyName == "ALPHASCINTILLATIONYIELD1" ||
      propertyName == "ALPHASCINTILLATIONYIELD2" ||
      propertyName == "ALPHASCINTILLATIONYIELD3" ||
      propertyName == "DEUTERONCINTILLATIONYIELD1" ||
      propertyName == "DEUTERONCINTILLATIONYIELD2" ||
      propertyName == "DEUTERONCINTILLATIONYIELD3" ||
      propertyName == "ELECTRONINTILLATIONYIELD1" ||
      propertyName == "ELECTRONINTILLATIONYIELD2" ||
      propertyName == "ELECTRONINTILLATIONYIELD3" ||
      propertyName == "IONSCINTILLATIONYIELD1" ||
      propertyName == "IONSCINTILLATIONYIELD2" ||
      propertyName == "IONSCINTILLATIONYIELD3" ||
      propertyName == "PROTONSCINTILLATIONYIELD1" ||
      propertyName == "PROTONSCINTILLATIONYIELD2" ||
      propertyName == "PROTONSCINTILLATIONYIELD3" ||
      propertyName == "RESOLUTIONSCALE" ||
      propertyName == "SCINTILLATIONCOMPONENT1" ||
      propertyName == "SCINTILLATIONCOMPONENT2" ||
      propertyName == "SCINTILLATIONCOMPONENT3" ||
      propertyName == "SCINTILLATIONYIELD1" ||
      propertyName == "SCINTILLATIONYIELD2" ||
      propertyName == "SCINTILLATIONYIELD3" ||
      propertyName == "TRITONSCINTILLATIONYIELD1" ||
      propertyName == "TRITONSCINTILLATIONYIELD2" ||
      propertyName == "TRITONSCINTILLATIONYIELD3" ||
      propertyName == "RS_SCALE_FACTOR" ||
      propertyName == "WLSCOMPONENT" ||
      propertyName == "WLSCOMPONENT2" ||
      propertyName == "WLSMEANNUMBERPHOTONS" ||
      propertyName == "WLSMEANNUMBERPHOTONS2" ||
      propertyName == "MIEHG_BACKWARD" ||
      propertyName == "MIEHG_FORWARD" ||
      propertyName == "MIEHG_FORWARD_RATIO" ||
      propertyName == "BACKSCATTERCONSTANT" ||
      propertyName == "EFFICIENCY" ||
      propertyName == "GROUPVEL" ||  // Check
      propertyName == "IMAGINARYRINDEX" ||
      propertyName == "REFLECTIVITY" ||
      propertyName == "REALRINDEX" ||
      propertyName == "RINDEX" ||
      propertyName == "SPECULARLOBECONSTANT" ||
      propertyName == "SPECULARSPIKECONSTANT" ||
      propertyName == "SURFACEROUGHNESS" ||
      propertyName == "TRANSMITTANCE") {

    return value;
  }
  // clang-format on

  TString text = "Unknown material property ";
  text += propertyName.data();
  text += "x\n";
  text += "No units conversion performed.";
  TG4Globals::Warning("TG4OpGeometryManager", "AddUnit", text);
  return value;
}

//
// public methods - TVirtualMC implementation
//

//_____________________________________________________________________________
void TG4OpGeometryManager::SetCerenkov(Int_t itmed, Int_t npckov,
  Float_t* ppckov, Float_t* absco, Float_t* effic, Float_t* rindex,
  Bool_t aspline, Bool_t rspline)
{
  ///
  ///  Geant3 desription:
  ///  ==================
  ///
  ///    Stores the tables for UV photon tracking in medium ITMED
  ///    Please note that it is the user's responsability to
  ///    provide all the coefficients:
  ///
  ///
  ///     - ITMED       Tracking medium number
  ///     - NPCKOV      Number of bins of each table
  ///     - PPCKOV      Value of photon momentum (in GeV)
  ///     - ABSCO       Absorbtion coefficients
  ///                   dielectric: absorbtion length in cm
  ///                   metals    : absorbtion fraction (0<=x<=1)
  ///     - EFFIC       Detection efficiency for UV photons
  ///     - RINDEX      Refraction index (if=0 metal)

  G4double* ppckovDbl = fGeometryServices->CreateG4doubleArray(ppckov, npckov);
  G4double* abscoDbl = fGeometryServices->CreateG4doubleArray(absco, npckov);
  G4double* efficDbl = fGeometryServices->CreateG4doubleArray(effic, npckov);
  G4double* rindexDbl = fGeometryServices->CreateG4doubleArray(rindex, npckov);

  SetCerenkov(
    itmed, npckov, ppckovDbl, abscoDbl, efficDbl, rindexDbl, aspline, rspline);

  delete[] ppckovDbl;
  delete[] abscoDbl;
  delete[] efficDbl;
  delete[] rindexDbl;
}

//_____________________________________________________________________________
void TG4OpGeometryManager::SetCerenkov(Int_t itmed, Int_t npckov,
  Double_t* ppckov, Double_t* absco, Double_t* /*effic*/, Double_t* rindex,
  Bool_t aspline, Bool_t rspline)
{
  ///
  ///  Geant3 desription:
  ///  ==================
  ///
  ///    Stores the tables for UV photon tracking in medium ITMED
  ///    Please note that it is the user's responsability to
  ///    provide all the coefficients:
  ///
  ///
  ///     - ITMED       Tracking medium number
  ///     - NPCKOV      Number of bins of each table
  ///     - PPCKOV      Value of photon momentum (in GeV)
  ///     - ABSCO       Absorbtion coefficients
  ///                   dielectric: absorbtion length in cm
  ///                   metals    : absorbtion fraction (0<=x<=1)
  ///     - EFFIC       Detection efficiency for UV photons
  ///     - RINDEX      Refraction index (if=0 metal)

  // add units and detect if Rindex == 0
  G4bool isRindex = false;
  G4double* ppckov2 = fGeometryServices->CreateG4doubleArray(ppckov, npckov);
  G4double* absco2 = fGeometryServices->CreateG4doubleArray(absco, npckov);
  for (G4int i = 0; i < npckov; i++) {
    ppckov2[i] *= TG4G3Units::Energy();
    absco2[i] *= TG4G3Units::Length();
    if (rindex[i] != 0.0) isRindex = true;
  }

  // get material of medium from table
  TG4Medium* medium = fGeometryServices->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    TString text = "Medium ";
    text += itmed;
    text += " not found.";
    TG4Globals::Warning("TG4OpGeometryManager", "SetCerenkov", text);
    delete[] ppckov2;
    delete[] absco2;
    return;
  }
  G4Material* material = medium->GetMaterial();

  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable();
    material->SetMaterialPropertiesTable(table);
  }

  // add material properties into the table
  table->AddProperty("ABSLENGTH", ppckov2, absco2, npckov, false, aspline);
  // used in G4OpAbsorption process

  // table->AddProperty("EFFICIENCY", ppckov2, effic, npckov);
  // EFFICIENCY must be set to optical surfaces

  // Set RINDEX only if defined with non zero values.
  // The zero value is a Geant3 convention to define material as metal which was
  // not adopted in Geant4, where 0 values cause unpredicatable behavior in
  // G4OpBoundary process
  if (isRindex) {
    table->AddProperty("RINDEX", ppckov2, rindex, npckov, false, rspline);
    // used in G4Cerenkov, G4OpRayleigh, G4OpBoundary
  }

  delete[] ppckov2;
  delete[] absco2;

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The tables for UV photon tracking set for "
           << material->GetName() << G4endl;
  }
}

//_____________________________________________________________________________
void TG4OpGeometryManager::DefineOpSurface(const char* name,
  EMCOpSurfaceModel model, EMCOpSurfaceType surfaceType,
  EMCOpSurfaceFinish surfaceFinish, Double_t sigmaAlpha)
{
  /// Define the optical surface

  auto g4Model = fGeometryServices->SurfaceModel(model);
  auto g4Type = fGeometryServices->SurfaceType(surfaceType);
  auto g4Finish = fGeometryServices->SurfaceFinish(surfaceFinish);
  auto surface =
    new G4OpticalSurface(name, g4Model, g4Finish, g4Type, sigmaAlpha);

  // Store the surface in the map
  (*fOpSurfaceMap)[name] = surface;
}

//_____________________________________________________________________________
void TG4OpGeometryManager::SetBorderSurface(const char* name,
  const char* vol1Name, int vol1CopyNo, const char* vol2Name, int vol2CopyNo,
  const char* opSurfaceName)
{
  /// Define the optical border surface

  // Get physical volumes
  G4VPhysicalVolume* pv1 =
    fGeometryServices->FindPhysicalVolume(vol1Name, vol1CopyNo, true);
  G4VPhysicalVolume* pv2 =
    fGeometryServices->FindPhysicalVolume(vol2Name, vol2CopyNo, true);

  if (!pv1 || !pv2) {
    TString text;
    if (!pv1) {
      text = TString("Cannot find physical volume: ") + vol1Name;
      text += TG4Globals::Endl();
    }
    if (!pv2) text += TString("Cannot find physical volume: ") + vol2Name;
    TG4Globals::Warning("TG4OpGeometryManager", "SetBorderSurface", text);
    return;
  }

  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap->find(opSurfaceName);
  if (it == fOpSurfaceMap->end()) {
    TG4Globals::Warning("TG4OpGeometryManager", "SetBorderSurface",
      TString("Cannot find optical surface: ") + opSurfaceName);
    return;
  }
  G4OpticalSurface* surface = (*it).second;

  // Create the border surface
  new G4LogicalBorderSurface(name, pv1, pv2, surface);
}

//_____________________________________________________________________________
void TG4OpGeometryManager::SetSkinSurface(
  const char* name, const char* volName, const char* opSurfaceName)
{
  /// Define the optical skin surface

  // Get logical volume
  G4LogicalVolume* lv = fGeometryServices->FindLogicalVolume(volName, true);

  if (!lv) {
    TG4Globals::Warning("TG4OpGeometryManager",
      "SetSkinSurface:", "Cannot find logical volume: " + TString(volName));
    return;
  }

  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap->find(opSurfaceName);
  if (it == fOpSurfaceMap->end()) {
    TG4Globals::Warning("TG4OpGeometryManager", "SetBorderSurface",
      "Cannot find optical surface: " + TString(opSurfaceName));
    return;
  }
  G4OpticalSurface* surface = (*it).second;

  // Create the skin surface
  new G4LogicalSkinSurface(name, lv, surface);
}

//_____________________________________________________________________________
void TG4OpGeometryManager::SetMaterialProperty(Int_t itmed,
  const char* propertyName, Int_t np, Double_t* pp, Double_t* values,
  Bool_t createNewKey, Bool_t spline)
{
  /// Set the material property specified by propertyName to the tracking medium

  // create material properties table
  // get material of medium from table
  TG4Medium* medium = fGeometryServices->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    TString text = "Medium ";
    text += itmed;
    text += "not found.";
    TG4Globals::Warning("TG4OpGeometryManager", "SetMaterialProperty", text);
    return;
  }
  G4Material* material = medium->GetMaterial();

  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable();
    material->SetMaterialPropertiesTable(table);
  }

  // add units
  G4double* pp2 = fGeometryServices->CreateG4doubleArray(pp, np);
  G4double* val2 = fGeometryServices->CreateG4doubleArray(values, np);
  for (G4int i = 0; i < np; i++) {
    pp2[i] = pp2[i] * TG4G3Units::Energy();
    val2[i] = AddUnit(propertyName, values[i]);
  }
  table->AddProperty(propertyName, pp2, val2, np, createNewKey, spline);

  delete[] pp2;
  delete[] val2;

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material properties " << propertyName << " set for "
           << material->GetName() << G4endl;
  }
}

//_____________________________________________________________________________
void TG4OpGeometryManager::SetMaterialProperty(
  Int_t itmed, const char* propertyName, Double_t value)
{
  /// Set the material property specified by propertyName to the tracking medium

  // create material properties table
  // get material of medium from table
  TG4Medium* medium = fGeometryServices->GetMediumMap()->GetMedium(itmed);
  if (!medium) {
    TString text = "Medium ";
    text += itmed;
    text += "not found.";
    TG4Globals::Warning("TG4OpGeometryManager", "SetMaterialProperty", text);
    return;
  }
  G4Material* material = medium->GetMaterial();

  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable();
    material->SetMaterialPropertiesTable(table);
  }

  // Add units
  value = AddUnit(propertyName, value);

  // Birks constant is set in a different way
  if (G4String(propertyName) == "BIRKS_CONSTANT") {
    material->GetIonisation()->SetBirksConstant(value);
  }
  else {
    table->AddConstProperty(propertyName, value);
  }

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material property " << propertyName << " set for "
           << material->GetName() << G4endl;
  }
}

//_____________________________________________________________________________
void TG4OpGeometryManager::SetMaterialProperty(const char* surfaceName,
  const char* propertyName, Int_t np, Double_t* pp, Double_t* values,
  Bool_t createNewKey, Bool_t spline)
{
  /// Set the material property specified by propertyName to the optical surface

  // create material properties table
  // get optical surface from the map
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap->find(surfaceName);
  if (it == fOpSurfaceMap->end()) {
    TG4Globals::Warning("TG4OpGeometryManager", "SetMaterialProperty",
      "Cannot find optical surface: " + TString(surfaceName));
    return;
  }
  G4OpticalSurface* surface = (*it).second;

  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table = surface->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable();
    surface->SetMaterialPropertiesTable(table);
  }

  // add units
  G4double* pp2 = fGeometryServices->CreateG4doubleArray(pp, np);
  G4double* val2 = fGeometryServices->CreateG4doubleArray(values, np);
  for (G4int i = 0; i < np; i++) {
    pp2[i] = pp2[i] * TG4G3Units::Energy();
    val2[i] = AddUnit(propertyName, values[i]);
  }
  table->AddProperty(propertyName, pp2, val2, np, createNewKey, spline);
  delete[] pp2;
  delete[] val2;

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material properties " << propertyName
           << " set for optical surface " << surface->GetName() << G4endl;
  }
}

//_____________________________________________________________________________
void TG4OpGeometryManager::Gfmate(Int_t imat, char* name, Float_t& a,
  Float_t& z, Float_t& dens, Float_t& radl, Float_t& absl, Float_t* ubuf,
  Int_t& nbuf)
{
  /// Return parameters for material specified by material number imat

  G4double da, dz, ddens, dradl, dabsl;
  Double_t dubuf[100];
  Gfmate(imat, name, da, dz, ddens, dradl, dabsl, dubuf, nbuf);

  a = da;
  z = dz;
  dens = ddens;
  radl = dradl;
  absl = dabsl;
  for (Int_t i = 0; i < nbuf; i++) ubuf[i] = dubuf[i];
}

//_____________________________________________________________________________
void TG4OpGeometryManager::Gfmate(Int_t imat, char* name, Double_t& a,
  Double_t& z, Double_t& dens, Double_t& radl, Double_t& absl,
  Double_t* /*ubuf*/, Int_t& nbuf)
{
  /// Return parameters for material specified by material number imat

  TG4Globals::Warning("TG4OpGeometryManager", "Gfmate",
    "Deprecated function - now replaced with GetMaterial(Int_t imat, ...)");

  G4Material* material = G4Material::GetMaterialTable()->at(imat - 1);

  if (material) {
    const char* chName = material->GetName();
    strcpy(name, chName);
    a = fGeometryServices->GetEffA(material);
    z = fGeometryServices->GetEffZ(material);

    dens = material->GetDensity();
    dens /= TG4G3Units::MassDensity();

    radl = material->GetRadlen();
    radl /= TG4G3Units::Length();

    // the following parameters are not defined in Geant4
    absl = 0.;
    nbuf = 0;
  }
  else {
    TString text = "Material ";
    text += imat;
    text += " has not been found.";
    TG4Globals::Exception("TG4OpGeometryManager", "Gfmate", text);
  }
}

//_____________________________________________________________________________
Bool_t TG4OpGeometryManager::GetMaterial(Int_t imat, TString& name, Double_t& a,
  Double_t& z, Double_t& density, Double_t& radl, Double_t& inter, TArrayD& par)
{
  /// Return parameters for material specified by material number imat
  /// (new function)

  G4Material* material = G4Material::GetMaterialTable()->at(imat - 1);

  if (!material) {
    TString text = "Material ";
    text += imat;
    text += " has not been found.";
    TG4Globals::Warning("TG4OpGeometryManager", "GetMaterial", text);
    return false;
  }

  name = material->GetName();
  a = fGeometryServices->GetEffA(material);
  z = fGeometryServices->GetEffZ(material);

  density = material->GetDensity();
  density /= TG4G3Units::MassDensity();

  radl = material->GetRadlen();
  radl /= TG4G3Units::Length();

  inter = 0.; // TO DO: check how to get this
  par.Set(0);
  return true;
}
