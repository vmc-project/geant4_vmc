// $Id: TG4GeometryManager.cxx,v 1.15 2005/11/18 21:29:35 brun Exp $
// Category: geometry
//
// Class TG4GeometryManager
// ------------------------
// See the class description in the header file.
// C++ interface to Geant3 basic routines for building Geant4 geometry
// by V. Berejnoi, 25.2.1999;
// materials, tracking media support 
// added by I.Hrivnacova, 27.5.1999.
//
// Author: V. Berejnoi, I. Hrivnacova

#include "TG4GeometryManager.h"
#include "TG4RootGeometryManager.h"
#include "TG4GeometryOutputManager.h"
#include "TG4GeometryServices.h"
#include "TG4Limits.h"
#include "TG4G3Units.h"
#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#include <G3toG4.hh> 
#include <G3toG4MANY.hh>
#include <G3toG4BuildTree.hh>
#include <G3VolTable.hh>
#include <G3RotTable.hh>
#include <G3EleTable.hh>
#include <G3MatTable.hh>
#include <G3MedTable.hh>
#include <G3SensVolVector.hh>

#include <G4LogicalVolumeStore.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4OpticalSurface.hh>
#include <G4Element.hh> 
#include <G4ReflectionFactory.hh>
#include <G4ReflectedSolid.hh>
#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4EllipticalTube.hh>
#include <G4Hype.hh>
#include <G4Para.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4Sphere.hh>
#include <G4Torus.hh>
#include <G4Trap.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4TwistedTrap.hh>
#include <G4Transform3D.hh>

#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TArrayD.h>
#include <TString.h>
#include <Riostream.h>

#include <math.h>

// extern global method from g3tog4
void G3CLRead(G4String &, char *);

TG4GeometryManager* TG4GeometryManager::fgInstance = 0;
const G4double      TG4GeometryManager::fgLimitDensity = 0.001*(g/cm3);
const G4double      TG4GeometryManager::fgMaxStep = 10*cm;

//_____________________________________________________________________________
TG4GeometryManager::TG4GeometryManager() 
  : TG4Verbose("geometryManager"),
    fMediumCounter(0),
    fMaterialCounter(0),
    fMatrixCounter(0),
    fWriteGeometry(false),
    fVMCGeometry(true)
{
//
  if (fgInstance) {
    TG4Globals::Exception(
      "TG4GeometryManager: attempt to create two instances of singleton.");
  }

  fOutputManager 
    = new TG4GeometryOutputManager();

  fGeometryServices 
    = new TG4GeometryServices(&fMediumMap, &fNameMap, &fOpSurfaceMap);

  fgInstance = this;
      
  // instantiate the default element table
  //TG4ElementTable::Instance();
}

//_____________________________________________________________________________
TG4GeometryManager::TG4GeometryManager(const TG4GeometryManager& right)
  : TG4Verbose("geometryManager") {
// 
  TG4Globals::Exception(
    "Attempt to copy TG4GeometryManager singleton.");
}


//_____________________________________________________________________________
TG4GeometryManager::~TG4GeometryManager() {
//
  delete fOutputManager;
  delete fGeometryServices;
}

//
// operators
//

//_____________________________________________________________________________
TG4GeometryManager& 
TG4GeometryManager::operator=(const TG4GeometryManager& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception(
    "Attempt to assign TG4GeometryManager singleton.");
    
  return *this;  
}    
          
//
// private methods
//
 
//_____________________________________________________________________________
void TG4GeometryManager::FillMediumMap()
{
/// Map G3 tracking medium IDs to volumes names.


  static G4int done = 0;
  
  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=done; i<G4int(lvStore->size()); i++) {
    G4String name  = ((*lvStore)[i])->GetName();
    
    G4String g3Name(name);
    
    // Filter out the reflected volumesname extension
    // added by reflection factory 
    G4String ext = G4ReflectionFactory::Instance()->GetVolumesNameExtension();
    if (name.find(ext)) g3Name = g3Name.substr(0, g3Name.find(ext));

    G4int mediumID = G3Vol.GetVTE(g3Name)->GetNmed();
    fMediumMap.Add(name, mediumID);     
  }
  
  done = lvStore->size();
}    

//
// public methods - TVirtualMC implementation
//

//_____________________________________________________________________________
Bool_t TG4GeometryManager::IsRootGeometrySupported() const
{
/// Returns info about supporting geometry defined via Root

#ifdef USE_VGM
  return true;
#else
  return false;
#endif    
}  

//_____________________________________________________________________________
void TG4GeometryManager::Material(Int_t& kmat, const char* name, Double_t a, 
          Double_t z, Double_t dens, Double_t radl, Double_t absl, Float_t* buf, 
          Int_t nwbuf)
{
/// Create material.                                                         \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate.

    G4double* bufin = fGeometryServices->CreateG4doubleArray(buf, nwbuf); 
    Material(kmat, name, a, z, dens, radl, absl, bufin, nwbuf);
    delete [] bufin;
}
  
 
//_____________________________________________________________________________
void TG4GeometryManager::Material(Int_t& kmat, const char* name, Double_t a, 
          Double_t z, Double_t dens, Double_t radl, Double_t absl, Double_t* buf, 
          Int_t nwbuf)
{
/// Create material.                                                         \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate

    kmat = ++fMaterialCounter;
    G4String namein = fGeometryServices->CutMaterialName(name);

    // write token to the output file
    if (fWriteGeometry) 
      fOutputManager->WriteGsmate(kmat, namein, a, z, dens, radl, nwbuf, buf); 

    // create new material only if it does not yet exist
    G4Material* material = fGeometryServices->FindMaterial(a, z, dens); 
    if (material) {
      // verbose
      if (VerboseLevel() > 1) {
        G4cout << "!!! Material " << namein << " already exists as "
               << material->GetName() << G4endl;
      }	       
      G3Mat.put(kmat, material);	    
    }  	       
    else
      G4gsmate(kmat, namein, a, z, dens, radl, nwbuf, buf); 
      
    // save the original material name
    fMaterialNameVector.push_back(namein);  

    if (nwbuf > 0) {  
      G4String text 
        = "TG4GeometryManager: user defined parameters for material ";
      text = text + namein;
      text = text + " are ignored by Geant4.";	
      TG4Globals::Warning(text);
    }
}
  
 
//_____________________________________________________________________________
void TG4GeometryManager::Mixture(Int_t& kmat, const char *name, Float_t *a, 
          Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat)
{ 
/// Create material composed of more elements.                            \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate

   G4double* ain = fGeometryServices->CreateG4doubleArray(a, abs(nlmat)); 
   G4double* zin = fGeometryServices->CreateG4doubleArray(z, abs(nlmat)); 
   G4double* wmatin = fGeometryServices->CreateG4doubleArray(wmat, abs(nlmat)); 

   Mixture(kmat, name, ain, zin, dens, nlmat, wmatin);

   // !!! in Geant3:
   // After a call with ratios by number (negative number of elements), 
   // the ratio array is changed to the ratio by weight, so all successive 
   // calls with the same array must specify the number of elements as 
   // positive
   
   // wmatin may be modified
   for (G4int i=0; i<abs(nlmat); i++) wmat[i] = wmatin[i]; 

   delete [] ain;
   delete [] zin;
   delete [] wmatin;
} 

