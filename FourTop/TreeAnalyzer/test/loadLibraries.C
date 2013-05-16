{

  gSystem->Load("libYumicevaTop7TeV.so");
  gSystem->Load("libYumicevaTreeAnalyzer.so");
  gSystem->Load("libCondFormatsJetMETObjects.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  gSystem->SetIncludePath("-I$CMSSW_BASE/src/");
  // -I$CMSSW_RELEASE_BASE/src/");

}
