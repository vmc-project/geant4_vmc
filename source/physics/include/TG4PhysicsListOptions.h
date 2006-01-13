// $Id: TG4PhysicsListOptions.h,v 1.8 2005/11/18 21:29:35 brun Exp $
/// \ingroup physics
//
/// \class TG4PhysicsListOptions
/// \brief The options for selection of provided physics constructors
///
/// Default selection:
/// - EM physics, Muon physics, Step Limiter - activated
/// - Hadron physics, Special cuts, Special controls - not activated
///
/// Author: I. Hrivnacova

#ifndef TG4_PHYSICS_LIST_OPTIONS_H
#define TG4_PHYSICS_LIST_OPTIONS_H

#include <Rtypes.h>

class TG4PhysicsListOptions
{
  public:
    TG4PhysicsListOptions();
    TG4PhysicsListOptions(const TG4PhysicsListOptions& right);
    virtual ~TG4PhysicsListOptions();
  
    // operators
    TG4PhysicsListOptions& operator=(const TG4PhysicsListOptions& right);

    // set methods
    void SetEMPhysics(Bool_t value);
    void SetMuonPhysics(Bool_t value);
    void SetHadronPhysics(Bool_t value);
    void SetOpticalPhysics(Bool_t value);
    void SetSpecialCutsPhysics(Bool_t value);
    void SetSpecialControlsPhysics(Bool_t value);
    void SetStepLimiterPhysics(Bool_t value);
    
    // get methods
    Bool_t GetEMPhysics() const;
    Bool_t GetMuonPhysics() const;
    Bool_t GetHadronPhysics() const;
    Bool_t GetOpticalPhysics() const;
    Bool_t GetSpecialCutsPhysics() const;
    Bool_t GetSpecialControlsPhysics() const;
    Bool_t GetStepLimiterPhysics() const;
     
  private:
    // data members
    Bool_t  fSetEMPhysics;          //electromagnetic physics control
    Bool_t  fSetMuonPhysics;        //muon physics control
    Bool_t  fSetHadronPhysics;      //hadron physics control
    Bool_t  fSetOpticalPhysics;     //optical physics control
    Bool_t  fSetSpecialCutsPhysics; //special cuts process control 
    Bool_t  fSetSpecialControlsPhysics;//special controls process control
    Bool_t  fSetStepLimiterPhysics; //step limiter process control
};

#endif //TG4_PHYSICS_LIST_OPTIONS_H

