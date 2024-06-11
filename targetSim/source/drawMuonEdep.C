void drawMuonEdep()
{
  TFile *f = TFile::Open("muons6GeV_alongZ.root");
  TH1F *hNaIEnDep[10];
  TH1F *hBGOEnDep[10];
  TH1F *hTotNaIEnDep = nullptr;
  TH1F *hTotBGOEnDep = nullptr;

  gROOT->ProcessLine(".x setStyle.C");
  
  for (int i = 0; i < 9; i++) {
    //hNaIEnDep[i] = (TH1F*)f->Get(Form("hEnDepInNaITriggered%d", i));
    //hBGOEnDep[i] = (TH1F*)f->Get(Form("hEnDepInBGOTriggered%d", i));
    hNaIEnDep[i] = (TH1F*)f->Get(Form("hMesEnInNaITriggered%d", i));
    hBGOEnDep[i] = (TH1F*)f->Get(Form("hMesEnInBGOTriggered%d", i));

    if (i == 0) {
      hTotNaIEnDep = (TH1F*)hNaIEnDep[0]->Clone("hTotNaIEnDep");
      hTotBGOEnDep = (TH1F*)hBGOEnDep[0]->Clone("hTotBGOEnDep");
    } else {
      hTotNaIEnDep->Add(hNaIEnDep[i]);
      hTotBGOEnDep->Add(hBGOEnDep[i]);
    }
  }

  TCanvas *c1 = new TCanvas("c1", "pic",  1200, 600);
  c1->Divide(2, 1);
  c1->cd(1);
  gPad->SetLogy(1);
  hTotNaIEnDep->GetXaxis()->SetRangeUser(0., 0.2);
  hTotNaIEnDep->GetYaxis()->SetRangeUser(0.5, 1.e3);
  hTotNaIEnDep->GetXaxis()->SetTitle("E_{dep}, GeV");
  hTotNaIEnDep->Draw();
  TPaveText * textNaI = new TPaveText(0.3, 0.7, 0.9, 0.9, "NBNDC");
  textNaI->AddText("NaI");
  textNaI->AddText("<E_{dep}> = 0.032 GeV");
  textNaI->SetFillStyle(0);
  textNaI->SetBorderSize(0);
  textNaI->Draw();
  
  c1->cd(2);
  gPad->SetLogy(1);
  hTotBGOEnDep->GetXaxis()->SetRangeUser(0., 0.2);
  hTotBGOEnDep->GetYaxis()->SetRangeUser(0.5, 1.e3);
  hTotBGOEnDep->GetXaxis()->SetTitle("E_{dep}, GeV");
  hTotBGOEnDep->Draw();
  TPaveText * textBGO = new TPaveText(0.3, 0.7, 0.9, 0.9, "NBNDC");
  textBGO->AddText("BGO");
  textBGO->AddText("<E_{dep}> = 0.065 GeV");
  textBGO->SetFillStyle(0);
  textBGO->SetBorderSize(0);
  textBGO->Draw();

  c1->Print("muons_GEANT4.eps");
}
