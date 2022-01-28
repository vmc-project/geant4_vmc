#ifndef OpNovicePostDetConstruction_h
#define OpNovicePostDetConstruction_h 1

/// \file OpNovicePostDetConstruction.hh
/// \brief Definition of OpNovicePostDetConstruction class
///
/// Geant4 OpNovice example adapted to G4Root test
/// Example of post detector construction user class.
///
/// \author A. Gheata; CERN

#include "TG4RootDetectorConstruction.h"

class TPolyLine3D;

/// This class defines optical properties which are normally defined in the 
/// user detector construction class and connects them to the GEANT4 geometry
/// objects which are internally created based on the ROOT geometry.

class OpNovicePostDetConstruction : public TVirtualUserPostDetConstruction
{
private:
   TObjArray            *fTracks;  // Array of tracks
   TPolyLine3D          *fCurrent; // Current track
   
   OpNovicePostDetConstruction();
   static OpNovicePostDetConstruction *fgInstance; // Self pointer
public:
   virtual ~OpNovicePostDetConstruction();

   static OpNovicePostDetConstruction *GetInstance();

   void                  NewTrack(Double_t x, Double_t y, Double_t z);
   void                  AddPoint(Double_t x, Double_t y, Double_t z);
   void                  WriteTracks(const char *filename);
   
   virtual void          Initialize(TG4RootDetectorConstruction *dc);
};
#endif
  

