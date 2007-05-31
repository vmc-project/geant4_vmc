// $Id: Ex02DetectorConstruction.h,v 1.2 2002/12/03 15:04:17 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Id: ExN02DetectorConstruction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002

#ifndef EX02_DETECTOR_CONSTRUCTION_H
#define EX02_DETECTOR_CONSTRUCTION_H

#include <TObject.h>
#include <TString.h>

class Ex02DetectorConstruction : public TObject
{
  public:  
    Ex02DetectorConstruction();
    virtual ~Ex02DetectorConstruction();

  public:
     void ConstructMaterials();
     void ConstructGeometry();
     
     // set methods
     void SetTargetMaterial (const TString& name);
     void SetChamberMaterial(const TString& name);

     Double_t GetTrackerFullLength() {return fTrackerLength;};
     Double_t GetTargetFullLength()  {return fTargetLength;};
     Double_t GetWorldFullLength()   {return fWorldLength;}; 
     
  private:
     Double_t  fWorldLength;   // Full length of the world volume
     Double_t  fTargetLength;  // Full length of Target
     Double_t  fTrackerLength; // Full length of Tracker
     Int_t     fNofChambers;   // Nb of chambers in the tracker region
     Double_t  fChamberWidth;  // Width of the chambers
     Double_t  fChamberSpacing;// Distance between chambers
     Int_t     fImedAir;
     Int_t     fImedPb;
     Int_t     fImedXe;
     
  ClassDef(Ex02DetectorConstruction,1) //Ex02DetectorConstruction
};

#endif //EX02_DETECTOR_CONSTRUCTION_H
