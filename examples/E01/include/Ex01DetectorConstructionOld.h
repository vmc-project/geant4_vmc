// $Id: Ex01DetectorConstructionOld.h,v 1.2 2002/12/03 15:04:17 brun Exp $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
// Old (deprecated) way of geometry definition via VMC
//
// Id: ExN02DetectorConstruction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002
// separated from Ex01MCAopplication, 8.12.2006

#ifndef EX01_DETECTOR_CONSTRUCTION_OLD_H
#define EX01_DETECTOR_CONSTRUCTION_OLD_H

#include <TObject.h>
#include <TString.h>

class Ex01DetectorConstructionOld : public TObject
{
  public:  
    Ex01DetectorConstructionOld();
    virtual ~Ex01DetectorConstructionOld();
 
    // methods
    void ConstructMaterials();
    void ConstructVolumes();

  private:
     // data members
     Int_t  fImedAr;
     Int_t  fImedAl;
     Int_t  fImedPb;
      
  ClassDef(Ex01DetectorConstructionOld,1) //Ex01DetectorConstructionOld
};

#endif //EX01_DETECTOR_CONSTRUCTION_OLD_H
