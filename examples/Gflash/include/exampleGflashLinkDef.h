//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  example03LinkDef.h
/// \brief The CINT link definitions for example Gflash classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  Gflash::MCApplication+;
#pragma link C++ class  Gflash::DetectorConstruction+;
#pragma link C++ class  Gflash::Hit+;
#pragma link C++ class  Gflash::SensitiveDetector+;
#pragma link C++ class  Gflash::PrimaryGenerator+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  std::stack<TParticle*,deque<TParticle*> >+;

#endif





