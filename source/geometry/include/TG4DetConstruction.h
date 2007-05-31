// $Id: TG4DetConstruction.h,v 1.11 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4DetConstruction
/// \brief Detector construction for building geometry using 
/// TVirtualMCApplication.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_DET_CONSTRUCTION_H
#define TG4_DET_CONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;

class TG4DetConstruction : public G4VUserDetectorConstruction
{
  public:
    TG4DetConstruction();
    virtual ~TG4DetConstruction();

    // methods
    virtual G4VPhysicalVolume* Construct();

  private:    
    TG4DetConstruction(const TG4DetConstruction& right);
    TG4DetConstruction& operator=(const TG4DetConstruction& right);
}; 

#endif //TG4_DET_CONSTRUCTION_H

