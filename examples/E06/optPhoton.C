// $Id: $
//
// Test macro for polarized optical photon
// (adapted optPhoton.mac)

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
