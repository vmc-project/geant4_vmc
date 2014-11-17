#ifndef A01_HAD_CALOR_HIT_H
#define A01_HAD_CALOR_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01HadCalorHit.h 
/// \brief Definition of the A01HadCalorHit class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>
#include <TGeoMatrix.h>

/// \ingroup A01
/// \brief The hadron calorimeter hit
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01HadCalorHit : public TObject
{
  public:
    A01HadCalorHit(Int_t icol, Int_t irow);
    A01HadCalorHit();
    virtual ~A01HadCalorHit();

    // methods
    virtual void Print(Option_t* option = "") const;
    void Reset();
                 
    // set methods
    void SetColumnID(Int_t z)  { fColumnID = z; }
    void SetRowID(Int_t volId) { fRowID = volId; }
    void SetEdep(Double_t de)  { fEdep = de; }
    void AddEdep(Double_t de)  { fEdep += de; }
    void SetTransformation(const TGeoHMatrix& transformation) { fTransformation = transformation;  }

    // get methods
    Int_t      GetColumnID() const  { return fColumnID; }
    Int_t      GetRowID() const   { return fRowID; }
    Double_t   GetEdep() const    { return fEdep; }
    const TGeoHMatrix& GetTransformation() const { return fTransformation; }
      
  private:
    Int_t        fColumnID; ///< The column ID
    Int_t        fRowID;    ///< The row ID
    Double_t     fEdep;     ///< The energy deposit
    TGeoHMatrix  fTransformation; ///< The transformation of the hit volume
    
  ClassDef(A01HadCalorHit,1) //A01HadCalorHit  
};

#endif //A01_HAD_CALOR_HIT_H


