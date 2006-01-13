// $Id: g4Config.C,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example03

void Config()
{
  // Default Geant4 VMC run configuration
  TG4RunConfiguration* runConfiguration = new TG4RunConfiguration();
  
  // Switch on optical physics (not activated by default)
  TG4PhysicsListOptions options;
  options.SetOpticalPhysics(true);
  runConfiguration->SetPhysicsListOptions(options);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
