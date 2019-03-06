#ifndef GARFIELD_DETECTOR_CONSTRUCTION_H
#define GARFIELD_DETECTOR_CONSTRUCTION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  ExGarfield/include/DetectorConstruction.h
/// \brief Definition of the ExGarfield::DetectorConstruction class
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

#include <map>

#include <Riostream.h>
#include <TObject.h>
#include <TString.h>

namespace VMC
{
namespace ExGarfield
{

/// \ingroup ExGarfield
/// \brief The detector construction (via TGeo )
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

class DetectorConstruction : public TObject
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    void Construct();

  ClassDef(DetectorConstruction,1) //DetectorConstruction
};

}
}

#endif //GARFIELD_DETECTOR_CONSTRUCTION_H
