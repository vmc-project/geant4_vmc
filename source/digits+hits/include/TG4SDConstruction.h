// $Id: TG4SDConstruction.h,v 1.2 2004/11/10 11:39:27 brun Exp $
/// \ingroup digits_hits
//
/// \class TG4SDConstruction
/// \brief Sensitive detector construction
/// 
/// Class for construction of user sensitive detectors.
/// Constructed sensitive detectors inherit from 
/// TG4VSensitiveDetector (see TG4VSensitiveDetector.h description);
/// all cloned logical volumes (which a single G3 volume correspond to)
/// share the same sensitive detector instance.
///
/// Author: I.Hrivnacova

#ifndef TG4_SD_CONSTRUCTION_H
#define TG4_SD_CONSTRUCTION_H

#include "TG4Verbose.h"

class G4LogicalVolume;

class TG4SDConstruction : public TG4Verbose

{
  public:
    TG4SDConstruction();
    virtual ~TG4SDConstruction();

    // methods
    virtual void Construct();

  private:
    // methods
    void CreateSD(G4LogicalVolume* lv) const;
};

#endif //TG4_SD_CONSTRUCTION_H

