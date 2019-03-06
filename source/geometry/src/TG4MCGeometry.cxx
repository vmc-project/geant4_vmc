//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MCGeometry.cxx
/// \brief Implementation of the TG4MCGeometry class
///
/// C++ interface to Geant3 basic routines for building Geant4 geometry
/// by V. Berejnoi, 25.2.1999;                                           \n
/// Materials, tracking media support added by I.Hrivnacova, 27.5.1999.
///
/// \author V. Berejnoi, CERN; I. Hrivnacova; IPN, Orsay

#include "TG4MCGeometry.h"
#include "TG4GeometryServices.h"
#include "TG4Medium.h"
#include "TG4Limits.h"
#include "TG4G3Units.h"
#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"
#include "TG4Globals.h"

#ifdef USE_G3TOG4
#include <G3toG4.hh>
#include <G3MatTable.hh>
#endif

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4OpticalSurface.hh>
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
#include <G4SystemOfUnits.hh>

#include <TArrayD.h>
#include <TString.h>
#include <Riostream.h>

#ifdef USE_G3TOG4
/// Extern global method from g3tog4
void G3CLRead(G4String &, char *);
#endif

//_____________________________________________________________________________
TG4MCGeometry::TG4MCGeometry()
  : TG4Verbose("g4MCGeometry"),
    fGeometryServices(0),
    fMaterialNameVector()
{
/// Default constructor

  fGeometryServices = TG4GeometryServices::Instance();
  if ( !fGeometryServices ) {
    TG4Globals::Exception(
      "TG4MCGeometry", "TG4MCGeometry",
      "TG4GeometryServices have to be defined first");
  }
}

//_____________________________________________________________________________
TG4MCGeometry::~TG4MCGeometry()
{
/// Destructor
}

//
// public methods
//

