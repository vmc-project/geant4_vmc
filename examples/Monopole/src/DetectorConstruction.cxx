//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file DetectorConstruction.cxx 
/// \brief Implementation of the DetectorConstruction class 
///
/// Geant4 Monopole example adapted to Virtual Monte Carlo \n
///
/// \date 15/07/2018
/// \author I. Hrivnacova; IPN, Orsay
 
#include <Riostream.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <TGeoMaterial.h>
#include <TVirtualMC.h>
#include <TList.h>

#include "DetectorConstruction.h"

using namespace std;

/// \cond CLASSIMP
ClassImp(VMC::Monopole::DetectorConstruction)
/// \endcond

namespace VMC
{
namespace Monopole
{

//_____________________________________________________________________________
DetectorConstruction::DetectorConstruction()
  : TObject(),
    fWorldMaterial("Galactic"),
    fAbsorberMaterial("Aluminium"),
    fAbsorberSizeX(10.),                 
    fAbsorberSizeYZ(10.),
    fWorldSizeX(1.2 * fAbsorberSizeX),
    fWorldSizeYZ(1.2 * fAbsorberSizeYZ),
    fMaxStepSize(0.5),
    fGeometryInitialized(false)
{
/// Default constuctor
}

//_____________________________________________________________________________
DetectorConstruction::~DetectorConstruction()
{
/// Destructor
}

//
// private methods
//

//
// public methods
//

//_____________________________________________________________________________
void DetectorConstruction::ConstructMaterials()
{
/// Construct materials using TGeo modeller

  //
  // Tracking medias (defaut parameters)
  //

  // Create Root geometry manager 
  new TGeoManager("Monopole_geometry", "Monopole VMC example geometry");

//--------- Material definition ---------

  TString name;      // Material name
  Double_t a;        // Mass of a mole in g/mole   
  Double_t z;        // Atomic number
  Double_t density;  // Material density in g/cm3
 
//
// define simple materials
//

  new TGeoMaterial("Aluminium", a=26.98, z=13., density=2.700); 
  new TGeoMaterial("Si", a=28.085, z=14., density=2.330); 
  new TGeoMaterial("Galactic", a=1.e-16, z=1.e-16, density=1.e-16); 

  //
  // Tracking media
  //

  // Paremeter for tracking media  
  Double_t param[20];
  param[0] = 0;     // isvol  - Not used
  param[1] = 2;     // ifield - User defined magnetic field
  param[2] = 10.;   // fieldm - Maximum field value (in kiloGauss)
  param[3] = -20.;  // tmaxfd - Maximum angle due to field deflection 
  param[4] = -0.01; // stemax - Maximum displacement for multiple scat 
  param[5] = -.3;   // deemax - Maximum fractional energy loss, DLS 
  param[6] = .001;  // epsil - Tracking precision
  param[7] = -.8;   // stmin
  for ( Int_t i=8; i<20; ++i) param[i] = 0.;
  
  Int_t mediumId = 0;
  TList* materials = gGeoManager->GetListOfMaterials();
  TIter next(materials);
  while (TObject *obj = next()) {
    TGeoMaterial* material = (TGeoMaterial*)obj;
    // set step limit to other than world material
    if ( material->GetName() != TString("Galactic") ) {
      param[4] = fMaxStepSize; 
    } else {
      param[4] = -0.01;
    }
    new TGeoMedium(material->GetName(), ++mediumId, material, param);
  }
}    

//_____________________________________________________________________________
void DetectorConstruction::ConstructGeometry()
{
/// Contruct volumes using TGeo modeller

  // Construct materials
  ConstructMaterials();

  // Media Ids
  Int_t worldMediumId = gGeoManager->GetMedium(fWorldMaterial.Data())->GetId();
  Int_t absorberMediumId = gGeoManager->GetMedium(fAbsorberMaterial.Data())->GetId();

  //     
  // World
  //
  
  Double_t world[3];
  world[0] = fWorldSizeX/2.;
  world[1] = fWorldSizeYZ/2.;
  world[2] = fWorldSizeYZ/2.;
  TGeoVolume *top = gGeoManager->Volume("World", "BOX", worldMediumId, world, 3);
  gGeoManager->SetTopVolume(top);

  //                               
  // Absorber
  //  
  Double_t absorber[3];
  absorber[0] = fAbsorberSizeX/2.;
  absorber[1] = fAbsorberSizeYZ/2.;
  absorber[2] = fAbsorberSizeYZ/2.;
  gGeoManager->Volume("Absorber", "BOX", absorberMediumId, absorber, 3);

  Double_t posX =  0.;
  Double_t posY =  0.;
  Double_t posZ =  0.;
  Double_t* ubuf = 0;
  gGeoManager->Node("Absorber", 1 ,"World", posX, posY, posZ, 0, kTRUE, ubuf);

  // close geometry
  gGeoManager->CloseGeometry();
    
  // notify VMC about Root geometry
  gMC->SetRootGeometry();
  
  PrintParameters();
}

//_____________________________________________________________________________
void DetectorConstruction::PrintParameters()
{
  cout << "\n---------------------------------------------------------\n";
  cout << "---> The Absorber is " << fAbsorberSizeX
       << " of " << fAbsorberMaterial << endl;
  cout << "\n---------------------------------------------------------\n";
}

//_____________________________________________________________________________
void DetectorConstruction::SetAbsorberSizeX(Double_t value)
{
  if ( fGeometryInitialized ) {
    cerr << "Geometry alredy initialized: cannot set absorber sizeX" << endl;
    return;
  }
  
  if ( value > 0.0 ) {
    fAbsorberSizeX = value; 
    fWorldSizeX = 1.2 * fAbsorberSizeX;
  }
}
  
//_____________________________________________________________________________
void DetectorConstruction::SetAbsorberSizeYZ(Double_t value)
{
  if ( fGeometryInitialized ) {
    cerr << "Geometry alredy initialized: cannot set absorber sizeXY" << endl;
    return;
  }
  
  if ( value > 0.0 ) {
    fAbsorberSizeYZ = value; 
    fWorldSizeYZ = 1.2 * fAbsorberSizeYZ;
  }
}  

//_____________________________________________________________________________
void DetectorConstruction::SetAbsorberMaterial(const TString& name)
{
  // search the material by its name   
  if ( fGeometryInitialized ) {
    cerr << "Geometry alredy initialized: cannot set absorber material" << endl;
    return;
  }

  fAbsorberMaterial = name;
}

//_____________________________________________________________________________

// void DetectorConstruction::SetMagField(Double_t fieldValue)
// {
//   fMonFieldSetup->SetMagField(fieldValue);

//   //apply a global uniform magnetic field along Z axis
//   G4FieldManager * fieldMgr = 
//     G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
//   if (fMagField) { delete fMagField; }    //delete the existing magn field

//   if (fieldValue != 0.)                   // create a new one if non nul
//     {
//       fMagField = new G4UniformMagField(G4ThreeVector(0., 0., fieldValue));
//       fieldMgr->SetDetectorField(fMagField);
//       fieldMgr->CreateChordFinder(fMagField);
//     }
//    else
//     {
//       fMagField = 0;
//       fieldMgr->SetDetectorField(fMagField);
//     }
// }

// //_____________________________________________________________________________
// void DetectorConstruction::ConstructSDandField()
// {

//   // Define magnetic field
//   bool bNewFieldValue = false;
//   if ( fFieldMessenger.Get() != 0 ) {
//     G4ThreeVector fieldSet =  fFieldMessenger.Get()->GetFieldValue();
//     if(fieldSet.z()!=fZMagFieldValue) bNewFieldValue = true;
//   }
//   else bNewFieldValue = true;

//   // Monopole particule specific magnetic field
//   if(bNewFieldValue&&fZMagFieldValue!=0.)
//     fMonFieldSetup->SetMagField(fZMagFieldValue, true);

//   if ( bNewFieldValue ) { 
//     // Create global magnetic field messenger.
//     // Uniform magnetic field is then created automatically if
//     // the field value is not zero.

//     if(fZMagFieldValue!=0.)
//       {
//         G4ThreeVector fieldValue = G4ThreeVector(0.,0.,fZMagFieldValue);
//         G4GlobalMagFieldMessenger* msg =  
//                               new G4GlobalMagFieldMessenger(fieldValue);
//         msg->SetVerboseLevel(1);
//         G4AutoDelete::Register(msg);
//         fFieldMessenger.Put( msg );        
//       }
//   }
  
// }

//_____________________________________________________________________________
void DetectorConstruction::SetMaxStepSize(Double_t step)
{
  if ( fGeometryInitialized ) {
    cerr << "Geometry alredy initialized: cannot set max step size" << endl;
    return;
  }

  fMaxStepSize = step;
}

}
}

