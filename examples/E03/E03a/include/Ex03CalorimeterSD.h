#ifndef EX03_CALORIMETER_SD_H
#define EX03_CALORIMETER_SD_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Ex03CalorimeterSD.h
/// \brief Definition of the Ex03CalorimeterSD class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo: \n
/// Id: ExN03CalorimeterSD.hh,v 1.4 2002/01/09 17:24:11 ranjard Exp
/// GEANT4 tag Name: geant4-05-00
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TNamed.h>
#include <TClonesArray.h>

class Ex03DetectorConstruction;
class Ex03CalorHit;
class TVirtualMC;

/// \ingroup E03a
/// \brief The calorimeter sensitive detector
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03CalorimeterSD : public TNamed
{
  public:
    Ex03CalorimeterSD(const char* name,
                      Ex03DetectorConstruction* detector);
    Ex03CalorimeterSD(const Ex03CalorimeterSD& origin,
                      Ex03DetectorConstruction* detector);
    Ex03CalorimeterSD();
    virtual ~Ex03CalorimeterSD();

    // methods
    void    Initialize();
    Bool_t  ProcessHits();
    void    EndOfEvent();
    void    Register();
    virtual void  Print(Option_t* option = "") const;
    void    PrintTotal() const;


    // set methods
    void SetVerboseLevel(Int_t level);

    // get methods
    Ex03CalorHit* GetHit(Int_t i) const;

  private:
    // methods
    void  ResetHits();

    // data members
    TVirtualMC*    fMC;            ///< The VMC implementation
    Ex03DetectorConstruction*  fDetector; ///< Detector construction
    TClonesArray*  fCalCollection; ///< Hits collection
    Int_t          fAbsorberVolId; ///< The absorber volume Id
    Int_t          fGapVolId;      ///< The gap volume Id
    Int_t          fVerboseLevel;  ///< Verbosity level

  ClassDef(Ex03CalorimeterSD,1) //Ex03CalorimeterSD

};

/// Set verbose level
/// \param level The new verbose level value
inline void Ex03CalorimeterSD::SetVerboseLevel(Int_t level)
{ fVerboseLevel = level; }

#endif //EX02_CALORIMETER_SD_H

