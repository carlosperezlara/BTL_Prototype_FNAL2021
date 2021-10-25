#include <iostream>
#include "testbeam.h"
#include "TString.h"

int main(int argn, char **argv) {
  if(argn!=3) return 1;
  TString argRun = argv[1];
  TString argDelay = argv[2];

  //int run = 46159;
  //int run = 44718;
  int run = argRun.Atoi();
  int delay = argDelay.Atoi();
  std::cout << "Run : " << run << std::endl;
  std::cout << "Delay : " << delay << std::endl;

  //TString ifile = Form("/Volumes/uva/testbeam_2021_may_copied/merged/Run_%d.root",run);
  //TString ifile = Form("Run_%d.root",run);
  TString ifile = Form("/Volumes/uva/testbeam_2021_apr/merged/Run_%d.root",run);
  TString ofile = Form("results/AnalysisOutput_%d_D%d.root",run,delay);

  testbeam *tb = new testbeam(run);
  tb->SetInputFileName( ifile.Data() );
  tb->SetOutputFileName( ofile.Data() );
  tb->SetDelay(delay);
  tb->Process();
  
  return 0;
}