//_____________________________________________________________________________
void TG4MCGeometry::Material(Int_t& kmat, const char* name, Double_t a,
          Double_t z, Double_t dens, Double_t radl, Double_t absl, Float_t* buf,
          Int_t nwbuf)
{
/// Create material.                                                         \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate.

    G4double* bufin = fGeometryServices->CreateG4doubleArray(buf, nwbuf);
    Material(kmat, name, a, z, dens, radl, absl, bufin, nwbuf);
    delete [] bufin;
}


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void TG4MCGeometry::Material(Int_t& kmat, const char* name, Double_t a,
          Double_t z, Double_t dens, Double_t radl, Double_t /*absl*/, Double_t* buf,
          Int_t nwbuf)
{
/// Create material.                                                         \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate
/// Available only with USE_G3TOG4 option.

    G4String namein = fGeometryServices->CutMaterialName(name);

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
      TG4Globals::Warning(
        "TG4MCGeometry", "Material",
        "User defined parameters for material " + TString(namein) +
        " are ignored by Geant4.");
    }
}
#else
//_____________________________________________________________________________
void TG4MCGeometry::Material(Int_t&, const char*, Double_t,
          Double_t, Double_t, Double_t, Double_t, Double_t*,Int_t)
{
/// Create material.                                                         \n
/// Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Material",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


//_____________________________________________________________________________
void TG4MCGeometry::Mixture(Int_t& kmat, const char *name, Float_t *a,
          Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat)
{
/// Create material composed of more elements.                            \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate

   G4cout << "TG4MCGeometry::Mixture Fl" << G4endl;
   G4cout << fGeometryServices << G4endl;

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

#ifdef USE_G3TOG4
//_____________________________________________________________________________
void TG4MCGeometry::Mixture(Int_t& kmat, const char *name, Double_t* a,
          Double_t* z, Double_t dens, Int_t nlmat, Double_t* wmat)
{
/// Create material composed of more elements.                            \n
/// !! Parameters radl, absl, buf, nwbuf are ignored in G4gsmate
/// Available only with USE_G3TOG4 option.

   G4String namein = fGeometryServices->CutMaterialName(name);

   G4cout << namein << G4endl;

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
#else
//_____________________________________________________________________________
void TG4MCGeometry::Mixture(Int_t&, const char*, Double_t*,
          Double_t*, Double_t, Int_t, Double_t*)
{
/// Create material composed of more elements.                            \n
/// Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Mixture",
     "This method requires Geant4 installation with G3toG4.");
}
#endif

//_____________________________________________________________________________
void TG4MCGeometry::Medium(Int_t& kmed, const char *name, Int_t nmat,
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
///  Geant3 desription:
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


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void TG4MCGeometry::Medium(Int_t& kmed, const char *name, Int_t nmat,
          Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd,
          Double_t stemax, Double_t deemax, Double_t epsil,
          Double_t stmin, Double_t* /*ubuf*/, Int_t nbuf)
{
/// Create a temporary "medium" that is used for
/// assigning corresponding parameters to G4 objects:
/// - NTMED is stored as a second material index;
/// - ISVOL is used for builing G3SensVolVector;
/// - STEMAX is passed in G4UserLimits
/// - !! The other parameters (IFIELD, FIELDM, TMAXFD, DEEMAX, EPSIL, STMIN)
/// are ignored by Geant4.
///
///  Geant3 desription:
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
/// Available only with USE_G3TOG4 option.

  G4String namein = fGeometryServices->CutMaterialName(name);

  G4gstmed(kmed, name, nmat, isvol, ifield, fieldm, tmaxfd, stemax, deemax,
       epsil, stmin, 0, stemax > 0.);
     // instead of the nbuf argument the bool is passed
     // in order to pass stemax into G4UserLimits

  // Add medium to the map (to keep its name which is not stored in G3toG4)
  TG4Medium* medium = fGeometryServices->GetMediumMap()->AddMedium(kmed);
  medium->SetName(name);
  medium->SetIfield(ifield);

  if (nbuf > 0) {
    TG4Globals::Warning(
      "TG4MCGeometry", "Medium",
      "User defined parameters for medium " + TString(namein) +
      " are ignored by Geant4.");
  }
}
#else
//_____________________________________________________________________________
void TG4MCGeometry::Medium(Int_t&, const char *, Int_t,
          Int_t, Int_t, Double_t, Double_t, Double_t, Double_t, Double_t,
          Double_t, Double_t*, Int_t)
{
/// Create a temporary "medium".
/// Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Medium",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void TG4MCGeometry::Matrix(Int_t& krot, Double_t thetaX, Double_t phiX,
           Double_t thetaY, Double_t phiY, Double_t thetaZ, Double_t phiZ)
{
/// Create rotation matrix.
/// Available only with USE_G3TOG4 option.

  G4gsrotm(krot, thetaX, phiX, thetaY, phiY, thetaZ, phiZ);
}
#else
//_____________________________________________________________________________
void TG4MCGeometry::Matrix(Int_t&, Double_t, Double_t,
           Double_t, Double_t, Double_t, Double_t)
{
/// Create rotation matrix.
/// Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Matrix",
     "This method requires Geant4 installation with G3toG4.");
}
#endif

//_____________________________________________________________________________
void TG4MCGeometry::Ggclos()
{
/// Set the top VTE in temporary G3 volume table.
/// Close geometry output file (if fWriteGeometry is set true).
///
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  close out the geometry

#ifdef USE_G3TOG4
  G4ggclos();
#else
  TG4Globals::Exception("TG4MCGeometry", "Ggclos()",
     "This method requires Geant4 installation with G3toG4.");
#endif
}


#ifdef USE_G3TOG4
//_____________________________________________________________________________
Int_t TG4MCGeometry::Gsvolu(const char *name, const char *shape,
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
///  Available only with USE_G3TOG4 option.
// ---

  G4gsvolu(fGeometryServices->CutName(name),
           fGeometryServices->CutName(shape), nmed, upar, npar);
  return 0;
}
#else
//_____________________________________________________________________________
Int_t TG4MCGeometry::Gsvolu(const char *, const char *,
          Int_t, Double_t*, Int_t)
{
/// It creates a new volume in the JVOLUM data structure.
/// Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Gsvolu",
     "This method requires Geant4 installation with G3toG4.");
  return 0;
}
#endif


//_____________________________________________________________________________
Int_t TG4MCGeometry::Gsvolu(const char *name, const char *shape,
          Int_t nmed, Float_t *upar, Int_t npar)
{
/// Single precision interface.

  G4double* parin = fGeometryServices->CreateG4doubleArray(upar, npar);

  G4int result
   = Gsvolu(name, shape, nmed, parin, npar);

  delete [] parin;

  return result;
}


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvn(const char *name, const char *mother,
           Int_t ndiv, Int_t iaxis)
{
///  Geant3 desription:
///  ==================
///  - NAME   Volume name
///  - MOTHER Mother volume name
///  - NDIV   Number of divisions
///  - IAXIS  Axis value
///
///  - X,Y,Z of CAXIS will be translated to 1,2,3 for IAXIS.                \n
///  It divides a previously defined volume.
///  Available only with USE_G3TOG4 option.
//  ---

  G4gsdvn(fGeometryServices->CutName(name),
          fGeometryServices->CutName(mother), ndiv, iaxis);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvn(const char *, const char *, Int_t, Int_t)
{
/// It divides a previously defined volume.
/// Not available without USE_G3TOG4 option.
//  ---

  TG4Globals::Exception("TG4MCGeometry", "Gsdvn",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvn2(const char *name, const char *mother,
           Int_t ndiv, Int_t iaxis, Double_t c0i, Int_t numed)
{
///  Geant3 desription:                                                       \n
///  ==================                                                       \n
///  Divides mother into ndiv divisions called name
///  along axis iaxis starting at coordinate value c0.
///  The new volume created will be medium number numed.
///  Available only with USE_G3TOG4 option.

  G4gsdvn2(fGeometryServices->CutName(name),
             fGeometryServices->CutName(mother), ndiv, iaxis, c0i, numed);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvn2(const char *, const char *,
           Int_t, Int_t, Double_t, Int_t)
{
///  Geant3 desription:                                                       \n
///  ==================                                                       \n
///  Divides mother into ndiv divisions called name
///  along axis iaxis starting at coordinate value c0.
///  Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Gsdvn2",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvt(const char *name, const char *mother,
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
///  Available only with USE_G3TOG4 option.

  G4gsdvt(fGeometryServices->CutName(name),
          fGeometryServices->CutName(mother), step, iaxis, numed, ndvmx);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvt(const char *, const char *,
           Double_t, Int_t, Int_t, Int_t)
{
///  Divides MOTHER into divisions called NAME along
///  axis IAXIS in steps of STEP.
///  Not available without USE_G3TOG4 option.

  TG4Globals::Exception("TG4MCGeometry", "Gsdvt",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvt2(const char *name, const char *mother,
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
///  Available only with USE_G3TOG4 option.

  G4gsdvt2(fGeometryServices->CutName(name),
           fGeometryServices->CutName(mother), step, iaxis, c0, numed, ndvmx);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gsdvt2(const char *, const char *,
           Double_t, Int_t, Double_t, Int_t, Int_t)
{
///  Divides MOTHER into divisions called NAME along
///  axis IAXIS starting at coordinate value C0 with step
///  size STEP.                                                              \n
///  Not available without USE_G3TOG4 option.


  TG4Globals::Exception("TG4MCGeometry", "Gsdvt",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


//_____________________________________________________________________________
void  TG4MCGeometry::Gsord(const char* /*name*/, Int_t /*iax*/)
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

  TG4Globals::Warning(
    "TG4MCGeometry", "Gsord", "Dummy method.");
}


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gspos(const char *vname, Int_t num,
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
///  Available only with USE_G3TOG4 option.
// ---

  G4gspos(fGeometryServices->CutName(vname), ++num,
          fGeometryServices->CutName(vmoth), x, y, z, irot, vonly);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gspos(const char *, Int_t,
          const char *, Double_t, Double_t, Double_t, Int_t, const char *)
{
///  Position a volume into an existing one
///  Not available without USE_G3TOG4 option.
// ---

  TG4Globals::Exception("TG4MCGeometry", "Gspos",
     "This method requires Geant4 installation with G3toG4.");
}
#endif

#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gsposp(const char *name, Int_t nr,
           const char *mother, Double_t x, Double_t y, Double_t z, Int_t irot,
           const char *konly, Double_t* upar, Int_t np )
{
///  Geant3 desription:                                                      \n
///  ==================                                                      \n
///  Place a copy of generic volume NAME with user number
///  NR inside MOTHER, with its parameters UPAR(1..NP)
///  Available only with USE_G3TOG4 option.

  G4gsposp(fGeometryServices->CutName(name), ++nr,
           fGeometryServices->CutName(mother), x, y, z, irot, konly,
           upar, np);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gsposp(const char *, Int_t,
          const char *, Double_t, Double_t, Double_t, Int_t,
          const char *, Double_t*, Int_t)
{
///  Place a copy of generic volume NAME with user number
///  NR inside MOTHER, with its parameters UPAR(1..NP)
///  Not available without USE_G3TOG4 option.

 TG4Globals::Exception("TG4MCGeometry", "Gsposp",
     "This method requires Geant4 installation with G3toG4.");
}
#endif


//_____________________________________________________________________________
void  TG4MCGeometry::Gsposp(const char *name, Int_t nr,
           const char *mother, Double_t x, Double_t y, Double_t z, Int_t irot,
           const char *konly, Float_t *upar, Int_t np )
{
/// Single precision interface.

   G4double* parin = fGeometryServices->CreateG4doubleArray(upar, np);
   Gsposp(name, nr, mother, x, y, z, irot, konly, parin, np);
   delete [] parin;
}


#ifdef USE_G3TOG4
//_____________________________________________________________________________
void  TG4MCGeometry::Gsbool(const char* onlyVolName,
           const char* manyVolName)
{
/// Help for resolving MANY.
/// Specify the ONLY volume that overlaps with the
/// specified MANY and has to be substracted.
/// Available only with USE_G3TOG4 option.
// ---

  G4gsbool(onlyVolName, manyVolName);
}
#else
//_____________________________________________________________________________
void  TG4MCGeometry::Gsbool(const char*, const char*)
{
/// Help for resolving MANY.
/// Not available without USE_G3TOG4 option.
// ---

  TG4Globals::Exception("TG4MCGeometry", "Gsbool",
     "This method requires Geant4 installation with G3toG4.");
}
#endif

//_____________________________________________________________________________
Bool_t TG4MCGeometry::GetTransformation(const TString& volumePath,
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

  if ( ! pvTop ) {
    TG4Globals::Warning(
      "TG4MCGeometry", "GetTransformation:",
      "Top volume in " + volumePath + " does not exist.");
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
      TG4Globals::Warning(
        "TG4MCGeometry", "GetTransformation",
        "Daughter volume " + TString(volName) + " in " + volumePath +
        " does not exist.");
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
Bool_t TG4MCGeometry::GetShape(const TString& volumePath,
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
   std::istringstream in(copyNoStr);
   G4int copyNo;
   in >> copyNo;

   // Find physical volume
   G4VPhysicalVolume* pv
     = fGeometryServices->FindPhysicalVolume(volName, copyNo, true);

   if ( ! pv ) {
     TG4Globals::Warning(
       "TG4MCGeometry", "GetShape",
       "Physical volume " + volumePath + " does not exist.");
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

  TG4Globals::Warning(
    "TG4MCGeometry", "GetShape",
     "Shape " + TString(solid->GetEntityType()) + " not implemented.");
  return false;
}

//_____________________________________________________________________________
Bool_t TG4MCGeometry::GetMaterial(const TString& volumeName,
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
    TG4Globals::Warning(
      "TG4MCGeometry", "GetMaterial",
      "Logical volume " + volumeName + " does not exist.");
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
Bool_t TG4MCGeometry::GetMedium(const TString& volumeName,
                              TString& name, Int_t& imed,
                              Int_t& nmat, Int_t& isvol, Int_t& ifield,
                              Double_t& fieldm, Double_t& tmaxfd, Double_t& stemax,
                              Double_t& deemax, Double_t& epsil, Double_t& stmin,
                              TArrayD& par)
{
/// Return the medium parameters for the volume specified by the
/// volume name.

  // Get volume
  G4LogicalVolume* lv =
    fGeometryServices->FindLogicalVolume(volumeName.Data(), true);
  if ( !lv ) {
    TG4Globals::Warning(
      "TG4MCGeometry", "GetMaterial",
      "Logical volume " + volumeName + " does not exist.");
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
Int_t TG4MCGeometry::MediumId(const Text_t* mediumName) const
{
/// Return the medium id for medium with given name

  TG4MediumMap* mediumMap = TG4GeometryServices::Instance()->GetMediumMap();
  TG4Medium* medium = mediumMap->GetMedium(G4String(mediumName), false);

  if ( ! medium ) {
    TG4Globals::Warning(
      "TG4MCGeometry", "MediumId",
      "Medium " + TString(mediumName) + " not found.");
    return 0;
  }

  return medium->GetID();
}


//
// Not implemented functions
//


//_____________________________________________________________________________
Int_t TG4MCGeometry::VolId(const Text_t* /*volName*/) const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "VolId", "Not implemented.");
  return 0;
}

//_____________________________________________________________________________
const char* TG4MCGeometry::VolName(Int_t /*id*/) const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "VolName", "Not implemented.");
  return "";
}


//_____________________________________________________________________________
Int_t TG4MCGeometry::NofVolumes() const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "NofVolumes", "Not implemented.");
  return 0;
}


//_____________________________________________________________________________
Int_t TG4MCGeometry::NofVolDaughters(const char* /*volName*/) const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "NofVolDaughters", "Not implemented.");
  return 0;
}


//_____________________________________________________________________________
const char*  TG4MCGeometry::VolDaughterName(const char* /*volName*/,
                  Int_t /*i*/) const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "VolDaughterName", "Not implemented.");
  return "";
}


//_____________________________________________________________________________
Int_t TG4MCGeometry::VolDaughterCopyNo(const char* /*volName*/,
                  Int_t /*i*/) const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "VolDaughterCopyNo", "Not implemented.");
  return 0;
}


//_____________________________________________________________________________
Int_t TG4MCGeometry::VolId2Mate(Int_t /*id*/) const
{
/// Not implemented here - not used for geometry building

  TG4Globals::Exception(
    "TG4MCGeometry", "VolId2Mate", "Not implemented.");
  return 0;
}

