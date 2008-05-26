#ifndef TG4_COLOUR_STORE_H
#define TG4_COLOUR_STORE_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4ColourStore.h
/// \brief Definition of the TG4ColourStore class 
///
/// \author I. Hrivnacova; IPN Orsay

#include <G4Colour.hh>
#include <globals.hh> 
#include <vector>

#include <TColor.h>

/// \ingroup visualization
/// \brief Singleton data type class - store for the predefined colours.
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4ColourStore 
{
  public:
    /// The vector of TColor
    typedef std::vector<TColor> ColourVector;

    /// The iterator for the vector of TColor
    typedef ColourVector::iterator ColourIterator;
  
    /// The constant iterator for the vector of TColor
    typedef ColourVector::const_iterator ColourConstIterator;

  public:
    virtual ~TG4ColourStore();
    
    // static methods
    static TG4ColourStore* Instance();

    // get methods
    G4Colour GetColour(const G4String& name) const;
    G4Colour GetColour(const TColor& color) const;
    G4String GetColoursList() const;
    G4String GetColoursListWithCommas() const;
    
  private:
    /// Not implemented
    TG4ColourStore();  
    /// Not implemented
    TG4ColourStore(const TG4ColourStore& right);
    /// Not implemented
    TG4ColourStore& operator=(const TG4ColourStore& right);

    // static data members
    static TG4ColourStore*  fgInstance; ///< this instance

    // data members
    ColourVector  fColours; ///< vector of TColor
};   

#endif //TG4_COLOUR_STORE_H
