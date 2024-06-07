#include "HyMCApplication.h"
#include <Math/Vector4D.h>

ClassImp(HyMCApplication)

using namespace std;
using Momentum4D = ROOT::Math::PxPyPzEVector;
//_____________________________________________________________________________
HyMCApplication::HyMCApplication(const char *name, const char *title)
    : TVirtualMCApplication(name, title) {
  /// Standard constructor
  /// \param name   The MC application name
  /// \param title  The MC application description

  EvntNumb = 0;
  evWr = 0;
  // create a user stack
  fStack = new HyMCStack(1000000);

  fLaNx = fDetConstruction.GetGeometry()
              .GetLaNx(); // Arrays of energy deposition in LGD2
  fLaNy = fDetConstruction.GetGeometry().GetLaNy();
  fSmNx = fDetConstruction.GetGeometry().GetSmNx();
  fSmNy = fDetConstruction.GetGeometry().GetSmNy();
  LaEnDep = new Double_t *[fLaNx];
  for (Int_t tmp = 0; tmp < fLaNx; tmp++)
    LaEnDep[tmp] = new Double_t[fLaNy];
  SmEnDep = new Double_t *[fSmNx];
  for (Int_t tmp = 0; tmp < fSmNx; tmp++)
    SmEnDep[tmp] = new Double_t[fSmNy];

  initial_photon_energy = new Double_t[100];
  energy_dep = new Double_t *[100];
  for (Int_t tmp = 0; tmp < 100; tmp++)
    energy_dep[tmp] = new Double_t[640];
}
//_____________________________________________________________________________
HyMCApplication::~HyMCApplication() {
  /// Destructor

  delete gMC;
}

//
// private methods
// *** Break *** segmentation violation

