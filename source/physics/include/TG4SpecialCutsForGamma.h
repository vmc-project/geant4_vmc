// $Id: TG4SpecialCutsForGamma.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup physics
//
/// \class TG4SpecialCutsForGamma
/// 
/// Special process that activates kinetic energy cuts
/// for gamma.
///
/// Author: I. Hrivnacova

#ifndef TG4_SPECIAL_CUTS_FOR_GAMMA_H
#define TG4_SPECIAL_CUTS_FOR_GAMMA_H

#include "TG4VSpecialCuts.h"

class TG4Limits;

class G4Track;

class TG4SpecialCutsForGamma: public TG4VSpecialCuts
{
  public:
    TG4SpecialCutsForGamma(const G4String& processName = "specialCutForGamma");
    // --> protected
    // TG4SpecialCutsForGamma();		   
    virtual ~TG4SpecialCutsForGamma();

    // methods
    virtual G4double GetMinEkine(const TG4Limits& limits,
                                 const G4Track& track) const;
			    
  protected:
    TG4SpecialCutsForGamma();		   
};

#endif //TG4_SPECIAL_CUTS_FOR_GAMMA_H



