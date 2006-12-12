// $Id: g4Config.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Configuration macro for Geant4 VMC for Example01
// for geometry defined with VMC and selected Geant4 native navigation

void Config()
{
  // RunConfiguration for Geant4 
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomVMCtoGeant4");
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
}
