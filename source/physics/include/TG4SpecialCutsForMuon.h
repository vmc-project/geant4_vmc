// $Id: TG4SpecialCutsForMuon.h,v 1.1 2002/06/20 11:57:45 hristov Exp $
// Category: physics
//
// Author: I. Hrivnacova
//
// Class TG4SpecialCutsForMuon
// ---------------------------
// Special process that activates kinetic energy cuts
// for muon.

#ifndef TG4_SPECIAL_CUTS_FOR_MUON_H
#define TG4_SPECIAL_CUTS_FOR_MUON_H

#include "TG4VSpecialCuts.h"

class TG4Limits;

class G4Track;

class TG4SpecialCutsForMuon: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForMuon(const G4String& processName = "specialCutForMuon");
    // --> protected
    // TG4SpecialCutsForMuon();		   
    virtual ~TG4SpecialCutsForMuon();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForMuon();		   
};

#endif //TG4_SPECIAL_CUTS_FOR_MUON_H



