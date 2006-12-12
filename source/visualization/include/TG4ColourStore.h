// $Id: TG4ColourStore.h,v 1.4 2005/09/01 10:04:33 brun Exp $
/// \ingroup visualization
//
/// \class TG4ColourStore
/// \brief Singleton data type class - store for the predefined colours.
///
/// Author: I. Hrivnacova

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
