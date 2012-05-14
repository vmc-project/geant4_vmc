#ifndef A01_ROOT_DETECTOR_CONSTRUCTION_H
#define A01_ROOT_DETECTOR_CONSTRUCTION_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2012 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file  A01RootDetectorConstruction.h
/// \brief Definition of the A01RootDetectorConstruction class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo: \n
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TObject.h>
#include <TString.h>

/// \ingroup A01
/// \brief The detector construction (via TGeo )
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01RootDetectorConstruction : public TObject
{
  public:  
    A01RootDetectorConstruction(const TString& geometryFileName);
    virtual ~A01RootDetectorConstruction();

  public:
     void ConstructGeometry();
          
  private:      
     // data members  
     TString fGeometryFileName; ///< The root geometry file name
     
  ClassDef(A01RootDetectorConstruction,1) //A01RootDetectorConstruction
};

#endif //A01_DETECTOR_CONSTRUCTION_H
