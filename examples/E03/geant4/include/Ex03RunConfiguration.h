// $Id: Ex03RunConfiguration.h,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration
// --------------------------
// This class overrides setting of the default Geant4 VMC physics list
// with the LHEP_BERT hadronics physics list

#ifndef EX03_RUN_CONFIGURATION_H
#define EX03_RUN_CONFIGURATION_H

#include "TG4RunConfiguration.h"

class Ex03RunConfiguration : public TG4RunConfiguration
{
  public:
    Ex03RunConfiguration();
    virtual ~Ex03RunConfiguration();

    // methods
    virtual G4VUserPhysicsList*  CreatePhysicsList();
};

#endif //EX03_RUN_CONFIGURATION_H

