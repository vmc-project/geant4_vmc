// $Id: $
//
// Macro for setting visualization for Example03 

void set_vis()
{
  if (TString(gMC->GetName()) == "TGeant3") {
    // Set drawing options
    gMC->Gsatt("*", "seen", 16);
    gMC->Gsatt("ABSO", "seen", 5);
    gMC->Gsatt("GAPX", "seen", 6);
  }    

  if (TString(gMC->GetName()) == "TGeant4") {
    // Setting Geant4 visualization
   ((TGeant4*)gMC)->ProcessGeantMacro("g4vis.in");
  }  
}  
