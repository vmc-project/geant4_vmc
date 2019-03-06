//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TG4VGMMessenger.cxx
/// \brief Implementation of the TG4VGMMessenger class
///
/// \author I. Hrivnacova; IPN, Orsay

#ifdef USE_VGM

#include "TG4VGMMessenger.h"
#include "TG4GeometryServices.h"

#include <Geant4GM/volumes/Factory.h>
#include <RootGM/volumes/Factory.h>
#include <RootGM/volumes/Placement.h>
#include <XmlVGM/AGDDExporter.h>
#include <XmlVGM/GDMLExporter.h>

#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithABool.hh>

#include <TGeoManager.h>

G4UIdirectory*           TG4VGMMessenger::fgDirectory = 0;
G4UIcmdWithoutParameter* TG4VGMMessenger::fgGenerateRootCmd = 0;
G4int                    TG4VGMMessenger::fgCounter = 0;

//_____________________________________________________________________________
TG4VGMMessenger::TG4VGMMessenger(const G4String& xmlFormat,
                                 const G4String& userGeometry)
  : G4UImessenger(),
    fGeometryInput(),
    fXmlFormat(xmlFormat),
    fImportFactory(0),
    fG4Factory(0),
    fRootFactory(0),
    fXmlVGMExporter(0),
    fGenerateXMLCmd()
{
/// Standard constructor

  if ( userGeometry == "geomVMCtoGeant4"  ||
       userGeometry == "geomRootToGeant4" ||
       userGeometry == "geomGeant4" ) {
    fGeometryInput = "geant4";
  }

  if ( userGeometry == "geomVMCtoRoot"    ||
       userGeometry == "geomRoot"      ) {
    fGeometryInput = "root";
  }

  if (!fgDirectory) {
    fgDirectory = new G4UIdirectory("/vgm/");
    fgDirectory->SetGuidance("XML geometry generator control commands.");

    G4String cmdName("/vgm/generateRoot");
    fgGenerateRootCmd = new G4UIcmdWithoutParameter(cmdName, this);
    fgGenerateRootCmd->SetGuidance("Export geometry in Root file");
    fgGenerateRootCmd->AvailableForStates(G4State_Idle);
  }

  G4String cmdName("/vgm/generate");
  cmdName = cmdName + xmlFormat;
  fGenerateXMLCmd = new G4UIcmdWithAString(cmdName, this);
  fGenerateXMLCmd->SetGuidance("Generate geometry XML file");
  fGenerateXMLCmd->SetGuidance("starting from a logical volume specified by name;");
  fGenerateXMLCmd->SetGuidance("if no name is given - the whole world is processed.");
  fGenerateXMLCmd->SetParameterName("lvName", true);
  fGenerateXMLCmd->SetDefaultValue("");
  fGenerateXMLCmd->AvailableForStates(G4State_Idle);

  cmdName = G4String("/vgm/set") + xmlFormat + G4String("NumWidth");
  fSetXMLNumWidthCmd = new G4UIcmdWithAnInteger(cmdName, this);
  fSetXMLNumWidthCmd->SetGuidance("Set number with for XML generation");
  fSetXMLNumWidthCmd->SetParameterName("xmlNumWidth", false);
  fSetXMLNumWidthCmd->AvailableForStates(G4State_Idle);

  cmdName = G4String("/vgm/set") + xmlFormat + G4String("NumPrecision");
  fSetXMLNumPrecisionCmd = new G4UIcmdWithAnInteger(cmdName, this);
  fSetXMLNumPrecisionCmd->SetGuidance("Set number precision for XML generation");
  fSetXMLNumPrecisionCmd->SetParameterName("xmlNumPrecision", false);
  fSetXMLNumPrecisionCmd->AvailableForStates(G4State_Idle);

  fSetAssembliesInNamesCmd = new G4UIcmdWithABool("/vgm/setAssembliesInNames", this);
  fSetAssembliesInNamesCmd->SetGuidance("Activate/inactivate including the names of Root assemblies");
  fSetAssembliesInNamesCmd->SetGuidance("in volume names when exporting Root geometry.");
  fSetAssembliesInNamesCmd->SetParameterName("assembliesInNames", false);
  fSetAssembliesInNamesCmd->AvailableForStates(G4State_PreInit);

  fSetNameSeparatorCmd = new G4UIcmdWithAString("/vgm/setNameSeparator", this);
  fSetNameSeparatorCmd->SetGuidance("Set the name separator used when ");
  fSetNameSeparatorCmd->SetGuidance("including the names of Root assemblies in volume names");
  fSetNameSeparatorCmd->SetGuidance("when exporting Root geometry is activated .");
  fSetNameSeparatorCmd->SetParameterName("nameSeparatoe", false);
  fSetNameSeparatorCmd->AvailableForStates(G4State_PreInit);

  fgCounter++;
}


