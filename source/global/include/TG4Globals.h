// $Id: TG4Globals.h,v 1.5 2004/11/10 11:39:28 brun Exp $
/// \ingroup global

/// \class TG4Globals
/// \brief Basic types and functions of general use.
///
/// Class provides the basic types and functions of general use. 
/// It is protected from instantiating (only static data members
/// and static methods are defined).
///
/// Author: I. Hrivnacova

#ifndef TG4_GLOBALS_H
#define TG4_GLOBALS_H

#include "TG4G3Control.h"

#include <globals.hh>
#include <vector>
#include <set>
#include <map>
#include <G4RotationMatrix.hh>

class G4Material;
class G4Element;

// basic types containers
typedef std::vector<G4bool>     TG4boolVector;
typedef std::vector<G4int>      TG4intVector;
typedef std::vector<G4double>   TG4doubleVector;
typedef std::vector<G4String>   TG4StringVector;
typedef std::map <G4int, G4int> TG4intMap;
typedef std::set <G4String, std::less<G4String> > TG4StringSet; 

class TG4Globals
{
  public:
    // --> protected 
    // TG4Globals();
    virtual ~TG4Globals();

    // static methods
    static void Exception(const char* string=0);
      // Global error function prints string to cerr, and aborts
      // program - according to G4Exception.cc
    static void Warning(const char* string=0);
      // Global warning function prints string to cerr
    static void AppendNumberToString(G4String& string, G4int number);
    static G4bool Compare(G4bool activation, TG4G3ControlValue controlValue);
    static void PrintStars(G4bool emptyLineFirst);
    static G4String Help();

  protected:
    TG4Globals();  
      // only typedefs's and static methods
};  

#endif //ALGLOBALS_H
