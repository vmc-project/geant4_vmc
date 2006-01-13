// $Id: TG4SpecialCuts.h,v 1.4 2005/09/01 10:04:33 brun Exp $
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
    TG4SpecialCutsForChargedHadron(
       const G4String& processName = "specialCutForChargedHadron");
    virtual ~TG4SpecialCutsForChargedHadron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
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
    TG4SpecialCutsForElectron(
       const G4String& processName = "specialCutForElectron");
    virtual ~TG4SpecialCutsForElectron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
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
    TG4SpecialCutsForEplus(
      const G4String& processName = "specialCutForEplus");
    virtual ~TG4SpecialCutsForEplus();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
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
    TG4SpecialCutsForGamma(
       const G4String& processName = "specialCutForGamma");
    virtual ~TG4SpecialCutsForGamma();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
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
    TG4SpecialCutsForMuon(
      const G4String& processName = "specialCutForMuon");
    virtual ~TG4SpecialCutsForMuon();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
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
    TG4SpecialCutsForNeutralHadron(
       const G4String& processName = "specialCutForNeutralHadron");
    virtual ~TG4SpecialCutsForNeutralHadron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
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
    TG4SpecialCutsForOther(
       const G4String& processName = "specialCutForOther");
    virtual ~TG4SpecialCutsForOther();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
};

#endif //TG4_SPECIAL_CUTS_H



