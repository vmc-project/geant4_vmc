// $Id: TG4DetConstruction.h,v 1.10 2006/04/12 10:37:23 brun Exp $
/// \ingroup geometry
//
/// \class TG4DetConstruction
/// \brief Detector construction for building geometry using 
/// TVirtualMCApplication.
///
/// Author: I. Hrivnacova

#ifndef TG4_DET_CONSTRUCTION_H
#define TG4_DET_CONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;

class TG4DetConstruction : public G4VUserDetectorConstruction
{
  public:
    TG4DetConstruction();
    virtual ~TG4DetConstruction();

    // methods
    virtual G4VPhysicalVolume* Construct();

  private:    
    TG4DetConstruction(const TG4DetConstruction& right);
    TG4DetConstruction& operator=(const TG4DetConstruction& right);
}; 

#endif //TG4_DET_CONSTRUCTION_H

