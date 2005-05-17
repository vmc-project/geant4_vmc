// $Id: TGeant4.cxx,v 1.12 2005/04/01 20:59:27 brun Exp $
// Category: run
//
// Class TGeant4
// -------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TGeant4.h"
#include "TG4VRunConfiguration.h"
#include "TG4GeometryManager.h" 
#include "TG4SDManager.h" 
#include "TG4PhysicsManager.h" 
#include "TG4StepManager.h" 
#include "TG4VisManager.h"
#include "TG4RunManager.h"
#include "TG4Globals.h"

ClassImp(TGeant4)

//_____________________________________________________________________________
TGeant4::TGeant4(const char* name, const char* title,
                 TG4VRunConfiguration* configuration, int argc, char** argv)
  : TVirtualMC(name, title),
    fVisManager(0)
{
  // create run manager
  fRunManager = new TG4RunManager(configuration, argc, argv);
  // add verbose level
  //G4cout << "TG4RunManager has been created." << G4endl;

  // create geometry manager
  fGeometryManager = new TG4GeometryManager();
  // add verbose level
  //G4cout << "TG4GeometryManager has been created." << G4endl;
  
  // create sensitive detectors manager
  fSDManager = new TG4SDManager(configuration->GetSDConstruction());
  // add verbose level
  //G4cout << "TG4SDManager has been created." << G4endl;
  
  // create physics manager  
  fPhysicsManager = new TG4PhysicsManager(configuration->GetPhysicsList());
  // add verbose level
  //G4cout << "TG4GeometryManager has been created." << G4endl;
  
  // create step manager 
  fStepManager = new TG4StepManager();
  // add verbose level
  //G4cout << "TG4StepManager has been created." << G4endl;

#ifdef MCDEBUG
  G4cout << "Debug mode is switched on." << G4endl;
#endif    
  
#ifdef G4VIS_USE
  // create visualization manager
  fVisManager = new TG4VisManager();
  fVisManager->Initialize();
#endif
}
   
//_____________________________________________________________________________
TGeant4::TGeant4(const char* name, const char* title,
                 TG4VRunConfiguration* configuration)
  : TVirtualMC(name, title),
    fVisManager(0)
{
  // create run manager
  fRunManager = new TG4RunManager(configuration);
  // add verbose level
  //G4cout << "TG4RunManager has been created." << G4endl;

  // create geometry manager
  fGeometryManager = new TG4GeometryManager();
  // add verbose level
  //G4cout << "TG4GeometryManager has been created." << G4endl;
  
  // create sensitive detectors manager
  fSDManager = new TG4SDManager(configuration->GetSDConstruction());
  // add verbose level
  //G4cout << "TG4SDManager has been created." << G4endl;
  
  // create physics manager  
  fPhysicsManager = new TG4PhysicsManager(configuration->GetPhysicsList());
  // add verbose level
  //G4cout << "TG4GeometryManager has been created." << G4endl;
  
  // create step manager 
  fStepManager = new TG4StepManager();
  // add verbose level
  //G4cout << "TG4StepManager has been created." << G4endl;
  
#ifdef MCDEBUG
  G4cout << "Debug mode is switched on." << G4endl;
#endif    
  
#ifdef G4VIS_USE
  // create visualization manager
  fVisManager = new TG4VisManager();
  fVisManager->Initialize();
#endif
}

    
//_____________________________________________________________________________
TGeant4::TGeant4() {
//
}

//_____________________________________________________________________________
TGeant4::TGeant4(const TGeant4& right) {
// 
  TG4Globals::Exception("TGeant4 is protected from copying.");
}

//_____________________________________________________________________________
TGeant4::~TGeant4() {
//
  delete fRunManager;
  delete fGeometryManager;
  delete fSDManager;
  delete fPhysicsManager;
  delete fStepManager;
  // fVisManager is deleted with G4RunManager destructor
}

//
// operators
//

//_____________________________________________________________________________
TGeant4& TGeant4::operator=(const TGeant4& right)
{
  // check assignement to self
  if (this == &right) return *this;

  TG4Globals::Exception("TGeant4 is protected from assigning.");
    
  return *this;  
}    
          

//
// methods for building/management of geometry
// 

//_____________________________________________________________________________
Bool_t TGeant4::IsRootGeometrySupported() const
{
/// Returns info about supporting geometry defined via Root

  return fGeometryManager->IsRootGeometrySupported();
}  

