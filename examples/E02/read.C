// $Id: run_g3.C,v 1.2 2002/08/29 10:12:26 ivana Exp $
//
// Example02 
// Macro for reading the Ex02MCApplication from Root file

{
  // Load basic libraries
  gROOT->LoadMacro("../macro/basiclibs.C");
  basiclibs();

  // Load this example library
  gSystem->Load("libexample02");
  
  // MC application
  Ex02MCApplication* appl 
    =  new Ex02MCApplication("Example02", "The example02 MC application", kRead);

  appl->ReadEvent(1);

}  