//_____________________________________________________________________________
//
// public methods
//
void HyMCApplication::PreInit() {
  if (fDebug)
    cout << endl << "-----------PreInit-----------" << endl;
}
//_____________________________________________________________________________
void HyMCApplication::AddParticles() {
  // add new particle (positively charged missing mass)
  // gMC->DefineParticle(992212,"MisMass",kPTHadron,0,3.,0.);
  // Float_t bratio[6]={0}; Int_t mode[6][3]={0};
  // bratio[0] = 0.2; //p+ pi0
  // bratio[1] = 0.2; //n pi+
  // bratio[2] = 0.2; //p+ pi+ pi-
  // bratio[3] = 0.2; //p+ pi0 pi0
  // bratio[4] = 0.2; //n pi+ pi0

  // mode[0][0] = 2212; mode[0][1] = 111;//p+ pi0
  // mode[1][0] = 2112; mode[1][1] = 211;//n pi+
  // mode[2][0] = 2212; mode[2][1] = 211; mode[2][2] = -211;//p+ pi+ pi-
  // mode[3][0] = 2212; mode[3][1] = 111; mode[3][2] = 111;//p+ pi0 pi0
  // mode[4][0] = 2112; mode[4][1] = 211; mode[4][2] = 111;//n pi+ pi0

  // gMC->SetDecayMode(2214,bratio,mode);
}
//_____________________________________________________________________________
bool HyMCApplication::InitMC(const char *setup) {
  /// Initialize MC, return true in case of success.
  /// The selection of the concrete MC is done in the macro. (use g3tgeo)
  /// \param setup The name of the configuration macro

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
  // My cuts in VMC style; just comment them for default parameters
  gMC->SetCut("CUTGAM", 0.001);
  gMC->SetCut("CUTELE", 0.001);
  gMC->SetCut("BCUTE", 0.001);
  gMC->SetCut("BCUTM", 0.001);
  gMC->SetStack(fStack);
  gMC->Init();
  gMC->BuildPhysics();

  cout << "is collect tracks = " << gMC->IsCollectTracks() << endl;
  fMC_results = fopen(fMCResults, "wr");
  fprintf(fMC_results, "b ");
  fFile = new TFile(fHistosRoot, "RECREATE");

  // fSavingTree = new TTree("HyMCResults", "Hyperon MC events");

  fLGD2EnergyHisto =
      new TH1F("LGD2EnergyHisto", "Registered LGD2 energy in event", 1000, 0.,
               10.); // Total energy dep. in LGD2
  fSaEnergyHisto = new TH1F("SaEnergyHisto", "Registered Sa energy in event",
                            10000, 0, 1); // Total energy dep. in Sa
  fS4EnergyHisto = new TH1F("S4EnergyHisto", "Registered S4 energy in event",
                            10000, 0., 1); // Total energy dep. in S4
  fEffMassHisto = new TH1F("hMass", "Mass of photon system", 2000, 0., 2.);
  fTargetEnergyHisto =
      new TH1F("TargetEnergyHisto", "target energy in event", 10000, 0., 1);

  fPhotonVertexZPosition =
      new TH1F("PhotonVertexZPositionHisto", "vertex Z pos", 10000, -100., 100);
  fPtHisto = new TH1F("hPt", "Pt of photon system", 5000, 0., 5.);
  fCoordHisto = new TH2F("hCoordHisto", "Coordinates Hist", 100, -10., 10., 100,
                         -10., 10.);

  // Hyperon MC event container (up to 20 primary particles)
  // fHyMCEvent = new HyMCEvent(20);
  // fSavingTree->Branch("HyMCEvents", fHyMCEvent);

  // Sa efficiency from environment variable
  char *env_val;
  env_val = getenv("EFFICIENCY_SA");
  if (env_val != NULL) {
    Double_t val;
    sscanf(env_val, "%lf", &val);
    cout << "EFFICIENCY_SA =  " << val << endl;
    if (val >= 0. && val <= 1.) {
      fSaEfficiency = val;
    } else {
      cout << "Sa efficiency must be within [0., 1.] interval. No changing anything (i.e. fSaEfficiency = 1.)" << endl;
    }
  }

  // config pythia
  fPythia = new Pythia8::Pythia();
  fPythia->readFile("py8Config.cfg");
  int pySeed = gRandom->TRandom::GetSeed();
  if (pySeed <= 0) {
    pySeed = gRandom->Integer(900000000);
  }
  fPythia->readString("Random:setSeed = on");
  fPythia->readString(Form("Random:seed = %d", pySeed));
  fPythia->readString(Form("Beams:idA = %d", fBeamPdg)); // beam particle
  fPythia->readString(Form("Beams:idB = %d", fTargetPdg));     // target particle
  double beamMass = TDatabasePDG::Instance()->GetParticle(fBeamPdg)->Mass();
  double beamEnergy = TMath::Sqrt(fMomentum * fMomentum + beamMass * beamMass);
  fPythia->readString(Form("Beams:eA = %lf", beamEnergy));
  fPythia->readString("Beams:eB = 0.");
  fPythia->readString("Beams:frameType = 2");
  
  if (!fPythia->init()) {
    cerr << " Pythia failed to initialize." << endl;
    return false;
  }
  return true;
}