//_____________________________________________________________________________
void TGeant4::FinishGeometry() 
{
/// Sets the top VTE in temporary G3 volume table.
/// Close geometry output file (if fWriteGeometry is set true).

  fGeometryManager->Ggclos();
} 

//_____________________________________________________________________________
void TGeant4::Gfmate(Int_t imat, char *name, Float_t &a, Float_t &z,  
  		         Float_t &dens, Float_t &radl, Float_t &absl,
		         Float_t* ubuf, Int_t& nbuf) 
{
/// Return parameters for material specified by material number imat 

  fGeometryManager
    ->Gfmate(imat, name, a, z, dens, radl, absl, ubuf, nbuf);
} 

//_____________________________________________________________________________
void TGeant4::Gfmate(Int_t imat, char *name, Double_t &a, Double_t &z,  
  		         Double_t &dens, Double_t &radl, Double_t &absl,
		         Double_t* ubuf, Int_t& nbuf) 
{
/// Return parameters for material specified by material number imat 

  fGeometryManager
    ->Gfmate(imat, name, a, z, dens, radl, absl, ubuf, nbuf);
}
  			 
//_____________________________________________________________________________
void TGeant4::Gckmat(Int_t itmed, char* natmed) 
{ 
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::Gckmat(..) is not implemented."); 
}

//_____________________________________________________________________________
void TGeant4::Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Float_t* buf, Int_t nwbuf) 
{
/// Create material.                                                       \n
/// !! Parameters radl, absl, buf, nwbuf are ignored.

  fGeometryManager
    ->Material(kmat, name, a, z, dens, radl, absl, buf, nwbuf); 
} 

//_____________________________________________________________________________
void TGeant4::Material(Int_t& kmat, const char* name, Double_t a, 
                     Double_t z, Double_t dens, Double_t radl, Double_t absl,
                     Double_t* buf, Int_t nwbuf)
{
/// Create material.                                                       \n
/// !! Parameters radl, absl, buf, nwbuf are ignored.

  fGeometryManager
    ->Material(kmat, name, a, z, dens, radl, absl, buf, nwbuf); 
}  
		     
//_____________________________________________________________________________
void TGeant4::Mixture(Int_t& kmat, const char *name, Float_t *a, 
                     Float_t *z, Double_t dens, Int_t nlmat, Float_t *wmat) 
{
/// Create material composed of more elements.                                                       \n
/// !! Parameters radl, absl, buf, nwbuf are ignored.

   fGeometryManager
     ->Mixture(kmat, name, a, z, dens, nlmat, wmat); 
} 

//_____________________________________________________________________________
void TGeant4::Mixture(Int_t& kmat, const char *name, Double_t *a, 
                     Double_t *z, Double_t dens, Int_t nlmat, Double_t *wmat) 
{
/// Create material composed of more elements.                                                       \n
/// !! Parameters radl, absl, buf, nwbuf are ignored.

   fGeometryManager
     ->Mixture(kmat, name, a, z, dens, nlmat, wmat); 
}  
		     
//_____________________________________________________________________________
void TGeant4::Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
		     Double_t stmin, Float_t* ubuf, Int_t nbuf) 
{ 
/// Create a temporary "medium" that is used for 
/// assigning corresponding parameters to G4 objects:

  fGeometryManager
    ->Medium(kmed, name, nmat, isvol, ifield, fieldm, tmaxfd, stemax, deemax, 
        epsil, stmin, ubuf, nbuf);
} 

//_____________________________________________________________________________
void TGeant4::Medium(Int_t& kmed, const char *name, Int_t nmat, 
                     Int_t isvol, Int_t ifield, Double_t fieldm, Double_t tmaxfd, 
                     Double_t stemax, Double_t deemax, Double_t epsil, 
		     Double_t stmin, Double_t* ubuf, Int_t nbuf) 
{
/// Create a temporary "medium" that is used for 
/// assigning corresponding parameters to G4 objects:

  fGeometryManager
    ->Medium(kmed, name, nmat, isvol, ifield, fieldm, tmaxfd, stemax, deemax, 
        epsil, stmin, ubuf, nbuf);
}  
		     

