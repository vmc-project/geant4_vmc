// $Id: TG4ProcessMCMap.h,v 1.1 2002/06/20 11:57:45 hristov Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4ProcessMCMap
// ---------------------
// Singleton map container for associated pairs
// of G4 process name and TMCProcess code.

#ifndef TG4_PROCESS_MC_MAP_H
#define TG4_PROCESS_MC_MAP_H

#include <g4std/map>
#include <globals.hh>

#include <Rtypes.h>
#include <TMCProcess.h>

class G4VProcess;

class TG4ProcessMCMap
{
  typedef G4std::map<G4String, TMCProcess, G4std::less<G4String> >  Map;
  typedef Map::iterator        MapIterator;
  typedef Map::const_iterator  MapConstIterator;

  public:
    TG4ProcessMCMap();
    // --> protected
    // TG4ProcessMCMap(const TG4ProcessMCMap& right);
    virtual ~TG4ProcessMCMap();

    // static access method
    static TG4ProcessMCMap* Instance();

    // methods
    G4bool Add(G4VProcess* process,  TMCProcess second);  
    G4bool Add(G4String processName, TMCProcess second);  
    void PrintAll() const;
    void Clear();

    // get methods
    TMCProcess  GetMCProcess(const G4VProcess* process);
    TMCProcess  GetMCProcess(const G4String& processName);
    G4String    GetMCProcessName(const G4VProcess* process);
    G4String    GetMCProcessName(const G4String& processName);

  protected:
    TG4ProcessMCMap(const TG4ProcessMCMap& right);

    // operators
    TG4ProcessMCMap& operator=(const TG4ProcessMCMap& right);
  
  private:
    // methods
    G4bool IsDefined(const G4String& processName);

    // static data members
    static TG4ProcessMCMap*  fgInstance; //this instance

    // data members
    Map  fMap; //map container
};

// inline methods

inline TG4ProcessMCMap* TG4ProcessMCMap::Instance() 
{ return fgInstance; }

#endif //TG4_PROCESS_MC_MAP_H
