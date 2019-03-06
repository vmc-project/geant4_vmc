#ifndef TG4_VGM_MESSENGER_H
#define TG4_VGM_MESSENGER_H

#ifdef USE_VGM

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VGMMessenger.h
/// \brief Definition of the TG4VGMMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#include <XmlVGM/VExporter.h>
#include <VGM/volumes/IFactory.h>
#include <Geant4GM/volumes/Factory.h>
#include <RootGM/volumes/Factory.h>

#include <G4UImessenger.hh>
#include <globals.hh>

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

/// \ingroup geometry
/// \brief Messenger class that defines commands for VGM XML Exporter
///
/// Implements commands:
/// - /vgm/generateRoot
/// - /vgm/generateAGDD [lvName]
/// - /vgm/generateGDML [lvName]
/// - /vgm/setAssembliesInNames  bool
/// - /vgm/setNameSeparator bool
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4VGMMessenger: public G4UImessenger
{
  public:
    TG4VGMMessenger(const G4String& xmlFormat, const G4String& userGeometry);
    virtual ~TG4VGMMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);

  private:
    /// Not implemented
    TG4VGMMessenger();
    /// Not implemented
    TG4VGMMessenger(const TG4VGMMessenger& right);
    /// Not implemented
    TG4VGMMessenger& operator=(const TG4VGMMessenger &right);

    //
    // static data members

    /// command directory
    static G4UIdirectory*  fgDirectory;

    ///  command: /xml/generateRoot
    static G4UIcmdWithoutParameter* fgGenerateRootCmd;

    /// instances counter
    static G4int           fgCounter;

    // methods
    void CreateVGMExporter();

    // data members
    G4String               fGeometryInput;  ///< The geometry input type (Root/Geant4)
    G4String               fXmlFormat;      ///< The XML format
    VGM::IFactory*         fImportFactory;  ///< VGM factory for import
    Geant4GM::Factory*     fG4Factory;      ///< Geant4 VGM Factory
    RootGM::Factory*       fRootFactory;    ///< Root VGM Factory
    XmlVGM::VExporter*     fXmlVGMExporter; ///< associated class
    G4UIcmdWithAString*    fGenerateXMLCmd; ///< command: /vgm/generateXXX
    G4UIcmdWithAnInteger*  fSetXMLNumWidthCmd; ///< command: /vgm/setXMLNumWidth
    G4UIcmdWithAnInteger*  fSetXMLNumPrecisionCmd; ///< command: /vgm/setXMLNumPrecision
    G4UIcmdWithABool*      fSetAssembliesInNamesCmd; ///< command: /xml/setAssembliesInNames
    G4UIcmdWithAString*    fSetNameSeparatorCmd;     ///< command: /xml/setNameSeparator
};

#endif //TG4_XML_VGM_MESSENGER_H

#endif //USE_VGM

