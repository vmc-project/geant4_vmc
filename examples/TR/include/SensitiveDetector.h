#ifndef SENSITIVE_DETECTOR_H
#define SENSITIVE_DETECTOR_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/include/ SensitiveDetector.h
/// \brief Definition of the SensitiveDetector class 
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>

namespace VMC
{
namespace TR
{

/// \ingroup TR
/// \brief The absorber sensitive detector
///
/// \date 18/12/2015
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
    void    EndOfEvent();
    virtual void  Print(Option_t* option = "") const;
    
    // set methods
    void SetVerboseLevel(Int_t level);

    // get methods
    Double_t GetEdep() const;

  private:
    // data members
    Double_t  fEdep;          ///< Energy deposit
    Int_t     fAbsorberVolId; ///< The absorber volume Id
    Int_t     fVerboseLevel;  ///< Verbosity level
   
  ClassDef(SensitiveDetector,1) //SensitiveDetector 
};

/// Set verbose level
/// \param level The new verbose level value
inline void SensitiveDetector::SetVerboseLevel(Int_t level) 
{ fVerboseLevel = level; }

/// Return energy deposit
inline Double_t SensitiveDetector::GetEdep() const
{ return fEdep; }

}
}

#endif //SENSITIVE_DETECTOR_H

