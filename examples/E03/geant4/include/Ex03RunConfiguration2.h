// $Id: Ex03RunConfiguration2.h,v 1.3 2006/04/12 10:39:33 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration2
// ----------------------------
// This class overrides setting of the default Geant4 VMC physics list
// with the LHEP_BERT hadronics physics list

#ifndef EX03_RUN_CONFIGURATION2_H
#define EX03_RUN_CONFIGURATION2_H

#include "TG4RunConfiguration.h"

class Ex03RunConfiguration2 : public TG4RunConfiguration
{
  public:
    Ex03RunConfiguration2(const TString& userGeometry);
    virtual ~Ex03RunConfiguration2();

    // methods
    virtual G4VUserPhysicsList*  CreatePhysicsList();

    //virtual G4VUserDetectorConstruction*   CreateDetectorConstruction();
    //virtual G4VUserPrimaryGeneratorAction* CreatePrimaryGenerator();
};

#endif //EX03_RUN_CONFIGURATION2_H

