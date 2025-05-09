#include "/home/evdokimov/mysw/INSTALL/pythia8/include/Pythia8/HeavyIons.h"
#include "/home/evdokimov/mysw/INSTALL/pythia8/include/Pythia8/Pythia.h"
using LVec = ROOT::Math::PxPyPzEVector;
void run_py8_standalone(int nEvents = 100000) {
  gSystem->Load("libpythia8");
  Pythia8::Pythia *fPythia;
  fPythia = new Pythia8::Pythia();
  int pySeed = gRandom->TRandom::GetSeed();
  fPythia->readString("Random:setSeed = on");
  fPythia->readString(Form("Random:seed = %d", pySeed));
  // fPythia->readString("Diffraction:sigmaRefPomP = 2.0");
  // fPythia->readString("HeavyIon:bWidth = 1.0");
  // fPythia->readString("StringFlav:etaSup = 1.");
  // fPythia->readString("StringFlav:mesonUDvector = 3.");
  // fPythia->readString("StringFlav:mesonUDL1S1J2 = 5.");
  // heavy ions settings
  /*
  fPythia->readString("HeavyIon:SigFitErr = "
                      "0.02,0.02,0.1,0.05,0.05,0.0,0.1,0.0");
  // These parameters are typicall suitable for sqrt(S_NN)=5TeV
  fPythia->readString("HeavyIon:SigFitDefPar = "
                      "17.24,2.15,0.33,0.0,0.0,0.0,0.0,0.0");
  // A simple genetic algorithm is run for 20 generations to fit the
  // parameters.
  fPythia->readString("HeavyIon:SigFitNGen = 20");
  */
  // fPythia->readString(
  //    "PartonLevel:MPI = off"); // switch off multiparton interactions
  fPythia->readFile("py8Config.cfg");
  fPythia->readString("HeavyIon:SigFitReuseInit=3");
  fPythia->init();

  TFile *fOut = new TFile("py8_standalone.root", "RECREATE");
  TH1F *hPsumX = new TH1F("hPsumX", "sum P_{X}", 1000, -1., 1.);
  TH1F *hPsumY = new TH1F("hPsumY", "sum P_{Y}", 1000, -1., 1.);
  TH1F *hPsumZ = new TH1F("hPsumZ", "sum P_{Z}", 1000, 0., 10.);
  TH1F *hEsum = new TH1F("hEsum", "sum E", 1000, 0., 10.);

  TH1F *hPi0Mass = new TH1F("hPi0Mass", "#pi^{0} mass spectrum", 100, 0., 2.);
  TH1F *hEtaEnergy =
      new TH1F("hEtaEnergy", "#eta energy spectrum", 700, 0., 7.);

  TH1F *hF2Mass = new TH1F("hF2Mass", "f2(1270) mass spectrum", 100, 0., 2.);
  TH1F *hF2Energy =
      new TH1F("hF2Energy", "f2(1270) energy spectrum", 700, 0., 7.);
  TH1F *hOmegaMass =
      new TH1F("hOmegaMass", "#omega mass spectrum", 100, 0., 2.);
  TH1F *hOmegaEnergy =
      new TH1F("hOmegaEnergy", "#omega energy spectrum", 700, 0., 7.);

  /*TH1F *hEtaMass = new TH1F("hEtaMass", "#eta mass spectrum", 100, 0., 2.);
  TH1F *hEtaEnergy =
      new TH1F("hOmegaEnergy", "#eta energy spectrum", 700, 0., 7.);
  TH1F *hEtaMass = new TH1F("hEtaMass", "#eta mass spectrum", 100, 0., 2.);
  TH1F *hEtaEnergy =
      new TH1F("hOmegaEnergy", "#eta energy spectrum", 700, 0., 7.);
  TH1F *hEtaMass = new TH1F("hEtaMass", "#eta mass spectrum", 100, 0., 2.);
  TH1F *hEtaEnergy =
      new TH1F("hOmegaEnergy", "#eta energy spectrum", 700, 0., 7.);
  */
  TH1F *hF2CosT = new TH1F("hF2CosT", "f2(1270) cos #Theta", 1000, 0., 1.);
  TH1F *hF2GamMinCosT = new TH1F(
      "hF2GamMinCosT", "f2(1270) min cos #Theta of gamma", 1000, 0., 1.);
  // TH1F *hF2GamMaxXY = new TH1F()
  TH1F *hProcessCode =
      new TH1F("hProcessCode", "Process code", 200, -0.5, 199.5);
  TH1F *hMass2G = new TH1F("hMass2G", "Mass 2#gamma", 200, 0., 2.);
  TH1F *hMass3G = new TH1F("hMass3G", "Mass 3#gamma", 200, 0., 2.);
  TH1F *hMass4G = new TH1F("hMass4G", "Mass 4#gamma", 200, 0., 2.);
  TH1F *hMass4G_F2 =
      new TH1F("hMass4G_F2", "Mass 4#gamma (f2(1270)-event)", 200, 0., 2.);
  TH1F *hMultG = new TH1F("hMultG", "Multiplicity of #gamma", 10, 0., 10.);
  double eCut = 0.;        // GeV
  double eGammaMin = 0.01; // GeV
  double caloDist = 3.7;   // m
  double caloSize = 1.;    // m
  double cosTCut =
      caloDist / TMath::Sqrt(caloDist * caloDist + caloSize * caloSize);

  int nF2 = 0;
  LVec pCalo, pSum;
  int nGamma = 0;
  for (int iEv = 0; iEv < nEvents; iEv++) {
    if (!fPythia->next()) {
      continue;
    }
    cout << "Event " << iEv << endl;
    pCalo.SetPxPyPzE(0., 0., 0., 0.);
    pSum.SetPxPyPzE(0., 0., 0., 0.);
    nGamma = 0;
    bool isSaTriggered = false;
    hProcessCode->Fill(fPythia->info.code());
    // fPythia->info.list();
    // fPythia->event.list();
    bool hasF2 = false;
    for (int i = 0; i < fPythia->event.size(); i++) {
      int f2Index = -1;
      const Pythia8::Particle &p = fPythia->event[i];
      if (p.id() == 225) { // f2(1270) meson

        nF2++;
        hF2Mass->Fill(p.m());
        hF2Energy->Fill(p.e());
        hF2CosT->Fill(TMath::Cos(p.theta()));
        // fPythia->info.list();
        // fPythia->event.list();
        if (p.daughter1() > 0 && p.daughter2() > 0) {
          const Pythia8::Particle &d1 = fPythia->event[p.daughter1()];
          const Pythia8::Particle &d2 = fPythia->event[p.daughter2()];
          if (d1.id() == 111 && d2.id() == 111) { // pi0pi0 decay
            const Pythia8::Particle &gam1 = fPythia->event[d1.daughter1()];
            const Pythia8::Particle &gam2 = fPythia->event[d1.daughter2()];
            const Pythia8::Particle &gam3 = fPythia->event[d2.daughter1()];
            const Pythia8::Particle &gam4 = fPythia->event[d2.daughter2()];

            if (gam1.id() == 22 && gam2.id() == 22 && gam3.id() == 22 &&
                gam4.id() == 22) {
              hasF2 = true;
              double maxGamT = gam1.theta();
              int maxGamId = gam1.id();
              if (maxGamT < gam2.theta()) {
                maxGamT = gam2.theta();
                maxGamId = gam2.id();
              }
              if (maxGamT < gam3.theta()) {
                maxGamT = gam3.theta();
                maxGamId = gam3.id();
              }
              if (maxGamT < gam4.theta()) {
                maxGamT = gam4.theta();
                maxGamId = gam4.id();
              }

              hF2GamMinCosT->Fill(TMath::Cos(maxGamT));
            }
          }
        }
      }
      if (!p.isFinal()) {
        continue;
      }
      pSum += LVec(p.px(), p.py(), p.pz(), p.e());
      double xCalo = TMath::Tan(p.theta()) * caloDist * TMath::Cos(p.phi());
      double yCalo = TMath::Tan(p.theta()) * caloDist * TMath::Sin(p.phi());
      // if (TMath::Cos(p.theta()) > cosTCut) { // calo acceptance
      if (TMath::Abs(xCalo) < caloSize && TMath::Abs(yCalo) < caloSize &&
          (p.e() - p.m()) > 2.5e-3) {
        if (p.charge() != 0. && gRandom->Rndm() < 1.) {
          isSaTriggered = true;
        }
        if (p.id() == 22 && p.e() > eGammaMin) {
          nGamma++;
          pCalo += LVec(p.px(), p.py(), p.pz(), p.e());
        }
      }
    }
    hPsumX->Fill(pSum.X());
    hPsumY->Fill(pSum.Y());
    hPsumZ->Fill(pSum.Z());
    hEsum->Fill(pSum.T());
    cout << "Sa trigger is " << (isSaTriggered ? "" : "not ") << "fired"
         << endl;
    if (!isSaTriggered && pCalo.e() > eCut) {
      hMultG->Fill(nGamma);
      if (nGamma == 2) {
        hMass2G->Fill(pCalo.M());
      } else if (nGamma == 3) {
        hMass3G->Fill(pCalo.M());
      } else if (nGamma == 4) {
        hMass4G->Fill(pCalo.M());
        if (hasF2) {
          hMass4G_F2->Fill(pCalo.M());
        }
      }
    }
  }
  fPythia->stat();
  cout << "I generated " << nF2 << " f2(1270) mesons." << endl;
  fOut->Write();
  fOut->Close();
  fPythia->settings.writeFile("dumpConfig.cfg");
}
