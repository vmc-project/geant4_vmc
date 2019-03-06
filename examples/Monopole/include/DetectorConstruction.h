#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  DetectorConstruction.h
/// \brief Definition of the DetectorConstruction class
///
/// Geant4 monopole adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

namespace VMC
{
namespace Monopole
{

/// \ingroup Monopole
/// \brief The detector construction (via TGeo )
///
/// \date 06/07/2018
/// \author I. Hrivnacova; IPN, Orsay

class DetectorConstruction : public TObject
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    void ConstructMaterials();
    void ConstructGeometry();

    // set methods
    void SetAbsorberSizeX(Double_t sizeX);
    void SetAbsorberSizeYZ(Double_t sizeYZ);
    void SetAbsorberMaterial(const TString& name);
    // void SetMagField(Double_t fieldValue);
    void SetMaxStepSize(Double_t maxStepSize);
    void SetGeometryInitialized(Bool_t geometryInitialized);
    // void UpdateGeometry();

    //
    // get methods
    Double_t  GetWorldSizeX() const          { return fWorldSizeX; }
    Double_t  GetAbsorberSizeX() const       { return fAbsorberSizeX; }
    Double_t  GetMaxStepSize() const         { return fMaxStepSize; }
    TString   GetAbsorberMaterial() const    { return fAbsorberMaterial; }
    Bool_t    GetGeometryInitialized() const { return fGeometryInitialized; }

  private:
    void PrintParameters();

    // data members
    TString   fWorldMaterial;
    TString   fAbsorberMaterial;
    Double_t  fAbsorberSizeX;
    Double_t  fAbsorberSizeYZ;
    Double_t  fWorldSizeX;
    Double_t  fWorldSizeYZ;
    Double_t  fMaxStepSize;
    Bool_t    fGeometryInitialized;

  ClassDef(DetectorConstruction,1) //DetectorConstruction
};

// inline functions

inline void DetectorConstruction::SetGeometryInitialized(Bool_t geometryInitialized) {
  fGeometryInitialized = geometryInitialized;
}

}
}

#endif //DETECTOR_CONSTRUCTION_H
