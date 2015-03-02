//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file A01DetectorConstruction.cxx 
/// \brief Implementation of the A01DetectorConstruction class 
///
/// Geant4 example A01 adapted to Virtual Monte Carlo

#include "A01DetectorConstruction.hh"
#include "B5MagneticField.hh"

// Added for VMC
#include "TG4GeometryManager.h"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//#include "A01CellParameterisation.h"
/*
#include "A01Hodoscope.hh"
#include "A01DriftChamber.hh"
#include "A01EmCalorimeter.hh"
#include "A01HadCalorimeter.hh"

#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "TGeoManager.h"
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal B5MagneticField* A01DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* A01DetectorConstruction::fFieldMgr = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

A01DetectorConstruction::A01DetectorConstruction(G4bool useLocalMagField)
 : fMagneticLogical(0),
   fVisAttributes(),
   fArmAngle(30.*deg), fSecondArmPhys(0),
   fUseLocalMagField(useLocalMagField)

{
  fArmRotation = new G4RotationMatrix();
  fArmRotation->rotateY(fArmAngle);

  // define commands for this class
  DefineCommands();
}

A01DetectorConstruction::~A01DetectorConstruction()
{
  delete fArmRotation;
  delete fMessenger;

  for (G4int i=0; i<G4int(fVisAttributes.size()); ++i) {
    delete fVisAttributes[i];
  }
}

G4VPhysicalVolume* A01DetectorConstruction::Construct()
{
  ConstructMaterials();
  G4Material* air = G4Material::GetMaterial("G4_AIR");
  //G4Material* argonGas = G4Material::GetMaterial("B5_Ar");
  G4Material* argonGas = G4Material::GetMaterial("G4_Ar");
  G4Material* scintillator
    = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* csI = G4Material::GetMaterial("G4_CESIUM_IODIDE");
  G4Material* lead = G4Material::GetMaterial("G4_Pb");
    
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // geometries --------------------------------------------------------------
  // experimental hall (world volume)
  G4VSolid* worldSolid = new G4Box("worldBox",10.*m,3.*m,10.*m);
  G4LogicalVolume* worldLogical
    = new G4LogicalVolume(worldSolid,air,"worldLogical",0,0,0);
  G4VPhysicalVolume* worldPhysical
    = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                        false,0,checkOverlaps);

  // Tube with Local Magnetic field
   
  G4VSolid* magneticSolid
    = new G4Tubs("magneticTubs",0.,1.*m,1.*m,0.,360.*deg);

  fMagneticLogical
    = new G4LogicalVolume(magneticSolid, air, "magneticLogical");
 
  // placement of Tube 
 
  G4RotationMatrix* fieldRot = new G4RotationMatrix();
  fieldRot->rotateX(90.*deg);
  new G4PVPlacement(fieldRot,G4ThreeVector(),fMagneticLogical,
                    "magneticPhysical",worldLogical,
                    false,0,checkOverlaps);

  // set "user limits" for drawing smooth curve
  G4UserLimits* userLimits = new G4UserLimits(5.0*cm);
  fMagneticLogical->SetUserLimits(userLimits);

  // first arm
  G4VSolid* firstArmSolid = new G4Box("firstArmBox",1.5*m,1.*m,3.*m);
  G4LogicalVolume* firstArmLogical
    = new G4LogicalVolume(firstArmSolid,air,"firstArmLogical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,-5.*m),firstArmLogical,
                    "firstArmPhysical",worldLogical,
                    false,0,checkOverlaps);

  // second arm
  G4VSolid* secondArmSolid = new G4Box("secondArmBox",2.*m,2.*m,3.5*m);
  G4LogicalVolume* secondArmLogical
    = new G4LogicalVolume(secondArmSolid,air,"secondArmLogical",0,0,0);
  G4double x = -5.*m * std::sin(fArmAngle);
  G4double z = 5.*m * std::cos(fArmAngle);
  fSecondArmPhys
    = new G4PVPlacement(fArmRotation,G4ThreeVector(x,0.,z),secondArmLogical,
                        "fSecondArmPhys",worldLogical,
                        false,0,checkOverlaps);

  // hodoscopes in first arm
  G4VSolid* hodoscope1Solid = new G4Box("hodoscope1Box",5.*cm,20.*cm,0.5*cm);
  G4LogicalVolume* hodoscope1Logical
    = new G4LogicalVolume(hodoscope1Solid,scintillator,"hodoscope1Logical",0,0,0);
  for (G4int i=0;i<15;i++)
  {
    G4double x1 = (i-7)*10.*cm;
    new G4PVPlacement(0,G4ThreeVector(x1,0.,-1.5*m),hodoscope1Logical,
                      "hodoscope1Physical",firstArmLogical,
                      false,i,checkOverlaps);
  }

  // drift chambers in first arm
  G4VSolid* chamber1Solid = new G4Box("chamber1Box",1.*m,30.*cm,1.*cm);
  G4LogicalVolume* chamber1Logical
    = new G4LogicalVolume(chamber1Solid,argonGas,"chamber1Logical",0,0,0);
  for (G4int i=0;i<5;i++)
  {
    G4double z1 = (i-2)*0.5*m;
    new G4PVPlacement(0,G4ThreeVector(0.,0.,z1),chamber1Logical,
                      "chamber1Physical",firstArmLogical,
                      false,i,checkOverlaps);
  }

  // "virtual" wire plane
  G4VSolid* wirePlane1Solid = new G4Box("wirePlane1Box",1.*m,30.*cm,0.1*mm);
  G4LogicalVolume* wirePlane1Logical
    = new G4LogicalVolume(wirePlane1Solid,argonGas,"wirePlane1Logical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),wirePlane1Logical,
                    "wirePlane1Physical",chamber1Logical,
                    false,0,checkOverlaps);

  // hodoscopes in second arm
  G4VSolid* hodoscope2Solid = new G4Box("hodoscope2Box",5.*cm,20.*cm,0.5*cm);
  G4LogicalVolume* hodoscope2Logical
    = new G4LogicalVolume(hodoscope2Solid,scintillator,"hodoscope2Logical",0,0,0);
  for (G4int i=0;i<25;i++)
  {
    G4double x2 = (i-12)*10.*cm;
    new G4PVPlacement(0,G4ThreeVector(x2,0.,0.),hodoscope2Logical,
                      "hodoscope2Physical",secondArmLogical,
                      false,0,checkOverlaps);
  }

  // drift chambers in second arm
  G4VSolid* chamber2Solid = new G4Box("chamber2Box",1.5*m,30.*cm,1.*cm);
  G4LogicalVolume* chamber2Logical
    = new G4LogicalVolume(chamber2Solid,argonGas,"chamber2Logical",0,0,0);
  for (G4int i=0;i<5;i++)
  {
    G4double z2 = (i-2)*0.5*m - 1.5*m;
    new G4PVPlacement(0,G4ThreeVector(0.,0.,z2),chamber2Logical,
                      "chamber2Physical",secondArmLogical,
                      false,i,checkOverlaps);
  }

  // "virtual" wire plane
  G4VSolid* wirePlane2Solid = new G4Box("wirePlane2Box",1.5*m,30.*cm,0.1*mm);
  G4LogicalVolume* wirePlane2Logical
    = new G4LogicalVolume(wirePlane2Solid,argonGas,"wirePlane2Logical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),wirePlane2Logical,
                    "wirePlane2Physical",chamber2Logical,
                    false,0,checkOverlaps);

  // CsI calorimeter
  G4VSolid* EMcalorimeterSolid = new G4Box("EMcalorimeterBox",1.5*m,30.*cm,15.*cm);
  G4LogicalVolume* EMcalorimeterLogical
    = new G4LogicalVolume(EMcalorimeterSolid,csI,"EMcalorimeterLogical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,2.*m),EMcalorimeterLogical,
                    "EMcalorimeterPhysical",secondArmLogical,
                    false,0,checkOverlaps);

/*
  // EMcalorimeter cells
  G4VSolid* cellSolid = new G4Box("cellBox",7.5*cm,7.5*cm,15.*cm);
  G4LogicalVolume* cellLogical
    = new G4LogicalVolume(cellSolid,fCsI,"cellLogical",0,0,0);
  G4VPVParameterisation* cellParam = new A01CellParameterisation();
  new G4PVParameterised("cellPhysical",cellLogical,EMcalorimeterLogical,
                         kXAxis,80,cellParam);
*/
  // EMcalorimeter calorimeter column
  G4VSolid* EmCalColumnSolid
    = new G4Box("EmCalColumnBox",7.5*cm,30.*cm,15.*cm);
  G4LogicalVolume* EmCalColumnLogical
    = new G4LogicalVolume(EmCalColumnSolid, csI,"EmCalColumnLogical",0,0,0);
  new G4PVReplica("EmCalColumnPhysical",EmCalColumnLogical,
                   EMcalorimeterLogical,kXAxis,20,15.*cm);

  // EM calorimeter cell
  G4VSolid* cellSolid = new G4Box("cellBox",7.5*cm,7.5*cm,15.*cm);
  G4LogicalVolume* cellLogical
    = new G4LogicalVolume(cellSolid,csI,"cellLogical",0,0,0);
  new G4PVReplica("cellPhysical", cellLogical,
                   EmCalColumnLogical,kYAxis,4,15.*cm);

  // hadron calorimeter
  G4VSolid* HadCalorimeterSolid
    = new G4Box("HadCalorimeterBox",1.5*m,30.*cm,50.*cm);
  G4LogicalVolume* HadCalorimeterLogical
    = new G4LogicalVolume(HadCalorimeterSolid,lead,"HadCalorimeterLogical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,3.*m),HadCalorimeterLogical,
                    "HadCalorimeterPhysical",secondArmLogical,
                    false,0,checkOverlaps);

  // hadron calorimeter column
  G4VSolid* HadCalColumnSolid
    = new G4Box("HadCalColumnBox",15.*cm,30.*cm,50.*cm);
  G4LogicalVolume* HadCalColumnLogical
    = new G4LogicalVolume(HadCalColumnSolid,lead,"HadCalColumnLogical",0,0,0);
  new G4PVReplica("HadCalColumnPhysical",HadCalColumnLogical,
                   HadCalorimeterLogical,kXAxis,10,30.*cm);

  // hadron calorimeter cell
  G4VSolid* HadCalCellSolid
    = new G4Box("HadCalCellBox",15.*cm,15.*cm,50.*cm);
  G4LogicalVolume* HadCalCellLogical
    = new G4LogicalVolume(HadCalCellSolid,lead,"HadCalCellLogical",0,0,0);
  new G4PVReplica("HadCalCellPhysical",HadCalCellLogical,
                   HadCalColumnLogical,kYAxis,2,30.*cm);

  // hadron calorimeter layers
  G4VSolid* HadCalLayerSolid
    = new G4Box("HadCalLayerBox",15.*cm,15.*cm,2.5*cm);
  G4LogicalVolume* HadCalLayerLogical
    = new G4LogicalVolume(HadCalLayerSolid,lead,"HadCalLayerLogical",0,0,0);
  new G4PVReplica("HadCalLayerPhysical",HadCalLayerLogical,
                  HadCalCellLogical,kZAxis,20,5.*cm);

  // scintillator plates
  G4VSolid* HadCalScintiSolid
    = new G4Box("HadCalScintiBox",15.*cm,15.*cm,0.5*cm);
  G4LogicalVolume* HadCalScintiLogical
    = new G4LogicalVolume(HadCalScintiSolid,scintillator,"HadCalScintiLogical",0,0,0);
  new G4PVPlacement(0,G4ThreeVector(0.,0.,2.*cm),HadCalScintiLogical,
                    "HadCalScintiPhysical",HadCalLayerLogical,
                    false,0,checkOverlaps);

  // visualization attributes ------------------------------------------------

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  worldLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray
  fMagneticLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  firstArmLogical->SetVisAttributes(visAttributes);
  secondArmLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
  hodoscope1Logical->SetVisAttributes(visAttributes);
  hodoscope2Logical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  chamber1Logical->SetVisAttributes(visAttributes);
  chamber2Logical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0,0.8888,0.0));
  visAttributes->SetVisibility(false);
  wirePlane1Logical->SetVisAttributes(visAttributes);
  wirePlane2Logical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0.0));
  visAttributes->SetVisibility(false);
  EMcalorimeterLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.0));
  cellLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
  HadCalorimeterLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
  visAttributes->SetVisibility(false);
  HadCalColumnLogical->SetVisAttributes(visAttributes);
  HadCalCellLogical->SetVisAttributes(visAttributes);
  HadCalLayerLogical->SetVisAttributes(visAttributes);
  HadCalScintiLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------

