// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4HadronPhysicsList.cxx
/// \brief Implementation of the TG4HadronPhysicsList class 
///
/// \author I. Hrivnacova; IPN, Orsay

#include "TG4HadronPhysicsList.h"

#include "G4PhysListFactory.hh"

const G4double TG4HadronPhysicsList::fgkDefaultCutValue = 1.0 * mm;

//
// static methods
//


//_____________________________________________________________________________
G4String TG4HadronPhysicsList::AvailableSelections()
{
/// Return list of all available selections

  G4String selections;
  selections += "FTFC ";
  selections += "FTFP FTFP_BERT FTFP_EMV FTF_BIC  ";
  selections += "LBE ";
  selections += "LHEP LHEP_BERT LHEP_EMV LHEP_PRECO_HP ";
  selections += "QBBC ";
  selections += "QGSC QGSC_BERT QGSC_EFLOW QGSC_EMV "; 
  selections += "QGSP ";
  selections += "QGSP_BERT QGSP_BERT_DIF QGSP_BERT_EMV QGSP_BERT_HP QGSP_BERT_NQE QGSP_BERT_TRV ";
  selections += "QGSP_BIC QGSP_BIC_HP ";
  selections += "QGSP_DIF ";
  selections += "QGSP_EMV QGSP_EMV_NQE QGSP_EMX QGSP_NQE QGSP_QEL ";
  selections += "QGS_BIC ";
  
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

//
// ctors, dtor
//

//_____________________________________________________________________________
TG4HadronPhysicsList::TG4HadronPhysicsList(const G4String& selection)
  : G4VUserPhysicsList(),
    TG4Verbose("hadronPhysicsList")
{
/// Standard constructor

  Configure(selection);

  defaultCutValue = fgkDefaultCutValue;

  SetVerboseLevel(TG4VVerbose::VerboseLevel());
}

//_____________________________________________________________________________
TG4HadronPhysicsList::~TG4HadronPhysicsList() 
{
/// Destructor

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

  G4PhysListFactory phyListFactory;

  fPhysicsList = phyListFactory.GetReferencePhysList(sel);
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