//_____________________________________________________________________________
void TGeant4::Matrix(Int_t& krot, Double_t thetaX, Double_t phiX, 
                     Double_t thetaY, Double_t phiY, Double_t thetaZ, 
		     Double_t phiZ) 
{
/// Create rotation matrix.

  fGeometryManager
    ->Matrix(krot, thetaX, phiX, thetaY, phiY, thetaZ, phiZ); 
} 

//_____________________________________________________________________________
Int_t TGeant4::Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Double_t *upar, Int_t np)  
{
/// Create volume

  return fGeometryManager->Gsvolu(name, shape, nmed, upar, np); 
}
 
//_____________________________________________________________________________
Int_t TGeant4::Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Float_t *upar, Int_t np)  
{
/// Create volume

  return fGeometryManager->Gsvolu(name, shape, nmed, upar, np); 
}
 
//_____________________________________________________________________________
void TGeant4::Gsdvn(const char *name, const char *mother, Int_t ndiv, 
                        Int_t iaxis) 
{
/// Create divided volume

  fGeometryManager->Gsdvn(name, mother, ndiv, iaxis); 
} 

//_____________________________________________________________________________
void TGeant4::Gsdvn2(const char *name, const char *mother, Int_t ndiv, 
                         Int_t iaxis, Double_t c0i, Int_t numed)
{
/// Create divided volume

  fGeometryManager->Gsdvn2(name, mother, ndiv, iaxis, c0i, numed); 
} 

//_____________________________________________________________________________
void TGeant4::Gsdvt(const char *name, const char *mother, Double_t step, 
                        Int_t iaxis, Int_t numed, Int_t ndvmx) 
{
/// Create divided volume

  fGeometryManager->Gsdvt(name, mother, step, iaxis, numed, ndvmx); 
} 

//_____________________________________________________________________________
void TGeant4::Gsdvt2(const char *name, const char *mother, Double_t step, 
                         Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx) 
{ 
/// Create divided volume

  fGeometryManager->Gsdvt2(name, mother, step, iaxis, c0, numed, ndvmx); 
} 

//_____________________________________________________________________________
void TGeant4::Gsord(const char *name, Int_t iax) 
{
/// No corresponding action in G4.

  fGeometryManager->Gsord(name, iax); 
} 

//_____________________________________________________________________________
void TGeant4::Gspos(const char *name, Int_t nr, const char *mother,  
                        Double_t x, Double_t y, Double_t z, Int_t irot, 
                        const char *konly) 
{
///  Place a volume into an existing one

  fGeometryManager->Gspos(name, nr, mother, x, y, z, irot, konly); 
} 

//_____________________________________________________________________________
void TGeant4::Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Double_t *upar, Int_t np)  
{
///  Place a copy of generic volume name with user number
///  nr inside mother, with its parameters upar(1..np)

  fGeometryManager->Gsposp(name, nr, mother, x, y, z, irot, konly, upar, np); 
} 

//_____________________________________________________________________________
void TGeant4::Gsposp(const char *name, Int_t nr, const char *mother,  
                         Double_t x, Double_t y, Double_t z, Int_t irot,
                         const char *konly, Float_t *upar, Int_t np)  
{
///  Place a copy of generic volume name with user number
///  nr inside mother, with its parameters upar(1..np)

  fGeometryManager->Gsposp(name, nr, mother, x, y, z, irot, konly, upar, np); 
} 

//_____________________________________________________________________________
void TGeant4::Gsbool(const char* onlyVolName, const char* manyVolName) 
{
/// Help for resolving MANY.
/// Specify the ONLY volume that overlaps with the 
/// specified MANY and has to be substracted.

  fGeometryManager->Gsbool(onlyVolName, manyVolName); 
} 

//_____________________________________________________________________________
void TGeant4::SetCerenkov(Int_t itmed, Int_t npckov, Float_t *ppckov,
                  Float_t *absco, Float_t *effic, Float_t *rindex) 
{
/// Set the tables for UV photon tracking in medium itmed 

  fGeometryManager->SetCerenkov(itmed, npckov, ppckov, absco, effic, rindex);
}  
    
//_____________________________________________________________________________
void TGeant4::SetCerenkov(Int_t itmed, Int_t npckov, Double_t *ppckov,
                  Double_t *absco, Double_t *effic, Double_t *rindex) 
{
/// Set the tables for UV photon tracking in medium itmed 

  fGeometryManager->SetCerenkov(itmed, npckov, ppckov, absco, effic, rindex);
}  
		     		  
