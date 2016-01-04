//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/include/ example03LinkDef.h
/// \brief The CINT link definitions for example TR classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  VMC::TR::SensitiveDetector+;
#pragma link C++ class  VMC::TR::DetectorConstruction+;
#pragma link C++ class  VMC::TR::MCApplication+;
#pragma link C++ class  VMC::TR::PrimaryGenerator+;
#pragma link C++ class  std::stack<TParticle*,deque<TParticle*> >+;

#endif





