// $Id: Ex02DetectorConstructionOld.h,v 1.2 2002/12/03 15:04:17 brun Exp $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Id: ExN02DetectorConstruction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002

#ifndef EX02_DETECTOR_CONSTRUCTION_OLD_H
#define EX02_DETECTOR_CONSTRUCTION_OLD_H

#include <TObject.h>
#include <TString.h>

class Ex02DetectorConstructionOld : public TObject
{
  public:  
    Ex02DetectorConstructionOld();
    virtual ~Ex02DetectorConstructionOld();

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
     
  ClassDef(Ex02DetectorConstructionOld,1) //Ex02DetectorConstructionOld
};

#endif //EX02_DETECTOR_CONSTRUCTION_OLD_H
