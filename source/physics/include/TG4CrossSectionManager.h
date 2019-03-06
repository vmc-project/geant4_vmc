#ifndef TG4_CROSS_SECTION_MANAGER_H
#define TG4_CROSS_SECTION_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CrossSectionManager.h
/// \brief Definition of the TG4CrossSectionManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4Verbose.h"
#include "TG4CrossSectionType.h"
#include "TG4CrossSectionMessenger.h"

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4ParticleDefinition;
class G4Element;

class TObjArray;

/// \ingroup physics
/// \brief The manager class for accessing hadronic cross sections.
///
/// The main methods:
/// -  MakeHistograms()
/// -  GetCrossSection(TG4CrossSectionType type)
/// -  PrintCrossSections()
/// -  PrintCrossSection(TG4CrossSectionType type)
///
/// The selection of the particle, the element and the histogram parameters
/// (energy and moment range and number of bins) has to be  done first via
/// the set methods of the class.
///
/// In MakeHistograms(), the array with the following histograms
/// for the selected parameters is filled and returned:
///  - Elastic cross section (barn) as a functions of log10(p/GeV)
///  - Elastic cross section (barn) as a functions of log10(E/MeV)
///  - Inelastic cross section (barn) as a functions of log10(p/GeV)
///  - Inelastic cross section (barn) as a functions of log10(E/MeV)
///  - Capture cross section (barn) as a functions of log10(E/MeV)
///  - Fission cross section (barn) as a functions of log10(E/MeV)
///  - Charge exchange cross section (barn) as a functions of log10(E/MeV)
///    (inactivated)
///
/// If verbose level is set >=1 then a table with cross section values
/// per energy values is printed.
///
/// It is also possible to access the cross section value for a selected energy
/// or a momentum via  GetCrossSection() method.
/// A selected cross section or all cross sections can be also printed via
/// PrintCrossSection[s] methods.
///
/// Implemented according to the Geant4 example: extended/hadronic/Hadr00.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4CrossSectionManager : public TG4Verbose
{
  public:
    TG4CrossSectionManager();
    virtual ~TG4CrossSectionManager();

    // methods
    TObjArray* MakeHistograms();

    G4double GetCrossSection(TG4CrossSectionType type) const;
    void PrintCrossSections() const;
    void PrintCrossSection(TG4CrossSectionType type) const;

    void SetParticleName(const G4String& name);
    void SetElementName(const G4String& name);

    void SetNumberOfBinsE(G4int val);
    void SetNumberOfBinsP(G4int val);

    void SetMinKinEnergy(G4double val);
    void SetMaxKinEnergy(G4double val);

    void SetMinMomentum(G4double val);
    void SetMaxMomentum(G4double val);

    void SetLabel(const G4String& label);

    void SetKinEnergy(G4double val);
    void SetMomentum(G4double val);

    void   SetMakeHistograms(G4bool makeHistograms);
    G4bool IsMakeHistograms() const;

  private:
    /// Not implemented
    TG4CrossSectionManager(const TG4CrossSectionManager& right);
    /// Not implemented
    TG4CrossSectionManager& operator=(const TG4CrossSectionManager& right);

    // methods
    const G4ParticleDefinition* GetParticle() const;
    const G4Element* GetElement() const;
    G4double GetMomentum() const;

    void CreateHistograms();
    void FillHistograms();

    // static data members
    static const G4String fgkDefaultParticleName; ///< default particle name
    static const G4String fgkDefaultElementName;  ///< default element name
    static const G4double fgkDefaultMinKinEnergy; ///< default minimum kinetic energy
    static const G4double fgkDefaultMaxKinEnergy; ///< default maximum kinetic energy
    static const G4double fgkDefaultMinMomentum;  ///< default minimum momentum
    static const G4double fgkDefaultMaxMomentum;  ///< default maximum momentum
    static const G4int    fgkDefaultNofBinsE;  ///< defualt number of bins in energy
    static const G4int    fgkDefaultNofBinsP;  ///< defualt number of bins in momentum
    static const G4double fgkDefaultKinEnergy; ///< defualt kinetic energy

    // data members
    TG4CrossSectionMessenger  fMessenger; ///< messenger
    TObjArray* fHistograms;   ///< array of histograms
    G4String   fParticleName; ///< particle name
    G4String   fElementName;  ///< element name
    G4double   fMinKinEnergy; ///< minimum kinetic energy (histogram range)
    G4double   fMaxKinEnergy; ///< maximum kinetic energy( histogram range)
    G4double   fMinMomentum;  ///< minimum momentum (histogram range)
    G4double   fMaxMomentum;  ///< maximum momentum (histogram range)
    G4int      fNofBinsE;     ///< number of bins in kinetic energy
    G4int      fNofBinsP;     ///< number of bins in momentum
    G4String   fLabel;        ///< the histogram label
    G4double   fKinEnergy;    ///< current kinetic energy
    G4bool     fIsInitialised;  ///< info if histograms are created
    G4bool     fMakeHistograms; ///< option to make histograms (for ecternal use)
};

// inline functions

inline void TG4CrossSectionManager::SetParticleName(const G4String& name) {
  /// Set the particle name
  fParticleName = name;
}

inline void TG4CrossSectionManager::SetElementName(const G4String& name) {
  /// Set the element name
  fElementName = name;
}

inline void TG4CrossSectionManager::SetMinKinEnergy(G4double val) {
  /// Set the minimum kinetic energy (histogram range)
  fMinKinEnergy = val;
}

inline void TG4CrossSectionManager::SetMaxKinEnergy(G4double val) {
  /// Set the maximum kinetic energy( histogram range)
  fMaxKinEnergy = val;
}

inline void TG4CrossSectionManager::SetMinMomentum(G4double val) {
  /// Set the minimum momentum (histogram range)
  fMinMomentum = val;
}

inline void TG4CrossSectionManager::SetMaxMomentum(G4double val) {
  /// Set the maximum momentum (histogram range)
  fMaxMomentum = val;
}

inline void TG4CrossSectionManager::SetNumberOfBinsE(G4int val) {
  /// Set the number of bins in kinetic energy
  fNofBinsE = val;
}

inline void TG4CrossSectionManager::SetNumberOfBinsP(G4int val) {
  /// Set the number of bins in momentum
  fNofBinsP = val;
}

inline void TG4CrossSectionManager::SetLabel(const G4String& label) {
  /// Set a label to the histogram.
  /// The label will be added at the beginning of the histogram title.
  fLabel = label;
}

inline void TG4CrossSectionManager::SetKinEnergy(G4double val) {
  /// Set the current kinetic energy
  fKinEnergy = val;
}

inline void TG4CrossSectionManager::SetMakeHistograms(G4bool makeHistograms) {
  /// Set the option to make histograms (for external use)
  fMakeHistograms = makeHistograms;
}

inline G4bool TG4CrossSectionManager::IsMakeHistograms() const {
  /// Return the option to make histograms (for external use)
  return fMakeHistograms;
}

#endif //TG4_CROSS_SECTION_MANAGER_H
