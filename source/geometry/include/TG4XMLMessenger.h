// $Id: TG4XMLMessenger.h,v 1.2 2002/10/10 13:17:55 brun Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4XMLMessenger
// ---------------------
// Messenger class that defines commands for TG4XMLGeometryGenerator

#ifndef TG4_XML_MESSENGER_H
#define TG4_XML_MESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4XMLGeometryGenerator;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;

class TG4XMLMessenger: public G4UImessenger
{
  public:
    TG4XMLMessenger(TG4XMLGeometryGenerator* geometryGenerator);
    // --> protected
    // TG4XMLMessenger();
    // TG4XMLMessenger(const TG4XMLMessenger& right);
    virtual ~TG4XMLMessenger();

    // methods
    virtual void SetNewValue(G4UIcommand* command, G4String newValues);
    
  protected:
    TG4XMLMessenger();
    TG4XMLMessenger(const TG4XMLMessenger& right);

    // operators
    TG4XMLMessenger& operator=(const TG4XMLMessenger &right);
             
  private:
    TG4XMLGeometryGenerator*  fXMLGeometryGenerator; //associated class
    G4UIdirectory*            fDirectory;            //command directory
    G4UIcmdWithAString*       fGenerateXMLCmd;       //command: generateXML
};

#endif //TG4_XML_MESSENGER_H

