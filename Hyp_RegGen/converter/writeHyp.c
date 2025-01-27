#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

// #define pi 3.14159265358979
using namespace std;

double rnd_gauss() {
  static double rho, phi, R, rmax = double(RAND_MAX);
  R = rand() / rmax;
  rho = sqrt(-2 * log(R));
  phi = (rand() / rmax) * 2 * 3.14159265358979;
  return rho * cos(phi);
}

const float mPi0 = 134.977; // MeV
const int kNch = 900;       // 24*24 + 8*8;
gzFile fFile = 0;
static int fRunNumber;
char fName[512];
const int kMaxLen = 40000;
static int gGroupLen = 0;
static int gTotalLen = 0;
static int fBlockNumber = 0;
int fIdx = 0;
int gGroupLenIdx; // pointer to group length value
const char *fRelativeConnect = "h_s_new.dat";
const char *fCoeff = "coeff_old.dat";
const char *fAbsoluteConnect = "hard_soft.dat";
const char *fMCCoeff = "MC_calibr.dat";
const char *fCalibrCards = "calibr.cards";
const char *fBadChannels = "bad_channels.dat";
const char *fMassShifts = "mass_shifts.dat";
const char *fMassShiftsMC = "mass_shifts_MC.dat";
int fHadr[900]; // hardware address
float fCalibr[900];
float fMCCalibr[640];
int gBuf[40000];
static int MISS[7] = {1100, 1200, 1300, 1400, 1500, 1600, 2900};
static int Cher[4] = {1, 32, 33, 34};
static bool badChannels[640];
// Cher[0] ==1 => new address format (by default); Cher[0] == 0 => old address
// format Cher[0]==-1 => do not write cherenkovs data

static float energy[900];
float eNorm = 1.;
int int_from_string_after_symbol(char *buf, char symbol) {
  if (buf == NULL) {
    cerr << "Cannot read int_from_string_after_symbol() from NULL string!!!"
         << endl;
    return 0;
  }
  int i = 0, val;
  while (buf[i] != symbol && buf[i] != '\n' && buf[i] != '\0') {
    // cout<<buf[i]<<endl;
    i++;
  }
  if (buf[i] == '\n' || buf[i] == '\0') {
    cerr << "Cannot read int_from_string_after_symbol(): symbol " << symbol
         << " not found!!!" << endl;
    return 0;
  }
  // cout<<&buf[i+1]<<endl;
  sscanf(&buf[i + 1], "%d", &val);
  return val;
}

