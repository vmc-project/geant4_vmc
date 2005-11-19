// $Id: Ex02TrackerHit.h,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Id: Ex02TrackerHit.hh,v 1.6 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002

#ifndef EX02_TRACKER_HIT_H
#define EX02_TRACKER_HIT_H

#include <TObject.h>
#include <TVector3.h>

class Ex02TrackerHit : public TObject
{
  public:
    Ex02TrackerHit();
    virtual ~Ex02TrackerHit();

    // methods
    //void Draw();
    virtual void Print(Option_t* option = "") const;

    // set methods
    void SetTrackID  (Int_t track)  { fTrackID = track; };
    void SetChamberNb(Int_t chamb)  { fChamberNb = chamb; };  
    void SetEdep     (Double_t de)  { fEdep = de; };
    void SetPos      (TVector3 xyz) { fPos = xyz; };
      
    // get methods
    Int_t GetTrackID()   { return fTrackID; };
    Int_t GetChamberNb() { return fChamberNb; };
    Double_t GetEdep()   { return fEdep; };      
    TVector3 GetPos()    { return fPos; };
      
  private:
    Int_t      fTrackID;
    Int_t      fChamberNb;
    Double_t   fEdep;
    TVector3   fPos;
    
  ClassDef(Ex02TrackerHit,1) //Ex02TrackerHit  
};

#endif //EX02_TRACKER_HIT_H


