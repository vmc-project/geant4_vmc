
// $Id$

/// \file Doxymain.h
/// \brief The main page for MTRoot code documenation


/*! \mainpage MTRoot

This package provides Root IO manager classes which facilitate use of 
ROOT IO in VMC examples and they also handle necessary locking in multi-threaded 
applications. It is independent from Geant4 VMC and it is used in VMC examples.

It provides:
  - TMCRootManager - the Root IO manager for VMC examples for sequential applications
  - TMCRootManagerMT - the Root IO manager for VMC examples for multi-threaded
  applications.

and also  

  - TMCTemplateAutoLock - which provides a mechanism to create a mutex and locks/unlocks it.
   Can be used by applications to implement in a portable way a mutexing logic.
   It was extracted from G4AutoLock implementation for Linux platforms.

*/
