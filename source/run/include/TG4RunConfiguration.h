// $Id: TG4RunConfiguration.h,v 1.2 2004/08/17 08:52:23 brun Exp $
/// \ingroup run
//
/// \class TG4RunConfiguration
/// 
/// This class takes care of creating all Geant4 user defined mandatory
/// and action classes that will be initialized and managed by Geant4
/// kernel (G4RunManager).
///
/// The virtual method CreateUserConfiguration() instantiates
/// the geant4 user defined classes implemented by TGeant4
/// using the VMC interfaces 
/// (TVirtualMCApplication, TVirtualMCStack, TVirtualMCDecayer).
///
/// The class can be extended in a user application by inheritence;
/// this gives a user possibility to extend each Geant4 user defined class.
///
/// Author: I. Hrivnacova


#ifndef TG4_RUN_CONFIGURATION_H
#define TG4_RUN_CONFIGURATION_H

#include "TG4VRunConfiguration.h"

#include <Rtypes.h>

class TG4RunConfiguration : public TG4VRunConfiguration
{
  public:
    TG4RunConfiguration(Bool_t specialStacking = false);
    virtual ~TG4RunConfiguration();

    // get methods
    virtual G4bool IsSpecialStacking() const;

  protected:
    // methods
    virtual void CreateUserConfiguration();

  private:
    // data members
    Bool_t  fSpecialStacking;
};

// inline functions

inline G4bool TG4RunConfiguration::IsSpecialStacking() const { 
  /// Return true if special stacking action is activated 
  return fSpecialStacking; 
}

#endif //TG4_RUN_CONFIGURATION_H

