#ifndef EX03_DETECTOR_CONSTRUCTION_H
#define EX03_DETECTOR_CONSTRUCTION_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file  Ex03DetectorConstruction.h
/// \brief Definition of the Ex03DetectorConstruction class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo: \n
/// Id: ExN03DetectorConstruction.hh,v 1.5 2002/01/09 17:24:11 ranjard Exp 
/// GEANT4 tag $Name:  $
///
/// \author I. Hrivnacova; IPN, Orsay

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

/// \ingroup E03
/// \brief The detector construction (via TGeo )
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03DetectorConstruction : public TObject
{
  public:  
    Ex03DetectorConstruction();
    virtual ~Ex03DetectorConstruction();

  public:
     void ConstructMaterials();
     void ConstructGeometry();
     void SetCuts();
     void SetControls();
     void PrintCalorParameters(); 
     //void UpdateGeometry();
     
     // set methods
     void SetNbOfLayers (Int_t value);  
     void SetDefaultMaterial(const TString& materialName);     
     void SetAbsorberMaterial(const TString& materialName);     
     void SetGapMaterial(const TString& materialName );     
     void SetCalorSizeYZ(Double_t value);          
     void SetAbsorberThickness(Double_t value);     
     void SetGapThickness(Double_t value);
     
     //
     // get methods

     /// \return The number of calorimeter layers
     Int_t    GetNbOfLayers() const    { return fNbOfLayers; }

     /// \return The world size x component
     Double_t GetWorldSizeX() const    { return fWorldSizeX; } 

     /// \return The world size y,z component
     Double_t GetWorldSizeYZ() const   { return fWorldSizeYZ; }

     /// \return The calorimeter size y,z component
     Double_t GetCalorSizeYZ() const   { return fCalorSizeYZ; }

     /// \return The calorimeter thickness
     Double_t GetCalorThickness()const { return fCalorThickness; } 

     /// \return The absorber thickness
     Double_t GetAbsorberThickness()const { return fAbsorberThickness; }      

     /// \return The gap thickness
     Double_t GetGapThickness()const   { return fGapThickness; }
          
  private:      
     // methods
     void  ComputeCalorParameters();

     // data members  
     Int_t     fNbOfLayers;       ///< The number of calorimeter layers
     Double_t  fWorldSizeX;       ///< The world size x component
     Double_t  fWorldSizeYZ;      ///< The world size y,z component
     Double_t  fCalorSizeYZ;      ///< The calorimeter size y,z component
     Double_t  fCalorThickness;   ///< The calorimeter thickness
     Double_t  fLayerThickness;   ///< The calorimeter layer thickness
     Double_t  fAbsorberThickness;///< The absorber thickness
     Double_t  fGapThickness;     ///< The gap thickness
     
     TString   fDefaultMaterial;  ///< The default material name
     TString   fAbsorberMaterial; ///< The absorber material name
     TString   fGapMaterial;      ///< The gap material name
     
  ClassDef(Ex03DetectorConstruction,1) //Ex03DetectorConstruction
};

#endif //EX03_DETECTOR_CONSTRUCTION_H
