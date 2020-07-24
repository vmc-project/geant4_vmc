//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TGeant4.cxx
/// \brief Implementation of the TGeant4 class
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TGeant4.h"
#include "TG4ApplicationState.h"
#include "TG4GeometryManager.h"
#include "TG4Globals.h"
#include "TG4OpGeometryManager.h"
#include "TG4PhysicsManager.h"
#include "TG4RunConfiguration.h"
#include "TG4RunManager.h"
#include "TG4SDManager.h"
#include "TG4StateManager.h"
#include "TG4StepManager.h"
#include "TG4Version.h"
#include "TG4VisManager.h"

#include <G4ApplicationState.hh>
#include <G4StateManager.hh>
#include <G4Threading.hh>
#include <G4VisExecutive.hh>

#include <TVirtualMCGeometry.h>

/// \cond CLASSIMP
ClassImp(TGeant4)
  /// \endcond

  //_____________________________________________________________________________
  TGeant4* TGeant4::fgMasterInstance = 0;
TVirtualMCApplication* TGeant4::fgMasterApplicationInstance = 0;

namespace
{

//_____________________________________________________________________________
Bool_t CheckApplicationState(const TString& methodName,
  TG4ApplicationState requiredState, Bool_t allowLater = false,
  Bool_t allowSooner = false, Bool_t allowJustAfter = false)
{
  /// If current application state does not correspond to the requiredState,
  /// gives a warning and returnse false; returns true otherwise.
  /// (To be changed in exception later.)

  TG4StateManager* stateManager = TG4StateManager::Instance();
  TG4ApplicationState currentState = stateManager->GetCurrentState();
  TG4ApplicationState previousState = stateManager->GetPreviousState();
  if (currentState != requiredState &&
      !((allowLater && currentState > requiredState) ||
        (allowLater && previousState >= requiredState)) &&
      !((allowSooner && currentState < requiredState) ||
        (allowSooner && previousState <= requiredState)) &&
      !(allowJustAfter && previousState == requiredState)) {

    TString message =
      TString("MC::") + methodName +
      +TString(" can be called only from VMCApplication::") +
      TString(stateManager->GetStateName(requiredState).c_str());
    if (allowLater) message += TString(" or after");
    if (allowSooner) message += TString(" or before");
    if (allowJustAfter) message += TString(" or just after");
    message += TG4Globals::Endl() +
               TString("while detected in VMCApplication::") +
               TString(stateManager->GetStateName(currentState).c_str());
    if (currentState == kNotInApplication)
      message += TString(" after VMCApplication::") +
                 TString(stateManager->GetStateName(previousState).c_str());

    TG4Globals::Warning("TGeant4", methodName, message);
    return false;
  }

  return true;
}

//_____________________________________________________________________________
Bool_t CheckG4ApplicationState(const TString& methodName,
  G4ApplicationState requiredState, Bool_t allowLater = false)
{
  /// If current application state does not correcpond to the requiredState,
  /// gives a warning and returnse false; returns true otherwise.
  /// (To be changed in exception later.)

  G4ApplicationState currentState =
    G4StateManager::GetStateManager()->GetCurrentState();

  if (currentState != requiredState &&
      !(allowLater && currentState > requiredState)) {

    // States as strings
    static std::vector<TString> g4StateNames;
    g4StateNames.push_back("G4State_PreInit");
    g4StateNames.push_back("G4State_Init");
    g4StateNames.push_back("G4State_Idle");
    g4StateNames.push_back("G4State_GeomClosed");
    g4StateNames.push_back("G4State_EventProc");
    g4StateNames.push_back("G4State_Quit");
    g4StateNames.push_back("G4State_Abort");

    TString message = TString("MC::") + methodName +
                      +TString(" can be called only when Geant4 is in state ") +
                      g4StateNames[requiredState];
    if (allowLater) message += TString(" or later");
    message += TG4Globals::Endl() + TString("while detected in state ") +
               g4StateNames[currentState];

    TG4Globals::Warning("TGeant4", methodName, message);
    return false;
  }

  return true;
}

//_____________________________________________________________________________
void PrintVersion()
{
  /// Prints the  version banner

  G4cout << G4endl
         << "============================================================="
         << G4endl << " Geant4 Virtual Monte Carlo " << G4endl << " Version "
         << GEANT4_VMC_RELEASE << " ( " << GEANT4_VMC_RELEASE_DATE << " )"
         << G4endl << " WWW : http://root.cern.ch/drupal/content/geant4-vmc"
         << G4endl
         << "============================================================="
         << G4endl << G4endl;
}

} // namespace

