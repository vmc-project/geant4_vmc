// $Id: $
//
// Author: I. Hrivnacova
//
// Class TG4TGeoUnits
// ------------------
// Class defines the TGeo default units of physical quantities.

#ifndef TG4_TGEO_UNITS_H
#define TG4_TGEO_UNITS_H

#include <globals.hh>

class TG4TGeoUnits
{
  public:
    // --> protected
    // TG4TGeoUnits();  
    virtual ~TG4TGeoUnits();

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
    TG4TGeoUnits();      
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

inline G4double TG4TGeoUnits::Length() { return fgkLength; }
inline G4double TG4TGeoUnits::Angle()  { return fgkAngle; }
inline G4double TG4TGeoUnits::Time()   { return fgkTime; }
inline G4double TG4TGeoUnits::Charge() { return fgkCharge; }
inline G4double TG4TGeoUnits::Energy() { return fgkEnergy; }
inline G4double TG4TGeoUnits::Mass()   { return fgkMass; }
inline G4double TG4TGeoUnits::MassDensity()  { return fgkMassDensity; }
inline G4double TG4TGeoUnits::AtomicWeight() { return fgkAtomicWeight; }
inline G4double TG4TGeoUnits::Field()  { return fgkField; }

#endif //TG4_TGEO_UNITS_H
