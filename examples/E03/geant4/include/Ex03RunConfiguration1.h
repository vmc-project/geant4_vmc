// $Id: Ex03RunConfiguration.h,v 1.3 2006/04/12 10:39:33 brun Exp $
//
// Author: I. Hrivnacova
//
// Class Ex03RunConfiguration
// --------------------------
// This class overrides setting of the default Geant4 VMC geometry
// construction with Geant4 detector construction

#ifndef EX03_RUN_CONFIGURATION1_H
#define EX03_RUN_CONFIGURATION1_H

#include "TG4RunConfiguration.h"

class Ex03RunConfiguration1 : public TG4RunConfiguration
{
  public:
    Ex03RunConfiguration1(const TString& physicsList = "emStandard",
                          const TString& specialProcess = "stepLimiter");
    virtual ~Ex03RunConfiguration1();

    // methods
    virtual G4VUserDetectorConstruction*   CreateDetectorConstruction();
};

#endif //EX03_RUN_CONFIGURATION1_H

