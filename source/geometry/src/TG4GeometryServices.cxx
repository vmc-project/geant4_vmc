//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GeometryServices.cxx
/// \brief Implementation of the TG4GeometryServices class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4GeometryServices.h"
#include "TG4MediumMap.h"
#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4IntMap.h"
#include "TG4NameMap.h"
#include "TG4G3Units.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#include <G4LogicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4Element.hh>
#include <G4UserLimits.hh>
#ifdef USE_G3TOG4
#include <G3toG4.hh>
#include <G3EleTable.hh>
#endif

#include <TGeoMatrix.h>
#include "Riostream.h"

// Moved after Root includes to avoid shadowed variables
// generated from short units names
#include <G4SystemOfUnits.hh>

#include <vector>
#include <iomanip>
#include <math.h>

TG4GeometryServices* TG4GeometryServices::fgInstance = 0;
G4String             TG4GeometryServices::fgBuffer="";
const G4double       TG4GeometryServices::fgkAZTolerance = 0.001;
const G4double       TG4GeometryServices::fgkDensityTolerance = 0.005;

//_____________________________________________________________________________
TG4GeometryServices::TG4GeometryServices()
  : TG4Verbose("geometryServices"),
    fIsG3toG4(false),
    fMediumMap(0),
    fOpSurfaceMap(0),
    fWorld(0)
{
/// Default constructor

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4GeometryServices", "TG4GeometryServices",
      "Cannot create two instances of singleton.");
  }

  fMediumMap = new TG4MediumMap();
  fOpSurfaceMap = new TG4OpSurfaceMap();

  fgInstance = this;

}

//_____________________________________________________________________________
TG4GeometryServices::~TG4GeometryServices()
{
/// Destructor

  delete fMediumMap;
  delete fOpSurfaceMap;
  fgInstance = 0;
}

//
// private methods
//

#ifdef USE_G3TOG4
//_____________________________________________________________________________
G4bool TG4GeometryServices::IsG3Volume(const G4String& lvName) const
{
/// Return true if the logical volume of given volumeName
/// was not created by Gsposp method with a generic name
/// (name_copyNo).

  if (lvName.contains(gSeparator))
    return false;
  else
    return true;
}

//_____________________________________________________________________________
G4bool TG4GeometryServices::CompareElement(G4double /*a*/, G4double z,
                                           const G4Element* element) const
{
/// Compare given parameters with those of a given element,
/// return true if they are equal, false otherwise.

  G4double ae = element->GetA()/TG4G3Units::AtomicWeight();
  G4double ze = element->GetZ();

  // g3tog4 can redefine A
  G4double ax;
  if (z<1) {
    // vacuum
    ax = 1.01*g/mole;
  }
  else
    ax = G3Ele.GetEle(z)->GetA()/TG4G3Units::AtomicWeight();

  if ( std::abs(ax - ae) < fgkAZTolerance &&
       std::abs(z  - ze) < fgkAZTolerance )
    return true;
  else
    return false;
}
#else
//_____________________________________________________________________________
G4bool TG4GeometryServices::IsG3Volume(const G4String& /*lvName*/) const
{
/// Return true if the logical volume of given volumeName
/// was not created by Gsposp method with a generic name
/// (name_copyNo).

  return false;
}

//_____________________________________________________________________________
G4bool TG4GeometryServices::CompareElement(G4double /*a*/, G4double /*z*/,
                                           const G4Element* /*element*/) const
{
/// Compare given parameters with those of a given element,
/// return true if they are equal, false otherwise.

    return false;
}
#endif

//_____________________________________________________________________________
G4bool TG4GeometryServices::CompareMaterial(G4int nofElements, G4double density,
                                            const G4Material* material) const
{
/// Compare given density with those of a given material,
/// return true if they are equal, false otherwise.

  G4double dm = material->GetDensity()/TG4G3Units::MassDensity();
  G4int ne = material->GetNumberOfElements();

  // density percentual difference
  G4double diff = std::abs(density - dm)/(density + dm)*2.;

  if (nofElements == ne && diff < fgkDensityTolerance)
    return true;
  else
    return false;
}

