// $Id: g3Config.C,v 1.1 2003/03/17 14:56:50 brun Exp $
//
// Configuration macro for Geant3+TGeo VirtualMC for Example03

void Config()
{
  TGeant3* geant3
    = new TGeant3TGeo("C++ Interface to Geant3 with TGeo geometry"); 
  
  cout << "Geant3 has been created." << endl;

  gMC->SetProcess("DRAY",1);
  gMC->SetProcess("LOSS",1);
  gMC->SetProcess("HADR",0);
}


