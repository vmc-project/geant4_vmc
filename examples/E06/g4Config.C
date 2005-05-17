// $Id: g4Config.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example03

void Config()
{
  // Default Geant4 VMC run configuration
  TG4RunConfiguration* runConfiguration = new TG4RunConfiguration();
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
