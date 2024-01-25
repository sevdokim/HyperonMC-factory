//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/run_g3.C
/// \brief Macro for loading libraries to run Example01 with Geant3.
#include "./macro/basiclibs.C"
#include "macro/g3libs.C"

void load_g3() {
  // Load basic libraries
  // gROOT->LoadMacro("./macro/basiclibs.C");
  basiclibs();

  // Load Geant3 libraries
  // gROOT->LoadMacro("./macro/g3libs.C");
  g3libs();

  // Load this example library
  // gSystem->Load("libHyperonMC_RegGen");
  // gSystem->Load("libpacklib");
  // gSystem->Load("libkernlib");
  // gSystem->Load("libmathlib");
  gSystem->Load("libpythia8");
  gSystem->Load("libvmc_Hyp_py8");

  // Load Geant3 + VMC libraries
  // g3libs();
}
