#ifndef A01_EM_CALOR_HIT_H
#define A01_EM_CALOR_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01EmCalorHit.h
/// \brief Definition of the A01EmCalorHit class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>
#include <TGeoMatrix.h>

/// \ingroup A01
/// \brief The EM calorimeter hit
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01EmCalorHit : public TObject
{
  public:
    A01EmCalorHit(Int_t z);
    A01EmCalorHit();
    virtual ~A01EmCalorHit();

    // methods
    virtual void Print(Option_t* option = "") const;
    void Reset();

    // set methods
    void SetCellID(Int_t z)    { fCellID = z; }
    void SetVolId(Int_t volId) { fVolID = volId; }
    void SetEdep(Double_t de)  { fEdep = de; }
    void AddEdep(Double_t de)  { fEdep += de; }
    void SetTransformation(const TGeoHMatrix& transformation) { fTransformation = transformation;  }

    // get methods
    Int_t      GetCellID() const  { return fCellID; }
    Int_t      GetVolId() const   { return fVolID; }
    Double_t   GetEdep() const    { return fEdep; }
    const TGeoHMatrix& GetTransformation() const { return fTransformation; }

  private:
    Int_t GetCellID(Int_t column, Int_t row) const;

    Int_t        fCellID; ///< The cell ID
    Int_t        fVolID;  ///< The volume ID
    Double_t     fEdep;   ///< The energy deposit
    TGeoHMatrix  fTransformation; ///< The transformation of the hit volume

  ClassDef(A01EmCalorHit,1) //A01EmCalorHit
};

#endif //A01_EM_CALOR_HIT_H


