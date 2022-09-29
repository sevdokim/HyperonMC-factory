#include "HyMCApplication.h"
TH2F* CoordHisto;
void test_func(){  std::cout << "this is a test !" << std::endl; return; }

ClassImp(HyMCApplication)

//_____________________________________________________________________________
HyMCApplication::HyMCApplication(const char *name, const char *title) 
: TVirtualMCApplication(name,title),
  fStack(0),
  fDetConstruction()
{
  /// Standard constructor
  /// \param name   The MC application name 
  /// \param title  The MC application description

  EvntNumb = 0;
  evWr=0;
  // create a user stack
  fStack = new HyMCStack(1000000);  

  fLaNx = fDetConstruction.GetGeometry().GetLaNx();  // Arrays of energy deposition in LGD2
  fLaNy = fDetConstruction.GetGeometry().GetLaNy();
  fSmNx = fDetConstruction.GetGeometry().GetSmNx();
  fSmNy = fDetConstruction.GetGeometry().GetSmNy();
  LaEnDep = new Double_t*[fLaNx];
  for(Int_t tmp = 0; tmp<fLaNx; tmp++)
    LaEnDep[tmp] = new Double_t[fLaNy];
  SmEnDep = new Double_t*[fSmNx];
  for(Int_t tmp = 0; tmp<fSmNx; tmp++)
    SmEnDep[tmp] = new Double_t[fSmNy];

  fHistosRoot="histos.root";
  fMCResults="MC_results.dat";
  fMCgendat="MCgen.dat";

  fMomentum = 7.;
  fPCNumber = 0;
  fControl = 0;
  fExTarget=-1;
  fExChanel=-1;
  fExReson=-1;
  fSaEnergyCut=0.0023;
  if (iseed <= 0) iseed = 1000000;  // ;kondr, v1.04

  initial_photon_energy = new Double_t[100];
  
  energy_dep = new Double_t*[100];
  for(Int_t tmp = 0; tmp<100; tmp++)energy_dep[tmp]= new Double_t[640];

  gen3 = new TRandom3(iseed); // ;kondr, v1.04
  gen3->SetSeed(iseed+2);     // ;kondr, v1.04

  
  Double_t eBeam = TMath::Sqrt(0.139570*0.139570 + fMomentum*fMomentum);

  fDebug = 0;
}
//_____________________________________________________________________________
HyMCApplication::HyMCApplication(const char *name, const char *title, int seed)
: TVirtualMCApplication(name,title),
  fStack(0),
  fDetConstruction()
{
  /// Kondratyuk constructor, with seed
  /// \param name   The MC application name
  /// \param title  The MC application description
  /// \param seed   The seed for random generators

  iseed = seed;
//   HyMCApplication(name, title);
  
  EvntNumb = 0;
  evWr=0;
  // create a user stack
  fStack = new HyMCStack(1000000);  

  fLaNx = fDetConstruction.GetGeometry().GetLaNx();  // Arrays of energy deposition in LGD2
  fLaNy = fDetConstruction.GetGeometry().GetLaNy();
  fSmNx = fDetConstruction.GetGeometry().GetSmNx();
  fSmNy = fDetConstruction.GetGeometry().GetSmNy();
  LaEnDep = new Double_t*[fLaNx];
  for(Int_t tmp = 0; tmp<fLaNx; tmp++)
    LaEnDep[tmp] = new Double_t[fLaNy];
  SmEnDep = new Double_t*[fSmNx];
  for(Int_t tmp = 0; tmp<fSmNx; tmp++)
    SmEnDep[tmp] = new Double_t[fSmNy];

  fHistosRoot="histos.root";
  fMCResults="MC_results.dat";
  fMCgendat="MCgen.dat";

  fMomentum = 7.;
  fPCNumber = 0;
  fControl = 0;
  fExTarget=-1;
  fExChanel=-1;
  fExReson=-1;
  fSaEnergyCut=0.0023;
  if (iseed <= 0) iseed = 1000000;  // ;kondr, v1.04

  initial_photon_energy = new Double_t[100];
  
  energy_dep = new Double_t*[100];
  for(Int_t tmp = 0; tmp<100; tmp++)energy_dep[tmp]= new Double_t[640];

  gen3 = new TRandom3(iseed); // ;kondr, v1.04
  gen3->SetSeed(iseed+2);     // ;kondr, v1.04

  Double_t eBeam = TMath::Sqrt(0.139570*0.139570 + fMomentum*fMomentum);
  
  iseed = seed;

  fDebug = 0;
  #ifndef _DONT_USE_NETWORK_FUNCTIONS_
    SetConnection();
  #endif
}
//_____________________________________________________________________________
void HyMCApplication::SetPCNumber(int pcnumber)
{
  fPCNumber = pcnumber;
}
//_____________________________________________________________________________
void HyMCApplication::SetControl(double control)
{
  fControl = control;
  cout << "HyMCApplication::SetControl() : control is now " << fControl << endl; 
}
//_____________________________________________________________________________
void HyMCApplication::SetExTarget(int hh)
{
  fExTarget = hh;
}
//_____________________________________________________________________________
void HyMCApplication::SetExReson(int hh)
{
  fExReson = hh;
}
//_____________________________________________________________________________
void HyMCApplication::SetExChanel(int hh)
{
  fExChanel = hh;
}
//_____________________________________________________________________________
void HyMCApplication::SetMCResults(char* filename)
{
  fMCResults=filename;
}
//_____________________________________________________________________________
void HyMCApplication::SetMCgendat(char* filename)
{
  fMCgendat=filename;
}
//_____________________________________________________________________________
void HyMCApplication::SetHistosRoot(char* filename)
{
  fHistosRoot=filename;
}
//_____________________________________________________________________________
void HyMCApplication::SetTargetMaterial(int MediumID)
{
  fDetConstruction.SetTargetMaterial(MediumID);
}
//_____________________________________________________________________________
void HyMCApplication::SetMomentum(Double_t NewMomentum)
{
  fMomentum = NewMomentum;
}
//_____________________________________________________________________________
HyMCApplication::HyMCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fDetConstruction()
{    
  /// Default constructor
  EvntNumb = 0;
  evWr=0;
  fStack = new HyMCStack(1000000); 

  fLaNx = fDetConstruction.GetGeometry().GetLaNx();  // Arrays of energy deposition in LGD2
  fLaNy = fDetConstruction.GetGeometry().GetLaNy();
  fSmNx = fDetConstruction.GetGeometry().GetSmNx();
  fSmNy = fDetConstruction.GetGeometry().GetSmNy();
  LaEnDep = new Double_t*[fLaNx];
  for(Int_t tmp = 0; tmp<fLaNx; tmp++)
    LaEnDep[tmp] = new Double_t[fLaNy];
  SmEnDep = new Double_t*[fSmNx];
  for(Int_t tmp = 0; tmp<fSmNx; tmp++)
    SmEnDep[tmp] = new Double_t[fSmNy];

  fHistosRoot="histos.root";
  fMCResults="MC_results.dat";
  fMCgendat="MCgen.dat";
  fMomentum = 7.;
  fPCNumber = 0;
  fControl = 0;
  fExTarget=-1;
  fExChanel=-1;
  fExReson=-1;
  fSaEnergyCut=0.0023;
  if (iseed <= 0) iseed = 0;  // ;kondr, v1.04

  initial_photon_energy = new Double_t[100];
  
  energy_dep = new Double_t*[100];
  for(Int_t tmp = 0; tmp<100; tmp++)energy_dep[tmp]= new Double_t[640];

  gen3 = new TRandom3(iseed);

  Double_t eBeam = TMath::Sqrt(0.139570*0.139570 + fMomentum*fMomentum);
  

  
   fDebug = 0;
}

