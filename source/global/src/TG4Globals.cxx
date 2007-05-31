// $Id: TG4Globals.cxx,v 1.5 2006/12/12 16:21:15 brun Exp $

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

// Category: global
// Class TG4Globals
// ----------------
// See the class description in the header file.

#include "TG4Globals.h"

#include <stdlib.h>

const TString TG4Globals::fgkEndl = "x\n";

//_____________________________________________________________________________
TG4Globals::~TG4Globals() {
//
}
  
//
// public static methods
//

//_____________________________________________________________________________
void TG4Globals::Exception(const TString& className,const TString& methodName,
                           const TString& text)
{
/// Print error message end exit the program.

  TString newText = "x\n" + className + "::" + methodName + ":x\n";
  newText += text + "\n";
  newText += "*** TG4Exception: Aborting execution ***";
  newText.ReplaceAll("x\n", "\n    ");
  
  G4cerr << newText.Data() << G4endl << G4endl;   
  exit(1);
}

//_____________________________________________________________________________
void TG4Globals::Warning(const TString& className,const TString& methodName,
                         const TString& text)
{
/// Print warning message.

  TString newText = "++++  TG4Warning:  ++++x\n";
  newText += className + "::" + methodName + ":x\n";
  newText += text + "\n";
  newText += "+++++++++++++++++++++++";
  newText.ReplaceAll("x\n", "\n    ");
  
  G4cerr << newText.Data() << G4endl << G4endl;   
}

//_____________________________________________________________________________
void TG4Globals::AppendNumberToString(G4String& s, G4int a)
{
/// Append number to string.

  const char* kpNumber="0123456789";
  G4String p=""; G4String q="";
  do 
  {
    G4int b=a/10;
    G4int c=a%10;
    p=kpNumber[c];
    q=p.append(q);
    a=b;        
  } while (a>0);
  s.append(q);
}

//_____________________________________________________________________________
G4bool TG4Globals::Compare(G4bool activation, TG4G3ControlValue controlValue)
{
/// Compare the boolean value of the process activation
/// with the process control value.
/// Return true if the values correspond, false otherwise.

  if (controlValue == kUnsetControlValue) {
    TG4Globals::Warning(
      "TG4Globals", "Compare", "Control value = kUnset.");
    return false;
  }    

  if (controlValue == kActivate || controlValue == kActivate2)
    return activation;
  else
    return !activation;  
}  

//_____________________________________________________________________________
void TG4Globals::PrintStars(G4bool emptyLineFirst)
{
/// Print stars.
  

  if (emptyLineFirst)  G4cout << G4endl;
  
  G4cout << "**********************************************" << G4endl;
     
  if (!emptyLineFirst) G4cout << G4endl;
}

//_____________________________________________________________________________
G4String TG4Globals::Help()
{
/// Return VMC mailing list address.
  
  return G4String("vmc@root.cern.ch");
}  
  