//_____________________________________________________________________________
void HyMCApplication::RunMC(Int_t nofEvents) {
  /// Run MC.
  /// \param nofEvents Number of events to be processed

  cout << "now " << nofEvents
       << " events will be processed. seed = " << gRandom->TRandom::GetSeed()
       << endl;

  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void HyMCApplication::FinishRun() {
  /// Finish MC run.
  cout << "I wrote " << evWr << "events" << endl;
  fclose(fMC_results);
  fFile->Write();
  fFile->Close();
  fPythia->stat();
}

//_____________________________________________________________________________
void HyMCApplication::ConstructGeometry() {
  /// Construct geometry using detector contruction class.
  /// The detector contruction class is using TGeo functions
  cout << "Geometry will be defined via VMC" << endl;
  fDetConstruction.ConstructMaterials();
  fDetConstruction.ConstructVolumes();
}

//_____________________________________________________________________________
void HyMCApplication::InitGeometry() {
  /// Initialize geometry
}

//_____________________________________________________________________________
void HyMCApplication::GeneratePrimaries() {
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  // Added 09.02.2011

  // Track ID (filled by stack)
  Int_t ntr;

  // Option: to be tracked
  Int_t toBeDone = 1;

  // Polarization
  Double_t polx = 0.;
  Double_t poly = 0.;
  Double_t polz = 0.;

  //
  Double_t vertex[3] = {0., 0., 0.};
  fDetConstruction.GambleVertex(vertex);

  // generate one pythia event
  while (!fPythia->next()) {
    continue;
  }
  if (fDebug) {
    fPythia->info.list();
  }
  int pdg;
  double px, py, pz, e, vx, vy, vz, tof;
  // Add particles to stack
  int nFinal = 0, nPrimaries = 0;
  Momentum4D pLab;
  for (int i = 0; i < fPythia->event.size(); i++) {
    if (!fPythia->event[i].isFinal()) {
      continue;
    }
    nPrimaries++;
  }
  /*fHyMCEvent->NewEvent(nPrimaries);
  fHyMCEvent->pdgA = fPythia->event[1].id();
  fHyMCEvent->eA = fPythia->event[1].e();
  fHyMCEvent->pxA = fPythia->event[1].px();
  fHyMCEvent->pyA = fPythia->event[1].py();
  fHyMCEvent->pzA = fPythia->event[1].pz();

  fHyMCEvent->pdgB = fPythia->event[2].id();
  fHyMCEvent->eB = fPythia->event[2].e();
  fHyMCEvent->pxB = fPythia->event[2].px();
  fHyMCEvent->pyB = fPythia->event[2].py();
  fHyMCEvent->pzB = fPythia->event[2].pz();

  for (int i = 0; i < 3; i++) {
    fHyMCEvent->vertex[i] = vertex[i];
    }*/
  for (int i = 0; i < fPythia->event.size(); i++) {
    if (!fPythia->event[i].isFinal()) {
      continue;
    }
    pdg = fPythia->event[i].id();
    px = fPythia->event[i].px();
    py = fPythia->event[i].py();
    pz = fPythia->event[i].pz();
    e = fPythia->event[i].e();
    pLab += Momentum4D(px, py, pz, e);

    vx = vertex[0] + fPythia->event[i].xProd() / 10.; // mm -> cm
    vx = vertex[1] + fPythia->event[i].yProd() / 10.; // mm -> cm
    vx = vertex[2] + fPythia->event[i].zProd() / 10.; // mm -> cm
    tof = fPythia->event[i].tProd();
    if (fDebug > 1) {
      cout << " pdg = " << pdg << " with (px, py, pz, e) = (" << px << ", "
           << py << ", " << pz << ", " << e << ")" << endl;
    }
    fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx,
                      poly, polz, kPPrimary, ntr, 1., 0);
    initial_photon_energy[nFinal] = e;
    /*fHyMCEvent->pdgCode[nFinal] = pdg;
    fHyMCEvent->pxP[nFinal] = px;
    fHyMCEvent->pyP[nFinal] = py;
    fHyMCEvent->pzP[nFinal] = pz;
    fHyMCEvent->eP[nFinal] = e;*/
    nFinal++;
  }
  if (fDebug > 1) {
    cout << "Number of final particles: " << nFinal << endl;
    cout << "Sum 4-momentum: (" << pLab.x() << ", " << pLab.y() << ", "
         << pLab.z() << ", " << pLab.t() << "), sqrt(s) = " << pLab.M() << endl;
  }
  /*
    float Pt;

    Double_t px, py, pz, e, x, y, z, p;
    Momentum4D Plab(0., 0., 0., 0.);
    int pdg;

    if (pdg == 100)
      pdg = 0; // geantino
    fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0] + x,
                      vertex[1] + y, vertex[2] + z, 0., polx, poly, polz,
                      kPPrimary, ntr, 1., 0); // adding particle to stack
    initial_photon_energy[primariesCounter] = e;
    primariesCounter++;
  */

  // backward going pion to simulate beam energy deposition in Sa
  pdg = 211; // pi+
  ROOT::Math::PxPyPzMVector pBeam(0., 0., fMomentum, 0.13957061);
  px = -pBeam.X();
  py = -pBeam.Y();
  pz = -pBeam.Z();
  e = pBeam.T();
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0], vertex[1],
                    vertex[2], 0., polx, poly, polz, kPPrimary, ntr, 1., 0);
  currentPrimaryIndex = nFinal - 1 + 1; //+1 is bacward going pion
  fNPrimary = currentPrimaryIndex;
  max_gamma = nFinal;
  fFinishTracking = false;
}
//_____________________________________________________________________________
void HyMCApplication::BeginEvent() {
  /// User actions at beginning of event.

  for (Int_t i = 0; i < fLaNx; i++) {
    for (Int_t j = 0; j < fLaNy; j++)
      LaEnDep[i][j] = 0;
  }

  for (Int_t i = 0; i < fSmNx; i++) {
    for (Int_t j = 0; j < fSmNy; j++)
      SmEnDep[i][j] = 0;
  }
  fTargetEnergyDep = 0;
  fSaEnergyDep = 0;
  fS4EnergyDep = 0;

  for (Int_t i = 0; i < 100; i++) {
    for (Int_t j = 0; j < 640; j++)
      energy_dep[i][j] = 0;
  }

  gGeoManager->ClearTracks();
  // currentPrimaryIndex = 0;
}

