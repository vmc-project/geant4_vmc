// $Id: Ex02ChamberParameterisation.cxx,v 1.1.1.1 2002/06/16 15:57:36 hristov Exp $
//
// Geant4 ExampleN02 adapted to Virtual Monte Carlo 
//
// Id: Ex02ChamberParameterisation.cc,v 1.7 2002/01/09 17:24:09 ranjard Exp 
// GEANT4 tag Name: geant4-04-00-patch-02 
//
// by Ivana Hrivnacova, 21.4.2002

#include <TError.h>

#include "Ex02ChamberParameterisation.h"

ClassImp(Ex02ChamberParameterisation)

//_____________________________________________________________________________
Ex02ChamberParameterisation::Ex02ChamberParameterisation(
                                 Int_t    NoChambers, 
                                 Double_t startZ,      
                                 Double_t spacingZ,   
                                 Double_t widthChamber, 
                                 Double_t lengthInitial, 
                                 Double_t lengthFinal )
  : TObject()				 
{
   fNoChambers =  NoChambers; 
   fStartZ     =  startZ; 
   fHalfWidth  =  widthChamber*0.5;
   fSpacing    =  spacingZ;
   fHalfLengthFirst = 0.5 * lengthInitial; 
   // fHalfLengthLast = lengthFinal;
   if( NoChambers > 0 ){
      fHalfLengthIncr =  0.5 * (lengthFinal-lengthInitial)/NoChambers;
      if (spacingZ < widthChamber) {
        Fatal("Ex02ChamberParameterisation",
              "Ex02ChamberParameterisation construction: Width>Spacing");
      }
   }
   
}

//_____________________________________________________________________________
Ex02ChamberParameterisation::~Ex02ChamberParameterisation()
{}


//_____________________________________________________________________________
void Ex02ChamberParameterisation::ComputeTransformation (
                                     Int_t copyNo, Double_t* position) const
{
  position[0] = 0.;
  position[1] = 0.;
  position[2] = fStartZ + (copyNo+1) * fSpacing;
}

//_____________________________________________________________________________
void Ex02ChamberParameterisation::ComputeDimensions(
                                      Int_t copyNo, Double_t* dimension) const
{
  Double_t  halfLength= fHalfLengthFirst + copyNo * fHalfLengthIncr;
  dimension[0] = halfLength;
  dimension[1] = halfLength;
  dimension[2] = fHalfWidth;
}
