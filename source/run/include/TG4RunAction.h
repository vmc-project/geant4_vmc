// $Id: TG4RunAction.h,v 1.3 2005/09/01 10:04:33 brun Exp $
/// \ingroup run
//
/// \class TG4RunAction
/// \brief Actions at the beginning and the end of run.
///
/// Author: I. Hrivnacova

#ifndef TG4_RUN_ACTION_H
#define TG4_RUN_ACTION_H

#include "TG4Verbose.h"

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4Timer;
    // in order to avoid the odd dependency for the
    // times system function this declaration must be the first
class G4Run;

class TG4RunAction : public G4UserRunAction,
                     public TG4Verbose
{
  public:
    TG4RunAction();
    virtual ~TG4RunAction();

    // methods
    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);

  private:
    TG4RunAction(const TG4RunAction& right);
    TG4RunAction& operator=(const TG4RunAction& right);

    // data members
    G4Timer*  fTimer; //G4Timer
    G4int     fRunID; //run ID
};

#endif //TG4_RUN_ACTION_H
