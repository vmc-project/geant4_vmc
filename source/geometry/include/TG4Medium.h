// $Id: TG4Medium.h,v 1.11 2006/04/12 10:37:23 brun Exp $
/// \ingroup geometry
//
/// \class TG4Medium
/// \brief Helper class to keep medium data
///
/// Author: I. Hrivnacova

#ifndef TG4_MEDIUM_H
#define TG4_MEDIUM_H

#include "globals.hh"

class G4UserLimits;
class G4Material;

class TG4Medium
{
  public:
    TG4Medium(G4int id);
    virtual ~TG4Medium();
    
    // methods
    void Print() const;
    
    // set methods
    void SetName(const G4String& name);
    void SetMaterial(G4Material* material);
    void SetLimits(G4UserLimits* limits);
    
    // get methods
    G4int         GetID() const;
    G4String      GetName() const;
    G4Material*   GetMaterial() const;
    G4UserLimits* GetLimits() const;
    
  private:
    TG4Medium();
    TG4Medium(const TG4Medium& right);
    TG4Medium& operator=(const TG4Medium& right);

    // static data members
    static const G4String  fgkUndefinedName; // the default (undefined) name
  
    // data members
    G4int         fID;       // medium ID
    G4String      fName;     // medium name
    G4Material*   fMaterial; // material associated with this medium
    G4UserLimits* fLimits;   // user limits associated with this medium
};

// inline functions

inline G4int TG4Medium::GetID() const
{ /// Return ID 
  return fID; 
}

inline G4String TG4Medium::GetName() const
{ /// Return name
  return fName; 
}

inline G4Material* TG4Medium::GetMaterial() const
{ /// Return material
  return fMaterial; 
}

inline G4UserLimits* TG4Medium::GetLimits() const
{ /// Return user limits
  return fLimits; 
}

#endif //TG4_MEDIUM_H



      
