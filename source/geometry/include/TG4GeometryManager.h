// $Id: TG4GeometryManager.h,v 1.10 2005/11/18 21:29:35 brun Exp $
/// \ingroup geometry
//
/// \class TG4GeometryManager
/// \brief Geant4 implementation of the TVirtualMC interface methods                    
/// for building geometry and access to it.
///
/// Author: V. Berejnoi, I. Hrivnacova

#ifndef TG4_GEOMETRY_MANAGER_H
#define TG4_GEOMETRY_MANAGER_H

#include "TG4Verbose.h"
#include "TG4IntMap.h"
#include "TG4Globals.h"
#include "TG4OpSurfaceMap.h"

#include <globals.hh>

#include <Rtypes.h>
#include <TMCOptical.h>


class TG4GeometryOutputManager;
class TG4GeometryServices;
class TG4G3CutVector;
class TG4G3ControlVector;

class G4VPhysicalVolume;
class G4OpticalSurface;

class TGeoHMatrix;
class TArrayD;
class TString;

class TG4GeometryManager : public TG4Verbose
{
  public:
    TG4GeometryManager();
    // --> protected
    // TG4GeometryManager(const TG4GeometryManager& right);
    virtual ~TG4GeometryManager();

    // static access method
    static TG4GeometryManager* Instance();

    //
    // methods (from the base class)
    
    // info about supporting geometry defined via Root
    virtual Bool_t IsRootGeometrySupported() const;

