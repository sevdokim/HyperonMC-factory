Int_t fImedAr;  ///< The Argon gas medium Id
Int_t fImedAir; ///< The air medium Id
Int_t fImedAl;  ///< The Aluminium medium Id
Int_t fImedPb;  ///< The Lead medium Id
int fImedBe;
int fImedC;
int fImedCu;
int fImedSn;
int fImedCH2;
int fImedTarget;
int fImedPlastic;
int fImedSci;
int fImedBGO;
int fImedNaI;
double fTargetRadius = 2.;
double fTargetThickness = 4.;

void ConstructMaterials();
void geometry() {

  ConstructMaterials();
  fImedTarget = fImedBe;

  Double_t *ubuf = 0;

  Double_t expHall[3];
  expHall[0] = 300.;
  expHall[1] = 100.;
  expHall[2] = 100.;
  TGeoVolume *top = gGeoManager->Volume("EXPH", "BOX", fImedAir, expHall, 3);
  gGeoManager->SetTopVolume(top);

  //------------------------------ Target
  Double_t targetSize[3];
  targetSize[0] = 0.;
  targetSize[1] = fTargetRadius;
  targetSize[2] = fTargetThickness / 2.;
  gGeoManager->Volume("TARGET", "TUBE", fImedTarget, targetSize, 3);

  Double_t posX = 0.;
  Double_t posY = 0.;
  Double_t posZ = 0.;
  gGeoManager->Node("TARGET", 1, "EXPH", posX, posY, posZ, 0, kTRUE, ubuf);

  //----------------------------- GaNT detector (NaI)
  TGeoVolume *singleGantNaI = gGeoManager->MakeTube(
      "SingleGantNaI", gGeoManager->GetMedium(fImedPlastic), 0., 4.6,
      24.5 / 2.);
  singleGantNaI->SetVisibility(kTRUE);
  TGeoVolume *insideSingleGantNaI = gGeoManager->MakeTube(
      "InsideSingleGantNaI", gGeoManager->GetMedium(fImedAir), 0., 4.1,
      24. / 2.);
  insideSingleGantNaI->SetVisibility(kFALSE);
  TGeoVolume *crystallNaI = gGeoManager->MakeTube(
      "crystallNaI", gGeoManager->GetMedium(fImedNaI), 0., 6.9 / 2., 6.9 / 2.);
  crystallNaI->SetVisibility(kTRUE);
  insideSingleGantNaI->AddNode(
      crystallNaI, 1, new TGeoTranslation(0., 0., -(24. / 2. - 6.9 / 2.)));
  singleGantNaI->AddNode(insideSingleGantNaI, 1,
                         new TGeoTranslation(0., 0., 0.));

  TGeoVolume *plastCPV = gGeoManager->MakeBox(
      "plastCPV", gGeoManager->GetMedium(fImedSci), 4.4, 0.2, 16.);

  double distGant = 14.6 / 2. + 0.4 + 0.5 + 24.5 / 2.;
  double distCPV = 14.6 / 2. + 0.2;
  distCPV = TMath::Sqrt(distCPV * distCPV + 0.7 * 0.7);
  double dphiCPV = TMath::ATan(0.7 / 14.6) * 180. / TMath::Pi();

  for (int i = 0; i < 3; i++) {
    TGeoRotation *rot =
        new TGeoRotation(Form("rot%d", i + 1), i * 240., -90., i * 240.);
    // 3 GsNT modules
    for (int iz = 0; iz < 3; iz++) {
      top->AddNode(singleGantNaI, i * 3 + 1 + iz,
                   new TGeoCombiTrans(
                       distGant * TMath::Sin(i * 120. / 180. * TMath::Pi()),
                       distGant * TMath::Cos(i * 120. / 180. * TMath::Pi()),
                       -9.2 + iz * 9.2, rot));
    }
    // CPV module
    top->AddNode(
        plastCPV, i,
        new TGeoCombiTrans(
            distCPV * TMath::Sin((i * 120. + dphiCPV) / 180. * TMath::Pi()),
            distCPV * TMath::Cos((i * 120. + dphiCPV) / 180. * TMath::Pi()), 0.,
            new TGeoRotation(Form("rotCPV%d", i), 0., 0., 60. * i)));
  }

  //---------------------------- GaNT detector (BGO)
  TGeoVolume *singleGantBGO = gGeoManager->MakeTube(
      "SingleGantBGO", gGeoManager->GetMedium(fImedPlastic), 0., 4.6,
      24.5 / 2.);
  singleGantBGO->SetVisibility(kTRUE);
  TGeoVolume *crystallBGO = gGeoManager->MakeTube(
      "crystallBGO", gGeoManager->GetMedium(fImedBGO), 0., 6.9 / 2., 6.9 / 2.);
  crystallBGO->SetVisibility(kTRUE);
  TGeoVolume *insideSingleGantBGO = gGeoManager->MakeTube(
      "InsideSingleGantBGO", gGeoManager->GetMedium(fImedAir), 0., 4.1,
      24. / 2.);

  insideSingleGantBGO->AddNode(
      crystallBGO, 1, new TGeoTranslation(0., 0., -(24. / 2. - 6.9 / 2.)));
  singleGantBGO->AddNode(insideSingleGantBGO, 1,
                         new TGeoTranslation(0., 0., 0.));

  for (int i = 3; i < 6; i++) {
    TGeoRotation *rot =
        new TGeoRotation(Form("rot%d", i + 1), 120. + (i - 3) * 240., 90.,
                         120. + (i - 3) * 240.);
    // 3 GaNT modules
    for (int iz = 0; iz < 3; iz++) {
      top->AddNode(
          singleGantBGO, i * 3 + 1 + iz,
          new TGeoCombiTrans(
              distGant * TMath::Sin((i - 2.5) * 120. / 180. * TMath::Pi()),
              distGant * TMath::Cos((i - 2.5) * 120. / 180. * TMath::Pi()),
              -9.2 + iz * 9.2, rot));
    }
    // CPV module
    top->AddNode(
        plastCPV, i,
        new TGeoCombiTrans(
            distCPV *
                TMath::Sin(((i - 2.5) * 120. + dphiCPV) / 180. * TMath::Pi()),
            distCPV *
                TMath::Cos(((i - 2.5) * 120. + dphiCPV) / 180. * TMath::Pi()),
            0., new TGeoRotation(Form("rotCPV%d", i), 0., 0., 60. * (i - 4))));
  }
  top->SetVisibility(kTRUE);
  // close geometry
  gGeoManager->CloseGeometry();

  gGeoManager->GetTopVolume()->Draw("ogle");

  // notify VMC about Root geometry
  // gMC->SetRootGeometry();
}
void ConstructMaterials() {
  new TGeoManager("E01_geometry", "E01 VMC example geometry");

  Double_t a;       // Mass of a mole in g/mole
  Double_t z;       // Atomic number
  Double_t density; // Material density in g/cm3

  TGeoElement *elN = new TGeoElement("Nitrogen", "N", z = 7., a = 14.01);
  TGeoElement *elO = new TGeoElement("Oxygen", "O", z = 8., a = 16.00);
  TGeoElement *elH = new TGeoElement("Hydrogen", "H", z = 1., a = 1.01);
  TGeoElement *elC = new TGeoElement("Carbon", "C", z = 6., a = 12.01);
  TGeoElement *elNa = new TGeoElement("Sodium", "Na", z = 11., a = 22.99);
  TGeoElement *elI = new TGeoElement("Iodine", "I", z = 53., a = 126.90447);
  TGeoElement *elBi = new TGeoElement("Bismut", "Bi", z = 83., a = 208.98);
  TGeoElement *elGe = new TGeoElement("Germanium", "Ge", z = 32., a = 72.63);

  //____Ar____
  a = 39.95;
  z = 18.;
  density = 1.782e-03;
  TGeoMaterial *matAr = new TGeoMaterial("ArgonGas", a, z, density);

  //____Air____
  TGeoMixture *matAir = new TGeoMixture("Air", 2, density = 1.29e-03);
  matAir->AddElement(elN, 0.761);
  matAir->AddElement(elO, 0.239);

  //____CH2_____
  TGeoMixture *matPoly = new TGeoMixture("CH2", 2, density = 0.89);
  matPoly->AddElement(elC, 0.8571);
  matPoly->AddElement(elH, 0.1429);

  //____Al____
  TGeoMaterial *matAl =
      new TGeoMaterial("Al", a = 26.98, z = 13., density = 2.7);
  //____Pb____
  TGeoMaterial *matLead =
      new TGeoMaterial("Pb", a = 207.19, z = 82., density = 11.35);
  //____C_____
  TGeoMaterial *matC = new TGeoMaterial("C", a = 12., z = 6., density = 1.742);
  //____Be____
  TGeoMaterial *matBe =
      new TGeoMaterial("Be", a = 9.01, z = 4., density = 1.848);
  //____Cu____
  TGeoMaterial *matCu =
      new TGeoMaterial("Cu", a = 63.546, z = 29., density = 8.96);
  //____Sn____
  TGeoMaterial *matSn =
      new TGeoMaterial("Sn", a = 118.71, z = 50., density = 7.31);

  //____NaI____
  TGeoMixture *matNaI = new TGeoMixture("NaI", 2, density = 3.67);
  matNaI->AddElement(elNa, int(1));
  matNaI->AddElement(elI, int(1));

  //____BGO____
  TGeoMixture *matBGO = new TGeoMixture("BGO", 3, density = 7.13);
  matBGO->AddElement(elBi, int(4));
  matBGO->AddElement(elGe, int(3));
  matBGO->AddElement(elO, int(12));

  //____Scintillator____
  TGeoMixture *matSci = new TGeoMixture("Scintillator", 2, density = 1.032);
  matSci->AddElement(elC, 9);
  matSci->AddElement(elH, 10);

  //____Plastic____
  TGeoMixture *matPlastic = new TGeoMixture("POMPlastic", 3, density = 1.42);
  matPlastic->AddElement(elC, 1);
  matPlastic->AddElement(elH, 2);
  matPlastic->AddElement(elO, 1);

  /*
    // Set material IDs
    // This step is needed, only if user wants to use the material Ids
    // in his application. Be aware that the material Ids vary
    // with each concrete MC.
    // It is recommended to use Media Ids instead, which values
    // set by user are preserved in all MCs
    Int_t imat = 0;
    matAr->SetUniqueID(imat++);
    matAl->SetUniqueID(imat++);
    matLead->SetUniqueID(imat++);
  */

  //
  // Tracking medias
  //

  Double_t param[20];
  param[0] = 0;     // isvol  - Not used
  param[1] = 2;     // ifield - User defined magnetic field
  param[2] = 10.;   // fieldm - Maximum field value (in kiloGauss)
  param[3] = -20.;  // tmaxfd - Maximum angle due to field deflection
  param[4] = -0.01; // stemax - Maximum displacement for multiple scat
  param[5] = -.3;   // deemax - Maximum fractional energy loss, DLS
  param[6] = .001;  // epsil - Tracking precision
  param[7] = -.8;   // stmin
  for (Int_t i = 8; i < 20; ++i)
    param[i] = 0.;

  // target material
  fImedAl = 3;
  new TGeoMedium("Aluminium", fImedAl, matAl, param);

  fImedPb = 6;
  new TGeoMedium("Lead", fImedPb, matLead, param);

  fImedBe = 1;
  new TGeoMedium("Be", fImedBe, matBe, param);

  fImedSn = 5;
  new TGeoMedium("Tin", fImedSn, matSn, param);

  fImedCu = 4;
  new TGeoMedium("Cuprum", fImedCu, matCu, param);

  fImedC = 2;
  new TGeoMedium("Carbon", fImedC, matC, param);

  fImedCH2 = 7;
  new TGeoMedium("Polyethylene", fImedCH2, matPoly, param);

  // all other material
  fImedAr = 10;
  new TGeoMedium("ArgonGas", fImedAr, matAr, param);

  fImedAir = 11;
  new TGeoMedium("Air", fImedAir, matAir, param);

  fImedNaI = 12;
  new TGeoMedium("NaI", fImedNaI, matNaI, param);

  fImedBGO = 13;
  new TGeoMedium("BGOcrystall", fImedBGO, matBGO, param);

  fImedPlastic = 14;
  new TGeoMedium("Plastic", fImedPlastic, matPlastic, param);

  fImedSci = 15;
  new TGeoMedium("Scintillator", fImedSci, matSci, param);
}
