//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2015 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file  Garfield/src/GarfieldPhysics.cxx
/// \brief Definition of the GarfieldPhysics class 
///
/// Garfield garfieldpp example adapted to Virtual Monte Carlo.
/// This class is imported from garfieldpp example with no modification
/// in the code.
///
/// \date 28/10/2015
/// \author D. Pheiffer, CERN

#include "GarfieldPhysics.h"

#include "TGeoManager.h"
#include "TGeoBBox.h"

// I.H.
//#include "TGDMLParse.h"
//#include "G4SystemOfUnits.hh"

GarfieldPhysics* GarfieldPhysics::fGarfieldPhysics = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GarfieldPhysics* GarfieldPhysics::GetInstance() {
	if (!fGarfieldPhysics) {
		fGarfieldPhysics = new GarfieldPhysics();
	}
	return fGarfieldPhysics;
}

void GarfieldPhysics::Dispose() {
	delete fGarfieldPhysics;
	fGarfieldPhysics = 0;
}

GarfieldPhysics::GarfieldPhysics() {
	fMapParticlesEnergy = new MapParticlesEnergy();
	fSecondaryElectrons = new std::vector<GarfieldElectron*>();
	fMediumMagboltz = 0;
	fSensor = 0;
	fAvalanche = 0;
	fDrift = 0;
	fComponentAnalyticField = 0;
	fTrackHeed = 0;
	fGeoManager = 0;
	fGeometryRoot = 0;
	fGeometrySimple = 0;
	fTube = 0;
	createSecondariesInGeant4 = false;

}

GarfieldPhysics::~GarfieldPhysics() {
	delete fMapParticlesEnergy;
	DeleteSecondaryElectrons();
	delete fSecondaryElectrons;
	delete fMediumMagboltz;
	delete fSensor;
	delete fAvalanche;
	delete fDrift;
	delete fComponentAnalyticField;
	delete fTrackHeed;
	delete fGeometryRoot;
	delete fGeometrySimple;

	std::cout << "Deconstructor GarfieldPhysics" << std::endl;
}

void GarfieldPhysics::AddParticleName(const std::string particleName,
		double ekin_min_keV, double ekin_max_keV) {
	if (ekin_min_keV >= ekin_max_keV) {
		std::cout << "Ekin_min=" << ekin_min_keV
				<< " keV is larger than Ekin_max=" << ekin_max_keV << " keV"
				<< std::endl;
		return;
	}
	std::cout << "Garfield model is applicable for G4Particle " << particleName
			<< " between " << ekin_min_keV << " keV and " << ekin_max_keV
			<< " keV" << std::endl;
	fMapParticlesEnergy->insert(
			std::make_pair(particleName,
					std::make_pair(ekin_min_keV, ekin_max_keV)));

}

bool GarfieldPhysics::FindParticleName(std::string name) {
	MapParticlesEnergy::iterator it;
	it = fMapParticlesEnergy->find(name);
	if (it != fMapParticlesEnergy->end()) {
		return true;
	}
	return false;
}

bool GarfieldPhysics::FindParticleNameEnergy(std::string name,
		double ekin_keV) {
	MapParticlesEnergy::iterator it;
	it = fMapParticlesEnergy->find(name);
	if (it != fMapParticlesEnergy->end()) {
		EnergyRange_keV range = it->second;
		if (range.first <= ekin_keV && range.second >= ekin_keV) {
			return true;
		}
	}
	return false;
}

void GarfieldPhysics::InitializePhysics() {

	fMediumMagboltz = new Garfield::MediumMagboltz();

	fMediumMagboltz->SetComposition("ar", 70., "co2", 30.);
	fMediumMagboltz->SetTemperature(293.15);
	fMediumMagboltz->SetPressure(760.);
	fMediumMagboltz->EnableDebugging();
	fMediumMagboltz->Initialise(true);
	fMediumMagboltz->DisableDebugging();
	// Set the Penning transfer efficiency.
	const double rPenning = 0.57;
	const double lambdaPenning = 0.;
	fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
	// Load the ion mobilities.
	fMediumMagboltz->LoadIonMobility("IonMobility_Ar+_Ar.txt");
	fMediumMagboltz->LoadGasFile("ar_70_co2_30_1000mbar.gas");

	fSensor = new Garfield::Sensor();
	fDrift = new Garfield::AvalancheMC();
	fAvalanche = new Garfield::AvalancheMicroscopic();
	fComponentAnalyticField = new Garfield::ComponentAnalyticField();

	CreateGeometry();

	fDrift->SetSensor(fSensor);
	fAvalanche->SetSensor(fSensor);

	fTrackHeed = new Garfield::TrackHeed();
	fTrackHeed->SetSensor(fSensor);

	fTrackHeed->EnableDeltaElectronTransport();
}