/*
  // Import Geant4 geometry to VGM
  //
  Geant4GM::Factory g4Factory;
  g4Factory.SetDebug(1);
  g4Factory.Import(worldPhysical);
  
  // Export VGM geometry to Root
  //
  RootGM::Factory rtFactory;
  rtFactory.SetDebug(1);
  g4Factory.Export(&rtFactory);
  gGeoManager->CloseGeometry();
  gGeoManager->Export("A01.root");

  //
  // End of Export geometry in Root
  //
*/
  return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void A01DetectorConstruction::ConstructSDandField()
{ 
  // Added for VMC (construct SDs)
  TG4GeometryManager::Instance()->ConstructSDandField();

  // magnetic field ----------------------------------------------------------
  if ( fUseLocalMagField ) {
    fMagneticField = new B5MagneticField();
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fMagneticField);
    fFieldMgr->CreateChordFinder(fMagneticField);
    G4bool forceToAllDaughters = true;
    fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

    // Register the field and its manager for deleting
    G4AutoDelete::Register(fMagneticField);
    G4AutoDelete::Register(fFieldMgr);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void A01DetectorConstruction::ConstructMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    // Air
    nistManager->FindOrBuildMaterial("G4_AIR");

    // Argon gas
    nistManager->FindOrBuildMaterial("G4_Ar");
    // With a density different from the one defined in NIST
    // G4double density = 1.782e-03*g/cm3;
    // nistManager->BuildMaterialWithNewDensity("B5_Ar","G4_Ar",density);
    // !! cases segmentation fault

    // Scintillator
    // (PolyVinylToluene, C_9H_10)
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    // CsI
    nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    // Lead
    nistManager->FindOrBuildMaterial("G4_Pb");

    // Vacuum "Galactic"
    // nistManager->FindOrBuildMaterial("G4_Galactic");

    // Vacuum "Air with low density"
    // G4Material* air = G4Material::GetMaterial("G4_AIR");
    // G4double density = 1.0e-5*air->GetDensity();
    // nistManager
    //   ->BuildMaterialWithNewDensity("Air_lowDensity", "G4_AIR", density);

    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void A01DetectorConstruction::SetArmAngle(G4double val)
{
  if(!fSecondArmPhys)
  {
    G4cerr << "Detector has not yet been constructed." << G4endl;
    return;
  }

  fArmAngle = val;
  *fArmRotation = G4RotationMatrix();  // make it unit vector
  fArmRotation->rotateY(fArmAngle);
  G4double x = -5.*m * std::sin(fArmAngle);
  G4double z = 5.*m * std::cos(fArmAngle);
  fSecondArmPhys->SetTranslation(G4ThreeVector(x,0.,z));

  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void A01DetectorConstruction::DefineCommands()
{
  // Define /B5/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/A01/detector/",
                                      "Detector control");

  // armAngle command
  G4GenericMessenger::Command& armAngleCmd
    = fMessenger->DeclareMethodWithUnit("armAngle","deg",
                                &A01DetectorConstruction::SetArmAngle,
                                "Set rotation angle of the second arm.");
  armAngleCmd.SetParameterName("angle", true);
  armAngleCmd.SetRange("angle>=0. && angle<180.");
  armAngleCmd.SetDefaultValue("30.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
