// $Id: TG4GeometryOutputManager.cxx,v 1.2 2003/12/18 13:28:08 brun Exp $
// Category: geometry
//
// Class TG4GeometryOutputManager
// ------------------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova 

#include "TG4GeometryOutputManager.h"
#include <iostream>
#include <iomanip>

//_____________________________________________________________________________
TG4GeometryOutputManager::TG4GeometryOutputManager()
  : TG4Verbose("geometryOutputManager") {
//
}

//_____________________________________________________________________________
TG4GeometryOutputManager::~TG4GeometryOutputManager() {
//
}

//
// public methods
//

//_____________________________________________________________________________
void TG4GeometryOutputManager::OpenFile(G4String filePath)
{ 
/// Open output files.

  if (VerboseLevel() > 0) {
    G4cout << "TG4GeometryOutputManager::OpenFile: " << filePath << G4endl;
  }  
  
  //fOutFile.open(filePath, ios::out, filebuf::openprot); 
  fOutFile.open(filePath, std::ios::out); //, std::filebuf::openprot); 
  
  if (!fOutFile) {
    G4String text = "Cannot open ";
    text = text + filePath;
    TG4Globals::Warning(text);  
  }
  
  // use FORTRAN compatibility output
  fOutFile << std::setiosflags(std::ios::showpoint | std::ios::uppercase);
}


//_____________________________________________________________________________
void TG4GeometryOutputManager::CloseFile()
{ 
/// Close output files.

  fOutFile.close(); 
}


