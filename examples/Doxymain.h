// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/*! 

/// \file Doxymain.h
/// \brief The main page for the VMC examples code documenation

\mainpage The VMC examples


\section exa_s1 Introduction

  To demonstrate use of VirtualMC, four Geant4 novice
  examples (N01, N02, N03, N06) and one extended example (analysis/A01) 
  have been rewritten as VMC applications: 
  \ref E01, \ref E02, \ref E03, \ref E06, \ref A01.
  
  See more detail description in the README files
  withing each example subdirectory:
  - \ref README_E01
  - \ref README_E02
  - \ref README_E03
  - \ref README_E06
  - \ref README_A01

\section exa_s2 Geometry definition

  The VirtualMC defines since beginning the methods for geometry 
  construction in the Geant3 style (the functions have the same names and 
  the same parameters sequence as the functions in Geant3).
  This makes the transition from a Geant3 application to VMC easier.
  
  Since the integration of the TGeo geometry modeller,
  user has a possibility to define geometry directly via TGeo,
  with more user-friendly interfaces.

  All VMC examples define by default geometry via TGeo (since 
  Geant4 VMC version 2.0).
  The equivalent geometry definition via TVirtualMC 
  interface can be activated by setting the option "oldGeometry"
  to the MC application (via Ex0nMCApplication::SetOldGeometry(kTRUE)).

\section exa_s3 Macros for running examples

  In all examples there are provides macros:
<pre>  
run_g3.C   - for running example with Geant3
run_g4.C   - for running example with Geant4      
</pre>  

\section exa_s4 Configuration macros:

  The selection of the concrete MC, geometry navigation
  and eventually other options is done in the configuration
  macros.
  Here is a list of configuration macros provided with
  the examples:
<pre>  
g3Config.C      - for G3 with native geometry 
g3tgeoConfig.C  - for G3 with TGeo geometry (default)

g4Config.C      - for G4 with native geometry navigation (default)
g4tgeoConfig.C  - for G4 with TGeo geometry navigation
xyzOld.C        - the same as above but for geometry defined via VMC 

g4config.in  - macro for G4 configuration using G4 commands
g4vis.in     - macro for G4 visualization settings
</pre>  

\section exa_s5 To run an example:
<pre>  
With G3 + TGeo:
root[0] .x run_g3.C  

With G4:
root[0] .x run_g4.C  

With G4 + TGeo navigation:
root[0] .x run_g4.C("g4tgeoConfig.C"); 
</pre>  
  
\section exa_s6 Test macros:

  To test all possible configurations, there is provided a test
  macro for each examples
<pre>  
test_En.C(const TString& configMacro, Bool_t oldGeometry)
</pre>  
  with parameters:
     - configMacro: configuration macro loaded in initialization 
     - oldGeometry: if true - geometry is defined via VMC, otherwise via TGeo
     
  The tests with all configurations can be run within
  the test suite:
<pre>  
test_suite.sh
</pre>  
  which saves all test outputs in the files which names correspond
  to run configuration:
<pre>
test_g3_tgeo_tgeo.out - G3, geometry defined via TGeo, TGeo navigation
test_g3_vmc_nat.out   - G3, geometry defined via VMC,  G3 native navigation
test_g3_vmc_tgeo.out  - G3, geometry defined via VMC,  TGeo navigation
test_g4_tgeo_nat.out  - G4, geometry defined via TGeo, G4 native navigation
test_g4_tgeo_tgeo.out - G4, geometry defined via TGeo, TGeo navigation
test_g4_vmc_nat.out   - G4, geometry defined via VMC,  G4 native navigation
test_g4_vmc_tgeo.out  - G4, geometry defined via VMC,  TGeo navigation
</pre>
  The files are saved in log/E0n directory.
  The reference output files, which are updated with each release,
  can be found at log_ref directory.  
   
  All run_*.C macros can be run via run suite script:
<pre>  
run_suite.sh
</pre>  
  which saves all output in run_*.out files.
  
*/