//_____________________________________________________________________________
void TG4GeometryManager::Mixture(Int_t& kmat, const char *name, Double_t* a, 
          Double_t* z, Double_t dens, Int_t nlmat, Double_t* wmat)
{ 
/// Create material composed of more elements.                            \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate

   G4String namein = fGeometryServices->CutMaterialName(name);

   kmat = ++fMaterialCounter;

   // write token to the output file
   if (fWriteGeometry) 
     fOutputManager->WriteGsmixt(kmat, namein, a, z, dens, nlmat, wmat);

   // create new material only if it does not yet exist
   G4Material* material 
     = fGeometryServices->FindMaterial(a, z, dens, nlmat, wmat); 
   if (material) {
     // verbose
     if (VerboseLevel() > 1) {
       G4cout << "!!! Material " << namein << " already exists as "
              << material->GetName() << G4endl;
     }	      
     G3Mat.put(kmat, material);	    
   }  
   else 
     G4gsmixt(kmat, namein, a, z, dens, nlmat, wmat);
     
    // save the original material name
    fMaterialNameVector.push_back(namein);  
} 

//_____________________________________________________________________________
void TG4GeometryManager::Medium(Int_t& kmed, const char *name, Int_t nmat, 
          Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
          Double_t stemax, Double_t deemax, Double_t epsil, 
          Double_t stmin, Float_t* ubuf, Int_t nbuf)
{ 
/// Create a temporary "medium" that is used for 
/// assigning corresponding parameters to G4 objects:
/// - NTMED is stored as a second material index;
/// - ISVOL is used for builing G3SensVolVector;
/// - STEMAX is passed in G4UserLimits
/// - !! The other parameters (IFIELD, FIELDM, TMAXFD, DEEMAX, EPSIL, STMIN)
/// are ignored by Geant4.
///
///  Geant3 desription:                                                      \n
///  ==================
///  - NTMED  Tracking medium number
///  - NAME   Tracking medium name
///  - NMAT   Material number
///  - ISVOL  Sensitive volume flag
///  - IFIELD Magnetic field
///  - FIELDM Max. field value (Kilogauss)
///  - TMAXFD Max. angle due to field (deg/step)
///  - STEMAX Max. step allowed
///  - DEEMAX Max. fraction of energy lost in a step
///  - EPSIL  Tracking precision (cm)
///  - STMIN  Min. step due to continuos processes (cm)
///
///  - IFIELD = 0 if no magnetic field; 
///  - IFIELD = -1 if user decision in GUSWIM;
///  - IFIELD = 1 if tracking performed with GRKUTA; 
///  - IFIELD = 2 if tracking performed with GHELIX; 
///  - IFIELD = 3 if tracking performed with GHELX3.  

  G4double* bufin = fGeometryServices->CreateG4doubleArray(ubuf, nbuf); 
  Medium(kmed, name, nmat, isvol, ifield, fieldm, tmaxfd, stemax, deemax, epsil, 
         stmin, bufin, nbuf);
  delete [] bufin;	 
} 


//_____________________________________________________________________________
void TG4GeometryManager::Medium(Int_t& kmed, const char *name, Int_t nmat, 
          Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
          Double_t stemax, Double_t deemax, Double_t epsil, 
          Double_t stmin, Double_t* ubuf, Int_t nbuf)
{ 
/// Create a temporary "medium" that is used for 
/// assigning corresponding parameters to G4 objects:
/// - NTMED is stored as a second material index;
/// - ISVOL is used for builing G3SensVolVector;
/// - STEMAX is passed in G4UserLimits
/// - !! The other parameters (IFIELD, FIELDM, TMAXFD, DEEMAX, EPSIL, STMIN)
/// are ignored by Geant4.
///
///  Geant3 desription:                                                      \n
///  ==================
///  - NTMED  Tracking medium number
///  - NAME   Tracking medium name
///  - NMAT   Material number
///  - ISVOL  Sensitive volume flag
///  - IFIELD Magnetic field
///  - FIELDM Max. field value (Kilogauss)
///  - TMAXFD Max. angle due to field (deg/step)
///  - STEMAX Max. step allowed
///  - DEEMAX Max. fraction of energy lost in a step
///  - EPSIL  Tracking precision (cm)
///  - STMIN  Min. step due to continuos processes (cm)
///
///  - IFIELD = 0 if no magnetic field; 
///  - IFIELD = -1 if user decision in GUSWIM;
///  - IFIELD = 1 if tracking performed with GRKUTA; 
///  - IFIELD = 2 if tracking performed with GHELIX; 
///  - IFIELD = 3 if tracking performed with GHELX3.  

  G4String namein = fGeometryServices->CutMaterialName(name);

  kmed = ++fMediumCounter;

  // write token to the output file
  if (fWriteGeometry) 
    fOutputManager->WriteGstmed(kmed, name, nmat, isvol, ifield, fieldm, tmaxfd, 
        stemax, deemax, epsil, stmin, 0, 0);

  G4gstmed(kmed, name, nmat, isvol, ifield, fieldm, tmaxfd, stemax, deemax, 
       epsil, stmin, 0, stemax > 0.);
     // instead of the nbuf argument the bool is passed
     // in order to pass stemax into G4UserLimits

  // generate new unique name  
  G4String newName 
    = fGeometryServices
        ->GenerateLimitsName(kmed, namein, fMaterialNameVector[nmat-1]);
  fMediumNameVector.push_back(newName);
  
  if (nbuf > 0) {  
    G4String medName = name;
    G4String text
      = "TG4GeometryManager: user defined parameters for medium ";
    text = text + medName;
    text = text + " are ignored by Geant4.";  
    TG4Globals::Warning(text);
  }
} 


//_____________________________________________________________________________
void TG4GeometryManager::Matrix(Int_t& krot, Double_t thetaX, Double_t phiX, 
           Double_t thetaY, Double_t phiY, Double_t thetaZ, Double_t phiZ)
{
/// Create rotation matrix.

  krot = ++fMatrixCounter;

  // write token to the output file
  if (fWriteGeometry) 
    fOutputManager->WriteGsrotm(krot, thetaX, phiX, thetaY, phiY, thetaZ, phiZ);

  G4gsrotm(krot, thetaX, phiX, thetaY, phiY, thetaZ, phiZ);
}
  

//_____________________________________________________________________________
void TG4GeometryManager::Ggclos() 
{
/// Set the top VTE in temporary G3 volume table.
/// Close geometry output file (if fWriteGeometry is set true).
///
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  close out the geometry

  if (fWriteGeometry) fOutputManager->WriteGgclos();

  G4ggclos();        
} 
 

