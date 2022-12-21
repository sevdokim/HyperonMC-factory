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
  fStack = new Ex01MCStack(10000);

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
}

//_____________________________________________________________________________
void Ex01MCApplication::ConstructVolumes() {
  /// Contruct volumes using TGeo modeller

  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis

  Double_t *ubuf = 0;

  Double_t expHall[3];
  expHall[0] = 300.;
  expHall[1] = 100.;
  expHall[2] = 100.;
  TGeoVolume *top = gGeoManager->Volume("EXPH", "BOX", fImedAir, expHall, 3);
  gGeoManager->SetTopVolume(top);

  //------------------------------ a tracker tube

  Double_t targetSize[3];
  targetSize[0] = 0.;
  targetSize[1] = fTargetRadius;
  targetSize[2] = fTargetThickness / 2.;
  gGeoManager->Volume("TARGET", "TUBE", fImedTarget, targetSize, 3);

  Double_t posX = 0.;
  Double_t posY = 0.;
  Double_t posZ = 0.;
  gGeoManager->Node("TARGET", 1, "EXPH", posX, posY, posZ, 0, kTRUE, ubuf);

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

  // Track ID (filled by stack)
  Int_t ntr;

  // Option: to be tracked
  Int_t toBeDone = 1;

  // photon
  Int_t pdg = 22;

  // Polarization
  Double_t polx = 0.;
  Double_t poly = 0.;
  Double_t polz = 0.;

  // Position
  Double_t vx = 6. * (gRandom->Rndm() - 0.5);
  Double_t vy = 6. * (gRandom->Rndm() - 0.5);
  while (vx * vx + vy * vy >= 9.) {
    vx = 6. * (gRandom->Rndm() - 0.5);
    vy = 6. * (gRandom->Rndm() - 0.5);
  }
  Double_t vz = fTargetThickness * (-0.5 + gRandom->Rndm());
  Double_t tof = 0.;

  // Momentum
  Double_t px, py, pz, e = fInitialEnergy;
  double phi = 2. * TMath::Pi() * gRandom->Rndm();
  double cosTheta = (gRandom->Rndm() - 0.5);
  double sinTheta = TMath::Sqrt(1. - cosTheta * cosTheta);
  px = e * sinTheta * TMath::Sin(phi);
  py = e * sinTheta * TMath::Cos(phi);
  pz = e * cosTheta;
  fInitialMomentum.SetXYZT(px, py, pz, e);

  // Add particle to stack
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx,
                    poly, polz, kPPrimary, ntr, 1., 0);
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
}

//_____________________________________________________________________________
void Ex01MCApplication::TestVMCGeometryGetters() {
  /// Test (new) TVirtualMC functions:
  /// GetTransform(), GetShape(), GetMaterial(), GetMedium()

  // Get transformation of 10th layer
  //
  TString volPath = "/EXPH_1/CALB_1/LAYB_9";
  TGeoHMatrix matrix;
  Bool_t result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  } else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  volPath = "/EXPH_1/CALB_1/LAYB_100";
  result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  } else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  volPath = "/EXPH_1/CALB_1/LAYB_9";
  result = gMC->GetTransformation(volPath, matrix);
  if (result) {
    cout << "Transformation for " << volPath.Data() << ": " << endl;
    matrix.Print();
  } else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  // Get shape
  //
  volPath = "/EXPH_1/CALB_1/LAYB_9";
  TString shapeType;
  TArrayD par;
  result = gMC->GetShape(volPath, shapeType, par);
  if (result) {
    cout << "Shape for " << volPath.Data() << ": " << endl;
    cout << shapeType.Data() << "  parameters: ";
    for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
      cout << par.At(ipar) << ",  ";
    cout << endl;
  } else {
    cerr << "Volume path " << volPath.Data() << " not found" << endl;
  }
  cout << endl;

  // Get material by material ID
  //
  TString matName;
  Int_t imat = 2;
  Double_t a, z, density, radl, inter;
  TArrayD mpar;
#if ROOT_VERSION_CODE >= ROOT_VERSION(5, 30, 0)
  result = gMC->GetMaterial(imat, matName, a, z, density, radl, inter, mpar);
  if (result) {
    cout << "Material with ID " << imat << ": " << endl;
    cout << matName.Data() << "  Aeff = " << a << "  Zeff = " << z
         << "  density = " << density << "  radl = " << radl
         << "  inter = " << inter << endl;
    if (mpar.GetSize() > 0) {
      cout << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl;
    }
  } else {
    cerr << "Material with ID " << imat << " not found" << endl;
  }
  cout << endl;
#endif

  // Get material by volume name
  //
  TString volName = "LAYB";
  mpar.Set(0);
  result = gMC->GetMaterial(volName, matName, imat, a, z, density, radl, inter,
                            mpar);
  if (result) {
    cout << "Material for " << volName.Data() << " volume: " << endl;
    cout << matName.Data() << "  " << imat << "  Aeff = " << a
         << "  Zeff = " << z << "  density = " << density << "  radl = " << radl
         << "  inter = " << inter << endl;
    if (mpar.GetSize() > 0) {
      cout << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl;
    }
  } else {
    cerr << "Volume " << volName.Data() << " not found" << endl;
  }
  cout << endl;

  // Get medium
  //
  TString medName;
  Int_t imed, nmat, isvol, ifield;
  Double_t fieldm, tmaxfd, stemax, deemax, epsil, stmin;
  result = gMC->GetMedium(volName, medName, imed, nmat, isvol, ifield, fieldm,
                          tmaxfd, stemax, deemax, epsil, stmin, mpar);
  if (result) {
    cout << "Medium for " << volName.Data() << " volume: " << endl;
    cout << medName.Data() << "  " << imed << "  nmat = " << nmat
         << "  isvol = " << isvol << "  ifield = " << ifield
         << "  fieldm = " << fieldm << "  tmaxfd = " << tmaxfd
         << "  stemax = " << stemax << "  deemax = " << deemax
         << "  epsil = " << epsil << "  stmin = " << stmin << endl;
    if (mpar.GetSize() > 0) {
      cout << " User defined parameters: ";
      for (Int_t ipar = 0; ipar < par.GetSize(); ipar++)
        cout << mpar.At(ipar) << ",  ";
      cout << endl;
    }
  } else {
    cerr << "Volume " << volName.Data() << " not found" << endl;
  }
  cout << endl;

  // Test getters non-existing position/volume
  //

  // Transformation
  volPath = "/EXPH_1/CALB_1/LAYB_100";
  result = gMC->GetTransformation(volPath, matrix);
  cout << "GetTransformation: Volume path " << volPath.Data();
  if (!result)
    cout << " not found" << endl;
  else
    cout << " found" << endl;

  // Shape
  result = gMC->GetShape(volPath, shapeType, par);
  cout << "GetShape: Volume path " << volPath.Data();
  if (!result)
    cout << " not found" << endl;
  else
    cout << " found" << endl;

  // Material
  volName = "XYZ";
  result = gMC->GetMaterial(volName, matName, imat, a, z, density, radl, inter,
                            mpar);
  cout << "GetMaterial: Volume name " << volName.Data();
  if (!result)
    cout << " not found" << endl;
  else
    cout << " found" << endl;

  // Medium
  result = gMC->GetMedium(volName, medName, imed, nmat, isvol, ifield, fieldm,
                          tmaxfd, stemax, deemax, epsil, stmin, mpar);
  cout << "GetMedium: Volume name " << volName.Data();
  if (!result)
    cout << " not found" << endl;
  else
    cout << " found" << endl;
}
