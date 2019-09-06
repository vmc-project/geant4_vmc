//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  exampleE03cLinkDef.h
/// \brief The CINT link definitions for example E03 classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Ex03MCApplication + ;
#pragma link C++ class Ex03MCStack + ;
#pragma link C++ class Ex03DetectorConstruction + ;
#pragma link C++ class Ex03CalorHit + ;
#pragma link C++ class Ex03CalorimeterSD + ;
#pragma link C++ class Ex03PrimaryGenerator + ;
#pragma link C++ class std::stack < TParticle*, deque < TParticle*>> + ;

#endif
