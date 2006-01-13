// $Id: TG4ModularPhysicsList.h,v 1.8 2005/11/18 21:29:35 brun Exp $
/// \ingroup physics
//
/// \class TG4ModularPhysicsList
/// \brief The Geant4 VMC modular physics list.
///
/// The default physics processes and particles are created
/// using the G4VPhysicsCreator derived classes
/// and registered to this physics list in the function Configure().
/// User can modify the default selection of physics
/// via TG4PhysicsListOptions class.
///
/// Author: I. Hrivnacova

#ifndef TG4_MODULAR_PHYSICS_LIST_H
#define TG4_MODULAR_PHYSICS_LIST_H

#include "TG4Verbose.h"
#include "TG4PhysicsListMessenger.h"
#include "TG4PhysicsListOptions.h"

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class TG4PhysicsConstructorOptical;

class TG4ModularPhysicsList: public G4VModularPhysicsList,
                             public TG4Verbose
{
  public:
    TG4ModularPhysicsList(const TG4PhysicsListOptions& options);
    TG4ModularPhysicsList();
    // --> protected
    // TG4ModularPhysicsList(const TG4ModularPhysicsList& right);
    virtual ~TG4ModularPhysicsList();
  
    // methods
    virtual void ConstructProcess();
    virtual void SetCuts();
    virtual G4int VerboseLevel() const;
    virtual void  VerboseLevel(G4int level);
    void PrintAllProcesses() const;
    void DumpAllProcesses() const;

    // set methods
    void SetProcessActivation();
    void SetRangeCut(Double_t value);
    void SetMaxNumPhotonsPerStep(Int_t maxNumPhotons);
    
  protected:
    TG4ModularPhysicsList(const TG4ModularPhysicsList& right);

    // operators
    TG4ModularPhysicsList& operator=(const TG4ModularPhysicsList& right);

    // static data members
    static const G4double  fgkDefaultCutValue; //default cut value

    // data members
    TG4PhysicsListMessenger        fMessenger; //messenger
    TG4PhysicsConstructorOptical*  fPhysicsConstructorOptical;
    TG4PhysicsListOptions          fOptions;

  private:
    // methods
    void Configure();
    void SetProcessActivation(G4ProcessManager* processManager,
                              G4int processId, G4bool activation);
    void SetSpecialControlsActivation();
    void SetSpecialCutsActivation();
};

#endif //TG4_MODULAR_PHYSICS_LIST_H

