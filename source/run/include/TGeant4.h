// $Id: TGeant4.h,v 1.2 2003/01/08 08:27:38 brun Exp $
// Category: run
//
// Author: I. Hrivnacova
//
// Class TGeant4
// -------------
// Geant4 implementation of the MonteCarlo interface.                      

#ifndef TGEANT4_H
#define TGEANT4_H

#include <TVirtualMC.h>
#include <TMCProcess.h>
#include <TArrayI.h>

class TG4VRunConfiguration;
class TG4GeometryManager;
class TG4SDManager;
class TG4PhysicsManager;
class TG4StepManager;
class TG4VisManager;
class TG4RunManager;

class TGeant4: public TVirtualMC
{
  public:
    TGeant4(const char* name, const char* title,
            TG4VRunConfiguration* configuration, int argc, char** argv);
    TGeant4(const char* name, const char* title,
            TG4VRunConfiguration* configuration);
    // --> protected
    // TGeant4();
    // TGeant4(const TGeant4& right);
    virtual ~TGeant4();

    //
    // methods for building/management of geometry
    // ------------------------------------------------
    //

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
    
    // get methods
    virtual Int_t VolId(const Text_t* volName) const;
    virtual const char* VolName(Int_t id) const;
    virtual Int_t NofVolumes() const;
    virtual Int_t VolId2Mate(Int_t id) const;

    //
    // methods for physics management
    // ------------------------------------------------
    //
 
    virtual void BuildPhysics();
    virtual void Gstpar(Int_t itmed, const char *param, Double_t parval); 

    // set methods
    virtual void SetCut(const char* cutName, Double_t cutValue);
    virtual void SetProcess(const char* flagName, Int_t flagValue);
    virtual Double_t Xsec(char* reac, Double_t energy, Int_t part, Int_t mate);

        // particle table usage         
    virtual Int_t IdFromPDG(Int_t pdgID) const;
    virtual Int_t PDGFromId(Int_t mcID) const;
    virtual void  DefineParticles();       

    //
    // methods for step management
    // ------------------------------------------------
    //

    // action methods
    virtual void StopTrack();
    virtual void StopEvent();   

    // set methods
    virtual void SetMaxStep(Double_t);
    virtual void SetMaxNStep(Int_t);
    virtual void SetUserDecay(Int_t);  //NEW

    // get methods
         // tracking volume(s) 
    virtual Int_t CurrentVolID(Int_t& copyNo) const;
    virtual Int_t CurrentVolOffID(Int_t off, Int_t& copyNo) const;
    virtual const char* CurrentVolName() const;
    virtual const char* CurrentVolOffName(Int_t off) const;
    virtual Int_t CurrentMaterial(Float_t &a, Float_t &z, 
                    Float_t &dens, Float_t &radl, Float_t &absl) const;  
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
    virtual void    TrackVertexPosition(TLorentzVector& position) const;
    virtual void    TrackVertexMomentum(TLorentzVector& momentum) const;
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
    virtual Int_t StepProcesses(TArrayI &proc) const;
  
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
    // NEW
    // Geant3 specific methods
    // !!! need to be transformed to common interface
    //
    virtual void Gdopt(const char* name , const char* value);
    virtual void SetClipBox(const char *name, Double_t xmin, Double_t xmax,
		       Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax);
    virtual void DefaultRange();
    virtual void Gdhead(Int_t isel, const char* name, Double_t chrsiz);   
    virtual void Gdman(Double_t u, Double_t v, const char* type);
    virtual void SetColors();
    virtual void Gtreve();
    virtual void GtreveRoot();
    virtual void Gckmat(Int_t itmed, char* natmed);
    virtual void InitLego();
    virtual void Gfpart(Int_t ipart, char *name, Int_t& itrtyp,  
		       Float_t& amass, Float_t& charge, Float_t& tlife);
    virtual void Gspart(Int_t ipart, const char *name, Int_t itrtyp,  
		       Double_t amass, Double_t charge, Double_t tlife); 

    //
    // methods for run control
    // ------------------------------------------------
    //

    virtual void Init();
    virtual void ProcessEvent();
    virtual void ProcessRun(Int_t nofEvents);

        // UI control methods
    void StartGeantUI();	
    void StartRootUI();	
    void ProcessGeantMacro(const char* macroName);
    void ProcessGeantCommand(const char* commandPath);

        // get methods
    virtual Int_t   CurrentEvent() const; 

  protected:
    TGeant4();
    TGeant4(const TGeant4& right);

    // operators
    TGeant4& operator=(const TGeant4& right);

  private:
    // data members
    TG4GeometryManager*  fGeometryManager; //geometry manager
    TG4SDManager*        fSDManager;       //sensitive detectors manager
    TG4PhysicsManager*   fPhysicsManager;  //physics manager
    TG4StepManager*      fStepManager;     //step manager
    TG4VisManager*       fVisManager;      //visualization manager
    TG4RunManager*       fRunManager;      //run manager

  ClassDef(TGeant4,0) // Geant4 implementation of the MonteCarlo interface 
};

#ifndef __CINT__

// inline methods
#include "TGeant4.icc"

#endif
#endif // TGEANT4_H

