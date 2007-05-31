// $Id: Ex02TrackerSD.h,v 1.2 2005/11/19 07:07:47 brun Exp $

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
// Id: ExN02TrackerSD.hh,v 1.6 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002

#ifndef EX02_TRACKER_SD_H
#define EX02_TRACKER_SD_H

#include <TNamed.h>
#include <TClonesArray.h>

class Ex02TrackerHit;

class Ex02TrackerSD : public TNamed
{
  public:
    Ex02TrackerSD(const char* name);
    Ex02TrackerSD();
    virtual ~Ex02TrackerSD();

    // methods
    void   Initialize();
    Bool_t  ProcessHits();
    void   EndOfEvent();
    void   Register();
    virtual void  Print(Option_t* option = 0) const;
    
    // set methods
    void SetVerboseLevel(Int_t level) { fVerboseLevel = level; }

  private:
    // methods
    Ex02TrackerHit* AddHit();
  
    // data members
    TClonesArray*  fTrackerCollection;
    Int_t          fSensitiveVolumeID;
    Int_t          fVerboseLevel;
   
  ClassDef(Ex02TrackerSD,1) //Ex02TrackerSD 

};

#endif //EX02_TRACKER_SD_H

