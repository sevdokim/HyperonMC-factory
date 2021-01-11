#ifndef HY_DETECTOR_CONSTRUCTION_H
#define HY_DETECTOR_CONSTRUCTION_H

#include <iostream>
#include <cstdlib>
#include <TRandom3.h>
#include <TVirtualMC.h>
#include <TObject.h>
#include <TString.h>
#include "HyGeoParameters.h"

class HyDetectorConstruction : public TObject
{
  public:  
    HyDetectorConstruction();
    virtual ~HyDetectorConstruction();
 
    // methods
    Int_t GetTargetMaterial();
	void SetGeometry(HyGeoParameters newgeom);
	void ConstructMaterials();
	void ConstructVolumes();
	HyGeoParameters GetGeometry();
        void CalcInteractProbabilities(float* ProbArray);
        void GambleDecayVertex(Int_t NofTarget, Double_t* vertex); //1 - S4, 2 - Target, 3 - Sa, other - returns (0,0, -450)
	void SetTargetMaterial(int mat);
	//int GetTargetMaterial(){return fTargetMaterial};

  private:
     // data members
        TRandom3* fRandGen;
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
	TString fTrgtType;
	Int_t fTargetMaterial;
	Int_t fSettedTargetMaterial;
	Int_t fDebug;
       
  ClassDef(HyDetectorConstruction,1)
};

#endif //HY_DETECTOR_CONSTRUCTION_H
