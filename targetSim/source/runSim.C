void runSim(int nEvents = 100000, int seed = 0) {
  gROOT->ProcessLine(".x ../load_g4.C");
  gROOT->ProcessLine(".L ../run_g4.C");
  gROOT->ProcessLine(Form("run_g4(%d, %d)", nEvents, seed));
}