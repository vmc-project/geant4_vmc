// $Id: TG4AGDDConvertor.cxx,v 1.3 2003/12/18 13:25:11 brun Exp $
//
// Author: I. Hrivnacova, 27.07.2000 
//
// Class TG4AGDDConvertor 
// ----------------------
// See the class description in the header file.

#include <iostream>
#include <iomanip>
#include <math.h>
#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

#include <G4PVReplica.hh>
#include <G4Material.hh>
#include <G4VSolid.hh>
#include <G4ReflectedSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Trd.hh>
#include <G4Trap.hh>
#include <G4Para.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>

#include "TG4AGDDConvertor.h"
#include "TG4Polycone.h"
#include "TG4Polyhedra.h"
#include "TG4XMLUnits.h"

const G4int TG4AGDDConvertor::fgkMaxVolumeNameLength   = 20;
const G4int TG4AGDDConvertor::fgkMaxMaterialNameLength = 20;
const G4int TG4AGDDConvertor::fgkDefaultNumWidth = 7;
const G4int TG4AGDDConvertor::fgkDefaultNumPrecision = 4;

//_____________________________________________________________________________
TG4AGDDConvertor::TG4AGDDConvertor(std::ofstream& outFile,
                                 const G4String& version,
		                 const G4String& date,
                                 const G4String& author,
				 const G4String dtdVersion) 
  : fOutFile(outFile),
    fVersion(version),
    fDate(date),
    fAuthor(author),
    fDtdVersion(dtdVersion),
    fkBasicIndention("   "),
    fIndention(fkBasicIndention),
    fNW(fgkDefaultNumWidth),
    fNP(fgkDefaultNumPrecision),
    fRotationCounter(0)
{
  fOutFile.width(fgkDefaultNumWidth);
  fOutFile.precision(fgkDefaultNumPrecision);
}

//_____________________________________________________________________________
TG4AGDDConvertor::~TG4AGDDConvertor() {
//
}

//
// private methods
//

//_____________________________________________________________________________
void TG4AGDDConvertor::Append(G4String& s, G4int a) const
{
// Appends number to string.
// ---

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else  
  std::strstream tmpStream;
#endif

  tmpStream << a;  

  s.append(tmpStream.str());
}