//______________________________________________________________________________
static void readConnectionTable() {
  //

  const char *file_name = 0;
  FILE *fp;
  char buf[1024];
  int ih, ig;
  int big;
  int chn = 0;
  int i = 0, j = 0;
  int module;
  int IsAbsolute = 0;
  for (i = 0; i < kNch; i++) {
    fHadr[i] = 0; // preset
  }

  file_name = fCalibrCards;

  fp = fopen(file_name, "r");
  if (!fp) {
    printf("failed to open calibr.cards; cherenkovs will not be used!");
    Cher[0] = -1;
  }
  while (fgets(buf, 1024, fp)) {
    if (buf[0] == 'C' && buf[1] == 'H' && buf[2] == 'A' && buf[3] == 'D')
      sscanf(&buf[4], "%d %d %d %d", Cher, &Cher[1], &Cher[2], &Cher[3]);
  }

  cout << "I read cherenkovs addresses from calibr.cards : " << endl;
  cout << "Address format : " << Cher[0] << " ; Adr 1 :" << Cher[1]
       << " ;Adr 2 :" << Cher[2] << " ;Adr 3 :" << Cher[3] << endl;

  file_name = fRelativeConnect;
  fp = fopen(file_name, "r");

  if (!fp) {
    printf("failed to open connection table: %s\n", file_name);
    return;
  }
  if (!IsAbsolute) {
    while ((fgets(buf, 1024, fp))) {
      if (buf[0] == '#')
        continue;
      if (strstr(buf, "hard/soft/centr/modul")) {
        for (i = 0; i < 7; i++) {
          for (j = 0; j < 96; j++) {
            fgets(buf, 1024, fp);
            sscanf(buf, "%d %d %d %d", &ih, &ig, &big, &module);
            if (ig > 0) {
              chn = big ? ig : 576 + ig;
              if (fHadr[chn - 1] > 0)
                printf("double geo connection to cell %d\n", chn);
              fHadr[chn - 1] = ih + MISS[i];
            }
          }
        }
      }
    }
  } else {
    while ((fgets(buf, 1024, fp))) {
      if (buf[0] == '#')
        continue;
      while ((fgets(buf, 1024, fp))) {
        sscanf(buf, "%d %d %d", &ih, &ig, &big);
        if (ig > 0)
          chn = big ? ig : 576 + ig;
        fHadr[chn - 1] = ih;
      }
    }
  }
  fclose(fp);

  file_name = "coeff_old.dat";
  fp = fopen(file_name, "r");

  if (!fp) {
    printf("failed to open coeff. table: %s\n", file_name);
    return;
  } else {
    printf("I read following coeffs:\n");
    char str[8];
    for (i = 0; i < 576; i++) {
      if ((i) % 12 == 0 && i != 0)
        fgets(str, 2, fp);
      fgets(str, 8, fp);
      sscanf(str, "%f", &fCalibr[i]);
      printf("%7.3f ", fCalibr[i]);
      if ((i + 1) % 12 == 0) {
        printf("\n");
      }
      // fscanf(fp, "%f", &fCalibr[i]);//doesn't work if you have something like
      // 123.456123.456 in coef_old.dat
    }
    for (i = 0; i < 64; i++) {
      if ((i) % 8 == 0)
        fgets(str, 2, fp);
      fgets(str, 8, fp);
      sscanf(str, "%f", &fCalibr[i + 576]);
      printf("%7.3f ", fCalibr[i + 576]);
      if ((i + 1) % 8 == 0) {
        printf("\n");
      }
      // fscanf(fp, "%f", &fCalibr[i+576]);
    }
  }
  fclose(fp);

  int int_eNorm = 0;
  fp = fopen("file_list.dat", "r");
  if (!fp) {
    cerr << "ERROR: No filelist found" << endl;
  } else {
    while ((fgets(buf, 500, fp))) {
      if (buf[0] == '#')
        continue;
      if ((buf[0] == '/') && (buf[2] == 'D') && (buf[3] == 'i')) {
        cout << buf << endl;
        int_eNorm = int_from_string_after_symbol(buf, '.');
        cout << "I read following eNorm coeff: " << int_eNorm << endl;
        break;
      }
    }
    fclose(fp);
  }
  if (int_eNorm > 0)
    eNorm = int_eNorm / 10000.;

  // bad channels
  for (int i = 0; i < 640; i++) {
    badChannels[i] = false;
  }
  fp = fopen(fBadChannels, "r");
  if (!fp) {
    cout << "No bad channels are provided. All channels considered to be good!"
         << endl;
  } else {
    cout << "Reading bad channels from " << fBadChannels << "." << endl;
    int badChannel = -1;
    while (EOF != fscanf(fp, "%d\n", &badChannel)) {
      if (badChannel >= 640 || badChannel < 0)
        continue;
      cout << "Bad channel " << badChannel << endl;
      badChannels[badChannel] = true;
    }
    fclose(fp);
  }

  // mass shifts of pi0 peak position in every cell (from experimental data)
  fp = fopen(fMassShifts, "r");
  if (!fp) {
    cout << "No experimental mass shifts for calibration coefs are provided. "
            "No shift to calib coeffs is applied.";
  } else {
    cout << "Reading experimental mass shifts from " << fMassShifts
         << ". Shifts will be applied to calibration coefs." << endl;
    int channel;
    float massShift;
    while (EOF != fscanf(fp, "%d %f\n", &channel, &massShift)) {
      if (channel >= 640 || channel < 0)
        continue;
      if (!badChannels[i] || fCalibr[i] == 0.)
        continue;
      fCalibr[i] += 2. * massShift / mPi0 * fCalibr[i];
      if (fCalibr[i] <= 0.) {
        fCalibr[i] = 0.;
        badChannels[i] = true;
      }
    }
    fclose(fp);
  }

  // mass shifts of pi0 peak position in every cell (from MC)
  fp = fopen(fMassShiftsMC, "r");
  if (!fp) {
    cout << "No MC mass shifts for calibration coefs are provided. No shift to "
            "calib coeffs is applied.";
  } else {
    cout << "Reading MC mass shifts from " << fMassShifts
         << ". Shifts will be applied to calibration coefs." << endl;
    int channel;
    float massShift;
    while (EOF != fscanf(fp, "%d %f\n", &channel, &massShift)) {
      if (channel >= 640 || channel < 0)
        continue;
      if (!badChannels[i] || fCalibr[i] == 0.)
        continue;
      fCalibr[i] -= 2. * massShift / mPi0 * fCalibr[i];
      if (fCalibr[i] <= 0.) {
        fCalibr[i] = 0.;
        badChannels[i] = true;
      }
    }
    fclose(fp);
  }

  // i don't use it by now
  /* fp = fopen(fMCCoeff, "r"); */
  /* if (!fp) {      printf("failed to open MC coeff. table: %s\n", fMCCoeff);
   */
  /*    return; */
  /* } */
  /* else */
  /*    { */

  /* 	 for (i = 0; i<576; i++) */
  /*      	{ */
  /* 		   fscanf(fp, "%f", &fMCCalibr[i]); */
  /* 		} */
  /*       for (i = 0; i<64; i++) */
  /*              { */
  /* 		   fscanf(fp, "%f", &fMCCalibr[i+576]); */
  /*              } */
  /*    } */
  /* fclose(fp); */
  // init calibr (temporary solution)
  // for (i = 0; i < kNch; i++) energy[i] = 0.2 ;  // used for debugging
  /* for(i=0;i<640;i++){ */
  /*     printf("%d\t%d\n",i,fHadr[i]); */
  /* } */
}