//_____________________________________________________________________________
TGeant4::TGeant4(const char* name, const char* title,
  TG4RunConfiguration* configuration, int argc, char** argv)
  : TVirtualMC(name, title),
    fStateManager(0),
    fGeometryManager(0),
    fSDManager(0),
    fPhysicsManager(0),
    fStepManager(0),
    fVisManager(0),
    fVisExecutive(0),
    fRunManager(0),
    fRunConfiguration(configuration),
    fMediumCounter(0),
    fMaterialCounter(0),
    fMatrixCounter(0),
    fUserGeometry(configuration->GetUserGeometry()),
    fIsMT(configuration->IsMTApplication())
{
  /// Standard constructor

  PrintVersion();

  G4bool isMaster = !G4Threading::IsWorkerThread();
  if (isMaster) {
    fgMasterInstance = this;
    fgMasterApplicationInstance = TVirtualMCApplication::Instance();
  }

  // Inactivate MT mode if Geant4 is built in sequential mode
  if (G4Threading::G4GetThreadId() == -2) {
    fIsMT = false;
  }

  // Update title with a physics selection
  TString newTitle = title;
  newTitle.Append(" : ");
  newTitle.Append(configuration->GetPhysicsListSelection());
  SetTitle(newTitle);

  // create state manager - thread local
  fStateManager = new TG4StateManager();
  fStateManager->SetNewState(kPreInit);
  // add verbose level
  // G4cout << "TG4StateManager has been created." << G4endl;

  // create geometry manager - shared
  if (isMaster) {
    fGeometryManager = new TG4GeometryManager(fUserGeometry);
    // add verbose level
    // G4cout << "TG4GeometryManager has been created." << G4endl;

    // create sensitive detectors manager - shared
    fSDManager = new TG4SDManager();
    // add verbose level
    // G4cout << "TG4SDManager has been created." << G4endl;

    // create physics manager
    fPhysicsManager = new TG4PhysicsManager();
    // add verbose level
    // G4cout << "TG4PhysicsManager has been created." << G4endl;
  }
  else {
    fGeometryManager = fgMasterInstance->fGeometryManager;
    fSDManager = fgMasterInstance->fSDManager;
    fPhysicsManager = fgMasterInstance->fPhysicsManager;
  }

  // create step manager
  fStepManager = new TG4StepManager(fUserGeometry);
  // add verbose level
  // G4cout << "TG4StepManager has been created." << G4endl;

  // create run manager
  fRunManager = new TG4RunManager(configuration, argc, argv);
  // add verbose level
  // G4cout << "TG4RunManager has been created." << G4endl;

  if (isMaster) {
    // create visualization managers
    fVisManager = new TG4VisManager();
    fVisExecutive = new G4VisExecutive();
  }
  else {
    fVisManager = fgMasterInstance->fVisManager;
    fVisExecutive = fgMasterInstance->fVisExecutive;
  }

#ifdef MCDEBUG
  G4cout << "Debug mode is switched on." << G4endl;
#endif

  // add verbose level
  // G4cout << "TGeant4::TGeant4 done " << this << G4endl;
}

//_____________________________________________________________________________
TGeant4::~TGeant4()
{
  /// Destructor

  // G4cout << "TGeant4::~TGeant4 " << this << G4endl;

  G4bool isMaster = !G4Threading::IsWorkerThread();
  if (isMaster) {
    fgMasterInstance = 0;
    fgMasterApplicationInstance = 0;
  }

  delete fRunManager;
  delete fStateManager;
  if (isMaster) {
    delete fGeometryManager;
    delete fSDManager;
    delete fPhysicsManager;
  }
  delete fStepManager;
  if (isMaster) {
    delete fVisManager;
    delete fVisExecutive;
  }
  // G4cout << "TGeant4::~TGeant4 done " << this << G4endl;
}

//
// methods for building/management of geometry
//

//_____________________________________________________________________________
Bool_t TGeant4::IsRootGeometrySupported() const
{
  /// Returns info about supporting geometry defined via Root

  return true;
}

//_____________________________________________________________________________
void TGeant4::FinishGeometry()
{
  /// Sets the top VTE in temporary G3 volume table.
  /// Close geometry output file (if fWriteGeometry is set true).

  if (!CheckApplicationState("FinishGeometry", kConstructGeometry)) return;

  // Ggclos();
  fGeometryManager->FinishGeometry();
}

//_____________________________________________________________________________
void TGeant4::Gfmate(Int_t imat, char* name, Float_t& a, Float_t& z,
  Float_t& dens, Float_t& radl, Float_t& absl, Float_t* ubuf, Int_t& nbuf)
{
  /// Return parameters for material specified by material number imat

  fGeometryManager->GetOpManager()->Gfmate(
    imat, name, a, z, dens, radl, absl, ubuf, nbuf);
}

//_____________________________________________________________________________
void TGeant4::Gfmate(Int_t imat, char* name, Double_t& a, Double_t& z,
  Double_t& dens, Double_t& radl, Double_t& absl, Double_t* ubuf, Int_t& nbuf)
{
  /// Return parameters for material specified by material number imat

  fGeometryManager->GetOpManager()->Gfmate(
    imat, name, a, z, dens, radl, absl, ubuf, nbuf);
}

//_____________________________________________________________________________
void TGeant4::Gckmat(Int_t /*itmed*/, char* /*natmed*/)
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "Gckmat", "Not implemented.");
}

