// $Id: g3Config.C,v 1.3 2003/02/26 13:36:44 brun Exp $
//
// Configuration macro for Geant3+TGeo VirtualMC for Example02 

void Config()
{
  TGeant3* geant3
    = new  TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry"); 
  
  cout << "Geant3 has been created." << endl;

  geant3->SetHADR(0);
}


