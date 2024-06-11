void setStyle() {
  // gStyle->SetOptLogy();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadRightMargin(0.1);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  TGaxis::SetMaxDigits(3);
  gStyle->SetTitleXSize(0.07);
  gStyle->SetTitleYSize(0.07);
  gStyle->SetLabelSize(0.07, "XY");
  gStyle->SetLabelFont(132, "XY");
  gStyle->SetTitleFont(132, "XY");
  gStyle->SetTextFont(132);
  gStyle->SetNdivisions(505, "X");
  gStyle->SetNdivisions(3, "Y");
  gStyle->SetHistLineColor(kBlack);
  gStyle->SetHistLineWidth(2);
  gStyle->SetTitleOffset(1., "X");
  gStyle->SetTitleOffset(1., "Y");
  gStyle->SetFrameLineWidth(2);
  gROOT->ForceStyle();
}