//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsvolu( 
              G4String vname, G4String shape, G4int nmed, G4double* Rpar,
              G4int npar)
{
/// From fortran (g3routines.F)                                             \n
/// write(fmt,'(A,I2,A)')'(a4,1x,a6,1x,a4,1x,a4,2i5,',max(npar,1),          \n
///>      '(1x,e16.8))'                                                     \n
/// write(lunlist,fmt) context, rname, name, shape, nmed, npar,             \n
///+      (par(k),k=1,npar)                                                 \n

  G4String context("----");
  G4String rname("GSVOLU");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname    << space
	  << shape
	  << std::setw(5) << nmed  
	  << std::setw(5) << npar;
  for (G4int i=0; i<npar; i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << Rpar[i];
  fOutFile << G4endl;   
}

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGspos(
             G4String vname, G4int num, G4String vmoth, G4double x,
             G4double y, G4double z, G4int irot, G4String vonly)
{	     
/// From fortran (g3routines.F)                                            \n
/// write(lunlist,                                                         \n
///+      '(a4,1x,a6,1x,a4,i5,1x,a4,3(1x,e16.8),i5,1x,a4)')                \n
///+      context, rname, name, num, moth, x, y, z, irot, only             \n

  G4String context("----");
  G4String rname("GSPOS");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname   << space
	  << std::setw(5) << num << space 
	  << vmoth   << space
          << std::setw(16) << std::setprecision(8) << x << space
          << std::setw(16) << std::setprecision(8) << y << space
          << std::setw(16) << std::setprecision(8) << z
	  << std::setw(5) << irot << space
	  << vonly 
	  << G4endl;
}	     
  
//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsposp(
              G4String vname, G4int num, G4String vmoth, G4double x,
              G4double y, G4double z, G4int irot, G4String vonly,
              G4double* pars, G4int npar)
{
/// From fortran (g3routines.F)                                             \n
/// write(fmt,'(A,A,I2,A)')                                                 \n
///+      '(a4,1x,a6,1x,a4,i5,1x,a4,3(1x,e16.8),',                          \n
///+      'i5,1x,a4,i5,',max(npar,1),'(1x,e16.8))'                          \n
/// write(lunlist,fmt)                                                      \n  
///+      context, rname, name, num, moth, x, y, z, irot, only,             \n
///+      npar,                                                             \n 
///+      (par(k),k=1,npar)                                                 \n

  G4String context("----");
  G4String rname("GSPOSP");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname   << space
	  << std::setw(5) << num << space 
	  << vmoth   << space
          << std::setw(16) << std::setprecision(8) << x << space
          << std::setw(16) << std::setprecision(8) << y << space
          << std::setw(16) << std::setprecision(8) << z
	  << std::setw(5) << irot << space
	  << vonly 
	  << std::setw(5) << npar;
  for (G4int i=0; i<npar; i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << pars[i];
  fOutFile << G4endl;
}	      

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsrotm(
              G4int irot, G4double theta1, G4double phi1,
              G4double theta2, G4double phi2, G4double theta3, G4double phi3)
{
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
///+      '(a4,1x,a6,i5,6f11.5)')                                           \n
///+      context, rname, irot, theta1, phi1, theta2, phi2,                 \n
///+      theta3, phi3                                                      \n
  
  G4String context("----");
  G4String rname("GSROTM");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << std::setw(5) << irot
          << std::setw(11) << std::setprecision(5) << theta1
          << std::setw(11) << std::setprecision(5) << phi1
          << std::setw(11) << std::setprecision(5) << theta2
          << std::setw(11) << std::setprecision(5) << phi2
          << std::setw(11) << std::setprecision(5) << theta3
          << std::setw(11) << std::setprecision(5) << phi3
	  << G4endl;
}	  

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsdvn(
              G4String vname, G4String vmoth, G4int ndiv, G4int iaxis)
{
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
///+      '(a4,1x,a6,1x,a4,1x,a4,i5,i3)')                                   \n
///+      context, rname, name, moth, ndiv, iaxis                           \n

  G4String context("----");
  G4String rname("GSDVN");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname    << space
	  << vmoth   << space
	  << std::setw(5) << ndiv  
	  << std::setw(5) << iaxis
          << G4endl;
}	     

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsdvn2(
               G4String vname, G4String vmoth, G4int ndiv, G4int iaxis,
               G4double c0, G4int numed)
{
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
///+      '(a4,1x,a6,1x,a4,1x,a4,i5,i3,(1x,e16.8),i5)')                     \n
///+      context, rname, name, moth, ndiv, iaxis, c0, numed                \n

  G4String context("----");
  G4String rname("GSDVN2");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname    << space
	  << vmoth   << space
	  << std::setw(5) << ndiv  
	  << std::setw(5) << iaxis << " "
          << std::setw(16) << std::setprecision(8) << c0
	  << std::setw(5) << numed
	  << G4endl;
}	     

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsdvt(
             G4String vname, G4String vmoth, G4double step, G4int iaxis,
             G4int numed, G4int ndvmx) 
{
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
/// +    '(a4,1x,a6,1x,a4,1x,a4,(1x,e16.8),3i5)')                           \n
/// +    context, rname, name, moth, step, iaxis, numed, ndvmx              \n

  G4String context("----");
  G4String rname("GSDVT");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname    << space
	  << vmoth   << space
          << std::setw(16) << std::setprecision(8) << step
	  << std::setw(5) << iaxis
	  << std::setw(5) << numed
	  << std::setw(5) << ndvmx
	  << G4endl;
}	     

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsdvt2(
               G4String vname, G4String vmoth, G4double step, G4int iaxis,
               G4double c0, G4int numed, G4int ndvmx)
{	       
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
///+      '(a4,1x,a6,1x,a4,1x,a4,(1x,e16.8),i3,(1x,e16.8),2i5)')            \n
///+      context, rname, name, moth, step, iaxis, c0, numed, ndvmx         \n

  G4String context("----");
  G4String rname("GSDVT2");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << vname    << space
	  << vmoth   << space
          << std::setw(16) << std::setprecision(8) << step
	  << std::setw(3) << iaxis << space
          << std::setw(16) << std::setprecision(8) << c0
	  << std::setw(5) << numed
	  << std::setw(5) << ndvmx
	  << G4endl;
}	     

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsdvx(
             G4String name, G4String moth, G4int ndiv, G4int iaxis,
             G4double step, G4double c0, G4int numed, G4int ndvmx)
{
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
/// +     '(a4,1x,a6,1x,a4,1x,a4,i5,i3,2(1x,e16.8),2i5)')                   \n
/// +     context, rname, name, moth, ndiv, iaxis,step, c0,                 \n
/// +     numed, ndvmx

  G4String context("----");
  G4String rname("GSDVX");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << name    << space
	  << moth    << space
	  << std::setw(5) << ndiv
	  << std::setw(3) << iaxis << space
          << std::setw(16) << std::setprecision(8) << step << space
          << std::setw(16) << std::setprecision(8) << c0
	  << std::setw(5) << numed
	  << std::setw(5) << ndvmx
	  << G4endl;
}	     

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsmate(
              G4int imate, G4String name, G4double ain, G4double zin,
              G4double densin, G4double radl, G4int nwbf, G4double* ubuf)
{
/// From fortran (g3routines.F)                                             \n
/// write(fmt,'(A,I3,A)')                                                   \n
///+      '(a4,1x,a6,i5,1x,''"'',a,''"'',4(1x,e16.8),i3,',                  \n
///+      max(nwbf,1),'(1x,e16.8))'                                         \n
/// write(lunlist,fmt)                                                      \n
///+      context, rname, imate, name, a, z, dens, radl,                    \n
///+      nwbf, (ubf(k), k=1,nwbf)                                          \n

  G4String context("----");
  G4String rname("GSMATE");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << std::setw(5) << imate << space 
	  << '"' << name << '"' << space
          << std::setw(16) << std::setprecision(8) << ain << space
          << std::setw(16) << std::setprecision(8) << zin << space
          << std::setw(16) << std::setprecision(8) << densin << space
          << std::setw(16) << std::setprecision(8) << radl
	  << std::setw(3) << nwbf;
  for (G4int i=0; i<nwbf; i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << ubuf[i];
  fOutFile << G4endl;
}	      

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGsmixt(
              G4int imate, G4String name, G4double* a, G4double* z,
              G4double dens, G4int nlmat, G4double* wmat)
{
/// From fortran (g3routines.F)                                             \n
/// write(fmt,'(A,I3,A,I3,A,I3,A)')                                         \n
///+      '(a4,1x,a6,i5,1x,''"'',a,''"'',1x,e16.8,1x,i3,',                  \n
///+      max(nlmata,1),                                                    \n
///+      '(1x,e16.8),',max(nlmata,1),'(1x,e16.8),',                        \n
///+      max(nlmata,1),'(1x,e16.8))'                                       \n
/// write(lunlist,fmt)                                                      \n
///+      context, rname, imate, name, dens,                                \n
///+      nlmat,                                                            \n
///+      (a(k), k=1,abs(nlmat)),                                           \n
///+      (z(k), k=1,abs(nlmat)),                                           \n
///+      (wmat(k), k=1,abs(nlmat))                                         \n

  G4String context("----");
  G4String rname("GSMIXT");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << std::setw(5) << imate << space 
	  << '"' << name << '"' << space
          << std::setw(16) << std::setprecision(8) << dens << space
	  << std::setw(3) << nlmat;
  G4int i;	  
  for (i=0; i<abs(nlmat); i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << a[i];
  for (i=0; i<abs(nlmat); i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << z[i];
  for (i=0; i<abs(nlmat); i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << wmat[i];
  fOutFile << G4endl;
}	      

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGstmed(
              G4int itmed, G4String name, G4int nmat, G4int isvol,
              G4int ifield, G4double fieldm, G4double tmaxfd,
              G4double stemax, G4double deemax, G4double epsil,
              G4double stmin, G4double* ubuf, G4int nwbuf)
{
/// From fortran (g3routines.F)                                             \n
/// write(fmt,'(A,I3,A)')                                                   \n
///>      '(a4,1x,a6,i5,1x,''"'',a,''"'',3i3,6(1x,e16.8),i3,',              \n
///>      max(nwbuf,1),'(1x,e16.8))'                                        \n
/// write(lunlist,fmt)                                                      \n
///+      context, rname, itmed, name, nmat, isvol, ifield, fieldm,         \n
///+      tmaxfd, stemax, deemax, epsil, stmin,                             \n
///+      nwbuf, (ubuf(k),k=1,nwbuf)                                        \n

  G4String context("----");
  G4String rname("GSTMED");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << std::setw(5) << itmed << space
	  << '"' << name << '"' 
	  << std::setw(3) << nmat
	  << std::setw(3) << isvol
	  << std::setw(3) << ifield << space
          << std::setw(16) << std::setprecision(8) << fieldm << space
          << std::setw(16) << std::setprecision(8) << tmaxfd << space
          << std::setw(16) << std::setprecision(8) << stemax << space
          << std::setw(16) << std::setprecision(8) << deemax << space
          << std::setw(16) << std::setprecision(8) << epsil << space
          << std::setw(16) << std::setprecision(8) << stmin << space
	  << std::setw(3) << nwbuf;
  for (G4int i=0; i<nwbuf; i++)	      
    fOutFile << space << std::setw(16) << std::setprecision(8) << ubuf[i];
  fOutFile << G4endl;
}	  

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGstpar(
               G4int itmed, G4String param, G4double parval)
{	       
/// From fortran (g3routines.F)                                             \n
/// write(lunlist,                                                          \n
///+     '(a4,1x,a6,i5,1x,a4,(1x,e16.8))')                                  \n
///+     context, rname, itmed, chpar, parval                               \n

  G4String context("----");
  G4String rname("GSTPAR");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   << space
	  << std::setw(5) << itmed << space 
	  << param   << space
          << std::setw(16) << std::setprecision(8) << parval
          << G4endl;
}	      

//_____________________________________________________________________________
void TG4GeometryOutputManager::WriteGgclos()
{
/// Write GGCLOS token

  G4String context("----");
  G4String rname("GGCLOS");
  G4String space(" "); 
  fOutFile << context << space 
          << rname   
	  << G4endl;
}	  
