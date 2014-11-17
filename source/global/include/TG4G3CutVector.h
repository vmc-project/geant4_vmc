#ifndef TG4_G3_CUT_VECTOR_H
#define TG4_G3_CUT_VECTOR_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4G3CutVector.h
/// \brief Definition of the TG4G3CutVector class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Globals.h"
#include "TG4G3Cut.h"

class G4Track;

/// \ingroup global
/// \brief Vector of kinetic energy cut values with
/// convenient set/get methods.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4G3CutVector
{
  public:
    TG4G3CutVector();
    TG4G3CutVector(const TG4G3CutVector& right);
    virtual ~TG4G3CutVector();
    
    // operators
    TG4G3CutVector& operator=(const TG4G3CutVector& right);
    G4double operator[](G4int index) const;
    
    // static methods
    static G4double Tolerance();
    static TG4G3Cut GetCut(const G4String& cutName);
    static G4bool   CheckCutValue(TG4G3Cut cut, G4double value);
    static const G4String& GetCutName(TG4G3Cut cut);
    
    // set methods
    void SetCut(TG4G3Cut cut, G4double cutValue);
    void SetG3Defaults();
    G4bool Update(const TG4G3CutVector& vector);
    void SetDeltaRaysOn(G4bool value);
    
    // methods
    G4String Format() const;
    void Print() const;

    // get methods
    G4double GetMinEkineForGamma(const G4Track& track) const;
    G4double GetMinEkineForElectron(const G4Track& track) const;
    G4double GetMinEkineForEplus(const G4Track& track) const;
    G4double GetMinEkineForChargedHadron(const G4Track& track) const;
    G4double GetMinEkineForNeutralHadron(const G4Track& track) const;
    G4double GetMinEkineForMuon(const G4Track& track) const;
    G4double GetMinEkineForOther(const G4Track& track) const;
    G4double GetMinEtotPair() const;
    G4bool IsCut() const;

  private:
    // static methods 
    static void FillCutNameVector();
  
    //
    // static data members
    
    /// cut for delta rays by e- (if off)
    static const G4double   fgkDCUTEOff;
    
    /// cut for delta rays by mu (if off)
    static const G4double   fgkDCUTMOff;
    
    /// tolerance for comparing cuts
    static const G4double   fgkTolerance;
    
    /// vector of cut parameters names
    static TG4StringVector  fgCutNameVector;
  
    //
    // data members
    
    /// vector of kinetic energy cut values 
    TG4doubleVector  fCutVector;
    
    /// delta rays process control
    G4bool           fDeltaRaysOn;
};

// inline methods

inline G4double TG4G3CutVector::Tolerance() { 
  /// Return tolerance for comparing cuts
  return fgkTolerance; 
}

inline void TG4G3CutVector::SetDeltaRaysOn(G4bool value) { 
  /// Return delta rays process control
  fDeltaRaysOn = value; 
}

#endif //TG4_CUT_VECTOR_H



