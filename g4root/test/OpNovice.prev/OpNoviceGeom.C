/// \file OpNoviceGeom.C
/// \brief Root macro to define OpNovice example geometry with Root
///
/// Geant4 OpNovice example adapted to G4Root test
///
/// \author A. Gheata; CERN

void OpNoviceGeom()
{
// Create root geometry corresponding to OpNovice example
  double fExpHall_x, fExpHall_y, fExpHall_z;
  double fTank_x, fTank_y, fTank_z;
  double fBubble_x, fBubble_y, fBubble_z;
  double a, z, density;
  int nelements;

  fExpHall_x = fExpHall_y = fExpHall_z = 10000.0;
  fTank_x    = fTank_y    = fTank_z    =  5000.0;
  fBubble_x  = fBubble_y  = fBubble_z  =  500.0;
  TGeoManager *geom = new TGeoManager("OpNoviceGeom", "GEANT4 OpNovice example");
// Air
//
  TGeoElement *N = new TGeoElement("Nitrogen", "N", z=7 , a=14.01);
  TGeoElement *O = new TGeoElement("Oxygen"  , "O", z=8 , a=16.00);
  TGeoMixture *air = new TGeoMixture("Air", 2, density=1.29E-3);
  air->AddElement(N, 0.7);
  air->AddElement(O, 0.3);
  TGeoMedium *medair = new TGeoMedium("Air", 1, air);
// Water
//
  TGeoElement *H = new TGeoElement("Hydrogen", "H", z=1 , a=1.01);
  TGeoMixture *water = new TGeoMixture("Water", 2, density=1.0);
  water->AddElement(H,2);
  water->AddElement(O,1);
  TGeoMedium *medwater = new TGeoMedium("Water", 2, water);
// The experimental Hall
//
  TGeoBBox *expHall_box = new TGeoBBox("World",fExpHall_x,fExpHall_y,fExpHall_z);
  TGeoVolume *expHall_log = new TGeoVolume("World", expHall_box, medair);
  expHall_log->SetLineColor(1);
  expHall_log->SetVisContainers();
  geom->SetTopVolume(expHall_log);
// The Water Tank
//
  TGeoBBox *waterTank_box = new TGeoBBox("Tank",fTank_x,fTank_y,fTank_z);
  TGeoVolume *waterTank_log = new TGeoVolume("Tank", waterTank_box, medwater);
  waterTank_log->SetLineColor(kBlue);
  waterTank_log->SetTransparency(70);
  waterTank_log->SetVisContainers();
  expHall_log->AddNode(waterTank_log, 0);
// The Air Bubble
//
  TGeoBBox *bubbleAir_box = new TGeoBBox("Bubble",fBubble_x,fBubble_y,fBubble_z);
  TGeoVolume *bubbleAir_log = new TGeoVolume("Bubble", bubbleAir_box, medair);
  bubbleAir_log->SetLineColor(kCyan);
  bubbleAir_log->SetTransparency(70);
  waterTank_log->AddNode(bubbleAir_log, 0, new TGeoTranslation(0,2500,0));

  geom->CloseGeometry();
  geom->SetTopVisible(true);
  geom->Export("OpNoviceGeom.root");
}




