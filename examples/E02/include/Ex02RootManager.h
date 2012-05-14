// $Id$

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file E02/include/Ex02RootManager.h 
/// \brief Definition of the Ex02RootManager class 
///
/// Geant4 ExampleN02 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay


#ifndef EX02_ROOT_MANAGER_H
#define EX02_ROOT_MANAGER_H

#include <TObject.h>
#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>

class TParticle;

/// Root file mode
enum FileMode { 
  kRead,  ///< Read mode 
  kWrite  ///< Write mode
};

/// \ingroup E02
/// \brief Class that takes care of Root IO
///
/// Geant4 novice ExampleN02 adapted to Virtual Monte Carlo 
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex02RootManager : public TObject
{
  public:
    Ex02RootManager(const char* projectName, FileMode fileMode);
    Ex02RootManager();
    virtual ~Ex02RootManager();     
  
    // static access method
    static Ex02RootManager* Instance(); 

    // methods
    void  Register(const char* name, const char* className, void* objAddress);
    void  Fill();
    void  WriteAll();
    void  ReadEvent(Int_t i);
    
  private:
    // data members
    static  Ex02RootManager* fgInstance; ///< Singleton instance

    // data members
    TFile*  fFile; ///< Root output file
    TTree*  fTree; ///< Root output tree 
    TString fPath; ///< The path to the root file
    
    ClassDef(Ex02RootManager,0) // Root IO manager
};

#endif //EX02_ROOT_MANAGER_H   
   

