#include <stdio.h>
#include <stdlib.h>
#include "writeHyp.h"
#include <math.h>
#include <iostream>

using namespace std;

char buf[40000];
double alfa1[100];
int KKK, ISTEP;
double une_corr(double x)
{
  double  
    a = 0.0829143,
    b = 0.587972,
    c = 0.488298,
    d = -0.22618,
    e = 0.0513797,
    f = -0.00569725,
    g = 0.000249205;
  return a+b*pow(x,1)+c*pow(x,2)+d*pow(x,3)+e*pow(x,4)+f*pow(x,5)+g*pow(x,6);
}
double calc_ecorr(double x, double* alfa, int istep, int kkk)
{
  double val_0,val_1;
  val_0=x;
  for(int j=1;j<=istep;j++){
    val_1=val_0+alfa[(j-1)*10];
    for(int i=2;i<=kkk;i++)
      val_1+=alfa[(j-1)*10+i-1]*pow(log(val_0),i-1);
    val_0=val_1;
  }
  return val_0;
}
double PolLaGrange(double x, double *xi, double *fxi, int n)
{
  if (n<=0) return 0;
  double *li = new double[n];
  double pol = 0.;
  //cout<<"xi[0] = "<<xi[0]<<endl;
  for (int i = 0; i<n; i++)
    {
      li[i] = 1.;
      for (int j = 0; j<n; j++)
	{
	  if (j!=i) li[i]*=(x-xi[j])/(xi[i]-xi[j]);
	}
      pol+=fxi[i]*li[i];
    }
  delete[] li;
  return pol;
}
double une_corr_new(double x)
{
  static bool first_time = true,isEcorMatrix=false;
  static double ecor_coefs[10];
  static double b[10],matrix[10][10];
  static double e[100001], e_corr[100001],dE, eBegin = 0.2, eEnd = 10.; 
  static int kkk, istep;

  if(first_time){
    first_time=false;
    // =========== read e_cor_matrix.dat first =================
    FILE* ecormatrix = fopen("e_cor_matrix.dat","r");
    if(ecormatrix!=NULL){
      fscanf(ecormatrix,"%d",&kkk);
      fscanf(ecormatrix,"%d",&istep);
      KKK=kkk;
      ISTEP=istep;
      for (int i = 0;i<10; i++){
	for (int j=0;j<10; j++){
	  fscanf(ecormatrix,"%lf", &matrix[i][j]);	
	  printf("%23.10e ", matrix[i][j]);
	}
	printf("\n");
      }
      printf("\n");
      for(int i = 0;i<10; i++){
	fscanf(ecormatrix,"%lf", &b[i]);
	printf("%23.10e ",b[i]);
      }
      printf("\n");
      for (int i = 0;i<10; i++){
	for (int j=0;j<10; j++){
	  fscanf(ecormatrix,"%lf", &alfa1[i*10+j]);
	  printf("%23.10e ", alfa1[i*10+j]);
	}
	printf("\n");
      }
      isEcorMatrix=true;
      // ============ prepare e vs e_corr array ================
      //e is photon energy
      //e_corr is corrected energy
      //range from eBegin to eEnd GeV
      //lets find e_corr[i] close to 0.5+dE*i
      // == first, find E such that ecorr(E) = 0.5
      dE=(eEnd-eBegin)/100000.;
      int iStep = 0;
      double ecor_cur, ecor_pr, e_cur, ecor_goal, EStep, ecor_new;
      e_cur=eBegin;
      for (iStep = 0;iStep<=100000;iStep++){
	ecor_goal = eBegin+iStep*dE;
	e_cur=ecor_goal;
	//if(iStep%200==0)cout<<"goal en ="<<ecor_goal<<" and current en="<<e_cur<<endl;
	ecor_cur = calc_ecorr(e_cur, alfa1, istep, kkk);
	//if(iStep%200==0)cout<<"ecor_cur en ="<<ecor_cur<<endl;
	EStep = ecor_cur-e_cur;
	ecor_pr  = (calc_ecorr(ecor_cur,alfa1, istep, kkk )-ecor_cur)/EStep;
	while(fabs(ecor_cur-ecor_goal)>0.0000001){
	  //cout<<"goal en ="<<ecor_goal<<" and current en="<<e_cur<<endl;
	  EStep = (ecor_goal-ecor_cur)/ecor_pr;
	  e_cur = e_cur+EStep;
	  ecor_new=calc_ecorr(e_cur, alfa1, istep, kkk);
	  ecor_pr  = (ecor_new - ecor_cur)/EStep;
	  ecor_cur = ecor_new;
	  
	}
	e_corr[iStep] = ecor_cur;
	e[iStep]      = e_cur;
	
	//if(iStep%200==0)cout<<"e_corr = " <<e_corr[iStep]<< "  for e = " << e[iStep]<< " and iStep = "<<iStep<<endl;
      }
    }
  }
  
  if(isEcorMatrix){
    int index = 100000*(x-eBegin)/(eEnd-eBegin);
    //cout<<"index = "<<index<<endl;
    if(index<0) return x;//no correction
    if(index==0){//linear approximation between 1st and 2nd points
      return PolLaGrange(x,e_corr,e,2);
    }
    if(index>=100000){//linear approximation between (n-1)th and n-th points
      return PolLaGrange(x,&e_corr[100000-1],&e[100000-1],2);
    }
    return PolLaGrange(x,&e_corr[index-1],&e[index-1],4);
  }
  else {return x;}//no energy correction
  
}
double draw_unecorr(double *x, double* par){
  return une_corr_new(x[0]);
}
double draw_ecorr(double *x, double* par){
  static bool first = true;
  if (first){une_corr_new(5.);first=false;}
  return calc_ecorr(x[0],alfa1, ISTEP,KKK);
}
double draw_ecorrOverE(double *x, double* par){
  return draw_ecorr(x,par)/x[0];
}
double draw_ecorrMinusE(double *x, double* par){
  return draw_ecorr(x,par)-x[0];
}
double draw_ecorrOverEminus1(double *x, double* par){
  return draw_ecorr(x,par)/x[0]-1.;
}



