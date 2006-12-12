// $Id: TG4ProcessMCMap.h,v 1.4 2005/09/01 10:04:33 brun Exp $
/// \ingroup physics
//
/// \class TG4ProcessMCMap
/// \brief Maps G4 process names to TMCProcess code 
///
/// Singleton map container for associated pairs
/// of G4 process name and TMCProcess code.
///
/// Author: I. Hrivnacova

#ifndef TG4_PROCESS_MC_MAP_H
#define TG4_PROCESS_MC_MAP_H

#include <map>
#include <globals.hh>

#include <Rtypes.h>
#include <TMCProcess.h>

class G4VProcess;

class TG4ProcessMCMap
{
  typedef std::map<G4String, TMCProcess, std::less<G4String> >  Map;
  typedef Map::iterator        MapIterator;
  typedef Map::const_iterator  MapConstIterator;

  public:
    TG4ProcessMCMap();
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

  private:
    TG4ProcessMCMap(const TG4ProcessMCMap& right);
    TG4ProcessMCMap& operator=(const TG4ProcessMCMap& right);
  
    // methods
    G4bool IsDefined(const G4String& processName);

    // static data members
    static TG4ProcessMCMap*  fgInstance; //this instance

    // data members
    Map  fMap; //map container
};

// inline methods

inline TG4ProcessMCMap* TG4ProcessMCMap::Instance() { 
  /// Return this instance
  return fgInstance; 
}

#endif //TG4_PROCESS_MC_MAP_H
