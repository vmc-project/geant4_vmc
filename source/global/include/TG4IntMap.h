// $Id: TG4IntMap.h,v 1.2 2003/12/18 13:28:08 brun Exp $
/// \ingroup global
//
/// \class TG4IntMap
/// 
/// The map container for integer numbers associated with names.
///
/// Author: I. Hrivnacova

#ifndef TG4_INT_MAP_H
#define TG4_INT_MAP_H

#include <map>
#include <globals.hh>

class TG4IntMap
{
  typedef std::map<G4String, G4int, std::less<G4String> >  Map;
  typedef Map:: iterator       MapIterator;
  typedef Map:: const_iterator MapConstIterator;

  public:
    TG4IntMap();
    // --> protected
    // TG4IntMap(const TG4IntMap& right);
    virtual ~TG4IntMap();

    // methods
    G4bool Add(const G4String& first, G4int second);  
    G4int GetSecond(const G4String& name, G4bool warn = true);
    void PrintAll() const;
    void Clear();

  protected:
    TG4IntMap(const TG4IntMap& right);

    // operators
    TG4IntMap& operator=(const TG4IntMap& right);
  
  private:
    G4bool IsDefined(const G4String& first);
  
    // data members
    Map  fMap; //map container
};

#endif //TG4_NAME_MAP_H
