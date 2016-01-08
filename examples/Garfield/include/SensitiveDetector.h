#ifndef GARFIELD_CALORIMETER_SD_H
#define GARFIELD_CALORIMETER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Garfield/include/SensitiveDetector.h
/// \brief Definition of the Garfield::SensitiveDetector class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>
#include <TClonesArray.h>

namespace VMC
{
namespace Garfield
{

class Hit;

/// \ingroup Garfield
/// \brief The calorimeter sensitive detector
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay


class SensitiveDetector : public TNamed
{
  public:
    SensitiveDetector(const char* name);
    SensitiveDetector(const SensitiveDetector& origin);
    SensitiveDetector();
    virtual ~SensitiveDetector();

    // methods
    void    Initialize();
    Bool_t  ProcessHits();
    Bool_t  UpdateFromGarfield();
    void    EndOfEvent();
    void    Register();
    virtual void  Print(Option_t* option = "") const;
    
    // set methods
    void SetVerboseLevel(Int_t level);

    // get methods
    Hit* GetHit() const;

  private:
    // data members
    Hit*     fHit;            ///< The detector hit
    Int_t    fAbsorberVolId;  ///< The absorber volume Id
    Int_t    fGasVolId;       ///< The gas volume Id
    Int_t    fVerboseLevel;   ///< Verbosity level
   
  ClassDef(SensitiveDetector,1) //SensitiveDetector 

};

/// Set verbose level
/// \param level The new verbose level value
inline void SensitiveDetector::SetVerboseLevel(Int_t level) 
{ fVerboseLevel = level; }

/// Return the hits collection
inline Hit* SensitiveDetector::GetHit() const
{ return fHit; }

}
}

#endif //EX02_CALORIMETER_SD_H

