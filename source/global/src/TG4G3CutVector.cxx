// $Id: TG4G3CutVector.cxx,v 1.4 2004/11/10 11:39:28 brun Exp $
// Category: global
//
// Class TG4G3CutVector
// --------------------
// See the class description in the header file.
//
// Author: I. Hrivnacova

#include "TG4G3CutVector.h"
#include "TG4G3Defaults.h"

#include <G4Track.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>

#if __GNUC__ >= 3
#include <sstream>
#else
#include <strstream>
#endif

const G4double  TG4G3CutVector::fgkDCUTEOff  = 10. * TeV;
const G4double  TG4G3CutVector::fgkDCUTMOff  = 10. * TeV;
const G4double  TG4G3CutVector::fgkTolerance =  1. * keV;
TG4StringVector TG4G3CutVector::fgCutNameVector;

//_____________________________________________________________________________
TG4G3CutVector::TG4G3CutVector()
  : fCutVector(),
    fDeltaRaysOn(true)
{
  // fill name vector
  if (fgCutNameVector.size() == 0) FillCutNameVector(); 

  // initialize fCutVector 
  for (G4int i=0; i<kTOFMAX; i++) fCutVector.push_back(0.); 
  fCutVector.push_back(DBL_MAX);
}

//_____________________________________________________________________________
TG4G3CutVector::TG4G3CutVector(const TG4G3CutVector& right)
  : fCutVector(right.fCutVector.size()),
    fDeltaRaysOn(right.fDeltaRaysOn)
{
  // copy stuff
  *this = right;
}

//_____________________________________________________________________________
TG4G3CutVector::~TG4G3CutVector() {
//
}

//
// operators
//

//_____________________________________________________________________________
TG4G3CutVector& TG4G3CutVector::operator=(const TG4G3CutVector& right)
{
  // check assignement to self
  if (this == &right) return *this;

  // copy fCutVector 
  for (G4int i=0; i<kNoG3Cuts; i++) fCutVector[i] = right.fCutVector[i];

  fDeltaRaysOn = right.fDeltaRaysOn;
  
  return *this;   
}  

//_____________________________________________________________________________
G4double TG4G3CutVector::operator[](G4int index) const 
{
//
  if (index < kNoG3Cuts)
    return fCutVector[index];
  else {
    TG4Globals::Exception(
      "TG4G3CutVector", "operator[]", "Index out of the vector scope");
    return 0.;  
  }    
}  

//
// private methods
//

//_____________________________________________________________________________
void TG4G3CutVector::FillCutNameVector()
{
/// Define fCutNameVector.

  fgCutNameVector.push_back("CUTGAM");
  fgCutNameVector.push_back("CUTELE");
  fgCutNameVector.push_back("CUTNEU");
  fgCutNameVector.push_back("CUTHAD");
  fgCutNameVector.push_back("CUTMUO");
  fgCutNameVector.push_back("BCUTE");
  fgCutNameVector.push_back("BCUTM"); 
  fgCutNameVector.push_back("DCUTE");
  fgCutNameVector.push_back("DCUTM");
  fgCutNameVector.push_back("PPCUTM");
  fgCutNameVector.push_back("TOFMAX");
  fgCutNameVector.push_back("NONE");
}

//
// public methods
//

//_____________________________________________________________________________
TG4G3Cut TG4G3CutVector::GetCut(const G4String& cutName)
{
/// Retrieve corresponding TG4G3Cut constant from the cutName.

  if      (cutName == fgCutNameVector[kCUTGAM]) return kCUTGAM; 
  else if (cutName == fgCutNameVector[kCUTELE]) return kCUTELE;
  else if (cutName == fgCutNameVector[kCUTNEU]) return kCUTNEU;
  else if (cutName == fgCutNameVector[kCUTHAD]) return kCUTHAD;
  else if (cutName == fgCutNameVector[kCUTMUO]) return kCUTMUO;
  else if (cutName == fgCutNameVector[kBCUTE])  return kBCUTE; 
  else if (cutName == fgCutNameVector[kBCUTM])  return kBCUTM; 
  else if (cutName == fgCutNameVector[kDCUTE])  return kDCUTE;
  else if (cutName == fgCutNameVector[kDCUTM])  return kDCUTM;
  else if (cutName == fgCutNameVector[kPPCUTM]) return kPPCUTM;
  else if (cutName == fgCutNameVector[kTOFMAX]) return kTOFMAX;
  else return kNoG3Cuts;
}

//_____________________________________________________________________________
const G4String& TG4G3CutVector::GetCutName(TG4G3Cut cut)
{
/// Return name of a specified cut.

  // fill name vector
  if (fgCutNameVector.size() == 0) TG4G3CutVector::FillCutNameVector(); 

  return fgCutNameVector[cut];
}  

//_____________________________________________________________________________
void TG4G3CutVector::SetCut(TG4G3Cut cut, G4double cutValue)
{
/// Set the cutValue for the specified cut.

  if (cut>=kNoG3Cuts) {
    TG4Globals::Exception(
      "TG4G3CutVector", "SetG3Cut", "Inconsistent cut.");
  }

  fCutVector[cut] = cutValue;          
}

