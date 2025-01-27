
#include "HyDetectorConstruction.h"
#include <TGeoManager.h>
#include <TMath.h>
#include <TVirtualMC.h>
#include <iostream>
#include <string.h>

ClassImp(HyDetectorConstruction)

    using namespace std;

//_____________________________________________________________________________
void HyDetectorConstruction::SetTargetMaterial(int mat) {
  switch (mat) {
  case 2:
  case 3:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
    fSettedTargetMaterial = mat;
    break;
  default:
    fSettedTargetMaterial = 0;
  }
}
//_____________________________________________________________________________
HyGeoParameters HyDetectorConstruction::GetGeometry() { return fGeom; }
//_____________________________________________________________________________
void HyDetectorConstruction::ConstructMaterials() {
  new TGeoManager("Hyperon_geometry", "Hyperon geometry");

  Double_t a;       // Mass of a mole in g/mole
  Double_t z;       // Atomic number
  Double_t density; // Material density in g/cm3

  // Elements

  TGeoElement *elN = new TGeoElement("Nitrogen", "N", z = 7., a = 14.01);
  TGeoElement *elO = new TGeoElement("Oxygen", "O", z = 8., a = 16.00);
  TGeoElement *elPb = new TGeoElement("Lead", "Pb", z = 82., a = 207.19);
  TGeoElement *elSi = new TGeoElement("Silicon", "Si", z = 14., a = 28.09);
  TGeoElement *elK = new TGeoElement("Potassium", "K", z = 19., a = 39.10);
  TGeoElement *elAs = new TGeoElement("Arsenic", "As", z = 33., a = 74.92);
  TGeoElement *elH = new TGeoElement("Hydrogen", "H", z = 1., a = 1.01);
  TGeoElement *elC = new TGeoElement("Carbon", "C", z = 6., a = 12.01);
  // TGeoElement* elCu = new TGeoElement("Cuprum" , "Cu" , z= 29., a= 63.546);
  // TGeoElement* elSn = new TGeoElement("Tin" , "Sn" , z= 50., a= 118.71);

  // Materials
  //____Air____
  TGeoMixture *matAir = new TGeoMixture("Air", 2, density = 1.29e-03);
  matAir->AddElement(elN, 0.761);
  matAir->AddElement(elO, 0.239);
  //____Glass____
  TGeoMixture *matGlass = new TGeoMixture("Glass", 5, density = 3.86);
  matGlass->AddElement(elPb, 0.4753);
  matGlass->AddElement(elO, 0.2697);
  matGlass->AddElement(elSi, 0.1931);
  matGlass->AddElement(elK, 0.0581);
  matGlass->AddElement(elAs, 0.0038);
  //____Plastic____
  TGeoMixture *matPlastic = new TGeoMixture("Plastic", 2, density = 1.032);
  matPlastic->AddElement(elC, 0.9167);
  matPlastic->AddElement(elH, 0.0833);
  //____Poly_____
  TGeoMixture *matPoly = new TGeoMixture("Poly", 2, density = 0.89);
  matPoly->AddElement(elC, 0.8571);
  matPoly->AddElement(elH, 0.1429);
  //____C_____
  TGeoMaterial *matC = new TGeoMaterial("C", a = 12., z = 6., density = 1.742);
  //____Be____
  TGeoMaterial *matBe =
      new TGeoMaterial("Be", a = 9.01, z = 4., density = 1.848);
  //____Al____
  TGeoMaterial *matAl =
      new TGeoMaterial("Al", a = 26.98, z = 13., density = 2.699);
  //____Mylar____
  TGeoMixture *matMylar = new TGeoMixture("Mylar", 3, density = 1.4);
  matMylar->AddElement(elC, 0.625);
  matMylar->AddElement(elO, 0.333);
  matMylar->AddElement(elH, 0.042);
  //____Cu____
  TGeoMaterial *matCu =
      new TGeoMaterial("Cu", a = 63.546, z = 29., density = 8.96);
  //____Sn____
  TGeoMaterial *matSn =
      new TGeoMaterial("Sn", a = 118.71, z = 50., density = 7.31);
  //____Pb____
  TGeoMaterial *matPb =
      new TGeoMaterial("Pb", a = 207.19, z = 82., density = 11.35);
  //___S____
  TGeoMaterial *matS =
      new TGeoMaterial("S", a = 32.076, z = 16., density = 2.07);
  // Tracking media

  Double_t param[20];
  param[0] = 0;     // isvol  - Is sensitive volume?
  param[1] = 0;     // ifield - User defined magnetic field
  param[2] = 10.;   // fieldm - Maximum field value (in kiloGauss)
  param[3] = -20.;  // tmaxfd - Maximum angle due to field deflection
  param[4] = -0.01; // stemax - Maximum displacement for multiple scat
  param[5] = -.3;   // deemax - Maximum fractional energy loss, DLS
  param[6] = .001;  // epsil - Tracking precision
  param[7] = -.8;   // stmin
  for (Int_t i = 8; i < 20; ++i)
    param[i] = 0.;

  fImedAir = 1;
  new TGeoMedium("Air", fImedAir, matAir, param);

  fImedMylar = 6;
  new TGeoMedium("Mylar", fImedMylar, matMylar, param);

  // Sensitive parts
  param[0] = 1;

  fImedBe = 2;
  new TGeoMedium("Be", fImedBe, matBe, param);

  fImedAl = 3;
  new TGeoMedium("Al", fImedAl, matAl, param);

  fImedPb = 7;
  new TGeoMedium("Lead", fImedPb, matPb, param);

  fImedSn = 8;
  new TGeoMedium("Tin", fImedSn, matSn, param);

  fImedCu = 9;
  new TGeoMedium("Cuprum", fImedCu, matCu, param);

  fImedC = 10;
  new TGeoMedium("Carbon", fImedC, matC, param);

  fImedGlass = 4;
  new TGeoMedium("Glass", fImedGlass, matGlass, param);

  fImedPlastic = 5;
  new TGeoMedium("Plastic", fImedPlastic, matPlastic, param);

  fImedPoly = 11;
  new TGeoMedium("Poly", fImedPoly, matPoly, param);

  fImedS = 12;
  new TGeoMedium("S", fImedS, matS, param);
}

