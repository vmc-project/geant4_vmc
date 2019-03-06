#ifndef TG4_MEDIUM_H
#define TG4_MEDIUM_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Medium.h
/// \brief Definition of the TG4Medium class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "globals.hh"

class G4UserLimits;
class G4Material;

/// \ingroup geometry
/// \brief Helper class to keep medium data
///
/// \author I. Hrivnacova; IPN, Orsay

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
    void SetIfield(G4int ifield);

    // get methods
    G4int         GetID() const;
    G4String      GetName() const;
    G4Material*   GetMaterial() const;
    G4UserLimits* GetLimits() const;
    G4int         GetIfield() const;

  private:
    /// Not implemented
    TG4Medium();
    /// Not implemented
    TG4Medium(const TG4Medium& right);
    /// Not implemented
    TG4Medium& operator=(const TG4Medium& right);

    // static data members
    static const G4String  fgkUndefinedName; ///< the default (undefined) name
    static const G4int     fgkDefaultIfield; ///< the default ifield value

    // data members
    G4int         fID;       ///< medium ID
    G4String      fName;     ///< medium name
    G4Material*   fMaterial; ///< material associated with this medium
    G4UserLimits* fLimits;   ///< user limits associated with this medium

    /// G3 tracking medium parameter 'ifield'
    /// -  0  no magnetic field
    /// - -1  user decision in guswim
    /// -  1  tracking performed with Runge Kutta
    /// -  2  tracking performed with helix
    /// -  3  constant magnetic field along z
    G4int  fIfield;
};

// inline functions

inline void TG4Medium::SetIfield(G4int ifield)
{ /// Set G3 tracking medium parameter 'ifield'
  fIfield = ifield;
}

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

inline G4int  TG4Medium::GetIfield() const
{ /// Return G3 tracking medium parameter 'ifield'
  return fIfield;
}

#endif //TG4_MEDIUM_H
