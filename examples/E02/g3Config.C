// $Id: $
//
// Configuration macro for Geant3 VirtualMC for Example01 

void Config()
{
  TGeant3 geant3
    = new  TGeant3("C++ Interface to Geant3"); 
  
  cout << "Geant3 has been created." << endl;

  geant3->SetHADR(0);
}


