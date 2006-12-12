// $Id: Ex06DetectorConstruction.h,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Geant4 ExampleN06 adapted to Virtual Monte Carlo 
// Id: ExN06DetectorConstruction.hh,v 1.4 2003/01/23 15:34:23 maire Exp 
// GEANT4 tag Name: geant4-07-00-cand-01 
//
//
// by Ivana Hrivnacova, 16.5.2005

#ifndef EX06_DETECTOR_CONSTRUCTION_H
#define EX06_DETECTOR_CONSTRUCTION_H

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

class Ex06DetectorConstruction : public TObject
{
  public:  
    Ex06DetectorConstruction();
    virtual ~Ex06DetectorConstruction();

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
     
  ClassDef(Ex06DetectorConstruction,1) //Ex06DetectorConstruction
};

#endif //EX06_DETECTOR_CONSTRUCTION_H
