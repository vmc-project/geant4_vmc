// $Id: TG4XMLMessenger.h,v 1.1 2003/07/22 06:46:58 brun Exp $
//
// Author: I. Hrivnacova
//
// Class TG4XMLMessenger
// ---------------------
// Messenger class that defines commands for TG4VXMLGeometryGenerator

#ifndef TG4_XML_MESSENGER_H
#define TG4_XML_MESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class TG4VXMLGeometryGenerator;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;

class TG4XMLMessenger: public G4UImessenger
{
  public:
    TG4XMLMessenger(TG4VXMLGeometryGenerator* geometryGenerator,
                    const G4String& xmlFormat);
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
    static G4UIdirectory*     fgDirectory;    //command directory
    static G4int              fgCounter;      //instances counter

    TG4VXMLGeometryGenerator* fXMLGeometryGenerator; //associated class
    G4UIcmdWithAString*       fGenerateXMLCmd;       //command: /xml/generateXXX
};

#endif //TG4_XML_MESSENGER_H

