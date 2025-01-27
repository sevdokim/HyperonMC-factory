#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

const int kNch = 900; // 24*24 + 8*8;
gzFile fFile = 0;
static int fRunNumber = 1000;
char fName[512];
const int kMaxLen = 40000;
int gGroupLen = 0;
static int gTotalLen = 0;
static int fBlockNumber = 0;
int fIdx = 0;
int gGroupLenIdx; // pointer to group length value
const char *fConnectTable = "geo_connection_table.dat.2004-2005";
// const char *fConnectTable =
// "/home/onuchin/geo_connection_table.dat.2004-2005";

int fHadr[900]; // hardware address
float fCalibr[900];
int gBuf[40000];

static float energy[900];

//______________________________________________________________________________
static void readConnectionTable() {
  //

  const char *file_name = 0;
  FILE *fp;
  char buf[1024];
  int ih, ig;
  int big;
  int beginRun;
  int endRun;
  int chn = 0;
  int i = 0;

  for (i = 0; i < kNch; i++) {
    fHadr[i] = 0; // preset
  }

  file_name = getenv("H2S_TABLE");

  if (!file_name) {
    file_name = fConnectTable;
  }

  fp = fopen(file_name, "r");

  if (!fp) {
    printf("failed to open connection table: %s\n", file_name);
    return;
  }

  while ((fgets(buf, 1024, fp))) {
    if (buf[0] == '#')
      continue;
    if (strstr(buf, "RUN")) {
      sscanf(buf, "RUN %d %d", &beginRun, &endRun);

      if (beginRun < 700) { // use table for 2005 year
        continue;
      }

      if (!endRun)
        endRun = 1000000;

      while ((fgets(buf, 1024, fp))) {
        sscanf(buf, "%d %d %d", &ih, &ig, &big);
        chn = big ? ig : 576 + ig;
        fHadr[chn - 1] = ih;
        // printf("%d %d %d %d\n", ih, ig, big, chn);
      }
    }
  }
  fclose(fp);

  // init calibr (temporary solution)
  for (i = 0; i < kNch; i++) {
    fCalibr[i] = 1.;
    energy[i] = 0.2 + 0.002 * i * fCalibr[i]; // used for debugging
  }
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
  gBuf[5] = 1886;       // length the group of data
  gBuf[6] = 1000;
  gBuf[7] = 1;
  gBuf[8] = 0;
  gBuf[9] = 0;
  gBuf[10] = 2701; // step (672*4 + 3 + 7 + 1)
  gBuf[11] = 0;
  gBuf[12] = 71; // LE71
  gBuf[13] = 7;  // N crates
  gBuf[14] = 96; // N channels in crate

  gBuf[15] = 2688;
  gBuf[16] = 2560;
  gBuf[17] = 2176;
  gBuf[18] = 2048;
  gBuf[19] = 2432;
  gBuf[20] = 2304;
  gBuf[21] = 1920;
  gBuf[22] = 672;

  adr = 22;

  for (i = 0; i < 672; i++) {
    idx = adr + 4 * i;

    hadr = fHadr[i];
    gBuf[idx + 1] = hadr; // hardware address
    gBuf[idx + 2] = 100;  // ped
    gBuf[idx + 3] = 3;    // pedw
    gBuf[idx + 4] = 100;  // thresh
  }
  gzwrite(fFile, gBuf, 10844);

  for (i = 0; i < 2711; i++) {
    gBuf[i] = 0; // clear buffer
  }
}

//______________________________________________________________________________
static void newFile() {
  //

  static int readTable = 0;

  if (readTable == 0) {
    readConnectionTable();
    readTable = 1;
  }

  sprintf(fName, "Run%d.gz", fRunNumber);

  fFile = gzopen(fName, "wb");

  writePeds();

  gTotalLen = 0;
}

//______________________________________________________________________________
static void newBlock() {
  //

  gTotalLen = 0;
  fBlockNumber++;

  gBuf[0] = 1048579; // the type of data (taken from the real data)
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
static void processEnergy(float *e) {
  //

  int hadr; // hardware address
  int v;    // value to write
  int a;    // amplitude
  int i;
  float c; // calibr
  int le69;

  gGroupLen = 0;

  for (i = 0; i < 640; ++i) {
    hadr = fHadr[i];

    if (!hadr || (e[i] == 0)) { // no such channel or no signal
      continue;
    }

    c = fCalibr[i];
    a = (int)(e[i] * 1000. / c);
    v = (hadr << 16) + a;

    if (a == 0) { // zero amplitude
      continue;
    }

    gGroupLen++;
    gBuf[fIdx] = v;

    fIdx++;
  }

  gBuf[5] = gGroupLen + 4 + 37;
  gBuf[11] = gGroupLen + 37 + 802816;
  le69 = gBuf[5] + 5 - 2; // le69 index

  gBuf[le69] = 4096 + fBlockNumber;
  gBuf[le69 + 1] = 0xA822;
  gBuf[le69 + 2] = 20;
  gBuf[le69 + 3] = 10;
  gBuf[le69 + 4] = 301;
  gBuf[le69 + 5] = 0;
  gBuf[le69 + 6] = 1;
  gBuf[le69 + 7] = fBlockNumber;

  // some dummy data
  gBuf[le69 + 8] = 68588057;
  gBuf[le69 + 9] = 393856;
  gBuf[le69 + 10] = 271450752;
  gBuf[le69 + 11] = 278200960;
  gBuf[le69 + 12] = 281608832;
  gBuf[le69 + 13] = 640;
  gBuf[le69 + 14] = 787120;
  gBuf[le69 + 15] = 271319728;
  gBuf[le69 + 16] = 278069936;
  gBuf[le69 + 17] = 688;
  gBuf[le69 + 18] = 656096;
  gBuf[le69 + 19] = 271057632;
  gBuf[le69 + 20] = 280298208;
  gBuf[le69 + 21] = 736;
  gBuf[le69 + 22] = 0;

  gBuf[3] = (gBuf[5] + 21) * 4;

  gTotalLen += gBuf[3] + 16;
}

//______________________________________________________________________________
static void writeAndReset() {
  //

  int hadr; // hardware address
  int a;    // amplitude
  int debug = 1;
  int i = 0;
  int len = gBuf[3] + 16;

  if (gGroupLen > 1) {
    gzwrite(fFile, gBuf, len);

    for (i = 0; i < len / 4; i++) {
      if (debug) {
        a = gBuf[i];
        hadr = (a >> 16) & 07777;
        a &= 07777;
      }
      gBuf[i] = 0;
    }
  }
  fIdx = 0;
  gGroupLen = 0;
}

//______________________________________________________________________________
void writehyp_(float *e) {
  //

  int debug = 0;

  if (!fFile) {
    newFile();
  }

  if (!fIdx) {
    newBlock();
  }

  // process
  processEnergy(debug ? energy : e);

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