//_____________________________________________________________________________
void  TGeant4::DefineOpSurface(const char *name,
                         TMCOpSurfaceModel model,
			 TMCOpSurfaceType surfaceType,
			 TMCOpSurfaceFinish surfaceFinish,
			 Double_t sigmaAlpha)
{			 
/// Define the optical surface

  fGeometryManager->DefineOpSurface(
                         name, model, surfaceType, surfaceFinish, sigmaAlpha);
}			 
			 
//_____________________________________________________________________________
void  TGeant4::SetBorderSurface(const char *name,
                         const char* vol1Name, int vol1CopyNo,
                         const char* vol2Name, int vol2CopyNo,
			 const char* opSurfaceName)
{			 
/// Define the optical border surface

  fGeometryManager->SetBorderSurface(name, 
			 vol1Name, vol1CopyNo, vol2Name, vol2CopyNo, opSurfaceName);
}			 
			 
//_____________________________________________________________________________
void  TGeant4::SetSkinSurface(const char *name,
                         const char* volName,
			 const char* opSurfaceName)
{			 
/// Define the optical skin surface

  fGeometryManager->SetSkinSurface(name, volName, opSurfaceName);
}			 
			 
//_____________________________________________________________________________
void  TGeant4::SetMaterialProperty(
                         Int_t itmed, const char* propertyName, 
			 Int_t np, Double_t* pp, Double_t* values)
{			 
/// Set the material property specified by propertyName to the tracking medium

  fGeometryManager->SetMaterialProperty(itmed, propertyName, np, pp, values); 
}			 
			 
//_____________________________________________________________________________
void  TGeant4::SetMaterialProperty(
                         Int_t itmed, const char* propertyName, 
			 Double_t value)
{			 
/// Set the material property specified by propertyName to the tracking medium

  fGeometryManager->SetMaterialProperty(itmed, propertyName, value); 
}			 
			 
//_____________________________________________________________________________
void  TGeant4::SetMaterialProperty(
                         const char* surfaceName, const char* propertyName, 
			 Int_t np, Double_t* pp, Double_t* values)
{			 
/// Set the material property specified by propertyName to the optical surface

  fGeometryManager->SetMaterialProperty(surfaceName, propertyName, np, pp, values); 
}			 
    
//_____________________________________________________________________________
void TGeant4::WriteEuclid(const char* fileName, const char* topVol, 
                          Int_t number, Int_t nlevel) 
{
/// Write out the geometry of the detector in EUCLID file format
/// Not implemented

   fGeometryManager->WriteEuclid(fileName, topVol, number, nlevel); 
} 
		               
//_____________________________________________________________________________
void TGeant4::SetRootGeometry() 
{                   
/// Convert Root geometry to G4 geometry objects.

  fGeometryManager->SetRootGeometry();
}

//_____________________________________________________________________________
Int_t TGeant4::VolId(const Text_t* volName) const 
{
/// Return the volume ID = sensitive detector identifier.

  return fSDManager->VolId(volName); 
} 

//_____________________________________________________________________________
const char* TGeant4::VolName(Int_t id) const {
//
/// Return the name of the volume specified by volume ID
/// ( = sensitive detector name) 

  return fSDManager->VolName(id); 
}
 
//_____________________________________________________________________________
Int_t TGeant4::NofVolumes() const 
{
/// Return the total number of VMC volumes 
/// ( = number of sensitive detectors).

  return fSDManager->NofVolumes(); 
} 

//_____________________________________________________________________________
Int_t TGeant4::NofVolDaughters(const char* volName) const 
{
/// Return the number of daughters of the volume specified by name

  return fSDManager->NofVolDaughters(volName); 
} 

//_____________________________________________________________________________
const char*  TGeant4::VolDaughterName(const char* volName, Int_t i) const 
{
/// Return the name of the i-th daughter of the volume specified by name.

  return fSDManager->VolDaughterName(volName, i); 
} 

//_____________________________________________________________________________
Int_t  TGeant4::VolDaughterCopyNo(const char* volName, Int_t i) const 
{
/// Return the copyNo of the i-th daughter of the volume specified by name.

  return fSDManager->VolDaughterCopyNo(volName, i); 
} 

//_____________________________________________________________________________
Int_t TGeant4::VolId2Mate(Int_t id) const 
{
/// Return the material number for a given volume id

  return fSDManager->VolId2Mate(id); 
} 