//______________________________________________________________________________
static void writePeds() {
  //

  int adr;
  int idx;
  int hadr;
  int i = 0;

  gBuf[0] = 65546; // pedestals
  gBuf[1] = 1;
  gBuf[2] = 1133544553; // time (some value taken from real data)
  gBuf[3] = 10828;      // block length of pedestal data
  gBuf[4] = 0;          // separator
  gBuf[5] = fRunNumber; // Run
  // gBuf[6] = 0;		// Number of events in run
  gBuf[6] = 1000;
  gBuf[7] = 1;     // Number_of device groups
  gBuf[8] = 0;     // not used
  gBuf[9] = 0;     // not used
  gBuf[10] = 2701; // step (672*4 + 3 + 7 + 1) group length(in words)
  gBuf[11] = 0;    // Device ID (LE71) ("name")
  gBuf[12] = 71;   // Device ID (LE71) ("surname")
  gBuf[13] = 7;    // N crates
  gBuf[14] = 96;   // N channels in crate

  gBuf[15] = MISS[0];
  gBuf[16] = MISS[1];
  gBuf[17] = MISS[2];
  gBuf[18] = MISS[3];
  gBuf[19] = MISS[4];
  gBuf[20] = MISS[5];
  gBuf[21] = MISS[6];
  gBuf[22] = 672; // (7*96)

  adr = 22;

  /*for (i = 0; i < 672; i++) { // work ok with rusakovich programm
    idx = adr + 4*i;

    hadr = fHadr[i];
    gBuf[idx + 1] = hadr;   // hardware address
    gBuf[idx + 2] = 100;    // ped
    gBuf[idx + 3] = 3;      // pedw
    gBuf[idx + 4] = 100;    // thresh
    }*/
  for (i = 0; i < 7; i++) { // for bogoliub programm
    for (int j = 0; j < 96; j++) {
      idx = adr + 4 * (i * 96 + j);

      hadr = MISS[i] + j;
      gBuf[idx + 1] = hadr;
      gBuf[idx + 2] = 100;
      gBuf[idx + 3] = 3;
      gBuf[idx + 4] = 100;
    }
  }

  gzwrite(fFile, gBuf, 10844);

  for (i = 0; i < 2711; i++) {
    gBuf[i] = 0; // clear buffer
  }
}

