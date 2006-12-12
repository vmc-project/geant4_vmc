// $Id: TG4VPhysicsConstructor.h,v 1.4 2006/01/13 16:59:38 brun Exp $
/// \ingroup physics
//
/// \class TG4VPhysicsConstructor
/// \brief Abstract base class for physics constructors with verbose.
///
/// Author: I. Hrivnacova

#ifndef TG4_V_PHYSICS_CONSTRUCTOR_H
#define TG4_V_PHYSICS_CONSTRUCTOR_H

#include "TG4Verbose.h"

#include <G4VPhysicsConstructor.hh>
#include <globals.hh>

class TG4VPhysicsConstructor: public G4VPhysicsConstructor,
                              public TG4Verbose
{
  public:
    TG4VPhysicsConstructor(const G4String& name);
    TG4VPhysicsConstructor(const G4String& name, G4int verboseLevel);
    virtual ~TG4VPhysicsConstructor();

    // methods
          // construct particle and physics
    virtual void ConstructParticle() = 0;
    virtual void ConstructProcess() = 0;

  protected:
    // overridden verbose methods
    virtual void  VerboseLevel(G4int level);
    virtual G4int VerboseLevel() const;

  private:
    TG4VPhysicsConstructor();
    TG4VPhysicsConstructor(const TG4VPhysicsConstructor& right);

};

#endif //TG4_V_PHYSICS_CONSTRUCTOR_H

