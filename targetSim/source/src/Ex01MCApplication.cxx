//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex01MCApplication.cxx
/// \brief Implementation of the Ex01MCApplication class
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo \n
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex01MCApplication.h"
#include "Ex01DetectorConstructionOld.h"
#include "Ex01MCStack.h"

#include <Riostream.h>
#include <TArrayD.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoMaterial.h>
#include <TGeoMatrix.h>
#include <TH1F.h>
#include <TInterpreter.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TThread.h>
#include <TVirtualGeoTrack.h>
#include <TVirtualMC.h>
#include <cstdlib>

using namespace std;

/// \cond CLASSIMP
ClassImp(Ex01MCApplication)
    /// \endcond

    //_____________________________________________________________________________
    Ex01MCApplication::Ex01MCApplication(const char *name, const char *title)
    : TVirtualMCApplication(name, title), fStack(0), fMagField(0), fImedAr(0),
      fImedAir(0), fImedAl(0), fImedPb(0), fImedBe(0), fImedC(0), fImedCu(0),
      fImedSn(0), fImedCH2(0), fOldGeometry(kFALSE) {
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  // create a user stack
  fStack = new Ex01MCStack(100000);

  // create magnetic field (with zero value)
  fMagField = new TGeoUniformMagField();
}

//_____________________________________________________________________________
Ex01MCApplication::Ex01MCApplication()
    : TVirtualMCApplication(), fStack(0), fMagField(0), fImedAr(0), fImedAir(0),
      fImedAl(0), fImedPb(0), fImedBe(0), fImedC(0), fImedCu(0), fImedSn(0),
      fImedCH2(0), fOldGeometry(kFALSE) {
  /// Default constructor
}

//_____________________________________________________________________________
Ex01MCApplication::~Ex01MCApplication() {
  /// Destructor

  delete fStack;
  delete fMagField;
  delete gMC;
}

//
// private methods
//

