#ifndef MC_APPLICATION_H
#define MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TR/include/MCApplication.h
/// \brief Definition of the MCApplication class
///
/// Geant4 TestEm10 adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

#include <TGeoUniformMagField.h>
#include <TMCVerbose.h>

class Ex03MCStack;

class TVirtualMCRootManager;

namespace VMC
{
namespace TR
{

class DetectorConstruction;
class SensitiveDetector;
class PrimaryGenerator;

/// \ingroup TR
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 18/12/2015
/// \author I. Hrivnacova; IPN, Orsay

class MCApplication : public TVirtualMCApplication
{
 public:
  MCApplication(const char* name, const char* title);
  MCApplication();
  virtual ~MCApplication();

  // methods
  void InitMC(const char* setup);
  void RunMC(Int_t nofEvents);
  void FinishRun();

  virtual TVirtualMCApplication* CloneForWorker() const;
  virtual void InitForWorker() const;
  virtual void FinishWorkerRun() const;

  virtual void ConstructGeometry();
  virtual void InitGeometry();
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

  // get methods
  DetectorConstruction* GetDetectorConstruction() const;
  SensitiveDetector* GetSensitiveDetector() const;
  PrimaryGenerator* GetPrimaryGenerator() const;

  // method for tests
  void SetOldGeometry(Bool_t oldGeometry = kTRUE);

 private:
  // methods
  MCApplication(const MCApplication& origin);
  void RegisterStack() const;
  void BookHisto() const;

  // data members
  mutable TVirtualMCRootManager* fRootManager; //!< Root manager
  Int_t fPrintModulo;  ///< The event modulus number to be printed
  Int_t fEventNo;      ///< Event counter
  TMCVerbose fVerbose; ///< VMC verbose helper
  Ex03MCStack* fStack; ///< VMC stack
  DetectorConstruction* fDetConstruction; ///< Dector construction
  SensitiveDetector* fSensitiveDetector;  ///< Absorber SD
  PrimaryGenerator* fPrimaryGenerator;    ///< Primary generator
  TGeoUniformMagField* fMagField;         ///< Magnetic field
  Bool_t fOldGeometry;                    ///< Option for geometry definition
  Bool_t fIsControls; ///< Option to activate special controls
  Bool_t fIsMaster;   ///< If is on master thread

  ClassDef(MCApplication, 1) // Interface to MonteCarlo application
};

// inline functions

/// Set the event modulus number to be printed
/// \param value  The new event modulus number value
inline void MCApplication::SetPrintModulo(Int_t value) { fPrintModulo = value; }

/// Set verbosity
/// \param verboseLevel  The new verbose level value
inline void MCApplication::SetVerboseLevel(Int_t verboseLevel)
{
  fVerbose.SetLevel(verboseLevel);
}

// Set magnetic field
// \param bz  The new field value in z
inline void MCApplication::SetField(Double_t bz)
{
  fMagField->SetFieldValue(0., 0., bz);
}

/// \return The detector construction
inline DetectorConstruction* MCApplication::GetDetectorConstruction() const
{
  return fDetConstruction;
}

/// \return The calorimeter sensitive detector
inline SensitiveDetector* MCApplication::GetSensitiveDetector() const
{
  return fSensitiveDetector;
}

/// \return The primary generator
inline PrimaryGenerator* MCApplication::GetPrimaryGenerator() const
{
  return fPrimaryGenerator;
}

/// Switch on/off the old geometry definition  (via VMC functions)
/// \param oldGeometry  If true, geometry definition via VMC functions
inline void MCApplication::SetOldGeometry(Bool_t oldGeometry)
{
  fOldGeometry = oldGeometry;
}

/// Switch on/off special process controls
/// \param isControls  If true, special process controls setting is activated
inline void MCApplication::SetControls(Bool_t isControls)
{
  fIsControls = isControls;
}

} // namespace TR
} // namespace VMC

#endif // MC_APPLICATION_H
