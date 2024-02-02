void runSim(Int_t targMaterial = 0, Int_t nev = 1000000,
	    const char *suffix = "", int ExTarget = -1,
	    int ExReson = -1, int ExChanel = -1, double control = 0,
	    int seed = 0,
	    const char *configMacro = "g3tgeoConfig.C") {

  gROOT->ProcessLine(".x load_g3.C");
  gROOT->ProcessLine(".L run_g3_control.C");
  gROOT->ProcessLine(Form("run_g3_control(%d, %d, \"%s\", %d, %d, %d, %lf, %d, \"%s\")", targMaterial, nev, suffix, ExTarget, ExReson, ExChanel, control, seed, configMacro));
}

