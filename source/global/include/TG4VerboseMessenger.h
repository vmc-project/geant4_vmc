// $Id: TG4VerboseMessenger.h,v 1.3 2004/11/10 11:39:28 brun Exp $
/// \ingroup global
//
/// \class TG4VerboseMessenger
/// 
/// Messenger class that defines commands for 
/// the verbose classes.
///
/// Author: I. Hrivnacova


#ifndef TG4_VERBOSE_MESSENGER_H
#define TG4_VERBOSE_MESSENGER_H 

#include <G4UImessenger.hh>
#include <globals.hh>
#include <vector>

class TG4VVerbose;

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;

class TG4VerboseMessenger: public G4UImessenger
{
  typedef std::vector<TG4VVerbose*>           VerboseVector;
  typedef std::vector<G4UIcmdWithAnInteger*>  CommandVector;

  public:
    TG4VerboseMessenger(const G4String& directoryName);
    // --> protected   
    // TG4VerboseMessenger();
    // TG4VerboseMessenger(const TG4VerboseMessenger& right);
    virtual ~TG4VerboseMessenger();
   
    // methods 
    G4UIcommand* AddCommand(TG4VVerbose* verbose, const G4String& cmdName);
            void RemoveCommand(TG4VVerbose* verbose, G4UIcommand* command);
    virtual void SetNewValue(G4UIcommand* command, G4String string);
    
  protected:
    TG4VerboseMessenger();  
    TG4VerboseMessenger(const TG4VerboseMessenger& right);

    // operators
    TG4VerboseMessenger& operator=(const TG4VerboseMessenger& right);

  private:
    // methods
    void SetNewValueToAll(const G4String value) const;
  
    // data members
    const G4String        fkDirectoryName;  //command directory name
    G4UIdirectory*        fDirectory;       //command directory
    G4UIcmdWithAnInteger* fGlobalVerboseCmd;//global verbose command 
    VerboseVector         fVerboseVector;   //associated verbose instances
    CommandVector         fCommandVector;   //verbose commands
};

#endif //TG4_VERBOSE_MESSENGER_H
