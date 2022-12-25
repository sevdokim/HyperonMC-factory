//note that ExReson is iReac1 and  ExChanel is iReac2
//extern "C" { int mi_main();}
//  Double_t pout[500][10];  Float_t FlBeam = 7;  Int_t nTries = 1,  iError = 0, jRec = 0, istOut = 0, fortran_seed = 1;  reggen_event_(&FlBeam,&nTries,&iError,&fortran_seed,&jRec,&istOut,&pout[0][0]);
void run_g3_control(Int_t targMaterial = 0,Int_t nev = 1000000, const char* suffix = "",int ExTarget=-1,int ExReson=-1,int ExChanel=-1, double control = 0, int seed = 0, const TString& configMacro = "g3tgeoConfig.C") 
{
/// Macro function for running Example01 with Geant3 from
/// Root interactive session
/// \param configMacro configuration macro name, default \ref E01/g3Config.C 
//   gSystem->Sleep(4e3);

  // Load basic libraries
  gROOT->LoadMacro("./macro/basiclibs.C");
  basiclibs();

  // Load Geant3 libraries
  gROOT->LoadMacro("./macro/g3libs.C");
  g3libs();

  // Load this example library
  //gSystem->Load("libHyperonMC_RegGen");
  gSystem->Load("libpacklib");
  gSystem->Load("libkernlib");
  gSystem->Load("libmathlib");
  gSystem->Load("libRegGen");
  gSystem->Load("libHyperonMC_RegGen");

  // MC application
  HyMCApplication* appl 
    = new HyMCApplication("Hyperon_geometry", "The Hyperon MC application#0", seed);
  // Target material
  // Be = 2
  // C  = 10
  // Al = 3
  // Cu = 9
  // Sn = 8
  // Pb = 7
  // Poly = 11
  if(targMaterial)  appl->SetTargetMaterial(targMaterial);
  gRandom->SetSeed(seed);  //randomizer

  //appl->SetMomentum(7); 
  appl->SetMomentum(7.200);  // Sdv
  appl->SetPCNumber(0);
  appl->SetExTarget(ExTarget);
  appl->SetExReson(ExReson);
  appl->SetExChanel(ExChanel);

  // Initialize MC
  appl->InitMC(configMacro);

  gMC->SetCollectTracks(kFALSE);
  // Run MC  appl->SetHistosRoot("histos_0.root");
  char MCres[400];
  char Hist[400];
  char MCgen[400];
  const char* extentionDAT  = ".dat";
  const char* extentionROOT = ".root";

  for(int i=0; i<=0; i+=1){
    //make new files to save mc_results
    sprintf(MCres,"MC_res",i);
    strcat(MCres,suffix);
    strcat(MCres,extentionDAT);
    sprintf(Hist,"Histos",i);
    strcat(Hist,suffix);
    strcat(Hist,extentionROOT);
    sprintf(MCgen,"MCgen",i);
    strcat(MCgen,suffix);
    strcat(MCgen,extentionDAT);

    appl->SetMCResults(MCres);
    appl->SetHistosRoot(Hist);
    appl->SetMCgendat(MCgen);
    //set eventgen control parameter(see generator source code in fortran_src/hyp_carlo.f)
    appl->SetControl(control);
    //run MC to calculate events with setted control parameter
//    appl->SetConnection("ui0005.m45.ihep.su", 18854);
//    appl->SetConnection("194.190.165.213", 54321);
//    appl->SetConnection("192.168.175.213", 54321);
//    appl->SetConnection("ui0004.m45.ihep.su", 54321);
//    appl->SetConnection("194.190.165.212", 54321);
//    appl->SetConnection("192.168.175.212", 54321);
//    appl->SetConnection("alice16.ihep.su", 18864);
//    appl->SetConnection("10.163.1.222", 18864);
//    appl->ThreadCycle(true);
//    appl->ThreadCycle2(true);
//    gSystem->Sleep(100);
    appl->RunMC(nev);
//    appl->RunMC(nev);
  }
  // delete appl;
//   cout << "Seed = '" <<    appl->GetSeed() << "'\n";
//    appl->SendMessage(appl->make_message(123, "message", "here is the message body !"));
  cout << "done" << endl;
//  gSystem->Sleep(1e3);
//  cout.flush();
//  cout << "done2" << endl;
//  gSystem->Sleep(3e3);
  cout.flush();
}