//_____________________________________________________________________________
void HyMCApplication::BeginPrimary() {
  /// User actions at beginning of a primary track.
  /// Nothing to be done this example
  if (fDebug)
    cout << "begin primary" << endl;
  if (fDebug == 2) { // print track info
    std::cout << "  Particle = ";
    TParticlePDG *particle =
        TDatabasePDG::Instance()->GetParticle(gMC->TrackPid());
    if (particle)
      std::cout << particle->GetName() << "  ";
    else
      std::cout << "unknown"
                << "  ";
    std::cout << "   Track ID = " << gMC->GetStack()->GetCurrentTrackNumber()
              << "  ";
    std::cout << "   Parent ID = "
              << gMC->GetStack()->GetCurrentParentTrackNumber() << endl;
  }
}

//_____________________________________________________________________________
void HyMCApplication::PreTrack() {
  /// User actions at beginning of each track.
  /// Nothing to be done this example
  // TVirtualGeoTrack* track =
  // gGeoManager->MakeTrack(fStack->GetCurrentTrackNumber(),
  // fStack->GetCurrentTrack()->GetPdgCode(), fStack->GetCurrentTrack());
  //  gGeoManager->AddTrack(track);
}

//_____________________________________________________________________________
void HyMCApplication::Stepping() {
  /// User actions at each step

  if (fFinishTracking) {
    gMC->StopTrack();
    return;
  }

  if (currentPrimaryIndex ==
      fNPrimary) { // this is backward going pion; we need it only to simulate
                   // Sa energy deposition.
    if (strcmp(gMC->CurrentVolName(), "SATR") != 0)
      gMC->StopTrack();
  }
  // Printing current track position
  if (fDebug == 3) {
    TLorentzVector position;
    gMC->TrackPosition(position);

    cout << "Track " << position.X() << " " << position.Y() << " "
         << position.Z() << "  in " << gMC->CurrentVolName() << "  ";
    cout << endl;
  }
  static Int_t Ix, Iy, tmp;

  // Small glasses energy lose calculation
  if ((strcmp(gMC->CurrentVolName(), "SM_SENS_GLAS") == 0)) { /*Center*/
    // 	cout<<"centre"<<endl;

    gMC->CurrentVolOffID(1, Iy);
    gMC->CurrentVolOffID(2, Ix);
    SmEnDep[Ix - 1][Iy - 1] += gMC->Edep();
    // cout<<"Ix= "<<Ix<<endl;
    // cout<<"Iy= "<<Iy<<endl;

    energy_dep[currentPrimaryIndex][Ix - 1 + (Iy - 1) * 8 + 576] += gMC->Edep();
  }

  // Large glasses energy lose calculation
  if (strcmp(gMC->CurrentVolName(), "LA_SENS_GLAS") == 0) {
    gMC->CurrentVolID(tmp);
    if (tmp == 1) {
      gMC->CurrentVolOffID(3, tmp);
      if (tmp == 1) { /*Left*/
        //		cout<<"left"<<endl;
        //	cout<<"tmp= "<< tmp<<endl;

        gMC->CurrentVolOffID(1, Iy); /**/
        gMC->CurrentVolOffID(2, Ix); /**/
        // cout<<"Ix= "<<Ix<<endl;
        //   cout<<"Iy= "<<Iy<<endl;

        LaEnDep[Ix - 1][Iy - 1] += gMC->Edep();
      } else { /*Right*/
        //		cout<<"right"<<endl;
        //	cout<<"tmp= "<< tmp<<endl;

        gMC->CurrentVolOffID(1, Iy);        /**/
        gMC->CurrentVolOffID(2, tmp);       /**/
        Ix = tmp + fLaNx / 2. + fSmNx / 4.; /**/
        //	cout<<"tmp= "<< tmp<<endl;

        //   cout<<"Ix= "<<Ix<<endl;
        //   cout<<"Iy= "<<Iy<<endl;

        LaEnDep[Ix - 1][Iy - 1] += gMC->Edep();
      }
    } else {
      gMC->CurrentVolOffID(3, tmp);
      if (tmp == 1) { /*Top*/
        //	cout<<"top"<<endl;
        //	cout<<"tmp= "<< tmp<<endl;
        gMC->CurrentVolOffID(1, tmp);       /**/
        Iy = tmp + fLaNy / 2. + fSmNy / 4.; /**/
        gMC->CurrentVolOffID(2, tmp);       /**/
        Ix = tmp + fLaNx / 2. - fSmNx / 4.; /**/
        //	cout<<"Ix= "<<Ix<<endl;
        //	cout<<"Iy= "<<Iy<<endl;

        LaEnDep[Ix - 1][Iy - 1] += gMC->Edep();
      } else { /*Bottom*/
        //		cout<<"bottom"<<endl;
        //	cout<<"tmp= "<< tmp<<endl;

        gMC->CurrentVolOffID(1, Iy);        /**/
        gMC->CurrentVolOffID(2, tmp);       /**/
        Ix = tmp + fLaNx / 2. - fSmNx / 4.; /**/
        //	cout<<"Ix= "<<Ix<<endl;
        //	cout<<"tmp= "<<tmp<<endl;
        //	cout<<"Iy= "<<Iy<<endl;
        LaEnDep[Ix - 1][Iy - 1] += gMC->Edep();
      }
    }
    energy_dep[currentPrimaryIndex][Ix - 1 + (Iy - 1) * 24] += gMC->Edep();
  }
  // cout<<"Current Parent Track Number =
  // "<<fStack->GetCurrentParentTrackNumber()<<endl;
  // cout<<"is collect tracks = "<< gMC->IsCollectTracks()<<endl;

  // Sa energy loss calculation
  if (strcmp(gMC->CurrentVolName(), "SATR") == 0) {
    fSaEnergyDep += gMC->Edep();
  }

  // S4 energy loss calculation
  if (strcmp(gMC->CurrentVolName(), "S4TR") == 0) {
    fS4EnergyDep += gMC->Edep();
  }

  // Target energy loss calculation
  if (strcmp(gMC->CurrentVolName(), "TARG") == 0) {
    fTargetEnergyDep += gMC->Edep();
  }
  if (fSaEnergyDep > fSaEnergyCut)
    fFinishTracking = true;
}

