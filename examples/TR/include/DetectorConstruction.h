#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/include/DetectorConstruction.h
/// \brief Definition of the DetectorConstruction class 
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
/// Only DetectorSimpleALICE is defined.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>

namespace VMC
{
namespace TR
{

//struct RadiatorDescription;

/// \ingroup TR
/// \brief The detector construction (via TGeo )
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

class DetectorConstruction : public TObject
{
  public:  
    DetectorConstruction();
    virtual ~DetectorConstruction();

    // methods
    void ConstructGeometry();

  ClassDef(DetectorConstruction,1) //DetectorConstruction
};

}
}

#endif //DETECTOR_CONSTRUCTION_H
