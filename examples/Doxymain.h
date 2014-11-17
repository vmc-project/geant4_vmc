//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/*! 

/// \file Doxymain.h
/// \brief The main page for the VMC examples code documenation

\mainpage The VMC examples


\section exa_s1 Introduction

  To demonstrate use of VirtualMC, five Geant4 examples has been rewritten
  as VMC applications:
  \ref E01, \ref E02, \ref E03, \ref E06, \ref A01.
  
  In previous versions of Geant4, these examples
  were provide as novice (N01, N02, N03, N06) and extended (A01) examples.
  The correspondence with the examples in Geant4 10.00.x is:
  - E01 - no equivalent
  - E02 - basic/B2
  - E03 - basic/B4
  - E06 - extended/optical/OpNovice
  - A01 - basic/B5
  
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
  This made the transition from a Geant3 application to VMC easier.
  
  Since the integration of the TGeo geometry modeller,
  users have a possibility to define geometry directly via TGeo,
  with more user-friendly interfaces.

  All VMC examples define by default geometry via TGeo (since 
  Geant4 VMC version 2.0).
  The equivalent geometry definition via TVirtualMC 
  interface can be activated by setting the option "oldGeometry"
  to the MC application (via Ex0NMCApplication::SetOldGeometry(kTRUE)).

\section exa_s3 Macros for running examples

  In all examples there are provides macros: 
<pre>  
run_g3.C   - for running the example with Geant3
run_g4.C   - for running the example with Geant4      
</pre>  

  Since Geant4 VMC 3.0 loading of libraries was separated from run_g3[g4].C
  macros in new macros: 
<pre>  
load_g3.C   - loading all libraries needed to run the example with Geant3
load_g4.C   - loading all libraries needed to run the example with Geant4    
</pre>  
  
\section exa_s4 Configuration macros:

  The selection of the concrete MC, geometry navigation
  and eventually other options is done in the configuration
  macros.
  The following configuration macros are provided with
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
root[0] .x load_g3.C  
root[1] .x run_g3.C  

With G4:
root[0] .x load_g4.C  
root[1] .x run_g4.C  

With G4 + TGeo navigation:
root[0] .x load_g4.C  
root[1] .x run_g4.C("g4tgeoConfig.C"); 
</pre>  
  
\section exa_s6 Test macros:

  To test all possible configurations, a test macro (or a set of macros) 
  is provided for each example: 
<pre>  
test_EN.C(const TString& configMacro, Bool_t oldGeometry)
</pre>  
  with parameters:
     - configMacro: configuration macro loaded in initialization 
     - oldGeometry: if true - geometry is defined via VMC, otherwise via TGeo
     
  The tests with all configurations can be run via the test suite: 
<pre>  
test_suite.sh
</pre>  
  which saves all test outputs in the files which names correspond
  to test run configuration:
<pre>
test_g3_tgeo_tgeo.out - G3, geometry defined via TGeo, TGeo navigation
test_g3_vmc_nat.out   - G3, geometry defined via VMC,  G3 native navigation
test_g3_vmc_tgeo.out  - G3, geometry defined via VMC,  TGeo navigation
test_g4_tgeo_nat.out  - G4, geometry defined via TGeo, G4 native navigation
test_g4_tgeo_tgeo.out - G4, geometry defined via TGeo, TGeo navigation
test_g4_vmc_nat.out   - G4, geometry defined via VMC,  G4 native navigation
test_g4_vmc_tgeo.out  - G4, geometry defined via VMC,  TGeo navigation
</pre>
  The files are saved in log/E0N directory.
  The reference output files, which are updated with each release,
  can be found at log_ref directory.  
   
  All run_*.C macros can be run via run suite script: 
<pre>  
run_suite.sh
</pre>  
  which saves all output in run_*.out files.
  
*/
