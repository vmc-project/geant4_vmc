//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file g4libs_old.C
/// \brief Old macro for loading Geant4 and Geant4 VMC libraries
///
/// Macro for loading Geant4 and Geant4 VMC libraries
/// with using liblist utility provided in Geant4
/// Besides loading libraries, the macro also resets
/// FPE mask to 0, in order to make sure than FPE for
/// FE_OVERFLOW is disabled what is required for Geant4.
///
/// \author Christian Holm Christensen, NBI;
///         Dmitry Naumov, JINR
//
// Macro for loading Geant4 and Geant4 VMC libraries

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <TSystem.h>
#include <Riostream.h>
#include <TCint.h>
#include <TError.h>
#include <TMath.h>
#include <TApplication.h>
#include <TROOT.h>

#endif

void loadg4libs();

void g4libs()
{
/// Function for loading all libraries for running VMC with Geant4
  loadg4libs();

  gSystem->SetFPEMask(0);
}

string NoSpaces(string s)
{
/// ???

  std::stringstream str(s);
  std::string token;
  std::getline(str, token, '\n');
  return token;
}

Bool_t isLibrary(const char* libName)
{
/// Helper function which testes the existence of the given library
/// \param libName  The library name

  if (TString(gSystem->DynamicPathName(libName, kTRUE)) != TString(""))
    return kTRUE;
  else
    return kFALSE;
}

Bool_t isBatch()
{
/// Helper function which testes if Root was started in batch mode

  for ( Int_t i=0; i<gApplication->Argc(); ++i )
    if ( TString(gROOT->GetApplication()->Argv(i)) == "-b" ) return true;

  return false;
}

Bool_t isSet(const char* variable)
{
/// Helper function which checks if the specified environment variable
/// is set.
/// \param variable  The environment variable name

  TString value = gSystem->Getenv(variable);
  if ( value != "") return true;

  return false;
}

void vgmlibs()
{
/// Function for loading VGM libraries.

  if ( isSet("USE_VGM") ) {
    cout << "Loading VGM libraries ... " << endl;
    gSystem->Load("libClhepVGM");
    gSystem->Load("libBaseVGM");
    gSystem->Load("libGeant4GM");
    gSystem->Load("libRootGM");
    gSystem->Load("libXmlVGM");
  }
}

void GetLinkLine(string& all_lines)
{
/// Build the string with the list of libraries using liblist

  // Geant4 lib directory
  TString g4lib  = gSystem->Getenv("G4LIB");
  if ( g4lib.Length() == 0 )
    g4lib  = gSystem->Getenv("G4INSTALL") + TString("/lib");
  g4lib += "/"+TString(gSystem->Getenv("G4SYSTEM"));

  // Build the string with the list of libraries using liblist
  TString command
    = "echo -L"+g4lib+" `" + g4lib+"/liblist -m "+g4lib +" <  " + g4lib+"/libname.map`";
  FILE* pipe = gSystem->OpenPipe(command, "r");
  char line[100];
  while ( fgets(line, sizeof(line), pipe ) != NULL ) {
    all_lines += line;
  }
}

void HandleLinkLine(const char* str, const char* what)
{
/// Tokenize the input string and load/unload the libraries
/// from the list.
/// \param str  The string output from Geant4 liblist
/// \param what The option specifying whether we want to load ('l') or
///             unload ('u') libraries

  // Fill the libs names in the vector
  std::vector<std::string> libs;
  std::stringstream sstream(str);
  unsigned int w = 0;
  while ( ! sstream.eof() ) {
    // Read one string
    std::string token;
    std::getline(sstream, token, ' ');

    // Check stream status
    if ( sstream.bad() ) break;

    // Check that we got a meaningful tokenonent
    if ( token.empty() || std::isspace(token[0]) ) continue;

    if ( token[0] != '-' ) {
      Warning("LoadLibraryList", "Unknown element %s", token.c_str());
      continue;
    }

    std::string dir_or_file = token.substr(2,token.size()-2);
    if ( token[1] == 'L' ) {
      std::stringstream path;
      path << gSystem->GetDynamicPath() << ":"
	   << dir_or_file;
      gSystem->SetDynamicPath(path.str().c_str());
    }
    else if ( token[1] == 'l' ) {
      std::stringstream ln;
      ln << "lib" << NoSpaces(dir_or_file) << '.' << gSystem->GetSoExt();
      std::string lib(ln.str());
      libs.push_back(lib);
      if ( lib.length() > w ) w = lib.length();
    }
    else {
      Warning("LoadLibraryList", "Unknown option %s in",
	      token.c_str(), str);
      continue;
     }
  }

  // Process the vector with libs names and load libraries
  size_t n = libs.size();
  TString sWhat(what);
  Bool_t load = sWhat.Contains("l");
  if (!load && !sWhat.Contains("u")) {
     std::cerr << "  Unknown load action " << what << std::endl;
     return;
  }

  for ( size_t i = 0; i < n; ++i ) {
    size_t idx = n - i - 1;

    // Uncomment to debug
    // size_t m = TMath::Log10(n)+1;
    // string say="   Loading ";
    // if ( TString(what).Contains("u") ) say = "   Unloading ";
    // std::cout << say         << std::setw(m) << (i+1)
    //	      << "/"            << std::setw(m) << n
    //        << ": "           << std::setw(w) << libs[idx] // << std::endl;
    //        << std::flush;

    int result = 0;
    if ( libs[idx].c_str() ) {
       if  (load)
          result = gSystem->Load(libs[idx].c_str());
       else
          gSystem->Unload(libs[idx].c_str());
    }
    // Uncomment to debug
    // if ( TString(what).Contains("l")  )
    //   std::cout << ( result < 0 ? " failed" : " ok" ) << "\r";
  }
  // std::cout << "\n   Done" << std::endl;
}

void loadg4libs()
{
/// The function to unload Geant4 libraries

  // CLHEP
  gSystem->Load("libCLHEP");

  // xerces-c library if GDML is activated
  if ( isSet("G4LIB_BUILD_GDML") ) {
    gSystem->Load("libxerces-c");
  }

  // Get the string with the list of libraries
  string all_lines;
  GetLinkLine(all_lines);

  // Load Geant4 libraries
  cout << "Loading Geant4 libraries (using liblist) ..." << endl;
  HandleLinkLine(all_lines.c_str(),"l");

  // VGM librares
  vgmlibs();

  // G4Root library (if available)
  if ( isLibrary("libg4root") ) {
    cout << "Loading g4root library ..." << endl;
    gSystem->Load("libg4root");
  }

  // Geant4 VMC library
  cout << "Loading geant4vmc library ..." << endl;
  gSystem->Load("libgeant4vmc");

  // Geant4 VMC GUI library
  // (if available and Root is not running in batch mode)
  if ( isLibrary("libgeant4vmc_gui") && ! isBatch() ) {
    cout << "Loading geant4vmc_gui library ... " << endl;
    gSystem->Load("libgeant4vmc_gui");
  }
}

void unloadg4libs()
{
/// The function to unload Geant4 libraries

  // Get the string with the list of libraries
  string all_lines;
  GetLinkLine(all_lines);

  // Load Geant4 libraries
  cout << "Unloading Geant4 libraries ..." << endl;
  HandleLinkLine(all_lines.c_str(),"u");
}

