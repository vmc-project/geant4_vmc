// $Id: TG4RunConfiguration.h,v 1.1 2002/06/20 11:58:25 hristov Exp $
// Category: run
//
// Author: I. Hrivnacova
//
// Class TG4RunConfiguration
// --------------------------
// This class takes care of creating all geant4 user defined mandatory
// and actions classes that will be initialized and managed by Geant4
// kernel (G4RunManager).
//
// The virtual method CreateUserConfiguration() instantiates
// the geant4 user defined classes implemented by TGeant4
// using the Virtual MC interfaces 
// (TVirtualMCApplication, TVirtualMCStack, TVirtualMCDecayer).
//
// The class can be extended in a user application by inheritence;
// this gives a user possibility to extend each geant4 user defined class.


#ifndef TG4_RUN_CONFIGURATION_H
#define TG4_RUN_CONFIGURATION_H

#include "TG4VRunConfiguration.h"

#include <Rtypes.h>

class TG4RunConfiguration : public TG4VRunConfiguration
{
  public:
    TG4RunConfiguration(Bool_t specialStacking = false);
    virtual ~TG4RunConfiguration();

  protected:
    // methods
    virtual void CreateUserConfiguration();

  private:
    // data members
    Bool_t  fSpecialStacking;
};

#endif //TG4_RUN_CONFIGURATION_H

