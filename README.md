
Geant4 VMC
===========

Geant4 VMC implements the Virtual Monte Carlo (VMC) for Geant4, see more: <br/>
[https://root.cern.ch/vmc](https://root.cern.ch/vmc)

It also includes G4Root and MTRoot packages, which are independent from Geant4 VMC and can be build and used stand-alone.

[![License: GPL v2](https://img.shields.io/badge/License-GPLv2-blue.svg)](http://www.gnu.org/licenses/gpl-2.0)


## Files and directories

      README.md  - this file
      history    - the tags history
      source     - Geant4 VMC source code
      g4root     - G4Root package
      mtroot     - MTRoot package
      examples   - examples demonstrating use of VMC

## Installation

**IMPORTANT NOTE**: This version currently only works when building against [this ROOT version](https://github.com/benedikt-voelkel/root/tree/v6-14-06-multi-engines-wip-mgr)

Geant4 VMC requires ROOT and Geant4 installed, and optionally, it can be
built with [VGM](http://ivana.home.cern.ch/ivana/VGM.html).

Since version 3.00, Geant4 VMC is installed with CMake. To install
geant4_vmc:

1. First get the Geant4 VMC source from the Download page. We will assume
   that the Geant4 VMC package sits in a subdirectory
   /mypath/geant4_vmc

2. Create build directory alongside our source directory

```bash
$ cd /mypath
$ mkdir geant4_vmc_build
$ ls
geant4_vmc geant4_vmc_build
```

3. To configure the build, change into the build directory and run CMake:

```bash
$ cd /mypath/geant4_vmc_build
$ cmake -DCMAKE_INSTALL_PREFIX=/mypath/geant4_vmc_install /mypath/geant4_vmc
```

   If ROOT and Geant4 environment was defined using thisroot.[c]sh and
   geant4.[c]sh scripts, there is no need to provide path to their installations.
   Otherwise, they can be provided using -DROOT_DIR and -DGeant4_DIR cmake
   options.

4. After the configuration has run, CMake will have generated Unix Makefiles
   for building Geant4 VMC. To run the build, simply execute make in the build
   directory:

```bash
$ make -jN
```

   where N is the number of parallel jobs you require (e.g. if your machine has a dual
   core processor, you could set N to 2).
   If you need more output to help resolve issues or simply for information, run make as

```bash
$ make -jN VERBOSE=1
```

5. Once the build has completed, you can install Geant4 VMC to the directory you specified
   earlier in CMAKE_INSTALL_PREFIX by running

```bash
$ make install
```

   This will build geant4_vmc, g4root and mtroot packages. For VMC examples see VMC
   examples installation page.

## Build Options

Geant4 VMC includes G4Root and MTRoot packages, which are independent from Geant4 VMC
and can be build and used stand-alone. Use of G4Root, VGM, Geant4 G3toG4, UI and VIS
packages in Geant4 VMC library is optional and can be switched on/off during CMake build.

Overview of available options and their default values:

      Geant4VMC_BUILD_G4Root       Build G4Root        ON
      Geant4VMC_BUILD_MTRoot       Build MTRoot        ON
      Geant4VMC_BUILD_Geant4VMC    Build Geant4VMC     ON
      Geant4VMC_BUILD_EXAMPLES     Build VMC examples  ON

      Geant4VMC_USE_G4Root         Build with G4Root                ON
      Geant4VMC_USE_VGM            Build with VGM                   OFF
      Geant4VMC_USE_GEANT4_UI      Build with Geant4 UI drivers     ON
      Geant4VMC_USE_GEANT4_VIS     Build with Geant4 Vis drivers    ON
      Geant4VMC_USE_GEANT4_G3TOG4  Build with Geant4 G3toG4 library OFF

      Geant4VMC_INSTALL_EXAMPLES   Install examples    ON


## Required and optional packages:

Geant4 VMC requires the Root and Geant4 installations,
and optionally VGM in case you want to run with Geant4
native navigation with geometry defined via Root TGeo.

The path to required and optional packages installations can be defined in these
complementary ways:

a) Via path to the CMake configuration file

      ROOT_DIR              ... path to ROOTConfig.cmake
      Geant4_DIR            ... path to Geant4Config.cmake
      VGM_DIR               ... path to VGMConfig.cmake

b) Using their configuration script which is available
   in your PATH (Geant4 and Root):

      geant4-config         ... Geant4 configuration script
      root-config           ... Root configuration script

   To make these configuration scripts available in your PATH,
   you should source the relevant script from the packages
   installation:

      bin/geant4.[c]sh      ... Geant4
      bin/thisroot.[c]sh    ... Root

c) With the environment variable ROOTSYS (Root only)

      ROOTSYS         ... path to Root

The compilation with VGM (optional) can be selected via the CMake option:

      -DGeant4VMC_USE_VGM=ON


## Examples

Since Geant4 VMC 3.00, VMC examples are installed with CMake.
The VMC examples libraries require ROOT installation, the VMC examples programs are built
against Geant3 with VMC or Geant4 VMC libraries.

More details about provided configuration macros and tests
can be found in examples/README.

The instructions how to build and run examples are provided
in examples/README_INSTALL.

##  Root and Geant4 UI

#### Switching UI

When running Geant4 with Virtual MC a user can switch between
Root UI and Geant4 UI:

From Root to Geant4 UI:

      root [0] ((TGeant4*)gMC)->StartGeantUI();

From Geant4 to Root UI:

      PreInit> /mcControl/root

#### Calling macros

Call Geant4 macro "myMacro.in" from Root:

      root [0] ((TGeant4*)gMC)->ProcessGeantMacro("myMacro.in");

Call Root macro "myMacro.C" from Geant4:

      PreInit> /mcControl/rootMacro myMacro


#### Calling commands

Call Geant4 command from Root:

      root [0] ((TGeant4*)gMC)->ProcessGeantCommand("/tracking/verbose 1");

Call Root command from Geant4:

      Idle> /mcControl/rootCmd TBrowser b;
