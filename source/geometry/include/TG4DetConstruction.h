#ifndef TG4_DET_CONSTRUCTION_H
#define TG4_DET_CONSTRUCTION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4DetConstruction.h
/// \brief Definition of the TG4DetConstruction class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;

/// \ingroup geometry
/// \brief Detector construction for building geometry using 
/// TVirtualMCApplication.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4DetConstruction : public G4VUserDetectorConstruction
{
  public:
    TG4DetConstruction();
    virtual ~TG4DetConstruction();

    // methods
    virtual G4VPhysicalVolume* Construct();
    virtual G4VPhysicalVolume* ConstructSlave();
    
    void SlaveTG4DetConstruction();

  private:    
    /// Not implemented
    TG4DetConstruction(const TG4DetConstruction& right);
    /// Not implemented
    TG4DetConstruction& operator=(const TG4DetConstruction& right);
    
    G4VPhysicalVolume* fWorld;
}; 

#endif //TG4_DET_CONSTRUCTION_H

