// $Id: TG4DetConstruction.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
// Category: geometry
//
// Author: I. Hrivnacova
//
// Class TG4DetConstruction
// ---------------------------
// Detector construction base class for building geometry
// using TVirtualMCApplication.

#ifndef TG4_DET_CONSTRUCTION_H
#define TG4_DET_CONSTRUCTION_H

#include "TG4Verbose.h"
#include "TG4DetConstructionMessenger.h"
#include "TG4MagneticFieldType.h"

#include <G4VUserDetectorConstruction.hh>
#include <globals.hh>
#include <g4std/vector>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4MagneticField;

class TG4DetConstruction : public G4VUserDetectorConstruction,
                           public TG4Verbose
{
  public:
    TG4DetConstruction();
    // --> protected
    // TG4DetConstruction(const TG4DetConstruction& right);
    virtual ~TG4DetConstruction();

    // methods
    virtual G4VPhysicalVolume* Construct();
    virtual void GenerateXMLGeometry() const;
    virtual void GenerateXMLGeometry(const G4String& lvName) const;
    virtual void PrintMaterials() const;

    // set methods
    void SetFieldType(TG4MagneticFieldType fieldType);
    void SetUniformFieldValue(G4double fieldValue);
    void SetReadGeometry(G4bool readGeometry);
    void SetWriteGeometry(G4bool writeGeometry);
    
  protected:
    TG4DetConstruction(const TG4DetConstruction& right);

    // operators
    TG4DetConstruction& operator=(const TG4DetConstruction& right);
    
  private:    
    // methods
    void CreateMagneticField();
    void GenerateXMLGeometry(G4LogicalVolume* lv) const;

    // data members
    TG4DetConstructionMessenger  fMessenger;        //messenger
    TG4MagneticFieldType         fMagneticFieldType;//magnetic field type
    G4MagneticField*             fMagneticField;    //magnetic field
    G4bool             fReadGeometry;     //option applied to all modules
    G4bool             fWriteGeometry;    //option applied to all modules     
};

// inline methods

inline void TG4DetConstruction::SetReadGeometry(G4bool readGeometry)
{ fReadGeometry = readGeometry; }

inline void TG4DetConstruction::SetWriteGeometry(G4bool writeGeometry)
{ fWriteGeometry = writeGeometry; }

#endif //TG4_DET_CONSTRUCTION_H

