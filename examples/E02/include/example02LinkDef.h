// $Id: example02LinkDef.h,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  Ex02MCApplication+;
#pragma link C++ class  Ex02MCStack+;
#pragma link C++ class  Ex02Particle+;
#pragma link C++ class  Ex02ChamberParameterisation+;
#pragma link C++ class  Ex02DetectorConstruction+;
#pragma link C++ class  Ex02DetectorConstructionOld+;
#pragma link C++ class  Ex02TrackerHit+;
#pragma link C++ class  Ex02TrackerSD+;
#pragma link C++ class  Ex02RootManager+;

#pragma link C++ enum   FileMode;

#endif





