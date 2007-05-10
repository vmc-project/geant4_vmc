// $Id: TG4StackPopper.h,v 1.5 2006/12/12 16:21:16 brun Exp $
/// \ingroup physics
//
/// \class TG4StackPopper
/// \brief The process which pops particles defined by user from 
///        the VMC stack and pass them to tracking  
///
/// Author: I. Hrivnacova

#ifndef TG4_STACK_POPPER_H
#define TG4_STACK_POPPER_H

#include <G4VContinuousProcess.hh>

class G4Track;

class TG4StackPopper: public G4VContinuousProcess
{
  public:
    TG4StackPopper(const G4String& processName = "stackPopper");
    virtual ~TG4StackPopper();
    
    // static access method
    static TG4StackPopper* Instance();

    // methods
    G4bool IsApplicable(const G4ParticleDefinition& /*particleDefinition*/); 

    G4double GetContinuousStepLimit(const G4Track& /*aTrack*/,
                                    G4double  previousStepSize ,
                                    G4double  currentMinimumStep ,
                                    G4double& currentSafety);

    virtual G4VParticleChange* AlongStepDoIt(
                                    const G4Track& aTrack, 
			            const G4Step&  aStep);                         
    void Notify();                         
    void Reset();                         

  private:
    TG4StackPopper(const TG4StackPopper& right);
    TG4StackPopper& operator = (const TG4StackPopper& right);
    
    // static data members
    static TG4StackPopper*  fgInstance; //this instance

    G4int fNofDoneTracks;
};

// inline methods

inline TG4StackPopper* TG4StackPopper::Instance() { 
  /// Return this instance.
  return fgInstance; 
}

inline G4bool TG4StackPopper::IsApplicable(
                   const G4ParticleDefinition& /*particleDefinition*/) {
  /// Applicable to any particles
  return true;
}
   
inline G4double TG4StackPopper::GetContinuousStepLimit(
                   const G4Track& /*aTrack*/,
                   G4double  /*previousStepSize*/,
                   G4double  /*currentMinimumStep*/,
                   G4double& /*currentSafety*/) {
  /// Does not limit step                   
  return DBL_MAX; 
}

#endif //TG4_STACK_POPPER_H



