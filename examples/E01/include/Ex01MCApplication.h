#ifndef EX01_MC_APPLICATION_H
#define EX01_MC_APPLICATION_H

// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file Ex01MCApplication.h
/// \brief Definition of the Ex01MCApplication class 
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>

class TVirtualMagField;

/// \ingroup E01
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex01MCApplication : public TVirtualMCApplication
{
  public:
    Ex01MCApplication(const char *name, const char *title);
    Ex01MCApplication();
    virtual ~Ex01MCApplication();
  
    // static access method
    static Ex01MCApplication* Instance(); 

    // methods
    void InitMC(const char *setup);
    void RunMC(Int_t nofEvents);
    void FinishRun();
            
    virtual TVirtualMCApplication* CloneForWorker() const; 
    virtual void InitForWorker() const; 
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

    // methods for tests
    void SetOldGeometry(Bool_t oldGeometry = kTRUE);
    void TestVMCGeometryGetters();

  private:
    // methods
    void ConstructMaterials();
    void ConstructVolumes();
  
    // data members
    TVirtualMCStack*  fStack;       ///< The VMC stack
    TVirtualMagField* fMagField;    ///< The magnetic field 
    Int_t             fImedAr;      ///< The Argon gas medium Id
    Int_t             fImedAl;      ///< The Aluminium medium Id
    Int_t             fImedPb;      ///< The Lead medium Id
    Bool_t            fOldGeometry; ///< Option for geometry definition

  ClassDef(Ex01MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

inline Ex01MCApplication* Ex01MCApplication::Instance()
{ 
  /// \return The MC application instance
  return (Ex01MCApplication*)(TVirtualMCApplication::Instance()); 
}

inline void Ex01MCApplication::SetOldGeometry(Bool_t oldGeometry)
{ 
  /// Select old geometry definition (via TVirtualMC)
  fOldGeometry = oldGeometry; 
}

#endif //EX01_MC_APPLICATION_H

