// $Id: TG4MCGeometry.h,v 1.1 2006/12/12 16:21:15 brun Exp $
/// \ingroup geometry
//
/// \class TG4MCGeometry
/// \brief Geant4 implementation of the TVirtualMCGeometry interface 
///
/// This interfaces defines methods for building geometry a la geant3 
/// and is implemented with use og G3toG4
///
/// Author: V. Berejnoi, I. Hrivnacova

#ifndef TG4_G3_MC_GEOMETRY_H
#define TG4_G3_MC_GEOMETRY_H

#include "TG4Verbose.h"
#include "TG4IntMap.h"
#include "TG4Globals.h"
#include "TG4OpSurfaceMap.h"

#include <globals.hh>

#include <Rtypes.h>
#include <TMCOptical.h>
#include <TVirtualMCGeometry.h>

class TG4GeometryServices;
class TG4G3CutVector;
class TG4G3ControlVector;

class G4VPhysicalVolume;
class G4OpticalSurface;

class TGeoHMatrix;
class TArrayD;
class TString;

class TG4MCGeometry :  public TVirtualMCGeometry,
                       public TG4Verbose
{
  public:
    TG4MCGeometry();
    virtual ~TG4MCGeometry();

    //
    // methods (from the base class)
    
    // detector composition
    virtual void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Float_t* buf, Int_t nwbuf);
    virtual void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Double_t* buf, Int_t nwbuf);
    virtual void  Mixture(Int_t& kmat, const char *name, Float_t *a, 
                     Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat);
    virtual void  Mixture(Int_t& kmat, const char *name, Double_t *a, 
                     Double_t *z, Double_t dens, Int_t nlmat, Double_t *wmat);
    virtual void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
                     Double_t stmin, Float_t* ubuf, Int_t nbuf);
    virtual void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
                     Double_t stmin, Double_t* ubuf, Int_t nbuf);
    virtual void  Matrix(Int_t& krot, Double_t thetaX, Double_t phiX, 
                     Double_t thetaY, Double_t phiY, Double_t thetaZ, 
                     Double_t phiZ);

    // functions from GBASE 
    virtual void  Ggclos(); 


    // functions from GGEOM 
    virtual Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Double_t *upar, Int_t np); 
    virtual Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Float_t *upar, Int_t np); 
    virtual void  Gsdvn(const char *name, const char *mother, Int_t ndiv, 
                        Int_t iaxis);
    virtual void  Gsdvn2(const char *name, const char *mother, Int_t ndiv, 
                         Int_t iaxis, Double_t c0i, Int_t numed); 
    virtual void  Gsdvt(const char *name, const char *mother, Double_t step,
                         Int_t iaxis, Int_t numed, Int_t ndvmx); 
    virtual void  Gsdvt2(const char *name, const char *mother, Double_t step, 
                         Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx); 

    virtual void  Gsord(const char *name, Int_t iax); 
    virtual void  Gspos(const char *name, Int_t nr, const char *mother,  
                        Double_t x, Double_t y, Double_t z, Int_t irot, 
                        const char *konly); 
    virtual void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Double_t *upar, Int_t np); 
    virtual void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Float_t *upar, Int_t np); 
    virtual void  Gsbool(const char* onlyVolName, const char* manyVolName);
        
                         
    // Info is geometry is defined
    Bool_t IsGeometryDefined() const;                         
                               
    // functions for access to geometry
    virtual Bool_t GetTransformation(const TString& volumePath,
                         TGeoHMatrix& matrix);
    virtual Bool_t GetShape(const TString& volumePath,
                         TString& shapeType, TArrayD& par);
    virtual Bool_t GetMaterial(const TString& volumeName,
                               TString& name, Int_t& imat,
                               Double_t& a, Double_t& z, Double_t& density,
                               Double_t& radl, Double_t& inter, TArrayD& par);
    virtual Bool_t GetMedium(const TString& volumeName,
                             TString& name, Int_t& imed,
                             Int_t& nmat, Int_t& isvol, Int_t& ifield,
                             Double_t& fieldm, Double_t& tmaxfd, Double_t& stemax,
                             Double_t& deemax, Double_t& epsil, Double_t& stmin,
                             TArrayD& par);
    virtual Int_t MediumId(const Text_t* mediumName) const;

    //
    // Not implemented functions from the base class
    // (these functions are implemented in SDmanager)
    //                 

    virtual Int_t VolId(const Text_t* volName) const;
    virtual const char* VolName(Int_t id) const;
    virtual Int_t NofVolumes() const;
    virtual Int_t NofVolDaughters(const char* volName) const;
    virtual const char*  VolDaughterName(const char* volName, Int_t i) const;
    virtual Int_t        VolDaughterCopyNo(const char* volName, Int_t i) const;
    virtual Int_t VolId2Mate(Int_t id) const;

  private:
    TG4MCGeometry(const TG4MCGeometry& right);
    TG4MCGeometry& operator=(const TG4MCGeometry& right);

    // data members
    TG4GeometryServices*  fGeometryServices;//geometry services
    TG4StringVector  fMaterialNameVector; // vector of material names sorted in the
                                          // the order of materials in G3Mat
    G4int            fMediumCounter;   //global medium counter
    G4int            fMaterialCounter; //global material counter
    G4int            fMatrixCounter;   //global matrix counter
};

#endif //TG4_VMC_GEOMETRY_MANAGER_H

