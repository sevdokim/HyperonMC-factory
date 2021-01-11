#ifndef HY_MC_APPLICATION_H
#define HY_MC_APPLICATION_H

#include <TROOT.h>

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>
#include <TGeoManager.h>
#include <TVirtualGeoTrack.h>
#include <TParticle.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>

#include <TInterpreter.h>
#include <TObject.h>
#include <TObjectTable.h>
#include <TString.h>
#include <Riostream.h>
#include <TFile.h>
#include <TMath.h>

#include "HyDetectorConstruction.h"
#include "HyMCStack.h"
#include "HyGeoParameters.h"

#include <TH1.h>
#include <TH2F.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TF1.h>
#include <TPDGCode.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <iostream>
#include <cstdlib>

#define _DONT_USE_NETWORK_FUNCTIONS_
// kondr; switches off all the fancy messaging staff

extern"C" {
  void reggen_event_(float *BeamMomentum,int *Ievent, int *Ierror,int *fortran_seed, int *Jrec, int *IstOut, double *Pout);
//  void reggen_event_(float *BeamMomentum, int *i1, int *i2, int *Ievent, int *Ierror,int *fortran_seed, int *Jrec, int *IstOut, double *Pout);
  //void decay_missingmass_(int *iResonanceType, float *rProtonsToNucleons, double* Pmismas, int* NPDGcodes, int* MPDGcode, double* Pproducts);
}

int SendTestMC();

class HyMCApplication : public TVirtualMCApplication
{
  public:
    HyMCApplication(const char* name,  const char *title/*,
                      FileMode fileMode = kWrite*/);
    HyMCApplication(const char* name,  const char *title, int seed/*,
                      FileMode fileMode = kWrite*/);    //kondr, v1.04
    HyMCApplication();
    virtual ~HyMCApplication();
  
    // static access method
    static HyMCApplication* Instance(); 

    // methods
    void PreInit();
    void InitMC(const char *setup);  //build physics, process macros
    void RunMC(Int_t nofEvents);     //run MC with setted number of events
    void FinishRun();                //user action when RunMC finished 
    void DrawLGD2EnergyHisto();  //rudiments
    void DrawSaEnergyHisto();    //rudiments
    void DrawS4EnergyHisto();    //rudiments
    void DrawEffMassHisto();     //rudiments
    void SetMomentum(Double_t NewMomentum);  //set beam momentum
    void SetMCResults(char* filename);       //set file name to write MC data
    void SetHistosRoot(char* filename);      //set file name to write root histos
    void SetPCNumber(int pcnumber);          //set process number (if necessary)
    void SetControl(int control);            //set parameter to control event gen
    void SetExTarget(int hh);  
    void SetExReson (int hh);  
    void SetExChanel(int hh);  
    void SetTargetMaterial(int MediumID);    //set target material, if not setted or setted to nonexisting - will ask user
    void SetMCgendat(char* filename);
    
    #ifndef _DONT_USE_NETWORK_FUNCTIONS_
      int GetSeed();
      void SendMessage(std::string message);
      void SendAlive();
      static void* HelperFunc(void * object);
      static void* HelperFunc2(void * object);
      void MessageCycle(bool sime_global);
      void MessageCycle2(bool sime_global);
      void ThreadCycle();
      void ThreadCycle2();
      void ThreadCycle(bool sime_global);
      void ThreadCycle2(bool sime_global);
      void SetPort(int _port);
      void SetServer(std::string _server);
      void SetConnection(std::string _server, int _port);
      void SetConnection();
      std::string make_head(std::string head);
      std::string make_block(std::string block);
      std::string append_block(std::string message, std::string block);
      std::string make_block(int block);
      std::string make_client_id(int int_client_id);
      std::string make_message(int int_client_id, std::string head, std::string message);
      std::string make_message(int int_client_id, std::string head, int message);
      std::string get_client_id(std::string message);
      int get_int_client_id(std::string message);
      std::string get_head(std::string message);
      std::string get_block(std::string message, int block_number);
      std::string get_chat_message(std::string message);
    #endif
 
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
    
    void   ReadEvent(Int_t i);  /// Not used in current version

  private:
    // methods

    // data members
    Int_t fDebug;
    
    Int_t fLaNx;   ///< geomety formats of LGD2
    Int_t fLaNy;   ///< geomety formats of LGD2
    Int_t fSmNx;   ///< geomety formats of LGD2
    Int_t fSmNy;   ///< geomety formats of LGD2
    TFile *fFile;  ///< file to save root histos
    TH1F* fLGD2EnergyHisto; ///< histos
    TH1F* fSaEnergyHisto;   ///< histos
    TH1F* fS4EnergyHisto;   ///< histos
    TH1F* fEffMassHisto;    ///< histos
    TH1F* fTargetEnergyHisto;       ///< histos
    TH1F* fPhotonVertexZPosition;   ///< histos
    TH1F* fPtHisto; // Pt in event  ///< histos
    HyMCStack* fStack;   ///< VMC particle stack
    HyDetectorConstruction fDetConstruction; ///< Dector geometry and physical volumes construction
    double fMomentum;  ///< beam momentum
    char* fMCResults;  ///< filename to save MC results
    FILE* fMC_results; ///< file to save MC results
    char* fHistosRoot; ///< filename to save histos
    FILE* fMCgen_dat; ///< file to save MC T spectrum
    char* fMCgendat; ///< filename to save T spectrum
    TLorentzVector fPbeam,fPpart;///< 4-momenta of beam and generated particle


    int   fPCNumber;   ///< Process copy number(using to randomize eventgen)
    int   fControl;    ///< parameter to control event gen(see describtion in hyp_carlo.f)
    int   fExReson;    ///< exclusive resonance  (steering parameter for event gen, if <0 then ignored)
    int   fExChanel;   ///< exclusive chanel     (steering parameter for event gen, if <0 then ignored)
    int   fExTarget;   ///< exclusive target     (steering parameter for event gen, if <0 then ignored)
    Double_t fS4EnergyDep, fSaEnergyDep, fTargetEnergyDep;
    Double_t fSaEnergyCut;
    Bool_t fFinishTracking;

    //bad variable names

    int iseed; //seed from the outside, to be used in random generators
    int EvntNumb;// current event number
    int evWr;//number of events which passed all the discriminations
    //massiv energiy dlya kajdogo otdelnogo fotona(10 fotonov max) i tekushiy nomer fotona
    Double_t* initial_photon_energy;
    Double_t** energy_dep;
    Int_t current_primary_number, max_gamma, fNPrimary;
    //
    TRandom3* gen3;        //random number generator
    Double_t** SmEnDep;    //small glass cells energy deposition
    Double_t** LaEnDep;    //large glass cells energy deposition
    float TargProb[10];    //array of interaction probabilities 
    ClassDef(HyMCApplication,1)  //Interface to MonteCarlo application
    
    #ifndef _DONT_USE_NETWORK_FUNCTIONS_
      std::string g_server;
      int g_port;
    #endif


};

// inline functions

/// \return The singleton instance 
inline HyMCApplication* HyMCApplication::Instance()
{ return (HyMCApplication*)(TVirtualMCApplication::Instance()); }

/// Switch on/off the old geometry definition  (via VMC functions)
/// \param oldGeometry  If true, geometry definition via VMC functions
//inline void HyMCApplication::SetOldGeometry(Bool_t oldGeometry)
//{ fOldGeometry = oldGeometry; }

#endif //HY_MC_APPLICATION_H

