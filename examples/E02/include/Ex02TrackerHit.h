#ifndef EX02_TRACKER_HIT_H
#define EX02_TRACKER_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02TrackerHit.h
/// \brief Definition of the Ex02TrackerHit class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: Ex02TrackerHit.hh,v 1.6 2002/01/09 17:24:09 ranjard Exp  \n
/// GEANT4 tag Name: geant4-04-00-patch-02 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>

/// \ingroup E02
/// \brief The tracker hit
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex02TrackerHit : public TObject
{
  public:
    Ex02TrackerHit();
    virtual ~Ex02TrackerHit();

    // methods
    //void Draw();
    virtual void Print(const Option_t* option = "") const;

    //
    // set methods
    
    /// Set track Id
    /// \param track The new track Id value
    void SetTrackID  (Int_t track)  { fTrackID = track; };
    
    /// Set chamber number
    /// \param chamb The new chamber number value
    void SetChamberNb(Int_t chamb)  { fChamberNb = chamb; };  

    /// Set energy deposit
    /// \param de The new energy deposit value
    void SetEdep     (Double_t de)  { fEdep = de; };
    
    /// Set position
    /// \param xyz The new position value
    void SetPos      (TVector3 xyz) { fPos = xyz; };
      
    //
    // get methods
    
    /// \return The track Id
    Int_t GetTrackID()   { return fTrackID; };

    /// \return The chamber number
    Int_t GetChamberNb() { return fChamberNb; };

    /// \return The energy deposit
    Double_t GetEdep()   { return fEdep; };      

    /// \return The track position
    TVector3 GetPos()    { return fPos; };
      
  private:
    Int_t      fTrackID;   ///< Track Id
    Int_t      fChamberNb; ///< Chamber number
    Double_t   fEdep;      ///< Energy deposit
    TVector3   fPos;       ///< Track position
    
  ClassDef(Ex02TrackerHit,1) //Ex02TrackerHit  
};

#endif //EX02_TRACKER_HIT_H