//_____________________________________________________________________________
HyMCApplication::~HyMCApplication() 
{
  /// Destructor  
  
  delete gMC;
  gMC = 0;
}

//
// private methods
// *** Break *** segmentation violation

//_____________________________________________________________________________
//
// public methods
//
void HyMCApplication::PreInit()
{
  if(fDebug) cout<<endl<<"-----------PreInit-----------"<<endl;

}
//_____________________________________________________________________________
void HyMCApplication::AddParticles()
{
  //add new particle (positively charged missing mass)
  //gMC->DefineParticle(992212,"MisMass",kPTHadron,0,3.,0.);
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
void HyMCApplication::InitMC(const char* setup)
{    
  /// Initialize MC.
  /// The selection of the concrete MC is done in the macro. (use g3tgeo)
  /// \param setup The name of the configuration macro 

  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
  //My cuts in VMC style; just comment them for default parameters
  gMC->SetCut("CUTGAM", 0.001 );
  gMC->SetCut("CUTELE", 0.001 );
  gMC->SetCut("BCUTE" , 0.001 );
  gMC->SetCut("BCUTM" , 0.001 );
  gMC->SetStack(fStack);
  gMC->Init();
  gMC->BuildPhysics(); 
  gMC->SetCollectTracks(false);
  
}

//_____________________________________________________________________________
void HyMCApplication::RunMC(Int_t nofEvents)
{    
  /// Run MC.
  /// \param nofEvents Number of events to be processed
  gMC->SetCollectTracks(false);
  
  fPbeam.SetXYZT(0.,0.,fMomentum,sqrt(fMomentum*fMomentum+0.139570*0.139570));

  cout<<"is collect tracks = "<< gMC->IsCollectTracks()<<endl;
  fMC_results=fopen(fMCResults,"wr");
  fprintf(fMC_results, "b ");
  fFile = new TFile(fHistosRoot,"RECREATE");
  fMCgen_dat=fopen(fMCgendat,"w");
  fprintf(fMCgen_dat,"/# means comment )\n");
  fprintf(fMCgen_dat,"/# This is generator output \n");
  fprintf(fMCgen_dat,"/# 1st string: 'begin event' word \n");
  fprintf(fMCgen_dat,"/# 2nd string: Nevent Ngamma Xvertex Yvertex Zvertex \n");
  fprintf(fMCgen_dat,"/# 3rd string: PbeamX PbeamY PbeamZ PbeamE \n");
  fprintf(fMCgen_dat,"/# 4th string: PgammaX PgammaY PgammaZ PgammaE of 1st photon\n");
  fprintf(fMCgen_dat,"/# ...........\n");
  fprintf(fMCgen_dat,"/# (3+Ngamma)th string: PgammaX PgammaY PgammaZ PgammaE of (Ngamma)th photon\n");
  fprintf(fMCgen_dat,"/# means comment )\n");
  
  

  fLGD2EnergyHisto = new TH1F("LGD2EnergyHisto", "Registered LGD2 energy in event", 1000 , 0., 10.);  // Total energy dep. in LGD2
  fSaEnergyHisto = new TH1F("SaEnergyHisto", "Registered Sa energy in event", 10000 , 0, 1);           // Total energy dep. in Sa
  fS4EnergyHisto = new TH1F("S4EnergyHisto", "Registered S4 energy in event", 10000 , 0., 1);          // Total energy dep. in S4
  fEffMassHisto =  new TH1F("EffMass 2Y", "EffMass", 100 , 0., 1);
  fTargetEnergyHisto = new TH1F("TargetEnergyHisto", "target energy in event", 10000 , 0., 1);

  fPhotonVertexZPosition=new TH1F("PhotonVertexZPositionHisto", "vertex Z pos", 10000, -100.,100);
  fPtHisto = new TH1F("PtHisto","Pt in event", 5000, 0., 5.);
  CoordHisto=new TH2F("CoordHisto","CoordHisto",100,-10.,10.,100,-10.,10.);
  cout<<"now "<<nofEvents<<" events will be processed. seed = " << iseed <<endl;
  gMC->ProcessRun(nofEvents);
  FinishRun();
}

//_____________________________________________________________________________
void HyMCApplication::FinishRun()
{    
  /// Finish MC run.
  cout<< "I wrote "<<evWr<<"events"<<endl;
  fclose(fMC_results);
  fFile->Write();
}

//_____________________________________________________________________________
void HyMCApplication::ConstructGeometry()
{    
  /// Construct geometry using detector contruction class.
  /// The detector contruction class is using TGeo functions or
  /// TVirtualMC functions (if oldGeometry is selected)

  cout << "Geometry will be defined via VMC" << endl;
  
  fDetConstruction.ConstructMaterials();  
  fDetConstruction.ConstructVolumes(); 
} 

//_____________________________________________________________________________
void HyMCApplication::InitGeometry()
{    
  /// Initialize geometry

}

//_____________________________________________________________________________
void HyMCApplication::GeneratePrimaries()
{    
  /// Fill the user stack (derived from TVirtualMCStack) with primary particles.

  // Added 09.02.2011
 
  // Track ID (filled by stack)
  Int_t ntr;
 
  // Option: to be tracked
  Int_t toBeDone = 1; 
 
  // Particle type
  //Int_t pdg  = 0;    // geantino
  Int_t pdg  = 22;     //22 - photon; kProton; kPositron; Pi+ 211; Pi0 111; Elektron - 11; 
 
  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 
  /*
  // Position
  Double_t vx  = 0.; 
  Double_t vy  = 0.; 
  Double_t vz = -450.;
  Double_t tof = 0.;

  // Energy
  Double_t mass = 0.000;
  Double_t e  = sqrt(mass*mass + fMomentum*fMomentum);
 
  // Momentum
  Double_t px, py, pz;
  px = 0.; 
  py = 0.; 
  pz = sqrt(e*e - mass*mass); 


  // Add particle to stack 
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
  kPPrimary, ntr, 1., 0);*/
 
  //Added 02.03.2011
  //------Photon generator by SA and me--------
  //------7 Gev pi+ interact with neutrons of target, Sa and S4
  //------Fortran subroutine returns array of 8-momentums (px, py, pz, E, M, x, y, z) of 10 photons 
  //------XYZ are non-zeros for K0s production only-----------

  if(EvntNumb == 0){//firts call; it's time to set up interaction probabilities
    fDetConstruction.CalcInteractProbabilities(TargProb);
  
    //process environment variables
    char *env_val;
    env_val = getenv("PROBABILITY_S4");
    if(env_val!=NULL){
      cout<<"S4 interaction probability is modified by setting the environment variable PROBABILITY_S4 = ";
      Double_t val;
      sscanf(env_val,"%lf",&val);
      cout<< val << ";" << endl;
      TargProb[0] = val;
    }
    env_val = getenv("PROBABILITY_TARG");
    if(env_val!=NULL){
      cout<<"Target interaction probability is modified by setting the environment variable PROBABILITY_TARG = ";
      Double_t val;
      sscanf(env_val,"%lf",&val);
      cout<< val << ";" << endl;
      TargProb[1] = val;
    }
    env_val = getenv("PROBABILITY_SA");
    if(env_val!=NULL){
      cout<<"SA interaction probability is modified by setting the environment variable PROBABILITY_SA = ";
      Double_t val;
      sscanf(env_val,"%lf",&val);
      cout<< val << ";" << endl;
      TargProb[2] = val;
    }
    //normalize probabilities to 1;
    float crSec = TargProb[0] + TargProb[1] + TargProb[2];
    TargProb[0]/=crSec;
    TargProb[1]/=crSec;
    TargProb[2]/=crSec;
    if (fDebug)
      {
	cout<<"TargProb[0] = "<<TargProb[0] ;
	cout<<"TargProb[1] = "<<TargProb[1] ;
	cout<<"TargProb[2] = "<<TargProb[2] ;
      }
  }
  int Ntarget;
  float rndmNumb = gen3->Rndm();
  if (rndmNumb<TargProb[0]){ Ntarget = 1;}
  else  if(rndmNumb<TargProb[0]+TargProb[1]) {Ntarget = 2;}
  else  Ntarget = 3;
  if(fExTarget>0) Ntarget=fExTarget;
  if(fDebug)cout<<"This event happened on target #"<<Ntarget<<endl;
  Double_t vertex[3]={0.,0.,0.};
  fDetConstruction.GambleDecayVertex(Ntarget, vertex);
 //  double Pgamma[10][8];
 //  int Ntarget, Nchanel, Nreson, Ngamma;
  float Pt;
  
 //  //first call to setup event generator
 //  if(EvntNumb == 0) {
 //    Ngamma = gRandom->Integer(4294967296); //randomize numerical continua in eventgen
 //    Ntarget=fExTarget; // if ExTarget, ExReson, ExChanel >= 0 then these values will be used as stearing parameters
 //    Nreson=fExReson;  //otherwise those from generator source code will be used
 //    Nchanel=fExChanel;
 //    hyp_event_(TargProb,&Ntarget,&Nchanel,&Nreson,&Ngamma,&Pgamma[0][0],&fMomentum,&fControl);
 //  } 
 // for (Int_t i =0;i<10;i++){
 //    for (Int_t j=0;j<8;j++) Pgamma[i][j]=0;}

 //  Ngamma = 0;
 //  int counter=0;
 //  while(Ngamma == 0){
 //    counter++;
 //    hyp_event_(TargProb,&Ntarget,&Nchanel,&Nreson,&Ngamma,&Pgamma[0][0],&fMomentum,&fControl);   // call Fortran routine
 //    //add cuts here
 //    if(counter > 10000) break;
 //  }
 //  if(fDebug) cout<<endl<<counter<<" times tried to generate photons"<<" for event number "<<EvntNumb<<endl;
 //  if(counter >= 10000){
 //    fStack->PushTrack(toBeDone, -1, 0, 0., 0., 1., 1., 0., 0., 0., 0., polx, poly, polz, kPPrimary, ntr, 1., 0);
 //    cout<<"Failed to generate photons"<<endl;
 //    cout<<"\nGeantino added"<<endl;
 //  }
 //  //Add all photons to stack
 

  //max_gamma=Ngamma;

  Double_t px,py,pz,e,x,y,z,p;
  Double_t Plab[4];
  Plab[0]=0.;
  Plab[1]=0.;
  Plab[2]=0.;
  Plab[3]=0.;

 //  if(fDebug==2){
 //    Ngamma=1;
 //    Pgamma[0][0]=0.;
 //    Pgamma[0][1]=0.;
 //    Pgamma[0][2]=1.;
 //    Pgamma[0][3]=1.;
 //  }
  // Rotate all gammas (beam's angle uncertainty)
  Double_t theta = gen3->Gaus(0.0,0.003); //RMS = 3 mrad
  //cout << "theta = "<<theta<<endl;
  Double_t phi= gen3->Uniform(2*TMath::Pi());
  TVector3 n(TMath::Cos(phi),TMath::Sin(phi),0.);

  TVector3 pBeam(0,0,fMomentum);
  Double_t eBeam = TMath::Sqrt(0.139570*0.139570 + fMomentum*fMomentum);
  pBeam.Rotate(theta,n);

  Double_t pout[500][10];//remember inversed order of indices in 2D array in fortran
  Float_t FlBeam = fMomentum;
  Int_t nTries = 0;
  Int_t iError = 0, jRec = 0, istOut = 0;
  Int_t fortran_seed = gRandom->Integer(iseed);  // pass the seed into the RegGen generator ;kondr v.1.04
  if(EvntNumb == 0) //randomize RegGen
     nTries = gRandom->Integer(4294967295); //randomize numerical continua in eventgen
//  reggen_event_(&FlBeam,&fExChanel,&fExReson,&nTries,&iError,&fortran_seed,&jRec,&istOut,&pout[0][0]);//generate 1 event
  pout[0][0] = fControl; // control parameter
  if (fDebug ==2) cout << "generating 1 interaction" << endl;
  reggen_event_(&FlBeam,&nTries,&iError,&fortran_seed,&jRec,&istOut,&pout[0][0]);//generate 1 event
  while(nTries<1000 && iError) { //in case of generator error try to generate until number of tries < 1000
    pout[0][0] = fControl;
    reggen_event_(&FlBeam,&nTries,&iError,&fortran_seed,&jRec,&istOut,&pout[0][0]);
  }
//    reggen_event_(&FlBeam,&fExChanel,&fExReson,&nTries,&iError,&fortran_seed,&jRec,&istOut,&pout[0][0]);
  if(fDebug ==2 ){//print generated reaction number
    cout<< "RegGen: I generated 1 event with reaction number "<<jRec<< " and I tried "<<nTries<<" times. seed is " << fortran_seed <<endl;
    }

  int nCharged = 0;
  int nFinal=istOut;
  double angles[500];//angles between primary particles direction and beamline
  int indices[500];//indices of primary particles
  // Loop over particles in one event
  for (int i = 0; i < nFinal; ++i) {
    //particle properties
    px = pout[i][0];
    py = pout[i][1];
    pz = pout[i][2];
    p = TMath::Sqrt(px*px+py*py+pz*pz);
    e=pout[i][3];
    angles[i] = TMath::ACos(pz/p);
    indices[i] = i;
    if(fDebug ==2 ){//print generated event
      cout<< "Particle "<<i<< " with PDG code "<<pout[i][8]<< " has (px,py,pz,e) = ( "<<px<<" , "<<py<< " , "<<pz<<", "<<e<<" )"<<endl;
    }
  }
  //we want to re-order generated particles as follows:
  //The closest to the beam line particle goes first as it will interact with Sa most likely (i.e. last in the stack)
  //sort angles from pi to 0
  for (int i = 0;i<nFinal;i++){
    int maxI;double maxA;
    maxA = angles[i];
    maxI = i;
    //find max angle
    for (int j = i+1; j<nFinal; j++)
      if(maxA<angles[j]){
	maxA = angles[j];
	maxI = j;
      }
    //swap i-th and max elements
    int tmpI = indices[i];double tmpD = angles[i];
    angles[i] = maxA;
    indices[i] =indices[maxI];
    angles[maxI] = tmpD;
    indices[maxI]=tmpI;
    if(fDebug)cout<<i<<"-th particle in stack has angle "<<angles[i]<<endl;
  }
  //add primary particles to the stack 
  //first to stack -> last to track
  //add neutral particles first and then charged particles
  //so charged particles will be tracked first
  //if they produce hits in Sa then such event is rejected by trigger conditions
  //and tracking is stopped to save time
  bool *isPushed = new bool[nFinal];
  TParticlePDG* particle;
  //add neutral particles to stack first
  int primariesCounter=0;
  for (int i = 0; i < nFinal; ++i) {
    isPushed[i] = 0;
    //particle properties
    px = pout[indices[i]][0];
    py = pout[indices[i]][1];
    pz = pout[indices[i]][2];
    e  = pout[indices[i]][3];
    x  = pout[indices[i]][5];
    y  = pout[indices[i]][6];
    z  = pout[indices[i]][7];
    pdg = pout[indices[i]][8];
    particle = TDatabasePDG::Instance()->GetParticle(pdg);
    //check if particle is neutral
    if(particle){
      if(particle->Charge()==0.)
	{
	  isPushed[i] = 1;
	}
    }
    if(!isPushed[i])continue;//charged particle will be added later
    if(pdg==100){ toBeDone = 0;}//do not track
    else toBeDone = 1;

    TVector3 Pg(px,py,pz);
    TVector3 PgVertex(x,y,z);

    Pg.Rotate(theta,n);     
    PgVertex.Rotate(theta,n);
    px=Pg.X();
    py=Pg.Y();
    pz=Pg.Z();
    x=PgVertex.X();
    y=PgVertex.Y();
    z=PgVertex.Z();

    //fprintf(fMCgen_dat,"%.16E %.16E %.16E %.16E %.16E %.16E %.16E\n",px,py,pz,e,x,y,z);

    Plab[0]+=px;
    Plab[1]+=py;
    Plab[2]+=pz;
    Plab[3]+=e;

    if(pdg==100) pdg=0;//geantino
    fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0]+x, vertex[1]+y, vertex[2]+z, 0., polx, poly, polz, kPPrimary, ntr, 1., 0);//adding particle to stack

    initial_photon_energy[primariesCounter]=e;
    primariesCounter++;
  }
  //add charged particles to stack
  for (int i = 0; i < nFinal; ++i) {
    if(isPushed[i])continue;//skip particle if it is already added to the stack
    //particle properties
    px = pout[indices[i]][0];
    py = pout[indices[i]][1];
    pz = pout[indices[i]][2];
    e  = pout[indices[i]][3];
    x  = pout[indices[i]][5];
    y  = pout[indices[i]][6];
    z  = pout[indices[i]][7];
    pdg = pout[indices[i]][8];
    if(pdg==100){ toBeDone = 0;}//do not track
    else toBeDone = 1;
    TVector3 Pg(px,py,pz);
    TVector3 PgVertex(x,y,z);
    Pg.Rotate(theta,n);     
    PgVertex.Rotate(theta,n);
    px=Pg.X();
    py=Pg.Y();
    pz=Pg.Z();
    x=PgVertex.X();
    y=PgVertex.Y();
    z=PgVertex.Z();
    Plab[0]+=px;
    Plab[1]+=py;
    Plab[2]+=pz;
    Plab[3]+=e;
    if(pdg==100) pdg=0;//geantino
    fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0]+x, vertex[1]+y, vertex[2]+z, 0., polx, poly, polz, kPPrimary, ntr, 1., 0);//adding particle to stack
    initial_photon_energy[primariesCounter]=e;
    primariesCounter++;
  }

  //backward going pion to simulate beam energy deposition in Sa
  toBeDone = 1;
  pdg=211;//pi+
  px=-pBeam.X();
  py=-pBeam.Y();
  pz=-pBeam.Z();
  e=eBeam;
  fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0], vertex[1], vertex[2], 0., polx, poly, polz, kPPrimary, ntr, 1., 0);

  current_primary_number=nFinal-1+1;//+1 is bacward going pion
  fNPrimary=current_primary_number;
  max_gamma= nFinal;
 //  // fprintf(fMCgen_dat,"/# means comment )\n");
 //  // fprintf(fMCgen_dat,"/# This is generator output \n");
 //  // fprintf(fMCgen_dat,"/# 1st string: 'begin event' word \n");
 //  // fprintf(fMCgen_dat,"/# 2nd string: Nevent Ngamma Xvertex Yvertex Zvertex \n");
 //  // fprintf(fMCgen_dat,"/# 3rd string: PbeamX PbeamY PbeamZ PbeamE \n");
 //  // fprintf(fMCgen_dat,"/# 4th string: PgammaX PgammaY PgammaZ PgammaE Xgamma Ygamma Zgamma of 1st photon\n");
 //  // fprintf(fMCgen_dat,"/# ...........\n");
 //  // fprintf(fMCgen_dat,"/# (3+Ngamma)th string: PgammaX PgammaY PgammaZ PgammaE Xgamma Ygamma Zgamma of (Ngamma)th photon\n");
 //  // fprintf(fMCgen_dat,"/# means comment )\n");


 //  fprintf(fMCgen_dat,"begin event\n");
 //  fprintf(fMCgen_dat,"%d %d %.16E %.16E %.16E\n",EvntNumb,Ngamma,vertex[0], vertex[1], vertex[2]);
 //  fprintf(fMCgen_dat,"%.16E %.16E %.16E %.16E\n", pBeam.X(), pBeam.Y(), pBeam.Z(), eBeam);
  

  // for(Int_t i=0; (i<Ngamma)&&(i<10) ; i++)
 
  //   {
  //     px = Pgamma[i][0];
  //     py = Pgamma[i][1];
  //     pz = Pgamma[i][2];
  //     e  = Pgamma[i][3];
  //     x  = Pgamma[i][5];
  //     y  = Pgamma[i][6];
  //     z  = Pgamma[i][7];

  //     TVector3 Pg(px,py,pz);
  //     TVector3 PgVertex(x,y,z);

  //     Pg.Rotate(theta,n);     
  //     PgVertex.Rotate(theta,n);
  //     px=Pg.X();
  //     py=Pg.Y();
  //     pz=Pg.Z();
  //     x=PgVertex.X();
  //     y=PgVertex.Y();
  //     z=PgVertex.Z();

  //     fprintf(fMCgen_dat,"%.16E %.16E %.16E %.16E %.16E %.16E %.16E\n",px,py,pz,e,x,y,z);

  //     Plab[0]+=px;
  //     Plab[1]+=py;
  //     Plab[2]+=pz;
  //     Plab[3]+=e;

  //     initial_photon_energy[i]=e;
  //     if(isnan(e))initial_photon_energy[i]=0;

  //     if (!isnan(e)){fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0]+x, vertex[1]+y, vertex[2]+z, 0., polx, poly, polz, kPPrimary, ntr, 1., 0);//adding photon to stack
  //     }else 
  // 	fStack->PushTrack(toBeDone, -1, 0, 0., 0., 1., 1., 0., 0., 0., 0., polx, poly, polz, kPPrimary, ntr, 1., 0);
  //     if(fDebug){
  // 	//gammas printing 
  // 	cout<<"\n=====gamma "<<(i+1)<<"======"<<endl;
  // 	cout<<"px= "<<px<<endl;
  // 	cout<<"py= "<<py<<endl;
  // 	cout<<"pz= "<<pz<<endl;
  // 	cout<<"e = "<<e<<endl;
  //     }
  //   }
  //also add missing mass
  //NOT NOW, do it later
  /*
  Double_t Pproducts[10][8],Pmismas[8];
  Int_t NPDGcodes,MPDGcode[10];
  Float_t rProtonsToNucleons = 0.5;//dummy
  int targMat=fDetConstruction.GetTargetMaterial();
  if(targMat==2)rProtonsToNucleons=4./9.;
  if(targMat==10)rProtonsToNucleons=6./12.;
  if(targMat==3)rProtonsToNucleons=13./27.;
  if(targMat==9)rProtonsToNucleons=29./63.;
  if(targMat==8)rProtonsToNucleons=50./120.;
  if(targMat==7)rProtonsToNucleons=82./208.;
  
  for(int iC=0;iC<8;iC++) Pmismas[iC]=Pgamma[Ngamma][iC];

  if(fDebug){
    cout<<"I've produced resonance Nreson= "<<Nreson<<endl;
    cout<<"Going to decay missing mass:"<<endl;
    cout<<"Px="<<Pmismas[0]<<endl;
    cout<<"Py="<<Pmismas[1]<<endl;
    cout<<"Pz="<<Pmismas[2]<<endl;
    cout<<"E ="<<Pmismas[3]<<endl;
    cout<<"M ="<<Pmismas[4]<<endl;
  }

  decay_missingmass_(&Nreson,&rProtonsToNucleons,&Pmismas[0],&NPDGcodes,&MPDGcode[0],&Pproducts[0][0]);
  for(int iPart=0;iPart<NPDGcodes;iPart++){
    px = Pproducts[iPart][0];
    py = Pproducts[iPart][1];
    pz = Pproducts[iPart][2];
    e  = Pproducts[iPart][3];
    x  = Pproducts[iPart][5];
    y  = Pproducts[iPart][6];
    z  = Pproducts[iPart][7];
    pdg = MPDGcode[iPart];
    initial_photon_energy[Ngamma+iPart]=e;
    if(isnan(e))initial_photon_energy[Ngamma+iPart]=0;
    if (!isnan(e))fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0]+x, vertex[1]+y, vertex[2]+z, 0., polx, poly, polz, kPPrimary, ntr, 1., 0);//adding missing mass products to stack
  }
  if(fDebug){
    cout<<"\n=====missing mass======"<<endl;
    for(int iPart=0;iPart<NPDGcodes;iPart++){
      cout<<"part"<<iPart+1<<endl;
    cout<<"px= "<<Pproducts[iPart][0]<<endl;
    cout<<"py= "<<Pproducts[iPart][1]<<endl;
    cout<<"pz= "<<Pproducts[iPart][2]<<endl;
    cout<<"e = "<<Pproducts[iPart][3]<<endl;
    cout<<"pdg = "<<MPDGcode[iPart]<<endl;
    }
  }
  */
  //fPpart.SetXYZT(Plab[0],Plab[1],Plab[2],Plab[3]);
  
  //backward going pion to simulate beam energy deposition. no. let's assume that we always have some signal in s4.
  // pdg=211;//pi+
  // px=-pBeam.X();
  // py=-pBeam.Y();
  // pz=-pBeam.Z();
  // e=eBeam;
  // fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vertex[0], vertex[1], vertex[2], 0., polx, poly, polz, kPPrimary, ntr, 1., 0);
  
  // if(fDebug)cout << " Ngamma = " << Ngamma;
  // if(Ngamma>0){
  //   //                    #gammas   #missing mass products  #beam particle  #counts from N-1 to 0
  //   current_primary_number=Ngamma-1;//  + NPDGcodes               +1              -1;
  //   fNPrimary=current_primary_number;// + 1;//number of primary particles
  //   max_gamma=Ngamma;//+NPDGcodes;
  //   fPhotonVertexZPosition->Fill(vertex[2]+Pgamma[0][7]);
  //   Pt=sqrt(Plab[0]*Plab[0]+Plab[1]*Plab[1]);
  //   fPtHisto->Fill(Pt);
  //   CoordHisto->Fill(vertex[0],vertex[1]);
    
  // }
  fFinishTracking=false;
}