    // detector composition
    void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Float_t* buf, Int_t nwbuf);
    void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Double_t* buf, Int_t nwbuf);
    void  Mixture(Int_t& kmat, const char *name, Float_t *a, 
                     Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat);
    void  Mixture(Int_t& kmat, const char *name, Double_t *a, 
                     Double_t *z, Double_t dens, Int_t nlmat, Double_t *wmat);
    void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
		     Double_t stmin, Float_t* ubuf, Int_t nbuf);
    void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
		     Double_t stmin, Double_t* ubuf, Int_t nbuf);
    void  Matrix(Int_t& krot, Double_t thetaX, Double_t phiX, 
                     Double_t thetaY, Double_t phiY, Double_t thetaZ, 
		     Double_t phiZ);

    // functions from GBASE 
    void  Ggclos(); 

    // functions from GCONS 
    void  Gfmate(Int_t imat, char *name, Float_t &a, Float_t &z,  
  		         Float_t &dens, Float_t &radl, Float_t &absl,
		         Float_t* ubuf, Int_t& nbuf); 
    void  Gfmate(Int_t imat, char *name, Double_t &a, Double_t &z,  
  		         Double_t &dens, Double_t &radl, Double_t &absl,
		         Double_t* ubuf, Int_t& nbuf);
    void  Gstpar(Int_t itmed, const char *param, Double_t parval); 
    void  SetCerenkov(Int_t itmed, Int_t npckov, Float_t *ppckov,
			 Float_t *absco, Float_t *effic, Float_t *rindex); 
    void  SetCerenkov(Int_t itmed, Int_t npckov, Double_t *ppckov,
			 Double_t *absco, Double_t *effic, Double_t *rindex); 

    // functions for definition of surfaces
    // and material properties for optical physics
    virtual void  DefineOpSurface(const char *name,
                         EMCOpSurfaceModel model,
			 EMCOpSurfaceType surfaceType,
			 EMCOpSurfaceFinish surfaceFinish,
			 Double_t sigmaAlpha);
    virtual void  SetBorderSurface(const char *name,
                         const char* vol1Name, int vol1CopyNo,
                         const char* vol2Name, int vol2CopyNo,
			 const char* opSurfaceName);
    virtual void  SetSkinSurface(const char *name,
                         const char* volName,
			 const char* opSurfaceName);
    virtual void  SetMaterialProperty(
                         Int_t itmed, const char* propertyName, 
			 Int_t np, Double_t* pp, Double_t* values);
    virtual void  SetMaterialProperty(
                         Int_t itmed, const char* propertyName,
			 Double_t value);
    virtual void  SetMaterialProperty(
                         const char* surfaceName, const char* propertyName, 
			 Int_t np, Double_t* pp, Double_t* values);
    
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

    // functions from GGEOM 
    Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Double_t *upar, Int_t np); 
    Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Float_t *upar, Int_t np); 
    void  Gsdvn(const char *name, const char *mother, Int_t ndiv, 
                        Int_t iaxis);
    void  Gsdvn2(const char *name, const char *mother, Int_t ndiv, 
                         Int_t iaxis, Double_t c0i, Int_t numed); 
    void  Gsdvt(const char *name, const char *mother, Double_t step,
                         Int_t iaxis, Int_t numed, Int_t ndvmx); 
    void  Gsdvt2(const char *name, const char *mother, Double_t step, 
                         Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx); 

    void  Gsord(const char *name, Int_t iax); 
    void  Gspos(const char *name, Int_t nr, const char *mother,  
                        Double_t x, Double_t y, Double_t z, Int_t irot, 
                        const char *konly); 
    void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Double_t *upar, Int_t np); 
    void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Float_t *upar, Int_t np); 
    void  Gsbool(const char* onlyVolName, const char* manyVolName);
        
    // Euclid		       
    void WriteEuclid(const char* fileName, const char* topVolName, 
                         Int_t number, Int_t nlevel); //new
		               
     // set geometry from Root (built via TGeo)
    void SetRootGeometry();                   
    
    // end of methods
    // 

    //
    // methods for Geant4 only 
 
    G4VPhysicalVolume* CreateG4Geometry();
    void FinishGeometry();
    void SetUserLimits(const TG4G3CutVector& cuts,
                       const TG4G3ControlVector& controls) const;
    void ReadG3Geometry(G4String filePath);
    void ClearG3Tables();       
    void ClearG3TablesFinal();
    void OpenOutFile(G4String filePath);
    void CloseOutFile();
    G4bool IsVMCGeometry() const;
   
    // set methods
    void SetWriteGeometry(G4bool writeGeometry);
     
  protected:
    TG4GeometryManager(const TG4GeometryManager& right);

    // operators
    TG4GeometryManager& operator=(const TG4GeometryManager& right);

  private:
    // methods
    void FillMediumMapFromG3();
    void FillMediumMapFromG4();
    void FillG3MedTableFromG4();
        
    // static data members
    static TG4GeometryManager*  fgInstance;     //this instance
    static const G4double       fgLimitDensity; //material density limit
                                                //for setting max allowed step 
    static const G4double       fgMaxStep;      //max allowed step in materials 
                                                //with density < fLimitDensity

    // data members
    TG4GeometryOutputManager*   fOutputManager;   //output manager 
    TG4GeometryServices*        fGeometryServices;//geometry services
    TG4IntMap        fMediumMap;       //map of volumes names to medias IDs
    TG4OpSurfaceMap  fOpSurfaceMap;    //map of optical surfaces names to their objects 
    TG4StringVector  fMaterialNameVector; // vector of material names sorted in the
                                          // the order of materials in G3Mat
    TG4StringVector  fMediumNameVector;   // vector of material names sorted in the
                                          // the order of medias in G3Med
    G4int            fMediumCounter;   //global medium counter
    G4int            fMaterialCounter; //global material counter
    G4int            fMatrixCounter;   //global matrix counter
    G4bool           fWriteGeometry;   //if true: geometry parameters are written
                                       //in a file (ASCII) 
    G4bool           fVMCGeometry;     //true if geometry is built using VMC calls
                                       //(false if geometry is built by conversion)				        
};

// inline methods

inline TG4GeometryManager* TG4GeometryManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline G4bool TG4GeometryManager::IsVMCGeometry() const { 
  /// Return true if geometry is built using VMC calls,
  /// false if geometry is built by conversion
  return fVMCGeometry; 
}

#endif //TG4_GEOMETRY_MANAGER_H