//_____________________________________________________________________________
void TGeant4::Material(Int_t& kmat, const char* name, Double_t a, Double_t z,
  Double_t dens, Double_t radl, Double_t absl, Float_t* buf, Int_t nwbuf)
{
  /// Create material.                                                       \n
  /// !! Parameters radl, absl, buf, nwbuf are ignored.

  if (!CheckApplicationState("Material", kConstructGeometry)) return;

  kmat = ++fMaterialCounter;

  fGeometryManager->GetMCGeometry()->Material(
    kmat, name, a, z, dens, radl, absl, buf, nwbuf);
}

//_____________________________________________________________________________
void TGeant4::Material(Int_t& kmat, const char* name, Double_t a, Double_t z,
  Double_t dens, Double_t radl, Double_t absl, Double_t* buf, Int_t nwbuf)
{
  /// Create material.                                                       \n
  /// !! Parameters radl, absl, buf, nwbuf are ignored.

  if (!CheckApplicationState("Material", kConstructGeometry)) return;

  kmat = ++fMaterialCounter;

  fGeometryManager->GetMCGeometry()->Material(
    kmat, name, a, z, dens, radl, absl, buf, nwbuf);
}

//_____________________________________________________________________________
void TGeant4::Mixture(Int_t& kmat, const char* name, Float_t* a, Float_t* z,
  Double_t dens, Int_t nlmat, Float_t* wmat)
{
  /// Create material composed of more elements. \n
  /// !! Parameters radl, absl, buf, nwbuf are ignored.

  if (!CheckApplicationState("Mixture", kConstructGeometry)) return;

  kmat = ++fMaterialCounter;

  fGeometryManager->GetMCGeometry()->Mixture(
    kmat, name, a, z, dens, nlmat, wmat);

#if (defined(USE_ROOT_VMC) && (ROOT_VERSION_CODE < ROOT_VERSION(6, 18, 5)))
  // Work around for a problem in TGeoMCGeometry where the wmat values
  // were not properly updated when  nlmat < 0 (fixed in ROOt v6.18/06)
  if (nlmat > 0) return;
  if (fUserGeometry == "VMCtoRoot" || fUserGeometry == "Root") {
    G4cout << "TGeant4::Mixture: work-around for updating wmat values"
           << G4endl;
    G4int nmat = -nlmat;
    G4double amol = 0;
    for (G4int i = 0; i < nmat; ++i) {
      amol += a[i] * wmat[i];
    }
    for (G4int i = 0; i < nmat; i++) {
      wmat[i] *= a[i] / amol;
    }
  }
#endif
}

//_____________________________________________________________________________
void TGeant4::Mixture(Int_t& kmat, const char* name, Double_t* a, Double_t* z,
  Double_t dens, Int_t nlmat, Double_t* wmat)
{
  /// Create material composed of more elements. \n
  /// !! Parameters radl, absl, buf, nwbuf are ignored.

  if (!CheckApplicationState("Mixture", kConstructGeometry)) return;

  kmat = ++fMaterialCounter;

  fGeometryManager->GetMCGeometry()->Mixture(
    kmat, name, a, z, dens, nlmat, wmat);
}

//_____________________________________________________________________________
void TGeant4::Medium(Int_t& kmed, const char* name, Int_t nmat, Int_t isvol,
  Int_t ifield, Double_t fieldm, Double_t tmaxfd, Double_t stemax,
  Double_t deemax, Double_t epsil, Double_t stmin, Float_t* ubuf, Int_t nbuf)
{
  /// Create a temporary "medium" that is used for
  /// assigning corresponding parameters to G4 objects:

  if (!CheckApplicationState("Medium", kConstructGeometry)) return;

  kmed = ++fMediumCounter;

  fGeometryManager->GetMCGeometry()->Medium(kmed, name, nmat, isvol, ifield,
    fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, nbuf);
}

//_____________________________________________________________________________
void TGeant4::Medium(Int_t& kmed, const char* name, Int_t nmat, Int_t isvol,
  Int_t ifield, Double_t fieldm, Double_t tmaxfd, Double_t stemax,
  Double_t deemax, Double_t epsil, Double_t stmin, Double_t* ubuf, Int_t nbuf)
{
  /// Create a temporary "medium" that is used for
  /// assigning corresponding parameters to G4 objects:

  if (!CheckApplicationState("Medium", kConstructGeometry)) return;

  kmed = ++fMediumCounter;

  fGeometryManager->GetMCGeometry()->Medium(kmed, name, nmat, isvol, ifield,
    fieldm, tmaxfd, stemax, deemax, epsil, stmin, ubuf, nbuf);
}

//_____________________________________________________________________________
void TGeant4::Matrix(Int_t& krot, Double_t thetaX, Double_t phiX,
  Double_t thetaY, Double_t phiY, Double_t thetaZ, Double_t phiZ)
{
  /// Create rotation matrix.

  if (!CheckApplicationState("Matrix", kConstructGeometry)) return;

  krot = ++fMatrixCounter;

  fGeometryManager->GetMCGeometry()->Matrix(
    krot, thetaX, phiX, thetaY, phiY, thetaZ, phiZ);
}