//_____________________________________________________________________________
void HyMCApplication::BeginEvent()
{    
  /// User actions at beginning of event.
  
  for(Int_t i = 0; i<fLaNx; i++){
    for(Int_t j = 0; j<fLaNy; j++)
      LaEnDep[i][j] = 0;
  }
  
  for(Int_t i = 0; i<fSmNx; i++){
    for(Int_t j = 0; j<fSmNy; j++)
      SmEnDep[i][j] = 0;
  }
  fTargetEnergyDep = 0;
  fSaEnergyDep = 0;
  fS4EnergyDep = 0;

  for(Int_t i = 0; i<100; i++){
    for(Int_t j = 0; j<640; j++)
      energy_dep[i][j] = 0;
  }
 
  gGeoManager->ClearTracks();
  //current_primary_number = 0;
}

//_____________________________________________________________________________
void HyMCApplication::BeginPrimary()
{    
  /// User actions at beginning of a primary track.
  /// Nothing to be done this example
  if(fDebug)cout<<"begin primary"<<endl;
  if(fDebug == 2){//print track info
     std::cout << "  Particle = ";
     TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(gMC->TrackPid());
     if (particle)
       std::cout << particle->GetName() << "  ";
     else
       std::cout << "unknown" << "  ";
     std::cout << "   Track ID = " << gMC->GetStack()->GetCurrentTrackNumber() << "  ";
     std::cout << "   Parent ID = " << gMC->GetStack()->GetCurrentParentTrackNumber()<<endl;
  }
}

