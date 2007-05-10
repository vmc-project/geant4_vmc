// $Id: TG4TrackingAction.cxx,v 1.10 2006/12/15 09:30:29 brun Exp $
// Category: event
//
// Class TG4TrackingAction
// -----------------------
// See the class description in the header file.
//
// Author: I.Hrivnacova

#include "TG4TrackingAction.h"
#include "TG4TrackInformation.h"
#include "TG4StepManager.h"
#include "TG4PhysicsManager.h"
#include "TG4ParticlesManager.h"
#include "TG4StackPopper.h"
#include "TG4SensitiveDetector.h"
#include "TG4SDServices.h"
#include "TG4Globals.h"

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>
#include <TMCProcess.h>

#include <G4TrackVector.hh>
#include <G4TrackingManager.hh>
#include <G4UImanager.hh>

// static data members
TG4TrackingAction* TG4TrackingAction::fgInstance = 0;

//_____________________________________________________________________________
TG4TrackingAction::TG4TrackingAction()     
  : G4UserTrackingAction(),
    TG4Verbose("trackingAction",2),    
    fMessenger(this),
    fPrimaryTrackID(0),
    fSavePrimaries(false),
    fSaveSecondaries(true),
    fNewVerboseLevel(0),
    fNewVerboseTrackID(-1),
    fTrackCounter(0)
{
//
  if (fgInstance) { 
    TG4Globals::Exception(
      "TG4TrackingAction", "TG4TrackingAction", 
      "Cannot create two instances of singleton.");
  }

  fgInstance = this;
}

//_____________________________________________________________________________
TG4TrackingAction::~TG4TrackingAction() {
//
  fgInstance = 0;
}

//
// private methods
//

//_____________________________________________________________________________
void TG4TrackingAction::SetTrackInformation(const G4Track* track)
{
/// Set track index to track information
/// and set the current track in stack.
// --- 

  TG4TrackInformation* trackInfo
    = GetTrackInformation(track, "SetTrackInformation");
  if ( ! trackInfo ) {
    // create track information and set it to G4Track
    // if it does not yet exist
    trackInfo = new TG4TrackInformation();
    fpTrackingManager->SetUserTrackInformation(trackInfo);
        // the track information is deleted together with its
        // G4Track object  
  }

  // track index in the particles array
  Int_t trackIndex = trackInfo->GetTrackParticleID();
  if ( trackIndex < 0 ) {
     // Do not reset particle ID if it is already set
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();
    if ( parentID == 0 ) { 
      // in VMC track numbering starts from 0
      trackIndex = trackID-1; 
    } 
    else { 
      if ( fSaveSecondaries )
        trackIndex = gMC->GetStack()->GetNtrack();
      else   
        trackIndex = fTrackCounter;
            // if secondaries are not stacked in VMC stack
            // use own counter for setting track index
    }
    trackInfo->SetTrackParticleID(trackIndex);
  }  

  // set current track number
  gMC->GetStack()->SetCurrentTrack(trackIndex);
  ++fTrackCounter;
}

//_____________________________________________________________________________
void TG4TrackingAction::SetParentToTrackInformation(const G4Track* track)
{
/// Set parent track particle index to the secondary tracks.

  G4TrackVector* secondaryTracks 
    = fpTrackingManager->GetSteppingManager()->GetSecondary();

  if (secondaryTracks){
    G4int i;
    for (i=0; i<G4int(secondaryTracks->size()); i++) {
      G4Track* secondary = (*secondaryTracks)[i]; 

      // get parent track index
      TG4TrackInformation* parentInfo
        = GetTrackInformation(track, "PostTrackingAction");
        
      G4int parentParticleID 
        = parentInfo->GetTrackParticleID();

      // get or create track information and set it to the G4Track
      TG4TrackInformation* trackInfo
        = GetTrackInformation(secondary, "PostTrackingAction");
      if ( ! trackInfo ) {
        trackInfo = new TG4TrackInformation(-1);
        // the track information is deleted together with its
        // G4Track object  
      }  
      trackInfo->SetParentParticleID(parentParticleID);
      secondary->SetUserInformation(trackInfo);
    }         
  }
      
}

