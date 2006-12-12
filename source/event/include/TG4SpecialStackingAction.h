// $Id: TG4SpecialStackingAction.h,v 1.3 2005/09/01 10:04:32 brun Exp $
/// \ingroup event
//
/// \class TG4SpecialStackingAction
/// \brief Defines a special stacking mechanism 
/// 
/// Class that defines a special stacking mechanism,
/// with which particles are popped from the stack in "families".
/// The primary particles are first reclassified into
/// the postpone stack, a new primary is transfered to the urgent 
/// stack only after all secondaries of its precedent primary
/// are tracked.                                                             \n                      
/// The secondary particles are classified into the waiting stack,
/// which is transfered to the urgent stack when the urgent stack
/// is empty.                                                                \n
/// This ensures that all daughters of any particle are tracked
/// subsequently and get successive track IDs:                               \n
/// n, n+1, n+2, n+3, ..., n+m  
///
/// Author: I. Hrivnacova

#ifndef TG4_STACKING_ACTION_H
#define TG4_STACKING_ACTION_H

#include "TG4Verbose.h"

#include <G4UserStackingAction.hh>
#include <globals.hh>

class TG4TrackingAction;

class G4Track;
class G4TrackStack;

class TG4SpecialStackingAction : public G4UserStackingAction,
                                 public TG4Verbose
{
  public:
    TG4SpecialStackingAction();
    virtual ~TG4SpecialStackingAction();

    // methods
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track);
    void NewStage();
    void PrepareNewEvent();

  private:
    TG4SpecialStackingAction(const TG4SpecialStackingAction& right);
    TG4SpecialStackingAction& operator=(const TG4SpecialStackingAction& right);

    // data members
    G4int               fStage;          //stage number
    TG4TrackingAction*  fTrackingAction; //the tracking action
};

#endif //TG4_STACKING_ACTION_H

