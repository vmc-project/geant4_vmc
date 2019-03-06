#ifndef TG4_GLOBALS_H
#define TG4_GLOBALS_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4Globals.h
/// \brief Definition of the TG4Globals class and basic container types
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4G3Control.h"

#include <globals.hh>
#include <G4RotationMatrix.hh>

#include <TString.h>

#include <vector>
#include <set>
#include <map>

class G4Material;
class G4Element;

//
// basic container types

/// \ingroup global
/// The vector of booleans
typedef std::vector<G4bool>     TG4boolVector;

/// \ingroup global
/// The vector of integers
typedef std::vector<G4int>      TG4intVector;

/// \ingroup global
/// The vector of doubles
typedef std::vector<G4double>   TG4doubleVector;

/// \ingroup global
/// The vector of strings
typedef std::vector<G4String>   TG4StringVector;

/// \ingroup global
/// The map of integers to integers
typedef std::map <G4int, G4int> TG4intMap;

/// \ingroup global
/// The set of strings
typedef std::set <G4String, std::less<G4String> > TG4StringSet;

/// \ingroup global
/// \brief Basic types and functions of general use.
///
/// Class provides the basic types and functions of general use.
/// It is protected from instantiating (only static data members
/// and static methods are defined).
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4Globals
{
  public:
    virtual ~TG4Globals();

    // static methods
    static void Exception(const TString& className,const TString& methodName,
                          const TString& text);
      // Global error function prints string to cerr, an
      // program - according to G4Exception.cc

    static void Warning(const TString& className,const TString& methodName,
                        const TString& text);
      // Global warning function prints string to cerr


    static TString Endl();
    static void AppendNumberToString(G4String& string, G4int number);
    static G4bool Compare(G4bool activation, TG4G3ControlValue controlValue);
    static void PrintStars(G4bool emptyLineFirst);
    static G4String Help();

    static G4String  GetToken(Int_t i, const TString& s);

  private:
    TG4Globals();

    static const TString fgkEndl;           ///< Special endl
    static const char    fgkTokenSeparator; ///< Separator in GetToken() method
};

// inline functions

inline TString  TG4Globals::Endl()
{
  /// Special endl which is then reformatted in Warning and Exception
  return fgkEndl;
}


#endif //ALGLOBALS_H
