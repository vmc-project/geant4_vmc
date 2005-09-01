// $Id: TG4SensitiveDetector.h,v 1.2 2004/11/10 11:39:27 brun Exp $
/// \ingroup digits_hits
//
/// \class TG4SensitiveDetector
/// \brief Sensitive detector common to all logical volumes
///
/// This class adds integer identifier data member to G4VSensitiveDetector.
/// It also takes care of setting step status (kBoundary, kNormalStep)
/// and passing G4Step to TG4StepManager and for calling a user application
/// stepping function.
///
/// Author: I. Hrivnacova

#ifndef TG4_SENSITIVE_DETECTOR_H
#define TG4_SENSITIVE_DETECTOR_H

#include <G4VSensitiveDetector.hh>
#include <globals.hh>

class TG4StepManager;

class TG4SensitiveDetector : public G4VSensitiveDetector
{
  public:
    TG4SensitiveDetector(G4String sdName);
    TG4SensitiveDetector(G4String sdName, G4int id);
    TG4SensitiveDetector(const TG4SensitiveDetector& right);
    // --> protected
    // TG4SensitiveDetector();
    virtual ~TG4SensitiveDetector();

    // operators
    TG4SensitiveDetector& operator=(const TG4SensitiveDetector &right);

    // methods
    virtual void UserProcessHits(const G4Track* track,const G4Step* step);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual G4bool ProcessHitsOnBoundary(G4Step* step);
 
    // static get method
    static G4int GetTotalNofSensitiveDetectors();

    // get methods
    G4int GetID() const;
    
  protected:
    TG4SensitiveDetector(); 

    // data members
    G4int  fID;                    //sensitive detector ID
    TG4StepManager*  fStepManager; //TG4StepManager
    
  private:          
    // data members
    static G4int  fgSDCounter; //sensitive detector counter
};

// inline methods

inline G4int TG4SensitiveDetector::GetTotalNofSensitiveDetectors() { 
  /// Returns the total number of sensitive detectors.
  return fgSDCounter; 
}

inline G4int TG4SensitiveDetector::GetID() const { 
  /// Returns sensitive detector ID.
  return fID; 
}

#endif //TG4V_SENSITIVE_DETECTOR_H