//_____________________________________________________________________________
void TG4AGDDConvertor::CutName(G4String& name) const
{
// Removes spaces after the name if present.
// ---

  G4int i = name.length();
  while (name(--i) == ' ') name = name(0,i);
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::CutName(G4String& name, G4int size) const
{
// Cuts name to given size.
// ---

  if (G4int(name.length()) > size) name = name(0, size);
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::PutName(G4String& element, G4String name, 
                              G4String templ) const
{
// Replaces given template in string element with a give name.
// ---

  CutName(name);
  // make better
  if (templ == "#") 
    CutName(name, fgkMaxVolumeNameLength);
  else if (templ == "!")  
    CutName(name, fgkMaxMaterialNameLength);
  
  element.replace(element.find(templ), name.size(), name);
  element.replace(element.find(templ), 1, "\"");
  while (element.contains(templ)) element.replace(element.find(templ), 1 , " ");
}    
  
//_____________________________________________________________________________
std::ostream& TG4AGDDConvertor::SmartPut(std::ostream& out, 
                                       G4int size, G4int precision,
                                       G4double number, 
				       const G4String& separator) const
{
// Help function to supress - sign in case the number == 0
// within the given precision
// ---

  if ( round(number*pow(10.,precision))/pow(10.,precision) == 0.0) {
    number = 0.;
  }  
  
  out << std::setw(size) << std::setprecision(precision) 
      << number << separator;
  
  return out;
}

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteBox(G4String lvName, const G4Box* box, 
                               G4String materialName, G4bool)
{
// Writes G4box solid.
// ---

  // get parameters
  G4double x = box->GetXHalfLength()/TG4XMLUnits::Length()*2.;
  G4double y = box->GetYHalfLength()/TG4XMLUnits::Length()*2.;
  G4double z = box->GetZHalfLength()/TG4XMLUnits::Length()*2.;

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<box    name=\"" + lvName + quota;
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "X_Y_Z=\"";
  G4String element4 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write element
  fOutFile << fkBasicIndention << element1 << G4endl  
           << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW) << std::setprecision(fNP) << x << "; "
           << std::setw(fNW) << std::setprecision(fNP) << y << "; "
           << std::setw(fNW) << std::setprecision(fNP) << z 
	   << element4 << G4endl << G4endl;
}
 
//_____________________________________________________________________________
void TG4AGDDConvertor::WriteTubs(G4String lvName, const G4Tubs* tubs, 
                                G4String materialName, G4bool)
{
// Writes G4tubs solid.
// ---

  // get parameters
  G4double rmin = tubs->GetInnerRadius()/TG4XMLUnits::Length();
  G4double rmax = tubs->GetOuterRadius()/TG4XMLUnits::Length();
  G4double hz   = tubs->GetZHalfLength()/TG4XMLUnits::Length()*2.;
  G4double sphi = tubs->GetStartPhiAngle()/TG4XMLUnits::Angle();
  G4double dphi = tubs->GetDeltaPhiAngle()/TG4XMLUnits::Angle();

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<tubs   name=\"" + lvName + quota; 
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "profile=\"";
  G4String element4 = "Rio_Z  =\"";
  G4String element5 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write element
  fOutFile << fkBasicIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW)   << std::setprecision(fNP) << sphi << "; "
           << std::setw(fNW)   << std::setprecision(fNP) << dphi
	   << quota << G4endl 	   	   
	   << indention        << element4
           << std::setw(fNW) << std::setprecision(fNP) << rmin << "; "
           << std::setw(fNW) << std::setprecision(fNP) << rmax << "; "
           << std::setw(fNW) << std::setprecision(fNP) << hz 
	   << element5 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteCons(G4String lvName, const G4Cons* cons, 
                                G4String materialName, G4bool isReflected)
{
// Writes G4cons solid.
// ---

  // get parameters
  G4double rmin1 = cons->GetInnerRadiusMinusZ()/TG4XMLUnits::Length();
  G4double rmax1 = cons->GetOuterRadiusMinusZ()/TG4XMLUnits::Length();
  G4double rmin2 = cons->GetInnerRadiusPlusZ()/TG4XMLUnits::Length();
  G4double rmax2 = cons->GetOuterRadiusPlusZ()/TG4XMLUnits::Length();
  G4double hz   = cons->GetZHalfLength()/TG4XMLUnits::Length()*2.;
  G4double sphi = cons->GetStartPhiAngle()/TG4XMLUnits::Angle();
  G4double dphi = cons->GetDeltaPhiAngle()/TG4XMLUnits::Angle();

  // take into account reflection
  if (isReflected) {
    G4double tmp1 = rmin1;
    G4double tmp2 = rmax1;
    rmin1 = rmin2;
    rmax1 = rmax2;
    rmin2 = tmp1;
    rmax2 = tmp2;
  }  

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<cons   name=\"" + lvName + quota; 
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "profile=\"";
  G4String element4 = "Rio1_Rio2_Z  =\"";
  G4String element5 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write element
  fOutFile << fkBasicIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW)   << std::setprecision(fNP) << sphi << "; "
           << std::setw(fNW)   << std::setprecision(fNP) << dphi
	   << quota << G4endl 	   	   
	   << indention        << element4
           << std::setw(fNW) << std::setprecision(fNP) << rmin1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << rmin2 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << rmax1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << rmax2 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << hz 
	   << element5 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteTrd(G4String lvName, const G4Trd* trd, 
                               G4String materialName, G4bool isReflected)
{
// Writes G4Trd solid.
// ---

  // get parameters
  G4double x1 = trd->GetXHalfLength1()/TG4XMLUnits::Length()*2;
  G4double x2 = trd->GetXHalfLength2()/TG4XMLUnits::Length()*2;
  G4double y1 = trd->GetYHalfLength1()/TG4XMLUnits::Length()*2;
  G4double y2 = trd->GetYHalfLength2()/TG4XMLUnits::Length()*2;
  G4double hz = trd->GetZHalfLength()/TG4XMLUnits::Length()*2;

  // take into account reflection
  if (isReflected) {
    G4double tmpx = x1;
    G4double tmpy = y1;
    x1 = x2;
    y1 = y2;
    x2 = tmpx;
    y2 = tmpy;
  }  

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<trd    name=\"" + lvName + quota; 
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "Xmp_Ymp_Z=\"";
  G4String element4 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write element
  fOutFile << fkBasicIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW) << std::setprecision(fNP) << x1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << x2 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << y1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << y2 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << hz
	   << element4 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteTrap(G4String lvName, const G4Trap* trap, 
                                G4String materialName, G4bool isReflected)
{
// Writes G4Trap solid.
// ---

  // get parameters
  G4double dz = trap->GetZHalfLength()/TG4XMLUnits::Length()*2.;
  G4ThreeVector symAxis = trap->GetSymAxis();
  G4double y1 = trap->GetYHalfLength1()/TG4XMLUnits::Length()*2.;
  G4double x1 = trap->GetXHalfLength1()/TG4XMLUnits::Length()*2.;
  G4double x2 = trap->GetXHalfLength2()/TG4XMLUnits::Length()*2.;
  G4double tanAlpha1 = trap->GetTanAlpha1();
  G4double y2 = trap->GetYHalfLength2()/TG4XMLUnits::Length()*2.;
  G4double x3 = trap->GetXHalfLength3()/TG4XMLUnits::Length()*2.;
  G4double x4 = trap->GetXHalfLength4()/TG4XMLUnits::Length()*2.;
  G4double tanAlpha2 = trap->GetTanAlpha2();

  // ordering of parameters in XML element
  // Xmumdpupd_Ymp_Z: 2x2 2x1 2x4 2x3 2y2 2y1 2dz
  // inclination: atan(symAxis.x/symAxis.z), atan(symAxis.y/symAxis.z)
  // declination: alpha1, alpha2

  // get angles
  G4double inc1 = atan(symAxis.x()/symAxis.z()) / TG4XMLUnits::Angle();
  G4double inc2 = atan(symAxis.y()/symAxis.z()) / TG4XMLUnits::Angle();
  G4double alpha1 = atan(tanAlpha1) / TG4XMLUnits::Angle();
  G4double alpha2 = atan(tanAlpha2) / TG4XMLUnits::Angle();

  // take into account reflection
  if (isReflected) {
    G4double tmp1 = y1;
    G4double tmp2 = x1;
    G4double tmp3 = x2;
    G4double tmp4 = alpha1;
    y1 = y2;
    x1 = x3;
    x2 = x4;
    alpha1 = alpha2;
    y2 = tmp1;
    x3 = tmp2;
    x4 = tmp3;
    alpha2 = tmp4;
    inc1 = -inc1;
    inc2 = -inc2;
  }    

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<trap   name=\"" + lvName + quota; 
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "Xmumdpupd_Ymp_Z=\"";
  G4String element4 = "inclination=\""; 
  G4String element5 = "declination=\""; 
  G4String element6 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;

  // write element
  fOutFile << fkBasicIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW) << std::setprecision(fNP) << x2 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << x1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << x4 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << x3 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << y2 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << y1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << dz 
	   << quota << G4endl
           << indention       << element4
           << std::setw(fNW) << std::setprecision(fNP) << inc1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << inc2 
	   << quota << G4endl
	   << indention       << element5
           << std::setw(fNW) << std::setprecision(fNP) << alpha1 << "; "
           << std::setw(fNW) << std::setprecision(fNP) << alpha2 
	   << element6 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WritePara(G4String lvName, const G4Para* para, 
                                G4String materialName, G4bool isReflected)
{
// Writes G4Para solid.
// ---

  // get parameters
  G4double dx = para->GetXHalfLength()/TG4XMLUnits::Length()*2.;
  G4double dy = para->GetYHalfLength()/TG4XMLUnits::Length()*2.;
  G4double dz = para->GetZHalfLength()/TG4XMLUnits::Length()*2.;
  G4double tanAlpha     = para->GetTanAlpha();
  G4ThreeVector symAxis = para->GetSymAxis();
  
  G4double alpha = atan(tanAlpha) / TG4XMLUnits::Angle();
  G4double theta = acos(symAxis.z()) / TG4XMLUnits::Angle();
  G4double phi;
  if (theta == 0.)
    phi = 0;
  else        
    phi = atan(symAxis.y()/symAxis.x()) / TG4XMLUnits::Angle();

  // take into account reflection
  if (isReflected) {
   theta = 180. - theta; 
  }    

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<para   name=\"" + lvName + quota;
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "X_Y_Z=\"";
  G4String element4 = "alpha=\"";
  G4String element5 = "theta=\"";
  G4String element6 = "phi=  \"";
  G4String element7 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write element
  fOutFile << fkBasicIndention << element1 << G4endl  
           << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW) << std::setprecision(fNP) << dx << "; "
           << std::setw(fNW) << std::setprecision(fNP) << dy << "; "
           << std::setw(fNW) << std::setprecision(fNP) << dz 
	   << quota << G4endl
	   << indention        << element4
           << std::setw(fNW) << std::setprecision(fNP) << alpha 
	   << quota << G4endl
	   << indention        << element5
           << std::setw(fNW) << std::setprecision(fNP) << theta
	   << quota << G4endl
	   << indention        << element6
           << std::setw(fNW) << std::setprecision(fNP) << phi 	   
	   << element7 << G4endl << G4endl;
}
 
