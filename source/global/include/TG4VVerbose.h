// $Id: TG4VVerbose.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup global
//
/// \class TG4VVerbose
/// 
/// Class defines the verbose level:
/// - 0 - no output
/// - 1 - minimal output (default)
/// - 2 and more - more detailed output
///
/// Author: I. Hrivnacova

#ifndef TG4_V_VERBOSE_H
#define TG4_V_VERBOSE_H

#include <globals.hh>

class G4UImessenger;

class TG4VVerbose
{
  public:
    TG4VVerbose();
    TG4VVerbose(G4int verboseLevel);      
    virtual ~TG4VVerbose();

    // set methods
    virtual void  VerboseLevel(G4int level);

    // get methods
    virtual G4int VerboseLevel() const;

  private:
    // methods
    virtual G4UImessenger* CreateMessenger() = 0;    

    // static data members
    static const G4int  fgkDefaultVerboseLevel; // default verbose level

    // data members
    G4int  fVerboseLevel; // verbose level
};     

// inline methods

inline void TG4VVerbose::VerboseLevel(G4int level) { 
  /// Set level of verbosity
  fVerboseLevel =  level; 
}

inline G4int TG4VVerbose::VerboseLevel() const { 
  /// Return level of verbosity
  return fVerboseLevel; 
}

#endif //TG4_V_VERBOSE_H
