// $Id: $
//
// Author: I. Hrivnacova, 31.03.2004 
//
// Class TG4GDMLConvertor
// ----------------------
// The class provides methods for conversion of G4 basic geometry objects 
// to XML defined by GDML.

#ifndef TG4_GDML_CONVERTOR_H
#define TG4_GDML_CONVERTOR_H

#include <globals.hh>
#include <fstream>
#include <vector>
#include <set>

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
class G4Sphere;

class TG4GDMLConvertor : public TG4VXMLConvertor
{
  typedef std::vector<const G4RotationMatrix*>      RotationMatrixVector;

  public:
    TG4GDMLConvertor(std::ofstream& outFile,
		     const G4String& unitName = "unit1", 
                     const G4String& version = "1.0"); 
    virtual ~TG4GDMLConvertor();

    // methods
    virtual void OpenSection(const G4String& topVolume);
    virtual void OpenPositions(); 
    virtual void OpenRotations();     
    virtual void OpenMaterials();
    virtual void OpenSolids(); 
    virtual void OpenStructure(); 
    virtual void OpenComposition(const G4String& name,
                                 const G4String& materialName);

    virtual void CloseSection(const G4String& topVolume);
    virtual void ClosePositions();
    virtual void CloseRotations();
    virtual void CloseMaterials();
    virtual void CloseSolids();
    virtual void CloseStructure();
    virtual void CloseComposition();

    virtual void WriteElement(const G4Element* element); 
    virtual void WriteMaterial(const G4Material* material); 
    virtual void WriteSolid(G4String lvName, const G4VSolid* solid, 
                            G4String materialName); 
    virtual void WritePosition(const G4String& name, 
                            G4ThreeVector position); 
    virtual void WriteRotation(const G4String& name, 
                            const G4RotationMatrix* rotation); 
    virtual void WritePositionWithRotation(
                            G4String /*name*/, G4ThreeVector /*position*/,
   			    const G4RotationMatrix* /*rotation*/) {}
    virtual void WritePositionWithRotation(
                            const G4String& lvName, 
			    const G4String& positionRef,
			    const G4String& rotationRef); 
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
    // types
    typedef std::set <G4String, std::less<G4String> > StringSet; 

    //methods
    G4String UpdateName(const G4String& name,       
                        const G4String& extension = "") const;
    G4double UpdateAngle(G4double angle) const;
    void Append(G4String& name, G4int size, G4String string) const;
    void RegisterName(const G4String& name, G4bool warning = true);
    std::ostream& SmartPut(std::ostream& out,
		        G4int size, G4int precision,
		        const G4String& separator1, 
		        G4double number, const G4String& separator2) const;
   
         // writing solids
    void WriteBox (G4String name, const G4Box*  box,  G4String materialName, 
                   G4bool isReflected); 
    void WriteTubs(G4String name, const G4Tubs* tubs, G4String materialName, 
                   G4bool isReflected); 
    void WriteCons(G4String name, const G4Cons* cons, G4String materialName, 
                   G4bool isReflected); 
    void WriteTrd (G4String name, const G4Trd*  trd,  G4String materialName, 
                   G4bool isReflected); 
    void WriteTrap(G4String name, const G4Trap* trap, G4String materialName, 
                   G4bool isReflected); 
    void WritePara(G4String name, const G4Para* para, G4String materialName, 
                   G4bool isReflected); 
    //void WritePolycone(G4String name, const G4Polycone* polycone, 
    //               G4String materialName, G4bool isReflected); 
    //void WritePolyhedra(G4String name, const G4Polyhedra* polyhedra, 
    //               G4String materialName, G4bool isReflected); 
    void WriteSphere(G4String name, const G4Sphere* sphere, 
                   G4String materialName, G4bool isReflected); 
    void WriteNotSupportedSolid(G4String name, G4String materialName); 
  
    // static data members
    static const G4int     fgkDefaultNumWidth;     //default output numbers width
    static const G4int     fgkDefaultNumPrecision; //default output numbers precision 
    static const G4String  fgkSolidNameExtension;  //solid names extension 
    static const G4String  fgkElementNameExtension;//element names extension 
    static const char      fgkCharReplacement;     //replacement for ' ' in names 
    static const G4String  fgkNotAllowedChars;     //not allowed characters in XML
    static const G4String  fgkNotAllowedChars1;    //not allowed first characters
                                                   //in XML names

    // data members
    std::ofstream&    fOutFile;          //output file
    G4String          fUnitName;         //unit name
    G4String          fVersion;          //geometry version
    const G4String    fkBasicIndention;  //basic indention 
    G4String          fIndention;        //indention string
    G4int             fNW;               //output numbers width
    G4int             fNP;               //output numbers precision 
    StringSet         fGDMLNames;        //names in GDML 
};

inline void TG4GDMLConvertor::SetNumWidth(G4int width)
{ fNW = width; }

inline void TG4GDMLConvertor::SetNumPrecision(G4int precision)
{ fNP = precision; }

inline G4double TG4GDMLConvertor::GetNumWidth() const
{ return fNW; }

inline G4double TG4GDMLConvertor::GetNumPrecision() const
{ return fNP; }

#endif //TG4_GDML_CONVERTOR_H

