// $Id: g3Config.C,v 1.2 2005/01/05 08:05:30 brun Exp $
//
// Configuration macro for Geant3 VirtualMC for Example03

void Config()
{
  new  TGeant3("C++ Interface to Geant3"); 
  cout << "Geant3 has been created." << endl;

  gMC->SetProcess("CKOV",1);
  gMC->SetCut("CUTELE",10e-06);
  gMC->SetCut("CUTGAM",10e-06);
  
  ((TGeant3*)gMC)->SetSWIT(4,1000);
      // reduce printing from GTREVE_ROOT (sets one printing per 1000 tracks) 
}