//______________________________________________________________________________
static void newFile(int runnumber) {
  //

  static int readTable = 0;
  /*   cerr << "\nType run number" <<endl;
  scanf("%d", &fRunNumber);
  */
  fRunNumber = runnumber;
  if (readTable == 0) {
    readConnectionTable();
    readTable = 1;
  }

  sprintf(fName, "Run%d.gz", fRunNumber);

  fFile = gzopen(fName, "wb");

  writePeds(); // record 1 - service data

  gTotalLen = 0;
}

//______________________________________________________________________________
static void newBlock() // in fact new record, not used
{
  //

  gTotalLen = 0;
  fBlockNumber++;

  gBuf[0] =
      1048579; // the type of data (taken from the real data) data type = 3
  gBuf[1] = fBlockNumber;
  gBuf[2] = 1133544553; // time (some value taken from real data)
  gBuf[3] = 0;          // block length (unknown yet)
  gBuf[4] = 0;          // separator
  gBuf[5] = 0;          // length of the first group of data + 4 (unknown yet)
  gBuf[6] = 0;          // group id
  gBuf[7] = 0;          // device id
  gBuf[8] = 0;          // buffer is overflowed

  gBuf[9] = -1;            // "begin spill" (037777777777)
  gBuf[10] = fBlockNumber; // fBlockNumber + 1 (???)
  gBuf[11] = 0;            // group length (unknown) + ermiss = [5] - 4

  gGroupLenIdx = 11;
  fIdx = 12;
}

