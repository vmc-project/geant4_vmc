// $Id: TG4MaterialConvertor.cxx,v 1.1 2003/07/22 06:46:58 brun Exp $
//
// Author: I. Hrivnacova, 8.1.2003 
//
// Class TG4MaterialConvertor
// ------------------------------
// See the class description in the header file.

#include <G3EleTable.hh>
#include <G4Element.hh>
#include <G4Material.hh>

#include <TGeoMaterial.h>
#include <TError.h>

#include "TG4MaterialConvertor.h"
#include "TG4TGeoUnits.h"

const G4double TG4MaterialConvertor::fgkG3MinimumDensity = 1.e-10*g/cm3;

//_____________________________________________________________________________
TG4MaterialConvertor::TG4MaterialConvertor() {
//
}

//_____________________________________________________________________________
TG4MaterialConvertor::TG4MaterialConvertor(
                                       const TG4MaterialConvertor& right) 
{
// 
  G4Exception("G4RootGeometryConvertor: Copy constructor not implemented");
}

//_____________________________________________________________________________
TG4MaterialConvertor::~TG4MaterialConvertor() {
//
}

//
// private methods
//

//_____________________________________________________________________________
G4Material* TG4MaterialConvertor::CreateMaterial(const TGeoMaterial* material)
{
// Creates the G4 material corresponding to the specified
// Root material.
// According to g3tog4 G4gsmate.cc.
// ---

  // Get parameters
  G4String name(material->GetName());
  G4double a = material->GetA();
  G4double z = material->GetZ();
  G4double density = material->GetDensity();
  
  // G4cout << "##########: " << name << " " << a << "  " << z << G4endl;
  
  // Convert units
  a       *= TG4TGeoUnits::AtomicWeight();
  density *= TG4TGeoUnits::MassDensity();
  
  // Create G4 material
  G4Material* g4Material; 
  if ( z<1 || density < fgkG3MinimumDensity ) {
    // Define vacuum according to definition from N03 example
    G4double density     = universe_mean_density;    //from PhysicalConstants.h
    G4double pressure    = 3.e-18*pascal;
    G4double temperature = 2.73*kelvin;
    g4Material = new G4Material(name, z=1., a=1.01*g/mole, density,
                                kStateGas,temperature,pressure);
  }
  else {
    //G4Element* element = CreateElement(z, a, name);
    G4Element* element = G3Ele.GetEle(z);
    g4Material = new G4Material(name, density, 1);
    g4Material->AddElement(element, 1.);    
  }  
    
  // Fill the map
  fMaterialsMap[material] = g4Material;

  return g4Material;
}     

//_____________________________________________________________________________
G4Material* TG4MaterialConvertor::CreateMixture(const TGeoMixture* mixture)
{
// Creates the G4 material corresponding to the specified
// Root matrial.
// According to g3tog4 G4gsmixt.cc.
// ---

  // Get parameters
  G4String name(mixture->GetName());
  G4double density = mixture->GetDensity();
  G4int  nele = mixture->GetNelements();
  G4double* a = mixture->GetAmixt();
  G4double* z = mixture->GetZmixt();
  G4double* w = mixture->GetWmixt();
  
  // G4cout << "##########: " << name << " " << a[0] << "  " << z[0] << G4endl;
  
  // Create G4 material with given number of components
  // (elements)
  G4Material* g4Material = new G4Material(name, density*g/cm3, nele);
    
  for (G4int i=0; i<nele; i++) {

    // Convert units
    density *= TG4TGeoUnits::MassDensity();
    a[i]    *= TG4TGeoUnits::AtomicWeight();
  
    // Create materrial
    // G4Element* element = G4Element(z[i], a[i]*g/mole, name);
    G4Element* element = G3Ele.GetEle(z[i]);
    g4Material->AddElement(element, w[i]);    
  }
  
  // Fill the map
  fMaterialsMap[mixture] = g4Material;

  return g4Material;
}     

//
// public methods
//

//_____________________________________________________________________________
void TG4MaterialConvertor::Convert(const TList* materials)
{
// Converts all materials in the specified list of Root materials
// and fills the materials map.
// ---

  TIter next(materials);
  while (TObject *obj = next()) {
    TGeoMixture* mixture = dynamic_cast<TGeoMixture*>(obj);    
    if (mixture) 
      CreateMixture(mixture);
    else {
      TGeoMaterial* material = dynamic_cast<TGeoMaterial*>(obj);
      if (material) 
        CreateMaterial(material);
      else 
        Fatal("Convert", "Wrong material type.");
    }  
  }
  
  // G4cout << "All materials converted" << G4endl;   
}

//_____________________________________________________________________________
G4Material* TG4MaterialConvertor::GetMaterial(const TGeoMaterial* material) const
{
// Returns G4 material corresponding to the specified Root material.
// ---

  MaterialsMapIterator it = fMaterialsMap.find(material);
  if (it != fMaterialsMap.end()) 
    return (*it).second;
  else
    return 0;   
}   
