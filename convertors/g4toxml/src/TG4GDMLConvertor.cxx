// $Id: TG4GDMLConvertor.cxx,v 1.2 2004/07/09 17:17:00 brun Exp $
//
// Author: I. Hrivnacova, 31.03.2004
//
// Class TG4GDMLConvertor 
// ----------------------
// See the class description in the header file.

#include <iostream>
#include <iomanip>
#include <math.h>
#include <float.h>
#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

#include <G4PVReplica.hh>
#include <G4Material.hh>
#include <G4Element.hh>
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
#include <G4Sphere.hh>

#include "TG4GDMLConvertor.h"
#include "TG4Polycone.h"
#include "TG4Polyhedra.h"
#include "TG4XMLUnits.h"

const G4int    TG4GDMLConvertor::fgkDefaultNumWidth = 10;
const G4int    TG4GDMLConvertor::fgkDefaultNumPrecision = 4;
const G4String TG4GDMLConvertor::fgkSolidNameExtension  = "_s";  
const G4String TG4GDMLConvertor::fgkElementNameExtension = "_e"; 
const char     TG4GDMLConvertor::fgkCharReplacement = '_'; 
const G4String TG4GDMLConvertor::fgkNotAllowedChars = " +-*/&<>%^";
const G4String TG4GDMLConvertor::fgkNotAllowedChars1 = "0123456789"; 

//_____________________________________________________________________________
TG4GDMLConvertor::TG4GDMLConvertor(std::ofstream& outFile,
		                   const G4String& unitName, 
                                   const G4String& version)
  : fOutFile(outFile),
    fUnitName(unitName),
    fVersion(version),
    fkBasicIndention("   "),
    fIndention(fkBasicIndention),
    fNW(fgkDefaultNumWidth),
    fNP(fgkDefaultNumPrecision),
    fGDMLNames()
{
  fOutFile.width(fgkDefaultNumWidth);
  fOutFile.precision(fgkDefaultNumPrecision);
}

//_____________________________________________________________________________
TG4GDMLConvertor::~TG4GDMLConvertor() {
//
}

//
// private methods
//

//_____________________________________________________________________________
G4String TG4GDMLConvertor::UpdateName(const G4String& name,
                                      const G4String& extension) const
{
// Removes spaces after the name if present,
// replaces not allowed characters with fgkCharReplacement inside names
// and appeds the specified extension.
// ---

  G4String newName(name);

  // Remove spaces after the name
  G4int i = newName.length();
  while (newName(--i) == ' ') newName = newName(0,i);
  
  // Replace not allowed characters
  //
  for (i=0; i<G4int(newName.length()); i++) {
  
    if (( i==0 && 
          fgkNotAllowedChars1.find(newName(i)) < fgkNotAllowedChars1.size()) ||
          fgkNotAllowedChars.find(newName(i)) < fgkNotAllowedChars.size() )
	    
      newName(i) = fgkCharReplacement;
  }      
      
  // Append extension
  newName.append(extension);
  
  return newName;     
}  

//_____________________________________________________________________________
G4double TG4GDMLConvertor::UpdateAngle(G4double angle) const
{
// Checks if the value of the angle is within (-2*PI, 2*PI),
// and converts the value to fit within (0, 2*PI).
// ---
 
  if (angle < -2.0*M_PI || angle > 2.0*M_PI) {
    std::cerr << "+++ Warning +++: " << G4endl;
    std::cerr << "    Angle " 
              << angle/TG4XMLUnits::Angle() 
              << " is outside <-360., 360.>; it was converted to 0." << G4endl;
    return 0.;	      
  }
  
  if (angle < 0.) return angle + 2.0*M_PI;
  
  return angle;  
}

