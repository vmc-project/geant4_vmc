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

/// \file Ex03MCApplication.h
/// \brief Definition of the Ex03MCApplication class
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

#include "Ex03CalorimeterSD.h"
#include "Ex03DetectorConstruction.h"

#include <TGeoUniformMagField.h>
#include <TMCVerbose.h>

class Ex03MCStack;
class Ex03PrimaryGenerator;

class TMCRootManager;

/// \ingroup E03b
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03MCApplication : public TVirtualMCApplication
{
 public:
  Ex03MCApplication(const char* name, const char* title);
  Ex03MCApplication();
  virtual ~Ex03MCApplication();

  // methods
  void InitMC(const char* setup);
  void RunMC(Int_t nofEvents);
  void FinishRun();
  void ReadEvent(Int_t i);

  virtual TVirtualMCApplication* CloneForWorker() const;
  virtual void InitForWorker() const;
  virtual void FinishWorkerRun() const;

  virtual void ConstructGeometry();
  virtual void ConstructSensitiveDetectors();
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
  virtual void EndOfEvent();
  virtual void FinishEvent();

  // set methods
  void SetPrintModulo(Int_t value);
  void SetVerboseLevel(Int_t verboseLevel);
  void SetControls(Bool_t isConstrols);
  void SetField(Double_t bz);

  // get methods
  Ex03DetectorConstruction* GetDetectorConstruction() const;
  Ex03CalorimeterSD* GetCalorimeterSD() const;
  Ex03PrimaryGenerator* GetPrimaryGenerator() const;

  // method for tests
  void SetOldGeometry(Bool_t oldGeometry = kTRUE);

 private:
  // methods
  Ex03MCApplication(const Ex03MCApplication& origin);
  void RegisterStack() const;

  // data members
  mutable TMCRootManager* fRootManager; //!< Root manager
  Int_t fPrintModulo;  ///< The event modulus number to be printed
  Int_t fEventNo;      ///< Event counter
  TMCVerbose fVerbose; ///< VMC verbose helper
  Ex03MCStack* fStack; ///< VMC stack
  Ex03DetectorConstruction* fDetConstruction; ///< Dector construction
  Ex03CalorimeterSD* fCalorimeterSD;          ///< Calorimeter SD
  Ex03PrimaryGenerator* fPrimaryGenerator;    ///< Primary generator
  TGeoUniformMagField* fMagField;             ///< Magnetic field
  Bool_t fOldGeometry; ///< Option for geometry definition
  Bool_t fIsControls;  ///< Option to activate special controls
  Bool_t fIsMaster;    ///< If is on master thread

  ClassDef(Ex03MCApplication, 1) // Interface to MonteCarlo application
};

// inline functions

/// Set the event modulus number to be printed
/// \param value  The new event modulus number value
inline void Ex03MCApplication::SetPrintModulo(Int_t value)
{
  fPrintModulo = value;
}

/// Set verbosity
/// \param verboseLevel  The new verbose level value
inline void Ex03MCApplication::SetVerboseLevel(Int_t verboseLevel)
{
  fVerbose.SetLevel(verboseLevel);
}

// Set magnetic field
// \param bz  The new field value in z
inline void Ex03MCApplication::SetField(Double_t bz)
{
  fMagField->SetFieldValue(0., 0., bz);
}

/// \return The detector construction
inline Ex03DetectorConstruction*
Ex03MCApplication::GetDetectorConstruction() const
{
  return fDetConstruction;
}

/// \return The calorimeter sensitive detector
inline Ex03CalorimeterSD* Ex03MCApplication::GetCalorimeterSD() const
{
  return fCalorimeterSD;
}

/// \return The primary generator
inline Ex03PrimaryGenerator* Ex03MCApplication::GetPrimaryGenerator() const
{
  return fPrimaryGenerator;
}

/// Switch on/off the old geometry definition  (via VMC functions)
/// \param oldGeometry  If true, geometry definition via VMC functions
inline void Ex03MCApplication::SetOldGeometry(Bool_t oldGeometry)
{
  fOldGeometry = oldGeometry;
}

/// Switch on/off special process controls
/// \param isControls  If true, special process controls setting is activated
inline void Ex03MCApplication::SetControls(Bool_t isControls)
{
  fIsControls = isControls;
}

#endif // EX03_MC_APPLICATION_H
