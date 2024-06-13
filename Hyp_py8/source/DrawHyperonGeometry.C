void DrawHyperonGeometry() {
  HyMCApplication *appl = new HyMCApplication(
      "HyMC_py8", "The Hyperon MC application with Pythia8");
  // Target material
  // Be = 2
  // C  = 10
  // Al = 3
  // Cu = 9
  // Sn = 8
  // Pb = 7
  // Poly = 11
  appl->SetTargetMaterial(2);
  gRandom->SetSeed(0); // randomizer
  appl->SetMomentum(7.);
  appl->SetExTarget(1);
  appl->SetExReson(1);
  appl->SetExChanel(1);
  appl->SetDebug(0);

  TString MCres = "MC_res.dat";
  TString Hist = "Histos.root";

  appl->SetMCResults(MCres);
  appl->SetHistosRoot(Hist);
  // set eventgen control parameter
  appl->SetControl(0);

  // Initialize MC
  const TString &configMacro = "g3tgeoConfig.C";
  appl->InitMC(configMacro);

  gGeoManager->SetVisOption(0);
  gGeoManager->SetVisLevel(7);
  gGeoManager->FindVolumeFast("LGD2")->SetTransparency(80);
  gGeoManager->FindVolumeFast("LGD2IN")->SetTransparency(99);
  gGeoManager->FindVolumeFast("GLASLEFT_DIVIDED")->SetTransparency(100);
  gGeoManager->FindVolumeFast("GLASLEFT_XDIV")->SetTransparency(100);
  gGeoManager->FindVolumeFast("GLASLEFT_DIVIDED")->SetTransparency(100);
  gGeoManager->FindVolumeFast("SM_SENS_GLAS")->SetLineColor(kRed);
  gGeoManager->FindVolumeFast("LA_SENS_GLAS")->SetLineColor(kBlue);
  // gGeoManager->FindVolumeFast("GLACENTR_DIVIDED")->SetLineColor(kCyan);
  gGeoManager->FindVolumeFast("LA_SENS_GLAS")->SetTransparency(1);
  gGeoManager->FindVolumeFast("SM_SENS_GLAS")->SetTransparency(1);
  gGeoManager->GetTopVolume()->Draw(/*"ogl"*/);
}
