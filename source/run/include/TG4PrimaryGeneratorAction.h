// $Id: TG4PrimaryGeneratorAction.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup run
//
/// \class TG4PrimaryGeneratorAction
/// 
/// Class that defines primary generator action
/// using TVirtualMCStack and TVirtualMCApplication.
///
/// Author: I. Hrivnacova

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


