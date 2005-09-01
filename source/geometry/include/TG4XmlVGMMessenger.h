// $Id: TG4XmlVGMMessenger.h,v 1.1 2005/02/02 14:16:21 brun Exp $
/// \ingroup geometry
//
/// \class TG4XmlVGMMessenger
/// \brief Messenger class that defines commands for VGM XML Exporter
///
/// Author: I. Hrivnacova

#ifdef USE_VGM

#ifndef TG4_XML_VGM_MESSENGER_H
#define TG4_XML_VGM_MESSENGER_H

#include <XmlVGM/VExporter.h>
#include <Geant4GM/volumes/Factory.h>

#include <G4UImessenger.hh>
#include <globals.hh>

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;

class TG4XmlVGMMessenger: public G4UImessenger
{
  public:
    TG4XmlVGMMessenger(const G4String& xmlFormat);
    // --> protected
    // TG4XmlVGMMessenger();
    // TG4XmlVGMMessenger(const TG4XmlVGMMessenger& right);
    virtual ~TG4XmlVGMMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  protected:
    TG4XmlVGMMessenger();
    TG4XmlVGMMessenger(const TG4XmlVGMMessenger& right);

    // operators
    TG4XmlVGMMessenger& operator=(const TG4XmlVGMMessenger &right);
             
  private:
    static G4UIdirectory*  fgDirectory;     // command directory
    static G4int           fgCounter;       // instances counter
    Geant4GM::Factory*     fG4Factory;      // Geant4 VGM Factory
    XmlVGM::VExporter*     fXmlVGMExporter; // associated class
    G4UIcmdWithAString*    fGenerateXMLCmd; // command: /xml/generateXXX
};

#endif //TG4_XML_VGM_MESSENGER_H

#endif //USE_VGM

