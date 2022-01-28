void OpNoviceGeom() {
   gSystem->Load("libGeom");
   new TGeoManager("OpNoviceGeom", "GEANT4 OpNovice example");

   Double_t dx,dy,dz;
   Double_t dx1, dx2, dy1, dy2;
   Double_t vert[20], par[20];
   Double_t theta, phi, h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2;
   Double_t twist;
   Double_t origin[3];
   Double_t rmin, rmax, rmin1, rmax1, rmin2, rmax2;
   Double_t r, rlo, rhi;
   Double_t phi1, phi2;
   Double_t a,b;
   Double_t point[3], norm[3];
   Double_t rin, stin, rout, stout;
   Double_t thx, phx, thy, phy, thz, phz;
   // Double_t alpha, theta1, theta2, phi1, phi2, dphi;
   Double_t tr[3], rot[9];
   Double_t z, density, radl, absl, w;
   Double_t lx,ly,lz,tx,ty,tz;
   Double_t xvert[50], yvert[50];
   Double_t zsect,x0,y0,scale0;
   Int_t nel, numed, nz, nedges, nvert;
   TGeoBoolNode *pBoolNode = 0;

   // MATERIALS, MIXTURES AND TRACKING MEDIA
// Mixture: Air
   nel     = 2;
   density = 0.0012899999999999999;
   auto pMat1 = new TGeoMixture("Air", nel,density);
      a = 14.01;   z = 7;   w = 0.69999999999999996;  // Nitrogen
   pMat1->DefineElement(0,a,z,w);
      a = 16;   z = 8;   w = 0.29999999999999999;  // Oxygen
   pMat1->DefineElement(1,a,z,w);
   pMat1->SetIndex(0);
// Medium: Air
   numed   = 1;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   auto pMed1 = new TGeoMedium("Air", numed,pMat1, par);
// Mixture: Water
   nel     = 2;
   density = 1;
   auto pMat2 = new TGeoMixture("Water", nel,density);
      a = 1.01;   z = 1;   w = 0.1120976692563818;  // Hydrogen
   pMat2->DefineElement(0,a,z,w);
      a = 16;   z = 8;   w = 0.88790233074361824;  // Oxygen
   pMat2->DefineElement(1,a,z,w);
   pMat2->SetIndex(1);
// Medium: Water
   numed   = 2;  // medium number
   par[0]  = 0; // isvol
   par[1]  = 0; // ifield
   par[2]  = 0; // fieldm
   par[3]  = 0; // tmaxfd
   par[4]  = 0; // stemax
   par[5]  = 0; // deemax
   par[6]  = 0; // epsil
   par[7]  = 0; // stmin
   auto pMed2 = new TGeoMedium("Water", numed,pMat2, par);

   // TRANSFORMATION MATRICES
   // Translation: 
   dx = 0;
   dy = 250;
   dz = 0;
   TGeoTranslation *pMatrix4 = new TGeoTranslation("",dx,dy,dz);


   // Shape: World type: TGeoBBox
   dx = 1500;
   dy = 1500;
   dz = 1500;
   TGeoShape *pWorld_1 = new TGeoBBox("world", dx,dy,dz);
   // Volume: World
   auto pWorld_7fb867f9e0a0 = new TGeoVolume("world",pWorld_1, pMed1);
   pWorld_7fb867f9e0a0->SetVisContainers(kTRUE);

   // SET TOP VOLUME OF GEOMETRY
   gGeoManager->SetTopVolume(pWorld_7fb867f9e0a0);

   // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
   // Shape: expHall type: TGeoBBox
   dx = 1000;
   dy = 1000;
   dz = 1000;
   TGeoShape *pExpHall = new TGeoBBox("expHall", dx,dy,dz);
   // Volume: Tank
   auto pExpHall_vol = new TGeoVolume("expHall",pExpHall, pMed1);
   pExpHall_vol->SetLineColor(600);
   pExpHall_vol->SetVisContainers(kTRUE);
   pWorld_7fb867f9e0a0->AddNode(pExpHall_vol, 0);
   // Shape: Tank type: TGeoBBox
   dx = 500;
   dy = 500;
   dz = 500;
   TGeoShape *pTank_2 = new TGeoBBox("Tank", dx,dy,dz);
   // Volume: Tank
   auto pTank_7fb867f9e2a0 = new TGeoVolume("Tank",pTank_2, pMed2);
   pTank_7fb867f9e2a0->SetLineColor(600);
   pTank_7fb867f9e2a0->SetVisContainers(kTRUE);
   pExpHall_vol->AddNode(pTank_7fb867f9e2a0, 0);
   // Shape: Bubble type: TGeoBBox
   dx = 50;
   dy = 50;
   dz = 50;
   TGeoShape *pBubble_3 = new TGeoBBox("Bubble", dx,dy,dz);
   // Volume: Bubble
   auto pBubble_7fb867f9e4a0 = new TGeoVolume("Bubble",pBubble_3, pMed1);
   pBubble_7fb867f9e4a0->SetLineColor(432);
   pBubble_7fb867f9e4a0->SetVisLeaves(kTRUE);
   pTank_7fb867f9e2a0->AddNode(pBubble_7fb867f9e4a0, 0, pMatrix4);

   // CLOSE GEOMETRY
   gGeoManager->CloseGeometry();
}
