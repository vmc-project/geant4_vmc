// $Id: TG4G3Units.h,v 1.2 2004/11/10 11:39:28 brun Exp $
/// \ingroup global
//
/// \class TG4G3Units
/// \brief G3 default units of physical quantities; 
///
/// All physical quantities returned by VMC are expressed in these units.
///
/// Author: I. Hrivnacova

#ifndef TG4_G3_UNITS_H
#define TG4_G3_UNITS_H

#include <globals.hh>

class TG4G3Units
{
  public:
    // --> protected
    // TG4G3Units();  
    virtual ~TG4G3Units();

    // static get methods
    static G4double Length(); 
    static G4double Angle(); 
    static G4double Time(); 
    static G4double Charge(); 
    static G4double Energy(); 
    static G4double Mass(); 
    static G4double MassDensity(); 
    static G4double AtomicWeight();     
    static G4double Field(); 
      
  protected:
    TG4G3Units();      
        // only static data members and methods

  private:
    // static data members  
    static const G4double fgkLength;       //G3 length unit 
    static const G4double fgkAngle;        //G3 angle unit 
    static const G4double fgkTime;         //G3 time unit 
    static const G4double fgkCharge;       //G3 charge unit  
    static const G4double fgkEnergy;       //G3 energy unit  
    static const G4double fgkMass;         //G3 mass unit
    static const G4double fgkMassDensity;  //G3 mass density unit 
    static const G4double fgkAtomicWeight; //G3 atomic weight unit  
    static const G4double fgkField;        //G3 magnetic field unit 
};     

// inline methods

inline G4double TG4G3Units::Length() { 
  /// Return G3 length unit
  return fgkLength; 
}

inline G4double TG4G3Units::Angle() { 
  /// Return G3 angle unit
  return fgkAngle; 
}

inline G4double TG4G3Units::Time() { 
  /// Return G3 time unit
  return fgkTime; 
}

inline G4double TG4G3Units::Charge() { 
  /// Return G3 chanrge unit
  return fgkCharge; 
}

inline G4double TG4G3Units::Energy() { 
  /// Return G3 energy unit
  return fgkEnergy; 
}

inline G4double TG4G3Units::Mass() { 
  /// Return G3 mass unit
  return fgkMass; 
}

inline G4double TG4G3Units::MassDensity()  
{ 
  /// Return G3 mass density unit
  return fgkMassDensity; 
}

inline G4double TG4G3Units::AtomicWeight() { 
  /// Return G3 atomic weight unit
  return fgkAtomicWeight; 
}

inline G4double TG4G3Units::Field() 
{ 
  /// Return G3 magnetic field unit
  return fgkField; 
}

#endif //TG4_G3_UNITS_H
