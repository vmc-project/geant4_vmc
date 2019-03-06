#ifndef GARFIELD_MC_APPLICATION_H
#define GARFIELD_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2016 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file ExGarfield/include/MCApplication.h
/// \brief Definition of the ExGarfield::MCApplication class
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

#include "DetectorConstruction.h"
#include "SensitiveDetector.h"

#include <TGeoUniformMagField.h>
#include <TMCVerbose.h>
#include <TStopwatch.h>

class Ex03MCStack;
class TMCRootManager;

namespace VMC
{
namespace ExGarfield
{

class PrimaryGenerator;

/// \ingroup ExGarfield
/// \brief Implementation of the TVirtualMCApplication
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
///
/// \date 28/10/2015
/// \author I. Hrivnacova; IPN, Orsay

class MCApplication : public TVirtualMCApplication
{
  public:
    MCApplication(const char* name,  const char* title);
    MCApplication();
    virtual ~MCApplication();

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
    DetectorConstruction* GetDetectorConstruction() const;
    SensitiveDetector*    GetCalorimeterSD() const;
    PrimaryGenerator*     GetPrimaryGenerator() const;

  private:
    // methods
    MCApplication(const MCApplication& origin);
    void RegisterStack() const;

    // data members
    mutable TMCRootManager* fRootManager; //!< Root manager
    Int_t                 fEventNo;              ///< Event counter
    TMCVerbose            fVerbose;              ///< VMC verbose helper
    Ex03MCStack*          fStack;                ///< VMC stack
    DetectorConstruction* fDetConstruction;      ///< Dector construction
    SensitiveDetector*    fSensitiveDetector;    ///< Calorimeter SD
    PrimaryGenerator*     fPrimaryGenerator;     ///< Primary generator
    Bool_t                fIsMaster;             ///< If is on master thread

  ClassDef(MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

/// Set verbosity
/// \param verboseLevel  The new verbose level value
inline void  MCApplication::SetVerboseLevel(Int_t verboseLevel)
{ fVerbose.SetLevel(verboseLevel); }

/// \return The detector construction
inline DetectorConstruction* MCApplication::GetDetectorConstruction() const
{ return fDetConstruction; }

/// \return The calorimeter sensitive detector
inline SensitiveDetector* MCApplication::GetCalorimeterSD() const
{ return fSensitiveDetector; }

/// \return The primary generator
inline PrimaryGenerator* MCApplication::GetPrimaryGenerator() const
{ return fPrimaryGenerator; }

}
}

#endif //GARFIELD_MC_APPLICATION_H

