// $Id: TG4VGMMessenger.h,v 1.2 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4VGMMessenger
/// \brief Messenger class that defines commands for VGM XML Exporter
///
/// Implements commands:
/// - /vgm/generateRoot
/// - /vgm/generateAGDD [lvName]
/// - /vgm/generateGDML [lvName]
///
/// \author I. Hrivnacova; IPN, Orsay

#ifdef USE_VGM

#ifndef TG4_VGM_MESSENGER_H
#define TG4_VGM_MESSENGER_H

#include <XmlVGM/VExporter.h>
#include <Geant4GM/volumes/Factory.h>
#include <RootGM/volumes/Factory.h>

#include <G4UImessenger.hh>
#include <globals.hh>

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

class TG4VGMMessenger: public G4UImessenger
{
  public:
    TG4VGMMessenger(const G4String& xmlFormat);
    virtual ~TG4VGMMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  private:
    TG4VGMMessenger();
    TG4VGMMessenger(const TG4VGMMessenger& right);
    TG4VGMMessenger& operator=(const TG4VGMMessenger &right);

    static G4UIdirectory*  fgDirectory;     // command directory
    static G4UIcmdWithoutParameter* fgGenerateRootCmd;// command: /xml/generateRoot
    static G4int           fgCounter;            // instances counter
    Geant4GM::Factory*     fG4Factory;      // Geant4 VGM Factory
    RootGM::Factory*       fRootFactory;    // Root VGM Factory
    XmlVGM::VExporter*     fXmlVGMExporter; // associated class
    G4UIcmdWithAString*    fGenerateXMLCmd; // command: /xml/generateXXX
};

#endif //TG4_XML_VGM_MESSENGER_H

#endif //USE_VGM

