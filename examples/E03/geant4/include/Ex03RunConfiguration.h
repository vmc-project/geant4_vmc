// $Id: $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration
// --------------------------
// This class takes care of creating all geant4 user defined mandatory
// and actions classes that will be initialized and managed by Geant4
// kernel (G4RunManager).
// In difference from the default Geant4 VMC physics list
// (TG4ModularPhysicsList), the LHEP_BERT hadronics physics list.

#ifndef EX03_RUN_CONFIGURATION_H
#define EX03_RUN_CONFIGURATION_H

#include "TG4VRunConfiguration.h"

class Ex03RunConfiguration : public TG4VRunConfiguration
{
  public:
    Ex03RunConfiguration();
    virtual ~Ex03RunConfiguration();

  protected:
    // methods
    virtual void CreateUserConfiguration();
    virtual G4bool IsSpecialStacking() const;
};

#endif //EX03_RUN_CONFIGURATION_H

