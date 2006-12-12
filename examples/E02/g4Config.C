// $Id: g4Config.C,v 1.2 2003/02/26 13:36:44 brun Exp $
//
// Configuration macro for Geant4 VirtualMC for Example02
// for geometry defined with Root and selected Geant4 native navigation

void Config()
{
  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRootToGeant4");
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  
  // Customise Geant4 setting
  // (verbose level, global range cut, ..)
  geant4->ProcessGeantMacro("g4config.in");
}
