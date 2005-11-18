// $Id: TG4Limits.h,v 1.5 2005/09/01 10:04:32 brun Exp $
/// \ingroup global
//
/// \class TG4Limits
/// \brief Extended G4UserLimits class.
/// 
/// Class extends G4UserLimits base class with the
/// vectors of kinetic energy cuts and control process flags
/// data members.
///
/// Author: I. Hrivnacova

#ifndef TG4_LIMITS_H
#define TG4_LIMITS_H

#include "TG4Globals.h"
#include "TG4G3Cut.h"
#include "TG4G3Control.h"
#include "TG4G3CutVector.h"
#include "TG4G3ControlVector.h"

#include <G4UserLimits.hh>

class G4VProcess;

class TG4Limits: public G4UserLimits
{
  public:
    TG4Limits(const TG4G3CutVector& cuts, 
              const TG4G3ControlVector& controls);
    TG4Limits(const G4String& name,
              const TG4G3CutVector& cuts, 
	      const TG4G3ControlVector& controls);
    TG4Limits(const G4UserLimits& g4Limits,
              const TG4G3CutVector& cuts, 
	      const TG4G3ControlVector& controls);
    TG4Limits(const TG4Limits& right);
    virtual ~TG4Limits();
    
    // operators
    TG4Limits& operator=(const TG4Limits& right);
    
    // static methods
    static G4int GetNofLimits();

    // set methods
    void SetName(const G4String& name);
    void SetG3Cut(TG4G3Cut cut, G4double cutValue);
    void SetG3Control(TG4G3Control control, TG4G3ControlValue controlValue);
    G4bool Update(const TG4G3ControlVector& controls);
    void SetG3DefaultCuts();
    void SetG3DefaultControls();
    
    // methods
    void Print() const;

    // get methods
    G4String GetName() const;
    G4double GetMaxUserStep() const;
    const TG4G3CutVector* GetCutVector() const;
    const TG4G3ControlVector* GetControlVector() const;
    G4bool  IsCut() const;
    G4bool  IsControl() const;
    virtual G4double GetUserMinEkine(const G4Track& track);
    G4double GetMinEkineForGamma(const G4Track& track) const;
    G4double GetMinEkineForElectron(const G4Track& track) const;
    G4double GetMinEkineForEplus(const G4Track& track) const;
    G4double GetMinEkineForChargedHadron(const G4Track& track) const;
    G4double GetMinEkineForNeutralHadron(const G4Track& track) const;
    G4double GetMinEkineForMuon(const G4Track& track) const;
    G4double GetMinEkineForOther(const G4Track& track) const;
    TG4G3ControlValue GetControl(G4VProcess* process) const; 

  protected:
    TG4Limits();

  private:
    // methods
    void Initialize(const TG4G3CutVector& cuts, 
                    const TG4G3ControlVector& controls);
  
    // static data members
    static G4int  fgCounter;           // counter 

    // data members
    G4String            fName;         //name
    G4bool              fIsCut;        //true if any cut value is set
    G4bool              fIsControl;    //true if any control value is set
    TG4G3CutVector      fCutVector;    //the vector of G3 cut values
    TG4G3ControlVector  fControlVector;//the vector of G3 control values 
};

// inline methods

inline G4int TG4Limits::GetNofLimits() { 
  /// Return the total number of all TG4Limits objects
  return fgCounter; 
}

inline G4bool TG4Limits::IsCut() const  { 
  /// Return true if any cut value is set
  return fIsCut; 
}

inline G4bool TG4Limits::IsControl() const  { 
  /// Return true if any control value is set
  return fIsControl; 
}

inline void TG4Limits::SetName(const G4String& name) { 
  /// Set name
  fName = name; 
}

inline G4String TG4Limits::GetName() const { 
  /// Return name
  return fName; 
}

inline G4double TG4Limits::GetMaxUserStep() const {
  /// Return max step
  return fMaxStep;
}

inline const TG4G3CutVector* TG4Limits::GetCutVector() const { 
  /// Return the vector of G3 cut values
  return &fCutVector; 
}

inline const TG4G3ControlVector* TG4Limits::GetControlVector() const { 
  /// Return the vector of G3 control values
  return &fControlVector; 
}

#endif //TG4_USER_LIMITS_H