//_____________________________________________________________________________
TG4TrackInformation* TG4TrackingAction::GetTrackInformation(
                                           const G4Track* track,
                                           const G4String& method) const
{
/// Return user track information.
 
#ifdef MCDEBUG
  G4VUserTrackInformation* trackInfo = track->GetUserInformation();
  if (!trackInfo) return 0;  

  TG4TrackInformation* tg4TrackInfo
    = dynamic_cast<TG4TrackInformation*>(trackInfo);
  if (!tg4TrackInfo) { 
     TG4Globals::Exception(
       "TG4TrackingAction", method, "Unknown track information type");
  }
  
  return tg4TrackInfo;
#else  
  return (TG4TrackInformation*)track->GetUserInformation();
#endif  
}    
  
//_____________________________________________________________________________
void TG4TrackingAction::UserProcessHits(const G4Track* track)
{
/// Let sensitive detector process the vertex step
/// (this ensures compatibility with G3 that
/// makes first step of zero length)

  TG4StepManager* stepManager = TG4StepManager::Instance();
  G4VPhysicalVolume* pv = stepManager->GetCurrentPhysicalVolume();
  
  if (!pv) {
    // was exception
    TG4Globals::Warning(
      "TG4TrackingAction", "UserProcessHits", "Cannot locate track vertex."); 
    return;
  }  
  
#ifdef MCDEBUG
  TG4SensitiveDetector* tsd
    = TG4SDServices::Instance()
         ->GetSensitiveDetector(
              pv->GetLogicalVolume()->GetSensitiveDetector());

  if (tsd) tsd->UserProcessHits((G4Track*)track, 0);
#else
  TG4SensitiveDetector* tsd
    = (TG4SensitiveDetector*) pv->GetLogicalVolume()->GetSensitiveDetector();

  if (tsd) tsd->UserProcessHits((G4Track*)track, 0);
#endif  
}

//_____________________________________________________________________________
void TG4TrackingAction::Verbose() const
{
/// Print primary track ID with a frequence defined
/// by verbose level.

  if (VerboseLevel() == 3) { 
    G4cout << "$$$ Primary track " << fPrimaryTrackID << G4endl;
  } 
  else if (VerboseLevel() == 2 &&  fPrimaryTrackID % 10 == 0 ) {
    G4cout << "$$$ Primary track " << fPrimaryTrackID  << G4endl;
  } 
  else if (VerboseLevel() == 1 &&  fPrimaryTrackID % 100 == 0 ) {
    G4cout << "$$$ Primary track " << fPrimaryTrackID  << G4endl;
  } 
}

//
// public methods
//

//_____________________________________________________________________________
void TG4TrackingAction::PrepareNewEvent()
{
/// Called by G4 kernel at the beginning of event.

  if (fSaveSecondaries)
    fTrackCounter = 0;
  else  
    fTrackCounter = gMC->GetStack()->GetNtrack();

  // set g4 stepping manager pointer
  TG4StepManager* stepManager = TG4StepManager::Instance();
  stepManager->SetSteppingManager(fpTrackingManager->GetSteppingManager());
}

//_____________________________________________________________________________
void TG4TrackingAction::PreUserTrackingAction(const G4Track* track)
{
/// Called by G4 kernel before starting tracking.

  // finish previous primary track first
  if (track->GetParentID() == 0) {  
    FinishPrimaryTrack();
  }  

  // reset stack popper (if activated
  if ( TG4StackPopper::Instance() )
    TG4StackPopper::Instance()->Reset();

  // set step manager status
  TG4StepManager* stepManager = TG4StepManager::Instance();
  stepManager->SetStep((G4Track*)track, kVertex);
  
  // clean fNofPoppedParticles in StackPopper process
  
  // set track information
  SetTrackInformation(track);

  if (track->GetParentID() == 0) {  
    fPrimaryTrackID = track->GetTrackID();
    // begin this primary track
    TVirtualMCApplication::Instance()->BeginPrimary();
  }
  else { 
    // save secondary particles info 
    if (  fSaveSecondaries && 
        ! GetTrackInformation(track, "PreUserTrackingAction")->IsUserTrack() ) {
      TrackToStack(track);

      // Notify a stack popper (if activated) about saving this secondary
      if ( TG4StackPopper::Instance() )
        TG4StackPopper::Instance()->Notify();
    }  
  }
  
  // verbose
  if ( track->GetTrackID() == fNewVerboseTrackID) {
    G4String command = "/tracking/verbose ";
    TG4Globals::AppendNumberToString(command, fNewVerboseLevel);
    G4UImanager::GetUIpointer()->ApplyCommand(command);
  }    

  // VMC application pre track action
  TVirtualMCApplication::Instance()->PreTrack();

  // call pre-tracking action of derived class
  PreTrackingAction(track);

  // Let sensitive detector process vertex step
  UserProcessHits(track);
}

