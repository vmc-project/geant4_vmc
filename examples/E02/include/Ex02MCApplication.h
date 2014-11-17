#ifndef EX02_MC_APPLICATION_H
#define EX02_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex02MCApplication.h
/// \brief Definition of the Ex02MCApplication class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>

#include "Ex02DetectorConstruction.h"
#include "Ex02TrackerSD.h"

#include <TVirtualMCRootManager.h>

class Ex02MCStack;

class TVirtualMagField;

/// \ingroup E02
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 21/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex02MCApplication : public TVirtualMCApplication
{
  public:
    Ex02MCApplication(const char* name,  const char *title);
    Ex02MCApplication();
    virtual ~Ex02MCApplication();

    // methods
    void InitMC(const char *setup);
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
    
    void   ReadEvent(Int_t i);

    // method for tests
    void SetOldGeometry(Bool_t oldGeometry = kTRUE);

  private:
    // methods
    Ex02MCApplication(const Ex02MCApplication& origin);
    void RegisterStack() const;
    
    // data members
    mutable TVirtualMCRootManager*   fRootManager;     //!< Root manager 
    Ex02MCStack*             fStack;           ///< VMC stack
    Ex02DetectorConstruction fDetConstruction; ///< Dector construction
    Ex02TrackerSD*           fTrackerSD;       ///< Tracker SD
    TVirtualMagField*        fMagField;        ///< Magnetic field
    Bool_t                   fOldGeometry;     ///< Option for geometry definition

  ClassDef(Ex02MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

/// Switch on/off the old geometry definition  (via VMC functions)
/// \param oldGeometry  If true, geometry definition via VMC functions
inline void Ex02MCApplication::SetOldGeometry(Bool_t oldGeometry)
{ fOldGeometry = oldGeometry; }

#endif //EX02_MC_APPLICATION_H

