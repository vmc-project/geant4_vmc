// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN06 adapted to Virtual Monte Carlo 
// Id: ExN06DetectorConstruction.hh,v 1.4 2003/01/23 15:34:23 maire Exp 
// GEANT4 tag Name: geant4-07-00-cand-01 
//
//
// by Ivana Hrivnacova, 16.5.2005

#ifndef EX06_DETECTOR_CONSTRUCTION_OLD_H
#define EX06_DETECTOR_CONSTRUCTION_OLD_H

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

class Ex06DetectorConstructionOld : public TObject
{
  public:  
    Ex06DetectorConstructionOld();
    virtual ~Ex06DetectorConstructionOld();

  public:
     void ConstructMaterials();
     void ConstructGeometry();
     void ConstructOpGeometry();
     
  private:      
     // data members  
     Double_t fExpHallSize;
     Double_t fTankSize;
     Double_t fBubbleSize;
     Int_t    fImedAir;
     Int_t    fImedWater;
     
  ClassDef(Ex06DetectorConstructionOld,1) //Ex06DetectorConstructionOld
};

#endif //EX06_DETECTOR_CONSTRUCTION_OLD_H