//_____________________________________________________________________________
void HyMCApplication::PostTrack() {
  /// User actions at each step.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void HyMCApplication::FinishPrimary() {
  /// User actions after finishing of a primary track.
  currentPrimaryIndex--;
}

//_____________________________________________________________________________
void HyMCApplication::FinishEvent() {
  // visualisation. just comment it if you don't want to visualize.
  // gMC->Gdraw("WRLD", 90., 180.);
  // cout<<"=========visualisation========="<<endl;
  // gGeoManager->SetVisOption(0);
  // gGeoManager->SetTopVisible();
  // gGeoManager->GetTopVolume()->Draw();
  // gGeoManager->DrawTracks("/\*");  // this means all tracks

  // Histo for total energy in LGD2
  Double_t EnTotal;
  EnTotal = 0;
  Int_t i;

  for (i = 0; i < fLaNx; i++) {
    for (Int_t j = 0; j < fLaNy; j++) {
      EnTotal += LaEnDep[i][j];
      //      e[j+i*LaNy]=float(LaEnDep[i][j]); // Eqvivalence: i =
      //      ix+(iy-1)*24 for large glasses
    }
  }

  for (i = 0; i < fSmNx; i++) {
    for (Int_t j = 0; j < fSmNy; j++) {
      EnTotal += SmEnDep[i][j];
      //      e[j+i*SmNy+fLaNx*LaNy]=float(SmEnDep[i][j]);// Eqvivalence: i =
      //      ix+(iy-1)*8+24*24 for small glasses
    }
  }

  // prepair collected energy
  // otbor po schetchiku Sa
  // fHyMCEvent->signalSa = fSaEnergyDep;
  // fHyMCEvent->signalS4 = fS4EnergyDep;
  // fHyMCEvent->targetEnDep = fTargetEnergyDep;
  if (fSaEnergyDep < fSaEnergyCut && gRandom->Rndm() < fSaEfficiency) { // begin Sa-discrimination
    fprintf(fMC_results, "%d ", EvntNumb);
    for (i = 0; i < max_gamma; i++) {
      fprintf(fMC_results, "%lf ", initial_photon_energy[i]);
      for (Int_t j = 0; j < 640; j++) {
        if (energy_dep[i][j] > 0) {
          fprintf(fMC_results, "%d %lf ", j, energy_dep[i][j]);
          // fHyMCEvent->cellNumber[i].push_back(j);
          // fHyMCEvent->enDep[i].push_back(energy_dep[i][j]);
        }
      }
      if ((i + 1) != max_gamma) {
        fprintf(fMC_results, "%d & ", -1);
      } else
        fprintf(fMC_results, "%d\n# ", -1);
    }
    // Writing data to file runXXXX.gz
    // no hyperon format writing in this version
    // convert MC_results.dat with ../converter/exe
    evWr++;
  } // end Sa-discrimination
  // fSavingTree->Fill();

  if (fLGD2EnergyHisto && (EnTotal > 0))
    fLGD2EnergyHisto->Fill(EnTotal);
  if (fDebug)
    cout << "Total energy = " << EnTotal << " GeV" << endl;

  // Histo for energy loses in Sa
  if (fSaEnergyHisto)
    fSaEnergyHisto->Fill(fSaEnergyDep);

  // Histo for energy loses in S4
  if (fS4EnergyHisto)
    fS4EnergyHisto->Fill(fS4EnergyDep);

  // Histo for energy loses in target
  if (fTargetEnergyHisto)
    fTargetEnergyHisto->Fill(fTargetEnergyDep);
  //  cout<<"chislo chastic v steke="<<fStack->GetNtrack()<<endl;
  // cout<<"chislo pervichnih chastic="<<fStack->GetNprimary()<<endl;
  fStack->Reset();
  // fHyMCEvent->ResetEvent();
  // gObjectTable->Print();

  cout << " \nFinished event " << EvntNumb << endl;
  EvntNumb++;
}
