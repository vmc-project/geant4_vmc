// $Id: $
/// \ingroup geometry
//
/// \class TG4VGMMessenger
/// \brief Messenger class that defines commands for VGM XML Exporter
///
/// Author: I. Hrivnacova

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
    // --> protected
    // TG4VGMMessenger();
    // TG4VGMMessenger(const TG4VGMMessenger& right);
    virtual ~TG4VGMMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  protected:
    TG4VGMMessenger();
    TG4VGMMessenger(const TG4VGMMessenger& right);

    // operators
    TG4VGMMessenger& operator=(const TG4VGMMessenger &right);
             
  private:
    static G4UIdirectory*  fgDirectory;     // command directory
    static G4UIcmdWithoutParameter* fgGenerateRootCmd;// command: /xml/generateRoot
    static G4int           fgCounter;	    // instances counter
    Geant4GM::Factory*     fG4Factory;      // Geant4 VGM Factory
    RootGM::Factory*       fRootFactory;    // Root VGM Factory
    XmlVGM::VExporter*     fXmlVGMExporter; // associated class
    G4UIcmdWithAString*    fGenerateXMLCmd; // command: /xml/generateXXX
};

#endif //TG4_XML_VGM_MESSENGER_H

#endif //USE_VGM

