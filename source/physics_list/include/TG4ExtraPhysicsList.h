#ifndef TG4_EXTRA_PHYSICS_LIST_H
#define TG4_EXTRA_PHYSICS_LIST_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2011 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ExtraPhysicsList.h
/// \brief Definition of the TG4ExtraPhysicsList class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

/// \ingroup physics_list
/// \brief The physics list with extra physics builders
///
/// The physics list is implemented as modular physics list
/// with the following physics builders: 
/// G4EmExtraPhysics,  G4OpticalPhysics and G4RadioactiveDecayPhysics
///
/// The builders are activated according to the user selection.
/// The EM extra physics processes are not activated by default.
/// If extra builder is selected, they can be then switched on using 
/// the following Geant4 commands:
/// - /physics_engine/tailor/SyncRadiation on
/// - /physics_engine/tailor/GammaNuclear on
/// - /physics_engine/tailor/MuonNuclear on
///
/// \author I. Hrivnacova; IPN Orsay

class TG4ExtraPhysicsList: public G4VModularPhysicsList,
                           public TG4Verbose
{
  public:
    TG4ExtraPhysicsList(const G4String& selection);
    virtual ~TG4ExtraPhysicsList();
  
    // static methods
    static G4String AvailableSelections();
    static G4bool   IsAvailableSelection(const G4String& selection);
 
    // methods
    virtual void  ConstructProcess();

                  /// No cuts are set here
    virtual void  SetCuts() {}
    
    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);

    // set methods
    void SetRangeCut(G4double value);
    
  protected:
    // static data members
    static const G4double  fgkDefaultCutValue; ///< default cut value

  private:
    /// Not implemented
    TG4ExtraPhysicsList(const TG4ExtraPhysicsList& right);
    /// Not implemented
    TG4ExtraPhysicsList& operator=(const TG4ExtraPhysicsList& right);

    // methods
    void Configure(const G4String& /*selection*/);
};

#endif //TG4_EXTRA_PHYSICS_LIST_H

