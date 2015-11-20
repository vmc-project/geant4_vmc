#ifndef GARFIELD_HIT_H
#define GARFIELD_HIT_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Garfield/include/Hit.h 
/// \brief Definition of the Garfield::Hit class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TVector3.h>

namespace Garfield
{

/// \ingroup Garfield
/// \brief The calorimeter hit
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

class Hit : public TObject
{
  public:
    Hit();
    virtual ~Hit();

    // methods
    virtual void Print(Option_t* option = "") const;
    void Reset();

         /// Add energy deposit in the absorber
         /// \param de  Enery deposit
    void AddEdepAbs(Double_t de) { fEdepAbs += de; }

         /// Add energy deposit in the gas
         /// \param de  Enery deposit
    void AddEdepGas(Double_t de) { fEdepGas += de;}

         /// Add track length in the absorber
         /// \param dl  Track length
    void AddTrackLengthAbs(Double_t dl) { fTrackLengthAbs += dl; }

         /// Add avalanche size in the gas
         /// \param das  Avalanche size
    void AddAvalancheSize(Double_t das) { fAvalancheSize += das; }
                 
         /// Add gain in the gas
         /// \param dg  Gain
    void AddGain(Double_t dg) { fGain += dg; }

    // get methods
             /// \return The energy deposit in the absorber 
    Double_t GetEdepAbs() const { return fEdepAbs; };
             /// \return The track length in the absorber 
    Double_t GetTrackLengthAbs() const { return fTrackLengthAbs; };
             /// \return The energy deposit in the gas 
    Double_t GetEdepGas() const { return fEdepGas; };
             /// \return The avalanche size in the gas 
    Double_t GetAvalancheSize() const { return fAvalancheSize; };
             /// \return The gain in the gas
    Double_t GetGain() const { return fGain; };
      
  private:
    Double_t  fEdepAbs;        ///< Energy deposit in the absorber
    Double_t  fEdepGas;        ///< Energy deposit in the gas
    Double_t  fTrackLengthAbs; ///< Track length in the absorber
    Double_t  fAvalancheSize;  ///< Avalanche size in the gas
    Double_t  fGain;           ///< Gain
    
  ClassDef(Hit,1) //Hit  
};

}

#endif //GARFIELD_HIT_H
