// $Id: TG4AGDDConvertor.h,v 1.2 2003/12/18 13:25:11 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4AGDDConvertor
// ----------------------
// The class provides methods for conversion of G4 basic geometry objects 
// to XML defined by AGDD.dtd
// (ATLAS Generic Detector Description)

#ifndef TG4_AGDD_CONVERTOR_H
#define TG4_AGDD_CONVERTOR_H

#include <globals.hh>
#include <fstream>
#include <vector>

#include "TG4VXMLConvertor.h"

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

class TG4AGDDConvertor : public TG4VXMLConvertor
{
  typedef std::vector<const G4RotationMatrix*> RotationMatrixVector;

  public:
    TG4AGDDConvertor(std::ofstream& outFile,
                     const G4String& version = "1.0", 
		     const G4String& date = "Undefined", 
                     const G4String& author = "G4 XML Convertor", 
		     const G4String dtdVersion = "v6");
    virtual ~TG4AGDDConvertor();

    // methods
    virtual void OpenSection(const G4String& topVolume);
    virtual void OpenPositions() {} 
    virtual void OpenRotations() {}     
    virtual void OpenMaterials();
    virtual void OpenSolids() {} 
    virtual void OpenStructure() {} 
    virtual void OpenComposition(const G4String& name,
                                 const G4String& /*materialName*/);

    virtual void CloseSection(const G4String& /*topVolume*/);
    virtual void ClosePositions() {}
    virtual void CloseRotations() {}
    virtual void CloseMaterials();
    virtual void CloseSolids() {}
    virtual void CloseStructure() {}
    virtual void CloseComposition();

    virtual void WriteElement(const G4Element* element) {} 
    virtual void WriteMaterial(const G4Material* material); 
    virtual void WriteSolid(G4String lvName, const G4VSolid* solid, 
                            G4String materialName); 
    virtual void WritePosition(const G4String& name, 
                            G4ThreeVector position); 
    virtual void WriteRotation(const G4String& name, 
                            const G4RotationMatrix* rotation); 
    virtual void WritePositionWithRotation(
                            G4String lvName, G4ThreeVector position,
   			    const G4RotationMatrix* rotation);
    virtual void WritePositionWithRotation(
                            const G4String& /*lvName*/, 
			    const G4String& /*positionRef*/,
			    const G4String& /*rotationRef*/) {} 
    virtual void WriteMultiplePosition(
                            const G4String& lvName,
                            EAxis axis, G4int nofReplicas, 
			    G4double width, G4double offset);			       
    virtual void WriteEmptyLine();
    virtual void IncreaseIndention();
    virtual void DecreaseIndention();
    
    // set methods
    virtual void SetNumWidth(G4int width);
    virtual void SetNumPrecision(G4int precision);

    // get methods
    virtual G4double GetNumWidth() const;
    virtual G4double GetNumPrecision() const;

  private:
    //methods
    void Append(G4String& string, G4int number) const;
    void CutName(G4String& name) const;
    void CutName(G4String& name, G4int size) const;
    void PutName(G4String& element, G4String name, G4String templ) const;
    std::ostream& SmartPut(std::ostream& out,
		 G4int size, G4int precision,
		 G4double number, const G4String& separator) const;
   
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
    void WriteNotSupportedSolid(G4String name, G4String materialName); 
  
    // static data members
    static const G4int fgkMaxVolumeNameLength;  //maximal volume name length
    static const G4int fgkMaxMaterialNameLength;//maximal material name length
    static const G4int fgkDefaultNumWidth;      //default output numbers width
    static const G4int fgkDefaultNumPrecision;  //default output numbers precision 

    // data members
    std::ofstream&    fOutFile;          //output file
    G4String          fVersion;          //geometry version
    G4String          fDate;             //date
    G4String          fAuthor;           //geometry author
    G4String          fDtdVersion;       //DTD version
    const G4String    fkBasicIndention;  //basic indention 
    G4String          fIndention;        //indention string
    G4int             fNW;               //output numbers width
    G4int             fNP;               //output numbers precision 
    G4int             fRotationCounter;  //counter of rotations
    RotationMatrixVector  fRotations;    //vector of rot matrices
};

inline void TG4AGDDConvertor::SetNumWidth(G4int width)
{ fNW = width; }

inline void TG4AGDDConvertor::SetNumPrecision(G4int precision)
{ fNP = precision; }

inline G4double TG4AGDDConvertor::GetNumWidth() const
{ return fNW; }

inline G4double TG4AGDDConvertor::GetNumPrecision() const
{ return fNP; }

#endif //TG4_AGDD_CONVERTOR_H

