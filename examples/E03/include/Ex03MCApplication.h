// $Id: Ex03MCApplication.h,v 1.1 2003/03/17 14:56:51 brun Exp $
//
// Geant4 ExampleN03 adapted to Virtual Monte Carlo 
//
// Class Ex03MCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 6.3.2003

#ifndef EX03_MC_APPLICATION_H
#define EX03_MC_APPLICATION_H

#include <TVirtualMCApplication.h>
#include "TMCVerbose.h"

#include "Ex03DetectorConstruction.h"
#include "Ex03CalorimeterSD.h"
#include "Ex02RootManager.h"

class Ex03MCStack;
class Ex03PrimaryGenerator;

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
    virtual void GeneratePrimaries();
    virtual void BeginEvent();
    virtual void BeginPrimary();
    virtual void PreTrack();
    virtual void Stepping();
    virtual void PostTrack();
    virtual void FinishPrimary();
    virtual void FinishEvent();
    virtual void Field(const Double_t* x, Double_t* b) const;
    
    // set methods
    void  SetPrintModulo(Int_t value);
    void  SetVerboseLevel(Int_t verboseLevel);
    void  SetField(Double_t bz);
    
    // get methods
    Ex03DetectorConstruction* GetDetectorConstruction() const;
    Ex03CalorimeterSD*        GetCalorimeterSD() const;
    Ex03PrimaryGenerator*     GetPrimaryGenerator() const;
 
  private:
    // methods
    void RegisterStack();
  
    // data members
    Int_t                     fPrintModulo;
    Int_t                     fEventNo;
    TMCVerbose                fVerbose;
    Ex03MCStack*              fStack;
    Ex03DetectorConstruction* fDetConstruction;
    Ex03CalorimeterSD*        fCalorimeterSD;
    Ex03PrimaryGenerator*     fPrimaryGenerator;
    Double_t*                 fFieldB;
    Ex02RootManager           fRootManager;

  ClassDef(Ex03MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

inline Ex03MCApplication* Ex03MCApplication::Instance()
{ return (Ex03MCApplication*)(TVirtualMCApplication::Instance()); }

inline void  Ex03MCApplication::SetPrintModulo(Int_t value)  
{ fPrintModulo = value; }

inline void  Ex03MCApplication::SetVerboseLevel(Int_t verboseLevel)
{ fVerbose.SetLevel(verboseLevel); }

inline void  Ex03MCApplication::SetField(Double_t bz)
{ fFieldB[2] = bz; }

inline Ex03DetectorConstruction* Ex03MCApplication::GetDetectorConstruction() const
{ return fDetConstruction; }

inline Ex03CalorimeterSD* Ex03MCApplication::GetCalorimeterSD() const
{ return fCalorimeterSD; }

inline Ex03PrimaryGenerator* Ex03MCApplication::GetPrimaryGenerator() const
{ return fPrimaryGenerator; }

#endif //EX03_MC_APPLICATION_H

