// $Id: Ex03DetectorConstructionOld.h,v 1.1 2006/12/12 16:21:38 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03DetectorConstruction.hh,v 1.5 2002/01/09 17:24:11 ranjard Exp 
// GEANT4 tag $Name:  $
//
// by Ivana Hrivnacova, 6.3.2003

#ifndef EX03_DETECTOR_CONSTRUCTION_OLD_H
#define EX03_DETECTOR_CONSTRUCTION_OLD_H

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

class Ex03DetectorConstructionOld : public TObject
{
  typedef map<TString, Int_t>  NameMap; 
  typedef NameMap::const_iterator   NameMapIterator; 

  public:  
    Ex03DetectorConstructionOld();
    virtual ~Ex03DetectorConstructionOld();

  public:
     void ConstructMaterials();
     void ConstructGeometry();
     void PrintCalorParameters(); 
     //void UpdateGeometry();
     
     // set methods
     void SetNbOfLayers (Int_t value);  
     void SetDefaultMaterial(const TString&);     
     void SetAbsorberMaterial(const TString&);     
     void SetGapMaterial(const TString&);     
     void SetCalorSizeYZ(Double_t value);          
     void SetAbsorberThickness(Double_t value);     
     void SetGapThickness(Double_t value);
     
     // get methods
     Int_t    GetNbOfLayers() const    { return fNbOfLayers; }
     Double_t GetWorldSizeX() const    { return fWorldSizeX; } 
     Double_t GetWorldSizeYZ() const   { return fWorldSizeYZ; }
     Double_t GetCalorSizeYZ() const   { return fCalorSizeYZ; }
     Double_t GetCalorThickness()const    { return fCalorThickness; } 
     Double_t GetAbsorberThickness()const { return fAbsorberThickness; }      
     Double_t GetGapThickness()const      { return fGapThickness; }
          
  private:      
     // methods
     void  ComputeCalorParameters();

     // data members  
     Int_t     fNbOfLayers;
     Double_t  fWorldSizeX;
     Double_t  fWorldSizeYZ;
     Double_t  fCalorSizeYZ;
     Double_t  fCalorThickness;
     Double_t  fLayerThickness;
     Double_t  fAbsorberThickness;
     Double_t  fGapThickness;
     
     TString   fDefaultMaterial;
     TString   fAbsorberMaterial;
     TString   fGapMaterial;
     
  ClassDef(Ex03DetectorConstructionOld,1) //Ex03DetectorConstructionOld
};

#endif //EX03_DETECTOR_CONSTRUCTION_H
