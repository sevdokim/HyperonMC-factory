Double_t ScaleX(Double_t x) {
  Double_t v;
  v = 1000 * x; // "linear scaling" function example
  return v;
}

Double_t ScaleY(Double_t y) {
  Double_t v;
  v = 20 * y + 200; // "linear scaling" function example
  return v;
}

Double_t ScaleZ(Double_t z) {
  Double_t v;
  v = 30 * z + 300; // "linear scaling" function example
  return v;
}

void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t)) {
  if (!a)
    return; // just a precaution
  if (a->GetXbins()->GetSize()) {
    // an axis with variable bins
    // note: bins must remain in increasing order, hence the "Scale"
    // function must be strictly (monotonically) increasing
    TArrayD X(*(a->GetXbins()));
    for (Int_t i = 0; i < X.GetSize(); i++)
      X[i] = Scale(X[i]);
    a->Set((X.GetSize() - 1), X.GetArray()); // new Xbins
  } else {
    // an axis with fix bins
    // note: we modify Xmin and Xmax only, hence the "Scale" function
    // must be linear (and Xmax must remain greater than Xmin)
    a->Set(a->GetNbins(),
           Scale(a->GetXmin()),  // new Xmin
           Scale(a->GetXmax())); // new Xmax
  }
  return;
}

void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t)) {
  if (!h)
    return; // just a precaution
  ScaleAxis(h->GetXaxis(), Scale);
  return;
}

void ScaleYaxis(TH1 *h, Double_t (*Scale)(Double_t)) {
  if (!h)
    return; // just a precaution
  ScaleAxis(h->GetYaxis(), Scale);
  return;
}

void ScaleZaxis(TH1 *h, Double_t (*Scale)(Double_t)) {
  if (!h)
    return; // just a precaution
  ScaleAxis(h->GetZaxis(), Scale);
  return;
}
int nPad = 0;
void draw_singleE(TH1 *hMes, TH1 *hDep, double lR, double rR,
                  const char *title);
