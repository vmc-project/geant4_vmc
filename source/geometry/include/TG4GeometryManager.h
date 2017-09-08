#ifndef TG4_GEOMETRY_MANAGER_H
#define TG4_GEOMETRY_MANAGER_H

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4GeometryManager.h
/// \brief Definition of the TG4GeometryManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4Verbose.h"
#include "TG4Globals.h"
#include "TG4FieldParameters.h"
#include "TG4DetConstructionMessenger.h"

#include <vector>

class TG4MagneticField;
class TG4GeometryServices;
class TG4OpGeometryManager;
class TG4ModelConfigurationManager;
class TG4G3CutVector;
class TG4G3ControlVector;
class TG4VUserRegionConstruction;
class TG4VUserPostDetConstruction;
class TG4RadiatorDescription;

class G4LogicalVolume;
class G4EquationOfMotion;
class G4MagIntegratorStepper;

class TVirtualMCGeometry;
class TVirtualMagField;

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
    TG4ModelConfigurationManager* GetFastModelsManager() const;
    TG4ModelConfigurationManager* GetEmModelsManager() const;

    // functions for building geometry
    void ConstructGeometry();
    void ConstructSDandField();
    void FinishGeometry();
    void UpdateMagField();
    void CreateMagFieldParameters(const G4String& fieldVolName);
    TG4RadiatorDescription*
         CreateRadiator(const G4String& volName);
    void SetUserLimits(const TG4G3CutVector& cuts,
                       const TG4G3ControlVector& controls) const;
    void SetIsLocalMagField(G4bool isLocalMagField);
    void SetIsZeroMagField(G4bool isZeroMagField);
    void SetIsUserMaxStep(G4bool isUserMaxStep);
    void SetIsMaxStepInLowDensityMaterials(G4bool isMaxStep);
     
    // set user region construction
    void SetUserRegionConstruction(
            TG4VUserRegionConstruction* userRegionConstruction);
    void SetUserPostDetConstruction(
            TG4VUserPostDetConstruction* userPostDetConstruction);
    void SetUserEquationOfMotion(G4EquationOfMotion* equation, G4String volumeName="");
    void SetUserStepper(G4MagIntegratorStepper* stepper, G4String volumeName="");
            
    void SetLimitDensity(G4double density);
    void SetMaxStepInLowDensityMaterials(G4double maxStep);

    // printing
    void PrintFieldStatistics() const;

    // get methods
    const std::vector<TG4RadiatorDescription*>& GetRadiators() const;

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
    TG4FieldParameters* GetOrCreateFieldParameters(const G4String& volumeName);
    void CreateMagField(TVirtualMagField* magField,
           TG4FieldParameters* fieldParameters, G4LogicalVolume* lv);
    void ConstructGlobalMagField();
    void ConstructZeroMagFields();
    void ConstructLocalMagFields();
        
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

    /// Fast simulation models manager
    TG4ModelConfigurationManager*  fFastModelsManager;

    /// EM models manager
    TG4ModelConfigurationManager*  fEmModelsManager;

    /// User geometry input
    G4String  fUserGeometry;

    /// Magnetic field parameters
    std::vector<TG4FieldParameters*>  fFieldParameters;

    /// Magnetic fields
    static G4ThreadLocal std::vector<TG4MagneticField*>*  fgMagneticFields;

    /// Radiators
    std::vector<TG4RadiatorDescription*>  fRadiators;

    /// User region construction
    TG4VUserRegionConstruction* fUserRegionConstruction;

    /// User post detector construction
    TG4VUserPostDetConstruction* fUserPostDetConstruction;

    /// option to activate getting local magnetic fields from Root geometry
    G4bool    fIsLocalMagField;

    /// option to activate propagating 'ifield = 0' defined in tracking media
    G4bool    fIsZeroMagField;

    /// info if a cached magnetic field is in use
    G4bool    fIsCachedMagneticField;

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

inline TG4ModelConfigurationManager* TG4GeometryManager::GetFastModelsManager() const {
  /// Return fast simulation models manager
  return fFastModelsManager;
}

inline TG4ModelConfigurationManager* TG4GeometryManager::GetEmModelsManager() const {
  /// Return fast simulation models manager
  return fEmModelsManager;
}

inline void TG4GeometryManager::SetLimitDensity(G4double density) {
  /// Set the material density limit for setting max allowed step
  fLimitDensity = density;
}  

inline void TG4GeometryManager::SetMaxStepInLowDensityMaterials(G4double maxStep) {
  /// Set max allowed step value in materials with density < fLimitDensity
  fMaxStepInLowDensityMaterials = maxStep;
}  

inline const std::vector<TG4RadiatorDescription*>& TG4GeometryManager::GetRadiators() const {
  /// Return the vectpr of defined radiators
  return fRadiators;
}

#endif //TG4_GEOMETRY_MANAGER_H

