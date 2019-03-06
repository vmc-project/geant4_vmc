//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file exampleMonopoleLinkDef.h
/// \brief The CINT link definitions for Monopole example classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  VMC::Monopole::MCApplication+;
#pragma link C++ class  VMC::Monopole::DetectorConstruction+;

#endif





