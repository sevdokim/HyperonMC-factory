DrawHyperonGeometry() {
  gGeoManager->SetVisOption(0);
  gGeoManager->SetVisLevel(7);
  gGeoManager->FindVolumeFast("LGD2")->SetTransparency(80);
  gGeoManager->FindVolumeFast("LGD2IN")->SetTransparency(99);
  gGeoManager->FindVolumeFast("GLASLEFT_DIVIDED")->SetTransparency(100);
  gGeoManager->FindVolumeFast("GLASLEFT_XDIV")->SetTransparency(100);
  gGeoManager->FindVolumeFast("GLASLEFT_DIVIDED")->SetTransparency(100);
  gGeoManager->FindVolumeFast("SM_SENS_GLAS")->SetLineColor(kRed);
  gGeoManager->FindVolumeFast("LA_SENS_GLAS")->SetLineColor(kBlue);
  // gGeoManager->FindVolumeFast("GLACENTR_DIVIDED")->SetLineColor(kCyan);
  gGeoManager->FindVolumeFast("LA_SENS_GLAS")->SetTransparency(1);
  gGeoManager->FindVolumeFast("SM_SENS_GLAS")->SetTransparency(1);
  gGeoManager->GetTopVolume()->Draw("ogl");
}
