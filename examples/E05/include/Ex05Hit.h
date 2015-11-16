#ifndef EX05_HIT_H
#define EX05_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05Hit.h 
/// \brief Definition of the Ex05Hit class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>

/// \ingroup E05
/// \brief The calorimeter hit
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

class Ex05Hit : public TObject
{
  public:
    Ex05Hit();
    virtual ~Ex05Hit();

    // methods
    virtual void Print(Option_t* option = "") const;
    void Reset();

    // set methods   
    void SetEdep(Double_t de)      { fEdep = de; };
    void SetPos(TVector3 xyz)      { fPos = xyz; };
    void SetCrystalNum(Int_t num)  { fCrystalNumber=num; };

    // get methods   
    Double_t  GetEdep() const       { return fEdep; };
    Int_t     GetCrystalNum() const { return fCrystalNumber; };  
    TVector3  GetPos() const        { return fPos; };
      
  private:
    Double_t  fEdep;     ///< Energy deposit in the absorber
    TVector3  fPos;      ///< Cell position
    Int_t     fCrystalNumber;   ///< Crystal number
    
  ClassDef(Ex05Hit,1) //Ex05Hit  
};

#endif //EX05_HIT_H


