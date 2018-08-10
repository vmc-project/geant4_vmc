//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup Monopole
/// \file Monopole/g4Config.C
/// \brief Configuration macro for Geant4 VMC for Monopole example
///
/// For geometry defined with Root and selected Geant4 native navigation

void Config()
{
/// The configuration function for Geant4 VMC for Monopole example
/// called during MC application initialization. 
/// For geometry defined with Root and selected Geant4 native navigation

  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4", "FTFP_BERT+monopole", "stepLimiter");
  
  // ALICE physics configuration
  // TG4RunConfiguration* runConfiguration 
  //   = new TG4RunConfiguration("geomRoot", "FTFP_INCLXX_EMV+optical+monopole", 
  //                             "specialCuts+stackPopper+stepLimiter", true);
  
  // Define monopole properties
  // (uncomment the lines below to change the defaults)
  // runConfiguration->SetParameter("monopoleMass", 100.);
  // runConfiguration->SetParameter("monopoleElCharge", 0.);
  // runConfiguration->SetParameter("monopoleMagCharge", 1.);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
    
  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");

  // Customization of Geant4 VMC (ALICE)
  //
  // geant4->ProcessGeantCommand("/control/verbose 2");  
  // geant4->ProcessGeantCommand("/mcVerbose/all 1");  
  // geant4->ProcessGeantCommand("/tracking/verbose 1");  
  // geant4->ProcessGeantCommand("/mcVerbose/geometryManager 1");  
  // geant4->ProcessGeantCommand("/mcVerbose/opGeometryManager 1");  
  // geant4->ProcessGeantCommand("/mcTracking/loopVerbose 1");     
  // geant4->ProcessGeantCommand("/mcPhysics/rangeCuts 0.01 mm"); 
  
  // geant4->ProcessGeantCommand("/mcVerbose/composedPhysicsList 2");  
  // // geant4->ProcessGeantCommand("/mcTracking/skipNeutrino true");
  // geant4->ProcessGeantCommand("/mcDet/setIsMaxStepInLowDensityMaterials true");
  // geant4->ProcessGeantCommand("/mcDet/setMaxStepInLowDensityMaterials 10 m");
  // geant4->ProcessGeantCommand("/mcMagField/setConstDistance 1 mm");
  // //
  // // optical
  // //
  // geant4->ProcessGeantCommand("/process/optical/verbose 0");
  // geant4->ProcessGeantCommand("/process/optical/processActivation Scintillation 0");
  // geant4->ProcessGeantCommand("/process/optical/processActivation OpWLS 0");
  // geant4->ProcessGeantCommand("/process/optical/processActivation OpMieHG 0");
  // geant4->ProcessGeantCommand("/process/optical/setTrackSecondariesFirst Cerenkov 0");

  // geant4->ProcessGeantCommand("/mcPhysics/emModel/setEmModel  PAI");
  // geant4->ProcessGeantCommand("/mcPhysics/emModel/setRegions  Si");
  // geant4->ProcessGeantCommand("/mcPhysics/emModel/setParticles  all");

  // // geant4->ProcessGeantCommand("/mcPhysics/emModel/setEmModel  SpecialUrbanMsc");
  // // geant4->ProcessGeantCommand("/mcPhysics/emModel/setRegions  Aluminium");
  // // geant4->ProcessGeantCommand("/mcPhysics/emModel/setParticles  e- e+");
}
