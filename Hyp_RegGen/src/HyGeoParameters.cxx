#include <iostream>

#include <TVirtualMC.h>

#include "HyGeoParameters.h"

ClassImp(HyGeoParameters)

    using namespace std;

//_____________________________________________________________________________
HyGeoParameters::HyGeoParameters()
    : TObject(), S4Dist(0.0), // Center of the coordinate system
      S4Radius(6.0),
      //      S4Thickness(0.1),                    // from 2009-11
      S4Thickness(0.5), // before 2009-11

      TargetDist(0.0), // Should be redefined
      TargetRadius(6.45), TargetThickness(7.5),

      SaDist(39.95), // OK 09.07.2011
                     //	SaDist(50.45),                       // new Sa position
                     //from 1.04.2015
      SaWidth(20.), SaThickness(1.5),

      LGD2Dist(339.2), // OK 09.07.2011
      MatrixDist(41.),

      SmGlassSize(4.25), LaGlassSize(8.5), GlassLength(35.), SmNx(8.), SmNy(8.),
      LaNx(24.), LaNy(24.),

      NofMembranes(18.), MembraneSize(50.), MembraneWidth(0.035),
      MembraneBlockLength(64.), MembraneDist(36.5) {}
//_____________________________________________________________________________
HyGeoParameters::~HyGeoParameters() {}
//_____________________________________________________________________________
Double_t HyGeoParameters::GetS4Dist() { return S4Dist; }
Double_t HyGeoParameters::GetSaDist() { return SaDist; }
Double_t HyGeoParameters::GetMatrixDist() { return MatrixDist; }
Double_t HyGeoParameters::GetSmGlassSize() { return SmGlassSize; }
Double_t HyGeoParameters::GetLaGlassSize() { return LaGlassSize; }
Double_t HyGeoParameters::GetSaWidth() { return SaWidth; }
Double_t HyGeoParameters::GetS4Radius() { return S4Radius; }
Double_t HyGeoParameters::GetS4Thickness() { return S4Thickness; }
Double_t HyGeoParameters::GetSaThickness() { return SaThickness; }
Double_t HyGeoParameters::GetLGD2Dist() { return LGD2Dist; }
Double_t HyGeoParameters::GetLaNx() { return LaNx; }
Double_t HyGeoParameters::GetLaNy() { return LaNy; }
Double_t HyGeoParameters::GetSmNx() { return SmNx; }
Double_t HyGeoParameters::GetSmNy() { return SmNy; }
Double_t HyGeoParameters::GetTargetDist() { return TargetDist; }
Double_t HyGeoParameters::GetTargetRadius() { return TargetRadius; }
Double_t HyGeoParameters::GetTargetThickness() { return TargetThickness; }
Double_t HyGeoParameters::GetGlassLength() { return GlassLength; }
Double_t HyGeoParameters::GetNofMembranes() { return NofMembranes; }
Double_t HyGeoParameters::GetMembraneSize() { return MembraneSize; }
Double_t HyGeoParameters::GetMembraneWidth() { return MembraneWidth; }
Double_t HyGeoParameters::GetMembraneBlockLength() {
  return MembraneBlockLength;
}
Double_t HyGeoParameters::GetMembraneDist() { return MembraneDist; }

//_____________________________________________________________________________
void HyGeoParameters::SetS4Dist(Double_t a) { S4Dist = a; }
void HyGeoParameters::SetS4Thickness(Double_t a) { S4Thickness = a; }
void HyGeoParameters::SetSaThickness(Double_t a) { SaThickness = a; }
void HyGeoParameters::SetSaWidth(Double_t a) { SaWidth = a; }
void HyGeoParameters::SetS4Radius(Double_t a) { S4Radius = a; }
void HyGeoParameters::SetSaDist(Double_t a) { SaDist = a; }
void HyGeoParameters::SetLGD2Dist(Double_t a) { LGD2Dist = a; }
void HyGeoParameters::SetMatrixDist(Double_t a) { MatrixDist = a; }
void HyGeoParameters::SetSmGlassSize(Double_t a) { SmGlassSize = a; }
void HyGeoParameters::SetLaGlassSize(Double_t a) { LaGlassSize = a; }
void HyGeoParameters::SetLaNx(Double_t a) { LaNx = a; }
void HyGeoParameters::SetLaNy(Double_t a) { LaNy = a; }
void HyGeoParameters::SetTargetDist(Double_t a) { TargetDist = a; }
void HyGeoParameters::SetTargetRadius(Double_t a) { TargetRadius = a; }
void HyGeoParameters::SetTargetThickness(Double_t a) { TargetThickness = a; }
void HyGeoParameters::SetGlassLength(Double_t a) { GlassLength = a; }
void HyGeoParameters::SetSmNx(Double_t a) { SmNx = a; }
void HyGeoParameters::SetSmNy(Double_t a) { SmNy = a; }
void HyGeoParameters::SetNofMembranes(Double_t a) { NofMembranes = a; }
void HyGeoParameters::SetMembraneSize(Double_t a) { MembraneSize = a; }
void HyGeoParameters::SetMembraneWidth(Double_t a) { MembraneWidth = a; }
void HyGeoParameters::SetMembraneBlockLength(Double_t a) {
  MembraneBlockLength = a;
}
void HyGeoParameters::SetMembraneDist(Double_t a) { MembraneDist = a; }
//_____________________________________________________________________________
