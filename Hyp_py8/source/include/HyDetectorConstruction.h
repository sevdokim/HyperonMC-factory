#ifndef HY_DETECTOR_CONSTRUCTION_H
#define HY_DETECTOR_CONSTRUCTION_H

#include "HyGeoParameters.h"
#include <TObject.h>
#include <TRandom3.h>
#include <TString.h>
#include <TVirtualMC.h>
#include <cstdlib>
#include <iostream>

class HyDetectorConstruction : public TObject {
public:
  HyDetectorConstruction() = default;
  ~HyDetectorConstruction() = default;

  // methods
  Int_t GetTargetMaterial();
  void SetGeometry(HyGeoParameters newgeom) { fGeom = newgeom; }
  void ConstructMaterials();
  void ConstructVolumes();
  HyGeoParameters GetGeometry();
  bool CalcInteractProbabilities();
  void GambleVertex(Double_t *vertex);
  void SetTargetMaterial(int mat);
  void SetExTarget(int exTarget) { fExTarget = exTarget; }
  void SetDebug(int deb) { fDebug = deb; }

private:
  // data members
  HyGeoParameters fGeom;
  Int_t fImedGlass;
  Int_t fImedPlastic;
  Int_t fImedPoly;
  Int_t fImedBe;
  Int_t fImedAir;
  Int_t fImedAl;
  Int_t fImedMylar;
  Int_t fImedSn;
  Int_t fImedCu;
  Int_t fImedPb;
  Int_t fImedC;
  TString fTrgtType = "Be";
  Int_t fTargetMaterial = 2; // default
  Int_t fSettedTargetMaterial;
  Int_t fDebug = 0;

  float fProbArray[3];

  int fExTarget = 0;
  bool fIsGeometryConstructed = false;
  ClassDef(HyDetectorConstruction, 1)
};

#endif // HY_DETECTOR_CONSTRUCTION_H
