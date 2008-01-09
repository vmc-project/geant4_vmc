#ifndef EX02_DETECTOR_CONSTRUCTION_OLD_H
#define EX02_DETECTOR_CONSTRUCTION_OLD_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex02DetectorConstructionOld.h
/// \brief Definition of the Ex02DetectorConstructionOld class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo \n
/// Id: ExN02DetectorConstruction.hh,v 1.7 2002/01/09 17:24:09 ranjard Exp  \n
/// GEANT4 tag Name: geant4-04-00-patch-02 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TString.h>

/// \ingroup E02
/// \brief The old detector construction (via VMC functions)
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex02DetectorConstructionOld : public TObject
{
  public:  
    Ex02DetectorConstructionOld();
    virtual ~Ex02DetectorConstructionOld();

  public:
     void ConstructMaterials();
     void ConstructGeometry();
     
     // set methods
     void SetTargetMaterial (const TString& name);
     void SetChamberMaterial(const TString& name);

              /// \return The tracker full length
     Double_t GetTrackerFullLength() {return fTrackerLength;};
              /// \return The target full length
     Double_t GetTargetFullLength()  {return fTargetLength;};
              /// \return The world full length
     Double_t GetWorldFullLength()   {return fWorldLength;}; 
     
  private:
     Double_t  fWorldLength;   ///< Full length of the world volume
     Double_t  fTargetLength;  ///< Full length of Target
     Double_t  fTrackerLength; ///< Full length of Tracker
     Int_t     fNofChambers;   ///< Nb of chambers in the tracker region
     Double_t  fChamberWidth;  ///< Width of the chambers
     Double_t  fChamberSpacing;///< Distance between chambers
     Int_t     fImedAir;       ///< The Air medium Id
     Int_t     fImedPb;        ///< The Lead medium Id
     Int_t     fImedXe;        ///< The Xenon gas medium Id
     
  ClassDef(Ex02DetectorConstructionOld,1) //Ex02DetectorConstructionOld
};

#endif //EX02_DETECTOR_CONSTRUCTION_OLD_H
