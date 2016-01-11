#ifndef TG4_ACTION_INITIALIZATION_H
#define TG4_ACTION_INITIALIZATION_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4ActionInitialization.h
/// \brief Definition of the TG4ActionInitialization class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4VUserActionInitialization.hh>
#include <G4Threading.hh>

class TG4RunConfiguration;
class TG4TrackingAction;
class TG4SteppingAction;

class G4UserEventAction;
class TG4UserStackingAction;

/// \ingroup run
/// \brief Action Initialization class (required for MT mode)
///
/// Besides all user action classes it instantiates also their
/// messenger classes to make their associated commands available in
/// PreInit phase (as in MT mode creation of user action classes
/// is delayed.)
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
    
    //  data members
    TG4RunConfiguration*  fRunConfiguration; ///< run configuration

    G4UserEventAction*    fEventAction;    ///< event action
    TG4TrackingAction*    fTrackingAction; ///< tracking action
    TG4SteppingAction*    fSteppingAction; ///< steping action
    G4UserStackingAction* fStackingAction; ///< stacking action
};

#endif //TG4_ACTION_INITIALIZATION_H

