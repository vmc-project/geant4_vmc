// $Id: g4Config.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example03
// for geometry defined with Root and selected Geant4 native navigation

void Config()
{
  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration 
      = new TG4RunConfiguration("geomRootToGeant4");
  
  // Uncomment these lines to switch on special cuts
  // TG4PhysicsListOptions options;
  // options.SetSpecialCutsPhysics(true);
  // runConfiguration->SetPhysicsListOptions(options);

  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
