#include "writeHyp.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

char buf[40000];

double une_corr(double x) {
  double a = 0.0829143, b = 0.587972, c = 0.488298, d = -0.22618, e = 0.0513797,
         f = -0.00569725, g = 0.000249205;
  return a + b * pow(x, 1) + c * pow(x, 2) + d * pow(x, 3) + e * pow(x, 4) +
         f * pow(x, 5) + g * pow(x, 6);
}
double Ecorr(double x) {
  double p0 = 0.9724, p1 = -0.02796, p2 = 0.004163, p3 = -0.0002716;
  return p0 + p1 * pow(x, 1) + p2 * pow(x, 2) + p3 * pow(x, 3);
}

int main(int Npar, char *par[]) {
  FILE *input;
  char string[40000];
  char inputFileName[300];
  int RunNumber, Nev_to_proceed = -1;
  if (Npar == 1 || Npar > 4) {
    cerr << "This is MC format to Hyperon format data converter." << endl;
    cerr << "Type 'h' for help, any else char to continue." << endl;
    cin >> string;
    if (string[0] == 'h') {
      cout << "try " << par[0] << " -h\n";
      cerr << "========Usage========" << endl
	   << "1st argument - MC data file to convert" << endl
	   << "2nd argument - run number" << endl
	   << "3rd argument - N of events to proceed (optional)" << endl;
      return 0;
    }
    cerr << "Type input file name" << endl;
    cin >> inputFileName;
    cerr << "Type run number" << endl;
    cin >> RunNumber;
  }
  if (Npar == 2) {
    cerr << "========Usage========" << endl
	 << "1st argument - MC data file to convert" << endl
	 << "2nd argument - run number" << endl
	 << "3rd argument - N of events to proceed (optional)" << endl;
    return 1;
  }
  if (Npar == 3) {
    sprintf(inputFileName, par[1]);
    RunNumber = atoi(par[2]);
  }
  if (Npar == 4) {
    sprintf(inputFileName, par[1]);
    RunNumber = atoi(par[2]);
    Nev_to_proceed = atoi(par[3]);
  }
  input = fopen(inputFileName, "r");
  if (!input) {
    cerr << "Can't open file " << inputFileName << endl;
    return 2;
  }
  // printf(string);
  float e[640];
  bool old_version = false, first_time = true;
  double energy_dep[100][640];
  double initial_photon_energy[100];
  int i, j, PhotonNumber = 0, fin = -1;
  int ieve = 0; // event number (will be written as time)
  while (fscanf(input, "%c", &string[0]) != EOF) {
    // guess format from data
    if (first_time) {
      if (string[0] == '#')
        old_version = true;
      first_time = false;
    }
    if (old_version) {
      // ========= old format version =============
      if (string[0] == '#') {

        for (i = 0; i < PhotonNumber; i++)
          for (j = 0; j < 640; j++) {

            e[j] += 0.985 * 1.017 * 1.005 * 1.037 * 1.041 * energy_dep[i][j] /
                    Ecorr(initial_photon_energy[i]);
            //	    e[j]+=energy_dep[i][j];      //  Possible
            // simplification 17.04.2019
            energy_dep[i][j] = 0;
          }

        writehyp_(e, RunNumber, ieve);
        for (j = 0; j < 640; j++)
          e[j] = 0;
        PhotonNumber = 0;
        ieve++;
        continue;
      }
      fscanf(input, "%lf\t%d", &initial_photon_energy[PhotonNumber], &fin);
      while (fin > 0) {
        fscanf(input, "%lf %d ", &energy_dep[PhotonNumber][fin], &fin);
      }
      PhotonNumber++;

    } else {
      // ========== new format version ==============
      // printf("%c",string[0]);
      if (string[0] == 'b') {
        int retVal = fscanf(input, " %d", &ieve);
	if (retVal == EOF) {
	  cerr << "Format error in file " << inputFileName << endl;
	  return 3;
	}
	// printf("%d\n",ieve);
      }
      if (string[0] == '#') { // beginning of new event
	// write previous event to gz file
        for (i = 0; i < PhotonNumber; i++)
          for (j = 0; j < 640; j++) {
            e[j] += energy_dep[i][j] * 1.159;
            //	e[j]+=energy_dep[i][j]*1.142;
            if (i + 1 == PhotonNumber && e[j] > 0.5)
              e[j] = une_corr(e[j]);
            energy_dep[i][j] = 0;
          }
        writehyp_(e, RunNumber, ieve);
        for (j = 0; j < 640; j++)
          e[j] = 0;
        PhotonNumber = 0;
	// read event number of new event
        int retVal = fscanf(input, " %d", &ieve);
        // printf("\# %d\n", ieve );
        continue;
      }
      j = fin;
      int retVal = fscanf(input, " %lf %d ", &initial_photon_energy[PhotonNumber], &fin);
      if (retVal == EOF) {
	continue;
      }
      // printf("%lf %d\n", initial_photon_energy[PhotonNumber],fin);
      while (fin >= 0) {
        // if(initial_photon_energy[PhotonNumber]<0.000001) {
        //  nextevent=true;
        //  break;
        //}
        j = fin;
        fscanf(input, "%lf %d ", &energy_dep[PhotonNumber][fin], &fin);
        // printf("%lf %d ", energy_dep[PhotonNumber][j], j );
      }
      // printf("\n");
      PhotonNumber++;
    }
    if (ieve == Nev_to_proceed)
      break;
  }
  endhyp_();
}