//_____________________________________________________________________________
void TG4GeometryManager::Gfmate(Int_t imat, char *name, Float_t &a, 
          Float_t &z, Float_t &dens, Float_t &radl, Float_t &absl,
          Float_t* ubuf, Int_t& nbuf) 
{ 
/// Return parameters for material specified by material number imat 

  G4double da, dz, ddens, dradl, dabsl;  
  Gfmate(imat, name, da, dz, ddens, dradl, dabsl, 0, nbuf);
  
  a = da;
  z = dz;
  dens = ddens;
  radl = dradl;
  absl = dabsl; 
} 

 
//_____________________________________________________________________________
void TG4GeometryManager::Gfmate(Int_t imat, char *name, Double_t &a, 
          Double_t &z, Double_t &dens, Double_t &radl, Double_t &absl,
          Double_t* ubuf, Int_t& nbuf) 
{ 
///  Return parameters for material specified by material number imat 

  G4Material* material = G3Mat.get(imat);
  
  if (material) {
    // to do: change this correctly 
    // !! unsafe conversion
    const char* chName = material->GetName();
    name = (char*)chName;
    a = fGeometryServices->GetEffA(material);
    z = fGeometryServices->GetEffZ(material);
    
    dens = material->GetDensity();
    dens /= TG4G3Units::MassDensity();

    radl = material->GetRadlen();
    radl /= TG4G3Units::Length();

    // the following parameters are not defined in Geant4
    absl = 0.; 
    ubuf = 0;
    nbuf = 0;
  }
  else {
    TG4Globals::Exception(
     "TG4GeometryManager::Gfmate: material has not been found.");
  }
} 

 
//_____________________________________________________________________________
void  TG4GeometryManager::Gstpar(Int_t itmed, const char *param, 
           Double_t parval) 
{ 
/// Write token to the output file only,
/// the method is performed by TG4PhysicsManager.

  if (fWriteGeometry) 
    fOutputManager->WriteGstpar(itmed, param, parval); 
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::SetCerenkov(Int_t itmed, Int_t npckov, 
                             Float_t* ppckov, Float_t* absco, Float_t* effic, 
			     Float_t* rindex)
{
///
///  Geant3 desription:                                                     \n
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
  G4double* abscoDbl  = fGeometryServices->CreateG4doubleArray(absco, npckov); 
  G4double* efficDbl  = fGeometryServices->CreateG4doubleArray(effic, npckov); 
  G4double* rindexDbl = fGeometryServices->CreateG4doubleArray(rindex, npckov); 
  
  SetCerenkov(itmed, npckov, ppckovDbl, abscoDbl, efficDbl, rindexDbl);

  delete ppckovDbl;
  delete abscoDbl;
  delete efficDbl;
  delete rindexDbl;	 
}			 

//_____________________________________________________________________________
void  TG4GeometryManager::SetCerenkov(Int_t itmed, Int_t npckov, 
                             Double_t* ppckov, Double_t* absco, Double_t* effic, 
			     Double_t* rindex)
{
///
///  Geant3 desription:                                                     \n
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

  // add units
  // add units
  G4double* ppckov2  = fGeometryServices->CreateG4doubleArray(ppckov, npckov); 
  G4double* absco2  = fGeometryServices->CreateG4doubleArray(absco, npckov); 
  G4int i;
  for (i=0; i<npckov; i++) {
    ppckov2[i] *= TG4G3Units::Energy();
    absco2[i]  *= TG4G3Units::Length();
  }  

  // get material of medium from table
  G3MedTableEntry* medium = G3Med.get(itmed);
  if (!medium) {
    G4String text = "TG4GeometryManager::SetCerenkov: \n";
    text = text + "    Medium not found."; 
    TG4Globals::Exception(text);
  }  
  G4Material* material = medium->GetMaterial();
  
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    material->SetMaterialPropertiesTable(table);
  }  

  // add material properties into the table 
  table->AddProperty("ABSLENGTH", ppckov2, absco2, npckov);
                    // used in G4OpAbsorption process
  table->AddProperty("EFFICIENCY", ppckov2, effic, npckov);
                    // used in G4OpBoundary process
  table->AddProperty("RINDEX", ppckov2, rindex, npckov);
                    // used in G4Cerenkov, G4OpRayleigh, G4OpBoundary

  delete [] ppckov2;	   
  delete [] absco2;	   

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The tables for UV photon tracking set for "
           << material->GetName() << G4endl;
  }	   
}			 


//_____________________________________________________________________________
void TG4GeometryManager::DefineOpSurface(const char *name,
                               EMCOpSurfaceModel model,
			       EMCOpSurfaceType surfaceType,
			       EMCOpSurfaceFinish surfaceFinish,
			       Double_t sigmaAlpha)
{
/// Define the optical surface

  G4OpticalSurface* surface = new G4OpticalSurface(name);
  surface->SetModel(fGeometryServices->SurfaceModel(model));
  surface->SetType(fGeometryServices->SurfaceType(surfaceType));
  surface->SetFinish(fGeometryServices->SurfaceFinish(surfaceFinish));
  surface->SetSigmaAlpha(sigmaAlpha);
  
  // Store the surface in the map
  fOpSurfaceMap[name] = surface;
}			       
			       
//_____________________________________________________________________________
void TG4GeometryManager::SetBorderSurface(const char *name,
                               const char* vol1Name, int vol1CopyNo,
                               const char* vol2Name, int vol2CopyNo,
			       const char* opSurfaceName)
{
/// Define the optical border surface

  // Get physical volumes
  G4VPhysicalVolume* pv1
    = fGeometryServices->FindPhysicalVolume( vol1Name, vol1CopyNo, false); 
  G4VPhysicalVolume* pv2
    = fGeometryServices->FindPhysicalVolume( vol2Name, vol2CopyNo, false); 

  if (!pv1 || !pv2) {
    G4String text = "TG4GeometryManager::SetBorderSurface: \n";
    text = text + "    Cannot find physical volume: ";
    if (!pv1) text = text + G4String(vol1Name) + G4String(" ");
    text = text + "    Cannot find physical volume: ";
    if (!pv2) text = text + G4String(vol2Name);
    TG4Globals::Exception(text);
    return;
  }  
  
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap.find(opSurfaceName);
  if (it == fOpSurfaceMap.end()) {
    G4String text = "TG4GeometryManager::SetBorderSurface: \n";
    text = text + "    Cannot find optical surfaace: ";
    text = text + G4String(opSurfaceName);
    TG4Globals::Exception(text);
    return;
  }  
  G4OpticalSurface* surface = (*it).second;
  
  // Create the border surface
  new G4LogicalBorderSurface(name, pv1, pv2, surface);
}
			       
//_____________________________________________________________________________
void TG4GeometryManager::SetSkinSurface(const char *name,
                         const char* volName,
			 const char* opSurfaceName)
{
/// Define the optical skin surface

  // Get logical volume
  G4LogicalVolume* lv
    = fGeometryServices->FindLogicalVolume(volName, false); 

  if (!lv ) {
    G4String text = "TG4GeometryManager::SetSkinSurface: \n";
    text = text + "    Cannot find logical volume: ";
    text = text + G4String(volName);
    TG4Globals::Exception(text);
    return;
  }  
  
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap.find(opSurfaceName);
  if (it == fOpSurfaceMap.end()) {
    G4String text = "TG4GeometryManager::SetBorderSurface: \n";
    text = text + "    Cannot find optical surfaace: ";
    text = text + G4String(opSurfaceName);
    TG4Globals::Exception(text);
    return;
  }  
  G4OpticalSurface* surface = (*it).second;
 
  // Create the skin surface
  new G4LogicalSkinSurface(name, lv, surface);
}
			 
//_____________________________________________________________________________
void TG4GeometryManager::SetMaterialProperty(
                            Int_t itmed, const char* propertyName, 
			    Int_t np, Double_t* pp, Double_t* values)
{
/// Set the material property specified by propertyName to the tracking medium

  // create material properties table
  // get material of medium from table
  G3MedTableEntry* medium = G3Med.get(itmed);
  if (!medium) {
    G4String text = "TG4GeometryManager::SetMaterialProperty: \n";
    text = text + "    Medium not found."; 
    TG4Globals::Exception(text);
  }  
  G4Material* material = medium->GetMaterial();
  
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    material->SetMaterialPropertiesTable(table);
  }  

  // add units
  G4double* pp2  = fGeometryServices->CreateG4doubleArray(pp, np); 
  G4double* val2 = fGeometryServices->CreateG4doubleArray(values, np); 
  G4int i;
  for (i=0; i<np; i++) {
    pp2[i] = pp2[i]*TG4G3Units::Energy();
    val2[i]  = values[i];
    if (G4String(propertyName) == "ABSLENGTH") 
      val2[i]  = val2[i]*TG4G3Units::Length();
  }  
  table->AddProperty(propertyName, pp2, val2, np);

  delete [] pp2;	   
  delete [] val2;	   

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material properties " << propertyName << " set for "
           << material->GetName() << G4endl;
  }  
}
			 
