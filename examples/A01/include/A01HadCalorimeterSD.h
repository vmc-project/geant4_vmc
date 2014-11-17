#ifndef A01_HAD_CALORIMETER_SD_H
#define A01_HAD_CALORIMETER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  A01HadCalorimeterSD.h
/// \brief Definition of the A01HadCalorimeterSD class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>
#include <TClonesArray.h>

class A01HadCalorHit;

/// \ingroup A01
/// \brief The hadron calorimeter sensitive detector
///
/// \author I. Hrivnacova; IPN, Orsay

class A01HadCalorimeterSD : public TNamed
{
  public:
    A01HadCalorimeterSD(const char* name);
    A01HadCalorimeterSD(const A01HadCalorimeterSD& origin);
    A01HadCalorimeterSD();
    virtual ~A01HadCalorimeterSD();

    // methods
    void    Initialize();
    Bool_t  ProcessHits();
    void    EndOfEvent();
    void    Register();
    virtual void  Print(Option_t* option = "") const;
    void    PrintTotal() const;
    
    
    // set methods
    void SetWriteHits(Bool_t writeHits);
    void SetVerboseLevel(Int_t level);

    // get methods
    A01HadCalorHit* GetHit(Int_t i) const;

  private:
    // methods
    void  ResetHits();
    
    // static data members
    static const Int_t fgkNofColumns; 
    static const Int_t fgkNofRows; 
  
    // data members
    TClonesArray*  fCalCollection; ///< Hits collection 
    Int_t          fVolId;         ///< The calorimeter volume Id
    Bool_t         fWriteHits;     ///< Option to write hits
    Int_t          fVerboseLevel;  ///< Verbosity level
   
  ClassDef(A01HadCalorimeterSD,1) //A01HadCalorimeterSD 

};

/// (In)Activate writing hits on file
/// \param writeHits  The new value of the option
inline void  A01HadCalorimeterSD::SetWriteHits(Bool_t writeHits)
{ fWriteHits = writeHits; }

/// Set verbose level
/// \param level The new verbose level value
inline void A01HadCalorimeterSD::SetVerboseLevel(Int_t level) 
{ fVerboseLevel = level; }

#endif //A01_HAD_CALORIMETER_SD_H