//_____________________________________________________________________________
G4double* TG4GeometryServices::ConvertAtomWeight(G4int nmat,
                                                 G4double* a, G4double* wmat) const
{
/// In case of proportions given in atom counts (nmat<0),
/// the wmat[i] are converted to weight fractions.
/// (From g3tog4 G4gsmixt.)                                                  \n
/// The new array has to be deleted by client.

  G4double* weight = new G4double[abs(nmat)];

  if (nmat<0) {
    G4double aMol = 0.;
    G4int i;
    for (i=0; i<abs(nmat); i++) {
      // total molecular weight
      aMol += wmat[i]*a[i];
    }
    if ( aMol == 0. ) {
      TG4Globals::Warning(
        "TG4GeometryServices", "ConvertAtomWeight",
        "Total molecular weight = 0.");
    }
    for (i=0; i<abs(nmat); i++) {
      // weight fractions
      weight[i] = wmat[i]*a[i]/aMol;
    }
  }
  else
    for (G4int j=0; j<nmat; j++) weight[j] = wmat[j];

  return weight;
}

#if G4VERSION_NUMBER >= 930
//_____________________________________________________________________________
void TG4GeometryServices::DumpG4MaterialPropertiesTable(
                              G4MaterialPropertiesTable* table) const
{
/// Redefined method from Geant4.
/// In difference from G4 implementation the info about Null vector
/// is printed to G4cout and not as G4Exception.

  typedef std::map< G4String, G4MaterialPropertyVector*, std::less<G4String> > MPTMap;
  typedef std::map< G4String, G4double, std::less<G4String> > MPTCMap;
  typedef MPTMap::const_iterator MPTiterator;
  typedef MPTCMap::const_iterator MPTCiterator;

  const MPTMap* MPT = table->GetPropertiesMap();
  MPTiterator i;
  for (i = MPT->begin(); i != MPT->end(); ++i)
  {
    G4cout << (*i).first << G4endl;
    if ( (*i).second != 0 )
    {
      (*i).second->DumpValues();
    }
    else
    {
      G4cout << "  NULL Material Property Vector Pointer." << G4endl;
    }
  }

  const MPTCMap* MPTC = table->GetPropertiesCMap();
  MPTCiterator j;
  for (j = MPTC->begin(); j != MPTC->end(); ++j)
  {
    G4cout << j->first << G4endl;
    if ( j->second != 0 )
    {
      G4cout << j->second << G4endl;
    }
    else
    {
      G4cout << "  No Material Constant Property." << G4endl;
    }
  }
}
#endif
//
// public methods
//

//_____________________________________________________________________________
G4double* TG4GeometryServices::CreateG4doubleArray(Float_t* array,
               G4int size) const
{
/// Convert Float_t* array to G4double*.                                    \n
/// !! The new array has to be deleted by user.

  G4double* doubleArray;
  if (size>0) {
    doubleArray = new G4double[size];
    for (G4int i=0; i<size; i++) doubleArray[i] = array[i];
  }
  else {
    doubleArray = 0;
  }
  return doubleArray;
}

//_____________________________________________________________________________
G4double* TG4GeometryServices::CreateG4doubleArray(Double_t* array,
               G4int size) const
{
/// Copy Double_t* array to G4double*.                                    \n
/// !! The new array has to be deleted by user.

  G4double* doubleArray;
  if (size>0) {
    doubleArray = new G4double[size];
    for (G4int i=0; i<size; i++) doubleArray[i] = array[i];
  }
  else {
    doubleArray = 0;
  }
  return doubleArray;
}

//_____________________________________________________________________________
G4String TG4GeometryServices::CutName(const char* name) const
{
/// Remove spaces after the name if present.

  G4String cutName = name;
  G4int i = cutName.length();
  while (cutName(--i) == ' ') cutName = cutName(0,i);

  return cutName;
}

//_____________________________________________________________________________
G4String TG4GeometryServices::CutMaterialName(const char* name) const
{
/// Remove the $ with precedent spaces at the name if present.

  G4String cutName = name;
  cutName = cutName.substr(0,cutName.find('$'));

  return CutName(cutName);
}

//_____________________________________________________________________________
G4String  TG4GeometryServices::CutVolumePath(const G4String& volumePath,
                      G4String& volName, G4int& copyNo) const
{
/// Extract the first volume name and copy number from the volumePath

  G4String path(volumePath);

  G4int npos1 = path.find('/');
  G4int npos2 = path.find('_');
  G4int npos3 = path.find('/', 2);
  if ( npos3<0 ) npos3 = path.length();

  volName = path(npos1+1, npos2-npos1-1 );
  G4String copyNoStr = path(npos2+1, npos3-npos2 );
  std::istringstream in(copyNoStr);
  in >> copyNo;

  return path(npos3, path.length()-npos3);
}

