#ifndef TG4_GEOMETRY_MANAGER_H
#define TG4_GEOMETRY_MANAGER_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4GeometryManager.h
/// \brief Definition of the TG4GeometryManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4FieldParameters.h"
#include "TG4DetConstructionMessenger.h"

class TG4MagneticField;
class TG4GeometryServices;
class TG4OpGeometryManager;
class TG4G3CutVector;
class TG4G3ControlVector;
class TG4VUserRegionConstruction;

class TVirtualMCGeometry;

/// \ingroup geometry
/// \brief The manager class for building Geant4 geometry depending on
///  a selected user input
///
/// \author I. Hrivnacova; IPN, Orsay

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
    void ConstructSDandField();
    void FinishGeometry();
    void UpdateMagField();
    void SetUserLimits(const TG4G3CutVector& cuts,
                       const TG4G3ControlVector& controls) const;
    void SetIsUserMaxStep(G4bool isUserMaxStep);
    void SetIsMaxStepInLowDensityMaterials(G4bool isMaxStep);
     
    // set user region construction
    void SetUserRegionConstruction(
            TG4VUserRegionConstruction* userRegionConstruction);
            
    void SetLimitDensity(G4double density);
    void SetMaxStepInLowDensityMaterials(G4double maxStep);
           
  private:
    /// Not implemented
    TG4GeometryManager(const TG4GeometryManager& right);
    /// Not implemented
    TG4GeometryManager& operator=(const TG4GeometryManager& right);

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
    static TG4GeometryManager*  fgInstance;     ///< this instance
    
    /// default material density limit for setting max allowed step 
    static const G4double  fgDefaultLimitDensity; 
    
    /// default max allowed step in materials with density < fLimitDensity
    static const G4double  fgDefaultMaxStep; 

    // data members
    TG4DetConstructionMessenger  fMessenger; ///< messenger
    TG4GeometryServices*  fGeometryServices; ///< geometry services
    TVirtualMCGeometry*   fMCGeometry;       ///< VirtualMC geometry
    TG4OpGeometryManager* fOpManager;        ///< optical geometry manager    
    G4String              fUserGeometry;     ///< user geometry input                                        
    TG4FieldParameters    fFieldParameters;  ///< magnetic field parameters
    static G4ThreadLocal TG4MagneticField*  fMagneticField; ///< magnetic field
    TG4VUserRegionConstruction* fUserRegionConstruction; ///< user region construction

    /// option to activate max step defined in tracking media
    G4bool    fIsUserMaxStep; 

    /// option to activate max step defined in low density materials           
    G4bool    fIsMaxStepInLowDensityMaterials; 
    
    /// material density limit for setting max allowed step 
    G4double  fLimitDensity;     
    
    /// max allowed step in materials with density < fLimitDensity
    G4double  fMaxStepInLowDensityMaterials;                                     
};

// inline methods

inline TG4GeometryManager* TG4GeometryManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline  TG4OpGeometryManager* TG4GeometryManager::GetOpManager() const {
  /// Return the optical geometry manager
  return fOpManager;
}       

inline void TG4GeometryManager::SetLimitDensity(G4double density) {
  /// Set the material density limit for setting max allowed step
  fLimitDensity = density;
}  

inline void TG4GeometryManager::SetMaxStepInLowDensityMaterials(G4double maxStep) {
  /// Set max allowed step value in materials with density < fLimitDensity
  fMaxStepInLowDensityMaterials = maxStep;
}  

#endif //TG4_GEOMETRY_MANAGER_H

