#ifndef TG4_OP_GEOMETRY_MANAGER_H
#define TG4_OP_GEOMETRY_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4OpGeometryManager.h
/// \brief Definition of the TG4OpGeometryManager class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"
#include "TG4IntMap.h"
#include "TG4Globals.h"
#include "TG4OpSurfaceMap.h"

#include <globals.hh>

#include <Rtypes.h>
#include <TMCOptical.h>
#include <TVirtualMCGeometry.h>

class TG4GeometryServices;

class G4OpticalSurface;

/// \ingroup geometry
/// \brief Geant4 implementation of the TVirtualMC interface methods
/// for definition of material properties for optical processes
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4OpGeometryManager : public TG4Verbose
{
  public:
    TG4OpGeometryManager();
    virtual ~TG4OpGeometryManager();

    // functions for definition of surfaces
    // and material properties for optical physics
    virtual void  SetCerenkov(Int_t itmed, Int_t npckov, Float_t *ppckov,
                         Float_t *absco, Float_t *effic, Float_t *rindex);
    virtual void  SetCerenkov(Int_t itmed, Int_t npckov, Double_t *ppckov,
                         Double_t *absco, Double_t *effic, Double_t *rindex);

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

    virtual void   Gfmate(Int_t imat, char *name, Float_t &a, Float_t &z,
                          Float_t &dens, Float_t &radl, Float_t &absl,
                          Float_t* ubuf, Int_t& nbuf);
    virtual void   Gfmate(Int_t imat, char *name, Double_t &a, Double_t &z,
                          Double_t &dens, Double_t &radl, Double_t &absl,
                          Double_t* ubuf, Int_t& nbuf);
    virtual Bool_t GetMaterial(Int_t imat, TString& name,
                          Double_t& a, Double_t& z, Double_t& density,
                          Double_t& radl, Double_t& inter, TArrayD& par);

  private:
    /// Not implemented
    TG4OpGeometryManager(const TG4OpGeometryManager& right);
    /// Not implemented
    TG4OpGeometryManager& operator=(const TG4OpGeometryManager& right);

    // methods
    Double_t AddUnit(const G4String& propertyName, Double_t value) const;

    //
    // data members

    /// geometry services
    TG4GeometryServices*  fGeometryServices;

    /// map of optical surfaces names to their objects
    TG4OpSurfaceMap*      fOpSurfaceMap;
};


#endif //TG4_OP_GEOMETRY_MANAGER_H

