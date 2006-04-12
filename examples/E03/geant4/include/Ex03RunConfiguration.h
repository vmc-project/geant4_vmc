// $Id: Ex03RunConfiguration.h,v 1.2 2006/01/13 16:58:58 brun Exp $
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
    virtual G4VUserPhysicsList*            CreatePhysicsList();

    //virtual G4VUserDetectorConstruction*   CreateDetectorConstruction();
    //virtual G4VUserPrimaryGeneratorAction* CreatePrimaryGenerator();
};

#endif //EX03_RUN_CONFIGURATION_H

