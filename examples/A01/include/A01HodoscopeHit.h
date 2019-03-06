#ifndef A01_HODOSCOPE_HIT_H
#define A01_HODOSCOPE_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01HodoscopeHit.h
/// \brief Definition of the A01HodoscopeHit class
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>
#include <TGeoMatrix.h>

/// \ingroup A01
/// \brief The hodoscope hit
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01HodoscopeHit : public TObject
{
  public:
    A01HodoscopeHit(Int_t id, Double_t t);
    A01HodoscopeHit();
    virtual ~A01HodoscopeHit();

    // methods
    virtual void Print(Option_t* option = "") const;
    void Reset();

    // set methods
    void SetID(Int_t id)       { fID = id; }
    void SetVolId(Int_t volId) { fVolID = volId; }
    void SetTime(Double_t t)   { fTime = t; }
    void SetTransformation(const TGeoHMatrix& transformation) { fTransformation = transformation;  }

    // get methods
    Int_t      GetID() const      { return fID; }
    Int_t      GetVolId() const   { return fVolID; }
    Double_t   GetTime() const    { return fTime; }
    const TGeoHMatrix& GetTransformation() const { return fTransformation; }

  private:
    Int_t        fID;    ///< The hit ID
    Int_t        fVolID; ///< The volume ID
    Double_t     fTime;  ///< The hit time
    TGeoHMatrix  fTransformation; ///< The transformation of the hit volume

  ClassDef(A01HodoscopeHit,1) //A01HodoscopeHit
};

#endif //A01_HODOSCOPE_HIT_H


