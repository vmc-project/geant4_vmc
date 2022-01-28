//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: OpNoviceActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file OpNoviceActionInitialization.cc
/// \brief Implementation of the OpNoviceActionInitialization class

#include "OpNoviceActionInitialization.hh"
#include "OpNovicePrimaryGeneratorAction.hh"
#include "OpNoviceRunAction.hh"
#include "OpNoviceSteppingAction.hh"
#include "OpNoviceStackingAction.hh"
#include "OpNoviceSteppingVerbose.hh"
  // Added for G4Root
#include "TG4RootNavMgr.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceActionInitialization::OpNoviceActionInitialization(G4bool useG4Root)
 : G4VUserActionInitialization(),
   fUseG4Root(useG4Root)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceActionInitialization::~OpNoviceActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceActionInitialization::BuildForMaster() const
{
  SetUserAction(new OpNoviceRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceActionInitialization::Build() const
{
  SetUserAction(new OpNovicePrimaryGeneratorAction());
  SetUserAction(new OpNoviceRunAction());
  SetUserAction(new OpNoviceSteppingAction());
  SetUserAction(new OpNoviceStackingAction());

  // Added for G4Root - start
  if ( fUseG4Root ) {
    // Master Root navigator
    TG4RootNavMgr* masterRootNavMgr = TG4RootNavMgr::GetMasterInstance();
    G4cout << "Got masterRootNavMgr: " << masterRootNavMgr << G4endl;
    // Create G4Root navigator on worker
    TG4RootNavMgr* rootNavMgr = TG4RootNavMgr::GetInstance(*masterRootNavMgr);
    G4cout << "TG4RootNavMgr has been created." << rootNavMgr << G4endl;

    //rootNavMgr->Initialize(new TG4PostDetConstruction());
    rootNavMgr->ConnectToG4();
  }
  // Added for G4Root - end
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose*
               OpNoviceActionInitialization::InitializeSteppingVerbose() const
{
  return new OpNoviceSteppingVerbose();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
