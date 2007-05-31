// $Id: TG4GeometryManager.h,v 1.12 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup geometry
/// \class TG4GeometryManager
/// \brief The manager class for building Geant4 geometry depending on
///  a selected user input
///
/// \author I. Hrivnacova; IPN, Orsay

#ifndef TG4_GEOMETRY_MANAGER_H
#define TG4_GEOMETRY_MANAGER_H

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4MagneticFieldType.h"
#include "TG4DetConstructionMessenger.h"

class TG4GeometryServices;
class TG4OpGeometryManager;
class TG4G3CutVector;
class TG4G3ControlVector;

class G4MagneticField;

class TVirtualMCGeometry;

class TG4GeometryManager : public TG4Verbose
{
  public:
    TG4GeometryManager(const TString& userGeometry);
    virtual ~TG4GeometryManager();

    // static access method
    static TG4GeometryManager* Instance();

    TVirtualMCGeometry*   GetMCGeometry() const;
    TG4OpGeometryManager* GetOpManager() const;

    // functions for building geometry
    void ConstructMagField();
    void ConstructGeometry();
    void FinishGeometry();
    void SetUserLimits(const TG4G3CutVector& cuts,
                       const TG4G3ControlVector& controls) const;
    
    void SetFieldType(TG4MagneticFieldType fieldType);
    void SetUniformFieldValue(G4double fieldValue);
     
  protected:
    TG4GeometryManager(const TG4GeometryManager& right);
    TG4GeometryManager& operator=(const TG4GeometryManager& right);

  private:
    // methods
    void CreateMCGeometry(); 
    void ConstructG4GeometryViaVMC();
    void ConstructG4GeometryViaVGM();
    void ConstructG4Geometry();
    void FillMediumMapFromG3();
    void FillMediumMapFromG4();
    void FillMediumMapFromRoot();
    void FillMediumMap();
        
    // static data members
    static TG4GeometryManager*  fgInstance;     //this instance
    static const G4double       fgLimitDensity; //material density limit
                                                //for setting max allowed step 
    static const G4double       fgMaxStep;      //max allowed step in materials 
                                                //with density < fLimitDensity

    // data members
    TG4DetConstructionMessenger  fMessenger; //messenger
    TG4GeometryServices*  fGeometryServices; // geometry services
    TVirtualMCGeometry*   fMCGeometry;       // 
    TG4OpGeometryManager* fOpManager;        //     
    G4String              fUserGeometry;     // user geometry input                                        
    TG4MagneticFieldType  fMagneticFieldType;// magnetic field type
    G4MagneticField*      fMagneticField;    // magnetic field
};

// inline methods

inline TG4GeometryManager* TG4GeometryManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline  TG4OpGeometryManager* TG4GeometryManager::GetOpManager() const {
  ///
  return fOpManager;
}   

#endif //TG4_GEOMETRY_MANAGER_H