//_____________________________________________________________________________
void TG4GeometryManager::SetMaterialProperty(
                            Int_t itmed, const char* propertyName, 
			    Double_t value)
{
/// Set the material property specified by propertyName to the tracking medium

  // create material properties table
  // get material of medium from table
  G3MedTableEntry* medium = G3Med.get(itmed);
  if (!medium) {
    G4String text = "TG4GeometryManager::SetMaterialProperty: \n";
    text = text + "    Medium not found."; 
    TG4Globals::Exception(text);
  }  
  G4Material* material = medium->GetMaterial();
  
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = material->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    material->SetMaterialPropertiesTable(table);
  }  

  // add units
  if ( G4String(propertyName) == "SCINTILLATIONYIELD") {
       value  = value/TG4G3Units::Energy();
  }  
  if ( G4String(propertyName) == "FASTTIMECONSTANT" || 
       G4String(propertyName) == "SLOWTIMECONSTANT" ) { 
       value  = value*TG4G3Units::Time();
  }  
  table->AddConstProperty(propertyName, value);

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material property " << propertyName << " set for "
           << material->GetName() << G4endl;
  }	   
}
			 
//_____________________________________________________________________________
void TG4GeometryManager::SetMaterialProperty(
                            const char* surfaceName, const char* propertyName, 
			    Int_t np, Double_t* pp, Double_t* values)
{
/// Set the material property specified by propertyName to the optical surface

  // create material properties table
  // get optical surface from the map
  // Get the optical surface
  TG4OpSurfaceMap::iterator it = fOpSurfaceMap.find(surfaceName);
  if (it == fOpSurfaceMap.end()) {
    G4String text = "TG4GeometryManager::SetMaterialProperty: \n";
    text = text + "    Cannot find optical surfaace: ";
    text = text + G4String(surfaceName);
    TG4Globals::Exception(text);
    return;
  }  
  G4OpticalSurface* surface = (*it).second;
 
  // get materials properties table or create it if it does not yet
  // exist
  G4MaterialPropertiesTable* table
    = surface->GetMaterialPropertiesTable();
  if (!table) {
    table = new G4MaterialPropertiesTable(); 
    surface->SetMaterialPropertiesTable(table);
  }  

  // add units
  G4double* pp2  = fGeometryServices->CreateG4doubleArray(pp, np); 
  G4int i;
  for (i=0; i<np; i++) {
    pp2[i] = pp2[i]*TG4G3Units::Energy();
  }  
  table->AddProperty(propertyName, pp2, values, np);
 
  delete [] pp2;  

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "The material properties " << propertyName 
           << " set for optical surface "
           << surface->GetName() << G4endl;
  }	   
}			     
 
//_____________________________________________________________________________
Bool_t TG4GeometryManager::GetTransformation(const TString& volumePath, 
                              TGeoHMatrix& matrix)
{			 
/// Return the transformation matrix between the volume specified by
/// the path volumePath and the first volume in the path.

  G4String path = volumePath.Data();

  G4String volName;
  G4int copyNo; 
  path = fGeometryServices->CutVolumePath(path, volName, copyNo);     

  // Get the first volume
  G4VPhysicalVolume* pvTop = 0;
  if ( fGeometryServices->GetWorld()->GetName() == volName) {
    // Check world volume first
    pvTop = fGeometryServices->GetWorld();  
  }	
  else  
    pvTop = fGeometryServices->FindPhysicalVolume(volName, copyNo, true);
   
  if ( !pvTop ) {
    G4String text = "TG4GeometryManager::GetTransformation: \n";
    text = text + "    Top volume in " + volumePath + " does not exist.";
    TG4Globals::Warning(text);
    return false;
  }

  // Calculate transformation
  //
  G4VPhysicalVolume* pvMother = pvTop;
  G4Transform3D transform;
  
  while ( path.length() > 0 ) {
    // Extract next volume name & copyNo
    path = fGeometryServices->CutVolumePath(path, volName, copyNo);     
  
    // Find daughter
    G4VPhysicalVolume* pvDaughter 
      = fGeometryServices
        ->FindDaughter(volName, copyNo, pvMother->GetLogicalVolume(), true);
	
    if ( !pvDaughter ) {
      G4String text = "TG4GeometryManager: GetTransformation: \n";
      text = text + "    Daughter volume " + volName;
      text = text + " in " + volumePath + " does not exist.";
      TG4Globals::Warning(text);
      return false;
    }
    
    transform = transform * G4Transform3D(*pvDaughter->GetObjectRotation(),
                                            pvDaughter->GetObjectTranslation());
    pvMother = pvDaughter; 
  }   
  
  // put transform in TGeoHMatrix here
  fGeometryServices->Convert(transform, matrix);
  return true;
}			 
   
