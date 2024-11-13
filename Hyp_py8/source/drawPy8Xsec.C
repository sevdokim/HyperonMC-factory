double fitTransparency(double *x, double *par);
string findString(TString fileName, string searchPattern);
void drawPy8Xsec()
{
  gROOT->ProcessLine(".x setStyle.C");
  int nTarg = 6;
  TString simName[] = {"pi9Be", "pi12C", "pi27Al", "pi63Cu", "pi120Sn", "pi208Pb"};
  string searchPattern = "(Estimated total cross section)";
  double A[] = {9, 12, 27, 63, 120, 208};
  // double xSecTot[] = {3.180e+01, 4.243e+01, 9.372e+01, 2.154e+02, 3.982e+02, 6.794e+02};
  // double eXSecTot[] = {1.037e-01, 1.389e-01, 3.155e-01, 7.618e-01, 1.462e+00, 2.595e+00};
  double *xSec = new double[nTarg];
  double *eXSec = new double[nTarg];
  for (int iF = 0; iF < nTarg; iF++) {
    TString fileName = "../simulations/" + simName[iF] + "/nohup.out";
    // cout << "Filename = " << fileName << endl;
    string foundString = findString(fileName, searchPattern);
    // cout << foundString << endl;
    size_t pos = foundString.find("|");
    pos = foundString.find("|", pos + 1);
    pos = foundString.find("|", pos + 1);
    string valStr = foundString.substr(pos + 1);
    double val = 0.0, err = 0.0;
    sscanf(valStr.data(), "%lf %lf", &val, &err);
    // cout << "val = " << val << ", err = " << err << endl;
    xSec[iF] = val;
    eXSec[iF] = err;
  }
  TGraphErrors* gr = new TGraphErrors(nTarg, A, xSec, 0x0, eXSec);
  gr->SetMarkerStyle(21);
  gr->GetYaxis()->SetRangeUser(0., 1000.);
  gr->GetYaxis()->SetTitle("#sigma_{tot} (mb)");
  gr->GetXaxis()->SetTitle("A");
  gr->Draw();
  TF1 *fTrans = new TF1("fTrans", fitTransparency, 1., 210., 2);
  fTrans->SetParameters(1., -0.333);
  fTrans->SetParName(0, "#sigma_{0}");
  fTrans->SetParName(1, "#alpha");
  gr->Fit(fTrans, "E", "", 0., 210.);

}
string findString(TString fileName, string searchPattern)
{
  ifstream data_store;
  string line;
  data_store.open(fileName.Data());
  while (!data_store.eof())  {
    getline(data_store, line);
    if (line.find(searchPattern) != std::string::npos) {
      data_store.close();
      return line;
    }
  }
  return string("");
}
double fitTransparency(double *x, double *par) {
  return par[0] * TMath::Power(x[0], par[1]);
}
