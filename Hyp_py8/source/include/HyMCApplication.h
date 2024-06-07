#ifndef HY_MC_APPLICATION_H
#define HY_MC_APPLICATION_H

#include <TROOT.h>

#include <TDatabasePDG.h>
#include <TGeoManager.h>
#include <TParticle.h>
#include <TParticlePDG.h>
#include <TVirtualGeoTrack.h>
#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>

#include <Riostream.h>
#include <TFile.h>
#include <TInterpreter.h>
#include <TMath.h>
#include <TObject.h>
#include <TObjectTable.h>
#include <TString.h>
#include <TTree.h>

#include "HyDetectorConstruction.h"
#include "HyGeoParameters.h"
#include "HyMCStack.h"

#include <TF1.h>
#include <TH1.h>
#include <TH2F.h>
#include <TLorentzVector.h>
#include <TPDGCode.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TVector3.h>
#include <cstdlib>
#include <iostream>

#include "Pythia8/Pythia.h"
// struct to write output tree
struct HyMCEvent : public TObject {
  HyMCEvent(int nPr = 10) {
    if (nPr > 0) {
      maxPrimary = nPr;
      pxP = new double[nPr];
      pyP = new double[nPr];
      pzP = new double[nPr];
      eP = new double[nPr];
      pdgCode = new int[nPr];
      cellNumber = new std::vector<int>[nPr];
      enDep = new std::vector<double>[nPr];
      for (int i = 0; i < nPr; i++) {
        cellNumber[i].reserve(64); // reserve at least 64 cells
        enDep[i].reserve(64);      // per primary particle
      }
    }
  }
  ~HyMCEvent() {
    if (nPrimary) {
      delete[] pxP;
      delete[] pyP;
      delete[] pzP;
      delete[] eP;
      delete[] pdgCode;
      delete[] cellNumber;
      delete[] enDep;
    }
  }
  void ResetEvent() {
    for (int i = 0; i < nPrimary; i++) {
      cellNumber[i].clear();
      enDep[i].clear();
    }
    nPrimary = 0;
  }
  void NewEvent(int nPr) {
    if (nPr > maxPrimary) { // extend arrays
      maxPrimary = nPr;

      delete[] pxP;
      delete[] pyP;
      delete[] pzP;
      delete[] eP;
      delete[] pdgCode;
      delete[] cellNumber;
      delete[] enDep;

      pxP = new double[nPr];
      pyP = new double[nPr];
      pzP = new double[nPr];
      eP = new double[nPr];
      pdgCode = new int[nPr];
      cellNumber = new std::vector<int>[nPr];
      enDep = new std::vector<double>[nPr];
      for (int i = 0; i < nPr; i++) {
        cellNumber[i].reserve(64); // reserve at least 64 cells
        enDep[i].reserve(64);      // per primary particle
      }
    } else { // clear vectors
      for (int i = 0; i < nPrimary; i++) {
        cellNumber[i].clear();
        enDep[i].clear();
      }
    }
    nPrimary = nPr;
  };
  int maxPrimary = 10;
  double vertex[3]; //
  int pdgA;         //
  int pdgB;         // pdg codes of projectile and target
  double eA;        // energies of projectile
  double eB = 0;    // energy of target assumed to be 0 (fix-target experiment)
  double pxA;       //
  double pyA;       //
  double pzA;       // momentum of projectile
  double pxB = 0.;  //
  double pyB = 0.;  //
  double pzB = 0.;  // momentum of target
  double signalSa;  //
  double signalS4;  //
  double targetEnDep;           //
  int nPrimary;                 //
  int *pdgCode;                 //[nPrimary] array of primaries' pdg codes
  double *pxP;                  //[nPrimary] primaries 4-momentum
  double *pyP;                  //[nPrimary]
  double *pzP;                  //[nPrimary]
  double *eP;                   //[nPrimary]
  std::vector<int> *cellNumber; //[nPrimary] cell addresses
  std::vector<double>
      *enDep; //[nPrimary] energy depositions left in LGD2 by each primary
  ClassDef(HyMCEvent, 1);
};

// HyperonMC application class
class HyMCApplication : public TVirtualMCApplication {
public:
  HyMCApplication(const char *name = "HyMCapp",
                  const char *title = "Hyperon MC application");
  virtual ~HyMCApplication();

  // static access method
  static HyMCApplication *Instance();