void draw_singleE_mc() {
  gROOT->ProcessLine(".x setStyle.C");
  TFile *fPhE3 = TFile::Open("c60mm_preshower0.2cm_photonE0.0063GeV.root");
  TH1 *hBGOMesPhE3 = (TH1 *)fPhE3->Get("hMesEnInBGO7");
  TH1 *hBGODepPhE3 = (TH1 *)fPhE3->Get("hEnDepInBGO7");
  TH1 *hNaIMesPhE3 = (TH1 *)fPhE3->Get("hMesEnInNaI7");
  TH1 *hNaIDepPhE3 = (TH1 *)fPhE3->Get("hEnDepInNaI7");

  TFile *fPhE2 = TFile::Open("c60mm_preshower0.2cm_photonE0.0043GeV.root");
  TH1 *hBGOMesPhE2 = (TH1 *)fPhE2->Get("hMesEnInBGO7");
  TH1 *hBGODepPhE2 = (TH1 *)fPhE2->Get("hEnDepInBGO7");
  TH1 *hNaIMesPhE2 = (TH1 *)fPhE2->Get("hMesEnInNaI7");
  TH1 *hNaIDepPhE2 = (TH1 *)fPhE2->Get("hEnDepInNaI7");

  TFile *fPhE1 = TFile::Open("c60mm_preshower0.2cm_photonE0.00146GeV.root");
  TH1 *hBGOMesPhE1 = (TH1 *)fPhE1->Get("hMesEnInBGO7");
  TH1 *hBGODepPhE1 = (TH1 *)fPhE1->Get("hEnDepInBGO7");
  TH1 *hNaIMesPhE1 = (TH1 *)fPhE1->Get("hMesEnInNaI7");
  TH1 *hNaIDepPhE1 = (TH1 *)fPhE1->Get("hEnDepInNaI7");

  gStyle->SetOptStat(0);
  TCanvas *c2 = new TCanvas("c2", "c2", 1000, 1000);
  c2->Divide(2, 3);
  // TH1 *hSum = (TH1 *)f->Get("hSumC");
  // TH1 *hSum = (TH1 *)f->Get("hSumCnoCpvnoSanti");
  // TH1 *hSum = (TH1 *)f->Get("hMesEnInBGO7");
  ScaleXaxis(hBGOMesPhE2, ScaleX);
  ScaleXaxis(hBGOMesPhE1, ScaleX);
  ScaleXaxis(hBGOMesPhE3, ScaleX);
  ScaleXaxis(hBGODepPhE2, ScaleX);
  ScaleXaxis(hBGODepPhE1, ScaleX);
  ScaleXaxis(hBGODepPhE3, ScaleX);
  ScaleXaxis(hNaIMesPhE2, ScaleX);
  ScaleXaxis(hNaIMesPhE1, ScaleX);
  ScaleXaxis(hNaIMesPhE3, ScaleX);
  ScaleXaxis(hNaIDepPhE2, ScaleX);
  ScaleXaxis(hNaIDepPhE1, ScaleX);
  ScaleXaxis(hNaIDepPhE3, ScaleX);

  // NaI 2Mev
  nPad = 1;
  c2->cd(1);
  draw_singleE(hNaIDepPhE1, hNaIMesPhE1, 0.2, 8., "NaI, E = 2 MeV");
  // BGO 2MeV
  c2->cd(2);
  nPad = 2;

  draw_singleE(hBGODepPhE1, hBGOMesPhE1, 0.2, 8., "BGO, E = 2 MeV");

  // NaI 4.3Mev
  c2->cd(3);
  nPad = 3;

  draw_singleE(hNaIDepPhE2, hNaIMesPhE2, 0.2, 8., "NaI, E = 4.3 MeV");
  // BGO 4.3MeV

  c2->cd(4);
  nPad = 4;

  draw_singleE(hBGODepPhE2, hBGOMesPhE2, 0.2, 8., "BGO, E = 4.3 MeV");
  // NaI 6.3Mev
  c2->cd(5);
  nPad = 5;

  draw_singleE(hNaIDepPhE3, hNaIMesPhE3, 0.2, 8., "NaI, E = 6.3 MeV");
  // BGO 6.3MeV
  c2->cd(6);
  nPad = 6;

  draw_singleE(hBGODepPhE3, hBGOMesPhE3, 0.2, 8., "BGO, E = 6.3 MeV");

  /*TPaveText *leg = new TPaveText(0.4, 0.6, 0.9, 0.9, "NDC");
  leg->SetFillColor(kWhite);
  leg->AddText(Form("E_{1} = %2.2f #pm %2.2f MeV", fit1->GetParameter(1),
                    fit1->GetParError(1)));
  leg->AddText(Form("E_{2} = %2.2f #pm %2.2f MeV", fit4->GetParameter(1),
                    fit4->GetParError(1)));
  leg->AddText(Form("E_{3} = %2.2f #pm %2.2f MeV", fit3->GetParameter(1),
                    fit3->GetParError(1)));
  leg->AddText(Form("E_{4} = %2.2f #pm %2.2f MeV", fit3->GetParameter(7),
                    fit3->GetParError(7)));

  leg->Draw();
  TText *t1 = new TText(0.2, 0.8, "1");*/
  c2->Print("MC_singleE.pdf");
}
void draw_singleE(TH1 *hDep, TH1 *hMes, double lR, double rR,
                  const char *title) {
  hDep->SetTitle(Form("%s;E, MeV; Counts/(0.1 MeV)", title));
  hDep->SetTitleFont(132, "XY");
  hDep->GetXaxis()->CenterTitle(true);
  hDep->GetYaxis()->SetNdivisions(50);
  hDep->GetYaxis()->CenterTitle(true);
  hDep->Rebin(2);
  gPad->SetLogy();

  gPad->Update();

  hDep->GetYaxis()->SetTitleOffset(1.0);
  hDep->GetXaxis()->SetRangeUser(lR, rR);
  hDep->GetYaxis()->SetRangeUser(100, 100000);
  hDep->Draw();

  hMes->SetLineColor(kRed);
  hMes->Rebin(2);
  hMes->Draw("same");
  TLegend *leg;
  if (nPad < 5) {
    leg = new TLegend(0.6, 0.75, 0.9, 0.9);
  } else {
    leg = new TLegend(0.15, 0.75, 0.45, 0.9);
  }
  // leg->SetHeader(title, "C");
  leg->SetFillColor(kWhite);
  leg->AddEntry(hDep, "Deposed energy", "l");
  leg->AddEntry(hMes, "Detected energy", "l");
  leg->Draw();
}