//_____________________________________________________________________________
Bool_t TG4GeometryManager::GetShape(const TString& volumePath, 
                              TString& shapeType, TArrayD& par)
{			 
/// Return the name of the shape and its parameters for the volume
/// specified by the volume name.

   // Get volume & copyNo   
   G4String path = volumePath.Data();
   G4int last1 = path.rfind('/');
   G4int last2 = path.rfind('_');
   G4String volName = path(last1+1, last2-last1-1 );
   G4String copyNoStr = path(last2+1, path.length()-last2 );
   istringstream in(copyNoStr);
   G4int copyNo;
   in >> copyNo;

   // Find physical volume
   G4VPhysicalVolume* pv
     = fGeometryServices->FindPhysicalVolume(volName, copyNo, true);
   
   if (!pv) {
     G4String text = "TG4GeometryManager: GetShape: \n";
     text = text + "    Physical volume " + volumePath + " does not exist.";
     TG4Globals::Warning(text);
     return false;
   } 
     
   // Get solid
   G4VSolid* solid = pv->GetLogicalVolume()->GetSolid();
   
   // Get constituent solid if reflected solid
   Bool_t isReflected = false;
   G4ReflectedSolid* reflSolid = dynamic_cast<G4ReflectedSolid*>(solid);
   if ( reflSolid ) {
     solid = reflSolid->GetConstituentMovedSolid();
     isReflected = true;
   }  
   
   Int_t npar;

   if ( solid->GetEntityType() == "G4Box" ) {
      shapeType = "BOX";
      npar = 3;
      par.Set(npar);
      G4Box *box = (G4Box*)solid;
      par.AddAt(box->GetXHalfLength()/cm, 0);
      par.AddAt(box->GetYHalfLength()/cm, 1);
      par.AddAt(box->GetZHalfLength()/cm, 2);
      return kTRUE;
   }
      
   if ( solid->GetEntityType() == "G4Cons" ) {
      shapeType = "CONS";
      npar = 7;
      par.Set(npar);
      G4Cons *cons = (G4Cons*)solid;
      par.AddAt(cons->GetZHalfLength()/cm,       0);
      if ( !isReflected) {
        par.AddAt(cons->GetInnerRadiusMinusZ()/cm, 1);
        par.AddAt(cons->GetOuterRadiusMinusZ()/cm, 2);
        par.AddAt(cons->GetInnerRadiusPlusZ()/cm,  3);
        par.AddAt(cons->GetOuterRadiusPlusZ()/cm,  4);
      }
      else {	
        par.AddAt(cons->GetInnerRadiusPlusZ()/cm, 1);
        par.AddAt(cons->GetOuterRadiusPlusZ()/cm, 2);
        par.AddAt(cons->GetInnerRadiusMinusZ()/cm,  3);
        par.AddAt(cons->GetOuterRadiusMinusZ()/cm,  4);
      }	
      par.AddAt(cons->GetStartPhiAngle()/deg,    5);
      par.AddAt(cons->GetDeltaPhiAngle()/deg,    6);
      return kTRUE;
   }   
   
   if ( solid->GetEntityType() == "G4EllipticalTube" ) {
      shapeType = "ELTU";
      npar = 3;
      par.Set(npar);
      G4EllipticalTube *eltu = (G4EllipticalTube*)solid;
      par.AddAt(eltu->GetDx()/cm, 0);
      par.AddAt(eltu->GetDy()/cm, 1);
      par.AddAt(eltu->GetDz()/cm, 2);
      return kTRUE;
   }   
   
   // Add to G3toG4, VGM
   if ( solid->GetEntityType() == "G4Hype" ) {
      shapeType = "HYPE";
      npar = 5;
      par.Set(npar);
      G4Hype *hype = (G4Hype*)solid;
      par.AddAt(hype->GetInnerRadius()/cm, 0);
      par.AddAt(hype->GetOuterRadius()/cm, 1);
      par.AddAt(hype->GetZHalfLength()/cm, 2);
      par.AddAt(hype->GetInnerStereo(), 3);    // ?? unit
      par.AddAt(hype->GetOuterStereo(), 4);    // ?? unit
      return kTRUE;
   }   

   // Add G4Orb   

   if ( solid->GetEntityType() == "G4Para" ) {
      shapeType = "PARA";
      npar = 6;
      par.Set(npar);
      G4Para *para = (G4Para*)solid;
      par.AddAt(para->GetXHalfLength()/cm, 0);
      par.AddAt(para->GetYHalfLength()/cm, 1);
      par.AddAt(para->GetZHalfLength()/cm, 2);
      par.AddAt(atan(para->GetTanAlpha())/deg,  3);
      if ( !isReflected) 
        par.AddAt(para->GetSymAxis().theta()/deg, 4);
      else
        par.AddAt((M_PI - para->GetSymAxis().theta())/deg, 4);
      par.AddAt(para->GetSymAxis().phi()/deg,   5);
      return kTRUE;
   }   
   
   if ( solid->GetEntityType() == "G4Polycone" ) {
      shapeType = "PCON";
      G4Polycone *pcon = (G4Polycone*)solid;
      Int_t nz = pcon->GetOriginalParameters()->Num_z_planes;
      const Double_t* rmin = pcon->GetOriginalParameters()->Rmin;
      const Double_t* rmax = pcon->GetOriginalParameters()->Rmax;
      const Double_t* z = pcon->GetOriginalParameters()->Z_values;
      npar = 3 + 3*nz;
      par.Set(npar);
      par.AddAt(pcon->GetStartPhi()/deg, 0);
      par.AddAt((pcon->GetEndPhi() - pcon->GetStartPhi())/deg, 1);
      par.AddAt(pcon->GetOriginalParameters()->Num_z_planes, 2);
      for (Int_t i=0; i<nz; i++) {
         if ( !isReflected ) 
           par.AddAt(z[i]/cm,    3+3*i);
	 else 
           par.AddAt(- z[i]/cm,    3+3*i);
         par.AddAt(rmin[i]/cm, 3+3*i+1);
         par.AddAt(rmax[i]/cm, 3+3*i+2);
      }   
      return kTRUE; 
   }   

   if ( solid->GetEntityType() == "G4Polyhedra" ) {
      shapeType = "PGON";
      G4Polyhedra *pgon = (G4Polyhedra*)solid;
      Int_t nz = pgon->GetOriginalParameters()->Num_z_planes;
      const Double_t* rmin = pgon->GetOriginalParameters()->Rmin;
      const Double_t* rmax = pgon->GetOriginalParameters()->Rmax;
      const Double_t* z = pgon->GetOriginalParameters()->Z_values;
      npar = 4 + 3*nz;
      par.Set(npar);
      par.AddAt(pgon->GetStartPhi()/deg,0);
      par.AddAt((pgon->GetEndPhi() - pgon->GetStartPhi())/deg, 1);
      par.AddAt(pgon->GetOriginalParameters()->numSide, 2);
      par.AddAt(pgon->GetOriginalParameters()->Num_z_planes, 3);
      for (Int_t i=0; i<nz; i++) {
         if ( !isReflected ) 
           par.AddAt(z[i]/cm,   4+3*i);
	 else 
           par.AddAt(- z[i]/cm, 4+3*i);
         par.AddAt(rmin[i]/cm, 4+3*i+1);
         par.AddAt(rmax[i]/cm, 4+3*i+2);
      }   
      return kTRUE; 
   }   

   if ( solid->GetEntityType() == "G4Sphere" ) {
      shapeType = "SPHE";
      npar = 6;
      par.Set(npar);
      G4Sphere *sphe = (G4Sphere*)solid;
      par.AddAt(sphe->GetInsideRadius()/cm,     0);
      par.AddAt(sphe->GetOuterRadius()/cm,      1);
      if ( !isReflected ) 
        par.AddAt(sphe->GetStartThetaAngle()/deg, 2);
      else	
        par.AddAt((M_PI - sphe->GetStartThetaAngle())/deg, 2);
      par.AddAt(sphe->GetStartPhiAngle()/deg,   3);
      par.AddAt(sphe->GetStartPhiAngle()/deg,   4);
      par.AddAt(sphe->GetDeltaPhiAngle()/deg,   5);
      return kTRUE;
   }   

   // Add G4Torus

   if ( solid->GetEntityType() == "G4Trap" ) {
      shapeType = "TRAP";
      npar = 11;
      par.Set(npar);
      G4Trap *trap = (G4Trap*)solid;
      par.AddAt(trap->GetZHalfLength()/cm,       0);
      if (!isReflected ) { 
        par.AddAt(trap->GetSymAxis().theta()/deg,  1);
        par.AddAt(trap->GetSymAxis().phi()/deg,    2);
        par.AddAt(trap->GetYHalfLength1()/cm,      3);
        par.AddAt(trap->GetXHalfLength1()/cm,      4);
        par.AddAt(trap->GetXHalfLength2()/cm,      5);
        par.AddAt(atan(trap->GetTanAlpha1())/deg,  6);
        par.AddAt(trap->GetYHalfLength2()/cm,      7);
        par.AddAt(trap->GetXHalfLength3()/cm,      8);
        par.AddAt(trap->GetXHalfLength4()/cm,      9);
      }	
      else {	
        par.AddAt(-(trap->GetSymAxis().theta())/deg, 1);
        par.AddAt(-(trap->GetSymAxis().phi())/deg,   2);
        par.AddAt(trap->GetYHalfLength2()/cm,      3);
        par.AddAt(trap->GetXHalfLength3()/cm,      4);
        par.AddAt(trap->GetXHalfLength4()/cm,      5);
        par.AddAt(atan(trap->GetTanAlpha2())/deg,  6);
        par.AddAt(trap->GetYHalfLength1()/cm,      7);
        par.AddAt(trap->GetXHalfLength1()/cm,      8);
        par.AddAt(trap->GetXHalfLength2()/cm,      9);
        par.AddAt(atan(trap->GetTanAlpha1())/deg, 10);
      }	
      return kTRUE;
   }   

   if ( solid->GetEntityType() == "G4Trd" ) {
      shapeType = "TRD2";
      npar = 5;
      par.Set(npar);
      G4Trd *trd2 = (G4Trd*)solid;
      if ( !isReflected ) {
        par.AddAt(trd2->GetXHalfLength1()/cm, 0);
        par.AddAt(trd2->GetXHalfLength2()/cm, 1);
        par.AddAt(trd2->GetYHalfLength1()/cm, 2);
        par.AddAt(trd2->GetYHalfLength2()/cm, 3);
      }
      else {	
        par.AddAt(trd2->GetXHalfLength2()/cm, 0);
        par.AddAt(trd2->GetXHalfLength1()/cm, 1);
        par.AddAt(trd2->GetYHalfLength2()/cm, 2);
        par.AddAt(trd2->GetYHalfLength1()/cm, 3);
      }
      par.AddAt(trd2->GetZHalfLength()/cm,  4);
      return kTRUE;
   }   

   if ( solid->GetEntityType() == "G4Tubs" ) {
      shapeType = "TUBS";
      npar = 5;
      par.Set(npar);
      G4Tubs *tubs = (G4Tubs*)solid;
      par.AddAt(tubs->GetInnerRadius()/cm,    0);
      par.AddAt(tubs->GetOuterRadius()/cm,    1);
      par.AddAt(tubs->GetZHalfLength()/cm,    2);
      par.AddAt(tubs->GetStartPhiAngle()/deg, 3);
      par.AddAt(tubs->GetDeltaPhiAngle()/deg, 4);
      return kTRUE;
   }   

   // To do: add to VGM
   if ( solid->GetEntityType() == "G4TwistedTrap" ) {
      shapeType = "GTRA";
      npar = 12;
      par.Set(npar);
      G4TwistedTrap* trap = (G4TwistedTrap*)solid;
      par.AddAt(trap->GetZHalfLength()/cm,          0);
      if ( !isReflected ) {
        par.AddAt(trap->GetPolarAngleTheta()/deg,   1);
        par.AddAt(trap->GetAzimuthalAnglePhi()/deg, 2);
        par.AddAt(trap->GetY1HalfLength()/cm,       3);
        par.AddAt(trap->GetX1HalfLength()/cm,       4);
        par.AddAt(trap->GetX2HalfLength()/cm,       5);
        par.AddAt(trap->GetTiltAngleAlpha()/deg,    6);
        par.AddAt(trap->GetY2HalfLength()/cm,       7);
        par.AddAt(trap->GetX3HalfLength()/cm,       8);
        par.AddAt(trap->GetX4HalfLength()/cm,       9);
        par.AddAt(trap->GetTiltAngleAlpha()/deg,   10);
      }
      else {
        par.AddAt(-trap->GetPolarAngleTheta()/deg,   1);
        par.AddAt(-trap->GetAzimuthalAnglePhi()/deg, 2);
        par.AddAt(trap->GetY2HalfLength()/cm,        3);
        par.AddAt(trap->GetX3HalfLength()/cm,        4);
        par.AddAt(trap->GetX4HalfLength()/cm,        5);
        par.AddAt(trap->GetTiltAngleAlpha()/deg,     6);
        par.AddAt(trap->GetY1HalfLength()/cm,        7);
        par.AddAt(trap->GetX1HalfLength()/cm,        8);
        par.AddAt(trap->GetX2HalfLength()/cm,        9);
        par.AddAt(trap->GetTiltAngleAlpha()/deg,    10);
      }
      par.AddAt(trap->GetPhiTwist()/deg,            11);
      return kTRUE;
  }   

  G4String text = "TG4GeometryManager: GetShape: ";
  text = text + "Shape " + solid->GetEntityType() + " not implemented.";
  TG4Globals::Warning(text);
  return false;
}  

