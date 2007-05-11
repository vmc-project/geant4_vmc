// $Id: TG4GeometryServices.h,v 1.13 2007/03/22 09:03:13 brun Exp $
/// \ingroup geometry
//
/// \class TG4GeometryServices
/// \brief Services for accessing to Geant4 geometry
/// 
/// The class provides service methods for accessing to Geant4 geometry,
/// namely using VMC volumes and materials identifiers. 
///
/// Author: I. Hrivnacova

#ifndef TG4_GEOMETRY_SERVICES_H
#define TG4_GEOMETRY_SERVICES_H

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4MediumMap.h"
#include "TG4OpSurfaceMap.h"

#include <globals.hh>
#include <G4OpticalSurface.hh>
#include <G4SurfaceProperty.hh>
#include <G4Transform3D.hh>

#include <Rtypes.h>
#include <TMCOptical.h>

#include <map>

class TG4MediumMap;
class TG4NameMap;
class TG4Limits;
class TG4G3CutVector;
class TG4G3ControlVector;

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4UserLimits;
class G4OpticalSurface;

class TGeoHMatrix;

class TG4GeometryServices : public TG4Verbose
{
  public:
    TG4GeometryServices();
    virtual ~TG4GeometryServices();

    // static access method
    static TG4GeometryServices* Instance();

    // methods
           // utilities  
    G4double* CreateG4doubleArray(Float_t* array, G4int size) const;
    G4double* CreateG4doubleArray(Double_t* array, G4int size) const;
    G4String  CutName(const char* name) const;
    G4String  CutMaterialName(const char* name) const;
    G4String  CutVolumePath(const G4String& volumePath, 
                            G4String& volName, G4int& copyNo) const; 
    G4String  UserVolumeName(const G4String& name) const;

    G4OpticalSurfaceModel  SurfaceModel(EMCOpSurfaceModel model) const;                                            
    G4SurfaceType          SurfaceType(EMCOpSurfaceType surfType) const;
    G4OpticalSurfaceFinish SurfaceFinish(EMCOpSurfaceFinish finish) const; 
    void  Convert(const G4Transform3D& transform, TGeoHMatrix& matrix) const;                                           

    G4Material* MixMaterials(G4String name, G4double density,
                             const TG4StringVector& matNames, 
                             const TG4doubleVector& matWeights);
           // printing 
    void PrintLimits(const G4String& name) const;
    void PrintVolumeLimits(const G4String& volumeName) const;
    void PrintStatistics(G4bool open, G4bool close) const;
    void PrintLogicalVolumeStore() const;
    void PrintPhysicalVolumeStore() const;
    void PrintElementTable() const;
    void PrintMaterials() const;
    void PrintMedia() const;

    // set methods
    void SetWorld(G4VPhysicalVolume* world);
    void SetIsG3toG4(G4bool isG3toG4);
    void SetG3toG4Separator(char separator);

    // get methods
           // volumes
    Int_t NofG3Volumes() const; 
    Int_t NofG4LogicalVolumes() const; 
    Int_t NofG4PhysicalVolumes() const; 
    G4VPhysicalVolume* GetWorld() const;

    TG4Limits* GetLimits(G4UserLimits* limits) const;
    TG4Limits* GetLimits(G4UserLimits* limits,
                         const TG4G3CutVector& cuts,
                         const TG4G3ControlVector& controls) const;

    G4LogicalVolume*   FindLogicalVolume(const G4String& name, 
                                       G4bool silent = false) const;
    G4VPhysicalVolume* FindPhysicalVolume(const G4String& name, G4int copyNo,
                                       G4bool silent = false) const;
    G4VPhysicalVolume* FindDaughter(const G4String& name, G4int copyNo,
                                       G4LogicalVolume* mlv,
                                       G4bool silent = false) const;
    TG4Limits*         FindLimits(const G4String& name, 
                                       G4bool silent = false) const;

          // materials
    G4int    GetMediumId(G4LogicalVolume* lv) const;    
    G4double GetEffA(G4Material* material) const;
    G4double GetEffZ(G4Material* material) const;
    G4Material* FindMaterial(G4double a, G4double z, G4double density) const;
    G4Material* FindMaterial(G4double* a, G4double* z, G4double density, 
                             G4int nmat, G4double* wmat) const;

    TG4MediumMap*    GetMediumMap() const;
    TG4OpSurfaceMap* GetOpSurfaceMap() const;

  protected:
    TG4GeometryServices(const TG4GeometryServices& right);
    TG4GeometryServices& operator=(const TG4GeometryServices& right);

  private:
    // methods        
    G4bool IsG3Volume(const G4String& lvName) const;
    G4bool CompareElement(G4double a, G4double z, const G4Element* elem) const;
    G4bool CompareMaterial(G4int nofElements, G4double density, 
                           const G4Material* material) const;
    G4double* ConvertAtomWeight(G4int nmat, G4double* a, G4double* wmat) const;

    // static data members
    static TG4GeometryServices*  fgInstance;   //this instance
    static const G4double  fgkAZTolerance;     //A,Z tolerance
    static const G4double  fgkDensityTolerance;//density tolerance (percentual)
 
    // data members
    G4bool             fIsG3toG4;  // info if user geometry is defined via G3toG4
    TG4MediumMap*      fMediumMap; // map of madia
    TG4OpSurfaceMap*   fOpSurfaceMap;//map of optical surfaces names to their objects 
    G4VPhysicalVolume* fWorld;     //top physical volume (world)
};

// inline methods

inline TG4GeometryServices* TG4GeometryServices::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline void TG4GeometryServices::SetWorld(G4VPhysicalVolume* world) { 
  /// Set the world physical volume
  fWorld = world; 
}

inline void TG4GeometryServices::SetIsG3toG4(G4bool isG3toG4) {
  /// Set the info if user geometry is defined via G3toG4
  fIsG3toG4 = isG3toG4;
}  

inline G4VPhysicalVolume* TG4GeometryServices::GetWorld() const {
  /// Set the world physical volume
  return fWorld; 
}

inline TG4MediumMap* TG4GeometryServices::GetMediumMap() const {
  /// Return the medium map
  return fMediumMap;
}  

inline TG4OpSurfaceMap* TG4GeometryServices::GetOpSurfaceMap() const{
  /// Return the medium map
  return fOpSurfaceMap;
}  

#endif //TG4_GEOMETRY_SERVICES_H

