// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file g4libs_old.C
/// \brief Old macro for loading Geant4 and Geant4 VMC libraries
///
/// Besides loading libraries, the macro also resets 
/// FPE mask to 0, in order to make sure than FPE for
/// FE_OVERFLOW is disabled what is required for Geant4.

//
// Macro for loading Geant4 and Geant4 VMC libraries

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>

#include <TSystem.h>
#include <TString.h>
#include <TApplication.h>
#include <TROOT.h>

#endif

void g4libs_granular();
void g4libs_global();

void g4libs()
{
/// Macro function for loading Geant4 granular libraries.
/// Change the comment if global libraries are used.

  g4libs_granular();
  //g4libs_global();
  
  gSystem->SetFPEMask(0); 
}   

Bool_t isSet(const char* variable)
{
/// Helper function which checks if the specified environment variable 
/// is set.
/// \param variable  The environment variable name

  TString value = gSystem->Getenv(variable);
  if ( value != "") return true;
  
  return false;
}  

Bool_t isLibrary(const char* libName)
{
/// Helper function which testes the existence of the given library
/// \param libName  The library name

  if (TString(gSystem->DynamicPathName(libName, kTRUE)) != TString(""))
    return kTRUE;
  else  
    return kFALSE;
}    

Bool_t isBatch()
{
/// Helper function which testes if Root was started in batch mode

  for ( Int_t i=0; i<gApplication->Argc(); ++i ) 
    if ( TString(gROOT->GetApplication()->Argv(i)) == "-b" ) return true;
  
  return false;
}    

void vgmlibs()
{ 
/// Macro function for loading VGM libraries.

  if (isSet("USE_VGM")) { 
    cout << "Loading VGM libraries ... " << endl;
    gSystem->Load("libClhepVGM");
    gSystem->Load("libBaseVGM");
    gSystem->Load("libGeant4GM");
    gSystem->Load("libRootGM");
    gSystem->Load("libXmlVGM");
  }  
}
  
void g4libs_graphics(Bool_t granular = true) 
{
/// Macro function for loading Geant4 graphics libraries, 
/// external packages: graphics drivers, .. used by Geant4.
  
  // Graphics configuration
  Bool_t isXt = isSet("G4VIS_BUILD_OPACS_DRIVER") ||
                isSet("G4VIS_BUILD_OPENGLXM_DRIVER") ||
                isSet("G4VIS_BUILD_OIX_DRIVER") ||
		isSet("G4UI_BUILD_XM_SESSION") ||
		isSet("G4UI_BUILD_XAW_SESSION") ||
		isSet("G4UI_BUILD_WO_SESSION");
  Bool_t isXm = isSet("G4VIS_BUILD_OPENGLXM_DRIVER") ||
  		isSet("G4UI_BUILD_XM_SESSION");
  Bool_t isXaw = isSet("G4UI_BUILD_XAW_SESSION");
  Bool_t isGAG = !isSet("G4UI_NONE") && isSet("G4UI_USE_GAG");
  Bool_t isUI  = !isSet("G4UI_NONE");
  Bool_t isDAWN = !isSet("G4VIS_NONE");
  Bool_t isOpenGL = !isSet("G4VIS_NONE") &&
                    (isSet("G4VIS_BUILD_OPENGLX_DRIVER") ||
                     isSet("G4VIS_BUILD_OPENGLXM_DRIVER"));
  Bool_t isQt = !isSet("G4VIS_NONE") &&
                (isSet("G4VIS_BUILD_OPENGLQT_DRIVER") ||
                 isSet("G44UI_BUILD_QT_SESSION"));
  Bool_t isOIX = !isSet("G4VIS_NONE") &&
                 (isSet("G4VIS_BUILD_OIX_DRIVER") ||
                  isSet("G4VIS_BUILD_OI_DRIVER"));
  Bool_t isVRML = !isSet("G4VIS_NONE");
  Bool_t isRayTracer = !isSet("G4VIS_NONE");

  // Geant4 interfaces
  //
  if (isXt) {
    gSystem->Load("libXt");
  }
  if (isXm) {
    gSystem->Load("libXm");
  }
  if (isXaw) {
    gSystem->Load("libXaw");
  }
  
  if (isQt) {
    gSystem->Load("libQtCore");
    gSystem->Load("libQtGui");
    gSystem->Load("libQtOpenGL");
    gSystem->Load("libQtSql");
    gSystem->Load("libQtXml");
  }

  if (isUI) {
    if (granular) {
      gSystem->Load("libG4UIcommon");
      gSystem->Load("libG4UIbasic");
    }
    else
      gSystem->Load("libG4interfaces");
  }    
   
  if (isGAG) 
    gSystem->Load("libG4UIGAG");

  // Geant4 visualization
  //
  gSystem->Load("libG4modeling");
  gSystem->Load("libG4vis_management");
  gSystem->Load("libG4Tree");
  gSystem->Load("libG4visHepRep");
  gSystem->Load("libG4visXXX");
  gSystem->Load("libG4GMocren");
  if (isDAWN)
    gSystem->Load("libG4FR");
  if (isOpenGL) {
    gSystem->Load("libGLU");
    gSystem->Load("libGL");
    gSystem->Load("libG4OpenGL");
  }  
  if (isOIX) {
    cout << "loading OpenInventor" << endl;
    gSystem->Load("libInventor");
    gSystem->Load("libG4OpenInventor");
  }  
  if (isVRML)
    gSystem->Load("libG4VRML");
  if (isRayTracer)
    gSystem->Load("libG4RayTracer");
}


