#ifndef TGEANT4_H
#define TGEANT4_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TGeant4.h
/// \brief Definition of the TGeant4 class 
///
/// \author I. Hrivnacova; IPN Orsay

#include "TG4ApplicationState.h"

#include <G4ApplicationState.hh>

#include <TVirtualMC.h>
#include <TMCProcess.h>
#include <TMCOptical.h>
#include <TArrayI.h>
#include <TString.h>

class TG4RunConfiguration;
class TG4StateManager;
class TG4GeometryManager;
class TG4SDManager;
class TG4PhysicsManager;
class TG4StepManager;
class TG4VisManager;
class TG4RunManager;

class G4VisExecutive;

class TGeoHMatrix;
class TArrayD;
class TString;

/// \ingroup run
/// \brief Implementation of the TVirtualMC interface for Geant4.                      
///
/// \author I. Hrivnacova; IPN, Orsay

class TGeant4: public TVirtualMC
{
  public:
    TGeant4(const char* name, const char* title,
            TG4RunConfiguration* configuration, 
            int argc = 0, char** argv = 0);
    virtual ~TGeant4();

    //
    // methods for building/management of geometry
    // ------------------------------------------------
    //

    // info about supporting geometry defined via Root
    virtual Bool_t IsRootGeometrySupported() const;

    // functions from GBASE 
    virtual void  FinishGeometry(); 
                  //Ggclos(); 

    // functions from GCONS 
    virtual void  Gfmate(Int_t imat, char *name, Float_t &a, Float_t &z,  
                           Float_t &dens, Float_t &radl, Float_t &absl,
                         Float_t* ubuf, Int_t& nbuf); 
    virtual void  Gfmate(Int_t imat, char *name, Double_t &a, Double_t &z,  
                           Double_t &dens, Double_t &radl, Double_t &absl,
                         Double_t* ubuf, Int_t& nbuf);
    virtual void  Gckmat(Int_t itmed, char* natmed);


