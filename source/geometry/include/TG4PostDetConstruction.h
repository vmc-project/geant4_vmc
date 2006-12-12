// $Id: TG4PostDetConstruction.h,v 1.10 2006/04/12 10:37:23 brun Exp $
/// \ingroup geometry
//
/// \class TG4PostDetConstruction
/// \brief The post detector construction used with G4Root navigation
///
/// Author: I. Hrivnacova

#ifndef TG4_POST_DET_CONSTRUCTION_H
#define TG4_POST_DET_CONSTRUCTION_H

#include <TG4RootDetectorConstruction.h>

class TG4PostDetConstruction;

class TG4PostDetConstruction : public TVirtualUserPostDetConstruction
{
  public:
    TG4PostDetConstruction();
    virtual ~TG4PostDetConstruction();

    // methods
    virtual void Initialize(TG4RootDetectorConstruction *dc);

  private:    
    TG4PostDetConstruction(const TG4PostDetConstruction& right);
    TG4PostDetConstruction& operator=(const TG4PostDetConstruction& right);
}; 

#endif //TG4_DET_CONSTRUCTION_H