//_____________________________________________________________________________
Bool_t TG4GeometryManager::GetMaterial(const TString& volumeName,
	 	              TString& name, Int_t& imat,
		              Double_t& a, Double_t& z, Double_t& density,
		              Double_t& radl, Double_t& inter, TArrayD& par)
{			 
/// Return the material parameters for the volume specified by
/// the volume name.

  // Get volume
  G4LogicalVolume* lv =
    fGeometryServices->FindLogicalVolume(volumeName.Data(), true);
  if ( !lv ) {
    G4String text = "TG4GeometryManager: GetMaterial: \n";
    text = text + "    Logical volume " + volumeName + " does not exist.";
    TG4Globals::Warning(text);
    return false;
  }  

  G4Material* material = lv->GetMaterial();
  imat = material->GetIndex();
  name = material->GetName();
  a = fGeometryServices->GetEffA(material);
  z = fGeometryServices->GetEffZ(material);
    
  density = material->GetDensity();
  density /= TG4G3Units::MassDensity();

  radl = material->GetRadlen();
  radl /= TG4G3Units::Length();

  // the following parameters are not defined in Geant4
  inter = 0.; 
  par.Set(0);
  return true;
}				        
		     
//_____________________________________________________________________________
Bool_t TG4GeometryManager::GetMedium(const TString& volumeName,
                              TString& name, Int_t& imed,
		              Int_t& nmat, Int_t& isvol, Int_t& ifield,
		              Double_t& fieldm, Double_t& tmaxfd, Double_t& stemax,
		              Double_t& deemax, Double_t& epsil, Double_t& stmin,
		              TArrayD& par)
{			 
// Returns the medium parameters for the volume specified by the
// volume name.

  // Get volume
  G4LogicalVolume* lv =
    fGeometryServices->FindLogicalVolume(volumeName.Data(), true);
  if ( !lv ) {
    G4String text = "TG4GeometryManager: GetMaterial: \n";
    text = text + "    Logical volume " + volumeName + " does not exist.";
    TG4Globals::Warning(text);
    return false;
  } 
   
  imed = fGeometryServices->GetMediumId(lv);
  nmat = lv->GetMaterial()->GetIndex();
  
  TG4Limits* limits = fGeometryServices->GetLimits(lv->GetUserLimits());
  if (limits) {
    name = limits->GetName();
    stemax = limits->GetMaxUserStep();
  }  
  else  
    stemax = 0.; 

  // the following parameters are not defined in Geant4
  isvol = 0;
  ifield = 0;
  fieldm = 0; 
  tmaxfd = 0.;
  deemax = 0.;
  epsil = 0.;
  stmin = 0.;
  par.Set(0);

  return true;
}				        

//_____________________________________________________________________________
Int_t TG4GeometryManager::Gsvolu(const char *name, const char *shape, 
          Int_t nmed, Double_t* upar, Int_t npar) 
{ 
///  Geant3 desription:                                                     \n
///  ==================                                                     \n
///  - NAME   Volume name
///  - SHAPE  Volume type
///  - NUMED  Tracking medium number
///  - NPAR   Number of shape parameters
///  - UPAR   Vector containing shape parameters
///
///  It creates a new volume in the JVOLUM data structure.
// ---  

  // write token to the output file
  if (fWriteGeometry) 
    fOutputManager->WriteGsvolu(name, shape, nmed, upar, npar);    

  G4gsvolu(fGeometryServices->CutName(name), 
           fGeometryServices->CutName(shape), nmed, upar, npar);
  
  // register name in name map
  fNameMap.AddName(fGeometryServices->CutName(name));

  return 0;
} 


//_____________________________________________________________________________
Int_t TG4GeometryManager::Gsvolu(const char *name, const char *shape, 
          Int_t nmed, Float_t *upar, Int_t npar) 
{ 
/// Single precision interface.

  //TG4Globals::Warning("TG4GeometryManager::Gsvolu in single precision.");

  G4double* parin = fGeometryServices->CreateG4doubleArray(upar, npar); 

  G4int result
   = Gsvolu(name, shape, nmed, parin, npar);

  delete [] parin;

  return result;
} 


