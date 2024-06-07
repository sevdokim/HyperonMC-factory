void run_g3_control(Int_t targMaterial = 0, Int_t nev = 1000000,
                    int ExTarget = -1,
                    int ExReson = -1, int ExChanel = -1, double control = 0,
                    int seed = 0,
                    const TString &configMacro = "g3tgeoConfig.C") {
  // MC application
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
  if (targMaterial) {
    appl->SetTargetMaterial(targMaterial);
  }
  gRandom->SetSeed(seed); // randomizer
  appl->SetMomentum(7.);
  appl->SetExTarget(ExTarget);
  appl->SetExReson(ExReson);
  appl->SetExChanel(ExChanel);
  appl->SetDebug(0);

  TString MCres = "MC_res.dat";
  TString Hist = "Histos.root";

  appl->SetMCResults(MCres);
  appl->SetHistosRoot(Hist);
  // set eventgen control parameter
  appl->SetControl(control);

  // Initialize & run MC
  appl->InitMC(configMacro);
  appl->RunMC(nev);
  cout << "done" << endl;
  cout.flush();
}
