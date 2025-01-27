#ifndef HY_PARTICLE_H
#define HY_PARTICLE_H

#include <TObject.h>
#include <TRef.h>
#include <TRefArray.h>

class TParticle;

class HyParticle : public TObject {
public:
  HyParticle(Int_t id, TParticle *particle);
  HyParticle(Int_t id, TParticle *particle, HyParticle *mother);
  HyParticle();
  virtual ~HyParticle();

  // methods
  void SetMother(HyParticle *particle);
  void AddDaughter(HyParticle *particle);
  virtual void Print(Option_t *option = "") const;
  void PrintDaughters() const;

  // get methods
  Int_t GetID() const;
  TParticle *GetParticle() const;
  HyParticle *GetMother() const;
  Int_t GetNofDaughters() const;
  HyParticle *GetDaughter(Int_t i) const;

private:
  // data members
  Int_t fID;            ///< The particle Id
  TParticle *fParticle; ///< The particle definition
  TRef fMother;         ///< The particle mother
  TRefArray fDaughters; ///< The particle daughters

  ClassDef(HyParticle, 1) // Extended TParticle
};

#endif // HY_PARTICLE_H
