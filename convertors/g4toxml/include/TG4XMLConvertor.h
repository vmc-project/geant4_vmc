// $Id: TG4XMLConvertor.h,v 1.1 2003/07/22 06:46:58 brun Exp $
// Category: geometry
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4XMLConvertor
// ---------------------
// The class provides methods for conversion of G4 basic geometry objects 
// to XML defined by AGDD.dtd
// (ATLAS Generic Detector Description)

#ifndef TG4_XML_CONVERTOR_H
#define TG4_XML_CONVERTOR_H

#include "TG4VXMLConvertor.h"

#include <globals.hh>
#include <fstream>
#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4PVReplica;
class G4Box;
class G4Tubs;
class G4Cons;
class G4Trd;
class G4Trap;
class G4Para;
class G4Polycone;
class G4Polyhedra;

class TG4XMLConvertor : public TG4VXMLConvertor
{
  typedef std::vector<const G4RotationMatrix*> RotationMatrixVector;

  public:
    TG4XMLConvertor(std::ofstream& outFile);
    virtual ~TG4XMLConvertor();

    // methods
    virtual void OpenMaterials(const G4String& version, const G4String& date, 
            const G4String& author, const G4String dtdVersion);
    virtual void OpenSection(const G4String& versionDTD, const G4String& name, 
            const G4String& version, const G4String& date, const G4String& author,
            const G4String& topVolume);
    virtual void OpenComposition(const G4String& name);
    virtual void CloseMaterials();
    virtual void CloseSection();
    virtual void CloseComposition();

    virtual void WriteMaterial(const G4Material* material); 
    virtual void WriteSolid(G4String lvName, const G4VSolid* solid, 
                            G4String materialName); 
    virtual void WriteRotation(const G4RotationMatrix* rotation); 
    virtual void WritePosition(G4String lvName, G4ThreeVector position); 
    virtual void WritePositionWithRotation(
                               G4String lvName, G4ThreeVector position,
   			       const G4RotationMatrix* rotation);
    virtual void WriteReplica(G4String lvName, G4PVReplica* pvr);			       
    virtual void WriteEmptyLine();
    virtual void IncreaseIndention();
    virtual void DecreaseIndention();
    
    // set methods
    void SetNumWidth(G4int width);
    void SetNumPrecision(G4int precision);

  private:
    //methods
    void Append(G4String& string, G4int number) const;
    void CutName(G4String& name) const;
    void CutName(G4String& name, G4int size) const;
    void PutName(G4String& element, G4String name, G4String templ) const;
    
         // writing solids
    void WriteBox (G4String lvName, const G4Box*  box,  G4String materialName, 
                   G4bool isReflected); 
    void WriteTubs(G4String lvName, const G4Tubs* tubs, G4String materialName, 
                   G4bool isReflected); 
    void WriteCons(G4String lvName, const G4Cons* cons, G4String materialName, 
                   G4bool isReflected); 
    void WriteTrd (G4String lvName, const G4Trd*  trd,  G4String materialName, 
                   G4bool isReflected); 
    void WriteTrap(G4String lvName, const G4Trap* trap, G4String materialName, 
                   G4bool isReflected); 
    void WritePara(G4String lvName, const G4Para* para, G4String materialName, 
                   G4bool isReflected); 
    void WritePolycone(G4String lvName, const G4Polycone* polycone, 
                   G4String materialName, G4bool isReflected); 
    void WritePolyhedra(G4String lvName, const G4Polyhedra* polyhedra, 
                   G4String materialName, G4bool isReflected); 
  
    // static data members
    static const G4int fgkMaxVolumeNameLength;  //maximal volume name length
    static const G4int fgkMaxMaterialNameLength;//maximal material name length
    static const G4int fgkDefaultNumWidth;      //default output numbers width
    static const G4int fgkDefaultNumPrecision;  //default output numbers precision 

    // data members
    std::ofstream&    fOutFile;          //output file
    const G4String    fkBasicIndention;  //basic indention 
    G4String          fIndention;        //indention string
    G4int             fNW;               //output numbers width
    G4int             fNP;               //output numbers precision 
    G4int             fRotationCounter;  //counter of rotations
    RotationMatrixVector  fRotations;    //vector of rot matrices
};

inline void TG4XMLConvertor::SetNumWidth(G4int width)
{ fNW = width; }

inline void TG4XMLConvertor::SetNumPrecision(G4int precision)
{ fNP = precision; }

#endif //TG4_XML_CONVERTOR_H

