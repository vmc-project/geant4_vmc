// $Id: $
//
// Example03 
// Macro for reading the E03 simulated data from Root file

{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load this example library
  gSystem->Load("libexample03");
  
  // MC application
  Ex03MCApplication* appl 
    =  new Ex03MCApplication("Example03", "The example03 MC application", kRead);

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i);
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }  
}  
