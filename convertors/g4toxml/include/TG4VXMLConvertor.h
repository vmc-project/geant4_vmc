// $Id: TG4VXMLConvertor.h,v 1.2 2003/12/18 13:25:11 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4VXMLConvertor
// ----------------------
// The interface for the XML convertor that
// converts G4 basic geometry objects to XML. 

#ifndef TG4_V_XML_CONVERTOR_H
#define TG4_V_XML_CONVERTOR_H

#include <geomdefs.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

class G4Element;
class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4PVReplica;

class TG4VXMLConvertor
{
  public:
    TG4VXMLConvertor();
    virtual ~TG4VXMLConvertor();

    // methods
    virtual void OpenSection(const G4String& topVolume) = 0;
    virtual void OpenPositions() = 0; 
    virtual void OpenRotations() = 0;     
    virtual void OpenMaterials() = 0; 
    virtual void OpenSolids() = 0; 
    virtual void OpenStructure() = 0; 
    virtual void OpenComposition(const G4String& name,
                                 const G4String& materialName) = 0;

    virtual void CloseSection(const G4String& topVolume) = 0;
    virtual void ClosePositions() = 0;
    virtual void CloseRotations() = 0;
    virtual void CloseMaterials() = 0;
    virtual void CloseSolids() = 0;
    virtual void CloseStructure() = 0;
    virtual void CloseComposition() = 0;

    virtual void WriteElement(const G4Element* element) = 0; 
    virtual void WriteMaterial(const G4Material* material) = 0; 
    virtual void WriteSolid(G4String lvName, const G4VSolid* solid, 
                            G4String materialName) = 0; 
    virtual void WritePosition(const G4String& name, 
                            G4ThreeVector position) = 0; 
    virtual void WriteRotation(const G4String& name, 
                            const G4RotationMatrix* rotation) = 0; 
    virtual void WritePositionWithRotation(
                            G4String lvName, G4ThreeVector position,
                            const G4RotationMatrix* rotation) = 0; 
    virtual void WritePositionWithRotation(
                            const G4String& lvName, 
			    const G4String& positionRef,
			    const G4String& rotationRef) = 0; 
    virtual void WriteMultiplePosition(
                            const G4String& lvName,
                            EAxis axis, G4int nofReplicas, 
			    G4double width, G4double offset) = 0;			       
    virtual void WriteEmptyLine() = 0;
    virtual void IncreaseIndention() = 0;
    virtual void DecreaseIndention() = 0;

    // set/get precision
    virtual void SetNumWidth(G4int width) = 0;
    virtual void SetNumPrecision(G4int precision) = 0;
    virtual G4double GetNumWidth() const = 0;
    virtual G4double GetNumPrecision() const = 0;
};

#endif //TG4_V_XML_CONVERTOR_H

