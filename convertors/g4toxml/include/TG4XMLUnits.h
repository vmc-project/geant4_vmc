// $Id: TG4XMLUnits.h,v 1.1 2003/01/29 11:27:38 brun Exp $
//
// Author: I. Hrivnacova
//
// Class TG4XMLUnits
// -----------------
// Class defines the XML default units of physical quantities.

#ifndef TG4_XML_UNITS_H
#define TG4_XML_UNITS_H

#include <globals.hh>

class TG4XMLUnits
{
  public:
    // --> protected
    // TG4XMLUnits();  
    virtual ~TG4XMLUnits();

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
    TG4XMLUnits();      
        // only static data members and methods

  private:
    // static data members  
    static const G4double fgkLength;       //TGeo length unit 
    static const G4double fgkAngle;        //TGeo angle unit 
    static const G4double fgkTime;         //TGeo time unit 
    static const G4double fgkCharge;       //TGeo charge unit  
    static const G4double fgkEnergy;       //TGeo energy unit  
    static const G4double fgkMass;         //TGeo mass unit
    static const G4double fgkMassDensity;  //TGeo mass density unit 
    static const G4double fgkAtomicWeight; //TGeo atomic weight unit  
    static const G4double fgkField;        //TGeo magnetic field unit 
};     

// inline methods

inline G4double TG4XMLUnits::Length() { return fgkLength; }
inline G4double TG4XMLUnits::Angle()  { return fgkAngle; }
inline G4double TG4XMLUnits::Time()   { return fgkTime; }
inline G4double TG4XMLUnits::Charge() { return fgkCharge; }
inline G4double TG4XMLUnits::Energy() { return fgkEnergy; }
inline G4double TG4XMLUnits::Mass()   { return fgkMass; }
inline G4double TG4XMLUnits::MassDensity()  { return fgkMassDensity; }
inline G4double TG4XMLUnits::AtomicWeight() { return fgkAtomicWeight; }
inline G4double TG4XMLUnits::Field()  { return fgkField; }

#endif //TG4_XML_UNITS_H
