// $Id: g3Config.C,v 1.1.1.1 2002/09/27 10:00:02 rdm Exp $
//
// Configuration macro for Geant3 VirtualMC for Example01 

void Config()
{
  TGeant3* geant3
    = new  TGeant3("C++ Interface to Geant3"); 
  
  cout << "Geant3 has been created." << endl;

  geant3->SetHADR(0);
}


