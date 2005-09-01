// $Id: TG4SpecialCuts.h,v 1.3 2005/03/29 10:39:53 brun Exp $
//
// Classes TG4pecialCutsFor*
//
// The specifications of TG4VSpecialCuts process class 
// for different particle types. They implement only 
// the GetMinEkine() abstract function.
//
// Author: I. Hrivnacova

#ifndef TG4_SPECIAL_CUTS_H
#define TG4_SPECIAL_CUTS_H

#include "TG4VSpecialCuts.h"

class TG4Limits;

class G4Track;

/// \ingroup physics
/// \class TG4SpecialCutsForChargedHadron
/// \brief Special process that activates kinetic energy cuts
/// for charged hadrons.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForChargedHadron: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForChargedHadron(const G4String& processName);
    virtual ~TG4SpecialCutsForChargedHadron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForChargedHadron();		   
};

/// \ingroup physics
/// \class TG4SpecialCutsForElectron
/// \brief Special process that activates kinetic energy cuts
/// for e-.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForElectron: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForElectron(const G4String& processName);
    virtual ~TG4SpecialCutsForElectron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForElectron();		   
};

/// \ingroup physics
/// \class TG4SpecialCutsForEplus
/// \brief Special process that activates kinetic energy cuts
/// for e+.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForEplus: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForEplus(const G4String& processName);
    virtual ~TG4SpecialCutsForEplus();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForEplus();		   
};

/// \ingroup physics
/// \class TG4SpecialCutsForGamma
/// \brief Special process that activates kinetic energy cuts
/// for gamma.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForGamma: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForGamma(const G4String& processName);
    virtual ~TG4SpecialCutsForGamma();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForGamma();		   
};

/// \ingroup physics
/// \class TG4SpecialCutsForMuon
/// \brief Special process that activates kinetic energy cuts
/// for muon.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForMuon: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForMuon(const G4String& processName);
    virtual ~TG4SpecialCutsForMuon();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForMuon();		   
};

/// \ingroup physics
/// \class TG4SpecialCutsForNeutralHadron
/// \brief Special process that activates kinetic energy cuts
/// for neutral hadrons.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForNeutralHadron: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForNeutralHadron(const G4String& processName);
    virtual ~TG4SpecialCutsForNeutralHadron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForNeutralHadron();		   
};

/// \ingroup physics
/// \class TG4SpecialCutsForOther
/// \brief Special process that activates the kinetic energy cuts
/// for other (than specified in TParticleWSP) particles.
///
/// Author: I. Hrivnacova

class TG4SpecialCutsForOther: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForOther(const G4String& processName);
    virtual ~TG4SpecialCutsForOther();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForOther();		   
};

#endif //TG4_SPECIAL_CUTS_H