//_____________________________________________________________________________
void HyMCApplication::PreTrack()
{  
  /// User actions at beginning of each track.
  /// Nothing to be done this example
  //TVirtualGeoTrack* track = gGeoManager->MakeTrack(fStack->GetCurrentTrackNumber(), fStack->GetCurrentTrack()->GetPdgCode(), fStack->GetCurrentTrack());
  //  gGeoManager->AddTrack(track);
}

//_____________________________________________________________________________
void HyMCApplication::Stepping()
{    
  /// User actions at each step
  
  if(fFinishTracking){
    gMC->StopTrack();
    return;
  }

  if(current_primary_number == fNPrimary){ //this is backward going pion; we need it only to simulate Sa energy deposition.
    if (strcmp(gMC->CurrentVolName(), "SATR") != 0) gMC->StopTrack();
  }
  // Printing current track position
  if(fDebug==3)
    {
      TLorentzVector position;
      gMC->TrackPosition(position);
  
      cout << "Track " 
	   << position.X() << " " << position.Y() << " " << position.Z() 
	   << "  in " <<  gMC->CurrentVolName() << "  ";
      cout << endl;
    }
  static Int_t Ix, Iy, tmp;
  
 
  // Small glasses energy lose calculation
  if ((strcmp(gMC->CurrentVolName(), "SM_SENS_GLAS") == 0)){/*Center*/
    // 	cout<<"centre"<<endl;

    gMC->CurrentVolOffID(1, Iy);
    gMC->CurrentVolOffID(2, Ix);
    SmEnDep[Ix-1][Iy-1]+=  gMC->Edep() ;
    // cout<<"Ix= "<<Ix<<endl;
    // cout<<"Iy= "<<Iy<<endl;

    energy_dep[current_primary_number][Ix-1+(Iy-1)*8+576]+=gMC->Edep();
  }

  // Large glasses energy lose calculation
  if (strcmp(gMC->CurrentVolName(), "LA_SENS_GLAS") == 0){
    gMC->CurrentVolID(tmp);
    if (tmp == 1) {
      gMC->CurrentVolOffID(3, tmp);
      if (tmp == 1){/*Left*/
	//		cout<<"left"<<endl;
	//	cout<<"tmp= "<< tmp<<endl;

	gMC->CurrentVolOffID(1, Iy);/**/ 
	gMC->CurrentVolOffID(2, Ix);/**/ 
	// cout<<"Ix= "<<Ix<<endl;
	//   cout<<"Iy= "<<Iy<<endl;

	LaEnDep[Ix-1][Iy-1]+= gMC->Edep();
      }
      else{/*Right*/
	//		cout<<"right"<<endl;
	//	cout<<"tmp= "<< tmp<<endl;

	gMC->CurrentVolOffID(1, Iy);/**/ 
	gMC->CurrentVolOffID(2, tmp);/**/ 
	Ix = tmp + fLaNx/2. + fSmNx/4.;/**/ 
	//	cout<<"tmp= "<< tmp<<endl;

	//   cout<<"Ix= "<<Ix<<endl;
	//   cout<<"Iy= "<<Iy<<endl;

	LaEnDep[Ix-1][Iy-1]+= gMC->Edep();
      }
    }
    else{
      gMC->CurrentVolOffID(3, tmp);
      if (tmp == 1){/*Top*/
	//	cout<<"top"<<endl;
	//	cout<<"tmp= "<< tmp<<endl;
	gMC->CurrentVolOffID(1, tmp);/**/ 
	Iy = tmp + fLaNy/2. + fSmNy/4.;/**/ 
	gMC->CurrentVolOffID(2, tmp);/**/ 
	Ix = tmp + fLaNx/2. - fSmNx/4.;/**/ 
	//	cout<<"Ix= "<<Ix<<endl;
	//	cout<<"Iy= "<<Iy<<endl;

	LaEnDep[Ix-1][Iy-1]+= gMC->Edep();
      }
      else{/*Bottom*/
	//		cout<<"bottom"<<endl;
	//	cout<<"tmp= "<< tmp<<endl;

	gMC->CurrentVolOffID(1, Iy);/**/ 
	gMC->CurrentVolOffID(2, tmp);/**/ 
	Ix = tmp + fLaNx/2. - fSmNx/4.;/**/ 
	//	cout<<"Ix= "<<Ix<<endl;
	//	cout<<"tmp= "<<tmp<<endl;
	//	cout<<"Iy= "<<Iy<<endl;
	LaEnDep[Ix-1][Iy-1]+= gMC->Edep();}
    }
    energy_dep[current_primary_number][Ix-1+(Iy-1)*24]+=gMC->Edep();
  }
  // cout<<"Current Parent Track Number = "<<fStack->GetCurrentParentTrackNumber()<<endl;
  //cout<<"is collect tracks = "<< gMC->IsCollectTracks()<<endl;

  // Sa energy loss calculation
  if (strcmp(gMC->CurrentVolName(), "SATR") == 0){
    fSaEnergyDep+=gMC->Edep();
  }
  
  // S4 energy loss calculation
  if (strcmp(gMC->CurrentVolName(), "S4TR") == 0){
    fS4EnergyDep+=gMC->Edep();
  }
  
  // Target energy loss calculation
  if (strcmp(gMC->CurrentVolName(), "TARG") == 0){
    fTargetEnergyDep+=gMC->Edep();
  }
  if(fSaEnergyDep>fSaEnergyCut)fFinishTracking=true;
}

