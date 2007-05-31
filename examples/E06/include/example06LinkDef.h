// $Id: example06LinkDef.h,v 1.2 2006/12/12 16:21:39 brun Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  Ex06MCApplication+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  Ex06DetectorConstruction+;
#pragma link C++ class  Ex06DetectorConstructionOld+;
#pragma link C++ class  Ex06PrimaryGenerator+;

#endif