//_____________________________________________________________________________
void Ex01MCApplication::ConstructMaterials() {
  /// Construct materials using TGeo modeller

  // Create Root geometry manager
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
  TGeoElement *elCs = new TGeoElement("Caesium", "Cs", z = 55., a = 133.);
  
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

  //____CsI____
  TGeoMixture *matCsI = new TGeoMixture("CsI", 2, density = 4.51);
  matCsI->AddElement(elCs, int(1));
  matCsI->AddElement(elI, int(1));
  
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

  //____Vacuum_____
  TGeoMaterial *matVac =
      new TGeoMaterial("Vacuum", a = 1., z = 1., density = 10e-20);

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

  fImedVac = 16;
  new TGeoMedium("Vacuum", fImedVac, matVac, param);

  fImedCsI = 17;
  new TGeoMedium("CsI", fImedCsI, matCsI, param);
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructVolumes() {
  /// Contruct volumes using TGeo modeller

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along z axis

  Double_t *ubuf = 0;

  Double_t expHall[3];
  expHall[0] = 20.;
  expHall[1] = 20.;
  expHall[2] = 20.;
  TGeoVolume *top = gGeoManager->Volume("EXPH", "BOX", fImedAir, expHall, 3);
  gGeoManager->SetTopVolume(top);

  //------------------------------ Target
  Double_t targetSize[3];
  targetSize[0] = 0.;
  targetSize[1] = fTargetRadius;
  targetSize[2] = fTargetThickness / 2.;
  // gGeoManager->Volume("TARGET", "TUBE", fImedVac, targetSize, 3);
  gGeoManager->Volume("TARGET", "TUBE", fImedTarget, targetSize, 3);

  Double_t posX = 0.;
  Double_t posY = 0.;
  Double_t posZ = 0.;
  gGeoManager->Node("TARGET", 1, "EXPH", posX, posY, posZ, 0, kTRUE, ubuf);

  int iMedPOM = fImedPlastic;
  // int iMedPOM = fImedVac;

  
  TGeoVolume *preshower = 0x0;
  if (fPreshowerThickness > 0.) {
    preshower = gGeoManager->MakeTube(
      "preshower", gGeoManager->GetMedium(fImedPb), 0., 6.9 / 2., fPreshowerThickness / 2.);
  }
  
  //----------------------------- GaNT detector (NaI)
  TGeoVolume *singleGantNaI = gGeoManager->MakeTube(
      "SingleGantNaI", gGeoManager->GetMedium(iMedPOM), 0., 4.6, 24.5 / 2.);
  singleGantNaI->SetVisibility(kTRUE);
  TGeoVolume *insideSingleGantNaI = gGeoManager->MakeTube(
      "InsideSingleGantNaI", gGeoManager->GetMedium(fImedAir), 0., 4.1,
      24. / 2.);
  insideSingleGantNaI->SetVisibility(kFALSE);
  TGeoVolume *crystallNaI = gGeoManager->MakeTube(
      "crystallNaI", gGeoManager->GetMedium(fImedCsI), 0., 6.9 / 2., 6.9 / 2.);
  crystallNaI->SetVisibility(kTRUE);
  
  insideSingleGantNaI->AddNode(
      crystallNaI, 1, new TGeoTranslation(0., 0., -(24. / 2. - 6.9 / 2. - fPreshowerThickness)));
  if (fPreshowerThickness > 0.) {
    insideSingleGantNaI->AddNode(
      preshower, 1, new TGeoTranslation(0., 0., -(24. / 2. - fPreshowerThickness / 2.)));
  }
  singleGantNaI->AddNode(insideSingleGantNaI, 1,
                         new TGeoTranslation(0., 0., 0.));

  TGeoVolume *plastCPV = gGeoManager->MakeBox(
					          "plastCPV", gGeoManager->GetMedium(fImedSci), 4.4, 0.2, 16.);
					      //"plastCPV", gGeoManager->GetMedium(fImedSci), 4.4, 0.2, 4.4);
					      
  double distGant = 14.6 / 2. + 0.4 + 0.5 + 24.5 / 2.;
  double distCPV = 14.6 / 2. + 0.2;
  distCPV = TMath::Sqrt(distCPV * distCPV + 0.7 * 0.7);
  double dphiCPV = TMath::ATan(0.7 / 14.6) * 180. / TMath::Pi();

  for (int i = 0; i < 1; i++) {
    TGeoRotation *rot =
        new TGeoRotation(Form("rot%d", i + 1), i * 240., -90., i * 240.);
    // 3 GNT modules
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
      "SingleGantBGO", gGeoManager->GetMedium(iMedPOM), 0., 4.6, 24.5 / 2.);
  singleGantBGO->SetVisibility(kTRUE);
  TGeoVolume *crystallBGO = gGeoManager->MakeTube(
      "crystallBGO", gGeoManager->GetMedium(fImedBGO), 0., 6.9 / 2., 6.9 / 2.);
  crystallBGO->SetVisibility(kTRUE);
  TGeoVolume *insideSingleGantBGO = gGeoManager->MakeTube(
      "InsideSingleGantBGO", gGeoManager->GetMedium(fImedAir), 0., 4.1,
      24. / 2.);

  insideSingleGantBGO->AddNode(
      crystallBGO, 1, new TGeoTranslation(0., 0., -(24. / 2. - 6.9 / 2. - fPreshowerThickness)));
  if (fPreshowerThickness > 0.) {
    insideSingleGantBGO->AddNode(
        preshower, 1, new TGeoTranslation(0., 0., -(24. / 2. - fPreshowerThickness / 2.)));
  }

  singleGantBGO->AddNode(insideSingleGantBGO, 1,
                         new TGeoTranslation(0., 0., 0.));

  for (int i = 3; i < 0; i++) {
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

  // S_F
  TGeoVolume *plastS_F = gGeoManager->MakeBox(
      "S_F", gGeoManager->GetMedium(fImedSci), 10., 10., 0.3);
  top->AddNode(plastS_F, 1, new TGeoTranslation(0., 0., 18.));

  // close geometry
  gGeoManager->CloseGeometry();

  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}

//
// public methods
//

//_____________________________________________________________________________
void Ex01MCApplication::InitMC(const char *setup) {
  /// Initialize MC.
  /// The selection of the concrete MC is done in the macro.
  /// \param setup The name of the configuration macro

  if (TString(setup) != "") {
    gROOT->LoadMacro(setup);
    gInterpreter->ProcessLine("Config()");
    if (!gMC) {
      Fatal("InitMC",
            "Processing Config() has failed. (No MC is instantiated.)");
    }
  }

  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
  gMC->Init();
  gMC->BuildPhysics();

  const char *fileName = "histos.root";
  cout << "Opening file " << fileName << " for writing." << endl;
  ;
  fFileSave = new TFile("histos.root", "RECREATE");
  hEnDepInTarget =
      new TH1F("hEnDepInTarget", "Deposed energy in target; E, GeV; counts",
               1000, 0., 0.5);
  hEnDepInAir = new TH1F("hEnDepInAir", "Deposed energy in air; E, GeV; counts",
                         100, 0., 0.1);
  hEnergyWhenExitingTarget = new TH1F(
      "hEnergyWhenExitingTarget",
      "Photon energy when leaving target; E, GeV; counts", 5000, 0., 5.);
  hEnergyOfSecondariesInTarget = new TH1F(
      "hEnergyOfSecondariesInTarget",
      "Energy of secondaries produced in target; E, GeV; counts", 5000, 0., 5.);
  hDeltaTheta = new TH1F("hDeltaTheta", "#Delta #Theta", 1000, -1., 1.);

  for (int i = 0; i < 6; i++) {
    hEnDepInCPV[i] = new TH1F(
        Form("hEnDepInCPV_%d", i),
        Form("Deposed energy in CPV M%d; E, GeV; counts", i), 50000, 0., 1.);
  }
  hSumEnDepInCPV =
      new TH1F("hSumEnDepInCPV", "Sum of deposed energy in CPV; E, GeV; counts",
               50000, 0., 1.);
  hEnDepInSF = new TH1F("hEnDepInSF", "Deposed energy in S_{F}; E, GeV; counts",
                        50000, 0., 1.);

  for (int i = 0; i < 9; i++) {
    hEnDepInBGO[i] = new TH1F(
        Form("hEnDepInBGO%d", i),
        Form("Deposed energy in BGO M%d; E, GeV; counts", i), 50000, 0., 1.);

    hEnDepInNaI[i] = new TH1F(
        Form("hEnDepInNaI%d", i),
        Form("Deposed energy in NaI M%d; E, GeV; counts", i), 50000, 0., 1.);
    hEnDepInBGOTriggered[i] = new TH1F(
        Form("hEnDepInBGOTriggered%d", i),
        Form("Deposed energy in BGO M%d (trigger passed); E, GeV; counts", i),
        50000, 0., 1.);

    hEnDepInNaITriggered[i] = new TH1F(
        Form("hEnDepInNaITriggered%d", i),
        Form("Deposed energy in NaI M%d (trigger passed); E, GeV; counts", i),
        50000, 0., 1.);
    hMesEnInBGO[i] = new TH1F(
        Form("hMesEnInBGO%d", i),
        Form("Measured energy in BGO M%d; E, GeV; counts", i), 50000, 0., 1.);

    hMesEnInNaI[i] = new TH1F(
        Form("hMesEnInNaI%d", i),
        Form("Measured energy in NaI M%d; E, GeV; counts", i), 50000, 0., 1.);
    hMesEnInBGOTriggered[i] = new TH1F(
        Form("hMesEnInBGOTriggered%d", i),
        Form("Measured energy in BGO M%d (trigger passed); E, GeV; counts", i),
        50000, 0., 1.);

    hMesEnInNaITriggered[i] = new TH1F(
        Form("hMesEnInNaITriggered%d", i),
        Form("Measured energy in NaI M%d (trigger passed); E, GeV; counts", i),
        50000, 0., 1.);
  }
  hSumEnDepInBGO =
      new TH1F("hSumEnDepInBGO", "Sum of deposed energy in BGO; E, GeV; counts",
               50000, 0., 1.);

  hSumEnDepInNaI =
      new TH1F("hSumEnDepInNaI", "Sum of deposed energy in NaI; E, GeV; counts",
               50000, 0., 1.);
  hSumEnDepInBGOTriggered =
      new TH1F("hSumEnDepInBGOTriggered",
               "Sum of deposed energy in BGO (trigger passed); E, GeV; counts",
               50000, 0., 1.);

  hSumEnDepInNaITriggered =
      new TH1F("hSumEnDepInNaItriggered",
               "Sum of deposed energy in NaI (trigger passed); E, GeV; counts",
               50000, 0., 1.);

  // init pythia8
  fPythia = new Pythia8::Pythia();
  int pySeed = gRandom->GetSeed();
  if (pySeed <= 0) {
    pySeed = gRandom->Integer(900000000);
  }
  fPythia->readString("LowEnergyQCD:all = on");
  fPythia->readString(Form("Random:Seed = %d", pySeed));
  fPythia->readString("SoftQCD:all = on");
  fPythia->readString("Beams:idA = 211");
  fPythia->readString("Beams:idB = 2112");
  fPythia->readString("Beams:eA = 7.");
  fPythia->readString("Beams:eB = 0.");
  fPythia->readString("Beams:frameType = 2");
  // fPythia->readString("2214:onMode = off");
  // fPythia->readString("2214:onIfAny = 2212 111");
  fPythia->init();

  // trigger thresholds
  fCPVThreshold = 0.0006;
  fSFThreashold = 0.0007;

  // read PDG code from environment
  fPdgCode = 22; // default (photon)
  if (char *pdg_code = getenv("particle_code")) {
    sscanf(pdg_code, "%d", &fPdgCode);
    cout << "PDG code of chosen particle is" << fPdgCode << endl;
    TParticlePDG *p = TDatabasePDG::Instance()->GetParticle(fPdgCode);
    cout << "The code corresponds to " << p->GetName() << endl;
  }
}

//__________________________________________________________________________
void Ex01MCApplication::RunMC(Int_t nofEvents) {
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  gMC->ProcessRun(nofEvents);
  FinishRun();
}
//_____________________________________________________________________________
void Ex01MCApplication::FinishRun() {
  /// Finish MC run.
  fFileSave->Write();
  fFileSave->Close();
  fPythia->stat();
}

//_____________________________________________________________________________
TVirtualMCApplication *Ex01MCApplication::CloneForWorker() const {
  return new Ex01MCApplication(GetName(), GetTitle());
}

//_____________________________________________________________________________
void Ex01MCApplication::InitOnWorker() {
  gMC->SetStack(fStack);
  gMC->SetMagField(fMagField);
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructGeometry() {
  /// Construct geometry using TGeo functions or
  /// TVirtualMC functions (if oldGeometry is selected)

  // Cannot use Root geometry if not supported with
  // selected MC
  if (!fOldGeometry && !gMC->IsRootGeometrySupported()) {
    cerr << "Selected MC does not support TGeo geometry" << endl;
    cerr << "Exiting program" << endl;
    exit(1);
  }

  if (!fOldGeometry) {
    cout << "Geometry will be defined via TGeo" << endl;
    ConstructMaterials();
    ConstructVolumes();
  } else {
    cout << "Geometry will be defined via VMC" << endl;
    Ex01DetectorConstructionOld detConstructionOld;
    detConstructionOld.ConstructMaterials();
    detConstructionOld.ConstructVolumes();
  }
}

//_____________________________________________________________________________
void Ex01MCApplication::InitGeometry() {
  /// Initialize geometry.
  /*
  fImedAr = gMC->MediumId("ArgonGas");
  fImedAir = gMC->MediumId("Air");
  fImedAl = gMC->MediumId("Aluminium");
  fImedPb = gMC->MediumId("Lead");
  */
}

//_____________________________________________________________________________
void Ex01MCApplication::GeneratePrimaries() {
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  static bool firstCall = true;

  // Track ID (filled by stack)
  Int_t ntr;

  // Option: to be tracked
  Int_t toBeDone = 1;

  // photon
  Int_t pdg = fPdgCode;
  // pdg = 13; //muon

  // Polarization
  Double_t polx = 0.;
  Double_t poly = 0.;
  Double_t polz = 0.;

  // spherical photons from target
  // Position
  Double_t vx = 2. * fTargetRadius * (gRandom->Rndm() - 0.5);
  Double_t vy = 2. * fTargetRadius * (gRandom->Rndm() - 0.5);
  while (vx * vx + vy * vy >= fTargetRadius * fTargetRadius) {
    vx = 2. * fTargetRadius * (gRandom->Rndm() - 0.5);
    vy = 2. * fTargetRadius * (gRandom->Rndm() - 0.5);
  }
  Double_t vz = fTargetThickness * (-0.5 + gRandom->Rndm());
  Double_t tof = 0.;

  // Momentum
  Double_t px, py, pz, e = fInitialEnergy, p;
  static double mass = 0.;
  if (firstCall) {
    TParticlePDG *partPdg = TDatabasePDG::Instance()->GetParticle(fPdgCode);
    mass = partPdg->Mass();
  }
  if (fInitialEnergy > mass) {
    double phi = 2. * TMath::Pi() * gRandom->Rndm();
    double cosTheta = TMath::Sqrt(3.) * (gRandom->Rndm() - 0.5);
    double sinTheta = TMath::Sqrt(1. - cosTheta * cosTheta);
    p = TMath::Sqrt(fInitialEnergy * fInitialEnergy - mass * mass);
    px = p * sinTheta * TMath::Sin(phi);
    py = p * sinTheta * TMath::Cos(phi);
    pz = p * cosTheta;
    fInitialMomentum.SetXYZT(px, py, pz, e);
    fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx,
                      poly, polz, kPPrimary, ntr, 1., 0);
  } else {
    fInitialMomentum.SetXYZT(0, 0, 1, 1);
  }

  // beam muons
  /*pdg = 13;
  // Position
  Double_t vx = 2. * 15. * (gRandom->Rndm() - 0.5);
  Double_t vy = 2. * 15. * (gRandom->Rndm() - 0.5);
  Double_t vz = -15.;
  Double_t tof = 0.;

  // Momentum
  TDatabasePDG *pdgDB = TDatabasePDG::Instance();
  double m = pdgDB->GetParticle(13)->Mass();
  Double_t px, py, pz, e = fInitialEnergy;
  px = 0.;
  py = 0.;
  pz = TMath::Sqrt(e * e - m * m);
  fInitialMomentum.SetXYZT(px, py, pz, e);
  */

  /*
  cout << "Generated primary:" << endl;
  cout << "pdg = " << pdg
       << "; px = " << px
       << "; py = " << py
       << "; pz = " << pz
       << "; e  = " << e
       << endl;
  */
  // generate one pythia event
  if (fUsePythia) {
    while (!fPythia->next()) {
      continue;
    }

    // Add particles to stack
    for (int i = 1; i < fPythia->event.size(); i++) {
      if (!fPythia->event[i].isFinal()) {
        continue;
      }
      pdg = fPythia->event[i].id();
      px = fPythia->event[i].px();
      py = fPythia->event[i].py();
      pz = fPythia->event[i].pz();
      e = fPythia->event[i].e();
      // if (pdg == 22) { // photons from pythia
      fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx,
                        poly, polz, kPPrimary, ntr, 1., 0);
      //}
    }
  }
  firstCall = false;
}

//_____________________________________________________________________________
void Ex01MCApplication::BeginEvent() {
  /// User actions at beginning of event.
  fStack->Reset();
  fEnDepTarget = 0;
  fEnergyWhenExitingTarget = 0;
  fEnDepOutTarget = 0;
  fEnergyOfSecondariesInTarget = 0;
  fDeltaTheta = 0;
  for (int i = 0; i < 9; i++) {
    fEnDepInBGO[i] = 0;
    fEnDepInNaI[i] = 0;
  }
  for (int i = 0; i < 6; i++)
    fEnDepInCPV[i] = 0;
  fEnDepInSF = 0;
}

//_____________________________________________________________________________
void Ex01MCApplication::BeginPrimary() {
  /// User actions at beginning of a primary track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::PreTrack() {
  /// User actions at beginning of each track.
  /// Print info message.

  // cout << endl;
  // cout << "Starting new track" << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::Stepping() {
  /// User actions at each step.
  TLorentzVector trackMomentum;
  gMC->TrackMomentum(trackMomentum);
  // inside target
  if (strcmp(gMC->CurrentVolName(), "TARGET") == 0) {
    fEnDepTarget += gMC->Edep();
    int currentTrackNumber = fStack->GetCurrentTrackNumber();
    // primary track
    if (currentTrackNumber == 0) {
      // leaving target
      if (gMC->IsTrackExiting()) {
        fEnergyWhenExitingTarget = trackMomentum.T();
        fDeltaTheta = trackMomentum.Theta() - fInitialMomentum.Theta();
      }
    } else { // secondary track
      // new secondary track produced by primary photon inside target
      if (gMC->IsNewTrack() && fStack->GetCurrentParentTrackNumber() == 0) {
        fEnergyOfSecondariesInTarget += trackMomentum.T();
      }
    }
  } else { // outside target
    fEnDepOutTarget += gMC->Edep();
  }
  // deposed energy in NaI crystall
  if (strcmp(gMC->CurrentVolName(), "crystallNaI") == 0) {
    int copyNo;
    gMC->CurrentVolOffID(2, copyNo);
    // cout << "Path: " << gMC->CurrentVolPath() << "; volId = " <<
    // gMC->CurrentVolOffID(2, copyNo) << "; copy number = " << copyNo << endl;
    fEnDepInNaI[copyNo - 1] += gMC->Edep();
  }
  // deposed energy in BGO crystall
  if (strcmp(gMC->CurrentVolName(), "crystallBGO") == 0) {
    int copyNo;
    gMC->CurrentVolOffID(2, copyNo);
    // cout << "Path: " << gMC->CurrentVolPath() << "; volId = " <<
    // gMC->CurrentVolOffID(2, copyNo) << "; copy number = " << copyNo << endl;
    fEnDepInBGO[copyNo - 10] += gMC->Edep();
  }
  // deposed energy in CPV
  if (strcmp(gMC->CurrentVolName(), "plastCPV") == 0) {
    int copyNo;
    // cout << "Path: " << gMC->CurrentVolPath() << "; volId = " <<
    // gMC->CurrentVolID(copyNo) << "; copy number = " << copyNo << endl;
    gMC->CurrentVolID(copyNo);
    fEnDepInCPV[copyNo] += gMC->Edep();
  }
  // deposed energy in S_F
  if (strcmp(gMC->CurrentVolName(), "S_F") == 0) {
    fEnDepInSF += gMC->Edep();
  }

  /// Print track position, the current volume and current medium names.
  /*
  TLorentzVector position;
  gMC->TrackPosition(position);

  cout << "Track " << position.X() << " " << position.Y() << " " << position.Z()
       << "  in " << gMC->CurrentVolName() << "  ";

  if (gMC->CurrentMedium() == fImed) cout << "ArgonGas";
  if (gMC->CurrentMedium() == fImedAl) cout << "Aluminium";
  if (gMC->CurrentMedium() == fImedPb) cout << "Lead";

  cout << endl;
  */
  // // Test other TVirtualMC::TrackPosition() functions

  // Double_t dx, dy, dz;
  // gMC->TrackPosition(dx, dy, dz);

  // Float_t x, y, z;
  // gMC->TrackPosition(x, y, z);

  // cout << "Track position (double): "  << dx << " " << dy << " " << dz
  //      << "  (float): "   << x << " " << y << " " << z << endl;
}

//_____________________________________________________________________________
void Ex01MCApplication::PostTrack() {
  /// User actions after finishing of each track
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishPrimary() {
  /// User actions after finishing of a primary track.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void Ex01MCApplication::FinishEvent() {
  /// User actions after finishing of an event
  /// Nothing to be done this example
  hEnDepInAir->Fill(fEnDepOutTarget);
  hEnDepInTarget->Fill(fEnDepTarget);
  hEnergyWhenExitingTarget->Fill(fEnergyWhenExitingTarget);
  hEnergyOfSecondariesInTarget->Fill(fEnergyOfSecondariesInTarget);
  hDeltaTheta->Fill(fDeltaTheta);
  double sumEnBGO = 0, sumEnNaI = 0, sumEnCPV = 0;
  static double kNaI = 0.3586, kBGO = 0.03586;
  static auto measuredE = [](double e, double k) {
    double mu = k * e * 1e6;
    double eMes = (mu + TMath::Sqrt(mu) * gRandom->Gaus(0., 1.)) / k * 1e-6;
    return eMes;
  };
  for (int i = 0; i < 9; i++) {
    hEnDepInBGO[i]->Fill(fEnDepInBGO[i]);
    hMesEnInBGO[i]->Fill(measuredE(fEnDepInBGO[i], kBGO));
    sumEnBGO += fEnDepInBGO[i];
    hEnDepInNaI[i]->Fill(fEnDepInNaI[i]);
    hMesEnInNaI[i]->Fill(measuredE(fEnDepInNaI[i], kNaI));
    sumEnNaI += fEnDepInNaI[i];
  }
  hSumEnDepInBGO->Fill(sumEnBGO);
  hSumEnDepInNaI->Fill(sumEnNaI);

  for (int i = 0; i < 6; i++) {
    sumEnCPV += fEnDepInCPV[i];
    hEnDepInCPV[i]->Fill(fEnDepInCPV[i]);
  }
  hSumEnDepInCPV->Fill(sumEnCPV);

  hEnDepInSF->Fill(fEnDepInSF);

  // trigger condition
  bool isCPVTriggered[6];
  for (int i = 0; i < 6; i++) {
    isCPVTriggered[i] = false;
    if (fEnDepInCPV[i] >= fCPVThreshold)
      isCPVTriggered[i] = true;
  }
  for (int i = 0; i < 9; i++) {
    if (!isCPVTriggered[i / 3]) {
      hMesEnInNaITriggered[i]->Fill(measuredE(fEnDepInNaI[i], kNaI));
      hEnDepInNaITriggered[i]->Fill(fEnDepInNaI[i]);
    }
    if (!isCPVTriggered[i / 3 + 3]) {
      hMesEnInBGOTriggered[i]->Fill(measuredE(fEnDepInBGO[i], kBGO));
      hEnDepInBGOTriggered[i]->Fill(fEnDepInBGO[i]);
    }
  }
}

//_____________________________________________________________________________

