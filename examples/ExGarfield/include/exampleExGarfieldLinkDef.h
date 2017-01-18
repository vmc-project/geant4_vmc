//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  exampleExGarfieldLinkDef.h
/// \brief The CINT link definitions for ExGarfield example classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  VMC::ExGarfield::MCApplication+;
#pragma link C++ class  VMC::ExGarfield::DetectorConstruction+;
#pragma link C++ class  VMC::ExGarfield::Hit+;
#pragma link C++ class  VMC::ExGarfield::SensitiveDetector+;
#pragma link C++ class  VMC::ExGarfield::PrimaryGenerator+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  std::stack<TParticle*,deque<TParticle*> >+;

#endif