//_____________________________________________________________________________
void HyDetectorConstruction::ConstructVolumes() {

  //---------------Target discuss--------------------------------------------
  Int_t targMaterial = fImedBe;
  char tmp;
  int thickness, distance;
  float BeamPos[3] = {
      0., 0.,
      0.}; // Beam position relativ. LGD2 center (read from calibr.cards)
  char buf[500];
  FILE *CalibrCards = fopen("calibr.cards", "r");
  if (!CalibrCards) {
    cerr << "No calibr.cards file found!!!" << endl;
    cerr << "Beam position setted to (0,0,0) " << endl;
  } else {
    while (fgets(buf, 500, CalibrCards)) {
      if (buf[0] == 'C' && buf[1] == '*')
        continue; // comment simbols "C*"
      if (strstr(buf, "TARG") != 0)
        sscanf(buf, "TARG %f %f %f", &BeamPos[0], &BeamPos[1], &BeamPos[2]);
    }
  }
  BeamPos[0] = BeamPos[0] / 10.; // mm to cm
  BeamPos[1] = BeamPos[1] / 10.; // mm to cm
  BeamPos[2] = BeamPos[2] / 10.; // mm to cm

  cerr << "\n BeamPosition(cm) " << BeamPos[0] << " " << BeamPos[1] << " "
       << BeamPos[2] << endl;

  if (fSettedTargetMaterial == 0) {
    cerr << "Type number of material and press enter";
    cerr << "\nBe is " << fImedBe;
    cerr << "\nAl is " << fImedAl;
    cerr << "\nCu is " << fImedCu;
    cerr << "\nSn is " << fImedSn;
    cerr << "\nC  is " << fImedC;
    cerr << "\nPb is " << fImedPb;
    cerr << "\nPoly is" << fImedPoly << "\n";
    scanf("%d", &targMaterial);

    switch (targMaterial) {

    case 3:
      fTrgtType = "Al";
      break;
    case 9:
      fTrgtType = "Cu";
      break;
    case 2:
      fTrgtType = "Be";
      break;
    case 8:
      fTrgtType = "Sn";
      break;
    case 7:
      fTrgtType = "Pb";
      break;
    case 10:
      fTrgtType = "Carbon";
      break;
    case 11:
      fTrgtType = "Poly";
      break;
    case 12:
      fTrgtType = "S";
      break;
    default:
      fTrgtType = "Be";
      targMaterial = fImedBe;
      cerr << "Typed material is incorrect. Target isn't changed. Press any "
              "key and then enter to continue.";
      cin >> tmp;
    }
  } else {
    targMaterial = fSettedTargetMaterial;
  }

  fTargetMaterial = targMaterial;
  FILE *fp = fopen("file_list.dat", "r");
  if (!fp) {
    cerr << "ERROR: No filelist found" << endl;
    cerr << "Distance setted to 3700 mm" << endl;
    cerr << "Thickness setted to 79mm" << endl;
    distance = 3700;
    thickness = 79;
  } else {
    while ((fgets(buf, 500, fp))) {
      if (buf[0] == '#')
        continue;
      if ((buf[0] == '/') && (buf[2] == 'D') && (buf[3] == 'i')) {
        sscanf(&buf[14], "%d", &distance);
        sscanf(&buf[20], "%d", &thickness);
        break;
      }
    }
  }
  fGeom.SetTargetDist(fGeom.GetLGD2Dist() + fGeom.GetMatrixDist() -
                      distance / 10.);
  fGeom.SetTargetThickness(thickness / 10.);

  // process environment variables
  char *env_val;
  env_val = getenv("DELTA_S4");
  if (env_val != NULL) {
    cout << "S4 is shifted by setting the environment variable DELTA_S4 = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << " mm." << endl;
    fGeom.SetS4Dist(val / 10.);
  }
  env_val = getenv("DELTA_SA");
  if (env_val != NULL) {
    cout << "Sa is shifted by setting the environment variable DELTA_SA = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << " mm." << endl;
    fGeom.SetSaDist(fGeom.GetSaDist() + val / 10.);
  }
  env_val = getenv("THICKNESS_S4");
  if (env_val != NULL) {
    cout << "S4 thickness is modified by setting the environment variable "
            "THICKNESS_S4 = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << " mm." << endl;
    fGeom.SetS4Thickness(val / 10.);
  }
  env_val = getenv("DELTA_TARGET");
  if (env_val != NULL) {
    cout << "Target is shifted by setting the environment variable "
            "DELTA_TARGET = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << " mm." << endl;
    fGeom.SetTargetDist(fGeom.GetTargetDist() + val / 10.);
  }
  // print out the geometry parameters
  cout << buf << endl;
  cout << "Following geometry used:" << endl << endl;

  cout << "S4 thickness = " << fGeom.GetS4Thickness() << " cm." << endl;
  cout << "S4 Z_position = " << fGeom.GetS4Dist() << " cm." << endl;

  cout << "Target thickness = " << thickness << " mm." << endl;
  cout << "Target distance = " << distance << " mm." << endl;
  cout << "Target Z_position =" << fGeom.GetTargetDist() << " cm." << endl
       << endl;

  cout << "Sa thickness = " << fGeom.GetSaThickness() << " cm." << endl;
  cout << "Sa Z_position =" << fGeom.GetSaDist() << " cm." << endl << endl;

  cout << "Z_position of LGD2 surface = "
       << fGeom.GetLGD2Dist() + fGeom.GetMatrixDist() << " cm." << endl;

  Double_t *ubuf = 0;

  //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------

  //------------------------------
  // World
  //------------------------------

  Double_t world[3];
  world[0] = 200.;
  world[1] = 200.;
  world[2] = 500.;
  TGeoVolume *top = gGeoManager->Volume("WRLD", "BOX", fImedAir, world, 3);
  gGeoManager->SetTopVolume(top);

  //------------------------------
  // Target
  //------------------------------

  Double_t target[3];
  target[0] = 0.;
  target[1] = fGeom.GetTargetRadius();
  target[2] = fGeom.GetTargetThickness() / 2.;
  gGeoManager->Volume("TARG", "TUBE", targMaterial, target, 3);

  Double_t posX = 0.;
  Double_t posY = 0.;
  Double_t posZ = fGeom.GetTargetDist();
  gGeoManager->Node("TARG", 1, "WRLD", posX, posY, posZ, 0, kTRUE, ubuf);

  //------------------------------
  // Sa trigger
  //------------------------------

  Double_t Satrig[3];
  Satrig[0] = fGeom.GetSaWidth() / 2.;
  Satrig[1] = fGeom.GetSaWidth() / 2.;
  Satrig[2] = fGeom.GetSaThickness() / 2.;
  gGeoManager->Volume("SATR", "BOX", fImedPlastic, Satrig, 3);

  posX = 0.;
  posY = 0.;
  posZ = fGeom.GetSaDist();
  gGeoManager->Node("SATR", 1, "WRLD", posX, posY, posZ, 0, kTRUE, ubuf);

  //------------------------------
  // S4 trigger
  //------------------------------

  Double_t S4trig[3];
  S4trig[0] = 0;
  S4trig[1] = fGeom.GetS4Radius();
  S4trig[2] = fGeom.GetS4Thickness() / 2.;
  gGeoManager->Volume("S4TR", "TUBE", fImedPlastic, S4trig, 3);

  posX = 0.;
  posY = 0.;
  posZ = fGeom.GetS4Dist();
  gGeoManager->Node("S4TR", 1, "WRLD", posX, posY, posZ, 0, kTRUE, ubuf);

  //------------------------------
  // LGD2 detector
  //------------------------------

  // Create Detector matrix
  Double_t LGD2[3];
  LGD2[0] =
      (fGeom.GetLaGlassSize()) * (fGeom.GetLaNx() / 2.) + fGeom.GetMatrixDist();
  LGD2[1] =
      (fGeom.GetLaGlassSize()) * (fGeom.GetLaNy() / 2.) + fGeom.GetMatrixDist();
  LGD2[2] = fGeom.GetGlassLength() / 2. + fGeom.GetMatrixDist();
  gGeoManager->Volume("LGD2", "BOX", fImedAl, LGD2, 3);

  posX = -BeamPos[0]; // sdvig po x na Beam Position
  posY = -BeamPos[1]; // sdvig po y na Beam Position
  posZ = fGeom.GetLGD2Dist() + LGD2[2];
  gGeoManager->Node("LGD2", 1, "WRLD", posX, posY, posZ, 0, kTRUE, ubuf);

  // inner Volume
  LGD2[0] -= .1;
  LGD2[1] -= .1;
  LGD2[2] -= .1;
  gGeoManager->Volume("LGD2IN", "BOX", fImedAir, LGD2, 3);
  gGeoManager->Node("LGD2IN", 1, "LGD2", 0, 0, 0, 0, kTRUE, ubuf);

  // lead glasses

  // Create Left, Top, and Central parts of detector
  Double_t Glass[3];
  Glass[0] = fGeom.GetLaGlassSize() * fGeom.GetLaNx() / 4. -
             fGeom.GetSmGlassSize() * fGeom.GetSmNx() / 4.;
  Glass[1] = (fGeom.GetLaGlassSize()) * (fGeom.GetLaNy() / 2.);
  Glass[2] = fGeom.GetGlassLength() / 2.;
  gGeoManager->Volume("GLASLEFT", "BOX", fImedAir, Glass, 3);

  Glass[0] = (fGeom.GetSmGlassSize()) * (fGeom.GetSmNx() / 2.);
  Glass[1] = fGeom.GetLaGlassSize() * fGeom.GetLaNy() / 4. -
             fGeom.GetSmGlassSize() * fGeom.GetSmNy() / 4.;
  Glass[2] = fGeom.GetGlassLength() / 2.;
  gGeoManager->Volume("GLASTOP", "BOX", fImedAir, Glass, 3);

  Glass[0] = (fGeom.GetSmGlassSize()) * (fGeom.GetSmNx() / 2.);
  Glass[1] = (fGeom.GetSmGlassSize()) * (fGeom.GetSmNy() / 2.);
  Glass[2] = fGeom.GetGlassLength() / 2.;
  gGeoManager->Volume("GLACENTR", "BOX", fImedAir, Glass, 3);

  gGeoManager->Node("GLACENTR", 1, "LGD2IN", 0, 0, 0, 0, kTRUE, ubuf);
  posX = (-1) * (fGeom.GetLaGlassSize()) *
         (fGeom.GetLaNx() / 4. + fGeom.GetSmNx() / 8.);
  gGeoManager->Node("GLASLEFT", 1, "LGD2IN", posX, 0, 0, 0, kTRUE, ubuf);
  posX = (-1) * posX;
  gGeoManager->Node("GLASLEFT", 2, "LGD2IN", posX, 0, 0, 0, kTRUE, ubuf);

  posY =
      (fGeom.GetLaGlassSize()) * (fGeom.GetLaNy() / 4. + fGeom.GetSmNy() / 8.);
  gGeoManager->Node("GLASTOP", 1, "LGD2IN", 0, posY, 0, 0, kTRUE, ubuf);
  posY = (-1) * posY;
  gGeoManager->Node("GLASTOP", 2, "LGD2IN", 0, posY, 0, 0, kTRUE, ubuf);

  // Divide on X axis
  gGeoManager->Division("GLASLEFT_XDIV", "GLASLEFT", 1, 0, 0,
                        fGeom.GetLaGlassSize(), 0, "S");
  gGeoManager->Division("GLASTOP_XDIV", "GLASTOP", 1, 0, 0,
                        fGeom.GetLaGlassSize(), 0, "S");
  gGeoManager->Division("GLACENTR_XDIV", "GLACENTR", 1, 0, 0,
                        fGeom.GetSmGlassSize(), 0, "S");

  // Create Mylar shell
  gGeoManager->Division("GLASLEFT_DIVIDED", "GLASLEFT_XDIV", 2, 0, 0,
                        fGeom.GetLaGlassSize(), fImedMylar, " S");
  gGeoManager->Division("GLASTOP_DIVIDED", "GLASTOP_XDIV", 2, 0, 0,
                        fGeom.GetLaGlassSize(), fImedMylar, "S");
  gGeoManager->Division("GLACENTR_DIVIDED", "GLACENTR_XDIV", 2, 0, 0,
                        fGeom.GetSmGlassSize(), fImedMylar, "S");

  // Put Glass into Mylar
  Glass[0] = fGeom.GetLaGlassSize() / 2. - 0.01;
  Glass[1] = Glass[0];
  Glass[2] = fGeom.GetGlassLength() / 2. - 0.01;
  gGeoManager->Volume("LA_SENS_GLAS", "BOX", fImedGlass, Glass, 3);
  gGeoManager->Node("LA_SENS_GLAS", 1, "GLASLEFT_DIVIDED", 0, 0, 0, 0, kTRUE,
                    ubuf);
  gGeoManager->Node("LA_SENS_GLAS", 2, "GLASTOP_DIVIDED", 0, 0, 0, 0, kTRUE,
                    ubuf);
  Glass[0] = fGeom.GetSmGlassSize() / 2. - 0.01;
  Glass[1] = Glass[0];
  Glass[2] = fGeom.GetGlassLength() / 2. - 0.01;
  gGeoManager->Volume("SM_SENS_GLAS", "BOX", fImedGlass, Glass, 3);
  gGeoManager->Node("SM_SENS_GLAS", 1, "GLACENTR_DIVIDED", 0, 0, 0, 0, kTRUE,
                    ubuf);

  // close geometry
  gGeoManager->CloseGeometry();

  // notify VMC about Root geometry
  gMC->SetRootGeometry();
  fIsGeometryConstructed = true;
}
//_________________________________________________________________________________
bool HyDetectorConstruction::CalcInteractProbabilities() {
  if (!fIsGeometryConstructed) {
    return false;
  }
  float CDens = 1.742;
  float BeDens = 1.848;
  float AlDens = 2.7;
  float CuDens = 8.96;
  float SnDens = 7.31;
  float PbDens = 11.35;
  float PolystDens = 1.03;
  float PolyDens = 0.89;
  float MylarDens = 1.4;
  float SDens = 2.07;
  float CDensNuclIntrLngth = 85.8;
  float BeDensNuclIntrLngth = 77.8;
  float AlDensNuclIntrLngth = 107.2;
  float CuDensNuclIntrLngth = 137.3;
  float SnDensNuclIntrLngth = 166.7;
  float PbDensNuclIntrLngth = 199.6;
  float PolystDensNuclIntrLngth = 81.3;
  float PolyDensNuclIntrLngth = 78.5;
  float MylarDensNuclIntrLngth = 84.9;
  float SDensNuclIntrLngth = 125.;
  fProbArray[0] = float(1 - TMath::Exp(-fGeom.GetS4Thickness() * PolystDens /
                                       PolystDensNuclIntrLngth));
  if (fDebug)
    cout << setprecision(4)
         << "Interaction probability of S4 = " << fProbArray[0] << endl;
  ;

  if (fTrgtType == "Be")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * BeDens /
                                         BeDensNuclIntrLngth));

  if (fTrgtType == "Al")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * AlDens /
                                         AlDensNuclIntrLngth));

  if (fTrgtType == "Cu")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * CuDens /
                                         CuDensNuclIntrLngth));

  if (fTrgtType == "Sn")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * SnDens /
                                         SnDensNuclIntrLngth));

  if (fTrgtType == "Pb")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * PbDens /
                                         PbDensNuclIntrLngth));

  if (fTrgtType == "Carbon")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * CDens /
                                         CDensNuclIntrLngth));

  if (fTrgtType == "Poly")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() *
                                         PolyDens / PolyDensNuclIntrLngth));

  if (fTrgtType == "S")
    fProbArray[1] = float(1 - TMath::Exp(-fGeom.GetTargetThickness() * SDens /
                                         SDensNuclIntrLngth));

  if (fDebug)
    cout << "Interaction probability of target = " << fProbArray[1] << endl;

  fProbArray[2] = float(
      1 - TMath::Exp(
              -fGeom.GetSaThickness() * PolystDens /
              PolystDensNuclIntrLngth)); // assuming that all pi+A interactions
                                         // gives some signal in Sa due to pi+
                                         // and are suppressed by trigger

  if (fDebug)
    cout << "Interaction probability of Sa = " << fProbArray[2] << endl;

  // process environment variables
  char *env_val;
  env_val = getenv("PROBABILITY_S4");
  if (env_val != NULL) {
    cout << "S4 interaction probability is modified by setting the "
            "environment variable PROBABILITY_S4 = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << ";" << endl;
    fProbArray[0] = val;
  }
  env_val = getenv("PROBABILITY_TARG");
  if (env_val != NULL) {
    cout << "Target interaction probability is modified by setting the "
            "environment variable PROBABILITY_TARG = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << ";" << endl;
    fProbArray[1] = val;
  }
  env_val = getenv("PROBABILITY_SA");
  if (env_val != NULL) {
    cout << "SA interaction probability is modified by setting the "
            "environment variable PROBABILITY_SA = ";
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << val << ";" << endl;
    fProbArray[2] = val;
  }
  // normalize probabilities to 1;
  float norm = fProbArray[0] + fProbArray[1] + fProbArray[2];
  fProbArray[0] /= norm;
  fProbArray[1] /= norm;
  fProbArray[2] /= norm;
  if (fDebug) {
    cout << "fProbArray[0] = " << fProbArray[0];
    cout << "fProbArray[1] = " << fProbArray[1];
    cout << "fProbArray[2] = " << fProbArray[2];
  }
  return true;
}
//__________________________________________________________________________________________________
void HyDetectorConstruction::GambleVertex(Double_t *vertex) {
  static bool isProbCalculated = false;
  if (!isProbCalculated) {
    isProbCalculated = CalcInteractProbabilities();
  }

  vertex[0] = 0.;    // x
  vertex[1] = 0.;    // y
  vertex[2] = -450.; // z
  if (!fIsGeometryConstructed) {
    return;
  }

  // XY beam profile
  double r = 10., phi;
  while (r > 6.) {
    r = sqrt(-2 * log(gRandom->Uniform(0., 1.)));
    // sigma xy
    r = r * 2.5; // cm
  }
  phi = gRandom->Uniform(0., 1.) * 2 * 3.1415926;
  vertex[0] = r * cos(phi);
  vertex[1] = r * sin(phi);

  // gamble target
  int Ntarget;
  if (fExTarget >= 1 && fExTarget <= 3) {
    Ntarget = fExTarget;
  } else {
    float rndmNumb = gRandom->Rndm();
    if (rndmNumb < fProbArray[0]) {
      Ntarget = 1;
    } else if (rndmNumb < fProbArray[0] + fProbArray[1]) {
      Ntarget = 2;
    } else
      Ntarget = 3;
  }
  if (fDebug)
    cout << "This event happened on target #" << Ntarget << endl;

  switch (Ntarget) {
  case 1: // S4
    vertex[2] = fGeom.GetS4Dist() - fGeom.GetS4Thickness() / 2. +
                gRandom->Uniform(fGeom.GetS4Thickness());
    break;
  case 2: // Target
    vertex[2] = fGeom.GetTargetDist() - fGeom.GetTargetThickness() / 2. +
                gRandom->Uniform(fGeom.GetTargetThickness());
    break;
  case 3: // Sa
    vertex[2] = fGeom.GetSaDist() - fGeom.GetSaThickness() / 2. +
                gRandom->Uniform(fGeom.GetSaThickness());
    break;
  default:
    vertex[2] = -450.;
  }
}
