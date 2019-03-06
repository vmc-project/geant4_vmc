#ifndef TG4_CROSS_SECTION_MESSENGER_H
#define TG4_CROSS_SECTION_MESSENGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4CrossSectionMessenger.h
/// \brief Definition of the TG4CrossSectionMessenger class
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4CrossSectionManager;

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

/// \ingroup physics
/// \brief Messenger class that defines commands for TG4CrossSectionManager
///
/// Implements commands:
/// - /mcCrossSection/makeHistograms  true|false
/// - /mcCrossSection/setParticle  particleName
/// - /mcCrossSection/setElement   elementName
/// - /mcCrossSection/setNofBinsE  number
/// - /mcCrossSection/setNofBinsP  number
/// - /mcCrossSection/setMinKinE   value unit
/// - /mcCrossSection/setMaxKinE   valueunit
/// - /mcCrossSection/setKinE      value unit
/// - /mcCrossSection/setMinMomentum  value unit
/// - /mcCrossSection/setMaxMomentum  value unit
/// - /mcCrossSection/setMomentum     value unit
/// - /mcCrossSection/setLabel     label
/// - /mcCrossSection/printCrossSection crossSectionType
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4CrossSectionMessenger: public G4UImessenger
{
  public:
    TG4CrossSectionMessenger(TG4CrossSectionManager* crossSectionManager);
    virtual ~TG4CrossSectionMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String string);

  private:
    /// Not implemented
    TG4CrossSectionMessenger();
    /// Not implemented
    TG4CrossSectionMessenger(const TG4CrossSectionMessenger& right);
    /// Not implemented
    TG4CrossSectionMessenger& operator=(const TG4CrossSectionMessenger& right);

    // data members
    TG4CrossSectionManager*  fCrossSectionManager; ///< associated class
    G4UIdirectory*  fDirectory;  ///< command directory

    G4UIcmdWithABool*           fMakeHistogramsCmd; ///< command: makeHistograms
    G4UIcmdWithAString*         fParticleCmd;   ///< command: setParticle
    G4UIcmdWithAString*         fElementCmd;    ///< command: setElement
    G4UIcmdWithAnInteger*       fNofBinsECmd;   ///< command: setNofBinsE
    G4UIcmdWithAnInteger*       fNofBinsPCmd;   ///< command: setNofBinsP
    G4UIcmdWithADoubleAndUnit*  fMinKinECmd;    ///< command: setMinKinE
    G4UIcmdWithADoubleAndUnit*  fMaxKinECmd;    ///< command: setMaxKinE
    G4UIcmdWithADoubleAndUnit*  fKinECmd;       ///< command: setMinKinE
    G4UIcmdWithADoubleAndUnit*  fMinMomentumCmd;///< command: setMinMomentum
    G4UIcmdWithADoubleAndUnit*  fMaxMomentumCmd;///< command: setMaxMomentum
    G4UIcmdWithADoubleAndUnit*  fMomentumCmd;   ///< command: setMomentum
    G4UIcmdWithAString*         fLabelCmd;      ///< command: setLabel
    G4UIcmdWithAString*         fPrintCmd;      ///< command: printCrossSection
};

#endif //TG4_CROSS_SECTION_MESSENGER_H
