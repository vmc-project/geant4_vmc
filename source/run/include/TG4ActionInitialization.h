#ifndef TG4_ACTION_INITIALIZATION_H
#define TG4_ACTION_INITIALIZATION_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ActionInitialization.h
/// \brief Definition of the TG4ActionInitialization class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4VUserActionInitialization.hh>
#include <G4Threading.hh>

class TG4RunConfiguration;
class TG4SpecialControlsV2;

/// \ingroup run
/// \brief 
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4ActionInitialization : public G4VUserActionInitialization
{
  public:
    TG4ActionInitialization(TG4RunConfiguration* runConfiguration);
    virtual ~TG4ActionInitialization();

    // methods 
    //
    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    /// Not implemented
    TG4ActionInitialization();
    /// Not implemented
    TG4ActionInitialization(const TG4ActionInitialization& right);
    /// Not implemented
    TG4ActionInitialization& operator=(const TG4ActionInitialization& right);
    
    //  static/thread local data members
    /// special controls manager
    static G4ThreadLocal TG4SpecialControlsV2* fgSpecialControls; 

    //  data members
    TG4RunConfiguration*  fRunConfiguration; ///< run configuration
};

#endif //TG4_ACTION_INITIALIZATION_H