  // methods
  void PreInit();
  bool InitMC(const char *setup); // build physics, process macros
  void RunMC(Int_t nofEvents);    // run MC with setted number of events
  void FinishRun();               // user action when RunMC finished
  void SetMomentum(Double_t NewMomentum) {
    fMomentum = NewMomentum;
  } // set beam momentum
  void SetMCResults(const char *filename) {
    fMCResults = filename;
  } // set file name to write MC data
  void SetHistosRoot(const char *filename) {
    fHistosRoot = filename;
  } // set file name to write root histos
  void SetPCNumber(int pcnumber) {
    fPCNumber = pcnumber;
  } // set process number (if necessary)
  void SetControl(double control) {
    fControl = control;
    std::cout << "HyMCApplication::SetControl() : control is now " << fControl
              << std::endl;
  } // set parameter to control event gen
  void SetExTarget(int hh) { fDetConstruction.SetExTarget(hh); }
  void SetExReson(int hh) { fExReson = hh; }
  void SetExChanel(int hh) { fExChanel = hh; }
  void SetTargetMaterial(int MediumID) {
    fDetConstruction.SetTargetMaterial(MediumID);
  } // set target material, if not setted or
    // setted to nonexisting - will ask user
  void SetDebug(int deb) {
    fDebug = deb;
    fDetConstruction.SetDebug(deb);
  }
  void SetBeamPdg(int pdg) { fBeamPdg = pdg; }
  void SetTargetPdg(int pdg) { fTargetPdg = pdg; }

  virtual void ConstructGeometry();
  void AddParticles();
  virtual void InitGeometry();
  virtual void GeneratePrimaries();
  virtual void BeginEvent();
  virtual void BeginPrimary();
  virtual void PreTrack();
  virtual void Stepping();
  virtual void PostTrack();
  virtual void FinishPrimary();
  virtual void FinishEvent();

private:
  // methods

  // data members
  Int_t fDebug = 0;
  Int_t fLaNx; ///< geomety formats of LGD2
  Int_t fLaNy; ///< geomety formats of LGD2
  Int_t fSmNx; ///< geomety formats of LGD2
  Int_t fSmNy; ///< geomety formats of LGD2

  TFile *fFile; ///< file to save root histos

  TH1F *fLGD2EnergyHisto;       ///< histos
  TH1F *fSaEnergyHisto;         ///< histos
  TH1F *fS4EnergyHisto;         ///< histos
  TH1F *fEffMassHisto;          ///< histos
  TH1F *fTargetEnergyHisto;     ///< histos
  TH1F *fPhotonVertexZPosition; ///< histos
  TH1F *fPtHisto;               // Pt in event  ///< histos
  TH2F *fCoordHisto;            // (X,Y) of event vertex

  HyMCStack *fStack; ///< VMC particle stack
  HyDetectorConstruction
      fDetConstruction; ///< Dector geometry and physical volumes construction

  double fMomentum = 7.;         ///< beam momentum
  int fBeamPdg = 211;            ///< pi+
  int fTargetPdg = 2112;         ///< neutron
  TLorentzVector fPbeam, fPpart; ///< 4-momenta of beam and generated particle
  int fPCNumber = 0;   ///< Process copy number(using to randomize eventgen)
  double fControl = 0; ///< parameter to control event gen(see describtion in
                       ///< hyp_carlo.f)
  int fExReson = -1;   ///< exclusive resonance  (steering parameter for event
                       ///< gen, if <0 then ignored)
  int fExChanel = -1;  ///< exclusive chanel     (steering parameter for event
                       ///< gen, if <0 then ignored)
  Pythia8::Pythia *fPythia; // pythia8

  const char *fMCResults = "MC_results.dat"; ///< filename to save MC results
  FILE *fMC_results;                         ///< file to save MC results
  const char *fHistosRoot = "histos.root";   ///< filename to save histos

  Double_t fS4EnergyDep, fSaEnergyDep, fTargetEnergyDep;
  Double_t fSaEnergyCut = 0.0023;
  double fSaEfficiency = 1.;
  Bool_t fFinishTracking;

  TTree *fSavingTree;
  HyMCEvent *fHyMCEvent;
  // bad variable names

  int EvntNumb; // current event number
  int evWr;     // number of events which passed all the discriminations
  // massiv energiy dlya kajdogo otdelnogo fotona(10 fotonov max) i tekushiy
  // nomer fotona
  Double_t *initial_photon_energy;
  Double_t **energy_dep;
  Int_t currentPrimaryIndex, max_gamma, fNPrimary;
  //
  Double_t **SmEnDep; // small glass cells energy deposition
  Double_t **LaEnDep; // large glass cells energy deposition

  ClassDef(HyMCApplication, 1) // Interface to MonteCarlo application
};

// inline functions

/// \return The singleton instance
inline HyMCApplication *HyMCApplication::Instance() {
  return (HyMCApplication *)(TVirtualMCApplication::Instance());
}

#endif // HY_MC_APPLICATION_H