void GarfieldPhysics::CreateGeometry() {
	// Wire radius [cm]
	const double rWire = 25.e-4;
	// Outer radius of the tube [cm]
	const double rTube = 1.451;
	// Half-length of the tube [cm]
	const double lTube = 10.;

	fGeometrySimple = new Garfield::GeometrySimple();
	// Make a tube (centered at the origin, inner radius: 0, outer radius: rTube).
	fTube = new Garfield::SolidTube(0., 0., 0, rWire, rTube, lTube);
	// Add the solid to the geometry, together with the medium inside.
	fGeometrySimple->AddSolid(fTube, fMediumMagboltz);
	fComponentAnalyticField->SetGeometry(fGeometrySimple);

	// Voltages
	const double vWire = 1000.;
	const double vTube = 0.;
	// Add the wire in the center.
	fComponentAnalyticField->AddWire(0., 0., 2 * rWire, vWire, "w");
	// Add the tube.
	fComponentAnalyticField->AddTube(rTube, vTube, 0, "t");

	fSensor->AddComponent(fComponentAnalyticField);

}

void GarfieldPhysics::DoIt(std::string particleName, double ekin_keV,
		double time, double x_cm, double y_cm, double z_cm, double dx,
		double dy, double dz) {

	DeleteSecondaryElectrons();

	// Wire radius [cm]
	const double rWire = 25.e-4;
	// Outer radius of the tube [cm]
	const double rTube = 1.45;
	// Half-length of the tube [cm]
	const double lTube = 10.;

	double eKin_eV = ekin_keV * 1000;

	double xc = 0., yc = 0., zc = 0., tc = 0.;
	// Number of electrons produced in a collision
	int nc = 0;
	// Energy loss in a collision
	double ec = 0.;
	// Dummy variable (not used at present)
	double extra = 0.;
	// Total energy loss along the track
	double esum = 0.;
	// Total number of electrons produced along the track
	int nsum = 0;

	fEnergyDeposit = 0;
	fAvalancheSize = 0;
	fGain = 0;

	fTrackHeed->SetParticle(particleName);
	fTrackHeed->SetKineticEnergy(eKin_eV);
	fTrackHeed->NewTrack(x_cm, y_cm, z_cm, time, dx, dy, dz);

	while (fTrackHeed->GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
		if (zc < lTube && zc > -lTube && sqrt(xc * xc + yc * yc) < rTube) {
			esum += ec;
			nsum += nc;

			for (int cl = 0; cl < nc; cl++) {
				double xe, ye, ze, te;
				double ee, dxe, dye, dze;
				fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);

				if (ze < lTube && ze > -lTube
						&& sqrt(xe * xe + ye * ye) < rTube) {
					if (createSecondariesInGeant4) {
						double newTime = te;
						if (newTime < time) {
							newTime += time;
						}
						fSecondaryElectrons->push_back(
								new GarfieldElectron(ee, newTime, xe, ye, ze,
										dxe, dye, dze));
					}

					fDrift->DriftElectron(xe, ye, ze, te);

					double xe1, ye1, ze1, te1;
					double xe2, ye2, ze2, te2;

					int status;
					fDrift->GetElectronEndpoint(0, xe1, ye1, ze1, te1, xe2, ye2,
							ze2, te2, status);

					if (0 < xe2 && xe2 < rWire) {
						xe2 += 2 * rWire;
					}
					if (0 > xe2 && xe2 > -rWire) {
						xe2 += -2 * rWire;
					}
					if (0 < ye2 && ye2 < rWire) {
						ye2 += 2 * rWire;
					}
					if (0 > ye2 && ye2 > -rWire) {
						ye2 += -2 * rWire;
					}

					double e2 = 0.1;
					fAvalanche->AvalancheElectron(xe2, ye2, ze2, te2, e2, 0, 0,
							0);

					int ne = 0, ni = 0;
					fAvalanche->GetAvalancheSize(ne, ni);
					fAvalancheSize += ne;

				} else {
					std::cout << "Info: electron outside gas at (" << xe << ","
							<< ye << "," << ze << ")" << std::endl;
				}
			}
		} else {
			std::cout << "Info: cluster outside gas  at (" << xc << "," << yc
					<< "," << zc << ")" << std::endl;
		}
	}
	fGain = fAvalancheSize / nsum;
	fEnergyDeposit = esum;

	std::cout << "esum " << esum / 1000 << " keV  -  nsum " << nsum
			<< " - nsum_avalanche " << fAvalancheSize << " - gain " << fGain
			<< std::endl;

}

std::vector<GarfieldElectron*>* GarfieldPhysics::GetSecondaryElectrons() {
	return fSecondaryElectrons;
}

void GarfieldPhysics::DeleteSecondaryElectrons() {
	if (!fSecondaryElectrons->empty()) {
		fSecondaryElectrons->erase(fSecondaryElectrons->begin(),
				fSecondaryElectrons->end());
	}
}

