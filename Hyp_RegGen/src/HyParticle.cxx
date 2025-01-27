#include "HyParticle.h"

#include <TObjArray.h>
#include <TParticle.h>

#include <iostream>

using namespace std;

/// \cond CLASSIMP
ClassImp(HyParticle)
    /// \endcond

    //_____________________________________________________________________________
    HyParticle::HyParticle(Int_t id, TParticle *particle)
    : fID(id), fParticle(particle), fMother(), fDaughters() {
  /// Standard constructor
  /// \param  id        The particle id
  /// \param  particle  The particle definition (TParticle)
}

//_____________________________________________________________________________
HyParticle::HyParticle(Int_t id, TParticle *particle, HyParticle *mother)
    : fID(id), fParticle(particle), fMother(mother), fDaughters() {
  /// Standard constructor
  /// \param  id        The particle id
  /// \param  particle  The particle definition (TParticle)
  /// \param  mother    The particle mother
}

//_____________________________________________________________________________
HyParticle::HyParticle() : fID(0), fParticle(0), fMother(), fDaughters() {
  /// Default constructor
}

//_____________________________________________________________________________
HyParticle::~HyParticle() {
  /// Destructor

  delete fParticle;
}

//
// public methods
//

//_____________________________________________________________________________
void HyParticle::SetMother(HyParticle *particle) {
  /// Set particle mother
  /// \param  particle  The mother particle

  fMother.SetObject(particle);
}

//_____________________________________________________________________________
void HyParticle::AddDaughter(HyParticle *particle) {
  /// Add particles daughter
  /// \param particle  The daughter particle

  fDaughters.Add(particle);
}

//_____________________________________________________________________________
void HyParticle::Print(Option_t * /*option*/) const {
  /// Print particle properties.

  cout << "Track ID:  " << fID << endl;

  fParticle->Print();

  if (GetMother()) {
    cout << "Mother:    " << GetMother()->GetParticle()->GetName()
         << "  with ID: " << GetMother()->GetID() << endl;
  } else
    cout << "Primary    " << endl;

  cout << "Number of daughters: " << GetNofDaughters() << endl;
  cout << endl;
}

//_____________________________________________________________________________
void HyParticle::PrintDaughters() const {
  /// Print particles daughters.

  for (Int_t i = 0; i < GetNofDaughters(); i++) {
    cout << i << "th daughter: " << endl;
    if (GetDaughter(i))
      GetDaughter(i)->Print();
    else
      cout << "0x0";
  }
  cout << endl;
}

//_____________________________________________________________________________
Int_t HyParticle::GetID() const {
  /// \return The particle Id.

  return fID;
}

//_____________________________________________________________________________
TParticle *HyParticle::GetParticle() const {
  /// \return The particle definition (TParticle).

  return fParticle;
}

//_____________________________________________________________________________
HyParticle *HyParticle::GetMother() const {
  /// \return The particle mother.

  return (HyParticle *)fMother.GetObject();
}

//_____________________________________________________________________________
Int_t HyParticle::GetNofDaughters() const {
  /// \return The number of daughters.

  return fDaughters.GetEntriesFast();
}

//_____________________________________________________________________________
HyParticle *HyParticle::GetDaughter(Int_t i) const {
  /// \return   \em i -th daughter
  /// \param i  The daughter index

  if (i < 0 || i >= GetNofDaughters())
    Fatal("GetDaughter", "Index out of range");

  return (HyParticle *)fDaughters.At(i);
}
