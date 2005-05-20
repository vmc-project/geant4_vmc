// $Id: optPhoton.C,v 1.1 2005/05/17 13:52:01 brun Exp $
//
// Test macro for polarized optical photon
// (adapted optPhoton.mac)
// (Note that Geant3 was not meant to use C photons as primary 
// particles, so this macro cannot be used with G3.)

void optPhoton()
{
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
