void drawEnDepInNaI() {
  TFile *f = TFile::Open("c60mm_py8_photonE0.0023GeV_GNTsingleRawCsI.root");
  TH1* hEnDepInNaI0 = (TH1*)f->Get("hEnDepInNaI0");
  TH1* hEnDepInNaI1 = (TH1*)f->Get("hEnDepInNaI1");
  TH1* hEnDepInNaI2 = (TH1*)f->Get("hEnDepInNaI2");
  TH1* hEnDepInNaITriggered0 = (TH1*)f->Get("hEnDepInNaITriggered0");
  TH1* hEnDepInNaITriggered1 = (TH1*)f->Get("hEnDepInNaITriggered1");
  TH1* hEnDepInNaITriggered2 = (TH1*)f->Get("hEnDepInNaITriggered2");

  TCanvas *c2 = new TCanvas("c2");
  c2->Divide(3, 1);
  c2->cd(1);
  hEnDepInNaI0->GetXaxis()->SetRangeUser(0.0001, 0.01);
  hEnDepInNaI0->GetYaxis()->SetRangeUser(0., 2000.);
  hEnDepInNaITriggered0->Draw();
  hEnDepInNaI0->Draw();
  hEnDepInNaITriggered0->SetLineColor(kRed);
  hEnDepInNaITriggered0->Draw("same");
  c2->cd(2);
  hEnDepInNaI1->GetXaxis()->SetRangeUser(0.0001, 0.01);
  hEnDepInNaI1->GetYaxis()->SetRangeUser(0., 2000.);
  hEnDepInNaITriggered1->Draw();
  hEnDepInNaI1->Draw();
  hEnDepInNaITriggered1->SetLineColor(kRed);
  hEnDepInNaITriggered1->Draw("same");
  c2->cd(3);
  hEnDepInNaI2->GetXaxis()->SetRangeUser(0.0001, 0.01);
  hEnDepInNaI2->GetYaxis()->SetRangeUser(0., 2000.);
  hEnDepInNaITriggered2->Draw();
  hEnDepInNaI2->Draw();
  hEnDepInNaITriggered2->SetLineColor(kRed);
  hEnDepInNaITriggered2->Draw("same");

}
