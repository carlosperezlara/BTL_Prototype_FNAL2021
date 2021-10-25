#ifndef TESTBEAM_H
#define TESTBEAM_H
#include <TString.h>
#include <TTree.h>
#include <TList.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>

#include "DRSWAVE.h"
#include "mcp.h"
#include "energy.h"
#include "timing.h"

class testbeam {
public:
  testbeam(int run);
  ~testbeam();
  void SetInputFileName(TString);
  void SetOutputFileName(TString);
  void SetDelay(int val) {fDelay=val;}
  void Process();

private:
  TGraph* DisplayWave(DRSWAVE *A, DRSWAVE *B, int showfit, int color=0);
  void CalibrateTime();
  void Init();
  void Terminate();
  void LoadWalkCorrection(TString cfgfile);
  void LoadLinearCorrection(TString cfgfile);

  int fRun;
  int fDelay;
  double fMinEneL;
  double fMinEneR;
  double fThresholds[7];
  double fWalk_P0[2][7];
  double fWalk_P1[2][7];
  double fWalk_P2[2][7];
  double fLinearX_P0[7];
  double fLinearX_P1[7];
  double fDeltaMCPLeft[7];
  double fDeltaMCPRight[7];

  TTree *fTree;
  TList *fList;
  TString fOutputFileName;

  mcp *fMCP0;
  mcp *fMCP1;
  energy *fEnergyL;
  energy *fEnergyR;
  timing *fTimingL;
  timing *fTimingR;
  timing *fTimingClipLineL;
  timing *fTimingClipLineR;

  TF1 *fQuadratic;
  
  TH1D *fEvents;
  TH1D *fTRKchi2;
  TH2D *fTRKxy0;

  TCanvas *fEvent1DisplayMCP;
  TCanvas *fEvent2DisplayMCP;
  TCanvas *fEvent4DisplayEnergy;
  TCanvas *fEvent5DisplayTiming;
  TCanvas *fEvent5DisplayTimingCL;

  TH2D *fMCPdiffpos;
  TH2D *fMCPcorrE;
  TH2D *fMCPcorr;
  TH1D *fMCPdiff;
  TH2D *fTRKxyMCP0;
  TH2D *fTRKxyMCP1;
  TH2D *fTRKxyMCPI;
  TH2D *fTRKxyEnergyL;
  TH2D *fTRKxyEnergyLS;
  TH2D *fTRKxyEnergyR;
  TH2D *fTRKxyEnergyRS;

  TH2D *fMCPta0;
  TH2D *fMCPta1;

  TH2D *fEnergyLX;
  TH2D *fEnergyRX;
  TH2D *fEnergyLR;
  TH2D *fEnergyAvgLR;

  TH1D *fAvgMPV;
  
  TH2D *fTimingNCvsCL[7];
  TH2D *fTimingNCvsCR[7];
  TH2D *fTimingLE[7];
  TH2D *fTimingRE[7];
  TH2D *fTimingLEC[7];
  TH2D *fTimingREC[7];
  TH2D *fTimingLR[7];
  TH2D *fTimingELEv[7];
  TH2D *fTimingEREv[7];
  TH2D *fTimingX[7];
  TH2D *fTimingXC[7];
  TH1D *fTiming[7];
  TH2D *fTimingX_P[7];
  TH2D *fTimingXC_P[7];
  TH1D *fTiming_P[7];

};

#endif