//_____________________________________________________________________________
const G4String& TG4GeometryServices::UserVolumeName(const G4String& name) const
{
/// Cut _copyNo extension added to logical volume name in case
/// the logical volume was created by Gsposp method.

#ifdef USE_G3TOG4
  if ( fIsG3toG4 && name.contains(gSeparator) )  {
    fgBuffer = name.substr(0,name.first(gSeparator));
    return fgBuffer;
  }
  else {
    return name;
  }
#else
  return name;
#endif
}

//_____________________________________________________________________________
G4OpticalSurfaceModel
TG4GeometryServices::SurfaceModel(EMCOpSurfaceModel model) const
{
/// Convert VMC enum to G4 enum

  switch (model) {
    case kGlisur:  return glisur;
    case kUnified: return unified;
    default:
      TG4Globals::Warning(
        "TG4GeometryServices", "SurfaceModel",
        "Unknown optical surface model, return Glisur.");
      return glisur;
  }
}

//_____________________________________________________________________________
G4SurfaceType
TG4GeometryServices::SurfaceType(EMCOpSurfaceType surfType) const
{
/// Convert VMC enum to G4 enum

  switch (surfType) {
    case kDielectric_metal:      return dielectric_metal;
    case kDielectric_dielectric: return dielectric_dielectric;
    case kFirsov:                return firsov;
    case kXray:                  return x_ray;
    default:
      TG4Globals::Warning(
        "TG4GeometryServices", "SurfaceType",
        "Unknown optical surface type, return dielectric_metal.");
      return dielectric_metal;
  }
}

//_____________________________________________________________________________
G4OpticalSurfaceFinish
TG4GeometryServices::SurfaceFinish(EMCOpSurfaceFinish finish) const
{
/// Convert VMC enum to G4 enum

  switch (finish) {
    case kPolished:             return polished;
    case kPolishedfrontpainted: return polishedfrontpainted;
    case kPolishedbackpainted:  return polishedbackpainted;
    case kGround:               return ground;
    case kGroundfrontpainted:   return groundfrontpainted;
    case kGroundbackpainted:    return groundbackpainted;
    default:
      TG4Globals::Warning(
        "TG4GeometryServices", "SurfaceFinish",
        "Unknown optical surface finish, return polished.");
      return polished;
  }
}

//_____________________________________________________________________________
void  TG4GeometryServices::Convert(const G4Transform3D& transform,
                                   TGeoHMatrix& matrix) const
{
/// Convert CLHEP Transform3D in Root TGeoHMatrix

  Double_t* translation = new Double_t[3];
  Double_t* rotation = new Double_t[9];
  for (G4int i=0; i<3; i++)
    for (G4int j=0; j<3; j++) {
      rotation[i*3+j] = transform(i, j);
    translation[i] = transform(i, 3)/cm;
  }
  matrix.SetTranslation(translation);
  matrix.SetRotation(rotation);
  delete [] translation;
  delete [] rotation;
}

