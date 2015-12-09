#ifndef TG4_SPECIAL_PHYSICS_LIST_H
#define TG4_SPECIAL_PHYSICS_LIST_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4SpecialPhysicsList.h
/// \brief Definition of the TG4SpecialPhysicsList class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class TG4StackPopperPhysics;
class TG4EmModelPhysics;

/// \ingroup physics_list
/// \brief The Geant4 VMC special physics list helper class 
///
/// The special physics list instatiates the Geant4 VMC special processes 
/// according to the selection passed in the constructor  
///
/// \author I. Hrivnacova; IPN Orsay

class TG4SpecialPhysicsList: public G4VModularPhysicsList,
                             public TG4Verbose
{
  public:
    TG4SpecialPhysicsList(const G4String& selection);
    TG4SpecialPhysicsList();
    virtual ~TG4SpecialPhysicsList();
  
    // static methods
    static TG4SpecialPhysicsList* Instance();
    static G4String AvailableSelections();
    static G4bool   IsAvailableSelection(const G4String& selection);
  
    // methods
    virtual void ConstructProcess();

                  /// No cuts are set here
    virtual void SetCuts() {}

    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);

    // get methods
    G4bool IsSpecialCuts() const;
    
  protected:
    // data members
    TG4StackPopperPhysics* fStackPopperPhysics;///< stack popper physics
    TG4EmModelPhysics*     fEmModelPhysics;    ///< EM models physics
    G4bool                 fIsSpecialCuts;     ///< option for special cuts

  private:
    /// Not implemented
    TG4SpecialPhysicsList(const TG4SpecialPhysicsList& right);
    /// Not implemented
    TG4SpecialPhysicsList& operator=(const TG4SpecialPhysicsList& right);

    // methods
    void Configure(const G4String& selection);
    
    // static data members
    static G4ThreadLocal TG4SpecialPhysicsList*  fgInstance; ///< this instance
};

// inline methods

inline TG4SpecialPhysicsList* TG4SpecialPhysicsList::Instance() { 
  /// Return this instance
  return fgInstance; 
}  

inline G4bool TG4SpecialPhysicsList::IsSpecialCuts() const {
  /// Return true if specialCuts are selected
  return fIsSpecialCuts;
}  

#endif //TG4_SPECIAL_PHYSICS_LIST_H