//______________________________________________________________________________
static void processEnergy(float *e, int time) {
  //

  int hadr; // hardware address
  int v;    // value to write
  int a;    // amplitude
  int i;
  float c; // calibr
  float TotalEnergy = 0;
  //   int   le69;
  float eshum, ashum;
  gGroupLen = 0;
  fIdx = 8;
  for (i = 0; i < 640; ++i) {
    // reject bad channels
    if (badChannels[i])
      continue;

    hadr = fHadr[i];
    // cout<<"e["<<i<<"]= "<<e[i]<<endl;
    TotalEnergy += e[i];
    if (!hadr || (e[i] == 0)) { // no such channel or no signal
      //	if (!hadr){cerr<<"cell= "<<i<<"hadr= "<<hadr<<endl;}
      continue;
    }

    c = fCalibr[i];
    // ashum = (int)(3.*rnd_gauss()+0.933*sqrt(e[i]*1000./c)*rnd_gauss());
    // ashum = (int)(4.*rnd_gauss()+1.244*sqrt(e[i]*1000./c)*rnd_gauss());
    // ashum = (int)(3.6*rnd_gauss()+1.196*sqrt(e[i]*1000./c)*rnd_gauss());
    // ashum = (int)(3.3*rnd_gauss()+1.0263*sqrt(e[i]*1000./c)*rnd_gauss());
    // Evd  17.04.2019
    // Sdv float A=0,B=3,C=3,sigma;// sigma/E = sqrt(A^2/E + B^2/E^2 + C^2)
    // float A=5.0,B=3,C=3,sigma;// sigma/E = sqrt(A^2/E + B^2/E^2 + C^2)
    // //before 27.01.22
    float A = 3.0, B = 3., C = 5, sigma;
    sigma = 0.01 * e[i] * sqrt(A * A / e[i] + B * B / e[i] / e[i] + C * C);
    ashum = (int)(sigma * rnd_gauss() * 1000. / c);
    a = (int)(e[i] * 1000. / (c * eNorm));
    if (c == 0)
      a = 0;
    v = /*(hadr << 16) + */ a + ashum;
    if (v <= 10) { // zero amplitude
      // cout<<"a= "<<a<<"c= "<<c<<"e[i]= "<<e[i]<<"i= "<<i<<endl;
      continue;
    }
    gGroupLen += 2;
    gBuf[fIdx] = hadr;
    gBuf[fIdx + 1] = v;

    fIdx += 2;
  }

  // cherenkovskie schetchiki
  if (Cher[0] >= 0) {
    // pi+
    // 1st
    gGroupLen += 2;
    hadr = Cher[0] * MISS[6] + Cher[1];
    gBuf[fIdx] = hadr;
    gBuf[fIdx + 1] = 1000;
    fIdx += 2;
    // 2nd
    gGroupLen += 2;
    hadr = Cher[0] * MISS[6] + Cher[2];
    gBuf[fIdx] = hadr;
    gBuf[fIdx + 1] = 1000;
    fIdx += 2;
    // 3rd
    gGroupLen += 2;
    hadr = Cher[0] * MISS[6] + Cher[3];
    gBuf[fIdx] = hadr;
    gBuf[fIdx + 1] = 1000;
    fIdx += 2;
  }
  /*
  //4rd=S4
  gGroupLen+=2;
  hadr = MISS[6]+Cher[3];
  gBuf[fIdx] = hadr;
  gBuf[fIdx+1] = 1000;
  fIdx+=2;
  */
  /*
  // Print energy dep. matrix
  for (int j = 0; j<24; j++)
  {
    for (i = 0; i<24; i++ )
    {
       printf("%4d ", int(1000.*e[i+j*24]));
    }
    cout << endl;
  }
  for (int j = 0; j<8; j++)
  {
    for (i = 0; i<8; i++ )
    {
       printf("%4d ", int(1000.*e[i+j*8+576]));
    }
    cout << endl;
    }*/
  // printf("total energy = %f \n", TotalEnergy);
  // printf("gGroupLen = %d \n", gGroupLen);
  //   gTotalLen = 0;
  fBlockNumber++;

  // gBuf[0] = 1048577; // taken from 2015-11 cu data (datatype = 1) ! not
  // working
  gBuf[0] =
      1048579; // the type of data (taken from the real data) data type = 3
  gBuf[1] = fBlockNumber;
  gBuf[2] = time; // time=event number
  // gBuf[3] = 4*(gGroupLen+4);   // block length (bytes) worked ok
  gBuf[3] = 4 * (gGroupLen + 4);
  gBuf[4] = 0; // separator
  // gBuf[5] = gGroupLen+4;   // length of the first group of data + 4 (words)
  // worked ok
  gBuf[5] = gGroupLen + 3;
  gBuf[6] = 0;  // group id
  gBuf[7] = 71; // device id
}

//______________________________________________________________________________
static void writeAndReset() {
  int len = gBuf[3] + 16;

  if (gGroupLen > 1) {
    gzwrite(fFile, gBuf, len);

    for (int i = 0; i < len / 4; i++) {
      gBuf[i] = 0;
    }
  }
  fIdx = 0;
  gGroupLen = 0;
}

//______________________________________________________________________________
void writehyp_(float *e, int runnumber, int time) {
  //
  int debug = 0;

  if (!fFile) {
    newFile(runnumber);
  }

  // if (!fIdx) {
  //    newBlock();
  // }

  // process
  processEnergy(e, time);

  writeAndReset();
  // gzclose(fFile);
}

//______________________________________________________________________________
void endhyp_() {
  //

  if (!fFile) {
    return;
  }

  writeAndReset();
  gzclose(fFile);
}