//_____________________________________________________________________________
void TG4TrackingAction::PostUserTrackingAction(const G4Track* track)
{
/// Called by G4 kernel after finishing tracking.

  // counter
  // if (fSaveSecondaries) fTrackCounter++;
  
  // set parent track particle index to the secondary tracks 
  SetParentToTrackInformation(track);
      
  // VMC application post track action
  TVirtualMCApplication::Instance()->PostTrack();

  // call post-tracking action of derived class
  PostTrackingAction(track);
}

//_____________________________________________________________________________
void TG4TrackingAction::FinishPrimaryTrack()
{
/// Call VMC application to finish a primary track.                          \n
/// !! This method has to be also called from the EventAction::EndOfEventAction() 
/// for storing the last primary track of the current event.
// --- 

  if (fPrimaryTrackID>0) {

    // set special step manager status
    // not in both stepping, vertex stage
    TG4StepManager* stepManager = TG4StepManager::Instance();
    G4Track* noTrack = 0;
    stepManager->SetStep(noTrack, kVertex);
   
    // verbose
    Verbose();

    // VMC application finish primary track         
    TVirtualMCApplication::Instance()->FinishPrimary();
  }
  
  fPrimaryTrackID = 0;
}  

//_____________________________________________________________________________
void TG4TrackingAction::TrackToStack(const G4Track* track)
{
/// Get all needed parameters from G4track and pass them
/// to the VMC stack.
// ----

  // parent particle index 
  G4int parentID = track->GetParentID();
  G4int motherIndex;
  if (parentID == 0) { 
    motherIndex = -1; 
  }
  else {
    motherIndex 
      = GetTrackInformation(track,"TrackToStack")->GetParentParticleID();
  }
     
  // PDG code
  G4int pdg 
    = TG4ParticlesManager::Instance()->GetPDGEncoding(track->GetDefinition());

  // track kinematics  
  G4ThreeVector momentum = track->GetMomentum(); 
  
  G4double px = momentum.x()/GeV;
  G4double py = momentum.y()/GeV;
  G4double pz = momentum.z()/GeV;
  G4double e = track->GetTotalEnergy()/GeV;

  G4ThreeVector position = track->GetPosition(); 
  G4double vx = position.x()/cm;
  G4double vy = position.y()/cm;
  G4double vz = position.z()/cm;
  // time of production - check if ekvivalent with G4
  G4double t = track->GetGlobalTime();

  G4ThreeVector polarization = track->GetPolarization(); 
  G4double polX = polarization.x();
  G4double polY = polarization.y();
  G4double polZ = polarization.z();

  // production process
  TMCProcess mcProcess;  
  const G4VProcess* kpProcess = track->GetCreatorProcess();
  if (!kpProcess) {
    mcProcess = kPPrimary;
  }
  else {  
    mcProcess = TG4PhysicsManager::Instance()->GetMCProcess(kpProcess);  
    // distinguish kPDeltaRay from kPEnergyLoss  
    if (mcProcess == kPEnergyLoss) mcProcess = kPDeltaRay;
  }  
  
  G4double weight = 1.;

  // Track charge
  // Store the dynamic particle charge (which in case of ion may
  // be different from PDG charge) as status as there is no other 
  // place where we can do it
  G4int charge = G4int(track->GetDynamicParticle()->GetCharge()/eplus);
  G4int status = charge;   
  
  G4int ntr;
  // create particle 
  gMC->GetStack()->PushTrack(0, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                            polX, polY, polZ, mcProcess, ntr, weight, status);  
                   
}

//_____________________________________________________________________________
void TG4TrackingAction::SetNewVerboseLevel(G4int level)
{ 
/// Set the new verbose level that will be set when the track with 
/// specified track ID (fNewVerboseTrackID) starts tracking.

  fNewVerboseLevel = level;  
}

//_____________________________________________________________________________
void TG4TrackingAction::SetNewVerboseTrackID(G4int trackID)
{ 
/// Set the trackID for which the new verbose level (fNewVerboseLevel)
/// will be applied.

  fNewVerboseTrackID = trackID; 
}

