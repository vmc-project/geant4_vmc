// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4StepManager.cxx
/// \brief Implementation of the TG4StepManager class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4StepManager.h"
#include "TG4SteppingAction.h"
#include "TG4GeometryServices.h"
#include "TG4SDServices.h"
#include "TG4ParticlesManager.h"
#include "TG4PhysicsManager.h"
#include "TG4TrackManager.h"
#include "TG4TrackInformation.h"
#include "TG4Globals.h"
#include "TG4G3Units.h"

#include <G4SteppingManager.hh>
#include <G4UserLimits.hh>
#include <G4UImanager.hh>
#include <G4AffineTransform.hh>
#include <G4TransportationManager.hh>
#include <G4Navigator.hh>
#include <G4VProcess.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4VTouchable.hh>

#include <TLorentzVector.h>
#include <TMath.h>

TG4StepManager* TG4StepManager::fgInstance = 0;

//_____________________________________________________________________________
TG4StepManager::TG4StepManager(const TString& userGeometry) 
  : fTrack(0),
    fStep(0),
    fStepStatus(kNormalStep),
    fTouchableHistory(0),
    fSteppingManager(0),
    fVolPathBuffer(),
    fCopyNoOffset(0)
{
/// Standard constructor
/// \param userGeometry  User selection of geometry definition and navigation 

  if (fgInstance) {
    TG4Globals::Exception(
      "TG4StepManager", "TG4StepManager", 
      "Cannot create two instances of singleton.");
  }
      
  fgInstance = this;  
  
  fTouchableHistory = new G4TouchableHistory();
  
  /// Set offset for passing copyNo to 1;
  /// as G3toG4 decrement copyNo passed by user by 1
  if ( userGeometry == "VMCtoGeant4" ) fCopyNoOffset = 1;
}

