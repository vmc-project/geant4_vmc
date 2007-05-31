// $Id: TG4PostDetConstruction.h,v 1.1 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4PostDetConstruction
/// \brief The post detector construction used with G4Root navigation
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_POST_DET_CONSTRUCTION_H
#define TG4_POST_DET_CONSTRUCTION_H

#include <TG4RootDetectorConstruction.h>

class TG4PostDetConstruction;

class TG4PostDetConstruction : public TVirtualUserPostDetConstruction
{
  public:
    TG4PostDetConstruction();
    virtual ~TG4PostDetConstruction();

    // methods
    virtual void Initialize(TG4RootDetectorConstruction *dc);

  private:    
    TG4PostDetConstruction(const TG4PostDetConstruction& right);
    TG4PostDetConstruction& operator=(const TG4PostDetConstruction& right);
}; 

#endif //TG4_DET_CONSTRUCTION_H