void g4libs_granular()
{
/// Macro function for loading Geant4 granular libraries, Geant4 VMC library
/// and external packages: CLHEP, graphics drivers, .. used by Geant4.

  cout << "Loading Geant4 granular libraries ..." << endl;

  // CLHEP
  gSystem->Load("libCLHEP");

  // G4 categories

  // global
  gSystem->Load("libG4globman");  
  gSystem->Load("libG4hepnumerics");

  // graphics_reps
  gSystem->Load("libG4graphics_reps");   

  // intercoms
  gSystem->Load("libG4intercoms");

  // materials
  gSystem->Load("libG4materials");

  // geometry
  gSystem->Load("libG4geometrymng");  
  gSystem->Load("libG4magneticfield");
  gSystem->Load("libG4volumes");
  gSystem->Load("libG4navigation");
  gSystem->Load("libG4geomBoolean");  
  gSystem->Load("libG4csg");  
  gSystem->Load("libG4brep"); 
  gSystem->Load("libG4specsolids"); 
  gSystem->Load("libG4geombias");
  gSystem->Load("libG4geomdivision");
  
  // particles  
  gSystem->Load("libG4partman");
  gSystem->Load("libG4partutils");
  gSystem->Load("libG4bosons");   
  gSystem->Load("libG4baryons");  
  gSystem->Load("libG4ions");
  gSystem->Load("libG4mesons");
  gSystem->Load("libG4leptons");
  gSystem->Load("libG4shortlived");

  // track
  gSystem->Load("libG4track");

  // processes
  gSystem->Load("libG4procman");
  gSystem->Load("libG4parameterisation");
  gSystem->Load("libG4transportation");
  gSystem->Load("libG4cuts");
  gSystem->Load("libG4decay");  
     
  gSystem->Load("libG4emutils");  
  gSystem->Load("libG4emstandard");   
  gSystem->Load("libG4emlowenergy");  
  gSystem->Load("libG4muons");
  gSystem->Load("libG4emhighenergy");
  gSystem->Load("libG4xrays");
  gSystem->Load("libG4optical");

  gSystem->Load("libG4hadronic_util");
  gSystem->Load("libG4hadronic_xsect");
  gSystem->Load("libG4hadronic_stop");
  gSystem->Load("libG4hadronic_mgt");   
  gSystem->Load("libG4hadronic_proc");
  gSystem->Load("libG4had_mod_man.so");
  gSystem->Load("libG4had_im_r_matrix.so");
  gSystem->Load("libG4had_string_man.so");
  gSystem->Load("libG4had_string_diff.so");
  gSystem->Load("libG4had_string_frag.so");
  gSystem->Load("libG4had_mod_util.so");
  gSystem->Load("libG4hadronic_qgstring");
  gSystem->Load("libG4hadronic_HE");  
  gSystem->Load("libG4hadronic_LE");  
  gSystem->Load("libG4hadronic_RPG");
  gSystem->Load("libG4hadronic_deex_util"); 
  gSystem->Load("libG4hadronic_bert_cascade.so");
  gSystem->Load("libG4had_muon_nuclear.so");
  gSystem->Load("libG4had_neu_hp.so");
  gSystem->Load("libG4had_lll_fis.so");
  gSystem->Load("libG4had_preequ_exciton.so");
  gSystem->Load("libG4had_theo_max.so");
  gSystem->Load("libG4hadronic_abrasion.so");
  gSystem->Load("libG4hadronic_binary.so"); 
  gSystem->Load("libG4hadronic_bert_cascade.so");
  gSystem->Load("libG4hadronic_body_ci.so");
  gSystem->Load("libG4hadronic_coherent_elastic.so"); 
  gSystem->Load("libG4hadronic_deex_management.so");
  gSystem->Load("libG4hadronic_deex_gem_evaporation.so");
  gSystem->Load("libG4hadronic_deex_evaporation.so");
  gSystem->Load("libG4hadronic_deex_fermi_breakup.so");
  gSystem->Load("libG4hadronic_deex_fission.so");
  gSystem->Load("libG4hadronic_deex_handler.so");
  gSystem->Load("libG4hadronic_deex_multifragmentation.so");
  gSystem->Load("libG4hadronic_deex_photon_evaporation.so");
  gSystem->Load("libG4hadronic_em_dissociation");
  gSystem->Load("libG4hadronic_hetcpp_evaporation.so");
  gSystem->Load("libG4hadronic_hetcpp_utils.so");
  gSystem->Load("libG4hadronic_interface_ci.so");
  gSystem->Load("libG4hadronic_incl_cascade.so");
  gSystem->Load("libG4hadronic_iso.so");
  gSystem->Load("libG4hadronic_qmd.so");
  gSystem->Load("libG4hadronic_leading_particle.so");
  gSystem->Load("libG4hadronic_radioactivedecay.so");

  // tracking
  gSystem->Load("libG4tracking");

  // digits_hits  
  gSystem->Load("libG4hits");
  gSystem->Load("libG4digits");   
  gSystem->Load("libG4detutils");
  gSystem->Load("libG4detector");   

  // parameterisation
  gSystem->Load("libG4gflash");
  //gSystem->Load("libG4trdmodels");

  // event
  gSystem->Load("libG4event");  

  // readout
  gSystem->Load("libG4readout");
  
  // run
  gSystem->Load("libG4run");
  
  // g3tog4
  gSystem->Load("libG3toG4");   

  // interfaces and graphics
  g4libs_graphics();
  
  // physics lists
  gSystem->Load("libG4phys_builders");
  gSystem->Load("libG4phys_lists");

  // VGM libraries
  vgmlibs();
    
  // G4Root library (if available)
  if ( isLibrary("libg4root") ) {   
    cout << "Loading g4root library ..." << endl;
    gSystem->Load("libg4root");
  }  

  // Geant4 VMC library
  cout << "Loading geant4vmc library ... " << endl;
  gSystem->Load("libgeant4vmc");
  
  // Geant4 VMC GUI library 
  // (if available and Root is not running in batch mode)
  if ( isLibrary("libgeant4vmc_gui") && ! isBatch() ) {
    cout << "Loading geant4vmc_gui library ... " << endl;
    gSystem->Load("libgeant4vmc_gui");
  }  
  
  cout << "Loading libraries ... finished" << endl;
}

void g4libs_global()
{
/// Macro function for loading Geant4 global libraries, Geant4 VMC library 
/// and external packages: CLHEP, graphics drivers, .. used by Geant4

  cout << "Loading Geant4 global libraries ..." << endl;
 
   // CLHEP
  gSystem->Load("$(CLHEP_BASE_DIR)/lib/libCLHEP");

  // Geant4
  gSystem->Load("libG4global");
  gSystem->Load("libG4graphics_reps");
  gSystem->Load("libG4intercoms");
  gSystem->Load("libG4materials");
  gSystem->Load("libG4geometry");
  gSystem->Load("libG4particles");
  gSystem->Load("libG4track");
  gSystem->Load("libG4processes");
  gSystem->Load("libG4tracking");
  gSystem->Load("libG4digits_hits");
  gSystem->Load("libG4event");
  gSystem->Load("libG4readout");
  gSystem->Load("libG4run");
  gSystem->Load("libG3toG4");

  // interfaces and graphics
  g4libs_graphics(false);
 
  // physics lists
  gSystem->Load("libG4physicslists");

  // VGM libraries
  vgmlibs();

  // geant4 VMC
  gSystem->Load("libgeant4vmc");

  cout << "Loading libraries ... finished" << endl;
}