//_____________________________________________________________________________
TG4StepManager::~TG4StepManager() 
{
/// Destructor

  delete fTouchableHistory;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4StepManager::CheckTrack() const
{
/// Give exception in case the track is not defined.

  if (!fTrack) 
    TG4Globals::Exception(
      "TG4StepManager", "CheckTrack", "Track is not defined.");
}     


//_____________________________________________________________________________
void TG4StepManager::CheckStep(const G4String& method) const
{
/// Give exception in case the step is not defined.

  if (!fStep) {
    TG4Globals::Exception(
      "TG4StepManager", method, "Step is not defined.");
  }
}     


//_____________________________________________________________________________
void TG4StepManager::CheckSteppingManager() const
{
/// Give exception in case the step is not defined.

  if (!fSteppingManager) 
    TG4Globals::Exception(
      "TG4StepManager", "CheckSteppingManager", 
      "Stepping manager is not defined.");
}     


//_____________________________________________________________________________
void TG4StepManager::SetTLorentzVector(G4ThreeVector xyz, G4double t, 
                                       TLorentzVector& lv) const                                       
{
/// Fill TLorentzVector with G4ThreeVector and G4double.

   lv[0] = xyz.x();                                         
   lv[1] = xyz.y();                                         
   lv[2] = xyz.z();                                         
   lv[3] = t;
}                                            

//_____________________________________________________________________________
const G4VTouchable* TG4StepManager::GetCurrentTouchable() const 
{
/// Return the current touchable.
 
  const G4VTouchable* touchable; 
  if (fStepStatus == kNormalStep) {

#ifdef MCDEBUG
    CheckStep("GetCurrentTouchable");
#endif    

    touchable = fStep->GetPreStepPoint()->GetTouchable();
  }  
  else if (fStepStatus == kBoundary) {

#ifdef MCDEBUG
    CheckStep("GetCurrentTouchable");
#endif 

    touchable = fStep->GetPostStepPoint()->GetTouchable();
  }  
  else {

#ifdef MCDEBUG
    CheckTrack();
#endif 
    const G4ThreeVector& position = fTrack->GetPosition();
    //const G4ThreeVector& direction = fTrack->GetMomentumDirection();
    G4Navigator* navigator =
      G4TransportationManager::GetTransportationManager()->
        GetNavigatorForTracking();
    
    navigator->LocateGlobalPointAndUpdateTouchable(
                     position, fTouchableHistory);

    touchable = fTouchableHistory;
  }   
    
  return touchable;
}  

//_____________________________________________________________________________
G4VPhysicalVolume* 
TG4StepManager::GetCurrentOffPhysicalVolume(G4int off, G4bool warn) const 
{
/// Return the physical volume of the off-th mother
/// of the current volume.
 
  // Get current touchable
  //
  const G4VTouchable* touchable = GetCurrentTouchable(); 

  // Check touchable depth
  //
  if (touchable->GetHistoryDepth() < off) {
    if (warn) {
      TString text = "level=";
      text += off;
      TG4Globals::Warning(
        "TG4StepManager", "GetCurrentOffPhysicalVolume", 
        "Volume " + TString(touchable->GetVolume()->GetName()) +
        " has not defined mother in " + text + ".");  
    }    
    return 0;
  }  

  if (off == 0)
    return touchable->GetVolume();
  
  // Get the off-th mother
  // 
  G4int index = touchable->GetHistoryDepth() - off;
        // in the touchable history volumes are ordered
        // from top volume up to mother volume;
        // the touchable volume is not in the history
  
  return touchable->GetHistory()->GetVolume(index); 
}     

//
// public methods
//

//_____________________________________________________________________________
void TG4StepManager::StopTrack()
{
/// Stop the current track and skips to the next.                          \n
/// Possible "stop" track status from G4:
///  - fStopButAlive       // Invoke active rest physics processes and
///                        // and kill the current track afterward
///  - fStopAndKill        // Kill the current track (used)
///  - fKillTrackAndSecondaries  // Kill the current track and also associated
///                       // secondaries.

  if (fTrack) {
    fTrack->SetTrackStatus(fStopAndKill);
    // fTrack->SetTrackStatus(fStopButAlive);
    // fTrack->SetTrackStatus(fKillTrackAndSecondaries);
  }
  else {
    TG4Globals::Warning(
      "TG4StepManager", "StopTrack()",
      "There is no current track to be stopped.");
  }    
}

//_____________________________________________________________________________
void TG4StepManager::StopEvent()
{
/// Abort the current event processing.

  if (fTrack) {
    fTrack->SetTrackStatus(fKillTrackAndSecondaries);
            //StopTrack();   // cannot be used as it keeps secondaries
  }
              
  G4UImanager::GetUIpointer()->ApplyCommand("/event/abort");
}

//_____________________________________________________________________________
void TG4StepManager::StopRun()
{
/// Abort the current run processing.

  TG4SDServices::Instance()->SetIsStopRun(true);

  StopEvent();
  G4UImanager::GetUIpointer()->ApplyCommand("/run/abort");
}

//_____________________________________________________________________________
void TG4StepManager::SetMaxStep(Double_t step)
{
/// Set the maximum step allowed in the current logical volume.

  G4UserLimits* userLimits 
    = GetCurrentPhysicalVolume()->GetLogicalVolume()->GetUserLimits();
  
#ifdef MCDEBUG
  if (!userLimits) {
    TG4Globals::Exception(
      "TG4StepManager", "SetMaxStep", "User limits not defined.");
  }
#endif  

  // set max step
  userLimits->SetMaxAllowedStep(step*TG4G3Units::Length()); 

}

//_____________________________________________________________________________
void TG4StepManager::SetMaxNStep(Int_t maxNofSteps)
{
/// Set the maximum number of steps.

  TG4SteppingAction::Instance()->SetMaxNofSteps(TMath::Abs(maxNofSteps));
}

//_____________________________________________________________________________
void TG4StepManager::SetUserDecay(Int_t /*pdg*/)
{
/// Set user decay.                                                          \n
/// Not yet implemented.

  TG4Globals::Exception(
    "TG4StepManager", "SetUserDecay", "Not yet implemented.");
}

//_____________________________________________________________________________
void TG4StepManager::ForceDecayTime(Float_t time)
{
/// Force decay time.                                                        \n
/// Not yet implemented.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4ParticleDefinition* particle
    = fTrack->GetDynamicParticle()->GetDefinition();
  
  // Store the original particle lifetime in track information
  // (as it has to be set back after track is finished)
  TG4TrackInformation* trackInformation
    =  TG4TrackManager::Instance()->GetTrackInformation(fTrack);
  trackInformation->SetPDGLifetime(particle->GetPDGLifeTime()); 
    
  // Set new lifetime value
  particle->SetPDGLifeTime(time*TG4G3Units::Time());
}

//_____________________________________________________________________________
G4VPhysicalVolume* TG4StepManager::GetCurrentPhysicalVolume() const 
{
/// Return the current physical volume.
/// According to fStepStatus the volume from track vertex,
/// pre step point or post step point is returned.

  G4VPhysicalVolume* physVolume; 
  if (fStepStatus == kNormalStep) {

#ifdef MCDEBUG
    CheckStep("GetCurrentPhysicalVolume");
#endif    

    physVolume = fStep->GetPreStepPoint()->GetPhysicalVolume();
  }  
  else if (fStepStatus == kBoundary) {

#ifdef MCDEBUG
    CheckStep("GetCurrentPhysicalVolume");
#endif 

    physVolume = fStep->GetPostStepPoint()->GetPhysicalVolume();
  }  
  else {

#ifdef MCDEBUG
    CheckTrack();
#endif 

    G4ThreeVector position = fTrack->GetPosition();
    G4Navigator* navigator =
      G4TransportationManager::GetTransportationManager()->
        GetNavigatorForTracking();
    physVolume
     = navigator->LocateGlobalPointAndSetup(position); 
     
    if ( ! physVolume ) {
      G4cerr << "No physical volume found at track vertex: "
             << position << G4endl; 
    }         
  }   
  return physVolume;
}     

//_____________________________________________________________________________
Int_t TG4StepManager::CurrentVolID(Int_t& copyNo) const
{
/// Return the current sensitive detector ID
/// and fill the copy number of the current physical volume 

  G4VPhysicalVolume* physVolume = GetCurrentPhysicalVolume();
  if ( ! physVolume ) {
    TG4Globals::Exception(
      "TG4StepManager", "CurrentVolID", "No current physical volume found");
    return 0;  
  }
  copyNo = physVolume->GetCopyNo() + fCopyNoOffset;

  // sensitive detector ID
  TG4SDServices* sdServices = TG4SDServices::Instance();  
  return sdServices->GetVolumeID(physVolume->GetLogicalVolume());
} 

//_____________________________________________________________________________
Int_t TG4StepManager::CurrentVolOffID(Int_t off, Int_t&  copyNo) const
{ 
/// Return the  the sensitive detector ID of the off-th mother of the current  
/// volume and  fill the copy number of its physical volume 

  if (off == 0) return CurrentVolID(copyNo);
#ifdef MCDEBUG
   G4VPhysicalVolume* mother = GetCurrentOffPhysicalVolume(off, true); 
#else
   G4VPhysicalVolume* mother = GetCurrentOffPhysicalVolume(off); 
#endif   

  if (mother) {
    copyNo = mother->GetCopyNo() + fCopyNoOffset;

    // sensitive detector ID
    TG4SDServices* sdServices = TG4SDServices::Instance();
    return sdServices->GetVolumeID(mother->GetLogicalVolume());
  }
  else {
    copyNo = 0;
    return 0;
  }  
}

//_____________________________________________________________________________
const char* TG4StepManager::CurrentVolName() const
{
/// Return the current physical volume name.

  return  TG4GeometryServices::Instance()
            ->UserVolumeName(GetCurrentPhysicalVolume()->GetName());
}

//_____________________________________________________________________________
const char* TG4StepManager::CurrentVolOffName(Int_t off) const
{ 
/// Return the off-th mother's physical volume name.

  if (off == 0) return CurrentVolName();

  G4VPhysicalVolume* mother = GetCurrentOffPhysicalVolume(off); 

  if (mother) {
    return TG4GeometryServices::Instance()
             ->UserVolumeName(mother->GetName());
  }             
  else 
    return "";
}

//_____________________________________________________________________________
const char* TG4StepManager::CurrentVolPath()
{ 
/// Return the current volume path.

  TG4GeometryServices* geometryServices = TG4GeometryServices::Instance();

  // Get current touchable
  const G4VTouchable* touchable = GetCurrentTouchable();
  
  // Check touchable depth
  //
  G4int depth = touchable->GetHistoryDepth();
  
  // Compose the path
  //
  fVolPathBuffer = "";
  for (G4int i=0; i<depth; i++) {
    G4VPhysicalVolume* physVolume = touchable->GetHistory()->GetVolume(i);
    fVolPathBuffer += "/";
    fVolPathBuffer 
      += geometryServices->UserVolumeName(physVolume->GetName());
    fVolPathBuffer += "_";
    TG4Globals::AppendNumberToString(fVolPathBuffer, physVolume->GetCopyNo());
  }     

  // Add current volume to the path
  G4VPhysicalVolume* curPhysVolume = GetCurrentPhysicalVolume(); 
  fVolPathBuffer += "/";
  fVolPathBuffer += geometryServices->UserVolumeName(curPhysVolume->GetName());
  fVolPathBuffer += "_";
  TG4Globals::AppendNumberToString(fVolPathBuffer, curPhysVolume->GetCopyNo());

  return fVolPathBuffer.data(); 

}

//_____________________________________________________________________________
Int_t TG4StepManager::CurrentMaterial(Float_t &a, Float_t &z, Float_t &dens, 
                          Float_t &radl, Float_t &absl) const
{
/// Get parameters of the current material material during transport.
/// Return the number of elements in the mixture
/// \param a     The atomic mass in au
/// \param z     The atomic number
/// \param dens  The density in g/cm3
/// \param radl  The radiation length in cm
/// \param absl  The absorption length in cm

  G4VPhysicalVolume* physVolume = GetCurrentPhysicalVolume(); 
    
  G4Material* material 
    = physVolume->GetLogicalVolume()->GetMaterial();

  G4int nofElements = material->GetNumberOfElements();
  TG4GeometryServices* geometryServices = TG4GeometryServices::Instance();
  a = geometryServices->GetEffA(material);
  z = geometryServices->GetEffZ(material);
      
  // density 
  dens = material->GetDensity();
  dens /= TG4G3Units::MassDensity();      
      
  // radiation length
  radl = material->GetRadlen();
  radl /= TG4G3Units::Length();
      
  absl = 0.;  // this parameter is not defined in Geant4
  return nofElements;
}

//_____________________________________________________________________________
Int_t TG4StepManager::CurrentMedium() const
{   
/// Return the second index of the current material (corresponding to
/// G3 tracking medium index).

  // current logical volume
  G4LogicalVolume* curLV = GetCurrentPhysicalVolume()->GetLogicalVolume();

  // medium index  
  TG4GeometryServices* geometryServices = TG4GeometryServices::Instance();
  return geometryServices->GetMediumId(curLV);
}

//_____________________________________________________________________________
void TG4StepManager::Gmtod(Float_t* xm, Float_t* xd, Int_t iflag) 
{ 
/// Transform a position from the world reference frame
/// to the current volume reference frame.
/// \param xm    Known coordinates in the world reference system
/// \param xd    Computed coordinates in the daughter reference system
/// \param iflag The option: 
///              - IFLAG=1  convert coordinates,                                 \n
///              - IFLAG=2  convert direction cosinus
///
 

  G4double* dxm = TG4GeometryServices::Instance()->CreateG4doubleArray(xm, 3);
  G4double* dxd = TG4GeometryServices::Instance()->CreateG4doubleArray(xd, 3);

  Gmtod(dxm, dxd, iflag);

  for (G4int i=0; i<3; i++) {
    xm[i] = dxm[i]; 
    xd[i] = dxd[i];
  }   

  delete [] dxm;
  delete [] dxd;
} 
 
//_____________________________________________________________________________
void TG4StepManager::Gmtod(Double_t* xm, Double_t* xd, Int_t iflag) 
{ 
/// Transform a position from the world reference frame
/// to the current volume reference frame.
/// \param xm    Known coordinates in the world reference system
/// \param xd    Computed coordinates in the daughter reference system
/// \param iflag The option: 
///              - IFLAG=1  convert coordinates,                                 \n
///              - IFLAG=2  convert direction cosinus
///

#ifdef MCDEBUG
  if (iflag != 1 && iflag != 2) {
      TString text = "iflag=";
      text += iflag;
      TG4Globals::Warning(
        "TG4StepManager", "Gmtod", text + " is different from 1..2.");
      return;        
  }        
#endif

  G4AffineTransform affineTransform;

  if (fStepStatus == kVertex) {
    G4Navigator* navigator =
      G4TransportationManager::GetTransportationManager()->
        GetNavigatorForTracking();
        
    affineTransform = navigator->GetGlobalToLocalTransform();
  }
  else if (fStepStatus == kBoundary) {

#ifdef MCDEBUG
    CheckStep("Gmtod");
#endif
 
    affineTransform
      = fStep->GetPostStepPoint()->GetTouchable()->GetHistory()
        ->GetTopTransform();
  }        
  else {

#ifdef MCDEBUG
    CheckStep("Gmtod");
#endif
 
    affineTransform
      = fStep->GetPreStepPoint()->GetTouchable()->GetHistory()
        ->GetTopTransform();
  }        

  G4ThreeVector theGlobalPoint(xm[0]* TG4G3Units::Length(),
                               xm[1]* TG4G3Units::Length(),                       
                               xm[2]* TG4G3Units::Length()); 
  G4ThreeVector theLocalPoint;
  if (iflag == 1) 
    theLocalPoint = affineTransform.TransformPoint(theGlobalPoint);
  else
    // if ( iflag == 2)
    theLocalPoint = affineTransform.TransformAxis(theGlobalPoint);

  xd[0] = theLocalPoint.x()/TG4G3Units::Length();
  xd[1] = theLocalPoint.y()/TG4G3Units::Length();
  xd[2] = theLocalPoint.z()/TG4G3Units::Length();
} 
 
//_____________________________________________________________________________
void TG4StepManager::Gdtom(Float_t* xd, Float_t* xm, Int_t iflag) 
{ 
/// Transform a position from the current volume reference frame
/// to the world reference frame.
/// \param xd    Known coordinates in the daughter reference system
/// \param xm    Computed coordinates in the world reference system
/// \param iflag The option: 
///              - IFLAG=1  convert coordinates,                                 \n
///              - IFLAG=2  convert direction cosinus


  G4double* dxd = TG4GeometryServices::Instance()->CreateG4doubleArray(xd, 3);
  G4double* dxm = TG4GeometryServices::Instance()->CreateG4doubleArray(xm, 3);

  Gdtom(dxd, dxm, iflag);

  for (G4int i=0; i<3; i++) {
    xd[i] = dxd[i];
    xm[i] = dxm[i]; 
  }   

  delete [] dxd;
  delete [] dxm;
} 
 
//_____________________________________________________________________________
void TG4StepManager::Gdtom(Double_t* xd, Double_t* xm, Int_t iflag) 
{ 
/// Transform a position from the current volume reference frame
/// to the world reference frame.
/// \param xd    Known coordinates in the daughter reference system
/// \param xm    Computed coordinates in the world reference system
/// \param iflag The option: 
///              - IFLAG=1  convert coordinates,                                 \n
///              - IFLAG=2  convert direction cosinus

  G4AffineTransform affineTransform;

  if (fStepStatus == kVertex) {
    G4Navigator* navigator =
      G4TransportationManager::GetTransportationManager()->
        GetNavigatorForTracking();
        
    affineTransform = navigator->GetLocalToGlobalTransform();
  }
  else if (fStepStatus == kBoundary) {

#ifdef MCDEBUG
    CheckStep("Gdtom");
#endif

    affineTransform
      = fStep->GetPostStepPoint()->GetTouchable()->GetHistory()
        ->GetTopTransform().Inverse();
  }        
  else {

#ifdef MCDEBUG
    CheckStep("Gdtom");
#endif

    affineTransform
      = fStep->GetPreStepPoint()->GetTouchable()->GetHistory()
        ->GetTopTransform().Inverse();
  }        

  G4ThreeVector theLocalPoint(xd[0]*TG4G3Units::Length(),
                              xd[1]*TG4G3Units::Length(),
                              xd[2]*TG4G3Units::Length()); 
  G4ThreeVector theGlobalPoint;
  if(iflag == 1)
       theGlobalPoint = affineTransform.TransformPoint(theLocalPoint);
  else if( iflag == 2)
       theGlobalPoint = affineTransform.TransformAxis(theLocalPoint);
  else {
    TString text = "iflag=";
    text += iflag;
    TG4Globals::Warning(
      "TG4StepManager", "Gdtom", text + " is different from 1..2.");
  }    

  xm[0] = theGlobalPoint.x()/TG4G3Units::Length();
  xm[1] = theGlobalPoint.y()/TG4G3Units::Length();
  xm[2] = theGlobalPoint.z()/TG4G3Units::Length();
} 
 
//_____________________________________________________________________________
Double_t TG4StepManager::MaxStep() const
{   
/// Return the maximum step allowed in the current logical volume
/// by user limits.

  G4LogicalVolume* curLogVolume
    = GetCurrentPhysicalVolume()->GetLogicalVolume();

  // check this
  G4UserLimits* userLimits 
    = curLogVolume->GetUserLimits();

  G4double maxStep;
  if (userLimits == 0) { 
    TG4Globals::Warning(
      "TG4StepManager", "MaxStep",
      "User Limits are not defined for the current logical volume " + 
       TString(curLogVolume->GetName()) + ".");
    return FLT_MAX;
  }
  else { 
    const G4Track& trackRef = *(fTrack);
    maxStep = userLimits->GetMaxAllowedStep(trackRef); 
    maxStep /= TG4G3Units::Length(); 
    return maxStep;
  }  
}

//_____________________________________________________________________________
Int_t TG4StepManager::GetMaxNStep() const
{   
/// Return the maximum number of steps.

  return TG4SteppingAction::Instance()->GetMaxNofSteps(); 
}

//_____________________________________________________________________________
void TG4StepManager::TrackPosition(TLorentzVector& position) const
{ 
/// Fill the current particle position in the world reference frame
/// and the local time since the current track is created
/// (position in the PostStepPoint).

#ifdef MCDEBUG
  CheckTrack();
#endif

  // get position
  // check if this is == to PostStepPoint position !!
  G4ThreeVector positionVector = fTrack->GetPosition();
  positionVector *= 1./(TG4G3Units::Length());   
     
  // local time   
  G4double time = fTrack->GetLocalTime();
  time /= TG4G3Units::Time();
    
  SetTLorentzVector(positionVector, time, position);
}

//_____________________________________________________________________________
void TG4StepManager::TrackPosition(Double_t& x, Double_t& y, Double_t& z) const
{ 
/// Fill the current particle position in the world reference frame
/// (position in the PostStepPoint).


#ifdef MCDEBUG
  CheckTrack();
#endif

  // get position
  // check if this is == to PostStepPoint position !!
  G4ThreeVector positionVector = fTrack->GetPosition();
  positionVector *= 1./(TG4G3Units::Length());   
     
  // local time   
  G4double time = fTrack->GetLocalTime();
  time /= TG4G3Units::Time();
    
  x = positionVector.x();
  y = positionVector.y();
  z = positionVector.z();
}

//_____________________________________________________________________________
void TG4StepManager::TrackMomentum(TLorentzVector& momentum) const
{  
/// Fill the current particle momentum (px, py, pz, Etot) 

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4ThreeVector momentumVector = fTrack->GetMomentum(); 
  momentumVector *= 1./(TG4G3Units::Energy());   

  G4double energy = fTrack->GetDynamicParticle()->GetTotalEnergy();
  energy /= TG4G3Units::Energy();  

  SetTLorentzVector(momentumVector, energy, momentum);
}

//_____________________________________________________________________________
void TG4StepManager::TrackMomentum(Double_t& px, Double_t& py, Double_t&pz,
                                   Double_t& etot) const
{  
/// Fill the current particle momentum

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4ThreeVector momentumVector = fTrack->GetMomentum(); 
  momentumVector *= 1./(TG4G3Units::Energy());   

  px = momentumVector.x();
  py = momentumVector.y();
  pz = momentumVector.z();

  etot = fTrack->GetDynamicParticle()->GetTotalEnergy();
  etot /= TG4G3Units::Energy();  
}

//_____________________________________________________________________________
void TG4StepManager::TrackVertexPosition(TLorentzVector& position) const
{ 
/// Fill the vertex particle position (in the world reference frame)
/// and the local time since the current track is created.

#ifdef MCDEBUG
  CheckTrack();
#endif

  // position
  G4ThreeVector positionVector = fTrack->GetVertexPosition();
  positionVector *= 1./(TG4G3Units::Length());   
     
  // local time 
  // to be checked  
  G4double time = fTrack->GetLocalTime();
  time /= TG4G3Units::Time();
      
  SetTLorentzVector(positionVector, time, position);
}

//_____________________________________________________________________________
void TG4StepManager::TrackVertexMomentum(TLorentzVector& momentum) const
{  
/// Fill the vertex particle momentum (px, py, pz, Ekin)
/// TO DO: change Ekin -> Etot 

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4ThreeVector momentumVector = fTrack->GetVertexMomentumDirection(); 
  momentumVector *= 1./(TG4G3Units::Energy());   

  G4double energy = fTrack->GetVertexKineticEnergy();
  energy /= TG4G3Units::Energy();  

  SetTLorentzVector(momentumVector, energy, momentum);
}

//_____________________________________________________________________________
Double_t TG4StepManager::TrackStep() const
{   
/// Return the current step length.

  G4double length;
  if (fStepStatus == kNormalStep) {

#ifdef MCDEBUG
    CheckStep("TrackStep");    
#endif

    length = fStep->GetStepLength();
    length /= TG4G3Units::Length();
  }  
  else 
    length = 0;

  return length;
}

//_____________________________________________________________________________
Double_t TG4StepManager::TrackLength() const
{
/// Return the length of the current track from its origin.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4double length = fTrack->GetTrackLength();
  length /= TG4G3Units::Length();
  return length;
}

//_____________________________________________________________________________
Double_t TG4StepManager::TrackTime() const
{
/// Return the global track time = time since the event in which 
/// the track belongs is created.                                           \n
/// Note that in Geant4: there is also defined proper time as
/// the proper time of the dynamical particle of the current track.

#ifdef MCDEBUG
  CheckTrack();
#endif
  
  G4double time = fTrack->GetGlobalTime();
  time /= TG4G3Units::Time();
  return time;
}

//_____________________________________________________________________________
Double_t TG4StepManager::Edep() const
{   
/// Return the total energy deposit in this step.

  G4double energyDeposit = 0;
  if (fStepStatus == kNormalStep) {

#ifdef MCDEBUG
    CheckStep("Edep");
#endif

    energyDeposit = fStep->GetTotalEnergyDeposit();
    energyDeposit /= TG4G3Units::Energy();
  }
  else if ( fStepStatus == kBoundary &&
            fTrack->GetTrackStatus() == fStopAndKill ) {
    G4VProcess *proc = fSteppingManager->GetfCurrentProcess();
    TG4PhysicsManager *physicsManager = TG4PhysicsManager::Instance();      
    if (proc && physicsManager->GetMCProcess(proc)==kPLightScattering &&
        physicsManager->GetOpBoundaryStatus(proc)==kPLightDetection) {
      energyDeposit = fTrack->GetTotalEnergy();
      energyDeposit /= TG4G3Units::Energy();
    }
  }    

  return energyDeposit;
}

//_____________________________________________________________________________
Int_t TG4StepManager::TrackPid() const
{   
/// Return the current particle PDG encoding.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4ParticleDefinition* particle
    = fTrack->GetDynamicParticle()->GetDefinition();
    
  // Ask TG4ParticlesManager to get PDG encoding 
  // (in order to get PDG from extended TDatabasePDG
  // in case the standard PDG code is not defined)
  G4int pdgEncoding 
    = TG4ParticlesManager::Instance()->GetPDGEncoding(particle);

  return pdgEncoding;
}

//_____________________________________________________________________________
Double_t TG4StepManager::TrackCharge() const
{   
/// Return the current particle charge.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4double charge
    = fTrack->GetDynamicParticle()->GetDefinition()
      ->GetPDGCharge();
  charge /= TG4G3Units::Charge();        
  return charge;
}

//_____________________________________________________________________________
Double_t TG4StepManager::TrackMass() const
{   
/// Return the current particle mass at rest.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4double mass
    = fTrack->GetDynamicParticle()->GetDefinition()
      ->GetPDGMass();
  mass /= TG4G3Units::Mass();        
  return mass;
}

//_____________________________________________________________________________
Double_t TG4StepManager::Etot() const
{   
/// Return the total energy of the current particle.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4double energy
    = fTrack->GetDynamicParticle()->GetTotalEnergy();
  energy /= TG4G3Units::Energy();  
  return energy;
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackInside() const
{   
/// Return true if the particle does not cross a geometrical boundary
/// and is not in the vertex.

  if (fStepStatus == kNormalStep  && !(IsTrackExiting()) ) {
    // track is always inside during a normal step
    return true; 
  }    

  return false;    
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackEntering() const
{   
/// Return true if the particle crosses a geometrical boundary
/// or is in the vertex.

  if (fStepStatus != kNormalStep) {
    // track is entering during a vertex or boundary step
    return true;  
  }
  
  return false;  
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackExiting() const
{   
/// Return true if the particle crosses a geometrical boundary.

  if (fStepStatus == kNormalStep) {

#ifdef MCDEBUG
    CheckStep("IsTrackExiting");
#endif    

    if (fStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) 
       return true;  
  }
  
  return false;  
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackOut() const
{   
/// Return true if the particle crosses the world boundary
/// at the post-step point.

  if (fStepStatus == kVertex) return false;

#ifdef MCDEBUG
  CheckStep("IsTrackCut");
#endif

  // check
  G4StepStatus status
    = fStep->GetPostStepPoint()->GetStepStatus();
  if (status == fWorldBoundary)
    return true; 
  else
    return false;
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackStop() const
{   
/// Return true if the particle has stopped 
/// or has been killed, suspended or postponed to the next event.
///
/// Possible track status from G4:
///   - fAlive,              // Continue the tracking
///   - fStopButAlive,       // Invoke active rest physics processes and
///                          // and kill the current track afterward
///   - fStopAndKill,        // Kill the current track
///   - fKillTrackAndSecondaries, // Kill the current track and also associated
///                          // secondaries.
///   - fSuspend,            // Suspend the current track
///   - fPostponeToNextEvent // Postpones the tracking of thecurrent track 
///                          // to the next event.

#ifdef MCDEBUG
  CheckTrack();
#endif

  // check
  G4TrackStatus status
     = fTrack->GetTrackStatus();
  if ((status == fStopAndKill) ||  
      (status == fKillTrackAndSecondaries) ||
      (status == fSuspend) ||
      (status == fPostponeToNextEvent)) {
    return true; 
  }
  else
    return false; 
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackDisappeared() const
{ 
/// Return true if particle has disappeared 
/// (due to any physical process)
/// or has been killed, suspended or postponed to next event.

#ifdef MCDEBUG
  CheckTrack();
#endif

  // check
  G4TrackStatus status
     = fTrack->GetTrackStatus();
  if ((status == fKillTrackAndSecondaries) ||
      (status == fSuspend) ||
      (status == fPostponeToNextEvent)) {
    return true; 
  }
  else
    return false;
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsTrackAlive() const
{   
/// Return true if particle continues tracking.

#ifdef MCDEBUG
  CheckTrack();
#endif

  G4TrackStatus status
     = fTrack->GetTrackStatus();
  if ( (status == fAlive) ||
       (status == fStopButAlive) )
    return true; 
  else
    return false; 
}

//_____________________________________________________________________________
Bool_t TG4StepManager::IsNewTrack() const
{
/// Return true when the track performs the first step.

  if (fStepStatus == kVertex)
    return true;
  else  
    return false;
}

//_____________________________________________________________________________
Int_t TG4StepManager::NSecondaries() const
{
/// Return the number of secondary particles generated 
/// in the current step.

  if (fStepStatus == kVertex) return 0;

#ifdef MCDEBUG
  CheckSteppingManager();
#endif

  G4int nofSecondaries = 0;
  nofSecondaries += fSteppingManager->GetfN2ndariesAtRestDoIt();
  nofSecondaries += fSteppingManager->GetfN2ndariesAlongStepDoIt();
  nofSecondaries += fSteppingManager->GetfN2ndariesPostStepDoIt();

  return nofSecondaries;
}

//_____________________________________________________________________________
void TG4StepManager::GetSecondary(Int_t index, Int_t& particleId, 
                          TLorentzVector& position, TLorentzVector& momentum)
{
/// Fill the parameters of the generated secondary particle 
/// !! Check if indexing of secondaries is same !!
/// \param index      The secondary particle index
/// \param particleId The PDG encoding
/// \param position   The position 
/// \param momentum   The momentum 

#ifdef MCDEBUG
  CheckSteppingManager();
#endif

  G4int nofSecondaries = NSecondaries();
  G4TrackVector* secondaryTracks = fSteppingManager->GetSecondary();

#ifdef MCDEBUG
  if (!secondaryTracks) {
    TG4Globals::Exception(
      "TG4StepManager", "GetSecondary", "Secondary tracks vector is empty");
  }
  
  if (index >= nofSecondaries) {
    TG4Globals::Exception(
      "TG4StepManager", "GetSecondary", "Wrong secondary track index.");
  }
#endif
  
  // the index of the first secondary of this step
  G4int startIndex 
    = secondaryTracks->size() - nofSecondaries;
         // (the secondaryTracks vector contains secondaries 
         // produced by the track at previous steps, too)
  G4Track* track 
    = (*secondaryTracks)[startIndex + index]; 
   
  // particle encoding
  particleId 
    = track->GetDynamicParticle()->GetDefinition()->GetPDGEncoding();
 
  // position & time
  G4ThreeVector positionVector = track->GetPosition();
  positionVector *= 1./(TG4G3Units::Length());
  G4double time = track->GetLocalTime();
  time /= TG4G3Units::Time();
  SetTLorentzVector(positionVector, time, position);

  // momentum & energy
  G4ThreeVector momentumVector = track->GetMomentum();        
  G4double energy = track->GetDynamicParticle()->GetTotalEnergy();
  energy /= TG4G3Units::Energy();
  SetTLorentzVector(momentumVector, energy, momentum);
}

//_____________________________________________________________________________
TMCProcess TG4StepManager::ProdProcess(Int_t isec) const
{
/// Return the VMC code of the process that has produced the secondary particle
/// specified by its index

  G4int nofSecondaries = NSecondaries();
  if (fStepStatus == kVertex || !nofSecondaries) return kPNoProcess;

#ifdef MCDEBUG
  CheckStep("ProdProcess");
#endif

  G4TrackVector* secondaryTracks = fSteppingManager->GetSecondary();
 
#ifdef MCDEBUG
  // should never happen
  if (!secondaryTracks) {
    TG4Globals::Exception(
      "TG4StepManager", "ProdProcess", "Secondary tracks vector is empty.");

    return kPNoProcess;  
  }    

  if (isec >= nofSecondaries) {
    TG4Globals::Exception(
      "TG4StepManager", "ProdProcess", "Wrong secondary track index.");

    return kPNoProcess;  
  }
#endif

  // the index of the first secondary of this step
  G4int startIndex 
    = secondaryTracks->size() - nofSecondaries;
         // the secondaryTracks vector contains secondaries 
         // produced by the track at previous steps, too

  // the secondary track with specified isec index
  G4Track* track = (*secondaryTracks)[startIndex + isec]; 
   
  const G4VProcess* kpProcess = track->GetCreatorProcess(); 
  
  TMCProcess mcProcess 
   = TG4PhysicsManager::Instance()->GetMCProcess(kpProcess);
  
  // distinguish kPDeltaRay from kPEnergyLoss  
  if (mcProcess == kPEnergyLoss) mcProcess = kPDeltaRay;
  
  return mcProcess;
}

//_____________________________________________________________________________
Int_t TG4StepManager::StepProcesses(TArrayI& processes) const
{
/// Fill the array of processes that were active in the current step.
/// The array is filled with the process VMC codes (TMCProcess).
/// Return the number of active processes    
/// (TBD: Distinguish between kPDeltaRay and kPEnergyLoss)

 if ( fStepStatus == kVertex || fStepStatus == kBoundary) {
   G4int nofProcesses = 1;
   processes.Set(nofProcesses);
   processes[0] = kPNull;
   return nofProcesses;
 }  
   
#ifdef MCDEBUG
  CheckSteppingManager();
  CheckStep("StepProcesses");
#endif

  // along step processes
  G4ProcessVector* processVector 
    = fStep->GetTrack()->GetDefinition()->GetProcessManager()
        ->GetAlongStepProcessVector();
  G4int nofAlongStep = processVector->entries();
  
  // process defined step
  const G4VProcess* kpLastProcess 
    = fStep->GetPostStepPoint()->GetProcessDefinedStep();

  // set array size
  processes.Set(nofAlongStep+1);
     // maximum number of processes:
     // nofAlongStep (along step) - 1 (transportations) + 1 (post step process)
     // + possibly 1 (additional process if kPLightScattering)
     // => nofAlongStep + 1
 
  // fill array with (nofAlongStep-1) along step processes 
  TG4PhysicsManager* physicsManager = TG4PhysicsManager::Instance();
  G4int counter = 0;  
  for (G4int i=0; i<nofAlongStep; i++) {
    G4VProcess* g4Process = (*processVector)[i];    
    // do not fill transportation along step process
    if (g4Process && g4Process->GetProcessName() != "Transportation")
      processes[counter++] = physicsManager->GetMCProcess(g4Process);
  }
    
  // fill array with 1 or 2 (if kPLightScattering) last process
  processes[counter++] = physicsManager->GetMCProcess(kpLastProcess);
  if (processes[counter-1] == kPLightScattering) {
     // add reflection/absorption as additional process
     processes[counter++] = physicsManager->GetOpBoundaryStatus(kpLastProcess);
  }        
    
  return counter;  
}