//_____________________________________________________________________________
TG4VGMMessenger::~TG4VGMMessenger()
{
/// Destructor

  delete fG4Factory;
  delete fRootFactory;
  delete fXmlVGMExporter;

  fgCounter--;
  if (fgCounter==0) {
    delete fgDirectory;
    delete fgGenerateRootCmd;
    fgDirectory = 0;
    fgGenerateRootCmd = 0;
  }
  delete fGenerateXMLCmd;
  delete fSetXMLNumWidthCmd;
  delete fSetXMLNumPrecisionCmd;
  delete fSetAssembliesInNamesCmd;
  delete fSetNameSeparatorCmd;
}

// private methods

//_____________________________________________________________________________
void TG4VGMMessenger::CreateVGMExporter()
{
/// Create VGM exporter if it does not yet exist
// ---

  if ( ! fXmlVGMExporter ) {
    // Create VGM exporter if not yet done
    if ( fXmlFormat == "AGDD" ) {
      fXmlVGMExporter = new XmlVGM::AGDDExporter(fImportFactory);
    }
    if ( fXmlFormat == "GDML" ) {
      fXmlVGMExporter = new XmlVGM::GDMLExporter(fImportFactory);
    }
  }
}

// public methods

//_____________________________________________________________________________
void TG4VGMMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
/// Applies command to the associated object.
// ---

  if ( command == fSetAssembliesInNamesCmd ) {
      RootGM::Placement::SetIncludeAssembliesInNames(
        fSetAssembliesInNamesCmd->GetNewBoolValue(newValues));
      return;
  }

  if ( command == fSetNameSeparatorCmd ) {
      RootGM::Placement::SetNameSeparator(
        newValues.at(0));
      return;
  }

  if ( fGeometryInput == "geant4" && (! fG4Factory) ) {
    // Import Geant4 geometry in VGM
    fG4Factory = new Geant4GM::Factory();
    fImportFactory = fG4Factory;
    fImportFactory->SetIgnore(true);
    // fG4Factory->SetDebug(1);
    fG4Factory->Import(TG4GeometryServices::Instance()->GetWorld());
  }

  if ( fGeometryInput == "root" && (! fRootFactory) ) {
    // Import Root geometry in VGM
    fRootFactory = new RootGM::Factory();
    fImportFactory = fRootFactory;
    fImportFactory->SetIgnore(true);
    // fRootFactory->SetDebug(1);
    fRootFactory->Import(gGeoManager->GetTopNode());
  }

  if ( command == fgGenerateRootCmd ) {
    if (!fRootFactory) {
      fRootFactory = new RootGM::Factory();
      fG4Factory->Export(fRootFactory);
      gGeoManager->CloseGeometry();
    }
    gGeoManager->Export("geometry.root");
  }

  if ( command == fGenerateXMLCmd ) {

    CreateVGMExporter();

    if (newValues == "")
      fXmlVGMExporter->GenerateXMLGeometry();
    else
      fXmlVGMExporter->GenerateXMLGeometry(newValues);
  }

  if (command == fSetXMLNumWidthCmd) {
    CreateVGMExporter();
    fXmlVGMExporter->SetNumWidth(fSetXMLNumWidthCmd->GetNewIntValue(newValues));
  }

  if (command == fSetXMLNumPrecisionCmd) {
    CreateVGMExporter();
    fXmlVGMExporter->SetNumPrecision(fSetXMLNumPrecisionCmd->GetNewIntValue(newValues));
  }

}

#endif //USE_VGM
