// $Id: g4tgeoConfig.C 341 2008-05-26 11:04:57Z ivana $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \ingroup E01
/// \file E01/g4tgeoConfig.C
/// \brief Configuration macro for Geant4 VMC for Example01
///
/// For geometry defined with Root and selected G4Root navigation

void Config()
{


/// The configuration function for Geant4 VMC for Example01
/// called during MC application initialization. 
/// For geometry defined with Root and selected RG4Root navigation

  // RunConfiguration for Geant4
  TG4RunConfiguration* runConfiguration 
    = new TG4RunConfiguration("geomRoot", "emStandard", "specialCuts");
  
  // TGeant4
  TGeant4* geant4
    = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  cout << "Geant4 has been created." << endl;
  geant4->ProcessGeantMacro("physics.in");
  
}
