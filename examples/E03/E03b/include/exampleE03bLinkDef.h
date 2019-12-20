//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  exampleE03LinkDef.h
/// \brief The CINT link definitions for example E03 classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class Ex03bMCApplication + ;
#pragma link C++ typedef Ex03MCApplication;
#pragma link C++ class Ex03MCStack + ;
#pragma link C++ class Ex03DetectorConstruction + ;
#pragma link C++ class Ex03DetectorConstructionOld + ;
#pragma link C++ class Ex03CalorHit + ;
#pragma link C++ class Ex03bCalorimeterSD + ;
#pragma link C++ class Ex03PrimaryGenerator + ;
#pragma link C++ class std::stack < TParticle*, deque < TParticle*>> + ;

#endif