//_____________________________________________________________________________
G4Material* TG4GeometryServices::MixMaterials(G4String name, G4double density,
                                    const TG4StringVector& matNames,
                                    const TG4doubleVector& matWeights)
{
/// Create a mixture of selected materials.

  // number of materials to be mixed
  G4int nofMaterials = matNames.size();
  if ( nofMaterials != G4int(matWeights.size()) ) {
    TG4Globals::Exception(
      "TG4GeometryServices", "MixMaterials",
      "Different number of material names and weigths.");
  }

  if (VerboseLevel() > 1) {
    G4cout << "Nof of materials to be mixed: " << nofMaterials << G4endl;
  }

  // fill vector of materials
  std::vector<G4Material*> matVector;
  G4int im;
  for (im=0; im< nofMaterials; im++) {
    // material
    G4Material* material = G4Material::GetMaterial(matNames[im]);
    matVector.push_back(material);
  }

  // create the mixed material
  G4Material* mixture = new G4Material(name, density, nofMaterials);
  for (im=0; im< nofMaterials; im++) {
    G4Material* material = matVector[im];
    G4double fraction = matWeights[im];
    mixture->AddMaterial(material, fraction);
  }

  return mixture;
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintLimits(const G4String& name) const
{
/// Find the limits with the specified name and prints them.

  TG4Limits* limits = FindLimits(name, true);

  if (limits)  limits->Print();
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintVolumeLimits(const G4String& volumeName) const
{
/// Find a logical volume with the specified name and prints
/// its limits.

  G4LogicalVolume* lv = FindLogicalVolume(volumeName, false);

  if (lv) {
    TG4Limits* limits = GetLimits(lv->GetUserLimits());
    G4cout << volumeName << "  ";
    if (limits)
      limits->Print();
    else
      G4cout << "has not the limits set." << G4endl;
  }
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintStatistics(G4bool open, G4bool close) const
{
/// Print G4 geometry statistics.


  if (open)  TG4Globals::PrintStars(true);

  G4cout << "    GEANT4 Geometry statistics: " << G4endl
         << "          " << std::setw(5) << NofG4LogicalVolumes()
                         << " logical volumes" << G4endl
         << "          "
                         << std::setw(5) << NofG4PhysicalVolumes()
                         << " physical volumes" << G4endl
         << "          "
                         << std::setw(5) << G4Material::GetNumberOfMaterials()
                         << " materials"        << G4endl
         << "          "
                         << std::setw(5) << TG4Limits::GetNofLimits()
                         << " user limits"      << G4endl;

  if (close) TG4Globals::PrintStars(false);
}

//_____________________________________________________________________________
void
TG4GeometryServices::PrintLogicalVolumeStore() const
{
/// Print all logical volumes and their daughters.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  G4cout << "Logical volume store: " << G4endl;

  for (G4int i=0; i<G4int(lvStore->size()); i++) {

    G4LogicalVolume* lv = (*lvStore)[i];

    void* address = lv->GetMaterial();
    G4cout << "Logical volume: " << G4endl;
    G4cout << "  " << std::setw(5)  << i
           << "  " << lv
           << "  " << lv->GetName()
           << "  " << std::setw(5)  << lv->GetNoDaughters() << " daughters"
           << "  limits: " << lv->GetUserLimits()
           << "  material: " << lv->GetMaterial()->GetName()
           << "  " << address
           << G4endl;

    for (G4int j=0; j<lv->GetNoDaughters(); j++) {
      void* addressd = lv->GetDaughter(j)->GetLogicalVolume()->GetMaterial();
      G4cout << "  Daughter: "
             << std::setw(5)  << j
             << "  " << lv->GetDaughter(j)
               << "  " << lv->GetDaughter(j)->GetName()
             << "  of LV: " << lv->GetDaughter(j)->GetLogicalVolume()
               << "  " << lv->GetDaughter(j)->GetLogicalVolume()->GetName()
             << "  copy no: " << lv->GetDaughter(j)->GetCopyNo()
             << "  material:  " << lv->GetDaughter(j)->GetLogicalVolume()->GetMaterial()->GetName()
             << "  " << addressd
             << G4endl;

    }
  }
}

//_____________________________________________________________________________
void
TG4GeometryServices::PrintPhysicalVolumeStore() const
{
/// Print all physical volumes

  G4PhysicalVolumeStore* pvStore = G4PhysicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(pvStore->size()); i++) {
    G4VPhysicalVolume* pv = (*pvStore)[i];
    G4cout << i << "th volume name="
               << pv->GetName() << "  g3name="
               << UserVolumeName(pv->GetName()) << "  copyNo="
               << pv->GetCopyNo()
               << G4endl;
  }
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintElementTable() const
{
/// Print the G4 element table.

  const G4ElementTable* elementTable = G4Element::GetElementTable();

  G4cout << "Element table: " << G4endl;

  for (G4int i=0; i<G4int(elementTable->size()); i++) {

    G4Element* element = (*elementTable)[i];
    G4cout << "  " << std::setw(5)  << i
           << "th element:"
           << "  " << element
           << G4endl;
  }
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintMaterials() const
{
/// Print all materials

  // Dump materials
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  G4cout << *matTable;
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintMaterialsProperties() const
{
/// Print all material properties tables

  // Dump material properties tables
  // associated with materials
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  for (G4int i=0; i<G4int(matTable->size()); i++) {
    if ( (*matTable)[i] &&
         (*matTable)[i]->GetMaterialPropertiesTable() ) {

      G4cout << (*matTable)[i]->GetName()
             << " material properties table: " << G4endl;
#if G4VERSION_NUMBER >= 930
      DumpG4MaterialPropertiesTable((*matTable)[i]->GetMaterialPropertiesTable());
#else
      (*matTable)[i]->GetMaterialPropertiesTable()->DumpTable();
#endif
    }
  }

  // Dump material properties tables
  // associated with optical surfaces
  TG4OpSurfaceMap::const_iterator it;
  for ( it=fOpSurfaceMap->begin(); it!=fOpSurfaceMap->end(); it++) {
    if ( it->second &&
         it->second->GetMaterialPropertiesTable() ) {

      G4cout << it->first
             << " optical surface material properties table: " << G4endl;
#if G4VERSION_NUMBER >= 930
      DumpG4MaterialPropertiesTable(it->second->GetMaterialPropertiesTable());
#else
      it->second->GetMaterialPropertiesTable()->DumpTable();
#endif
    }
  }
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintMedia() const
{
/// Print all media from medium map

  fMediumMap->Print();
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintCuts(const G4String& cutName) const
{
/// Print the cut value for the cut with given cutName for all
/// tracking media

   TG4G3Cut cut = TG4G3CutVector::GetCut(cutName);
   if ( cut == kNoG3Cuts ) {
     TG4Globals::Exception("TG4GeometryServices", "PrintCuts",
       TString(cutName.c_str()) + " not defined.");
     return;
   }

   G4cout << "Cut " << cutName << G4endl;
   G4cout << "Medium ID" << "  " << "cutValue(MeV)" << G4endl;
   for ( G4int i=0; i< fMediumMap->GetNofMedia(); i++ ) {
     TG4Medium* medium = fMediumMap->GetMedium(i+1);
     if ( dynamic_cast<TG4Limits*>(medium->GetLimits()) ) {
       G4double cutValue = (*GetLimits(medium->GetLimits())->GetCutVector())[cut];
       G4cout << i+1 << "  " << cutValue << G4endl;
     }
     else {
       //G4cout << "Medium " << medium->GetName()
       //       << " has not TG4Limits " << G4endl;
     }
   }
}

//_____________________________________________________________________________
void TG4GeometryServices::PrintControls(const G4String& controlName) const
{
/// Print the control value for the control with given controlName for all
/// tracking media

   TG4G3Control control = TG4G3ControlVector::GetControl(controlName);
   if ( control == kNoG3Controls ) {
     TG4Globals::Exception("TG4GeometryServices", "PrintControls",
       TString(controlName.c_str()) + " not defined.");
     return;
   }

   G4cout << "Control " << controlName << G4endl;
   G4cout << "Medium ID" << "  " << "controlValue" << G4endl;
   for ( G4int i=0; i< fMediumMap->GetNofMedia(); i++ ) {

     TG4Medium* medium = fMediumMap->GetMedium(i+1);
     G4int controlValue = (*GetLimits(medium->GetLimits())->GetControlVector())[control];

     G4cout << i+1 << "  " << controlValue << G4endl;
   }
}

#ifdef USE_G3TOG4
//_____________________________________________________________________________
void TG4GeometryServices::SetG3toG4Separator(char separator)
{
/// Set the volumes name separator that will be
/// applied in both roottog4 and g3tog4

  gSeparator = separator;
}
#else
//_____________________________________________________________________________
void TG4GeometryServices::SetG3toG4Separator(char /*separator*/)
{
/// Set the volumes name separator that will be
/// applied in both roottog4 and g3tog4

}
#endif

//_____________________________________________________________________________
Int_t TG4GeometryServices::NofG3Volumes() const
{
/// Return the total number of logical volumes corresponding
/// to G3 volumes.
/// The logical volumes that were created by Gsposp method
/// with a generic name (name_copyNo) are NOT included.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  G4int counter = 0;
  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    if (IsG3Volume(lv->GetName())) counter++;
  }

  return counter;
}

//_____________________________________________________________________________
Int_t TG4GeometryServices::NofG4LogicalVolumes() const
{
/// Return the total number of logical volumes in the geometry.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
  return lvStore->size();
}

//_____________________________________________________________________________
Int_t TG4GeometryServices::NofG4PhysicalVolumes() const
{
/// Return the total number of physical volumes in the geometry.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  G4int counter = 0;
  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    counter += ((*lvStore)[i])->GetNoDaughters();
  }

  return counter;
}


//_____________________________________________________________________________
TG4Limits* TG4GeometryServices::GetLimits(G4UserLimits* limits) const
{
/// Check and convert the type of the given limits.

  if (!limits) return 0;

  TG4Limits* tg4Limits = dynamic_cast<TG4Limits*> (limits);
  if ( !tg4Limits ) {
    TG4Globals::Exception(
      "TG4GeometryServices", "GetLimits(.)", "Wrong limits type");
    return 0;
  }

  return tg4Limits;
}

//_____________________________________________________________________________
TG4Limits* TG4GeometryServices::GetLimits(
                                   G4UserLimits* limits,
                                   const TG4G3CutVector& cuts,
                                   const TG4G3ControlVector& controls) const
{
/// Check and convert the type of the given limits;
/// create TG4Limits object if it does not yet exist

  if (!limits) return 0;

  TG4Limits* tg4Limits = dynamic_cast<TG4Limits*> (limits);

  if (tg4Limits) return tg4Limits;

  G4UserLimits* g4Limits = dynamic_cast<G4UserLimits*> (limits);

  if (g4Limits) {
    tg4Limits = new TG4Limits(*limits, cuts, controls);
    delete limits;
    return tg4Limits;
  }

  TG4Globals::Exception(
    "TG4GeometryServices", "GetLimits(..)", "Wrong limits type.");
  return 0;
}

//_____________________________________________________________________________
G4LogicalVolume*
TG4GeometryServices::FindLogicalVolume(const G4String& name, G4bool silent) const
{
/// Find a logical volume with the specified name in G4LogicalVolumeStore.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    if (lv->GetName() == name) return lv;
  }

  if ( ! silent ) {
    TG4Globals::Warning(
      "TG4GeometryServices", "FindLogicalVolume",
      "Logical volume " + TString(name) + " not found.");
  }
  return 0;
}

//_____________________________________________________________________________
G4VPhysicalVolume*
TG4GeometryServices::FindPhysicalVolume(const G4String& name, G4int copyNo,
                                        G4bool silent) const
{
/// Find a physical volume with the specified name and copyNo in
/// G4PhysicalVolumeStore.

  G4PhysicalVolumeStore* pvStore = G4PhysicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(pvStore->size()); i++) {
    G4VPhysicalVolume* pv = (*pvStore)[i];
    //G4cout << i << "th volume "
    //           << pv->GetName() << "  "
    //           << UserVolumeName(pv->GetName()) << "  "
    //           << pv->GetCopyNo()
    //           << G4endl;
    if ( UserVolumeName(pv->GetName()) == name &&
         pv->GetCopyNo() == copyNo ) return pv;
  }

  if ( ! silent ) {
    TG4Globals::Warning(
      "TG4GeometryServices", "FindPhysicalVolume",
      "Physical volume " + TString(name) + " not found.");
  }
  return 0;
}

//_____________________________________________________________________________
G4VPhysicalVolume*
TG4GeometryServices::FindDaughter(const G4String& name, G4int copyNo,
                                  G4LogicalVolume* mlv, G4bool silent) const
{
/// Find daughter specified by name and copyNo in the given
/// mother logical volume

  for (G4int i=0; i<mlv->GetNoDaughters(); i++) {
    G4VPhysicalVolume* dpv = mlv->GetDaughter(i);
    if ( UserVolumeName(dpv->GetName()) == name &&
         dpv->GetCopyNo() == copyNo ) return dpv;
  }

  if ( ! silent ) {
    TG4Globals::Warning(
      "TG4GeometryServices", "FindDaughter",
      "Physical volume " + TString(name) + " not found.");
  }
  return 0;
}

//_____________________________________________________________________________
TG4Limits*
TG4GeometryServices::FindLimits(const G4String& name, G4bool silent) const
{
/// Find limits with the specified name.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    TG4Limits* limits = GetLimits(lv->GetUserLimits());
    if (limits && limits->GetName() == name) return limits;
  }

  if (!silent) {
    TG4Globals::Warning(
      "TG4GeometryServices", "FindLimits",
      "Limits " + TString(name) + " not found.");
  }
  return 0;
}

//_____________________________________________________________________________
TG4Limits*
TG4GeometryServices::FindLimits2(const G4String& name, G4bool silent) const
{
/// Find limits with the specified name.
/// Do not give an exception when limits of G4UserLimits are processed

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
    if ( ! dynamic_cast<TG4Limits*>(lv->GetUserLimits()) ) {
      continue;
    }
    TG4Limits* limits = GetLimits(lv->GetUserLimits());
    if (limits && limits->GetName() == name) return limits;
  }

  if (!silent) {
    TG4Globals::Warning(
      "TG4GeometryServices", "FindLimits",
      "Limits " + TString(name) + " not found.");
  }
  return 0;
}

//_____________________________________________________________________________
TG4Limits*
TG4GeometryServices::FindLimits(const G4Material* material, G4bool silent) const
{
/// Find limits with the specified name.

  // Get medium
  TG4Medium* medium = fMediumMap->GetMedium(material, !silent);
  if ( ! medium ) return 0;

  return FindLimits(medium->GetName(), silent);
}

//_____________________________________________________________________________
G4int TG4GeometryServices::GetMediumId(G4LogicalVolume* lv) const
{
/// Return the second index for materials (having its origin in
/// G4 tracking media concept)

  TG4Medium* medium = fMediumMap->GetMedium(lv, false);

  if ( ! medium ) return 0;

  return medium->GetID();
}

//_____________________________________________________________________________
G4double TG4GeometryServices::GetEffA(G4Material* material) const
{
/// Return A or the effective A=sum(pi*Ai) (if compound/mixture)
/// of the given material.

  G4double a = 0.;
  G4int nofElements = material->GetNumberOfElements();
  if (nofElements > 1) {
    //TG4Globals::Warning(
    //  "TG4GeometryServices", "GetEffA",
    //  "Effective A for material mixture (" + TString(material->GetName()) +
    //  ") is used.");

    for (G4int i=0; i<nofElements; i++) {
      G4double aOfElement = material->GetElement(i)->GetA();
      G4double massFraction = material->GetFractionVector()[i];
      a += aOfElement*massFraction /(TG4G3Units::AtomicWeight());
    }
  }
  else {
    a = material->GetA();
    a /= TG4G3Units::AtomicWeight();
  }
  return a;
}

//_____________________________________________________________________________
G4double TG4GeometryServices::GetEffZ(G4Material* material) const
{
/// Return Z or the effective Z=sum(pi*Zi) (if compound/mixture)
/// of the given material.

  G4double z = 0.;
  G4int nofElements = material->GetNumberOfElements();
  if (nofElements > 1) {
    //TG4Globals::Warning(
    //  "TG4GeometryServices", "GetEffZ",
    //  "Effective Z for material mixture (" + TString(material->GetName()) +
    //  ") is used.");

    for (G4int i=0; i<nofElements; i++) {
      G4double zOfElement = material->GetElement(i)->GetZ();
      G4double massFraction = material->GetFractionVector()[i];
      z += zOfElement*massFraction;
    }
  }
  else {
    z = material->GetZ();
  }
  return z;
}

//_____________________________________________________________________________
G4Material* TG4GeometryServices::FindMaterial(G4double a, G4double z,
                                             G4double density) const
{
/// Find the material in G4MaterialTable with specified parameters,
/// return 0 if not found.

  const G4MaterialTable* kpMatTable = G4Material::GetMaterialTable();

  for (G4int i=0; i<G4int(G4Material::GetNumberOfMaterials()); i++) {

    G4Material* material = (*kpMatTable)[i];

    if (CompareElement(a, z, material->GetElement(0)) &&
        CompareMaterial(1, density, material))

        return material;
  }

  return 0;
}

//_____________________________________________________________________________
G4Material* TG4GeometryServices::FindMaterial(G4double* a, G4double* z,
                                              G4double density,
                                              G4int nmat, G4double* wmat) const
{
/// Find the material in G4MaterialTable with specified parameters,
/// return 0 if not found.

  G4double* weight = ConvertAtomWeight(nmat, a, wmat);

  // loop over materials
  G4Material* found = 0;
  for (G4int i=0; i<G4int(G4Material::GetNumberOfMaterials()); i++) {

    G4Material* material = (*G4Material::GetMaterialTable())[i];
    G4int nofElements = material->GetNumberOfElements();

    if (CompareMaterial(nofElements, density, material)) {

      // loop over elements
      G4bool equal = true;
      for (G4int ie=0; ie<nofElements; ie++) {

        G4double we = (material->GetFractionVector())[ie];

        if ( !CompareElement(a[ie], z[ie], material->GetElement(ie)) ||
             std::abs(weight[ie] - we) > fgkAZTolerance ) {

          equal = false;
          break;
        }
      }
      if (equal) {
        found = material;
        break;
      }
    }
  }

  delete [] weight;
  return found;
}