//_____________________________________________________________________________
void TG4GDMLConvertor::Append(G4String& name, G4int size, G4String string) const
{
// Completes the specified string with spaces to get the desired size
// and appends it to the name.
// ---

  for (G4int i=0; i < size - G4int(string.size()); i++) {
    //G4cout << string << "appending " << i << G4endl;
    name.append(" ");
  }  
  name.append(string);
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::RegisterName(const G4String& name, G4bool warning)
 {
  
  // Check if the name is unique
  if (fGDMLNames.find(name) == fGDMLNames.end())
    fGDMLNames.insert(name);
  else 
    if (warning) {
      std::cerr << "+++ Warning +++: " << G4endl;
      std::cerr << "    Duplicated name has occured: \"" << name << "\"" 
                << " (in geometry: \"" << name << "\")" << G4endl;
      std::cerr << "    Duplicated names are not allowed in GDML." << G4endl;
    }
}  

//_____________________________________________________________________________
std::ostream& TG4GDMLConvertor::SmartPut(std::ostream& out, 
                                       G4int size, G4int precision,
				       const G4String& separator1,
                                       G4double number, 
				       const G4String& separator2) const
{
// Help function to supress - sign in case the number == 0
// within the given precision

  if ( Round(number*pow(10.,precision))/pow(10.,precision) == 0.0) {
    number = 0.;
  }  
  
  out << separator1;
  out << std::setw(size) << std::setprecision(precision) << number;
  out << separator2;
  
  return out;
}

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteBox(G4String name, const G4Box* box, 
                                G4String materialName, G4bool)
{
// Writes G4box solid.
// ---

  // get parameters
  G4double x = box->GetXHalfLength()/TG4XMLUnits::Length();
  G4double y = box->GetYHalfLength()/TG4XMLUnits::Length();
  G4double z = box->GetZHalfLength()/TG4XMLUnits::Length();

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<box  lunit=\"cm\" aunit=\"degree\"";
  G4String element2 = "name=\"" + name + quota;
  G4String element3 = "x=\"";
  G4String element4 = "y=\"";
  G4String element5 = "z=\"";
  G4String element6 = "\" />";
  G4String indention = fIndention + fkBasicIndention;
  
  // write element
  fOutFile << fIndention << element1 << G4endl  
           << indention  << element2 << G4endl
	   << indention        
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << x << quota << "  "
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << y << quota << "  "
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << z 
	   << element6 << G4endl << G4endl;
}
 
//_____________________________________________________________________________
void TG4GDMLConvertor::WriteTubs(G4String name, const G4Tubs* tubs, 
                                 G4String materialName, G4bool)
{
// Writes G4tubs solid.
// ---

  // get parameters
  G4double rmin = tubs->GetInnerRadius()/TG4XMLUnits::Length();
  G4double rmax = tubs->GetOuterRadius()/TG4XMLUnits::Length();
  G4double hz   = tubs->GetZHalfLength()/TG4XMLUnits::Length();
  G4double sphi = UpdateAngle(tubs->GetStartPhiAngle())/TG4XMLUnits::Angle();
  G4double dphi = UpdateAngle(tubs->GetDeltaPhiAngle())/TG4XMLUnits::Angle();

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<tube  lunit=\"cm\" aunit=\"degree\"";
  G4String element2 = "name=\"" + name + quota;
  G4String element3 = "z=\"";
  G4String element4 = "rmin=\"";
  G4String element5 = "rmax=\"";
  G4String element6 = "startphi=\"";
  G4String element7 = "deltaphi=\"";
  G4String element8 = "\" />";
  G4String indention = fIndention + fkBasicIndention;
  
  // write element
  fOutFile << fIndention << element1 << G4endl
	   << indention  << element2 << G4endl
	   << indention        
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << rmin << quota << "  "
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << rmax << quota << "  " 
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << hz << quota
	   << G4endl
	   << indention        
	   << element6 << std::setw(fNW)   << std::setprecision(fNP) << sphi << quota << "  "
	   << element7 << std::setw(fNW)   << std::setprecision(fNP) << dphi
	   << element8 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteCons(G4String name, const G4Cons* cons, 
                                 G4String materialName, G4bool isReflected)
{
// Writes G4cons solid.
// ---

  // get parameters
  G4double rmin1 = cons->GetInnerRadiusMinusZ()/TG4XMLUnits::Length();
  G4double rmax1 = cons->GetOuterRadiusMinusZ()/TG4XMLUnits::Length();
  G4double rmin2 = cons->GetInnerRadiusPlusZ()/TG4XMLUnits::Length();
  G4double rmax2 = cons->GetOuterRadiusPlusZ()/TG4XMLUnits::Length();
  G4double hz   = cons->GetZHalfLength()/TG4XMLUnits::Length();
  G4double sphi = UpdateAngle(cons->GetStartPhiAngle())/TG4XMLUnits::Angle();
  G4double dphi = UpdateAngle(cons->GetDeltaPhiAngle())/TG4XMLUnits::Angle();

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
  G4String element1 = "<cone  lunit=\"cm\" aunit=\"degree\"";
  G4String element2 = "name=\"" + name + quota;
  G4String element3 = "z=\"";
  G4String element4 = "rmin1=\"";
  G4String element5 = "rmin2=\"";
  G4String element6 = "rmax1=\"";
  G4String element7 = "rmax2=\"";
  G4String element8 = "startphi=\"";
  G4String element9 = "deltaphi=\"";
  G4String element10 = "\" />";
  G4String indention = fIndention + fkBasicIndention;
  
  // write element
  fOutFile << fIndention << element1 << G4endl
	   << indention  << element2 << G4endl
	   << indention        
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << rmin1 << quota << "  "
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << rmin2 << quota << "  "
	   << element6 << std::setw(fNW) << std::setprecision(fNP) << rmax1 << quota << "  "
	   << element7 << std::setw(fNW) << std::setprecision(fNP) << rmax2 << quota << G4endl
	   << indention        
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << hz << quota << G4endl
	   << indention        
	   << element8 << std::setw(fNW)   << std::setprecision(fNP) << sphi << quota << "  "
	   << element9 << std::setw(fNW)   << std::setprecision(fNP) << dphi
	   << element10 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteTrd(G4String name, const G4Trd* trd, 
                                G4String materialName, G4bool isReflected)
{
// Writes G4Trd solid.
// ---

  // get parameters
  G4double x1 = trd->GetXHalfLength1()/TG4XMLUnits::Length();
  G4double x2 = trd->GetXHalfLength2()/TG4XMLUnits::Length();
  G4double y1 = trd->GetYHalfLength1()/TG4XMLUnits::Length();
  G4double y2 = trd->GetYHalfLength2()/TG4XMLUnits::Length();
  G4double hz = trd->GetZHalfLength()/TG4XMLUnits::Length();

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
  G4String element1 = "<trd  lunit=\"cm\" aunit=\"degree\"";
  G4String element2 = "name=\"" + name + quota;
  G4String element3 = "x1=\"";
  G4String element4 = "x2=\"";
  G4String element5 = "y1=\"";
  G4String element6 = "y2=\"";
  G4String element7 = "z=\"";
  G4String element8 = "\" />";
  G4String indention = fIndention + fkBasicIndention;
  
  // write element
  fOutFile << fIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << x1 << quota << "  "
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << x2 << quota << "  "
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << y1 << quota << "  "
	   << element6 << std::setw(fNW) << std::setprecision(fNP) << y2 << quota << G4endl
	   << indention
	   << element7 << std::setw(fNW) << std::setprecision(fNP) << hz
	   << element8 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteTrap(G4String name, const G4Trap* trap, 
                                 G4String materialName, G4bool isReflected)
{
// Writes G4Trap solid.
// ---

  // get parameters
  G4double dz = trap->GetZHalfLength()/TG4XMLUnits::Length();
  G4ThreeVector symAxis = trap->GetSymAxis();
  G4double y1 = trap->GetYHalfLength1()/TG4XMLUnits::Length();
  G4double x1 = trap->GetXHalfLength1()/TG4XMLUnits::Length();
  G4double x2 = trap->GetXHalfLength2()/TG4XMLUnits::Length();
  G4double tanAlpha1 = trap->GetTanAlpha1();
  G4double y2 = trap->GetYHalfLength2()/TG4XMLUnits::Length();
  G4double x3 = trap->GetXHalfLength3()/TG4XMLUnits::Length();
  G4double x4 = trap->GetXHalfLength4()/TG4XMLUnits::Length();
  G4double tanAlpha2 = trap->GetTanAlpha2();

  // Get angles
  G4double theta  = symAxis.theta() / TG4XMLUnits::Angle();
  G4double phi    = symAxis.phi() / TG4XMLUnits::Angle();
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
    theta = -theta;
    phi = -phi;
  }    

  // compose element string template
  G4String quota = "\"";
  G4String element1  = "<trap  lunit=\"cm\" aunit=\"degree\"";
  G4String element2  = "name=\"" + name + quota;
  G4String element3  = "z=\"";
  G4String element4  = "theta=\"";
  G4String element5  = "phi=\"";
  G4String element6  = "y1=\"";
  G4String element7  = "x1=\"";
  G4String element8  = "x2=\"";
  G4String element9  = "alpha1=\"";
  G4String element10 = "y2=\"";
  G4String element11 = "x3=\"";
  G4String element12 = "x4=\"";
  G4String element13 = "alpha2=\"";
  G4String element14 = "\" />";
  G4String indention = fIndention + fkBasicIndention;

  // write element
  fOutFile << fIndention << element1 << G4endl
	   << indention        << element2 << G4endl
	   << indention        
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << dz << quota << G4endl
	   << indention        
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << theta << quota << "  "
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << phi << quota << G4endl
	   << indention        
	   << element6 << std::setw(fNW) << std::setprecision(fNP) << y1 << quota << "  "
	   << element7 << std::setw(fNW) << std::setprecision(fNP) << x1 << quota << "  "
	   << element8 << std::setw(fNW) << std::setprecision(fNP) << x2 << quota << "  "
	   << element9 << std::setw(fNW) << std::setprecision(fNP) << alpha1 << quota << G4endl
	   << indention        
	   << element10 << std::setw(fNW) << std::setprecision(fNP) << y2 << quota << "  "
	   << element11 << std::setw(fNW) << std::setprecision(fNP) << x3 << quota << "  "
	   << element12 << std::setw(fNW) << std::setprecision(fNP) << x4 << quota << "  "
	   << element13 << std::setw(fNW) << std::setprecision(fNP) << alpha2
	   << element14 << G4endl << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WritePara(G4String name, const G4Para* para, 
                                 G4String materialName, G4bool isReflected)
{
// Writes G4Para solid.
// ---

  // get parameters
  G4double dx = para->GetXHalfLength()/TG4XMLUnits::Length();
  G4double dy = para->GetYHalfLength()/TG4XMLUnits::Length();
  G4double dz = para->GetZHalfLength()/TG4XMLUnits::Length();
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
   theta = M_PI/TG4XMLUnits::Angle() - theta; 
  }    

  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<para  lunit=\"cm\" aunit=\"degree\"";
  G4String element2 = "name=\"" + name + quota;
  G4String element3 = "x=\"";
  G4String element4 = "y=\"";
  G4String element5 = "z=\"";
  G4String element6 = "alpha=\"";
  G4String element7 = "theta=\"";
  G4String element8 = "phi=\"";
  G4String element9 = "\" />";
  G4String indention = fIndention + fkBasicIndention;
  
  // write element
  fOutFile << fIndention << element1 << G4endl  
           << indention        << element2 << G4endl
	   << indention        
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << dx << quota << "  "
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << dy << quota << "  "
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << dz << quota << G4endl
	   << indention        
	   << element6 << std::setw(fNW) << std::setprecision(fNP) << alpha << quota << "  "
	   << element7 << std::setw(fNW) << std::setprecision(fNP) << theta << quota << "  "
	   << element8 << std::setw(fNW) << std::setprecision(fNP) << phi 
	   << element9 << G4endl << G4endl;
}
 
/*
//_____________________________________________________________________________
void TG4GDMLConvertor::WritePolycone(G4String name, const G4Polycone* polycone, 
                                    G4String materialName, G4bool isReflected)
{
// Writes G4Polycone solid.
// Not yet supported by GDML.
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
  G4String element1 = "<pcon   name=\"" + name + quota; 
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
*/

/*
//_____________________________________________________________________________
void TG4GDMLConvertor::WritePolyhedra(G4String name, const G4Polyhedra* polyhedra, 
                                    G4String materialName, G4bool isReflected)
{
// Writes G4Polycone solid.
// Not yet supported by GDML.
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
  G4String element1 = "<phedra name=\"" + name + quota; 
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
*/

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteSphere(G4String name, const G4Sphere* sphere, 
                                   G4String materialName, G4bool)
{
// Writes G4Sphere solid.
// ---

  // get parameters
  G4double rmin = sphere->GetInsideRadius()/TG4XMLUnits::Length();
  G4double rmax = sphere->GetOuterRadius()/TG4XMLUnits::Length();
  G4double sphi = UpdateAngle(sphere->GetStartPhiAngle())/TG4XMLUnits::Angle();
  G4double dphi = UpdateAngle(sphere->GetDeltaPhiAngle())/TG4XMLUnits::Angle();
  G4double stheta = UpdateAngle(sphere->GetStartThetaAngle())/TG4XMLUnits::Angle();
  G4double dtheta = UpdateAngle(sphere->GetDeltaThetaAngle())/TG4XMLUnits::Angle();


  // compose element string template
  G4String quota = "\"";
  G4String element1 = "<sphere  lunit=\"cm\" aunit=\"degree\"";
  G4String element2 = "name=\"" + name + quota;
  G4String element3 = "rmin=\"";
  G4String element4 = "rmax=\"";
  G4String element5 = "startphi=\"";
  G4String element6 = "deltaphi=\"";
  G4String element7 = "starttheta=\"";
  G4String element8 = "deltatheta=\"";
  G4String element9 = "\" />";
  G4String indention = fIndention + fkBasicIndention;
  
  // write element
  fOutFile << fIndention << element1 << G4endl  
           << indention  << element2 << G4endl
	   << indention        
	   << element3 << std::setw(fNW) << std::setprecision(fNP) << rmin << quota << "  "
	   << element4 << std::setw(fNW) << std::setprecision(fNP) << rmax << quota << G4endl
	   << indention        
	   << element5 << std::setw(fNW) << std::setprecision(fNP) << sphi << quota << "  "
	   << element6 << std::setw(fNW) << std::setprecision(fNP) << dphi << quota << "  "
	   << element7 << std::setw(fNW) << std::setprecision(fNP) << stheta << quota << "  "
	   << element8 << std::setw(fNW) << std::setprecision(fNP) << dtheta
	   << element9 << G4endl << G4endl;
}
 
//
// public methods
//

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenDocument()
{
// Writes document opening.
// Could be made customizable in future.
// ---
			 
  fOutFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"\?>" 
           << std::endl
           << "<gdml xmlns:gdml=\"http://cern.ch/2001/Schemas/GDML\"" 
	   << std::endl
	   << "      xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" 
	   << std::endl
	   << "      xsi:noNamespaceSchemaLocation=\"gdml_1.0.xsd\">" 
	   << std::endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenSection(const G4String& /*topVolume*/)
{
// Writes section opening.
// ---

  // nothing to be done in GDML			 
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenPositions()
{
// Opens element for general definitions 
// (positions and rotations for generated XML files.)
// ---

  G4String element1 = "<define>"; 

  // write element
  fOutFile << fIndention << element1 << G4endl;

  // increase indention
  IncreaseIndention();	   
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenRotations()
{
// Rotations are written in the same element as positions.
// The element is already open.
// ---
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenMaterials()
{
// Writes materials opening.
// ---
			 
  G4String element = "<materials>";
  
  // write element
  fOutFile << fIndention << element << G4endl;

  // increase indention
  IncreaseIndention();	   
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenSolids()
{
// Writes solids opening. 
// ---

  G4String element = "<solids>"; 

  // write element
  fOutFile << fIndention << element << G4endl;

  // increase indention
  IncreaseIndention();	   
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenStructure()
{
// Writes composition opening.
// ---
			 
  G4String element = "<structure>";
  
  // Write element
  fOutFile << fIndention << element << G4endl;

  // increase indention
  IncreaseIndention();	   
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::OpenComposition(const G4String& name,
                                       const G4String& materialName)
{
// Writes composition opening.
// ---

  // Update names
  G4String volName = UpdateName(name);
  G4String matName = UpdateName(materialName);
  G4String sldName = UpdateName(name, fgkSolidNameExtension);
  
  // Register volume name
  RegisterName(volName);
			 
  G4String element1 = "<volume name=\"";
  element1.append(volName);
  element1.append("\">");
  
  G4String element2 = "<materialref ref=\"";
  element2.append(matName);
  element2.append("\"/>");
  
  G4String element3 = "<solidref ref=\"";
  element3.append(sldName);
  element3.append("\"/>");
  
  G4String indention =  fIndention + fkBasicIndention;

  // write element
  fOutFile << fIndention << element1 << G4endl
           << indention  << element2 << G4endl
           << indention  << element3 << G4endl;

  // increase indention
  IncreaseIndention();	   
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseDocument()
{
// Writes document closing.
// ---

   fOutFile << "</gdml>" << std::endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseSection(const G4String& topVolume)
{
// Writes section closing.
// ---

  // Define element
  //
  G4String quota = "\"";
  G4String element2 = "<setup name=\"";
  element2.append(fUnitName);
  element2.append(quota);
  
  G4String element3 = "version=\"";
  element3.append(fVersion);
  element3.append(quota);
  		
  G4String element4 = "<world ref=\"";
  element4.append(topVolume);
  element4.append(quota);
    
  G4String element5 = "</setup>";
  G4String indention = fIndention + fkBasicIndention;
 
  // Write element
  //
  fOutFile << fIndention       << element2
	   << fkBasicIndention << element3 << ">"  << G4endl
	   << indention        << element4 << "/>" << G4endl
	   << fIndention       << element5 << G4endl
	   << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::ClosePositions()
{
// Do not close the element.
// Rotations will be written in the same element as positions.
// ---

}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseRotations()
{
// Close element for general definitions 
// (positions and rotations for generated XML files.)
// ---

  // decrease indention
  DecreaseIndention();	   

  G4String element1 = "</define>"; 

  // write element
  fOutFile << fIndention << element1 << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseMaterials()
{
// Writes materials closing.
// ---

  // decrease indention
  DecreaseIndention();	   

  // define element
  G4String element = "</materials>";

  // write element
  fOutFile << fIndention << element << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseSolids()
{
// Close element for solids. 
// ---

  // decrease indention
  DecreaseIndention();	   

  G4String element1 = "</solids>"; 

  // write element
  fOutFile << fIndention << element1 << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseStructure()
{
// Close element for solids. 
// ---

  // decrease indention
  DecreaseIndention();	   

  G4String element = "</structure>";

  // write element
  fOutFile << fIndention << element << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::CloseComposition()
{
// Writes composition closing.
// ---

  // decrease indention
  DecreaseIndention();

  // define element
  G4String element = "</volume>";

  // write element
  fOutFile << fIndention << element << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteElement(const G4Element* element) 
{
// Writes G4Element. 
// ---

  G4String name = UpdateName(element->GetName(), fgkElementNameExtension);
  RegisterName(name);  
    
  // Get parameters
  G4double theZ = element->GetZ();
  G4int    theN = (G4int) round(element->GetN());
  G4double theA = element->GetA()/ TG4XMLUnits::AtomicWeight();

  // Compose element string template
  G4String quota1 = "\"";
  G4String quota2 = "\"  ";
  G4String element1 = "<element  name=";
  Append(element1, 15, quota1 + name + quota2);
  
  G4String element2 = "Z=\"";
  G4String element3 = "N=\"";
  G4String element4 = "<atom type=\"A\" unit=\"g/mol\" value=\"";
  G4String element5 = "\" />";
  G4String element6 = "</element>";
  
  G4String indention = fIndention + fkBasicIndention;
  
  // Write element
  fOutFile << fIndention << element1;
  SmartPut(fOutFile, fNW-2, fNP, element2, theZ, quota2);
  //SmartPut(fOutFile, fNW-2, fNP, element3, theN, "\" >");
  //fOutFile << G4endl; 
  fOutFile << element3 << std::setw(3) << theN << "\" >" << G4endl;
  
  fOutFile << indention;
  SmartPut(fOutFile, fNW-2, fNP, element4, theA, element5);
  fOutFile << G4endl; 

  fOutFile << fIndention << element6 << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteMaterial(const G4Material* material) 
{
// Writes G4Material. 
// ---

  G4String materialName = UpdateName(material->GetName());
  RegisterName(materialName);

  // Get parameters
  G4double density = material->GetDensity()/ TG4XMLUnits::MassDensity();
 
  // Compose material string template
  G4String quota = "\"  ";
  G4String element1 = "<material  name=\"";
  element1.append(materialName);
  element1.append(quota);
  element1.append(">");
  
  G4String element2 = "<D type=\"density\" unit=\"g/cm3\" value=\"";
  G4String element3 = "<fraction n=\"";
  G4String element4 = "ref=\"";
  G4String element5 = "\"/>";
  G4String element6 = "</material>";
  
  G4String indention = fIndention + fkBasicIndention;
  
  // Write element
  fOutFile << fIndention << element1 << G4endl;

  fOutFile << indention;
  SmartPut(fOutFile, fNW+1, fNP, element2, density, element5);
  fOutFile << G4endl; 
  
  for (G4int i=0; i<G4int(material->GetNumberOfElements()); i++) {
    G4double fraction 
      = (material->GetFractionVector())[i];
    G4String elementName
      = UpdateName(material->GetElement(i)->GetName(), 
                   fgkElementNameExtension);  

    fOutFile << indention;
    SmartPut(fOutFile, fNW, fNP, element3, fraction, quota);
    fOutFile << element4 << elementName << element5 << G4endl;
  }  

  fOutFile << fIndention << element6 << G4endl;
 }  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteNotSupportedSolid(G4String name, 
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

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteSolid(G4String lvName, const G4VSolid* solid, 
                                  G4String materialName) 
{
// Finds G4Solid concrete type and calls writing function. 
// For not yet implemented solids, only XML comment element is written.
// ---

  G4String solidName = UpdateName(lvName, fgkSolidNameExtension);
  RegisterName(solidName);
  
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
    WriteBox(solidName, box, materialName, isReflected); 
    return;
  }
  
  const G4Tubs* tubs = dynamic_cast<const G4Tubs*>(consSolid);
  if (tubs) { 
    WriteTubs(solidName, tubs, materialName, isReflected); 
    return;
  }
  
  const G4Cons* cons = dynamic_cast<const G4Cons*>(consSolid);
  if (cons) { 
    WriteCons(solidName, cons, materialName, isReflected); 
    return;
  }
  
  const G4Trd* trd = dynamic_cast<const G4Trd*>(consSolid);
  if (trd) { 
    WriteTrd(solidName, trd, materialName, isReflected); 
    return;
  }
  
  const G4Trap* trap = dynamic_cast<const G4Trap*>(consSolid);
  if (trap) { 
    WriteTrap(solidName, trap, materialName, isReflected); 
    return;
  }
  
  const G4Para* para = dynamic_cast<const G4Para*>(consSolid);
  if (para) { 
    WritePara(solidName, para, materialName, isReflected); 
    return;
  }
  
  const G4Sphere* sphere = dynamic_cast<const G4Sphere*>(consSolid);
  if (sphere) { 
    WriteSphere(solidName, sphere, materialName, isReflected); 
    return;
  }

/*
  const G4Polycone* polycone = dynamic_cast<const G4Polycone*>(consSolid);
  if (polycone) { 
    WritePolycone(solidName, polycone, materialName, isReflected); 
    return;
  }
  
  const G4Polyhedra* polyhedra = dynamic_cast<const G4Polyhedra*>(consSolid);
  if (polyhedra) { 
    WritePolyhedra(solidName, polyhedra, materialName, isReflected); 
    return;
  }
*/  
  // Not supported solid
  WriteNotSupportedSolid(solidName, materialName);

}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WritePosition(const G4String& name, 
                                     G4ThreeVector position) 
{
// Writes position element with a given name. 
// ---

  // get parameters
  G4double x = position.x()/TG4XMLUnits::Length();
  G4double y = position.y()/TG4XMLUnits::Length();
  G4double z = position.z()/TG4XMLUnits::Length();

  // compose element string template
  G4String quota1 = "\"";
  G4String quota2 = "\"  ";
  G4String element1 = "<position  name=";
  Append(element1, 12, quota1 + name + quota2);

  G4String element2 = "x=\"";
  G4String element3 = "y=\"";
  G4String element4 = "z=\"";
  G4String element5 = "\"  unit=\"cm\" />";
  
  // write element
  fOutFile << fIndention << element1;

  SmartPut(fOutFile, fNW+1, fNP, element2, x, quota2);
  SmartPut(fOutFile, fNW+1, fNP, element3, y, quota2);
  SmartPut(fOutFile, fNW+1, fNP, element4, z, "");

  fOutFile << element5 << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteRotation(const G4String& name, 
                                     const G4RotationMatrix* rotation)
{
// Writes rotation element with a given name. 
// ---

  // Get parameters
  double angleX;
  double angleY;
  double angleZ;
  double cosb = sqrt(  rotation->xx()*rotation->xx()
                     + rotation->yx()*rotation->yx() ); 
  if (cosb > 16*FLT_EPSILON) {
    angleX = atan2( rotation->zy(), rotation->zz());
    angleY = atan2(-rotation->zx(), cosb);
    angleZ = atan2( rotation->yx(), rotation->xx());
  }
  else{
    angleX = atan2(-rotation->yz(), rotation->yy());
    angleY = atan2(-rotation->zx(), cosb);
    angleZ = 0.;
  }

  // Apply units
  angleX /= TG4XMLUnits::Angle();
  angleY /= TG4XMLUnits::Angle();
  angleZ /= TG4XMLUnits::Angle();


  // Compose element string template
  G4String quota1 = "\"";
  G4String quota2 = "\"  ";
  G4String element1 = "<rotation  name=";
  Append(element1, 12, quota1 + name + quota2);

  G4String element2 = "x=\"";
  G4String element3 = "y=\"";
  G4String element4 = "z=\"";
  G4String element5 = "\"  unit=\"degree\" />";
  
  // Write element
  fOutFile << fIndention << element1;

  SmartPut(fOutFile, fNW+1, fNP, element2, angleX, quota2);
  SmartPut(fOutFile, fNW+1, fNP, element3, angleY, quota2);
  SmartPut(fOutFile, fNW+1, fNP, element4, angleZ, "");

  fOutFile << element5 << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WritePositionWithRotation(
                            const G4String& lvName, 
			    const G4String& positionRef,
			    const G4String& rotationRef)
{
// Writes position with rotation with a given volume name.
// ---

  // Update name
  G4String name = UpdateName(lvName);

  // Compose element strings
  //
  G4String element0 = "\"/>";
  G4String element1 = "<child>";
 
  G4String element2 = "<volumeref ref=\"";
  element2.append(name);
  element2.append(element0);

  G4String element3 = "<positionref ref=\"";
  element3.append(positionRef);
  element3.append(element0);
  
  G4String element4 = "<rotationref ref=\"";
  element4.append(rotationRef);
  element4.append(element0);

  G4String element5 = "</child>";
  
  G4String indention1 = fIndention + fkBasicIndention;
  G4String indention2 = fIndention + fkBasicIndention + fkBasicIndention;
  
  // Write element
  fOutFile << fIndention << element1 << G4endl
           << indention1 << element2 << G4endl
           << indention2 << element3 << G4endl
           << indention2 << element4 << G4endl
	   << fIndention << element5 << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteMultiplePosition(
                            const G4String& lvName,
                            EAxis axis, G4int nofReplicas,
			    G4double width, G4double offset)			       
{
// Not yet available in GDML
// ---
/*
  // get parameters
  EAxis axis;
  G4int nReplicas;
  G4double width;
  G4double offset;
  G4bool consuming;
  pvr->GetReplicationData(axis, nReplicas, width, offset, consuming);
  
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
  G4double value0 = - width*(nReplicas-1)*0.5 + offset;
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
           << std::setw(fNW+1) << std::setprecision(fNP) << nReplicas
	   << element2
           << std::setw(fNW+1) << std::setprecision(fNP) << value0
	   << element3	   
           << std::setw(fNW+1) << std::setprecision(fNP) << dValue
	   << element4
	   << G4endl;
*/
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::WriteEmptyLine()
{
// Writes empty line.
// ---

  fOutFile << G4endl;
}  

//_____________________________________________________________________________
void TG4GDMLConvertor::IncreaseIndention()
{
// Increase indention
// ---

  fIndention.append(fkBasicIndention);	   
}

//_____________________________________________________________________________
void TG4GDMLConvertor::DecreaseIndention()
{
// Decrease indention
// ---

  fIndention.replace(fIndention.find(fkBasicIndention), 3 , "");
}
