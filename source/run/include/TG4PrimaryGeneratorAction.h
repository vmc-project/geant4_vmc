// $Id: TG4PrimaryGeneratorAction.h,v 1.2 2004/11/10 11:39:28 brun Exp $
/// \ingroup run
//
/// \class TG4PrimaryGeneratorAction
/// \brief Primary generator action defined via TVirtualMCStack 
/// and TVirtualMCApplication.
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


