// first load nesessary libraries like this:
// root -l load_g3.C run_g3_control.C

void run_g3_control(Int_t targMaterial = 0, Int_t nev = 1000000,
                    int ExTarget = -1, int ExReson = -1, int ExChanel = -1,
                    double control = 0, int seed = 0,
                    const TString &configMacro = "g3tgeoConfig.C") {
  /// Macro function for running HyperonMC with Geant3 from
  /// Root interactive session

  // MC application
  HyMCApplication *appl = new HyMCApplication(
      "Hyperon_geometry", "The Hyperon MC application#0", seed);
  // Target material
  // Be = 2
  // C  = 10
  // Al = 3
  // Cu = 9
  // Sn = 8
  // Pb = 7
  // Poly = 11
  if (targMaterial)
    appl->SetTargetMaterial(targMaterial);
  gRandom->SetSeed(seed); // randomizer

  // appl->SetMomentum(7);
  appl->SetMomentum(7.200);
  appl->SetPCNumber(0);
  appl->SetExTarget(ExTarget);
  appl->SetExReson(ExReson);
  appl->SetExChanel(ExChanel);

  // Initialize MC
  appl->InitMC(configMacro);

  gMC->SetCollectTracks(kFALSE);
  // Run MC  appl->SetHistosRoot("histos_0.root");
  appl->SetMCResults("MC_res.dat");
  appl->SetHistosRoot("Histos.root");
  appl->SetMCgendat("MCgen.dat");
  // set eventgen control parameter(see generator source code in hyp_RegGen.f
  appl->SetControl(control);
  appl->RunMC(nev);
  cout << "done" << endl;
  cout.flush();
}
