// $Id: TG4HadronPhysicsList.cxx,v 1.13 2007/08/09 13:43:34 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: physics_lists
// Class TG4HadronPhysicsList
// ---------------------------
// See the class description in the header file.

#include "TG4HadronPhysicsList.h"

#include <FTFC.hh>
#include <FTFP.hh>
#include <FTFP_EMV.hh>
#include <LBE.hh>           // needs additional data
#include <LHEP.hh>
#include <LHEP_BERT.hh>
#include <LHEP_EMV.hh>
#include <LHEP_PRECO_HP.hh> // needs additional data
#include <QBBC.hh>          // needs additional data
#include <QGSC.hh>
#include <QGSC_EFLOW.hh>
#include <QGSC_EMV.hh>
#include <QGSP.hh>
#include <QGSP_BERT_EMV.hh>
#include <QGSP_BERT_HP.hh>  // needs additional data
#include <QGSP_BERT_NQE.hh> 
#include <QGSP_BERT_TRV.hh> 
#include <QGSP_BERT.hh>
#include <QGSP_BIC.hh>
#include <QGSP_BIC_HP.hh>   // needs additional data
#include <QGSP_EMV.hh>
#include <QGSP_EMV_NQE.hh>
#include <QGSP_EMX.hh>
#include <QGSP_NQE.hh>
#include <QGSP_QEL.hh>

const G4double TG4HadronPhysicsList::fgkDefaultCutValue = 1.0 * mm;

//_____________________________________________________________________________
G4String TG4HadronPhysicsList::AvailableSelections()
{
/// Return list of all available selections

  G4String selections;
  selections += "FTFC ";
  selections += "FTFP FTFP_EMV ";
  selections += "LBE ";
  selections += "LHEP LHEP_BERT LHEP_EMV LHEP_PRECO_HP ";
  selections += "QBBC ";
  selections += "QGSC QGSC_EFLOW QGSC_EMV "; 
  selections += "QGSP ";
  selections += "QGSP_BERT QGSP_BERT_EMV QGSP_BERT_HP QGSP_BERT_NQE QGSP_BERT_TRV ";
  selections += "QGSP_BIC QGSP_BIC_HP ";
  selections += "QGSP_EMV QGSP_EMV_NQE QGSP_EMX QGSP_NQE QGSP_QEL ";
  
  return selections;
}  

//_____________________________________________________________________________
G4bool TG4HadronPhysicsList::IsAvailableSelection(const G4String& selection)
{
/// Return list of all available selections

  G4String available = AvailableSelections();
  G4String checkSelection = selection;
  checkSelection += " ";
  
  return available.contains(checkSelection);
}  

//_____________________________________________________________________________
TG4HadronPhysicsList::TG4HadronPhysicsList(const G4String& selection)
  : G4VUserPhysicsList(),
    TG4Verbose("hadronPhysicsList")
 {
//
  Configure(selection);

  defaultCutValue = fgkDefaultCutValue;

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4HadronPhysicsList::~TG4HadronPhysicsList() {
//
  //delete fExtDecayer;
       // fExtDecayer is deleted in G4Decay destructor
}

//
// private methods
//

//_____________________________________________________________________________
void TG4HadronPhysicsList::Configure(const G4String& sel)
{
/// Create the selected physics constructors
/// and registeres them in the modular physics list.

  if      ( sel == "FTFC" )          fPhysicsList = new FTFC();  
  else if ( sel == "FTFP" )          fPhysicsList = new FTFP();
  else if ( sel == "FTFP_EMV" )      fPhysicsList = new FTFP_EMV();
  else if ( sel == "LBE" )           fPhysicsList = new LBE();
  else if ( sel == "LHEP" )          fPhysicsList = new LHEP();
  else if ( sel == "LHEP_BERT" )     fPhysicsList = new LHEP_BERT();
  else if ( sel == "LHEP_EMV" )      fPhysicsList = new LHEP_EMV();
  else if ( sel == "LHEP_PRECO_HP" ) fPhysicsList = new LHEP_PRECO_HP();
  else if ( sel == "QBBC" )          fPhysicsList = new QBBC();
  else if ( sel == "QGSC" )          fPhysicsList = new QGSC();
  else if ( sel == "QGSC_EFLOW" )    fPhysicsList = new QGSC_EFLOW();
  else if ( sel == "QGSC_EMV" )      fPhysicsList = new QGSC_EMV();
  else if ( sel == "QGSP" )          fPhysicsList = new QGSP();
  else if ( sel == "QGSP_BERT" )     fPhysicsList = new QGSP_BERT();
  else if ( sel == "QGSP_BERT_EMV" ) fPhysicsList = new QGSP_BERT_EMV();
  else if ( sel == "QGSP_BERT_HP" )  fPhysicsList = new QGSP_BERT_HP();
  else if ( sel == "QGSP_BERT_NQE" ) fPhysicsList = new QGSP_BERT_NQE();
  else if ( sel == "QGSP_BERT_TRV" ) fPhysicsList = new QGSP_BERT_TRV();
  else if ( sel == "QGSP_BIC" )      fPhysicsList = new QGSP_BIC();
  else if ( sel == "QGSP_BIC_HP" )   fPhysicsList = new QGSP_BIC_HP();
  else if ( sel == "QGSP_EMV" )      fPhysicsList = new QGSP_EMV();
  else if ( sel == "QGSP_EMV_NQE" )  fPhysicsList = new QGSP_EMV_NQE();
  else if ( sel == "QGSP_EMX" )      fPhysicsList = new QGSP_EMX();
  else if ( sel == "QGSP_NQE" )      fPhysicsList = new QGSP_NQE();
  else if ( sel == "QGSP_QEL" )      fPhysicsList = new QGSP_QEL();
}    

//
// public methods
//

//_____________________________________________________________________________
void TG4HadronPhysicsList::ConstructParticle()
{
/// Construct particles.

  // create processes for registered physics
  fPhysicsList->ConstructParticle();
}

//_____________________________________________________________________________
void TG4HadronPhysicsList::ConstructProcess()
{
/// Construct processes.

  // create processes for registered physics
  fPhysicsList->ConstructProcess();

  if (VerboseLevel() > 0) 
    G4cout << "### Hadron physics constructed. " << G4endl;
}

//_____________________________________________________________________________
G4int TG4HadronPhysicsList::VerboseLevel() const 
{
/// Return verbose level (via TG4VVerbose)

  return TG4VVerbose::VerboseLevel();
}

//_____________________________________________________________________________
void TG4HadronPhysicsList::VerboseLevel(G4int level) 
{
/// Set the specified level to both TG4Verbose and 
/// G4VModularPhysicsList.
/// The verbose level is also propagated to the registered physics list.

  TG4VVerbose::VerboseLevel(level);
  fPhysicsList->SetVerboseLevel(level);
}

//_____________________________________________________________________________
void TG4HadronPhysicsList::SetRangeCut(G4double value)
{
/// Reset the default cut to a given value.                                 \n
/// !!! Should be used only in PreInit phase,
/// use SetDefaultCutValue() method of base class to reset
/// the cut value in later phases.

  defaultCutValue = value;
}  