//_____________________________________________________________________________
void HyMCApplication::PostTrack()
{    
  /// User actions at each step.
  /// Nothing to be done this example
}

//_____________________________________________________________________________
void HyMCApplication::FinishPrimary()
{    
  /// User actions after finishing of a primary track.
  current_primary_number--;
}

//_____________________________________________________________________________
void HyMCApplication::FinishEvent()
{
  //visualisation. just comment it if you don't want to visualize.
  // gMC->Gdraw("WRLD", 90., 180.);
  // cout<<"=========visualisation========="<<endl;
  // gGeoManager->SetVisOption(0);     
  // gGeoManager->SetTopVisible();
  // gGeoManager->GetTopVolume()->Draw();
  // gGeoManager->DrawTracks("/\*");  // this means all tracks
  
  //Histo for total energy in LGD2 
  Double_t EnTotal;
  EnTotal = 0;
  Int_t i;

  for( i = 0; i<fLaNx; i++){
    for(Int_t j = 0; j<fLaNy; j++){
      EnTotal+=LaEnDep[i][j];
      //      e[j+i*LaNy]=float(LaEnDep[i][j]); // Eqvivalence: i = ix+(iy-1)*24 for large glasses
    }
  }
  
  for( i = 0; i<fSmNx; i++){
    for(Int_t j = 0; j<fSmNy; j++){
      EnTotal+=SmEnDep[i][j];
      //      e[j+i*SmNy+fLaNx*LaNy]=float(SmEnDep[i][j]);// Eqvivalence: i = ix+(iy-1)*8+24*24 for small glasses
    }
  }

  //prepair collected energy
  //otbor po schetchiku Sa
  if(fSaEnergyDep<fSaEnergyCut)
    {//begin Sa-discrimination
      fprintf(fMC_results,"%d ", EvntNumb);
      for (i=0;i<max_gamma; i++)
	{
	  fprintf(fMC_results,"%lf ", initial_photon_energy[i]);
	  for(Int_t j=0; j<640; j++)
	    {
	      if(energy_dep[i][j]>0) fprintf(fMC_results, "%d %lf ", j, energy_dep[i][j]);
	    }
	  if((i+1)!=max_gamma){fprintf(fMC_results, "%d & ", -1);}
	  else fprintf(fMC_results, "%d\n# ", -1);
	}
      // Writing data to file runXXXX.gz
      // no hyperon format writing in this version
      // convert MC_results.dat with ../converter/exe
      evWr++;
    }//end Sa-discrimination

  if(fLGD2EnergyHisto&&(EnTotal>0)) fLGD2EnergyHisto->Fill(EnTotal);
  if(fDebug)cout << "Total energy = " << EnTotal << " GeV" << endl;

  //Histo for energy loses in Sa
  if (fSaEnergyHisto) fSaEnergyHisto->Fill(fSaEnergyDep);

  //Histo for energy loses in S4
  if (fS4EnergyHisto) fS4EnergyHisto->Fill(fS4EnergyDep);

  //Histo for energy loses in target
  if (fTargetEnergyHisto) fTargetEnergyHisto->Fill(fTargetEnergyDep);
  //  cout<<"chislo chastic v steke="<<fStack->GetNtrack()<<endl;
  //cout<<"chislo pervichnih chastic="<<fStack->GetNprimary()<<endl;
  fStack->Reset(); 
  //gObjectTable->Print();

  cout << " \nEvent number = " << EvntNumb << endl;
  EvntNumb++;


} 

