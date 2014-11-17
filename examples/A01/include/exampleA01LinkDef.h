//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  exampleA01LinkDef.h
/// \brief The CINT link definitions for example E03 classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  A01MCApplication+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  A01MagField+;
#pragma link C++ class  A01DriftChamberHit+;
#pragma link C++ class  A01EmCalorHit+;
#pragma link C++ class  A01HadCalorHit+;
#pragma link C++ class  A01HodoscopeHit+;
#pragma link C++ class  A01DriftChamberSD+;
#pragma link C++ class  A01EmCalorimeterSD+;
#pragma link C++ class  A01HadCalorimeterSD+;
#pragma link C++ class  A01HodoscopeSD+;
#pragma link C++ class  A01PrimaryGenerator+;
#pragma link C++ class  A01RootDetectorConstruction+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  std::stack<TParticle*,deque<TParticle*> >+;

#endif





