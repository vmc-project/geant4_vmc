// $Id: Ex03RunConfiguration.cxx,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration
// --------------------------
// See the class description in the header file.

#include "Ex03RunConfiguration.h"

#include <LHEP_BERT.hh>

//_____________________________________________________________________________
Ex03RunConfiguration::Ex03RunConfiguration()
  : TG4RunConfiguration(false) {
//
}

//_____________________________________________________________________________
Ex03RunConfiguration::~Ex03RunConfiguration(){
//
}

//
// protected methods
//


//_____________________________________________________________________________
G4VUserPhysicsList*  Ex03RunConfiguration::CreatePhysicsList()
{
// Create LHEP_BERT physics list

  return new LHEP_BERT();
}  