//_____________________________________________________________________________

//_____________________________________________________________________________
void HyMCApplication::DrawLGD2EnergyHisto()
{
 
  fLGD2EnergyHisto->Draw();
  
}
//_____________________________________________________________________________
void HyMCApplication::DrawSaEnergyHisto()
{

  fSaEnergyHisto->Draw();
  
}
//_____________________________________________________________________________
void HyMCApplication::DrawS4EnergyHisto()
{


  fS4EnergyHisto->Draw();
  
}
//_____________________________________________________________________________
void HyMCApplication::DrawEffMassHisto()
{


  fEffMassHisto->Draw();

}

#ifndef _DONT_USE_NETWORK_FUNCTIONS_
#define _DONT_USE_NETWORK_FUNCTIONS_

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//message parts:
#define _BLOCK_LENGTH_ 16
#define _HEAD_LENGTH_ _BLOCK_LENGTH_  // 16
#define _ID_LENGTH_ _BLOCK_LENGTH_    // 4
#define _EMPTY_SYMBOL_ '_'

#define _NF_MAX_BUFFER_LENGTH_ 1024
// #define _NF_HOST_ "alice16"
#define _NF_HOST_ "10.8.0.2"
#define _NF_PORT_ 54321

int HyMCApplication::GetSeed()
{
  return iseed;
}

