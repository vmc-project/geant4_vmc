//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file E06/optPhoton.C
/// \brief est macro for polarized optical photon for Example06

void optPhoton()
{
/// Test macro for polarized optical photon
/// (adapted optPhoton.mac).
/// (Note that Geant3 was not meant to use C photons as primary 
/// particles, so this macro cannot be used with G3.)

  // Primary generator
  Ex06PrimaryGenerator* generator
    =  Ex06MCApplication::Instance()->GetPrimaryGenerator();
    
  // Set optical photon properties
  generator->SetParticle(50000050);  // optical photon
  generator->SetKinEnergy(3.0e-09);  // 3 eV 
  generator->SetDirection(5, -3, 0); // 
  generator->SetOptPhotonPolar(90.); // 90. deg 

  generator->SetNofPrimaries(10); 
}    
