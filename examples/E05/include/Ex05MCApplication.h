#ifndef EX05_MC_APPLICATION_H
#define EX05_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex05MCApplication.h 
/// \brief Definition of the Ex05MCApplication class 
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

#include "Ex05DetectorConstruction.h"
#include "Ex05SensitiveDetector.h"

#include <TGeoUniformMagField.h>
#include <TMCVerbose.h>
#include <TStopwatch.h>

class Ex03MCStack;
class Ex05PrimaryGenerator;

class TVirtualMCRootManager;

/// \ingroup E05
/// \brief Implementation of the TVirtualMCApplication
///
/// Geant4 gflash adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

class Ex05MCApplication : public TVirtualMCApplication
{
  public:
    Ex05MCApplication(const char* name,  const char* title);
    Ex05MCApplication();
    virtual ~Ex05MCApplication();
  
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
    virtual void GeneratePrimaries();
    virtual void BeginEvent();
    virtual void BeginPrimary();
    virtual void PreTrack();
    virtual void Stepping();
    virtual void PostTrack();
    virtual void FinishPrimary();
    virtual void FinishEvent();
    
    // set methods
    void  SetVerboseLevel(Int_t verboseLevel);

    // get methods
    Ex05DetectorConstruction* GetDetectorConstruction() const;
    Ex05SensitiveDetector*    GetCalorimeterSD() const;
    Ex05PrimaryGenerator*     GetPrimaryGenerator() const;
 
  private:
    // methods
    Ex05MCApplication(const Ex05MCApplication& origin);
    void RegisterStack() const;
    void ComputeEventStatistics() const;
  
    // data members
    mutable TVirtualMCRootManager* fRootManager; //!< Root manager
    Int_t                     fEventNo;          ///< Event counter
    TMCVerbose                fVerbose;          ///< VMC verbose helper
    Ex03MCStack*              fStack;            ///< VMC stack
    Ex05DetectorConstruction* fDetConstruction;  ///< Dector construction
    Ex05SensitiveDetector*    fSensitiveDetector;///< Calorimeter SD
    Ex05PrimaryGenerator*     fPrimaryGenerator; ///< Primary generator
    Bool_t                    fIsMaster;         ///< If is on master thread
    TStopwatch*               fEventTimer;       ///< Event timer

  ClassDef(Ex05MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

/// Set verbosity 
/// \param verboseLevel  The new verbose level value
inline void  Ex05MCApplication::SetVerboseLevel(Int_t verboseLevel)
{ fVerbose.SetLevel(verboseLevel); }

/// \return The detector construction
inline Ex05DetectorConstruction* Ex05MCApplication::GetDetectorConstruction() const
{ return fDetConstruction; }

/// \return The calorimeter sensitive detector
inline Ex05SensitiveDetector* Ex05MCApplication::GetCalorimeterSD() const
{ return fSensitiveDetector; }

/// \return The primary generator
inline Ex05PrimaryGenerator* Ex05MCApplication::GetPrimaryGenerator() const
{ return fPrimaryGenerator; }

#endif //EX05_MC_APPLICATION_H

