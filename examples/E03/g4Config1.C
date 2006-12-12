// $Id: g4Config2.C,v 1.2 2006/01/13 16:58:17 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example03
// for geometry defined with Geant4 in a user run configuration


void Config()
{
  // Load G4 novice N02 example library (with detector construction)
  gSystem->Load("$G4INSTALL/tmp/$G4SYSTEM/exampleN03/libexampleN03");

  // Load library with a user run configuration
  gSystem->Load("libgeant4e03");

  // Run configuration with user geometry construction via Geant4
  Ex03RunConfiguration1* runConfiguration 
    = new Ex03RunConfiguration1();
  
  // Uncomment these lines to switch on special cuts
  // TG4PhysicsListOptions options;
  // options.SetSpecialCutsPhysics(true);
  // runConfiguration->SetPhysicsListOptions(options);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");

  cout << "Geant4 has been created." << endl;
}