//_____________________________________________________________________________
void TG4G3CutVector::SetG3Defaults()
{
/// Set G3 default values for all cuts.

  for (G4int i=0; i<kNoG3Cuts; i++)
    fCutVector[i] = TG4G3Defaults::Instance()->CutValue(i);
}

//_____________________________________________________________________________
G4bool TG4G3CutVector::Update(const TG4G3CutVector& vector)
{
/// Update only those values that have not yet been set ( =0.)
/// with given vector. 
/// Return true if some value was modified.

  G4bool result = false;

  for (G4int i=0; i<kNoG3Cuts; i++) 
    if (fCutVector[i] < fgkTolerance ) {
       fCutVector[i] = vector[i];
       result = true;
    }
    
  return result;     
}

//_____________________________________________________________________________
G4String TG4G3CutVector::Format() const
{
/// Format the output into a string.

#if __GNUC__ >= 3
  std::ostringstream tmpStream;
#else  
  std::strstream tmpStream;
#endif

  tmpStream << "  G3 cut vector:" << G4endl; 
  if (fDeltaRaysOn)
    tmpStream << "    Delta rays On" << G4endl; 
  else  
    tmpStream << "    Delta rays Off" << G4endl; 
  
  // energy cuts
  for (G4int i=0; i<kTOFMAX; i++) {

    tmpStream << "    " << fgCutNameVector[i] << " cut value (MeV): ";

    if (i == kDCUTE && !fDeltaRaysOn)
      tmpStream << fgkDCUTEOff/MeV << G4endl;
    else if (i == kDCUTM && !fDeltaRaysOn)
      tmpStream << fgkDCUTMOff/MeV << G4endl;
    else                  
      tmpStream << fCutVector[i]/MeV << G4endl;
  }      

  // time cut
  tmpStream << "    " << fgCutNameVector[kTOFMAX] << " cut value (s):   "
            << fCutVector[kTOFMAX]/s << G4endl;

  return tmpStream.str();
}           

//_____________________________________________________________________________
void TG4G3CutVector::Print() const
{
/// Print the cuts.

  G4cout << Format();
}           

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForGamma(const G4Track& track) const
{
/// Return the cut value for gamma.
/// (Cut is not applied for "opticalphoton" 
///  as it is treated in G4 as a particle different 
///  from "gamma" in G4.)

  const G4VProcess* kpCreatorProcess = track.GetCreatorProcess();
  G4String processName = "";
  if (kpCreatorProcess) processName = kpCreatorProcess->GetProcessName();

  if ((processName == "eBrem") || (processName == "IeBrem")) {
    return fCutVector[kBCUTE];
  }     
  else if ((processName == "MuBrems") || (processName == "IMuBrems")) { 
           //(processName == "//hBrems")|| (processName == "//IhBrems") 
           // hadron Brehmstrahlung is not defined in G4
    return fCutVector[kBCUTM];
  }
  else {
    return fCutVector[kCUTGAM];
  }
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForElectron(const G4Track& track) const
{
/// Return the cut value for e-. 

  const G4VProcess* kpCreatorProcess = track.GetCreatorProcess();
  G4String processName = "";
  if (kpCreatorProcess) processName = kpCreatorProcess->GetProcessName();

  if ((processName == "eIoni") || (processName == "IeIoni")) {
    // delta rays by e-, e+

    if (fDeltaRaysOn) 
      return fCutVector[kDCUTE];
    else 
      return fgkDCUTEOff;
  }
  else if ((processName == "MuIoni") || (processName == "IMuIoni") ||
           (processName == "hIoni")  || (processName == "IhIoni")) {
    // delta rays by other particles (mu, hadron)

    if (fDeltaRaysOn) 
      return fCutVector[kDCUTM];
    else 
      return fgkDCUTMOff;
  }
  else if (processName == "MuPairProd") {
    //direct pair production by muons

    return fCutVector[kPPCUTM];
  }  
  else {   
    return fCutVector[kCUTELE];
  }
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForEplus(const G4Track& track) const
{
/// Return the cut value for e+. 

  const G4VProcess* kpCreatorProcess = track.GetCreatorProcess();
  G4String processName = "";
  if (kpCreatorProcess) processName = kpCreatorProcess->GetProcessName();

  if (processName == "MuPairProd") {
    //direct pair production by muons
    return fCutVector[kPPCUTM];
  }   
  else
    return fCutVector[kCUTELE];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForChargedHadron(const G4Track& /*track*/) const
{
/// Return the cut value for charged hadron.

  return fCutVector[kCUTHAD];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForNeutralHadron(const G4Track& /*track*/) const
{
/// Return the cut value for neutral hadron.

  return fCutVector[kCUTNEU];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForMuon(const G4Track& /*track*/) const
{
/// Return the cut value for neutral muon.

  return fCutVector[kCUTMUO];
}

//_____________________________________________________________________________
G4double TG4G3CutVector::GetMinEkineForOther(const G4Track& /*track*/) const
{
/// Return 0.

  return 0.;
}
//_____________________________________________________________________________
G4bool TG4G3CutVector::IsCut() const
{
/// Return true if any of cuts is set.

  for (G4int i=0; i<kNoG3Cuts; i++) 
    if (fCutVector[i] > fgkTolerance )  return true;
       
  return false;  
}  
