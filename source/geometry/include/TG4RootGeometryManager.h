// $Id: TG4RootGeometryManager.h,v 1.1 2004/05/05 13:32:02 brun Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4RootGeometryManager
// ------------------------
// Class for importing Root TGeo geometry in Geant4 VMC.

#ifndef TG4_ROOT_GEOMETRY_MANAGER_H
#define TG4_ROOT_GEOMETRY_MANAGER_H

#include <Rtypes.h>

#include <globals.hh>

#include "TG4Globals.h"
#include "TG4Verbose.h"
#include "TG4IntMap.h"
#include "TG4RootGeometryConvertor.h"

class TG4GeometryServices;
class TG4NameMap;

class G4VPhysicalVolume;

class TG4RootGeometryManager : public TG4Verbose
{
  public:
    TG4RootGeometryManager(TG4GeometryServices* geometryServices,
                           TG4IntMap* mediumMap, 
			   TG4StringVector* mediumNameVector,
                           G4bool useG3TMLimits);
    // --> protected
    // TG4RootGeometryManager();
    // TG4RootGeometryManager(const TG4RootGeometryManager& right);
    virtual ~TG4RootGeometryManager();

    //
    // methods 
    // import geometry from Root (built via TGeo)
    void ImportRootGeometry();                   
    
  protected:
    TG4RootGeometryManager();
    TG4RootGeometryManager(const TG4RootGeometryManager& right);

    // operators
    TG4RootGeometryManager& operator=(const TG4RootGeometryManager& right);

  private:
    // methods
    void Gstmed(G4int itmed, G4String, const G4Material* material, 
                G4int isvol, G4int, G4double, G4double,
                G4double stemax, G4double, G4double,
                G4double, G4double*, G4int useG3TMLimits);
    void Medium(Int_t& kmed, const char *name, const G4Material* material, 
                Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                Double_t stemax, Double_t deemax, Double_t epsil, 
		Double_t stmin, Double_t* ubuf, Int_t nbuf);
    void ConvertRootGeometry();
    void ConvertRootMedias();
    void FillMediumMap();
        
    // static data members
    TG4RootGeometryConvertor fConvertor;       // roottog4 convertor
    TG4GeometryServices*     fGeometryServices;// geometry services
    TG4IntMap*        fMediumMap;        // map of volumes names to medias IDs
    TG4intMap         fMediumIdMap;      // map of medium IDs
    TG4StringVector*  fMediumNameVector; // vector of material names sorted in
                                         // the order of medias in G3Med
    G4int   fMediumCounter;   //global medium counter
    G4bool  fUseG3TMLimits;   //if true: G3 limits are passed to G4 
                                         //(in development)
};

#endif //TG4_ROOT_GEOMETRY_MANAGER_H

