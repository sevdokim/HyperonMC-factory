#ifndef HY_STACK_H
#define HY_STACK_H

#include "HyParticle.h"

#include <TVirtualMCStack.h>

#include <stack>

class HyMCStack : public TVirtualMCStack
{
  public:
    HyMCStack(Int_t size);
    HyMCStack();
    virtual ~HyMCStack();     

    // methods
    virtual void  PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	              Double_t px, Double_t py, Double_t pz, Double_t e,
  		      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		      Double_t polx, Double_t poly, Double_t polz,
		      TMCProcess mech, Int_t& ntr, Double_t weight,
		      Int_t is) ;
    virtual TParticle* PopNextTrack(Int_t& track);
    virtual TParticle* PopPrimaryForTracking(Int_t i); 
    virtual void Print(Option_t* option = "") const; 
    void Reset();   
   
    // set methods
    virtual void  SetCurrentTrack(Int_t track);                           
            void  SetObjectNumber();                           

    // get methods
    virtual Int_t  GetNtrack() const;
    virtual Int_t  GetNprimary() const;
    virtual TParticle* GetCurrentTrack() const;    
    virtual Int_t  GetCurrentTrackNumber() const;
    virtual Int_t  GetCurrentParentTrackNumber() const;
    HyParticle*  GetParticle(Int_t id) const;
    
  private:
    // data members
    std::stack<HyParticle*>  fStack;        //!< The stack of particles (transient)
    TObjArray*                 fParticles;    ///< The array of particle (persistent)
    Int_t                      fCurrentTrack; ///< The current track number
    Int_t                      fNPrimary;     ///< The number of primaries
    Int_t                      fObjectNumber; ///< The Root object number counter
    Int_t			fSize;
    
    ClassDef(HyMCStack,1) // HyMCStack
};

#endif //HY_STACK_H   
   

