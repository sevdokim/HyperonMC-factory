#ifndef EX01_MC_APPLICATION_H
#define EX01_MC_APPLICATION_H

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file Ex01MCApplication.h
/// \brief Definition of the Ex01MCApplication class
///
/// Geant4 ExampleN01 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay

#include "Ex01MCStack.h"
#include "Pythia8/Pythia.h"
#include <TLorentzVector.h>
#include <TVirtualMCApplication.h>

class TVirtualMagField;
class TH1F;
class TFile;

/// \ingroup E01
/// \brief Implementation of the TVirtualMCApplication
///
/// \date 05/04/2002
/// \author I. Hrivnacova; IPN, Orsay

class Ex01MCApplication : public TVirtualMCApplication {
public:
  Ex01MCApplication(const char *name, const char *title);
  Ex01MCApplication();
  virtual ~Ex01MCApplication();

  // static access method
  static Ex01MCApplication *Instance();

  // methods
  void InitMC(const char *setup);
  void RunMC(Int_t nofEvents);
  void FinishRun();
  void SetTargetMaterial(int mat) { fImedTarget = mat; }
  void SetTargetRadius(double rad) { fTargetRadius = rad; }
  void SetTargetThickness(double thic) { fTargetThickness = thic; }
  void SetInitialEnergy(double initEn) { fInitialEnergy = initEn; }

  virtual TVirtualMCApplication *CloneForWorker() const;
  virtual void InitOnWorker();
  virtual void ConstructGeometry();
  virtual void InitGeometry();
  virtual void GeneratePrimaries();
  virtual void BeginEvent();
  virtual void BeginPrimary();
  virtual void PreTrack();
  virtual void Stepping();
  virtual void PostTrack();
  virtual void FinishPrimary();
  virtual void FinishEvent();

  // methods for tests
  void SetOldGeometry(Bool_t oldGeometry = kTRUE);
  void TestVMCGeometryGetters();

private:
  // methods
  void ConstructMaterials();
  void ConstructVolumes();

  // data members
  Ex01MCStack *fStack;         ///< The VMC stack
  TVirtualMagField *fMagField; ///< The magnetic field
  Int_t fImedAr;               ///< The Argon gas medium Id
  Int_t fImedAir;              ///< The air medium Id
  Int_t fImedAl;               ///< The Aluminium medium Id
  Int_t fImedPb;               ///< The Lead medium Id
  int fImedBe;
  int fImedC;
  int fImedCu;
  int fImedSn;
  int fImedCH2;
  int fImedTarget;
  int fImedPlastic;
  int fImedSci;
  int fImedBGO;
  int fImedNaI;
  double fTargetRadius;
  double fTargetThickness;
  double fEnDepTarget;
  double fEnDepOutTarget;
  double fEnergyWhenExitingTarget;
  double fInitialEnergy;
  double fEnergyOfSecondariesInTarget;
  double fDeltaTheta;
  double fEnDepInBGO;
  double fEnDepInNaI;
  double fEnDepInSF;
  double fEnDepInCPV;
  double fCPVThreshold;
  double fSFThreashold;
  TLorentzVector fInitialMomentum;
  Bool_t fOldGeometry; ///< Option for geometry definition
  TFile *fFileSave;
  TH1F *hEnDepInTarget;
  TH1F *hEnDepInAir;
  TH1F *hEnergyWhenExitingTarget;
  TH1F *hEnergyOfSecondariesInTarget;
  TH1F *hDeltaTheta;
  TH1F *hEnDepInCPV;
  TH1F *hEnDepInNaI;
  TH1F *hEnDepInBGO;
  TH1F *hEnDepInSF;
  TH1F *hEnDepInNaITriggered;
  TH1F *hEnDepInBGOTriggered;

  Pythia8::Pythia *fPythia;

  ClassDef(Ex01MCApplication, 1) // Interface to MonteCarlo application
};

// inline functions

inline Ex01MCApplication *Ex01MCApplication::Instance() {
  /// \return The MC application instance
  return (Ex01MCApplication *)(TVirtualMCApplication::Instance());
}

inline void Ex01MCApplication::SetOldGeometry(Bool_t oldGeometry) {
  /// Select old geometry definition (via TVirtualMC)
  fOldGeometry = oldGeometry;
}

#endif // EX01_MC_APPLICATION_H