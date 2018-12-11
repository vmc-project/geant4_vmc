#ifndef MC_APPLICATION_H
#define MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file MCApplication.h
/// \brief Definition of the MCApplication class 
///
/// Geant4 Monopole example adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include <TVirtualMCApplication.h>
#include <TGeoUniformMagField.h>

class TVirtualMagField;
class TMCRootManager;
class Ex03MCStack;

namespace VMC
{
namespace Monopole
{

class DetectorConstruction;

/// \ingroup Monopole
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class MCApplication : public TVirtualMCApplication
{
  public:
    MCApplication(const char *name, const char *title);
    MCApplication();
    virtual ~MCApplication();
  
    // static access method
    static MCApplication* Instance(); 

    // methods
    void InitMC(const char *setup);
    void RunMC(Int_t nofEvents);
    void FinishRun();
            
    // virtual TVirtualMCApplication* CloneForWorker() const; 
    // virtual void InitForWorker() const; 
    // virtual void FinishWorkerRun() const
    
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

    void SetBinSize(Double_t binSize);

    DetectorConstruction* GetDetectorConstruction() const;

  private:  
    // methods
    MCApplication(const MCApplication& origin);
    void  RegisterStack() const;

    // data members
    mutable TMCRootManager* fRootManager; //!< Root manager
    Ex03MCStack*           fStack;            ///< The VMC stack
    DetectorConstruction*  fDetConstruction;  ///< Dector construction
    TGeoUniformMagField*   fMagField;         ///< Magnetic field
    Double_t               fBinSize;          ///< Edep histogram bin size
    Double_t               fOffsetX;          ///< The Edep histogram offset
    Double_t               fProjRange;        ///< Projected range
    Double_t               fProjRange2;       ///< Projected range square
    Int_t                  fImedAl;           ///< The Aluminium medium Id
    Int_t                  fNofEvents;        ///< Number of events
    Bool_t                 fIsMaster;         ///< If is on master thread

  ClassDef(MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

/// \return The MC application instance
inline MCApplication* MCApplication::Instance()
{ 
  return (MCApplication*)(TVirtualMCApplication::Instance()); 
}

/// \return the detector construction 
inline DetectorConstruction* MCApplication::GetDetectorConstruction() const
{
  return fDetConstruction;
}

}
}

#endif //EX01_MC_APPLICATION_H

