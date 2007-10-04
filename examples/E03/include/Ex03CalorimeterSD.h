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
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03CalorimeterSD.hh,v 1.4 2002/01/09 17:24:11 ranjard Exp 
// GEANT4 tag Name: geant4-05-00
//
// by Ivana Hrivnacova, 6.3.2003

#ifndef EX03_CALORIMETER_SD_H
#define EX03_CALORIMETER_SD_H

#include <TNamed.h>
#include <TClonesArray.h>

class Ex03DetectorConstruction;
class Ex03CalorHit;

class Ex03CalorimeterSD : public TNamed
{
  public:
    Ex03CalorimeterSD(const char* name, Ex03DetectorConstruction* detector);
    Ex03CalorimeterSD();
    virtual ~Ex03CalorimeterSD();

    // methods
    void    Initialize();
    Bool_t  ProcessHits();
    void    EndOfEvent();
    void    Register();
    virtual void  Print(Option_t* option = "") const;
    void    PrintTotal() const;
    
    // set methods
    void SetVerboseLevel(Int_t level) { fVerboseLevel = level; }

    // get methods
    Ex03CalorHit* GetHit(Int_t i) const;

  private:
    // methods
    void  ResetHits();
  
    // data members
    Ex03DetectorConstruction*  fDetector;
    TClonesArray*  fCalCollection;
    Int_t          fAbsorberVolId;
    Int_t          fGapVolId;
    Int_t          fVerboseLevel;
   
  ClassDef(Ex03CalorimeterSD,1) //Ex03CalorimeterSD 

};

#endif //EX02_CALORIMETER_SD_H

