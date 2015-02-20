#ifndef A01_MC_APPLICATION_H
#define A01_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01MCApplication.h 
/// \brief Definition of the A01MCApplication class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>
#include <TGeoUniformMagField.h> 
#include "TMCVerbose.h"

#include "A01RootDetectorConstruction.h"

class Ex03MCStack;
class A01MagField;
class A01PrimaryGenerator;
class A01DriftChamberSD;
class A01EmCalorimeterSD;
class A01HadCalorimeterSD;
class A01HodoscopeSD;

class TVirtualMCRootManager;

/// \ingroup A01
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 12/05/2012
/// \author I. Hrivnacova; IPN, Orsay

class A01MCApplication : public TVirtualMCApplication
{
  public:
    A01MCApplication(const char* name,  const char *title);
    A01MCApplication();
    virtual ~A01MCApplication();

    // methods
    void InitMC(const char *setup);
    void RunMC(Int_t nofEvents);
    void FinishRun();
    void ReadEvent(Int_t i);

    virtual TVirtualMCApplication* CloneForWorker() const;
    virtual void InitForWorker() const;
    virtual void FinishWorkerRun() const;
 
    virtual void ConstructGeometry();
    virtual void InitGeometry();
    virtual void AddParticles();
    virtual void AddIons();
    virtual void GeneratePrimaries();
    virtual void BeginEvent();
    virtual void BeginPrimary();
    virtual void PreTrack();
    virtual void Stepping();
    virtual void PostTrack();
    virtual void FinishPrimary();
    virtual void FinishEvent();
    
    // set methods
    void  SetCuts();
    void  SetWriteStack(Bool_t writeStack);
    void  SetWriteHits(Bool_t writeHits);
    void  SetUseLocalMagField(Bool_t localMagField);
    void  SetVerboseLevel(Int_t verboseLevel);
    
    // get methods
    A01RootDetectorConstruction* GetDetectorConstruction() const;
    A01PrimaryGenerator*         GetPrimaryGenerator() const;
    A01DriftChamberSD*           GetDriftChamberSD1() const;
    A01DriftChamberSD*           GetDriftChamberSD2() const; 
    A01EmCalorimeterSD*          GetEmCalorimeterSD() const; 
    A01HadCalorimeterSD*         GetHadCalorimeterSD() const;
    A01HodoscopeSD*              GetHodoscopeSD1() const;    
    A01HodoscopeSD*              GetHodoscopeSD2() const;    
 
  private:
    // methods
    A01MCApplication(const A01MCApplication& origin);
    void RegisterStack() const;
  
    // data members
    mutable TVirtualMCRootManager* fRootManager;//!< Root manager
    Bool_t                    fWriteStack;      ///< Option to write stack
    Bool_t                    fWriteHits;       ///< Option to write hits
    /// Option to use local magnetic field (working only with Geant4 !)
    Bool_t                    fUseLocalMagField;
    TMCVerbose                fVerbose;         ///< VMC verbose helper
    Ex03MCStack*              fStack;           ///< VMC stack
    A01RootDetectorConstruction*  fDetConstruction; ///< Detector construction
    A01DriftChamberSD*        fDriftChamberSD1; ///< Drift Chamber 1 SD
    A01DriftChamberSD*        fDriftChamberSD2; ///< Drift Chamber 2 SD
    A01EmCalorimeterSD*       fEmCalorimeterSD; ///< Em Calorimeter SD
    A01HadCalorimeterSD*      fHadCalorimeterSD;///< Had Calorimeter SD
    A01HodoscopeSD*           fHodoscopeSD1;    ///< Hodoscope 1 SD
    A01HodoscopeSD*           fHodoscopeSD2;    ///< Hodoscope 2 SD
    A01PrimaryGenerator*      fPrimaryGenerator;///< Primary generator
    A01MagField*              fMagField;        ///< Magnetic field
    Bool_t                    fIsMaster;        ///< If is on master thread

  ClassDef(A01MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

/// (In)Activate writing stack on file
/// \param writeStack  The new value of the option
inline void  A01MCApplication::SetWriteStack(Bool_t writeStack)
{ fWriteStack = writeStack; } 

/// Set verbosity 
/// \param verboseLevel  The new verbose level value
inline void  A01MCApplication::SetVerboseLevel(Int_t verboseLevel)
{ fVerbose.SetLevel(verboseLevel); }

// Set magnetic field
// \param bz  The new field value in z
//inline void  A01MCApplication::SetField(Double_t bz)
//{ fMagField->SetFieldValue(0., 0., bz); }

/// \return The detector construction
inline A01RootDetectorConstruction* A01MCApplication::GetDetectorConstruction() const
{ return fDetConstruction; }

/// \return The primary generator
inline A01PrimaryGenerator* A01MCApplication::GetPrimaryGenerator() const
{ return fPrimaryGenerator; }

/// \return The Drift Chamber 1 SD
inline A01DriftChamberSD* A01MCApplication::GetDriftChamberSD1() const
{ return fDriftChamberSD1; }

/// \return The Drift Chamber 2 SD
inline A01DriftChamberSD* A01MCApplication::GetDriftChamberSD2() const
{ return fDriftChamberSD2; }
 
/// \return The Em Calorimeter SD
inline A01EmCalorimeterSD* A01MCApplication::GetEmCalorimeterSD() const
{ return fEmCalorimeterSD; }

/// \return The Had Calorimeter SD
inline A01HadCalorimeterSD* A01MCApplication::GetHadCalorimeterSD() const
{ return fHadCalorimeterSD; }

/// \return The Hodoscope 1 SD 
inline A01HodoscopeSD* A01MCApplication::GetHodoscopeSD1() const
{ return fHodoscopeSD1; }
    
/// \return The Hodoscope 2 SD
inline A01HodoscopeSD* A01MCApplication::GetHodoscopeSD2() const
{ return fHodoscopeSD2; }  

#endif //A01_MC_APPLICATION_H

