// $Id: Ex03CalorHit.h,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Id: ExN03CalorHit.hh,v 1.5 2002/01/09 17:24:11 ranjard Exp 
// GEANT4 tag Name: geant4-05-00 
//
// by Ivana Hrivnacova, 6.3.2003

#ifndef EX03_CALOR_HIT_H
#define EX03_CALOR_HIT_H

#include <TObject.h>
#include <TVector3.h>

class Ex03CalorHit : public TObject
{
  public:
    Ex03CalorHit();
    virtual ~Ex03CalorHit();

    // methods
    virtual void Print(Option_t* option = "") const;
    void AddAbs(Double_t de, Double_t dl) { fEdepAbs += de; fTrackLengthAbs += dl; }
    void AddGap(Double_t de, Double_t dl) { fEdepGap += de; fTrackLengthGap += dl; }
    void Reset();
                 
    // get methods
    Double_t GetEdepAbs() { return fEdepAbs; };
    Double_t GetTrakAbs() { return fTrackLengthAbs; };
    Double_t GetEdepGap() { return fEdepGap; };
    Double_t GetTrakGap() { return fTrackLengthGap; };
      
  private:
    Double_t fEdepAbs;
    Double_t fTrackLengthAbs;
    Double_t fEdepGap; 
    Double_t fTrackLengthGap;     
    
  ClassDef(Ex03CalorHit,1) //Ex03CalorHit  
};

#endif //EX03_CALOR_HIT_H