double Ecorr(double x)
{
  double 
    p0 = 0.9724,
    p1 = -0.02796,
    p2 = 0.004163,
    p3 = -0.0002716;
  return p0+p1*pow(x,1)+p2*pow(x,2)+p3*pow(x,3);
}

int main(int Npar, char* par[])
{
  cout<<"une_corr_new(0.2) = " << une_corr_new(0.2)<<endl;
  //return 0;
  FILE* input;
  char string[40000];
  int RunNumber,Nev_to_proceed=-1;
  if(Npar==1||Npar>4){
    cerr<<"This is MC format to Hyperon formft data converter."<<endl;
    cerr<<"Type 'h' for help, any else char to continue."<<endl;
    cin>>string;
    if(string[0]=='h')cout<<"try ./exe -help\n";
    cerr<<"Type input file name"<<endl;
    cin>>string;
    cerr<<"Type run number"<<endl;
    cin>>RunNumber;
  }
  if(Npar==2){
    cerr<<"\n========Usage========\n1st argument - MC data file to convert"<<endl;
    cerr<<"2nd argument - run number\n"<<endl;
    cerr<<"3rd argument - N of events to proceed\n"<<endl;
    return 0;
  }
  if(Npar==3){
    sprintf(string,par[1]);
    RunNumber=atoi(par[2]);
  }
  if(Npar==4){
    sprintf(string,par[1]);
    RunNumber=atoi(par[2]);
    Nev_to_proceed=atoi(par[3]);
  }
  input = fopen(string,"r");
  if(!input){cerr<<"Can't open file "<<string<<endl;return 0;}
  //printf(string);
  float e[640];
  bool old_version=false,first_time=true;
  double energy_dep[100][640];
  double initial_photon_energy[100];
  int i,j,PhotonNumber=0,fin;
  int ieve=0;//event number (will be written as time)
    while(fscanf(input,"%c",&string[0])!=EOF)
    {
      if(first_time){
	if(string[0]=='#')old_version=true;
	first_time = false;
      }
      if(old_version){//old format version
	if(string[0]=='#') 
	  {
          
	    for(i=0;i<PhotonNumber;i++)
	      for(j=0;j<640;j++){
              
            e[j]+=0.985*1.017*1.005*1.037*1.041*energy_dep[i][j]/Ecorr(initial_photon_energy[i]);
//	    e[j]+=energy_dep[i][j];      //  Possible simplification 17.04.2019
		  energy_dep[i][j]=0;}
		  
	    writehyp_(e,RunNumber,ieve);
	    for(j=0;j<640;j++)e[j]=0;
	    PhotonNumber=0;
	    ieve++;
	    continue;
	  }
	fscanf(input,"%lf\t%d", &initial_photon_energy[PhotonNumber],&fin);
	while(fin>0)
	  {
	    fscanf(input,"%lf %d ", &energy_dep[PhotonNumber][fin], &fin );
	  }
	PhotonNumber++;

      }
      else{//new format version
	//printf("%c",string[0]);
	if(string[0]=='b') {
	  fscanf(input," %d",&ieve);
	  //printf("%d\n",ieve);
	  }
	if(string[0]=='#') 
	  {
	    for(i=0;i<PhotonNumber;i++)
	      for(j=0;j<640;j++){
		//	e[j]+=energy_dep[i][j]*1.159;
	//	e[j]+=energy_dep[i][j]*1.142;
		//if(i+1==PhotonNumber&&e[j]>0.05)e[j]=une_corr_new(e[j]); //uncor for every cell - old way
		//e[j]+=energy_dep[i][j]*1.159; //uncor for every cell - old way
		e[j]+=energy_dep[i][j]*1.159*une_corr_new(initial_photon_energy[i])/initial_photon_energy[i];//unecor for every photon - new way
		energy_dep[i][j]=0;}
	    writehyp_(e,RunNumber,ieve);
	    for(j=0;j<640;j++)e[j]=0;
	    PhotonNumber=0;
	    fscanf(input," %d",&ieve);
	    //printf("\# %d\n", ieve );
	    continue;
	  }
	j=fin;
	fscanf(input," %lf %d ", &initial_photon_energy[PhotonNumber],&fin);
	
	//printf("%lf %d\n", initial_photon_energy[PhotonNumber],fin);
	while(fin>=0)
	  {
	    // if(initial_photon_energy[PhotonNumber]<0.000001) {
	    //  nextevent=true;
	    //  break;
	    //}
	    j=fin;
	    if(fscanf(input,"%lf %d ", &energy_dep[PhotonNumber][fin], &fin )==EOF) break;
	    //printf("%lf %d ", energy_dep[PhotonNumber][j], j );
	  }
	//printf("\n");
	PhotonNumber++;
      }
      if(ieve==Nev_to_proceed)break;
    }
  endhyp_();
}

