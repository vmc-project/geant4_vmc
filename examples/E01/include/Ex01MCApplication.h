// $Id: Ex01MCApplication.h,v 1.2 2003/07/22 06:37:48 brun Exp $
//
// Geant4 ExampleN01 adapted to Virtual Monte Carlo 
//
// Class Ex01MCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 5.4.2002

#ifndef EX01_MC_APPLICATION_H
#define EX01_MC_APPLICATION_H

#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>

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

  private:
    // methods
    void ConstructMaterialsOld();
    void ConstructMaterials();
    void ConstructVolumesOld();
    void ConstructVolumes();
  
    // data members
    TVirtualMCStack*  fStack;
    Int_t  fImedAr;
    Int_t  fImedAl;
    Int_t  fImedPb;

  ClassDef(Ex01MCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

inline Ex01MCApplication* Ex01MCApplication::Instance()
{ return (Ex01MCApplication*)(TVirtualMCApplication::Instance()); }

#endif //EX01_MC_APPLICATION_H