//
// methods for physics management
//
 
//_____________________________________________________________________________
void TGeant4::Gstpar(Int_t itmed, const char *param, Double_t parval) 
{
/// Pass the tracking medium parameter to TG4Limits.

  fGeometryManager->Gstpar(itmed, param, parval); 
  fPhysicsManager->Gstpar(itmed, param, parval); 
}    

//_____________________________________________________________________________
Bool_t TGeant4::SetCut(const char* cutName, Double_t cutValue) 
{ 
/// Set cut specified by cutName.

  return fPhysicsManager->SetCut(cutName, cutValue);
}  

//_____________________________________________________________________________
Bool_t TGeant4::SetProcess(const char* flagName, Int_t flagValue) 
{
/// Set process control specified by flagName.

  return fPhysicsManager->SetProcess(flagName, flagValue);
}  
 
//_____________________________________________________________________________
Bool_t TGeant4::DefineParticle(Int_t pdg, const char* name, TMCParticleType type, 
                          Double_t mass, Double_t charge, Double_t lifetime) 
{
/// Only check if particle with specified pdg is available in Geant4;
/// if not gives exception.

  return fPhysicsManager->DefineParticle(pdg, name, type, mass, charge, lifetime);
}  			
			
//_____________________________________________________________________________
Bool_t TGeant4::DefineIon(const char* name, Int_t Z, Int_t A, 
                        Int_t Q, Double_t excEnergy, Double_t mass) 
{
/// Keep user defined ion properties in order to be able to use
/// them later as a primary particle.
 
  return fPhysicsManager->DefineIon(name, Z, A, Q, excEnergy, mass);
}  

//_____________________________________________________________________________
Double_t TGeant4::Xsec(char* reac, Double_t energy, Int_t part, Int_t mate) 
{
/// Not yet implemented -> give exception.

  return fPhysicsManager->Xsec(reac, energy, part, mate);
}  

//_____________________________________________________________________________
Int_t TGeant4::IdFromPDG(Int_t pdgID) const 
{ 
/// G4 does not use the integer particle identifiers
/// Id <-> PDG is identity.

  return fPhysicsManager->IdFromPDG(pdgID);
}  

//_____________________________________________________________________________
Int_t TGeant4::PDGFromId(Int_t mcID) const 
{
/// G4 does not use the integer particle identifiers
/// Id <-> PDG is identity.

  return fPhysicsManager->PDGFromId(mcID);
}  

//_____________________________________________________________________________
TString   TGeant4::ParticleName(Int_t pdg) const 
{
/// Return name of G4 particle specified by pdg.

  return fPhysicsManager->ParticleName(pdg);
}  
	  
//_____________________________________________________________________________
Double_t  TGeant4::ParticleMass(Int_t pdg) const 
{	  
/// Return mass of G4 particle specified by pdg.

  return fPhysicsManager->ParticleMass(pdg);
}  
	  
//_____________________________________________________________________________
Double_t  TGeant4::ParticleCharge(Int_t pdg) const 
{	  
/// Return charge (in e units) of G4 particle specified by pdg.

  return fPhysicsManager->ParticleCharge(pdg);
}  
	  
//_____________________________________________________________________________
Double_t  TGeant4::ParticleLifeTime(Int_t pdg) const 
{	  
/// Return life time of G4 particle specified by pdg.

  return fPhysicsManager->ParticleLifeTime(pdg);
}  
	  
//_____________________________________________________________________________
TMCParticleType TGeant4::ParticleMCType(Int_t pdg) const 
{
/// Return VMC type of G4 particle specified by pdg.

  return fPhysicsManager->ParticleMCType(pdg);
}  
	  

//
// methods for step management
// inlined (in TGeant4.icc)
//

//
// methods for visualization
//

#ifdef G4VIS_USE
//_____________________________________________________________________________
void TGeant4::DrawOneSpec(const char* name) 
{
/// Function not implemented 

  fVisManager->DrawOneSpec(name); 
} 

//_____________________________________________________________________________
void TGeant4::Gsatt(const char* name, const char* att, Int_t val) 
{
/// Set visualization attributes to the volume specified by name

  fVisManager->Gsatt(name, att, val); 
} 

