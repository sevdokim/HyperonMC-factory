void readKinematics(const char *filename = "MCgen.dat") {
  Int_t eventNumb = 0, Ngamma = 0, eventCounter;
  Int_t lineNumb = 0;
  Double_t eventVertex[3];
  Double_t Pb[4];
  Double_t Pphotons[7][10];
  TFile fHistos("kinematicHistos.root", "RECREATE");
  TLorentzVector Pmeson, Pgamma[10], Pbeam;
  TH1F *hMassMeson =
      new TH1F("hMassMeson", "Mass of all photons", 2000, 0., 2.);
  TH1F *hEnergyMeson =
      new TH1F("hEnergyMeson", "Energy of all photons", 10000, 0., 1.);
  TH1F *hPtMeson = new TH1F("hPtMeson", "Pt of all photons", 10000, 0., 1.);
  FILE *file = fopen(filename, "r");
  if (!file) {
    cout << "Couldn't open file " << filename << endl << "Abort!" << endl;
    return;
  }
  char string[40000];
  while (fgets(string, 30000, file)) {
    lineNumb++;
    if (string[0] == '#')
      continue; // comments
    if (string[0] == '/')
      continue; // comments
    if (string[0] == '\n')
      continue;
    if (strstr(string, "begin event")) {

      if (eventCounter % 100 == 0)
        cout << "Read event # " << eventCounter << endl;
      fgets(string, 30000, file);
      lineNumb++;
      sscanf(string, "%d %d %le %le %le\n", &eventNumb, &Ngamma,
             &eventVertex[0], &eventVertex[1], &eventVertex[2]);
      // cout<< "event number = " << eventNumb<< ", Ngamma = "<<Ngamma<<", X of
      // vertex = "<< eventVertex[0]<< endl;
      fgets(string, 30000, file);
      lineNumb++;
      sscanf(string, "%le %le %le %le\n", &Pb[0], &Pb[1], &Pb[2], &Pb[3]);
      Pbeam.SetXYZT(Pb[0], Pb[1], Pb[2], Pb[3]);
      for (int i = 0; i < Ngamma; i++) {
        fgets(string, 30000, file);
        lineNumb++;
        // cout<< string<<endl;
        sscanf(string, "%le %le %le %le %le %le %le\n", &Pphotons[0][i],
               &Pphotons[1][i], &Pphotons[2][i], &Pphotons[3][i],
               &Pphotons[4][i], &Pphotons[5][i], &Pphotons[6][i]);
        // cout<<Pphotons[0][i]<<endl;
      }

      // fill histos
      Pmeson.SetXYZT(0, 0, 0, 0);
      for (int i = 0; i < Ngamma; i++) {
        Pgamma[i].SetXYZT(Pphotons[0][i], Pphotons[1][i], Pphotons[2][i],
                          Pphotons[3][i]);
        // Pgamma[i].Print();
        Pmeson += Pgamma[i];
      }
      hMassMeson->Fill(Pmeson.M());
      // cout<<Pmeson.M()<<endl;

      // end of filling histos

    } else {
      cout << "Broken format at line " << lineNumb << " : " << endl
           << string << endl
           << "Abort!" << endl;
      return;
    }
    eventCounter++;
  }
  fHistos.Write();
  fHistos.Close();
}