//_____________________________________________________________________________
void  TG4GeometryManager::Gsdvn(const char *name, const char *mother, 
           Int_t ndiv, Int_t iaxis) 
{ 
///  Geant3 desription:                                                     \n
///  ==================
///  - NAME   Volume name
///  - MOTHER Mother volume name
///  - NDIV   Number of divisions
///  - IAXIS  Axis value
///
///  - X,Y,Z of CAXIS will be translated to 1,2,3 for IAXIS.                \n
///  It divides a previously defined volume.
//  ---

    // write token to the output file
    if (fWriteGeometry) 
      fOutputManager->WriteGsdvn(name, mother, ndiv, iaxis);

    G4gsdvn(fGeometryServices->CutName(name), 
            fGeometryServices->CutName(mother), ndiv, iaxis);

    // register name in name map
    fNameMap.AddName(fGeometryServices->CutName(name));
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsdvn2(const char *name, const char *mother, 
           Int_t ndiv, Int_t iaxis, Double_t c0i, Int_t numed) 
{ 
///  Geant3 desription:                                                       \n
///  ==================                                                       \n
///  Divides mother into ndiv divisions called name
///  along axis iaxis starting at coordinate value c0.
///  The new volume created will be medium number numed.

    // write token to the output file
    if (fWriteGeometry) 
      fOutputManager->WriteGsdvn2(name, mother, ndiv, iaxis, c0i, numed);

    G4gsdvn2(fGeometryServices->CutName(name),
             fGeometryServices->CutName(mother), ndiv, iaxis, c0i, numed);

    // register name in name map
    fNameMap.AddName(fGeometryServices->CutName(name));
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsdvt(const char *name, const char *mother, 
           Double_t step, Int_t iaxis, Int_t numed, Int_t ndvmx)
{ 
///  Geant3 desription:                                                       \n
///  ==================                                                       \n
///  Divides MOTHER into divisions called NAME along
///  axis IAXIS in steps of STEP. If not exactly divisible 
///  will make as many as possible and will centre them 
///  with respect to the mother. Divisions will have medium 
///  number NUMED. If NUMED is 0, NUMED of MOTHER is taken.
///  NDVMX is the expected maximum number of divisions
///  (If 0, no protection tests are performed) 

    // write token to the output file
    if (fWriteGeometry) 
      fOutputManager->WriteGsdvt(name, mother, step, iaxis, numed, ndvmx);

    G4gsdvt(fGeometryServices->CutName(name), 
            fGeometryServices->CutName(mother), step, iaxis, numed, ndvmx);

    // register name in name map
    fNameMap.AddName(fGeometryServices->CutName(name));
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsdvt2(const char *name, const char *mother, 
           Double_t step, Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx)
{ 
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  Create a new volume by dividing an existing one
///                                                                    
///  Divides MOTHER into divisions called NAME along          
///  axis IAXIS starting at coordinate value C0 with step    
///  size STEP.                                                              \n       
///  The new volume created will have medium number NUMED.    
///  If NUMED is 0, NUMED of mother is taken.                                \n               
///  NDVMX is the expected maximum number of divisions        
///  (If 0, no protection tests are performed)              

    // write token to the output file
    if (fWriteGeometry) 
      fOutputManager->WriteGsdvt2(name, mother, step, iaxis, c0, numed, ndvmx);

    G4gsdvt2(fGeometryServices->CutName(name), 
             fGeometryServices->CutName(mother), step, iaxis, c0, numed, ndvmx);

    // register name in name map
    fNameMap.AddName(fGeometryServices->CutName(name));
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsord(const char *name, Int_t iax) 
{ 
/// No corresponding action in G4.
///
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///    Flags volume CHNAME whose contents will have to be ordered 
///    along axis IAX, by setting the search flag to -IAX
///    -       IAX = 1    X axis 
///    -       IAX = 2    Y axis 
///    -       IAX = 3    Z axis 
///    -       IAX = 4    Rxy (static ordering only  -> GTMEDI)
///    -       IAX = 14   Rxy (also dynamic ordering -> GTNEXT)
///    -       IAX = 5    Rxyz (static ordering only -> GTMEDI)
///    -       IAX = 15   Rxyz (also dynamic ordering -> GTNEXT)
///    -       IAX = 6    PHI   (PHI=0 => X axis)
///    -       IAX = 7    THETA (THETA=0 => Z axis)

  TG4Globals::Warning("TG4GeometryManager::Gsord: dummy method.");
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gspos(const char *vname, Int_t num, 
          const char *vmoth, Double_t x, Double_t y, Double_t z, Int_t irot, 
          const char *vonly) 
{ 
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  Position a volume into an existing one
///
///  -  NAME   Volume name
///  -  NUMBER Copy number of the volume
///  -  MOTHER Mother volume name
///  -  X      X coord. of the volume in mother ref. sys.
///  -  Y      Y coord. of the volume in mother ref. sys.
///  -  Z      Z coord. of the volume in mother ref. sys.
///  -  IROT   Rotation matrix number w.r.t. mother ref. sys.
///  -  ONLY   ONLY/MANY flag
///
///  It positions a previously defined volume in the mother.
// ---  

   // write token to the output file
   if (fWriteGeometry) 
     fOutputManager->WriteGspos(vname, num, vmoth, x, y, z, irot, vonly);

   G4gspos(fGeometryServices->CutName(vname), ++num,
           fGeometryServices->CutName(vmoth), x, y, z, irot, vonly);

   // register name in name map
   fNameMap.AddName(fGeometryServices->CutName(vname));
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsposp(const char *name, Int_t nr, 
           const char *mother, Double_t x, Double_t y, Double_t z, Int_t irot, 
           const char *konly, Double_t* upar, Int_t np ) 
{ 
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  Place a copy of generic volume NAME with user number
///  NR inside MOTHER, with its parameters UPAR(1..NP)

   // write token to the output file
   if (fWriteGeometry) 
     fOutputManager->WriteGsposp(name, nr, mother, x, y, z, irot, konly, upar, np);

   G4gsposp(fGeometryServices->CutName(name), ++nr, 
            fGeometryServices->CutName(mother), x, y, z, irot, konly, 
             upar, np);

   // register name in name map
   fNameMap.AddName(fGeometryServices->CutName(name));
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsposp(const char *name, Int_t nr, 
           const char *mother, Double_t x, Double_t y, Double_t z, Int_t irot, 
           const char *konly, Float_t *upar, Int_t np ) 
{ 
/// Single precision interface.

   //TG4Globals::Warning("TG4GeometryManager::Gsposp in single precision.");

   G4double* parin = fGeometryServices->CreateG4doubleArray(upar, np); 
   Gsposp(name, nr, mother, x, y, z, irot, konly, parin, np);
   delete [] parin;
} 
 
 
//_____________________________________________________________________________
void  TG4GeometryManager::Gsbool(const char* onlyVolName, 
                                 const char* manyVolName)
{ 
/// Help for resolving MANY.
/// Specify the ONLY volume that overlaps with the 
/// specified MANY and has to be substracted.
// ---  

   // write token to the output file
   //if (fWriteGeometry) 
   //  fOutputManager->WriteGsbool(onlyVolName, manyVolName);

   G4gsbool(onlyVolName, manyVolName);
} 
 
 
//_____________________________________________________________________________
void TG4GeometryManager::WriteEuclid(const char* fileName, 
          const char* topVolName, Int_t number, Int_t nlevel)
{
///  Geant3 desription:                                                     \n
///  ==================                                                     \n
///
///     ******************************************************************  \n
///     *                                                                *  \n
///     *  Write out the geometry of the detector in EUCLID file format  *  \n
///     *                                                                *  \n
///     *       filnam : will be with the extension .euc                 *  \n
///     *       topvol : volume name of the starting node                *  \n
///     *       number : copy number of topvol (relevant for gsposp)     *  \n
///     *       nlevel : number of  levels in the tree structure         *  \n
///     *                to be written out, starting from topvol         *  \n
///     *                                                                *  \n
///     *       Author : M. Maire                                        *  \n
///     *                                                                *  \n
///     ******************************************************************  \n
///
///     File filnam.tme is written out with the definitions of tracking
///     medias and materials.                                               \n
///     As to restore original numbers for materials and medias, program
///     searches in the file euc_medi.dat and comparing main parameters of
///     the mat. defined inside geant and the one in file recognizes them
///     and is able to take number from file. If for any material or medium,
///     this procedure fails, ordering starts from 1.
///     Arrays IOTMED and IOMATE are used for this procedure

  TG4Globals::Warning(
    "TG4GeometryManager::WriteEuclid(..) is not yet implemented.");
}

//_____________________________________________________________________________
void TG4GeometryManager::SetRootGeometry()
{
/// Convert Root geometry to G4 geometry objects.

#ifdef USE_VGM
  TG4RootGeometryManager rootGeometryManager( fGeometryServices, 
			                     &fMediumMap, &fMediumNameVector);
					     
  rootGeometryManager.VerboseLevel(VerboseLevel());			      
  rootGeometryManager.ImportRootGeometry();
  
  fVMCGeometry = false;
#else
  G4String text = "TG4GeometryManager::SetRootGeometry: \n";
  text = text + "    Geant4 VMC has been installed without VGM. \n"; 
  text = text + "    Root geometry is not supported."; 
  TG4Globals::Exception(text);
#endif
}                   

//
// public methods - Geant4 only
//
 
//_____________________________________________________________________________
G4VPhysicalVolume* TG4GeometryManager::CreateG4Geometry()
{
/// Create G4 geometry objects according to the G3VolTable 
/// and returns the world physical volume.

  // set the first entry in the G3Vol table
  Ggclos();
  G3VolTableEntry* first = G3Vol.GetFirstVTE();
  
  // transform MANY to Boolean solids
  G3toG4MANY(first);
  
  // create G4 geometry
  G3toG4BuildTree(first,0);  
  
  // fill medium map
  FillMediumMap();

  // print G3 volume table statistics
  G3Vol.VTEStat();

  // print G4 geometry statistics
  if (VerboseLevel() > 0) {
    G4cout << "G4 Stat: instantiated " 
           << fGeometryServices->NofG4LogicalVolumes()  
	   << " logical volumes \n"
	   << "                      " 
	   << fGeometryServices->NofG4PhysicalVolumes() 
	   << " physical volumes" << G4endl;
  }	   

  // position the first entry 
  // (in Geant3 the top volume cannot be positioned)
  // 
  if (!fGeometryServices->GetWorld()) {
    G4VPhysicalVolume* world
       = new G4PVPlacement(0, G4ThreeVector(), first->GetName(), 
                           first->GetLV(), 0, false, 0);
    fGeometryServices->SetWorld(world);			    
  }

  // with g3tog4 the volume name separator cannot be customised
  // by a user
  if (fGeometryServices->GetSeparator() != gSeparator) {
    
    fGeometryServices->SetSeparator(gSeparator);
    
    G4String text = "TG4GeometryManager::CreateG4Geometry: \n";
    text = text + "    The volume name separator cannot be overriden in G3toG4. \n";
    text = text + "    Its value is reset to \'";
    text = text + gSeparator;
    text = text + "\'. \n";
    text = text + "    The command /mcDet/volNameSeparator will have no effect.";
    TG4Globals::Warning(text);
  }

  return fGeometryServices->GetWorld();
}

 
//_____________________________________________________________________________
void TG4GeometryManager::SetUserLimits(const TG4G3CutVector& cuts,
                               const TG4G3ControlVector& controls) const
{
/// Set user limits defined in G3MedTable for all logical volumes.

  G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();

  for (G4int i=0; i<G4int(lvStore->size()); i++) {
    G4LogicalVolume* lv = (*lvStore)[i];
 
    // get limits from G3Med
    TG4Limits* tg4Limits = 0;
    G4int mediumIndex = fGeometryServices->GetMediumId(lv); 
    G3MedTableEntry* medium = G3Med.get(mediumIndex);   
    G4UserLimits* limits = medium->GetLimits();
    tg4Limits = fGeometryServices->GetLimits(limits, cuts, controls);

    // get tracking medium name
    G4String name = fMediumNameVector[mediumIndex-1];
    
    if (tg4Limits) 
      tg4Limits->SetName(name);
    else {
      tg4Limits = fGeometryServices->FindLimits(name, true);  
      if (!tg4Limits) 
        tg4Limits = new TG4Limits(name, cuts, controls); 
    }
    
    // set new limits back to medium
    medium->SetLimits(tg4Limits);

    // limit max step for low density materials (< AIR)
    if (lv->GetMaterial()->GetDensity() < fgLimitDensity ) 
      tg4Limits->SetMaxAllowedStep(fgMaxStep);
      
    // update controls in limits according to the setup 
    // in the passed vector
    tg4Limits->Update(controls);

    // set limits to logical volume
    lv->SetUserLimits(tg4Limits);
  } 
}

//_____________________________________________________________________________
void TG4GeometryManager::ReadG3Geometry(G4String filePath)
{
/// Process g3calls.dat file and fills G3 tables.

  // verbose
  if (VerboseLevel() > 0) {
    G4cout << "Reading the call list file " << filePath << "..." << G4endl;
  }
    
  G3CLRead(filePath, NULL);

  if (VerboseLevel() > 0) {
    G4cout << "Call list file read completed. Build geometry" << G4endl;
  }  
}
 
//_____________________________________________________________________________
void TG4GeometryManager::ClearG3Tables()
{ 
/// Clear G3 volumes, materials, rotations(?) tables
/// and sensitive volumes vector.
/// The top volume is kept in the vol table.

  // clear volume table 
  // but keep the top volume in the table 
  G3VolTableEntry* top = G3Vol.GetFirstVTE();
  G4String name = top->GetName();
  G4String shape = top->GetShape(); 
  G3VolTableEntry* keep 
    = new G3VolTableEntry(name, shape, top->GetRpar(), top->GetNpar(), 
                           top->GetNmed(), top->GetSolid(), false);
  keep->SetLV(top->GetLV());
  G3Vol.Clear();  
  G3Vol.PutVTE(keep);
  
  // clear other tables
  //G3Rot.Clear();
  G3SensVol.clear(); 
}

 
//_____________________________________________________________________________
void TG4GeometryManager::ClearG3TablesFinal()
{
/// Clear G3 medias and volumes tables
/// (the top volume is removed from the vol table)

  G3Mat.Clear();
  G3Med.Clear();
  G3Vol.Clear();  
}

 
//_____________________________________________________________________________
void TG4GeometryManager::OpenOutFile(G4String filePath)
{ 
/// Open output file.

  fOutputManager->OpenFile(filePath);
}

 
//_____________________________________________________________________________
void TG4GeometryManager::CloseOutFile()
{ 
/// Close output file.

  fOutputManager->CloseFile();
}

 
//_____________________________________________________________________________
void TG4GeometryManager::SetWriteGeometry(G4bool writeGeometry)
{ 
/// Control geometry output.

  fWriteGeometry = writeGeometry; 
}

 
//_____________________________________________________________________________
void TG4GeometryManager::SetMapSecond(const G4String& name)
{
/// Set the second name for the map of volumes names.

  fNameMap.SetSecond(name);
}