void HyMCApplication::SetServer(std::string _server)
{
  g_server = _server;  
}

void HyMCApplication::SetPort(int _port)
{
  g_port = _port;
}

void HyMCApplication::SetConnection()
{
  SetConnection(_NF_HOST_, _NF_PORT_);
}

void HyMCApplication::SetConnection(std::string _server, int _port)
{
  SetServer(_server);
  SetPort(_port);  
}

void HyMCApplication::SendAlive()
{
  std::string message = make_message(GetSeed(), "alive", "");
  printf ("GREPME: sending alive signal: '%s'\n", message.c_str());
  SendMessage(message);
//make_message(GetSeed(), make_head("alive"), ""));
}

void HyMCApplication::SendMessage(std::string message)
{
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) { printf("ERROR opening socket\n"); return; }

  server = gethostbyname(g_server.c_str());
  if (server == NULL) { fprintf(stderr,"ERROR, no such host\n"); return; }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(g_port);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { printf("ERROR connecting\n"); return ; }

//   printf("%s\n",message.c_str());
//   usleep(1e6);
  n = write(sockfd,message.c_str(),message.size());
  if (n < 0) { printf("ERROR writing to socket\n"); return; }
//   printf("wait..\n");
  
  close(sockfd);
}


void *HyMCApplication::HelperFunc(void * object)
{
//   printf ("Helper here \n");
  ((HyMCApplication *) object)->MessageCycle(true);
}
void *HyMCApplication::HelperFunc2(void * object)
{
//   printf ("Helper2 here \n");
  ((HyMCApplication *) object)->MessageCycle2(true);
}

void HyMCApplication::MessageCycle(bool sime_global)
{
//   printf ("message cycle here \n");
//   printf("seed: '%d'\n",GetSeed());
//  SendAlive();
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  printf ("GREPWORD: server: '%s', port: '%d'; \n", g_server.c_str(), g_port);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) { printf("ERROR opening socket\n"); return; }

  server = gethostbyname(g_server.c_str());
  if (server == NULL) { fprintf(stderr,"ERROR, no such host\n"); return; }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(g_port);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { printf("ERROR connecting\n"); return ; }
  std::string message; // = make_message(123, "message", "here is the message ge !");
  message = make_message(GetSeed(), "alive", "justmc");    printf("message to send: '%s'\n",message.c_str());  n = write(sockfd,message.c_str(),message.size());
//  message = make_message(GetSeed(), "justmc", "");    printf("message to send: '%s'\n",message.c_str());  n = write(sockfd,message.c_str(),message.size());
  usleep(1e6);  
//   printf("z '%s'\n",message.c_str());
//   int i = 0;
//   string message;
  while(true)
  {
//     printf("b\n");
// //     if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { printf("ERROR connecting\n"); return ; }
//     printf("zz '%s'\n",message.c_str());
    
    message = make_message(GetSeed(), make_head("event"), EvntNumb);
//    message = make_message(GetSeed(), make_head("chat"), EvntNumb);
// //     message = make_message(321, make_head("event"), "current event");
    printf("message to send: '%s'\n",message.c_str());
//     printf("seed: '%d'\n",GetSeed());
//     printf("i: '%d'\n",i);
// //     n = write(sockfd,message.c_str(),message.size());
// //     n = send(sockfd,message.c_str(),message.size(), 0);
    n = write(sockfd,message.c_str(),message.size());
//    message = make_message(GetSeed(), make_head("chat"), EvntNumb);    n = write(sockfd,message.c_str(),message.size());
    
    if (n < 0) { printf("ERROR writing to socket\n"); return; }
    usleep(3e6);
//     close(sockfd);
//     if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { printf("ERROR connecting\n"); return ; }
//     close(sockfd);
  }
  
  close(sockfd);
}