//_____________________________________________________________________________
void TG4AGDDConvertor::WritePolycone(G4String lvName, const G4Polycone* polycone, 
                                    G4String materialName, G4bool isReflected)
{
// Writes G4Polycone solid.
// ---

  // get profile parameters
  G4double sphi = polycone->GetStartPhi()/TG4XMLUnits::Angle();
  G4double ephi = polycone->GetEndPhi()/TG4XMLUnits::Angle();
  
  // get polycone Z planes parameters
  TG4Polycone historicalPolycone = TG4Polycone(*polycone);

  G4int nofZPlanes = historicalPolycone.GetNofZPlanes();
  G4double* rminArray = historicalPolycone.GetRmin();
  G4double* rmaxArray = historicalPolycone.GetRmax();
  G4double* zArray    = historicalPolycone.GetZ();

  // take into account reflection
  if (isReflected) {
    for (G4int i=0; i<nofZPlanes; i++) {
      zArray[i] = -zArray[i];
    }
  }    

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<pcon   name=\"" + lvName + quota; 
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "profile=\"";
  G4String element4 = "\" >";
  G4String element5 = "<polyplane Rio_Z=\"";
  G4String element6 = "\" />";
  G4String element7 = "</pcon>";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write pcon element
  fOutFile << fkBasicIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW) << std::setprecision(fNP) << sphi << "; "
           << std::setw(fNW) << std::setprecision(fNP) << ephi-sphi
	   << element4 << G4endl;

  // write polyplane elements
  for (G4int i=0; i<nofZPlanes; i++) {
  
    // set units
    G4double rmin = rminArray[i]/TG4XMLUnits::Length();
    G4double rmax = rmaxArray[i]/TG4XMLUnits::Length();
    G4double z    = zArray[i]/TG4XMLUnits::Length();

    fOutFile << indention << element5
             << std::setw(fNW) << std::setprecision(fNP) << rmin << "; "
             << std::setw(fNW) << std::setprecision(fNP) << rmax << "; " 
             << std::setw(fNW) << std::setprecision(fNP) << z 
	     << element6
	     << G4endl;
  }
  
  // close pcon element
  fOutFile << fkBasicIndention
           << element7 << G4endl << G4endl;  	     
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WritePolyhedra(G4String lvName, const G4Polyhedra* polyhedra, 
                                    G4String materialName, G4bool isReflected)
{
// Writes G4Polycone solid.
// ---

  // get parameters
  G4int nofSides = polyhedra->GetNumSide();
  G4double sphi = polyhedra->GetStartPhi()/TG4XMLUnits::Angle();
  G4double ephi = polyhedra->GetEndPhi()/TG4XMLUnits::Angle();
  
  // get polyhedra Z planes parameters
  TG4Polyhedra historicalPolyhedra = TG4Polyhedra(*polyhedra);

  G4int nofZPlanes = historicalPolyhedra.GetNofZPlanes();
  G4double* rminArray = historicalPolyhedra.GetRmin();
  G4double* rmaxArray = historicalPolyhedra.GetRmax();
  G4double* zArray    = historicalPolyhedra.GetZ();

  // take into account reflection
  if (isReflected) {
    for (G4int i=0; i<nofZPlanes; i++) {
      zArray[i] = -zArray[i];
    }
  }    

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<phedra name=\"" + lvName + quota; 
  G4String element2 = "material=\"" + materialName + quota;
  G4String element3 = "profile=\"";
  G4String element4 = "sides =\"";
  G4String element5 = "Ris=\"";
  G4String element6 = "Ros=\"";
  G4String element7 = "Zs =\"";
  G4String element8 = "\" />";
  G4String indention = fkBasicIndention + fkBasicIndention;
  
  // write element
  fOutFile << fkBasicIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        << element3
           << std::setw(fNW) << std::setprecision(fNP) << sphi << "; "
           << std::setw(fNW) << std::setprecision(fNP) << ephi-sphi
	   << quota << G4endl
	   << indention       << element4 
	   << nofSides
	   << quota << G4endl;

  fOutFile << indention << element5;
  G4int i;
  for (i=0; i<nofZPlanes; i++) {  
    // set units    
    G4double rmin = rminArray[i]/TG4XMLUnits::Length();
    if (i>0) fOutFile << "; ";
    fOutFile << std::setw(fNW) << std::setprecision(fNP) << rmin;
  };
  fOutFile << quota << G4endl;

  fOutFile << indention << element6;
  for (i=0; i<nofZPlanes; i++) {  
    // set units
    G4double rmax = rmaxArray[i]/TG4XMLUnits::Length();
    if (i>0) fOutFile << "; ";
    fOutFile << std::setw(fNW) << std::setprecision(fNP) << rmax;
  };
  fOutFile << quota << G4endl;

  fOutFile << indention << element7;
  for (i=0; i<nofZPlanes; i++) {  
    // set units
    G4double z = zArray[i]/TG4XMLUnits::Length();
    if (i>0) fOutFile << "; ";
    fOutFile << std::setw(fNW) << std::setprecision(fNP) << z;
  };
  fOutFile << element8 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteNotSupportedSolid(G4String name, 
                                  G4String materialName)
{				   
// Write a comment line with awarning
// and then write a box element instead 
// ---

  // Compose comment
  G4String element1 = "<!-- !!! unsupported shape  !!!  name= \""; 
  G4String element2 = "\" -->";
  G4String element3 = "<!-- dummy box is written instead  -->"; 
  
  // Write element with warning
  fOutFile << fIndention << element1 << name << element2 << G4endl
	   << fIndention << element3 << G4endl;
	   
  // Write dummy box element
  G4Box box(name, 1., 1., 1.);
  WriteBox(name, &box, materialName, false); 
}  	   


//
// public methods
//

//_____________________________________________________________________________
void TG4AGDDConvertor::OpenSection(const G4String& topVolume)
{
// Writes section opening.
// ---
			 
  G4String element1 = "<section DTD_version = \"";
  G4String element2 = "         name        = \"";
  G4String element3 = "         version     = \"";
  G4String element4 = "         date        = \"";
  G4String element5 = "         author      = \"";
  G4String element6 = "         top_volume  = \"";
  G4String element7 = "  >";
  G4String quota = "\"";   
  
  // write element
  fOutFile << element1 << fDtdVersion << quota << G4endl
           << element2 << topVolume   << quota << G4endl
           << element3 << fVersion    << quota << G4endl
           << element4 << fDate       << quota << G4endl
           << element5 << fAuthor     << quota << G4endl
           << element6 << topVolume   << quota
           << element7 << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::OpenMaterials()
{
// Writes materials opening.
// ---
			 
  G4String element1 = "<materials  version = \"";
  G4String element2 = "            date    = \"";
  G4String element3 = "            author  = \"";
  G4String element4 = "            DTD_version=\"";
  G4String element5 = "  >";
  G4String quota = "\"";   
  
  // write element
  fOutFile << element1 << fVersion << quota << G4endl
           << element2 << fDate    << quota << G4endl
           << element3 << fAuthor  << quota << G4endl
           << element4 << fDtdVersion << quota
           << element5 << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::OpenComposition(const G4String& name,
                                       const G4String& /*materialName*/)
{
// Writes composition opening.
// ---
			 
  G4String element = "<composition name=\"";
  element.append(name);
  element.append("\">");

  // write element
  fOutFile << fIndention
           << element
	   << G4endl;

  // increase indention
  IncreaseIndention();	   
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::CloseSection(const G4String& /*topVolume*/)
{
// Writes section closing.
// ---

  // define element
  G4String element = "</section>";

  // write element
  fOutFile << element
	   << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::CloseMaterials()
{
// Writes materials closing.
// ---

  // define element
  G4String element = "</materials>";

  // write element
  fOutFile << element
	   << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::CloseComposition()
{
// Writes composition closing.
// ---

  // decrease indention
  DecreaseIndention();

  // define element
  G4String element = "</composition>";

  // write element
  fOutFile << fIndention
           << element
	   << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteMaterial(const G4Material* material) 
{
// Writes G4Material. 
// Not yet implemented, only XML comment element is written.
// ---

  G4String name = material->GetName();
  CutName(name);

  // only comment line
  G4String element1 = "<!-- material = \""; 
  G4String element2 = "\" -->";
  
  // write element
  fOutFile << fkBasicIndention
           << element1 << name
	   << element2
           << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteSolid(G4String lvName, const G4VSolid* solid, 
                                 G4String materialName) 
{
// Finds G4Solid concrete type and calls writing function. 
// For not yet implemented solids, only XML comment element is written.
// ---

  // to be removed when materials are supported
  // materialName = "Hydrogen";
  
  const G4ReflectedSolid* reflSolid
    = dynamic_cast<const G4ReflectedSolid*>(solid);

  const G4VSolid* consSolid;
  G4bool isReflected = false;
  if (reflSolid) { 
    consSolid = reflSolid->GetConstituentMovedSolid();
    isReflected = true;
  }  
  else  
    consSolid = solid;  
     
  const G4Box* box = dynamic_cast<const G4Box*>(consSolid);
  if (box) { 
    WriteBox(lvName, box, materialName, isReflected); 
    return;
  }
  
  const G4Tubs* tubs = dynamic_cast<const G4Tubs*>(consSolid);
  if (tubs) { 
    WriteTubs(lvName, tubs, materialName, isReflected); 
    return;
  }
  
  const G4Cons* cons = dynamic_cast<const G4Cons*>(consSolid);
  if (cons) { 
    WriteCons(lvName, cons, materialName, isReflected); 
    return;
  }
  
  const G4Trd* trd = dynamic_cast<const G4Trd*>(consSolid);
  if (trd) { 
    WriteTrd(lvName, trd, materialName, isReflected); 
    return;
  }
  
  const G4Trap* trap = dynamic_cast<const G4Trap*>(consSolid);
  if (trap) { 
    WriteTrap(lvName, trap, materialName, isReflected); 
    return;
  }
  
  const G4Para* para = dynamic_cast<const G4Para*>(consSolid);
  if (para) { 
    WritePara(lvName, para, materialName, isReflected); 
    return;
  }
  
  const G4Polycone* polycone = dynamic_cast<const G4Polycone*>(consSolid);
  if (polycone) { 
    WritePolycone(lvName, polycone, materialName, isReflected); 
    return;
  }
  
  const G4Polyhedra* polyhedra = dynamic_cast<const G4Polyhedra*>(consSolid);
  if (polyhedra) { 
    WritePolyhedra(lvName, polyhedra, materialName, isReflected); 
    return;
  }
  
  // write comment line in case of unsupported
  // shape

  // Not supported solid
  WriteNotSupportedSolid(lvName, materialName);
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WritePosition(const G4String& name, 
                                     G4ThreeVector position) 
{
// Writes position without rotation with a given solid name. 
// ---

  // get parameters
  G4double x = position.x()/TG4XMLUnits::Length();
  G4double y = position.y()/TG4XMLUnits::Length();
  G4double z = position.z()/TG4XMLUnits::Length();

  // compose element string template
  G4String element1 = "<posXYZ      volume=\"#####################   X_Y_Z=\"";
  G4String element2 = "\" />";
  
  // put solid name
  PutName(element1, name, "#");
  
  // write element
  fOutFile << fIndention
           << element1;

  SmartPut(fOutFile, fNW+1, fNP, x, "; ");
  SmartPut(fOutFile, fNW+1, fNP, y, "; ");
  SmartPut(fOutFile, fNW+1, fNP, z, "");

  fOutFile << element2
	   << std::endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteRotation(const G4String& /*name*/, 
                                     const G4RotationMatrix* rotation)
{
// Writes G4RotationMatrix. 
// Not yet implemented, only XML comment element is written.
// ---

  // return if this rotation was already written
  G4int nofRotations = fRotations.size();
  if (nofRotations>0)
    for (G4int i=0; i<nofRotations; i++) 
      if (fRotations[i] == rotation) return;

  fRotations.push_back(rotation);  


  // get parameters
  G4double xx = rotation->xx();
  G4double xy = rotation->xy();
  G4double xz = rotation->xz();
  G4double yx = rotation->yx();
  G4double yy = rotation->yy();
  G4double yz = rotation->yz();
  G4double zx = rotation->zx();
  G4double zy = rotation->zy();
  G4double zz = rotation->zz();
  G4String id = "RM";
  Append(id, ++fRotationCounter);
 
  // compose element string template
  G4String quota = "\"\n";
  G4String element1 = "<rot_matrix   id=\"#######  XX_XY_XZ=\"";
  G4String element2 = "                           YX_YY_YZ=\"";
  G4String element3 = "                           ZX_ZY_ZZ=\"";
  G4String element4 = "\" />";
  
  // put identifier
  PutName(element1, id, "#");

  // write element
  fOutFile << fkBasicIndention
           << element1
	   << std::setw(8) << std::setprecision(5) << xx << "; "  
	   << std::setw(8) << std::setprecision(5) << xy << "; "  
	   << std::setw(8) << std::setprecision(5) << xz << quota
           << fkBasicIndention
           << element2
	   << std::setw(8) << std::setprecision(5) << yx << "; "  
	   << std::setw(8) << std::setprecision(5) << yy << "; "  
	   << std::setw(8) << std::setprecision(5) << yz << quota
	   << fkBasicIndention
           << element3
	   << std::setw(8) << std::setprecision(5) << zx << "; "  
	   << std::setw(8) << std::setprecision(5) << zy << "; "  
	   << std::setw(8) << std::setprecision(5) << zz 
	   << element4 	   
           << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WritePositionWithRotation(
                           G4String lvName, G4ThreeVector position, 
			   const G4RotationMatrix* rotation)
{
// Writes position with rotation and reflection with a given solid name. 
// ---

  // get parameters
  G4double x = position.x()/TG4XMLUnits::Length();
  G4double y = position.y()/TG4XMLUnits::Length();
  G4double z = position.z()/TG4XMLUnits::Length();
  G4double xx = rotation->xx();
  G4double xy = rotation->xy();
  G4double xz = rotation->xz();
  G4double yx = rotation->yx();
  G4double yy = rotation->yy();
  G4double yz = rotation->yz();
  G4double zx = rotation->zx();
  G4double zy = rotation->zy();
  G4double zz = rotation->zz();
  
/*
  // find rotation
  G4int i=0;
  while (i<fRotations.size() && fRotations[i] != rotation) i++; 
  if (i==fRotations.size()) {
    G4cerr << "    TG4AGDDConvertor::WritePositionWithRotation: " << G4endl;
    G4cerr << "    Unknown rotation - fatal error." << G4endl;   
    G4cerr << "*** Exception: Aborting execution ***" << G4endl;   
    exit(1);
  }  
  G4String id = "RM";
  Append(id, i); 
*/  

  // compose element string template
  G4String quota = "\"\n";
  G4String element1 = "<transform   volume=\"#####################     pos=\"";
  G4String element2 = "                                            matrix=\"";
  G4String element3 = "                                                    ";
  G4String element4 = "\" />";
  
  // put solid name
  PutName(element1, lvName, "#");
  
  // write element
  fOutFile << fIndention
           << element1;

  SmartPut(fOutFile, fNW+1, fNP, x, "; ");	   
  SmartPut(fOutFile, fNW+1, fNP, y, "; ");	   
  SmartPut(fOutFile, fNW+1, fNP, z, quota);	   

  fOutFile << fIndention
	   << element2; 
	   
  SmartPut(fOutFile, 8, 5, xx, "; ");	   
  SmartPut(fOutFile, 8, 5, xy, "; ");	   
  SmartPut(fOutFile, 8, 5, xz, "; ");	   

  fOutFile << std::endl
           << fIndention
           << element3;

  SmartPut(fOutFile, 8, 5, yx, "; ");	   
  SmartPut(fOutFile, 8, 5, yy, "; ");	   
  SmartPut(fOutFile, 8, 5, yz, "; ");	   

  fOutFile << std::endl
	   << fIndention
           << element3;

  SmartPut(fOutFile, 8, 5, zx, "; ");	   
  SmartPut(fOutFile, 8, 5, zy, "; ");	   
  SmartPut(fOutFile, 8, 5, zz, "");	   

  fOutFile << element4
	   << std::endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteMultiplePosition(
                            const G4String& lvName,
                            EAxis axis, G4int nofReplicas,
			    G4double width, G4double offset)			       
			     
{
// Writes multiple position. 
// ---

  G4String tag;
  switch (axis) {
    case kXAxis: tag = "X"; break;
    case kYAxis: tag = "Y"; break;
    case kZAxis: tag = "Z"; break;
    case kRho:   tag = "R"; break;
    case kPhi:   tag = "Phi"; break;
    case kRadial3D:  tag = "R3D"; break; // CHECK
    case kUndefined: tag = "Undefined"; break;
  }  

  // set units
  G4double value0 = - width*(nofReplicas-1)*0.5 + offset;
  G4double dValue = width;
  if (axis != kPhi) {
    value0 = value0/TG4XMLUnits::Length();
    dValue = dValue/TG4XMLUnits::Length();
  }  
  else  {
    value0 = value0/TG4XMLUnits::Angle();
    dValue = dValue/TG4XMLUnits::Angle();
  }  
  
  // set tag and attributes names
  G4String a0 = "mpos"; a0 = a0 + tag;
  G4String a1 = tag;  a1 = a1 + "0";
  G4String a2 = "d";  a2 = a2 + tag; 

  // compose element string template
  G4String element1 = "<" + a0 + "      volume=\"#####################   ncopy=\"";
  G4String element2 = "\"   " + a1 + "=\"";
  G4String element3 = "\"   " + a2 + "=\"";
  G4String element4 = "\" />";
  
  // put solid name
  PutName(element1, lvName, "#");
  
  // write element
  fOutFile << fIndention
           << element1
           << std::setw(fNW+1) << std::setprecision(fNP) << nofReplicas
	   << element2
           << std::setw(fNW+1) << std::setprecision(fNP) << value0
	   << element3	   
           << std::setw(fNW+1) << std::setprecision(fNP) << dValue
	   << element4
	   << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::WriteEmptyLine()
{
// Writes empty line.
// ---

  fOutFile << G4endl;
}  

//_____________________________________________________________________________
void TG4AGDDConvertor::IncreaseIndention()
{
// Increase indention
// ---

  fIndention.append(fkBasicIndention);	   
}

//_____________________________________________________________________________
void TG4AGDDConvertor::DecreaseIndention()
{
// Decrease indention
// ---
  
  fIndention.replace(fIndention.find(fkBasicIndention), 3 , "");
}
