#ifndef TG4_MEDIUM_MAP_H
#define TG4_MEDIUM_MAP_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4MediumMap.h
/// \brief Definition of the TG4MediumMap class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "globals.hh"

#include <map>

class TG4Medium;

class G4LogicalVolume;
class G4Material;

/// \ingroup geometry
/// \brief The map of media to logical volumes
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4MediumMap
{
  public:
    TG4MediumMap();
    virtual ~TG4MediumMap();

    // methods
    TG4Medium* AddMedium(G4int mediumID, G4bool warn = true);
    void MapMedium(G4LogicalVolume* lv, G4int mediumID);
    void MapMedium(const G4String& lvName, G4int mediumID);
    void Print() const;

    // get methods
    G4int       GetNofMedia() const;
    TG4Medium*  GetMedium(G4int mediumID, G4bool warn = true) const;
    TG4Medium*  GetMedium(const G4String& name, G4bool warn = true) const;
    TG4Medium*  GetMedium(G4LogicalVolume* lv, G4bool warn = true) const;
    TG4Medium*  GetMedium(const G4Material* material, G4bool warn = true) const;

  private:
    /// Not implemented
    TG4MediumMap(const TG4MediumMap& right);
    /// Not implemented
    TG4MediumMap& operator=(const TG4MediumMap& right);

    //
    // data members

    /// map of medias to their IDs
    std::map<G4int, TG4Medium*>             fIdMap;

    /// map of medias to the logical volumes
    std::map<G4LogicalVolume*, TG4Medium*>  fLVMap;

    /// map of materials to medias
    std::map<const G4Material*, TG4Medium*> fMaterialMap;
};


#endif //TG4_MEDIUM_MAP_H

