// The Geant4 VMC application executable
// with explicitely instantiated TGeant4 and linked with Geant4 VMC

#include "Ex02MCApplication.h"
#include "TMCRootManagerMT.h"

#include "G4ParTopC.icc"
#include "G4ThreadManager.hh"

#include "TG4RunConfiguration.h"
#include "TGeant4.h"
//#include "TMCRootMutex.h"

#include "G4ios.hh"

int main(int /*argc*/,char** /*argv*/)
{
  // Get current thread rank
  G4int threadRank = G4ThreadManager::GetThreadRank();

  // Initialize Root threading 
  if ( threadRank == 0 ) {
    // Activate debug printing
    //TVirtualMCRootManager::SetDebug(true);
    TMCRootManagerMT::Initialize(G4ThreadManager::GetNofThreads());
  }  
  
  // Create MC application (thread local)
  Ex02MCApplication* appl 
    =  new Ex02MCApplication("Example02", 
                             "The example02 MC application");

  // Create MC (thread local)
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4");
  // Lock Root when creating data - seems not to be needed ?
  //TMCRootMutex::Lock();
  new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
  //TMCRootMutex::UnLock();
  
  // Initialize MC application for this thread rank  
  appl->InitMC(threadRank);  
  appl->RunMC(1000);

  delete appl;

  return 0;
}  