//_____________________________________________________________________________
Int_t TGeant4::Gsvolu(
  const char* name, const char* shape, Int_t nmed, Double_t* upar, Int_t np)
{
  /// Create volume

  if (!CheckApplicationState("Gsvolu", kConstructGeometry)) return 0;

  return fGeometryManager->GetMCGeometry()->Gsvolu(name, shape, nmed, upar, np);
}

//_____________________________________________________________________________
Int_t TGeant4::Gsvolu(
  const char* name, const char* shape, Int_t nmed, Float_t* upar, Int_t np)
{
  /// Create volume

  if (!CheckApplicationState("Gsvolu", kConstructGeometry)) return 0;

  return fGeometryManager->GetMCGeometry()->Gsvolu(name, shape, nmed, upar, np);
}

//_____________________________________________________________________________
void TGeant4::Gsdvn(
  const char* name, const char* mother, Int_t ndiv, Int_t iaxis)
{
  /// Create divided volume

  if (!CheckApplicationState("Gsdvn", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsdvn(name, mother, ndiv, iaxis);
}

//_____________________________________________________________________________
void TGeant4::Gsdvn2(const char* name, const char* mother, Int_t ndiv,
  Int_t iaxis, Double_t c0i, Int_t numed)
{
  /// Create divided volume

  if (!CheckApplicationState("Gsdvn2", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsdvn2(
    name, mother, ndiv, iaxis, c0i, numed);
}

//_____________________________________________________________________________
void TGeant4::Gsdvt(const char* name, const char* mother, Double_t step,
  Int_t iaxis, Int_t numed, Int_t ndvmx)
{
  /// Create divided volume

  if (!CheckApplicationState("Gsdvt", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsdvt(
    name, mother, step, iaxis, numed, ndvmx);
}

//_____________________________________________________________________________
void TGeant4::Gsdvt2(const char* name, const char* mother, Double_t step,
  Int_t iaxis, Double_t c0, Int_t numed, Int_t ndvmx)
{
  /// Create divided volume

  if (!CheckApplicationState("Gsdvt2", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsdvt2(
    name, mother, step, iaxis, c0, numed, ndvmx);
}

//_____________________________________________________________________________
void TGeant4::Gsord(const char* name, Int_t iax)
{
  /// No corresponding action in G4.

  if (!CheckApplicationState("Gsord", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsord(name, iax);
}

//_____________________________________________________________________________
void TGeant4::Gspos(const char* name, Int_t nr, const char* mother, Double_t x,
  Double_t y, Double_t z, Int_t irot, const char* konly)
{
  ///  Place a volume into an existing one

  if (!CheckApplicationState("Gspos", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gspos(
    name, nr, mother, x, y, z, irot, konly);
}

//_____________________________________________________________________________
void TGeant4::Gsposp(const char* name, Int_t nr, const char* mother, Double_t x,
  Double_t y, Double_t z, Int_t irot, const char* konly, Double_t* upar,
  Int_t np)
{
  ///  Place a copy of generic volume name with user number
  ///  nr inside mother, with its parameters upar(1..np)

  if (!CheckApplicationState("Gsposp", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsposp(
    name, nr, mother, x, y, z, irot, konly, upar, np);
}

//_____________________________________________________________________________
void TGeant4::Gsposp(const char* name, Int_t nr, const char* mother, Double_t x,
  Double_t y, Double_t z, Int_t irot, const char* konly, Float_t* upar,
  Int_t np)
{
  ///  Place a copy of generic volume name with user number
  ///  nr inside mother, with its parameters upar(1..np)

  if (!CheckApplicationState("Gsposp", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsposp(
    name, nr, mother, x, y, z, irot, konly, upar, np);
}

//_____________________________________________________________________________
void TGeant4::Gsbool(const char* onlyVolName, const char* manyVolName)
{
  /// Help for resolving MANY.
  /// Specify the ONLY volume that overlaps with the
  /// specified MANY and has to be substracted.

  if (!CheckApplicationState("Gsbool", kConstructGeometry)) return;

  fGeometryManager->GetMCGeometry()->Gsbool(onlyVolName, manyVolName);
}

//_____________________________________________________________________________
void TGeant4::SetCerenkov(Int_t itmed, Int_t npckov, Float_t* ppckov,
  Float_t* absco, Float_t* effic, Float_t* rindex)
{
  /// Set the tables for UV photon tracking in medium itmed

  if (!CheckApplicationState("SetCerenkov", kConstructOpGeometry)) return;

  fGeometryManager->GetOpManager()->SetCerenkov(
    itmed, npckov, ppckov, absco, effic, rindex);
}

//_____________________________________________________________________________
void TGeant4::SetCerenkov(Int_t itmed, Int_t npckov, Double_t* ppckov,
  Double_t* absco, Double_t* effic, Double_t* rindex)
{
  /// Set the tables for UV photon tracking in medium itmed

  if (!CheckApplicationState("SetCerenkov", kConstructOpGeometry)) return;

  fGeometryManager->GetOpManager()->SetCerenkov(
    itmed, npckov, ppckov, absco, effic, rindex);
}

//_____________________________________________________________________________
void TGeant4::DefineOpSurface(const char* name, EMCOpSurfaceModel model,
  EMCOpSurfaceType surfaceType, EMCOpSurfaceFinish surfaceFinish,
  Double_t sigmaAlpha)
{
  /// Define the optical surface

  if (!CheckApplicationState("DefineOpSurface", kConstructOpGeometry)) return;

  fGeometryManager->GetOpManager()->DefineOpSurface(
    name, model, surfaceType, surfaceFinish, sigmaAlpha);
}

//_____________________________________________________________________________
void TGeant4::SetBorderSurface(const char* name, const char* vol1Name,
  int vol1CopyNo, const char* vol2Name, int vol2CopyNo,
  const char* opSurfaceName)
{
  /// Define the optical border surface

  if (!CheckApplicationState("SetBorderSurface", kConstructOpGeometry)) return;

  fGeometryManager->GetOpManager()->SetBorderSurface(
    name, vol1Name, vol1CopyNo, vol2Name, vol2CopyNo, opSurfaceName);
}

//_____________________________________________________________________________
void TGeant4::SetSkinSurface(
  const char* name, const char* volName, const char* opSurfaceName)
{
  /// Define the optical skin surface

  if (!CheckApplicationState("SetSkinSurface", kConstructOpGeometry)) return;

  fGeometryManager->GetOpManager()->SetSkinSurface(
    name, volName, opSurfaceName);
}

//_____________________________________________________________________________
void TGeant4::SetMaterialProperty(Int_t itmed, const char* propertyName,
  Int_t np, Double_t* pp, Double_t* values)
{
  /// Set the material property specified by propertyName to the tracking medium

  if (!CheckApplicationState("SetMaterialProperty", kConstructOpGeometry))
    return;

  fGeometryManager->GetOpManager()->SetMaterialProperty(
    itmed, propertyName, np, pp, values);
}

//_____________________________________________________________________________
void TGeant4::SetMaterialProperty(
  Int_t itmed, const char* propertyName, Double_t value)
{
  /// Set the material property specified by propertyName to the tracking medium

  if (!CheckApplicationState("SetMaterialProperty", kConstructOpGeometry))
    return;

  fGeometryManager->GetOpManager()->SetMaterialProperty(
    itmed, propertyName, value);
}

//_____________________________________________________________________________
void TGeant4::SetMaterialProperty(const char* surfaceName,
  const char* propertyName, Int_t np, Double_t* pp, Double_t* values)
{
  /// Set the material property specified by propertyName to the optical surface

  if (!CheckApplicationState("SetMaterialProperty", kConstructOpGeometry))
    return;

  fGeometryManager->GetOpManager()->SetMaterialProperty(
    surfaceName, propertyName, np, pp, values);
}

//_____________________________________________________________________________
Bool_t TGeant4::GetTransformation(
  const TString& volumePath, TGeoHMatrix& matrix)
{
  /// Fill the transformation matrix between the volume specified by
  /// the path volumePath and the top volume; return false if the path is not
  /// found.

  if (!CheckApplicationState("GetTransformation", kInitGeometry, true))
    return false;

  return fGeometryManager->GetMCGeometry()->GetTransformation(
    volumePath, matrix);
}

//_____________________________________________________________________________
Bool_t TGeant4::GetShape(
  const TString& volumePath, TString& shapeType, TArrayD& par)
{
  /// Fill the name of the shape and its parameters for the volume
  /// specified by the volume name; return false if the path is not found.

  if (!CheckApplicationState("GetShape", kInitGeometry, true)) return false;

  return fGeometryManager->GetMCGeometry()->GetShape(
    volumePath, shapeType, par);
}

//______________________________________________________________________
Bool_t TGeant4::GetMaterial(Int_t imat, TString& name, Double_t& a, Double_t& z,
  Double_t& density, Double_t& radl, Double_t& inter, TArrayD& par)
{
  /// Fill the material parameters for the material specified by
  /// material ID; return false if the material is not found.

  return fGeometryManager->GetOpManager()->GetMaterial(
    imat, name, a, z, density, radl, inter, par);
}

//_____________________________________________________________________________
Bool_t TGeant4::GetMaterial(const TString& volumeName, TString& name,
  Int_t& imat, Double_t& a, Double_t& z, Double_t& density, Double_t& radl,
  Double_t& inter, TArrayD& par)
{
  /// Fill the material parameters for the volume specified by
  /// the volume name; return false if the material is not found.

  if (!CheckApplicationState("GetMaterial", kInitGeometry, true)) return false;

  return fGeometryManager->GetMCGeometry()->GetMaterial(
    volumeName, name, imat, a, z, density, radl, inter, par);
}

//_____________________________________________________________________________
Bool_t TGeant4::GetMedium(const TString& volumeName, TString& name, Int_t& imed,
  Int_t& nmat, Int_t& isvol, Int_t& ifield, Double_t& fieldm, Double_t& tmaxfd,
  Double_t& stemax, Double_t& deemax, Double_t& epsil, Double_t& stmin,
  TArrayD& par)
{
  /// Fill the medium parameters for the volume specified by the
  /// volume name; return false if the material is not found.

  if (!CheckApplicationState("GetMedium", kInitGeometry, true)) return false;

  return fGeometryManager->GetMCGeometry()->GetMedium(volumeName, name, imed,
    nmat, isvol, ifield, fieldm, tmaxfd, stemax, deemax, epsil, stmin, par);
}

//_____________________________________________________________________________
void TGeant4::WriteEuclid(const char* /*fileName*/, const char* /*topVol*/,
  Int_t /*number*/, Int_t /*nlevel*/)
{
  /// Write out the geometry of the detector in EUCLID file format
  /// Not implemented

  TG4Globals::Warning("TGeant4", "WriteEuclid", "Not implemented.");
}

//_____________________________________________________________________________
void TGeant4::SetRootGeometry()
{
  /// Nothing has to be done here
}

//_____________________________________________________________________________
void TGeant4::SetUserParameters(Bool_t isUserParameters)
{
  /// Activate the parameters defined in tracking media
  /// (DEEMAX, STMIN, STEMAX), which are, be default, ignored.
  /// In Geant4 case, only STEMAX is taken into account.

  if (!CheckApplicationState("SetUserParameters", kInitGeometry, false, true))
    return;

  fGeometryManager->SetIsUserMaxStep(isUserParameters);
}

//_____________________________________________________________________________
Int_t TGeant4::VolId(const Text_t* volName) const
{
  /// Return the volume ID = sensitive detector identifier.

  if (!CheckApplicationState("VolId", kInitGeometry, true)) return 0;

  return fSDManager->VolId(volName);
}

//_____________________________________________________________________________
const char* TGeant4::VolName(Int_t id) const
{
  //
  /// Return the name of the volume specified by volume ID
  /// ( = sensitive detector name)

  if (!CheckApplicationState("VolName", kInitGeometry, true)) return "";

  return fSDManager->VolName(id);
}

//_____________________________________________________________________________
Int_t TGeant4::MediumId(const Text_t* medName) const
{
  /// Return the medium ID for medium with given name

  TG4ApplicationState requiredState = kConstructGeometry;
  if (fUserGeometry == "RootToGeant4" || fUserGeometry == "Geant4")
    requiredState = kInitGeometry;

  if (!CheckApplicationState("MediumId", requiredState, true)) return 0;

  return fGeometryManager->GetMCGeometry()->MediumId(medName);
}

//_____________________________________________________________________________
Int_t TGeant4::NofVolumes() const
{
  /// Return the total number of VMC volumes
  /// ( = number of sensitive detectors).

  if (!CheckApplicationState("NofVolumes", kInitGeometry, true)) return 0;

  return fSDManager->NofVolumes();
}

//_____________________________________________________________________________
Int_t TGeant4::NofVolDaughters(const char* volName) const
{
  /// Return the number of daughters of the volume specified by name

  if (!CheckApplicationState("NofVolDaughters", kInitGeometry, true)) return 0;

  return fSDManager->NofVolDaughters(volName);
}

//_____________________________________________________________________________
const char* TGeant4::VolDaughterName(const char* volName, Int_t i) const
{
  /// Return the name of the i-th daughter of the volume specified by name.

  if (!CheckApplicationState("VolDaughterName", kInitGeometry, true)) return "";

  return fSDManager->VolDaughterName(volName, i);
}

//_____________________________________________________________________________
Int_t TGeant4::VolDaughterCopyNo(const char* volName, Int_t i) const
{
  /// Return the copyNo of the i-th daughter of the volume specified by name.

  if (!CheckApplicationState("VolDaughterCopyNo", kInitGeometry, true))
    return 0;

  return fSDManager->VolDaughterCopyNo(volName, i);
}

//_____________________________________________________________________________
Int_t TGeant4::VolId2Mate(Int_t id) const
{
  /// Return the material number for a given volume id

  if (!CheckApplicationState("VolId2Mate", kInitGeometry, true)) return 0;

  return fSDManager->VolId2Mate(id);
}

//
// methods for sensitive detectors
//

//_____________________________________________________________________________
void TGeant4::SetSensitiveDetector(
  const TString& volName, TVirtualMCSensitiveDetector* sd)
{
  if (!CheckApplicationState("SetSensitiveDetector", kConstructSD)) return;

  fSDManager->SetSensitiveDetector(volName, sd);
}

//_____________________________________________________________________________
TVirtualMCSensitiveDetector* TGeant4::GetSensitiveDetector(
  const TString& volName) const
{
  if (!CheckApplicationState("SetSensitiveDetector", kConstructSD, true))
    return 0;

  return fSDManager->GetSensitiveDetector(volName);
}

//_____________________________________________________________________________
void TGeant4::SetExclusiveSDScoring(Bool_t exclusiveSDScoring)
{
  if (!CheckApplicationState("SetSensitiveDetector", kConstructSD, true))
    return;

  fSDManager->SetExclusiveSDScoring(exclusiveSDScoring);
}

//
// methods for physics management
//

//_____________________________________________________________________________
void TGeant4::Gstpar(Int_t itmed, const char* param, Double_t parval)
{
  /// Pass the tracking medium parameter to TG4Limits.

  if (!CheckApplicationState("Gstpar", kInitGeometry, false, false, true))
    return;

  fPhysicsManager->Gstpar(itmed, param, parval);
}

//_____________________________________________________________________________
Bool_t TGeant4::SetCut(const char* cutName, Double_t cutValue)
{
  /// Set cut specified by cutName.

  if (!CheckApplicationState("SetCut", kPreInit)) return false;

  return fPhysicsManager->SetCut(cutName, cutValue);
}

//_____________________________________________________________________________
Bool_t TGeant4::SetProcess(const char* flagName, Int_t flagValue)
{
  /// Set process control specified by flagName.

  if (!CheckApplicationState("SetProcess", kPreInit)) return false;

  return fPhysicsManager->SetProcess(flagName, flagValue);
}

//_____________________________________________________________________________
Bool_t TGeant4::DefineParticle(Int_t /*pdg*/, const char* /*name*/,
  TMCParticleType /*mcType*/, Double_t /*mass*/, Double_t /*charge*/,
  Double_t /*lifetime*/)
{
  /// Old function definition, now replaced with more arguments

  TG4Globals::Warning("TGeant4", "DefineParticle",
    "Deprecated function: the function with long argument list should be used "
    "instead.");

  return false;
}

//_____________________________________________________________________________
Bool_t TGeant4::DefineParticle(Int_t pdg, const char* name,
  TMCParticleType mcType, Double_t mass, Double_t charge, Double_t lifetime,
  const TString& pType, Double_t width, Int_t iSpin, Int_t iParity,
  Int_t iConjugation, Int_t iIsospin, Int_t iIsospinZ, Int_t gParity,
  Int_t lepton, Int_t baryon, Bool_t stable, Bool_t shortlived,
  const TString& subType, Int_t antiEncoding, Double_t magMoment,
  Double_t excitation)
{
  /// Add the user defined particle with specified characteristics.

  if (!CheckApplicationState("DefineParticle", kAddParticles)) return false;

  return fPhysicsManager->DefineParticle(pdg, name, mcType, mass, charge,
    lifetime, pType, width, iSpin, iParity, iConjugation, iIsospin, iIsospinZ,
    gParity, lepton, baryon, stable, shortlived, subType, antiEncoding,
    magMoment, excitation);
}

//_____________________________________________________________________________
Bool_t TGeant4::DefineIon(const char* name, Int_t Z, Int_t A, Int_t Q,
  Double_t excEnergy, Double_t mass)
{
  /// Keep user defined ion properties in order to be able to use
  /// them later as a primary particle.

  if (!CheckApplicationState("DefineIon", kAddIons)) return false;

  return fPhysicsManager->DefineIon(name, Z, A, Q, excEnergy, mass);
}

//_____________________________________________________________________________
inline void TGeant4::SetUserDecay(Int_t pdg)
{
  /// Force the decay of particle with given PDG to be done with user
  /// defined decay or external decayer

  if (!CheckApplicationState("SetUserDecay", kAddParticles) &&
      !CheckApplicationState("SetUserDecay", kAddIons))
    return;

  fPhysicsManager->SetUserDecay(pdg);
}

//_____________________________________________________________________________
Bool_t TGeant4::SetDecayMode(Int_t pdg, Float_t bratio[6], Int_t mode[6][3])
{
  /// Set a user phase space decay for a particle

  if (!CheckApplicationState("SetDecayMode", kAddParticles)) return false;

  return fPhysicsManager->SetDecayMode(pdg, bratio, mode);
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

  if (!CheckG4ApplicationState("IdFromPDG", G4State_Idle, true)) return 0;

  return fPhysicsManager->IdFromPDG(pdgID);
}

//_____________________________________________________________________________
Int_t TGeant4::PDGFromId(Int_t mcID) const
{
  /// G4 does not use the integer particle identifiers
  /// Id <-> PDG is identity.

  if (!CheckG4ApplicationState("PDGFromId", G4State_Idle, true)) return 0;

  return fPhysicsManager->PDGFromId(mcID);
}

//_____________________________________________________________________________
TString TGeant4::ParticleName(Int_t pdg) const
{
  /// Return name of G4 particle specified by pdg.

  if (!CheckG4ApplicationState("ParticleName", G4State_Idle, true)) return "";

  return fPhysicsManager->ParticleName(pdg);
}

//_____________________________________________________________________________
Double_t TGeant4::ParticleMass(Int_t pdg) const
{
  /// Return mass of G4 particle specified by pdg.

  if (!CheckG4ApplicationState("ParticleName", G4State_Idle, true)) return 0;

  return fPhysicsManager->ParticleMass(pdg);
}

//_____________________________________________________________________________
Double_t TGeant4::ParticleCharge(Int_t pdg) const
{
  /// Return charge (in e units) of G4 particle specified by pdg.

  if (!CheckG4ApplicationState("ParticleCharge", G4State_Idle, true)) return 0;

  return fPhysicsManager->ParticleCharge(pdg);
}

//_____________________________________________________________________________
Double_t TGeant4::ParticleLifeTime(Int_t pdg) const
{
  /// Return life time of G4 particle specified by pdg.

  if (!CheckG4ApplicationState("ParticleLifeTime", G4State_Idle, true))
    return 0;

  return fPhysicsManager->ParticleLifeTime(pdg);
}

//_____________________________________________________________________________
TMCParticleType TGeant4::ParticleMCType(Int_t pdg) const
{
  /// Return VMC type of G4 particle specified by pdg.

  if (!CheckG4ApplicationState("ParticleMCType", G4State_Idle, true))
    return kPTUndefined;

  return fPhysicsManager->ParticleMCType(pdg);
}

//
// methods for step management
// inlined (in TGeant4.icc)
//

//
// methods for visualization
//

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
void TGeant4::Gdraw(const char* name, Double_t theta, Double_t phi,
  Double_t psi, Double_t u0, Double_t v0, Double_t ul, Double_t vl)
{
  /// Draw the volume specified by name and all its daughters

  fVisManager->Gdraw(name, theta, phi, psi, u0, v0, ul, vl);
}

//
// methods for run control
//

//_____________________________________________________________________________
void TGeant4::Init()
{
  /// Initialize G4 run manager.

  fRunManager->Initialize();

  fVisExecutive->SetVerboseLevel(0);
  fVisExecutive->Initialize();

  fRunManager->CreateGeantUI();
}

//_____________________________________________________________________________
void TGeant4::InitMT(Int_t /*threadRank*/)
{
  /// Initialize G4 run manager.

  TG4Globals::Warning("TGeant4", "InitMT", "Deprecated function.");

  Init();
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
void TGeant4::ProcessEvent(Int_t eventId)
{
  /// Process one event passing eventId and flag whether that is interruptible
  /// meaning that GEANT4_VMC relies on TVirtualMCApplication::BeginEvent() and
  /// ::FinishEvent() will be called from outside.

  fRunManager->ProcessEvent(eventId, kFALSE);
}

//_____________________________________________________________________________
void TGeant4::ProcessEvent(Int_t eventId, Bool_t isInterruptible)
{
  /// Process one event passing eventId and flag whether that is interruptible
  /// meaning that GEANT4_VMC relies on TVirtualMCApplication::BeginEvent() and
  /// ::FinishEvent() will be called from outside.

  fRunManager->ProcessEvent(eventId, isInterruptible);
}

//_____________________________________________________________________________
Bool_t TGeant4::ProcessRun(Int_t nofEvents)
{
  /// Process Geant4 run.

  return fRunManager->ProcessRun(nofEvents);
}

//_____________________________________________________________________________
Bool_t TGeant4::FinishRun()
{
  /// Process Geant4 run.

  return fRunManager->FinishRun();
}

//_____________________________________________________________________________
void TGeant4::SetCollectTracks(Bool_t collectTracks)
{
  /// (In)Activate collecting TGeo tracks

  fStepManager->SetCollectTracks(collectTracks);
}

//_____________________________________________________________________________
Bool_t TGeant4::IsCollectTracks() const
{
  /// Return the info if collecting tracks is activated

  return fStepManager->IsCollectTracks();
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
Bool_t TGeant4::SecondariesAreOrdered() const
{
  ///  Secondaries are ordered if the special stacking
  /// (defined in TG4SpecialStackingAction) is activated.

  return fRunManager->SecondariesAreOrdered();
}

//_____________________________________________________________________________
TGeant4* TGeant4::CloneForWorker() const
{
  /// Clone this instance

  TGeant4* geant4 = new TGeant4(GetName(), GetTitle(), fRunConfiguration);

  // G4cout << "TGeant4::CloneForWorker: " << geant4 << G4endl;
  // G4cout << "TVirtualMCApplication: " << TVirtualMCApplication::Instance() <<
  // G4endl;

  return geant4;
}

// Geant3 specific methods
// !!! need to be transformed to common interface
// ------------------------------------------------

//_____________________________________________________________________________
void TGeant4::Gdopt(const char* /*name*/, const char* /*value*/)
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "Gdopt", "Not implemented.");
}

//_____________________________________________________________________________
void TGeant4::SetClipBox(const char* /*name*/, Double_t /*xmin*/,
  Double_t /*xmax*/, Double_t /*ymin*/, Double_t /*ymax*/, Double_t /*zmin*/,
  Double_t /*zmax*/)
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "SetClipBox", "Not implemented.");
}

//_____________________________________________________________________________
void TGeant4::DefaultRange()
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "DefaultRange", "Not implemented.");
}

//_____________________________________________________________________________
void TGeant4::Gdhead(Int_t /*isel*/, const char* /*name*/, Double_t /*chrsiz*/)
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "Gdhead", "Not implemented.");
}

//_____________________________________________________________________________
void TGeant4::Gdman(Double_t /*u*/, Double_t /*v*/, const char* /*type*/)
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "Gdman", "Not implemented.");
}
//_____________________________________________________________________________
void TGeant4::InitLego()
{
  /// Function not implemented.
  /// Return warning.

  TG4Globals::Warning("TGeant4", "InitLego", "Not implemented.");
}
