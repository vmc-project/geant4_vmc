// $Id: TG4LVStructure.h,v 1.2 2003/12/18 13:28:08 brun Exp $
/// \ingroup geometry
//
/// \class TG4LVStructure
/// 
/// Class that associates the name tree with the logical volumes tree. 
/// Used for printing volumes trees.  
///
/// Author: I. Hrivnacova

#ifndef TG4_LV_STRUCTURE_H
#define TG4_LV_STRUCTURE_H

#include <globals.hh>
#include <vector>

class G4LogicalVolume;
class TG4LVStructure;

typedef std::vector<TG4LVStructure*>  LVStructuresVector;
typedef std::vector<G4LogicalVolume*> LogicalVolumesVector;

class TG4LVStructure 
{
  public:
    TG4LVStructure(G4String aPath);
    TG4LVStructure(const TG4LVStructure& right);
    // --> protected 
    // TG4LVStructure();
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

  protected:
    TG4LVStructure(); 

  private:
    // methods
    TG4LVStructure* FindSubDirectory(const G4String& subDir) const;
    G4String ExtractDirName(const G4String& path) const;
    void ClearAndDestroy(LVStructuresVector* structures); 

    // data members
    LVStructuresVector    fStructures;     //vector of contained structures
    LogicalVolumesVector  fLogicalVolumes; //vector of contained logical volumes
				           //(parallel to fStructures)
    G4String  fPathName;     //full path name
    G4String  fDirName;      //directory name
    G4int     fVerboseLevel; //verbose level
};

#endif //TG4_LV_STRUCTURE_H

