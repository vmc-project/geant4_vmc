// $Id: TG4SpecialCutsForElectron.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup physics
//
/// \class TG4SpecialCutsForElectron
/// 
/// Special process that activates kinetic energy cuts
/// for e-.
///
/// Author: I. Hrivnacova

#ifndef TG4_SPECIAL_CUTS_FOR_ELECTRON_H
#define TG4_SPECIAL_CUTS_FOR_ELECTRON_H

#include "TG4VSpecialCuts.h"

class TG4Limits;

class G4Track;

class TG4SpecialCutsForElectron: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForElectron(const G4String& processName
                                              = "specialCutForElectron");
    // --> protected
    // TG4SpecialCutsForElectron();		   
    virtual ~TG4SpecialCutsForElectron();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForElectron();		   
};

#endif //TG4_SPECIAL_CUTS_FOR_ELECTRON_H



