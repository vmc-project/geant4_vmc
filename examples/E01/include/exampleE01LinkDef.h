//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleE01LinkDef.h
/// \brief The CINT link definitions for example E01 classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Ex01MCApplication + ;
#pragma link C++ class Ex01MCStack + ;
#pragma link C++ class Ex01DetectorConstructionOld + ;
#pragma link C++ class Ex01Particle + ;

#endif
