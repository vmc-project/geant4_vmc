#ifndef EX03_MC_APPLICATION_H
#define EX03_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2014 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex03cMCApplication.h
/// \brief Definition of the Ex03cMCApplication class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author Benedikt Volkel, CERN

#include <initializer_list>

#include <TVirtualMCApplication.h>

#include "Ex03cCalorimeterSD.h"
#include "Ex03cDetectorConstruction.h"

#include <TGeoUniformMagField.h>
#include <TMCVerbose.h>

class Ex03cMCStack;
class Ex03PrimaryGenerator;

class TMCRootManager;

/// \ingroup E03
/// \brief Implementation of the TVirtualMCApplication
///
/// A variant of the Ex03MCApplication class
/// updated for multiple engine runs.
///

/// \date 21/08/2019
/// \author Benedikt Volkel, CERN

class Ex03cMCApplication : public TVirtualMCApplication
{
 public:
  Ex03cMCApplication(const char* name, const char* title,
    Bool_t isMulti = kFALSE, Bool_t splitSimulation = kFALSE);
  Ex03cMCApplication();
  virtual ~Ex03cMCApplication();

  // methods
  void InitMC(const char* setup);
  void InitMC(std::initializer_list<const char*> setupMacros);
  void InitMC();
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
  void SetPrintModulo(Int_t value);
  void SetVerboseLevel(Int_t verboseLevel);
  void SetControls(Bool_t isConstrols);
  void SetField(Double_t bz);
  void SetDebug(Int_t debug);

  // get methods
  Ex03cDetectorConstruction* GetDetectorConstruction() const;
  Ex03cCalorimeterSD* GetCalorimeterSD() const;
  Ex03PrimaryGenerator* GetPrimaryGenerator() const;

  // method for tests
  void SetOldGeometry(Bool_t oldGeometry = kTRUE);

 private:
  // methods
  Ex03cMCApplication(const Ex03cMCApplication& origin);
  void RegisterStack() const;

  // data members
  mutable TMCRootManager* fRootManager; //!< Root manager
  Int_t fPrintModulo;   ///< The event modulus number to be printed
  Int_t fEventNo;       ///< Event counter
  TMCVerbose fVerbose;  ///< VMC verbose helper
  Ex03cMCStack* fStack; ///< VMC stack
  Ex03cDetectorConstruction* fDetConstruction; ///< Dector construction
  Ex03cCalorimeterSD* fCalorimeterSD;          ///< Calorimeter SD
  Ex03PrimaryGenerator* fPrimaryGenerator;     ///< Primary generator
  TGeoUniformMagField* fMagField;              ///< Magnetic field
  Bool_t fOldGeometry;     ///< Option for geometry definition
  Bool_t fIsControls;      ///< Option to activate special controls
  Bool_t fIsMaster;        ///< If is on master thread
  Bool_t fIsMultiRun;      ///< Flag if having multiple engines
  Bool_t fSplitSimulation; ///< Split geometry given user criteria
  Int_t fG3Id;             ///< engine ID of Geant3
  Int_t fG4Id;             ///< engine ID of Geant4
  Int_t fDebug;           ///< debug option for multiple run

  ClassDef(Ex03cMCApplication, 1) // Interface to MonteCarlo application
};

// Define typedef so that we can use the E03 macros for all examples variants
typedef Ex03cMCApplication Ex03MCApplication;

// inline functions

/// Set the event modulus number to be printed
/// \param value  The new event modulus number value
inline void Ex03cMCApplication::SetPrintModulo(Int_t value)
{
  fPrintModulo = value;
}

/// Set verbosity
/// \param verboseLevel  The new verbose level value
inline void Ex03cMCApplication::SetVerboseLevel(Int_t verboseLevel)
{
  fVerbose.SetLevel(verboseLevel);
}

/// Set magnetic field
/// \param bz  The new field value in z
inline void Ex03cMCApplication::SetField(Double_t bz)
{
  fMagField->SetFieldValue(0., 0., bz);
}

/// Set debug option for multiple run
inline void Ex03cMCApplication::SetDebug(Int_t debug)
{
  fDebug = debug;
}

/// \return The detector construction
inline Ex03cDetectorConstruction*
Ex03cMCApplication::GetDetectorConstruction() const
{
  return fDetConstruction;
}

/// \return The calorimeter sensitive detector
inline Ex03cCalorimeterSD* Ex03cMCApplication::GetCalorimeterSD() const
{
  return fCalorimeterSD;
}

/// \return The primary generator
inline Ex03PrimaryGenerator* Ex03cMCApplication::GetPrimaryGenerator() const
{
  return fPrimaryGenerator;
}

/// Switch on/off the old geometry definition  (via VMC functions)
/// \param oldGeometry  If true, geometry definition via VMC functions
inline void Ex03cMCApplication::SetOldGeometry(Bool_t oldGeometry)
{
  fOldGeometry = oldGeometry;
}

/// Switch on/off special process controls
/// \param isControls  If true, special process controls setting is activated
inline void Ex03cMCApplication::SetControls(Bool_t isControls)
{
  fIsControls = isControls;
}

#endif // EX03_MC_APPLICATION_H
