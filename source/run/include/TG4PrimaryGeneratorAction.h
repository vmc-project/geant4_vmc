// $Id: TG4PrimaryGeneratorAction.h,v 1.1 2002/06/20 11:58:25 hristov Exp $
// Category: run
//
// Author: I. Hrivnacova
//
// Class TG4PrimaryGeneratorAction
// -------------------------------
// Class that defines primary generator action
// using TVirtualMCStack and TVirtualMCApplication.

#ifndef TG4_PRIMARY_GENERATOR_ACTION_H
#define TG4_PRIMARY_GENERATOR_ACTION_H

#include "TG4Verbose.h"

#include <G4VUserPrimaryGeneratorAction.hh>
#include <globals.hh>

class G4ParticleGun;
class G4Event;

class TG4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction,
                                  public TG4Verbose
{
  public:
    TG4PrimaryGeneratorAction();
    virtual ~TG4PrimaryGeneratorAction();

    // methods
    virtual void GeneratePrimaries(G4Event* event);
    
  private:
    // methods
    void TransformPrimaries(G4Event* event);
};

#endif //TG4_PRIMARY_GENERATOR_ACTION_H


