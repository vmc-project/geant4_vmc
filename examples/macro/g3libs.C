// $Id: $
//
// Macro for loading Geant3 libraries

#include <iostream>

void g3libs()
{
  cout << "Loading Geant3 library ..." << endl;

  gSystem->Load("libgeant321.so");

  cout << "Loading Geant3 library ... finished" << endl;
}
