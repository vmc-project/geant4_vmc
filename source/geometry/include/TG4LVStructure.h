#ifndef TG4_LV_STRUCTURE_H
#define TG4_LV_STRUCTURE_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4LVStructure.h
/// \brief Definition of the TG4LVStructure class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include <globals.hh>
#include <vector>

class G4LogicalVolume;
class TG4LVStructure;

/// \ingroup geometry
/// \brief A helper class for LVTree
/// 
/// Class that associates the name tree with the logical volumes tree. 
/// Used for printing volumes trees.  
///
/// \author I. Hrivnacova; IPN, Orsay

class TG4LVStructure 
{
  public:
    /// The vecor of TG4 logical volume structures
    typedef std::vector<TG4LVStructure*>  LVStructuresVector;

    /// The vector of G4 logical volumes
    typedef std::vector<G4LogicalVolume*> LogicalVolumesVector;

  public:
    TG4LVStructure(G4String aPath);
    TG4LVStructure(const TG4LVStructure& right);
    virtual ~TG4LVStructure();

    // operators
    TG4LVStructure& operator=(const TG4LVStructure& right);
    G4int operator==(const TG4LVStructure &right) const;

    // methods
    void AddNewVolume(G4LogicalVolume* lv, const G4String& treeStructure);
    void ListTree() const;
    void ListTreeLong() const;

    // set methods
    void SetVerboseLevel(G4int verbose); 
#ifdef G4VIS_USE
    void SetTreeVisibility(G4bool visibility);       
    void SetTreeColour(const G4String& colName);
#endif             

    // get methods
    G4LogicalVolume* GetVolume(const G4String& name) const;
    G4LogicalVolume* FindVolume(const G4String& name) const;

  private:
    /// Not implemented
    TG4LVStructure(); 

    // methods
    TG4LVStructure* FindSubDirectory(const G4String& subDir) const;
    G4String ExtractDirName(const G4String& path) const;
    void ClearAndDestroy(LVStructuresVector* structures); 

    //
    // data members
    
    /// vector of contained structures
    LVStructuresVector    fStructures;
    
    /// vector of contained logical volumes (parallel to fStructures)
    LogicalVolumesVector  fLogicalVolumes;
    
    /// full path name
    G4String  fPathName;
    
    /// directory name
    G4String  fDirName;
    
    /// verbose level
    G4int     fVerboseLevel;
};

#endif //TG4_LV_STRUCTURE_H