    // detector composition
    virtual void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Float_t* buf, Int_t nwbuf);
    virtual void  Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Double_t* buf, Int_t nwbuf);
    virtual void  Mixture(Int_t& kmat, const char *name, Float_t *a, 
                     Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat);
    virtual void  Mixture(Int_t& kmat, const char *name, Double_t *a, 
                     Double_t *z, Double_t dens, Int_t nlmat, Double_t *wmat);
    virtual void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
                     Double_t stmin, Float_t* ubuf, Int_t nbuf);
    virtual void  Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
                     Double_t stmin, Double_t* ubuf, Int_t nbuf);
    virtual void  Matrix(Int_t& krot, Double_t thetaX, Double_t phiX, 
                     Double_t thetaY, Double_t phiY, Double_t thetaZ, 
                     Double_t phiZ);
    virtual void  SetCerenkov(Int_t itmed, Int_t npckov, Float_t *ppckov,
                     Float_t *absco, Float_t *effic, Float_t *rindex);
    virtual void  SetCerenkov(Int_t itmed, Int_t npckov, Double_t *ppckov,
                     Double_t *absco, Double_t *effic, Double_t *rindex);

    // new (more general functions) for definition of surfaces
    // and material properties for optical physics
    virtual void  DefineOpSurface(const char *name,
                         EMCOpSurfaceModel model,
                         EMCOpSurfaceType surfaceType,
                         EMCOpSurfaceFinish surfaceFinish,
                         Double_t sigmaAlpha);
    virtual void  SetBorderSurface(const char *name,
                         const char* vol1Name, int vol1CopyNo,
                         const char* vol2Name, int vol2CopyNo,
                         const char* opSurfaceName);
    virtual void  SetSkinSurface(const char *name,
                         const char* volName,
                         const char* opSurfaceName);
    virtual void  SetMaterialProperty(
                         Int_t itmed, const char* propertyName, 
                         Int_t np, Double_t* pp, Double_t* values);
    virtual void  SetMaterialProperty(
                         Int_t itmed, const char* propertyName,
                         Double_t value);
    virtual void  SetMaterialProperty(
                         const char* surfaceName, const char* propertyName, 
                         Int_t np, Double_t* pp, Double_t* values);
    
    // functions for access to geometry
    virtual Bool_t GetTransformation(const TString& volumePath, 
                         TGeoHMatrix& matrix);
    virtual Bool_t GetShape(const TString& volumePath, 
                         TString& shapeType, TArrayD& par);
    virtual Bool_t GetMaterial(Int_t imat, TString& name,
                         Double_t& a, Double_t& z, Double_t& density,
                         Double_t& radl, Double_t& inter, TArrayD& par);
    virtual Bool_t GetMaterial(const TString& volumeName,
                          TString& name, Int_t& imat,
                         Double_t& a, Double_t& z, Double_t& density,
                         Double_t& radl, Double_t& inter, TArrayD& par);
    virtual Bool_t GetMedium(const TString& volumeName,
                         TString& name, Int_t& imed,
                         Int_t& nmat, Int_t& isvol, Int_t& ifield,
                         Double_t& fieldm, Double_t& tmaxfd, Double_t& stemax,
                         Double_t& deemax, Double_t& epsil, Double_t& stmin,
                         TArrayD& par);

    // functions from GGEOM 
    virtual Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Double_t *upar, Int_t np); 
    virtual Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Float_t *upar, Int_t np); 
    virtual void  Gsdvn(const char *name, const char *mother, Int_t ndiv, 
                        Int_t iaxis); 
    virtual void  Gsdvn2(const char *name, const char *mother, Int_t ndiv, 
                         Int_t iaxis, Double_t c0i, Int_t numed); 
    virtual void  Gsdvt(const char *name, const char *mother, Double_t step, 
                        Int_t iaxis, Int_t numed, Int_t ndvmx); 
    virtual void  Gsdvt2(const char *name, const char *mother, Double_t step, 
                         Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx); 
    virtual void  Gsord(const char *name, Int_t iax); 
    virtual void  Gspos(const char *name, Int_t nr, const char *mother,  
                        Double_t x, Double_t y, Double_t z, Int_t irot, 
                        const char *konly="ONLY"); 
    virtual void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Double_t *upar, Int_t np);
    virtual void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Float_t *upar, Int_t np); 
    virtual void  Gsbool(const char* onlyVolName, const char* manyVolName);
    
    // Euclid                       
    virtual void WriteEuclid(const char* fileName, const char* topVol, 
                             Int_t number, Int_t nlevel);
                               
    // set geometry from Root (built via TGeo)
    virtual void SetRootGeometry();                   
    
    // Activate the parameters defined in tracking media
    // (DEEMAX, STMIN, STEMAX), which are, be default, ignored.
    // In Geant4 case, only STEMAX is taken into account.
    virtual void SetUserParameters(Bool_t isUserParameters);

    // get methods
    virtual Int_t VolId(const Text_t* volName) const;
    virtual const char* VolName(Int_t id) const;
    virtual Int_t MediumId(const Text_t* medName) const;
    virtual Int_t NofVolumes() const;
    virtual Int_t NofVolDaughters(const char* volName) const;
    virtual const char*  VolDaughterName(const char* volName, Int_t i) const;
    virtual Int_t        VolDaughterCopyNo(const char* volName, Int_t i) const;
    virtual Int_t VolId2Mate(Int_t id) const;

    //
    // methods for physics management
    // ------------------------------------------------
    //
 
    virtual void BuildPhysics();
    virtual void Gstpar(Int_t itmed, const char *param, Double_t parval); 

    // set methods
    virtual Bool_t SetCut(const char* cutName, Double_t cutValue);
    virtual Bool_t SetProcess(const char* flagName, Int_t flagValue);
    virtual Bool_t DefineParticle(Int_t pdg, const char* name,
                        TMCParticleType mcType, 
                        Double_t mass, Double_t charge, Double_t lifetime);
    virtual Bool_t DefineParticle(Int_t pdg, const char* name,
                        TMCParticleType mcType, 
                        Double_t mass, Double_t charge, Double_t lifetime, 
                        const TString& pType, Double_t width, 
                        Int_t iSpin, Int_t iParity, Int_t iConjugation, 
                        Int_t iIsospin, Int_t iIsospinZ, Int_t gParity,
                        Int_t lepton, Int_t baryon,
                        Bool_t stable, Bool_t shortlived = kFALSE,
                        const TString& subType = "",
                        Int_t antiEncoding = 0, Double_t magMoment = 0.0,
                        Double_t excitation = 0.0);
    virtual Bool_t DefineIon(const char* name, Int_t Z, Int_t A,  
                        Int_t Q, Double_t excEnergy, Double_t mass);
    virtual Double_t Xsec(char* reac, Double_t energy, Int_t part, Int_t mate);

        // particle table usage         
    virtual Int_t IdFromPDG(Int_t pdgID) const;
    virtual Int_t PDGFromId(Int_t mcID) const;

        // get methods
    virtual TString   ParticleName(Int_t pdg) const;          
    virtual Double_t  ParticleMass(Int_t pdg) const;          
    virtual Double_t  ParticleCharge(Int_t pdg) const;          
    virtual Double_t  ParticleLifeTime(Int_t pdg) const;          
    virtual TMCParticleType ParticleMCType(Int_t pdg) const;

    //
    // methods for step management
    // ------------------------------------------------
    //

    // action methods
    virtual void StopTrack();
    virtual void StopEvent();   
    virtual void StopRun();   

    // set methods
    virtual void SetMaxStep(Double_t);
    virtual void SetMaxNStep(Int_t);
    virtual void SetUserDecay(Int_t);     //NEW
    virtual void ForceDecayTime(Float_t); //NEW
    virtual Bool_t SetDecayMode(Int_t pdg, Float_t bratio[6], Int_t mode[6][3]); //new 

    // get methods
         // tracking volume(s) 
    virtual Int_t CurrentVolID(Int_t& copyNo) const;
    virtual Int_t CurrentVolOffID(Int_t off, Int_t& copyNo) const;
    virtual const char* CurrentVolName() const;
    virtual const char* CurrentVolOffName(Int_t off) const;
    virtual const char* CurrentVolPath();
    virtual Bool_t CurrentBoundaryNormal(
                    Double_t &x, Double_t &y, Double_t &z) const;
    virtual Int_t  CurrentMaterial(Float_t &a, Float_t &z, 
                    Float_t &dens, Float_t &radl, Float_t &absl) const;  
    virtual Int_t CurrentMedium() const;
    virtual void  Gmtod(Float_t* xm, Float_t* xd, Int_t iflag);
    virtual void  Gmtod(Double_t* xm, Double_t* xd, Int_t iflag);
    virtual void  Gdtom(Float_t* xd, Float_t* xm, Int_t iflag);
    virtual void  Gdtom(Double_t* xd, Double_t* xm, Int_t iflag);
    virtual Double_t MaxStep() const;
    virtual Int_t GetMaxNStep() const;
    virtual Int_t GetMedium() const;

        // tracking particle 
        // dynamic properties
    virtual void    TrackPosition(TLorentzVector& position) const;
    virtual void    TrackPosition(Double_t& x, Double_t& y, Double_t& z) const;
    virtual void    TrackMomentum(TLorentzVector& momentum) const;
    virtual void    TrackMomentum(Double_t& px, Double_t& py, Double_t&pz,
                                  Double_t& etot) const;
    virtual Double_t TrackStep() const;
    virtual Double_t TrackLength() const; 
    virtual Double_t TrackTime() const;
    virtual Double_t Edep() const;
        // static properties
    virtual Int_t    TrackPid() const;
    virtual Double_t TrackCharge() const;
    virtual Double_t TrackMass() const;
    virtual Double_t Etot() const;

        // track status
    virtual Bool_t  IsTrackInside() const;
    virtual Bool_t  IsTrackEntering() const;
    virtual Bool_t  IsTrackExiting() const;
    virtual Bool_t  IsTrackOut() const;
    virtual Bool_t  IsTrackDisappeared() const;
    virtual Bool_t  IsTrackStop() const;
    virtual Bool_t  IsTrackAlive() const;
    virtual Bool_t  IsNewTrack() const;

        // secondaries
    virtual Int_t NSecondaries() const;
    virtual void  GetSecondary(Int_t isec, Int_t& particleId, 
                    TLorentzVector& position, TLorentzVector& momentum);
    virtual TMCProcess ProdProcess(Int_t isec) const; 
    virtual Int_t  StepProcesses(TArrayI &proc) const;
  
    //
    // methods for visualization
    // ------------------------------------------------
    //
    // functions for drawing
    virtual void  DrawOneSpec(const char* name);
    virtual void  Gsatt(const char* name, const char* att, Int_t val);
    virtual void  Gdraw(const char*,Double_t theta, Double_t phi, Double_t psi,
                        Double_t u0, Double_t v0, Double_t ul, Double_t vl);

    //
    // Geant3 specific methods
    // !!! to be removed with move to TGeo
    //
    virtual void Gdopt(const char* name , const char* value);
    virtual void SetClipBox(const char *name, Double_t xmin, Double_t xmax,
                       Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax);
    virtual void DefaultRange();
    virtual void Gdhead(Int_t isel, const char* name, Double_t chrsiz);   
    virtual void Gdman(Double_t u, Double_t v, const char* type);
    virtual void InitLego();

    //
    // methods for run control
    // ------------------------------------------------
    //

    virtual void   Init();
    virtual void   ProcessEvent();
    virtual Bool_t ProcessRun(Int_t nofEvents);
    virtual void   SetCollectTracks(Bool_t collectTracks);
    virtual Bool_t IsCollectTracks() const;

        // UI control methods
    void StartGeantUI();        
    void StartRootUI();        
    void ProcessGeantMacro(const char* macroName);
    void ProcessGeantCommand(const char* commandPath);

        // get methods
    virtual Int_t   CurrentEvent() const; 
    virtual Bool_t  SecondariesAreOrdered() const;

  private:
    /// Not implemented
    TGeant4();
    /// Not implemented
    TGeant4(const TGeant4& right);
    /// Not implemented
    TGeant4& operator=(const TGeant4& right);
    
    // methods
    Bool_t CheckApplicationState(const TString& methodName,
                                 TG4ApplicationState requiredState,
                                 Bool_t allowLater = false,
                                 Bool_t allowSooner = false,
                                 Bool_t allowJustAfter = false ) const;
    Bool_t CheckG4ApplicationState(const TString& methodName,
                                 G4ApplicationState requiredState,
                                 Bool_t allowLater = false) const;

    // data members
    TG4StateManager*     fStateManager;    ///< application state manager
    TG4GeometryManager*  fGeometryManager; ///< geometry manager
    TG4SDManager*        fSDManager;       ///< sensitive detectors manager
    TG4PhysicsManager*   fPhysicsManager;  ///< physics manager
    TG4StepManager*      fStepManager;     ///< step manager
    TG4VisManager*       fVisManager;      ///< visualization manager
    G4VisExecutive*      fVisExecutive;    ///< Geant4 visualization manager
    TG4RunManager*       fRunManager;      ///< run manager
    Int_t                fMediumCounter;   ///< global medium counter
    Int_t                fMaterialCounter; ///< global material counter
    Int_t                fMatrixCounter;   ///< global matrix counter
    TString              fUserGeometry;    ///< user geometry

  ClassDef(TGeant4,0) // Geant4 implementation of the TVirtualMC interface 
};

#ifndef __CINT__

// inline methods
#include "TGeant4.icc"

#endif
#endif // TGEANT4_H