void HyMCApplication::MessageCycle2(bool sime_global)
{
//   cout << endl << endl<< endl;
//   cout << "ee-";
//   cout .flush();
//  if (mi_main != NULL){ cout << mi_main << "hmmmm-" << (*mi_main); cout .flush();
//   if (sime_global || true){  cout << "tested!" ;
  if (sime_global)
  {
    cout << "tested!" ;
    SendTestMC();
  }
//}
//   cout << "eeee" <<endl;
//   cout.flush();
}

void HyMCApplication::ThreadCycle() { ThreadCycle  (false); }
void HyMCApplication::ThreadCycle2(){ ThreadCycle2 (false); }

void HyMCApplication::ThreadCycle(bool sime_global)
{
  if (sime_global == false) return;
  pthread_t a;
//   int err = pthread_create(&a, NULL, HyMCApplication::HelperFunc, NULL);
  int err = pthread_create(&a, NULL, HelperFunc, this);
  if (err != 0)   printf("\ncan't create thread :[%s]", strerror(err));
  else            printf("\n Thread created successfully\n");
//   usleep(2e6);
//   pthread_join( a, NULL);
//   printf ("wow\n");
  
}
void HyMCApplication::ThreadCycle2(bool sime_global)
{
  if (sime_global == false) return;
  pthread_t a2;
//   int err = pthread_create(&a, NULL, HyMCApplication::HelperFunc, NULL);
  int err = pthread_create(&a2, NULL, HelperFunc2, this);
  if (err != 0)   printf("\ncan't create thread :[%s]", strerror(err));
  else            printf("\n Thread2 created successfully\n");
//   usleep(2e6);
//   pthread_join( a, NULL);
//   printf ("wow\n");
  
}


//*****************************************************************************************//
//the following functions withdraw some information from the message or create message of necessary format:
std::string HyMCApplication::make_head(std::string head)
{
   head.resize (_HEAD_LENGTH_,_EMPTY_SYMBOL_);
   return head;
}

std::string HyMCApplication::make_block(std::string block)
{
  if (block.size() == 0)
    return "";
  if ( block.size() % _BLOCK_LENGTH_ == 0 )
    return block;
  if ( block.size() + _BLOCK_LENGTH_ - ( block.size() % _BLOCK_LENGTH_ ) < _NF_MAX_BUFFER_LENGTH_ )
    block.resize ( block.size() + _BLOCK_LENGTH_ - ( block.size() % _BLOCK_LENGTH_ ), _EMPTY_SYMBOL_);
  else
    block.resize ( _NF_MAX_BUFFER_LENGTH_, _EMPTY_SYMBOL_);
  return block; // "[____][____][____][__" -> "[____][____][____][____]"  fill the rest of the block at the end with the "_EMPTY_SYMBOL_" (don't forget to compare it with the max_buffer_length)
}

/*
std::string HyMCApplication::append_block(std::string &message, std::string block)
{
  message.append(make_block(block));
  return message;
}
*/

std::string HyMCApplication::append_block(std::string message, std::string block)
{
  message.append(make_block(block));
  return message;
}


std::string HyMCApplication::make_block(int block)
{
  std::stringstream ss;
  ss << block;
  std::string block_id = ss.str();
  return make_block(block_id);
}

std::string HyMCApplication::make_client_id(int int_client_id)
{
  std::stringstream ss;
  ss << int_client_id;
  std::string client_id = ss.str();
  client_id.resize(_ID_LENGTH_, _EMPTY_SYMBOL_);
  return client_id;
}

std::string HyMCApplication::make_message(int int_client_id, std::string head, std::string message = "")
{
  std::stringstream ss;
  std::string client_id = make_client_id(int_client_id);
  ss << client_id;
  for (int i = 0; i < _ID_LENGTH_ - (int)client_id.length(); i++)
    ss << _EMPTY_SYMBOL_;
  head = make_head(head);
  ss << head;
  for (int i = 0; i < _HEAD_LENGTH_ - (int)head.length(); i++)
    ss << _EMPTY_SYMBOL_;

  ss << make_block(message);
  return ss.str();
}

std::string HyMCApplication::make_message(int int_client_id, std::string head, int message)
{
  std::stringstream ss;
  std::string client_id = make_client_id(int_client_id);
  ss << client_id;
  for (int i = 0; i < _ID_LENGTH_ - (int)client_id.length(); i++)
    ss << _EMPTY_SYMBOL_;
  head = make_head(head);
  ss << head;
  for (int i = 0; i < _HEAD_LENGTH_ - (int)head.length(); i++)
    ss << _EMPTY_SYMBOL_;

  ss << make_block(message);
  return ss.str();
}

std::string HyMCApplication::get_client_id(std::string message)
{
  try
  {
    std::string client_id = message.substr(0, _ID_LENGTH_);
    client_id = message.substr(0, std::min(_ID_LENGTH_, (int)(client_id.find_first_of(_EMPTY_SYMBOL_))));
    return client_id;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in get_client_id: " << e.what() << "\n";
    return make_client_id(-1);
  }
}

int HyMCApplication::get_int_client_id(std::string message)
{
   return std::atoi(get_client_id(message).c_str());  //atoi transfers c-string (char) into int. (atof-into float)
}

std::string HyMCApplication::get_head(std::string message)
{
  try
  {
    std::string head = message.substr(_ID_LENGTH_, _HEAD_LENGTH_);
    head = head.substr(0, std::min(_HEAD_LENGTH_, (int)(head.find_first_of(_EMPTY_SYMBOL_))));
    return head;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in get_head: " << e.what() << "\n";
    return make_head((std::string)"error");
  }
}

std::string HyMCApplication::get_block(std::string message, int block_number = 2) // from 2 ; id -> 0 ; head -> 1.
{
  try
  {
    std::string block = message.substr(_ID_LENGTH_ + _HEAD_LENGTH_ + _BLOCK_LENGTH_ * (block_number - 2), _BLOCK_LENGTH_);
    block = block.substr(0, std::min(_BLOCK_LENGTH_, (int)(block.find_first_of(_EMPTY_SYMBOL_))));
    return block;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in get_block: " << e.what() << "\n";
    return make_block((std::string)"error");
  }
}

std::string HyMCApplication::get_chat_message(std::string message)
{
  try
  {
    return message.substr(_ID_LENGTH_ + _HEAD_LENGTH_, message.size() - (_ID_LENGTH_ + _HEAD_LENGTH_));;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in get_head: " << e.what() << "\n";
    return (std::string)"error";
  }

}

#endif