//_____________________________________________________________________________
void  TGeant4::Gdraw(const char* name, Double_t theta, Double_t phi, 
                        Double_t psi, Double_t u0, Double_t v0, 
			Double_t ul, Double_t vl) 
{
/// Draw the volume specified by name and all its daughters

  fVisManager->Gdraw(name, theta, phi, psi, u0, v0, ul, vl); 
} 

#else
//_____________________________________________________________________________
void TGeant4::DrawOneSpec(const char* name) 
{
/// Function not enabled in no visualization mode

  TG4Globals:: Warning("TGeant4::DrawOneSpec(): no visualization available."); 
} 

//_____________________________________________________________________________
void TGeant4::Gsatt(const char* name, const char* att, Int_t val) 
{
/// Function not enabled in no visualization mode

  TG4Globals:: Warning("TGeant4::Gsatt(): no visualization available."); 
} 

//_____________________________________________________________________________
void TGeant4::Gdraw(const char* p1, Double_t theta, Double_t phi,
		        Double_t psi, Double_t u0, Double_t v0,
		        Double_t ul, Double_t vl) 
{
/// Function not enabled in no visualization mode

  TG4Globals:: Warning("TGeant4::Gdraw(): no visualization available."); 
} 

#endif //G4VIS_USE

//
// methods for run control
//

//_____________________________________________________________________________
void TGeant4::Init() 
{ 
/// Initialize G4 run manager.

  fRunManager->Initialize();
}  
  
//_____________________________________________________________________________
void TGeant4::BuildPhysics() 
{
/// Finish initialization of Geant4 after the G4 run manager initialization
/// is finished. 

  fRunManager->LateInitialize();
}  

//_____________________________________________________________________________
void TGeant4::ProcessEvent() 
{
/// Processing event by user is not availablein Geant4 VMC.
/// Return exception

  fRunManager->ProcessEvent();
}  

//_____________________________________________________________________________
Bool_t TGeant4::ProcessRun(Int_t nofEvents) 
{
/// Process Geant4 run.

  return fRunManager->ProcessRun(nofEvents);
}  

//_____________________________________________________________________________
void TGeant4::StartGeantUI() 
{
/// Start interactive/batch Geant4.

  fRunManager->StartGeantUI();
}  

//_____________________________________________________________________________
void TGeant4::StartRootUI() 
{
/// Starts interactive Root.

  fRunManager->StartRootUI();
}  

//_____________________________________________________________________________
void TGeant4::ProcessGeantMacro(const char* macroName) 
{
/// Process Geant4 macro.

  fRunManager->ProcessGeantMacro(macroName);
}  
  
//_____________________________________________________________________________
void TGeant4::ProcessGeantCommand(const char* command)
{
/// Process Geant4 command.

  fRunManager->ProcessGeantCommand(command);
}  
  
//_____________________________________________________________________________
Int_t TGeant4::CurrentEvent() const 
{
/// Return the number of the current event.

  return fRunManager->CurrentEvent(); 
} 

//_____________________________________________________________________________
Bool_t  TGeant4::SecondariesAreOrdered() const 
{
///  Secondaries are ordered if the special stacking
/// (defined in TG4SpecialStackingAction) is activated.

  return fRunManager->SecondariesAreOrdered();
}  

// Geant3 specific methods
// !!! need to be transformed to common interface
// ------------------------------------------------
    
//_____________________________________________________________________________
void TGeant4::Gdopt(const char* name, const char* value) 
{
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::Gdopt(..) is not implemented."); 
}

//_____________________________________________________________________________
void TGeant4::SetClipBox(const char *name, Double_t xmin, Double_t xmax,
		     Double_t ymin, Double_t ymax, Double_t zmin, Double_t zmax) 
{ 
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::SetClipBox(..) is not implemented."); 
}

//_____________________________________________________________________________
void TGeant4::DefaultRange() 
{ 
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::DefaultRange() is not implemented."); 
}

//_____________________________________________________________________________
void TGeant4::Gdhead(Int_t isel, const char* name, Double_t chrsiz) 
{ 
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::Gdhead(..) is not implemented."); 
}

//_____________________________________________________________________________
void TGeant4::Gdman(Double_t u, Double_t v, const char* type) 
{ 
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::Gdman(..) is not implemented."); 
}
//_____________________________________________________________________________
void TGeant4::InitLego() 
{ 
/// Function not implemented.
/// Return warning.

  TG4Globals:: Warning("TGeant4::InitLego() is not implemented."); 
}
