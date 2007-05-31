// $Id: TG4ColourStore.h,v 1.5 2006/12/12 16:21:16 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup visualization
/// \class TG4ColourStore
/// \brief Singleton data type class - store for the predefined colours.
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_COLOUR_STORE_H
#define TG4_COLOUR_STORE_H

#include <G4Colour.hh>
#include <globals.hh> 
#include <vector>

#include <TColor.h>

class TG4ColourStore 
{
  typedef std::vector<TColor>          ColourVector;
  typedef ColourVector::iterator       ColourIterator;
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
    TG4ColourStore();  
    TG4ColourStore(const TG4ColourStore& right);
    TG4ColourStore& operator=(const TG4ColourStore& right);

    // static data members
    static TG4ColourStore*  fgInstance; //this instance

    // data members
    ColourVector  fColours; //vector of TColor
};   

#endif //TG4_COLOUR_STORE_H
