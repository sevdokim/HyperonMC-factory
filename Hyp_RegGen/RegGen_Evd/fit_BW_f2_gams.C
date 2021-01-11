double  BW_f2_gams(double *x, double* par){
// C     The GAMS relativistic BW function for f2:      S.Sadovsky 17.08.2011 
// C     Eur.Phys.J. A3, 361-371 (1998)
// C     Input:   X    - 2pi0 mass in MeV
// C              Rmas - resonanse mass  in MeV
// C              Rwid - resonanse width in MeV
// C
//  data Ampi0, r0  /  134.9766, 197.326      /  ! pi0 mass and r0 (in 1/MeV)
  static double Ampi0 = 134.9766, r0 = 197.326;
  static double Rmas,Rwid,ppi0,ppiX,D2pi0,D2piX,Gamma,BW,value, mass;
  Rmas = 1000.*par[1];
  Rwid = 1000.*par[2];
  mass = 1000.*x[0];

  ppi0  = sqrt(abs(pow(Rmas/2.,2)-Ampi0*Ampi0));
  ppiX  = sqrt(abs(pow(mass/2.,2)-Ampi0*Ampi0));
    
  D2pi0 = 9.+3.*pow(ppi0/r0,2)+pow(ppi0/r0,4);     // ! Blatte-Weiskopf factor for f2
  D2piX = 9.+3.*pow(ppiX/r0,2)+pow(ppiX/r0,4);
      
  Gamma = Rwid*pow(ppiX/ppi0,5)*D2pi0/D2piX;
  BW    = pow(Rmas*Gamma,2)/(pow(mass*mass-Rmas*Rmas,2)+pow(Rmas*Gamma,2));
  value = BW*pow(mass,2)/ppiX;
  return par[0]*value;
}
void fit_BW_f2_gams(const char* filename = "reggen.root"){
  TFile *file = TFile::Open(filename);
  TH1F* hMass = (TH1F*)file->Get("h22");
  TF1* fitF = new TF1("BW_f2_gams", BW_f2_gams, 0.28,2.0,3);
  fitF->SetParameters(1.4,1.275, 0.185);
  //fitF->Draw();
  hMass->Fit(fitF,"","",0.28,1.8);
}
  
