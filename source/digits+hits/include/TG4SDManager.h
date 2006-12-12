// $Id: TG4SDManager.h,v 1.5 2006/01/13 16:59:38 brun Exp $
/// \ingroup digits_hits
//
/// \class TG4SDManager
/// \brief Geant4 implementation of the TVirtualMC interface methods                    
/// for access to Geant4 geometry related with sensitive detectors.
///
/// Author: I. Hrivnacova

#ifndef TG4_SD_MANAGER_H
#define TG4_SD_MANAGER_H

#include <globals.hh>

#include <Rtypes.h>

class TG4SDServices;
class TG4SDConstruction;

class TG4SDManager
{
  public:
    TG4SDManager();
    virtual ~TG4SDManager();

    // static methods
    static TG4SDManager* Instance();

    // methods
    void Initialize();
    
    // TVirtualMC methods
    Int_t VolId(const Text_t* volName) const;                
    const char* VolName(Int_t id) const;
    Int_t MediumId(const Text_t* mediumName) const;
    Int_t NofVolumes() const; 
    Int_t NofVolDaughters(const char* volName) const;
    const char*  VolDaughterName(const char* volName, Int_t i) const;
    Int_t        VolDaughterCopyNo(const char* volName, Int_t i) const;
    Int_t VolId2Mate(Int_t volumeId) const;

    // get methods
    TG4SDConstruction* GetSDConstruction() const;

  private:
    TG4SDManager(const TG4SDManager& right);
    TG4SDManager& operator=(const TG4SDManager& right);

    // static data members
    static TG4SDManager* fgInstance;   //this instance
    
    // data members
    TG4SDConstruction*  fSDConstruction; //sensitive detectors construction
    TG4SDServices*      fSDServices;     //services related with sensitive
                                         //detectors

};

// inline methods

inline TG4SDManager* TG4SDManager::Instance() { 
  /// Return this instance
  return fgInstance; 
}

inline TG4SDConstruction* TG4SDManager::GetSDConstruction() const { 
  /// Return sensitive detctor construction
  return fSDConstruction; 
}

#endif //TG4_SD_MANAGER_H

