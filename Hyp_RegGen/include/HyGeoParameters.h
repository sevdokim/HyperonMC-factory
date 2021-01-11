#ifndef HY_GEO_PARAMETERS_H
#define HY_GEO_PARAMETERS_H



#include <TObject.h>
#include <TString.h>

class HyGeoParameters : public TObject
{
	public:
		HyGeoParameters();
		virtual ~HyGeoParameters();
		Double_t GetS4Dist();
		Double_t GetSaWidth();
		Double_t GetS4Radius();
		Double_t GetS4Thickness();
		Double_t GetSaThickness();
		Double_t GetSaDist();
		Double_t GetLGD2Dist();
		Double_t GetMatrixDist();
		Double_t GetSmGlassSize();
		Double_t GetLaGlassSize();
		Double_t GetLaNx();
		Double_t GetLaNy();
		Double_t GetSmNx();
		Double_t GetSmNy();
		Double_t GetTargetDist();
		Double_t GetTargetRadius();
		Double_t GetTargetThickness();
		Double_t GetGlassLength();
		Double_t GetNofMembranes();
		Double_t GetMembraneSize();
		Double_t GetMembraneWidth();
		Double_t GetMembraneBlockLength();
		Double_t GetMembraneDist();
		void SetS4Dist(Double_t a);
		void SetS4Thickness(Double_t a);
		void SetSaThickness(Double_t a);
		void SetSaWidth(Double_t a);
		void SetS4Radius(Double_t a);
		void SetSaDist(Double_t a);
		void SetLGD2Dist(Double_t a);
		void SetMatrixDist(Double_t a);
		void SetSmGlassSize(Double_t a);
		void SetLaGlassSize(Double_t a);
		void SetLaNx(Double_t a);
		void SetLaNy(Double_t a);
		void SetTargetDist(Double_t a);
		void SetTargetRadius(Double_t a);
		void SetTargetThickness(Double_t a);
		void SetGlassLength(Double_t a);
		void SetSmNx(Double_t a);
		void SetSmNy(Double_t a);
		void SetNofMembranes(Double_t a);
		void SetMembraneSize(Double_t a);
		void SetMembraneWidth(Double_t a);
		void SetMembraneBlockLength(Double_t a);
		void SetMembraneDist(Double_t a);
		
	private:
		Double_t S4Dist; 
		Double_t SaDist;
		Double_t S4Radius;
		Double_t S4Thickness;
		Double_t SaThickness;
		Double_t SaWidth;
		Double_t LGD2Dist;
		Double_t MatrixDist;
		Double_t SmGlassSize;
		Double_t LaGlassSize;
		Double_t LaNx;
		Double_t LaNy;
		Double_t SmNx;
		Double_t SmNy;
		Double_t TargetDist;
		Double_t TargetRadius;
		Double_t TargetThickness;
		Double_t GlassLength;
		Double_t NofMembranes;
		Double_t MembraneSize;
		Double_t MembraneWidth;
		Double_t MembraneBlockLength;
		Double_t MembraneDist;
		
	ClassDef(HyGeoParameters,1)
		
};

#endif //HY_GEO_PARAMETERS_H
