#ifndef EX03_MC_APPLICATION_H
#define EX03_MC_APPLICATION_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex03MCApplication.h 
/// \brief Definition of the Ex03MCApplication class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>
#include <TGeoUniformMagField.h> 
#include "TMCVerbose.h"

#include "Ex03DetectorConstruction.h"
#include "Ex03CalorimeterSD.h"
#include "Ex02RootManager.h"

class Ex03MCStack;
class Ex03PrimaryGenerator;

/// \ingroup E03
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 06/03/2003
/// \author I. Hrivnacova; IPN, Orsay

class Ex03MCApplication : public TVirtualMCApplication
{
  public:
    Ex03MCApplication(const char* name,  const char *title, 
                      FileMode fileMode = kWrite);
    Ex03MCApplication();
    virtual ~Ex03MCApplication();
  
    // static access method
    static Ex03MCApplication* Instance(); 

    // methods
    void InitMC(const char *setup);
    void RunMC(Int_t nofEvents);
    void FinishRun();
    void ReadEvent(Int_t i);
 
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
    void  SetPrintModulo(Int_t value);
    void  SetVerboseLevel(Int_t verboseLevel);
    
    // get methods
    Ex03DetectorConstruction* GetDetectorConstruction() const;
    Ex03CalorimeterSD*        GetCalorimeterSD() const;
    Ex03PrimaryGenerator*     GetPrimaryGenerator() const;

    // method for tests
    void SetOldGeometry(Bool_t oldGeometry = kTRUE);
 
  private:
    // methods
    void RegisterStack();
  
    // data members
    Int_t                     fPrintModulo;     ///< The event modulus number to be printed 
    Int_t                     fEventNo;         ///< Event counter
    TMCVerbose                fVerbose;         ///< VMC verbose helper
    Ex03MCStack*              fStack;           ///< VMC stack
    Ex03DetectorConstruction* fDetConstruction; ///< Dector construction
    Ex03CalorimeterSD*        fCalorimeterSD;   ///< Calorimeter SD
    Ex03PrimaryGenerator*     fPrimaryGenerator;///< Primary generator
    TGeoUniformMagField*      fMagField;        ///< Magnetic field
    Ex02RootManager           fRootManager;     ///< Root manager 
    Bool_t                    fOldGeometry;     ///< Option for geometry definition

  ClassDef(Ex03MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

/// \return The singleton instance 
inline Ex03MCApplication* Ex03MCApplication::Instance()
{ return (Ex03MCApplication*)(TVirtualMCApplication::Instance()); }

/// Set the event modulus number to be printed 
/// \param value  The new event modulus number value
inline void  Ex03MCApplication::SetPrintModulo(Int_t value)  
{ fPrintModulo = value; }

/// Set verbosity 
/// \param verboseLevel  The new verbose level value
inline void  Ex03MCApplication::SetVerboseLevel(Int_t verboseLevel)
{ fVerbose.SetLevel(verboseLevel); }

/// Set magnetic field
/// \param bz  The new field value in z
//inline void  Ex03MCApplication::SetField(Double_t bz)
//{ fMagField->SetFieldValue(0., 0., bz); }

/// \return The detector construction
inline Ex03DetectorConstruction* Ex03MCApplication::GetDetectorConstruction() const
{ return fDetConstruction; }

/// \return The calorimeter sensitive detector
inline Ex03CalorimeterSD* Ex03MCApplication::GetCalorimeterSD() const
{ return fCalorimeterSD; }

/// \return The primary generator
inline Ex03PrimaryGenerator* Ex03MCApplication::GetPrimaryGenerator() const
{ return fPrimaryGenerator; }

/// Switch on/off the old geometry definition  (via VMC functions)
/// \param oldGeometry  If true, geometry definition via VMC functions
inline void Ex03MCApplication::SetOldGeometry(Bool_t oldGeometry)
{ fOldGeometry = oldGeometry; }

#endif //EX03_MC_APPLICATION_H

