//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  exampleGarfieldLinkDef.h
/// \brief The CINT link definitions for Garfield example classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  VMC::Garfield::MCApplication+;
#pragma link C++ class  VMC::Garfield::DetectorConstruction+;
#pragma link C++ class  VMC::Garfield::Hit+;
#pragma link C++ class  VMC::Garfield::SensitiveDetector+;
#pragma link C++ class  VMC::Garfield::PrimaryGenerator+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  std::stack<TParticle*,deque<TParticle*> >+;

#endif





