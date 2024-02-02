#include "/data/mysw/INSTALL/pythia8/include/Pythia8/HeavyIons.h"
#include "/data/mysw/INSTALL/pythia8/include/Pythia8/Pythia.h"
using LVec = ROOT::Math::PxPyPzEVector;
void run_py8_standalone(int nEvents = 100) {
  gSystem->Load("libpythia8");
  Pythia8::Pythia *fPythia;
  fPythia = new Pythia8::Pythia();
  int pySeed = gRandom->TRandom::GetSeed();
  fPythia->readString("LowEnergyQCD:all = on");
  fPythia->readString("Random:setSeed = on");
  fPythia->readString(Form("Random:seed = %d", pySeed));
  // fPythia->readString("Beams:idA = 211"); // pi+
  fPythia->readString("Beams:idA = 2212"); // proton
  fPythia->readString("Beams:idB = 2112"); // neutron
  // fPythia->readString("Beams:idB = 1000060120");
  fPythia->readString("Beams:eA = 7.");
  fPythia->readString("Beams:eB = 0.");
  fPythia->readString("Beams:frameType = 2");
  fPythia->readString("StringFlav:etaSup = 1.");
  fPythia->readString("StringFlav:mesonUDvector = 1.");
  fPythia->readString("StringFlav:mesonUDL1S1J2 = 5.");
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
  fPythia->readString(
      "PartonLevel:MPI = off"); // switch off multiparton interactions

  fPythia->init();

  TH1F *hF2Mass = new TH1F("hF2Mass", "f2(1270) mass spectrum", 100, 0., 2.);
  TH1F *hProcessCode =
      new TH1F("hProcessCode", "Process code", 200, -0.5, 199.5);
  TH1F *hMass2G = new TH1F("hMass2G", "Mass 2#gamma", 200, 0., 2.);
  TH1F *hMass3G = new TH1F("hMass3G", "Mass 3#gamma", 200, 0., 2.);
  TH1F *hMass4G = new TH1F("hMass4G", "Mass 4#gamma", 200, 0., 2.);
  TH1F *hMultG = new TH1F("hMultG", "Multiplicity of #gamma", 10, 0., 10.);
  double eCut = 1.;      // GeV
  double caloDist = 3.7; // m
  double caloSize = 1.;  // m
  double cosTCut =
      caloDist / TMath::Sqrt(caloDist * caloDist + caloSize * caloSize);

  int nF2 = 0;
  LVec pCalo, pPart;
  int nGamma = 0;
  for (int iEv = 0; iEv < nEvents; iEv++) {
    if (!fPythia->next()) {
      continue;
    }
    pCalo.SetPxPyPzE(0., 0., 0., 0.);
    nGamma = 0;
    bool isSaTriggered = false;
    hProcessCode->Fill(fPythia->info.code());
    // fPythia->info.list();
    for (int i = 0; i < fPythia->event.size(); i++) {
      const Pythia8::Particle &p = fPythia->event[i];
      if (p.id() == 225) { // f2(1270) meson
        nF2++;
        hF2Mass->Fill(p.m());
        fPythia->info.list();
        fPythia->event.list();
      }
      if (!p.isFinal()) {
        continue;
      }
      if (TMath::Cos(p.theta()) > cosTCut) { // calo acceptance
        if (p.charge() != 0.) {
          isSaTriggered = true;
        }
        if (fPythia->event[i].id() == 22) {
          nGamma++;
          pCalo += LVec(p.px(), p.py(), p.pz(), p.e());
        }
      }
    }
    if (!isSaTriggered) {
      hMultG->Fill(nGamma);
      if (nGamma == 2) {
        hMass2G->Fill(pCalo.M());
      } else if (nGamma == 3) {
        hMass3G->Fill(pCalo.M());
      } else if (nGamma == 4) {
        hMass4G->Fill(pCalo.M());
      }
    }
  }
  fPythia->stat();
  cout << "I generated " << nF2 << " f2(1270) mesons." << endl;
}