// $Id: TG4EventAction.cxx,v 1.4 2006/04/12 10:38:21 brun Exp $
// Category: event
//
// Class TG4EventAction
// ---------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4EventAction.h"
#include "TG4TrackingAction.h"
#include "TG4ParticlesManager.h"
#include "TG4Globals.h"

#include <G4Event.hh>
#include <G4TrajectoryContainer.hh>
#include <G4Trajectory.hh>
#include <G4VVisManager.hh>
#include <G4UImanager.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>

#include <TVirtualMC.h>
#include <TVirtualMCStack.h>
#include <TVirtualMCApplication.h>

#include <math.h>

//_____________________________________________________________________________
TG4EventAction::TG4EventAction()
  : TG4Verbose("eventAction"),
    fMessenger(this),
    fTimer(),
    fDrawFlag("CHARGED")
{
//
}

//_____________________________________________________________________________
TG4EventAction::~TG4EventAction() {
//
}

//
// private methods
//

//_____________________________________________________________________________
void TG4EventAction::DisplayEvent(const G4Event* event) const
{
/// Draw trajectories.

  if (G4VVisManager::GetConcreteInstance()) {

    // trajectories processing
    G4TrajectoryContainer* trajectoryContainer 
      = event->GetTrajectoryContainer();

    G4int nofTrajectories = 0;
    if (trajectoryContainer)
      nofTrajectories = trajectoryContainer->entries(); 
  
    if (VerboseLevel() > 0 && nofTrajectories > 0) {
      G4cout << "    " << nofTrajectories; 
      G4cout << " trajectories stored." << G4endl;
    }  

    for (G4int i=0; i<nofTrajectories; i++) { 
      G4VTrajectory* vtrajectory = (*(event->GetTrajectoryContainer()))[i];
      G4Trajectory* trajectory = dynamic_cast<G4Trajectory*>(vtrajectory);
      if (!trajectory) {
        TG4Globals::Exception(
          "TG4EventAction", "DisplayEvent", "Unknown trajectory type.");
      }
      if ( (fDrawFlag == "ALL") ||
          ((fDrawFlag == "CHARGED") && (trajectory->GetCharge() != 0.))){
         trajectory->DrawTrajectory(50); 
            // the argument number defines the size of the step points
            // use 2000 to make step points well visible
      }        
    }      
  }
}

//_____________________________________________________________________________
void TG4EventAction::PrimaryToStack(const G4PrimaryVertex* vertex,
                                    const G4PrimaryParticle* particle) const
{
/// Add primary particle to VMC stack

  // Mother particle index 
  G4int motherIndex = -1;
     
  // Track charge
  G4int charge = G4int(particle->GetCharge()/eplus);
 
  // PDG code
  G4int pdg 
    = TG4ParticlesManager::Instance()
      ->GetPDGEncodingFast(particle->GetG4code(), charge);

  // track kinematics  
  G4ThreeVector momentum = particle->GetMomentum(); 
  G4double px = momentum.x()/GeV;
  G4double py = momentum.y()/GeV;
  G4double pz = momentum.z()/GeV;
  G4double mass = particle->GetMass();
  G4double e = sqrt(momentum.mag()*momentum.mag() + mass*mass);

  G4ThreeVector position = vertex->GetPosition(); 
  G4double vx = position.x()/cm;
  G4double vy = position.y()/cm;
  G4double vz = position.z()/cm;
  // time of production - check if ekvivalent with G4
  G4double t = particle->GetProperTime();

  G4ThreeVector polarization = particle->GetPolarization(); 
  G4double polX = polarization.x();
  G4double polY = polarization.y();
  G4double polZ = polarization.z();

  // production process
  TMCProcess mcProcess = kPPrimary; 
  
  G4double weight = particle->GetWeight();
  G4int status = 0;   
  
  G4int ntr;
  // create particle 
  gMC->GetStack()->PushTrack(1, motherIndex, pdg, px, py, pz, e, vx, vy, vz, t,
                            polX, polY, polZ, mcProcess, ntr, weight, status);  
}                   

//
// public methods
//

//_____________________________________________________________________________
void TG4EventAction::BeginOfEventAction(const G4Event* event)
{
/// Called by G4 kernel at the beginning of event.

  // reset the tracks counters
  if (TG4TrackingAction::Instance()) 
    TG4TrackingAction::Instance()->PrepareNewEvent();   
    
  // fill primary particles in VMC stack if stack is empty
  if ( gMC->GetStack()->GetNtrack() == 0 ) {
    if (VerboseLevel() > 0)
      G4cout << "Filling VMC stack with primaries" << G4endl;
    
    for (G4int iv=0; iv<event->GetNumberOfPrimaryVertex(); iv++) {
      G4PrimaryVertex* vertex = event->GetPrimaryVertex(iv);
      
      for (G4int ip=0; ip<vertex->GetNumberOfParticle(); ip++) {
        G4PrimaryParticle* particle = vertex->GetPrimary(ip);
        PrimaryToStack(vertex, particle);
      }        
    }
  }  

  if (VerboseLevel() > 0) {
    G4cout << ">>> Event " << event->GetEventID() << G4endl;
    fTimer.Start();
  }  
}

//_____________________________________________________________________________
void TG4EventAction::EndOfEventAction(const G4Event* event)
{
/// Called by G4 kernel at the end of event.

  // finish the last primary track of the current event
  TG4TrackingAction* trackingAction = TG4TrackingAction::Instance();
  if (trackingAction) trackingAction->FinishPrimaryTrack();   

  if (VerboseLevel() > 0) {
    G4cout << G4endl;
    G4cout << ">>> End of Event " << event->GetEventID() << G4endl;
  }

  if (VerboseLevel() > 1) {
    G4int nofPrimaryTracks = gMC->GetStack()->GetNprimary();
    G4int nofSavedTracks = gMC->GetStack()->GetNtrack();
   
    G4cout  << "    " << nofPrimaryTracks << 
               " primary tracks processed." << G4endl;
    G4cout  << "    " << nofSavedTracks << 
               " tracks saved." << G4endl;
    if (trackingAction) {
       G4int nofAllTracks = trackingAction->GetNofTracks();
       G4cout  << "    " << nofAllTracks << 
                  " all tracks processed." << G4endl;
    }          
  }               

  // display event
  DisplayEvent(event);

  // aliroot finish event
  TVirtualMCApplication::Instance()->FinishEvent();    

  if (VerboseLevel() > 1) {
    // print time
    fTimer.Stop();
    G4cout << "Time of this event: ";
    fTimer.Print();
  }  
}
