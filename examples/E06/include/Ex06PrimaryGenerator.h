// $Id: $
//
// Geant4 ExampleN06 adapted to Virtual Monte Carlo 
//
// Id: ExN06PrimaryGeneratorAction.hh,v 1.4 2003/01/23 15:34:23 maire Exp 
// GEANT4 tag Name: geant4-07-00-cand-01 
//
// by Ivana Hrivnacova, 16.5.2005

#ifndef EX06_PRIMARY_GENERATOR_H
#define EX06_PRIMARY_GENERATOR_H

#include <TVirtualMCApplication.h>

class TVirtualMCStack;
class TVector3;

class Ex06PrimaryGenerator : public TObject
{
  public:
    Ex06PrimaryGenerator(TVirtualMCStack* stack); 
    Ex06PrimaryGenerator();
    virtual ~Ex06PrimaryGenerator();

    // methods
    void GeneratePrimaries();

    // set methods
    void SetParticle(Int_t pdg);
    void SetKinEnergy(Double_t kinEnergy);
    void SetDirection(Double_t dirX, Double_t dirY, Double_t dirZ);
    void SetOptPhotonPolar(Double_t angle);
    void SetNofPrimaries(Int_t nofPrimaries);
 
  private:
    // methods
    void GeneratePrimary();

    // data members
    TVirtualMCStack*  fStack; 
    Int_t             fPdg;
    Double_t          fKinEnergy;
    Double_t          fDirX;
    Double_t          fDirY;
    Double_t          fDirZ;
    Double_t          fPolAngle;   
    Int_t             fNofPrimaries;

  ClassDef(Ex06PrimaryGenerator,1)  //Ex06PrimaryGenerator
};

// inline functions

inline void Ex06PrimaryGenerator::SetParticle(Int_t pdg)
{ fPdg = pdg; }

inline void Ex06PrimaryGenerator::SetKinEnergy(Double_t kinEnergy)
{ fKinEnergy = kinEnergy; }

inline void Ex06PrimaryGenerator::SetOptPhotonPolar(Double_t angle) 
{ fPolAngle = angle; }  

inline void Ex06PrimaryGenerator::SetNofPrimaries(Int_t nofPrimaries)
{ fNofPrimaries = nofPrimaries; }

#endif //EX06_PRIMARY_GENERATOR_H

