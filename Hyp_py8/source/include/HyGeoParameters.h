#ifndef HY_GEO_PARAMETERS_H
#define HY_GEO_PARAMETERS_H

#include <TObject.h>
#include <TString.h>

class HyGeoParameters : public TObject {
public:
  HyGeoParameters() = default;
  ~HyGeoParameters() = default;
  Double_t GetS4Dist() { return S4Dist; }
  Double_t GetSaDist() { return SaDist; }
  Double_t GetMatrixDist() { return MatrixDist; }
  Double_t GetSmGlassSize() { return SmGlassSize; }
  Double_t GetLaGlassSize() { return LaGlassSize; }
  Double_t GetSaWidth() { return SaWidth; }
  Double_t GetS4Radius() { return S4Radius; }
  Double_t GetS4Thickness() { return S4Thickness; }
  Double_t GetSaThickness() { return SaThickness; }
  Double_t GetLGD2Dist() { return LGD2Dist; }
  Double_t GetLaNx() { return LaNx; }
  Double_t GetLaNy() { return LaNy; }
  Double_t GetSmNx() { return SmNx; }
  Double_t GetSmNy() { return SmNy; }
  Double_t GetTargetDist() { return TargetDist; }
  Double_t GetTargetRadius() { return TargetRadius; }
  Double_t GetTargetThickness() { return TargetThickness; }
  Double_t GetGlassLength() { return GlassLength; }

  //_____________________________________________________________________________
  void SetS4Dist(Double_t a) { S4Dist = a; }
  void SetS4Thickness(Double_t a) { S4Thickness = a; }
  void SetSaThickness(Double_t a) { SaThickness = a; }
  void SetSaWidth(Double_t a) { SaWidth = a; }
  void SetS4Radius(Double_t a) { S4Radius = a; }
  void SetSaDist(Double_t a) { SaDist = a; }
  void SetLGD2Dist(Double_t a) { LGD2Dist = a; }
  void SetMatrixDist(Double_t a) { MatrixDist = a; }
  void SetSmGlassSize(Double_t a) { SmGlassSize = a; }
  void SetLaGlassSize(Double_t a) { LaGlassSize = a; }
  void SetLaNx(Double_t a) { LaNx = a; }
  void SetLaNy(Double_t a) { LaNy = a; }
  void SetTargetDist(Double_t a) { TargetDist = a; }
  void SetTargetRadius(Double_t a) { TargetRadius = a; }
  void SetTargetThickness(Double_t a) { TargetThickness = a; }
  void SetGlassLength(Double_t a) { GlassLength = a; }
  void SetSmNx(Double_t a) { SmNx = a; }
  void SetSmNy(Double_t a) { SmNy = a; }

private:
  Double_t S4Dist = 0.0;
  Double_t SaDist = 39.95; //	SaDist = 50.45 from 1.04.2015
  Double_t S4Radius = 6.0;
  Double_t S4Thickness = 0.5;
  Double_t SaThickness = 1.5;
  Double_t SaWidth = 20.;
  Double_t LGD2Dist = 339.2;
  Double_t MatrixDist = 41.;
  Double_t SmGlassSize = 4.25;
  Double_t LaGlassSize = 8.5;
  Double_t LaNx = 24.;
  Double_t LaNy = 24.;
  Double_t SmNx = 8.;
  Double_t SmNy = 8.;
  Double_t TargetDist = 0.0; // should be redifined later
  Double_t TargetRadius = 6.45;
  Double_t TargetThickness = 7.;
  Double_t GlassLength = 35.;

  ClassDef(HyGeoParameters, 1)
};

#endif // HY_GEO_PARAMETERS_H
