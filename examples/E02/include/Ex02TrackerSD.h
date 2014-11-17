#ifndef EX02_TRACKER_SD_H
#define EX02_TRACKER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02TrackerSD.h
/// \brief Definition of the Ex02TrackerSD class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: ExN02TrackerSD.hh,v 1.6 2002/01/09 17:24:09 ranjard Exp  \n
/// GEANT4 tag Name: geant4-04-00-patch-02 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>
#include <TClonesArray.h>

class Ex02TrackerHit;

/// \ingroup E02
/// \brief The tracker sensitive detector
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex02TrackerSD : public TNamed
{
  public:
    Ex02TrackerSD(const char* name);
    Ex02TrackerSD(const Ex02TrackerSD& origin);
    Ex02TrackerSD();
    virtual ~Ex02TrackerSD();

    // methods
    void   Initialize();
    Bool_t  ProcessHits();
    void   EndOfEvent();
    void   Register();
    virtual void  Print(const Option_t* option = 0) const;
    
    // set methods
    void SetVerboseLevel(Int_t level);

  private:
    // methods
    Ex02TrackerHit* AddHit();
  
    // data members
    TClonesArray*  fTrackerCollection; ///< Hits collection    
    Int_t          fSensitiveVolumeID; ///< Sensitive volume Id
    Int_t          fVerboseLevel;      ///< Verbosity level
   
  ClassDef(Ex02TrackerSD,1) //Ex02TrackerSD 

};

/// Set verbose level
/// \param level The new verbose level value
inline void Ex02TrackerSD::SetVerboseLevel(Int_t level) 
{ fVerboseLevel = level; }


#endif //EX02_TRACKER_SD_H

