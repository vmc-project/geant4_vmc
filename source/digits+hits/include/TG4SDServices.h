// $Id: TG4SDServices.h,v 1.5 2005/09/01 10:04:32 brun Exp $
/// \ingroup digits_hits
//
/// \class TG4SDServices
/// \brief Sensitive detectors services
///
/// The class provides service methods for accessing to Geant4 geometry,
/// namely using TVirtualMC volumes identifiers
/// (implemented via TG4SensitiveDetector instances).
///
/// Author: I. Hrivnacova

#ifndef TG4_SD_SERVICES_H
#define TG4_SD_SERVICES_H

#include <globals.hh>

#include <Rtypes.h>

class TG4SensitiveDetector;

class G4LogicalVolume;
class G4VSensitiveDetector;

class TG4SDServices
{
  public:
    TG4SDServices();
    virtual ~TG4SDServices();

    // static methods
    static TG4SDServices* Instance();

    // methods
    void PrintStatistics(G4bool open, G4bool close) const;

    // set methods
    void SetIsStopRun(G4bool stopRun);

    // get methods
          // volume IDs conversions
    G4int GetVolumeID(const G4String& volumeName) const;
    G4int GetVolumeID(G4LogicalVolume* volume) const;
    G4String         GetVolumeName(G4int volumeId) const;
    G4LogicalVolume* GetLogicalVolume(G4int volumeId) const;   
    G4int            GetMediumId(G4int volumeId) const;
    G4bool  GetIsStopRun() const; 
          // SDs
    Int_t NofSensitiveDetectors() const; 
    TG4SensitiveDetector* GetSensitiveDetector(G4VSensitiveDetector* sd) const;  

          // Daughters
    Int_t NofVolDaughters(const char* volName) const;
    const char*  VolDaughterName(const char* volName, Int_t i) const;
    Int_t        VolDaughterCopyNo(const char* volName, Int_t i) const;

  private:
    TG4SDServices(const TG4SDServices& right);
    TG4SDServices& operator=(const TG4SDServices& right);

    // static data members
    static TG4SDServices* fgInstance;   //this instance

    G4bool  fIsStopRun; // keeps info about run stopping by user  
};

// inline methods

inline TG4SDServices* TG4SDServices::Instance() { 
  /// Returns this instance.
  return fgInstance; 
}

inline void TG4SDServices::SetIsStopRun(G4bool isStopRun) { 
  /// Sets flag for notifying about stopping run by a user.
  fIsStopRun = isStopRun; 
}

inline G4bool TG4SDServices::GetIsStopRun() const { 
  /// Returns flag for notifying about stopping run by a user.
  return fIsStopRun; 
}

#endif //TG4_SD_SERVICES_H

