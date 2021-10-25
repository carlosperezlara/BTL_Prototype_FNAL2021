#include "testbeam.h"

#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TList.h>
#include <TString.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TLine.h>

#include "DRSWAVE.h"
#include "mcp.h"
#include "energy.h"
#include "timing.h"

//=================================
testbeam::testbeam(int run) {
  fRun = run;
  fList = NULL;
  fTree = NULL;
  fMCP0 = NULL;
  fMCP1 = NULL;
  fEvent1DisplayMCP=NULL;
  fEvent2DisplayMCP=NULL;
  fEvents = NULL;
  fTRKchi2 = NULL;
  fTRKxy0 = NULL;
  fTRKxyMCP0 = NULL;
  fTRKxyMCP1 = NULL;
  fMCPcorr = NULL;
  fMCPdiff = NULL;
  fMCPta0 = NULL;
  fMCPta1 = NULL;
  fEnergyL = NULL;
  fEnergyR = NULL;
  fTimingL = NULL;
  fTimingR = NULL;
}
//=================================
testbeam::~testbeam() {
  if(fList) delete fList;
}
//=================================
void testbeam::SetInputFileName(TString val) {
  TFile *inputfile = new TFile( val.Data() );
  fTree = (TTree*) inputfile->Get("pulse");
}
//=================================
void testbeam::SetOutputFileName(TString val) {
  fOutputFileName = val;
}
//=================================
TGraph* testbeam::DisplayWave(DRSWAVE *A, DRSWAVE *B, int showfit, int c1) {
  int color1, color2, color3;
  if(c1==0) {
    color1 = kBlue-3;
    color2 = kCyan-3;
  } else {
    color1 = kRed-3;
    color2 = kOrange-3;
  }
  TGraph *a, *b;
  double tmp;
  double aplot = 999;
  if(A!=NULL) {
    a = new TGraph(1024,A->GetX(),A->GetY());
    a->SetLineColor(color1);
    a->SetMarkerColor(color1);
    a->SetMarkerStyle(24);
    A->GetMinimum(aplot, tmp, 0, 1024, 0);
  }
  double bplot = 999;
  if(B!=NULL) {
    b = new TGraph(1024,B->GetX(),B->GetY());
    b->SetLineColor(color2);
    b->SetMarkerColor(color2);
    b->SetMarkerStyle(24);
    B->GetMinimum(bplot, tmp, 0, 1024, 0);
  }

  TLatex *tex = new TLatex();
  TLine *lin = new TLine();
  TGraph *ret = NULL;
  if(aplot>990 && bplot>990) return NULL;
  if(aplot<bplot)
    ret = a;
  else
    ret = b;
  if(ret==NULL) return NULL;
  if(c1==0) ret->Draw("AP");
  if(A!=NULL) a->Draw("PSAME");
  if(B!=NULL) b->Draw("PSAME");
  if(showfit==2) {
    if(A!=NULL) {
      fQuadratic->SetParameter(0,A->GetQFa());
      fQuadratic->SetParameter(1,A->GetQFb());
      fQuadratic->SetParameter(2,A->GetQFc());
      fQuadratic->SetRange( A->GetTime()-4, A->GetTime()+4 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
      tex->SetTextColor(kBlue-3);
      tex->DrawLatexNDC(0.65, 0.30, Form("AvgEne  %.2f",A->GetAvgEne()) );
      lin->SetLineColor(color1);
      lin->DrawLine( A->GetRefTime(),-800, A->GetRefTime(),100);
    }
    if(B!=NULL) {
      fQuadratic->SetParameter(0,B->GetQFa());
      fQuadratic->SetParameter(1,B->GetQFb());
      fQuadratic->SetParameter(2,B->GetQFc());
      fQuadratic->SetRange( B->GetTime()-4, B->GetTime()+4 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
      tex->SetTextColor(kGreen-3);
      tex->DrawLatexNDC(0.65, 0.25, Form("AvgEne  %.2f",B->GetAvgEne()) );
      lin->SetLineColor(color2);
      lin->DrawLine( B->GetRefTime(),-800, B->GetRefTime(),100);
    }
  }
  if(showfit==1) {
    if(A!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,A->GetLFa());
      fQuadratic->SetParameter(2,A->GetLFb());
      fQuadratic->SetRange( A->GetTime()-0.3, A->GetTime()+0.3 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
    if(B!=NULL) {
      fQuadratic->SetParameter(0,0);
      fQuadratic->SetParameter(1,B->GetLFa());
      fQuadratic->SetParameter(2,B->GetLFb());
      fQuadratic->SetRange( B->GetTime()-0.3, B->GetTime()+0.3 );
      fQuadratic->SetLineColor(kBlack);
      fQuadratic->DrawCopy("same");
    }
  }
  delete tex;
  delete lin;
  return ret;
}
//=================================
void testbeam::Init() {
  fList = new TList();
  fList->SetName( "output" );
  fList->SetOwner();
  fEvents = new TH1D( "fEvents","fEvents",10,-0.5,9.5 );
  fEvents->GetXaxis()->SetBinLabel(1,"raw");
  fEvents->GetXaxis()->SetBinLabel(2,"ntrks=1");
  fEvents->GetXaxis()->SetBinLabel(3,"passed both mcp");
  fEvents->GetXaxis()->SetBinLabel(4,"acceptance");
  fList->Add( fEvents );

  //--------------
  TList *listTRK = new TList();
  listTRK->SetName("trk");
  listTRK->SetOwner();
  fTRKchi2 = new TH1D( "fTRKchi2","fTRKchi2",100,0,100 );
  listTRK->Add( fTRKchi2 );
  fTRKxy0 = new TH2D( "fTRKxy0","fTRKxy0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listTRK->Add( fTRKxy0 );
  fList->Add( listTRK );
  //--------------
  TList *listMCP = new TList();
  listMCP->SetName("mcp");
  listMCP->SetOwner();
  fMCP0 = new mcp("MCP0");
  listMCP->Add( fMCP0->GetList() );
  fMCP1 = new mcp("MCP1");
  listMCP->Add( fMCP1->GetList() );
  fTRKxyMCP0 = new TH2D( "fTRKxyMCP0","fTRKxyMCP0;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP->Add( fTRKxyMCP0 );
  fTRKxyMCP1 = new TH2D( "fTRKxyMCP1","fTRKxyMCP1;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCP->Add( fTRKxyMCP1 );
  fEvent1DisplayMCP = new TCanvas("ED1_MCP");
  fEvent1DisplayMCP->Divide(5,5);
  listMCP->Add( fEvent1DisplayMCP );
  fEvent2DisplayMCP = new TCanvas("ED2_MCP");
  fEvent2DisplayMCP->Divide(5,5);
  listMCP->Add( fEvent2DisplayMCP );
  fList->Add( listMCP );
  //--------------
  TList *listMCPI = new TList();
  listMCPI->SetName("mcpI");
  listMCPI->SetOwner();
  fTRKxyMCPI = new TH2D( "fTRKxyMCPI","fTRKxyMCPI;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listMCPI->Add( fTRKxyMCPI );
  fMCPdiffpos = new TH2D( "fMCPdiffpos","fMCPdiffpos;Distance  from  center  [mm];MCP0 - MCP1  [ns]", 100,0,8, 100,-0.3,+0);
  listMCPI->Add( fMCPdiffpos );
  fMCPcorrE = new TH2D( "fMCPcorrE","fMCPcorrE;MCP0  [mV];MCP1  [mV]", 100,0,520, 100,0,400 );
  listMCPI->Add( fMCPcorrE );
  fMCPcorr = new TH2D( "fMCPcorrT","fMCPcorrT;MCP0  [ns];MCP1  [ns]", 100,96,120, 100,96,120 );
  listMCPI->Add( fMCPcorr );
  fMCPdiff = new TH1D( "fMCPdiff","fMCPdiff;MCP0 - MCP1  [ns]", 200,-0.3,+0);
  listMCPI->Add( fMCPdiff );
  fMCPta0 = new TH2D( "fMCPta0","fMCPta0;MCP0 amp  [mV];MCP0 - MCP1  [ns]",
                     100,0,520, 100,-0.3,0 );
  listMCPI->Add( fMCPta0 );
  fMCPta1 = new TH2D( "fMCPta1","fMCPta1;MCP1 amp  [mV];MCP0 - MCP1  [ns]",
                     100,0,400, 100,-0.3,0 );
  listMCPI->Add( fMCPta1 );
  fList->Add( listMCPI );
  //--------------
  TList *listEnergy = new TList();
  listEnergy->SetName("energy");
  listEnergy->SetOwner();
  fEnergyL = new energy("ENERGYL");
  listEnergy->Add( fEnergyL->GetList() );
  fEnergyR = new energy("ENERGYR");
  listEnergy->Add( fEnergyR->GetList() );
  fTRKxyEnergyL = new TH2D( "fTRKxyEnergyL","fTRKxyEnergyL;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyL );
  fTRKxyEnergyR = new TH2D( "fTRKxyEnergyR","fTRKxyEnergyR;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyR );
  fEvent4DisplayEnergy = new TCanvas("ED4_Energy");
  fEvent4DisplayEnergy->Divide(5,5);
  listEnergy->Add( fEvent4DisplayEnergy );
  fTRKxyEnergyLS = new TH2D( "fTRKxyEnergyLS","fTRKxyEnergyLS;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyLS );
  fTRKxyEnergyRS = new TH2D( "fTRKxyEnergyRS","fTRKxyEnergyRS;[mm];[mm]", 100,-10,50, 100, -10,50 );
  listEnergy->Add( fTRKxyEnergyRS );
  fEnergyLR = new TH2D( "fEnergyLR","fEnergyLR;[mV];[mV]", 100,0,800, 100,0,800 );
  listEnergy->Add( fEnergyLR );
  fEnergyLX = new TH2D( "fEnergyLX","fEnergyLX;[mm];[mV]", 100,10,20, 100,0,800 );
  listEnergy->Add( fEnergyLX );
  fEnergyRX = new TH2D( "fEnergyRX","fEnergyRX;[mm];[mV]", 100,10,20, 100,0,800 );
  listEnergy->Add( fEnergyRX );
  fEnergyAvgLR = new TH2D( "fEnergyAvgLR","fEnergyAvgLR;[mV];[mV]", 100,0,5000, 100,0,5000 );
  listEnergy->Add( fEnergyAvgLR );
  fList->Add( listEnergy );
  //--------------
  TList *listTiming = new TList();
  listTiming->SetName("timing");
  listTiming->SetOwner();
  fTimingL = new timing("TIMINGL");
  listTiming->Add( fTimingL->GetList() );
  fTimingR = new timing("TIMINGR");
  listTiming->Add( fTimingR->GetList() );
  fEvent5DisplayTiming = new TCanvas("ED5_Timing");
  fEvent5DisplayTiming->Divide(5,5);
  listTiming->Add( fEvent5DisplayTiming );
  fTimingClipLineL = new timing("TIMINGCLIPLINEL");
  listTiming->Add( fTimingClipLineL->GetList() );
  fTimingClipLineR = new timing("TIMINGCLIPLINER");
  listTiming->Add( fTimingClipLineR->GetList() );
  fEvent5DisplayTimingCL = new TCanvas("ED5_TimingClipLine");
  fEvent5DisplayTimingCL->Divide(5,5);
  listTiming->Add( fEvent5DisplayTimingCL );
  for(int thr=0; thr!=7; ++thr) {
    TList *list = new TList();
    list->SetName( Form("thr_%d",thr) );
    list->SetOwner();
    fTimingNCvsCL[thr] = new TH2D(Form("fTimingNCvsCL_%d",thr),
                                  Form("fTimingNCvsCL_%d;[ns];[ns]",thr), 100,-3,+6, 100,-3,+6 );
    list->Add( fTimingNCvsCL[thr] );
    fTimingNCvsCR[thr] = new TH2D(Form("fTimingNCvsCR_%d",thr),
                                  Form("fTimingNCvsCR_%d;[ns];[ns]",thr), 100,-3,+6, 100,-3,+6 );
    list->Add( fTimingNCvsCR[thr] );
    fTimingLE[thr] = new TH2D(Form("fTimingLE_%d",thr),
                              Form("fTimingLE_%d;[mV];[ns]",thr), 300,0,700, 100,-3,+5 );
    list->Add( fTimingLE[thr] );
    fTimingRE[thr] = new TH2D(Form("fTimingRE_%d",thr),
                              Form("fTimingRE_%d;[mV];[ns]",thr), 300,0,700, 100,-3,+5 );
    list->Add( fTimingRE[thr] );
    fTimingLEC[thr] = new TH2D(Form("fTimingLEC_%d",thr),
                               Form("fTimingLEC_%d;[mV];[ns]",thr), 300,0,700, 100,-2,+1 );
    list->Add( fTimingLEC[thr] );
    fTimingREC[thr] = new TH2D(Form("fTimingREC_%d",thr),
                               Form("fTimingREC_%d;[mV];[ns]",thr), 300,0,700, 100,-2,+1 );
    list->Add( fTimingREC[thr] );
    fTimingLR[thr] = new TH2D(Form("fTimingLR_%d",thr),
                              Form("fTimingLR_%d;LEFT  [ns];RIGHT  [ns]",thr), 100,-2,+2, 100,-2,+2 );
    list->Add( fTimingLR[thr] );
    fTimingELEv[thr] = new TH2D(Form("fTimingELEv_%d",thr),
                              Form("fTimingELEv_%d;Event  Number;AvgEne  LEFT",thr), 100,0,50000, 100,0,1000 );
    list->Add( fTimingELEv[thr] );
    fTimingEREv[thr] = new TH2D(Form("fTimingEREv_%d",thr),
                              Form("fTimingEREv_%d;Event  Number;AvgEne  RIGHT",thr), 100,0,50000, 100,0,1000 );
    list->Add( fTimingEREv[thr] );
    fTimingX[thr] = new TH2D(Form("fTimingX_%d",thr),
                             Form("fTimingX_%d;Position  [mm];LEFT - RIGHT  [ns]",thr), 100,8,24, 100,-2,+2 );
    list->Add( fTimingX[thr] );
    fTimingXC[thr] = new TH2D(Form("fTimingXC_%d",thr),
                              Form("fTimingXC_%d;Position  [mm];CORRECTED  [ns]",thr), 100,8,24, 100,-2,+2 );
    list->Add( fTimingXC[thr] );
    fTiming[thr] = new TH1D(Form("fTiming_%d",thr),
                            Form("fTiming_%d;LEFT - RIGHT  [ns]",thr), 100,-2,+2 );
    list->Add( fTiming[thr] );
    fTimingX_P[thr] = new TH2D(Form("fTimingX_P_%d",thr),
                               Form("fTimingX_P_%d;Position  [mm];LEFT + RIGHT  [ns]",thr), 100,8,24, 100,-2,+2 );
    list->Add( fTimingX_P[thr] );
    fTimingXC_P[thr] = new TH2D(Form("fTimingXC_P_%d",thr),
                                Form("fTimingXC_P_%d;Position  [mm];CORRECTED  [ns]",thr), 100,8,24, 100,-2,+2 );
    list->Add( fTimingXC_P[thr] );
    fTiming_P[thr] = new TH1D(Form("fTiming_P_%d",thr),
                              Form("fTiming_P_%d;LEFT + RIGHT  [ns]",thr), 100,-2,+2 );
    list->Add( fTiming_P[thr] );
    listTiming->Add( list );
  }
  fAvgMPV = new TH1D("fAvgMPV","fAvgMPV;[mV]",100,0,1000);
  listTiming->Add( fAvgMPV );

  fList->Add( listTiming );
  //--------------
  fQuadratic = new TF1("fQuadratic","[0]*x*x+[1]*x+[2]");
}
//=================================
void testbeam::LoadWalkCorrection(TString filecfg) {
  std::ifstream fin(filecfg.Data());
  for(int ithr=0; ithr!=7; ++ithr) {
    fin >> fWalk_P0[0][ithr] >> fWalk_P1[0][ithr] >> fWalk_P2[0][ithr];
    fin >> fWalk_P0[1][ithr] >> fWalk_P1[1][ithr] >> fWalk_P2[1][ithr];
  }
  fin.close();
}
//=================================
void testbeam::LoadLinearCorrection(TString filecfg) {
  std::ifstream fin(filecfg.Data());
  for(int ithr=0; ithr!=7; ++ithr) {
    fin >> fLinearX_P0[ithr] >> fLinearX_P1[ithr];
  }
  fin.close();
}
//=================================
void testbeam::CalibrateTime() {
  for(int i=0; i!=7; ++i) {
    fWalk_P0[0][i] = 0; fWalk_P1[0][i] = 0; fWalk_P2[0][i] = 0;
    fWalk_P0[1][i] = 0; fWalk_P1[1][i] = 0; fWalk_P2[1][i] = 0;
    fLinearX_P0[i] = 0; fLinearX_P1[i] = 0;
  }
  LoadWalkCorrection( Form("RunConfig/Run_%d_%d_WALK.csv",fRun,fDelay) );
  LoadLinearCorrection( Form("RunConfig/Run_%d_%d_LIN.csv",fRun,fDelay) );
  #include "../RunConfig/calibration.cpp"
  for(int i=0; i!=7; ++i) {
    std::cout << "Threshold " << i << " " << fThresholds[i] << std::endl;
    std::cout << " Left:  " << fWalk_P0[0][i] << " " << fWalk_P1[0][i] << " " << fWalk_P2[0][i] << std::endl;
    std::cout << " Right: " << fWalk_P0[1][i] << " " << fWalk_P1[1][i] << " " << fWalk_P2[1][i] << std::endl;
    std::cout << " XCorr: " << fLinearX_P0[i] << " " << fLinearX_P1[i] << std::endl;
  }

}
//=================================
void testbeam::Terminate() {
  //std::cout << "Terminate() was called" << std::endl;
  TFile *outputfile = new TFile( fOutputFileName.Data(), "RECREATE" );
  fList->Write( fList->GetName(), TObject::kSingleKey );
  outputfile->Close();
  std::cout << "File " << fOutputFileName.Data() << " was saved." << std::endl;

}
//=================================
void testbeam::Process() {
  if(!fTree) return;
  Init();
  //fTree->Print();
  int event;
  unsigned short tc[2]; // trigger counter bin
  float times[2][1024]; // calibrated time
  float channel[18][1024]; // calibrated input (in V)
  float xIntercept;
  float yIntercept;
  float xSlope;
  float ySlope;
  float chi2;
  int ntracks;
  
  fTree->SetBranchAddress("event", &event);
  fTree->SetBranchAddress("tc", tc);
  fTree->SetBranchAddress("channel", channel);
  fTree->SetBranchAddress("times", times);
  fTree->SetBranchAddress("xIntercept", &xIntercept);
  fTree->SetBranchAddress("yIntercept", &yIntercept);
  fTree->SetBranchAddress("xSlope", &xSlope);
  fTree->SetBranchAddress("ySlope", &ySlope);
  fTree->SetBranchAddress("chi2", &chi2);
  fTree->SetBranchAddress("ntracks", &ntracks);

  Long64_t iEvent = 0;
  Long64_t iEvent1 = 0;
  Long64_t iEvent2 = 0;
  Long64_t iEvent4 = 0;
  Long64_t iEvent5 = 0;
  Long64_t nEvents = fTree->GetEntries();
  //nEvents = 3;
  TGraph *tmpMCP0[25];
  TGraph *tmpMCP1[25];

  CalibrateTime();

  for(;iEvent!=nEvents; ++iEvent) {
    fTree->GetEntry( iEvent );
    fEvents->Fill( 0 );
    if(iEvent%2000==0) std::cout << "Events read so far: " << iEvent << std::endl;

    if(ntracks!=1) continue;
    fEvents->Fill( 1 );
    
    double xproj = xIntercept*1e-3;
    double yproj = yIntercept*1e-3;
    fTRKxy0->Fill( xproj, yproj );
    fTRKchi2->Fill( chi2 );

    fMCP0->Fill( times[0], channel[0] ); // group 0 channel 0
    fMCP1->Fill( times[0], channel[1] ); // group 0 channel 1
    fEnergyL->Fill( times[1], channel[13] );
    fEnergyR->Fill( times[1], channel[14] );
    fTimingL->Fill( times[0], channel[4]  );
    fTimingR->Fill( times[0], channel[5]  );
    fTimingClipLineL->FillClipLine( times[0], channel[4], fDelay ); // delay=100 channel delay and invert
    fTimingClipLineR->FillClipLine( times[0], channel[5], fDelay ); // which is about 20 ns

    if(iEvent1<25) {
      fEvent1DisplayMCP->cd(iEvent1+1);
      TGraph *ret = DisplayWave(fMCP0, fMCP1, 1);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent1++;

    bool passMCP0 = fMCP0->Process(-100); //100mV cut
    bool passMCP1 = fMCP1->Process(-77); //77mV cut
    fEnergyL->SetRefTime( fMCP0->GetTime() + 18 );
    fEnergyR->SetRefTime( fMCP0->GetTime() + 24 );
    bool passENEL = fEnergyL->Process();
    bool passENER = fEnergyR->Process();

    if( passMCP0 ) fTRKxyMCP0->Fill( xproj, yproj );
    if( passMCP1 ) fTRKxyMCP1->Fill( xproj, yproj );
    if( passENEL ) fTRKxyEnergyL->Fill( xproj, yproj );
    if( passENER ) fTRKxyEnergyR->Fill( xproj, yproj );

    if( !passMCP0 ) continue;
    if(-fMCP0->GetAmplitude()>420) continue;
    
    if( passENEL ) fTRKxyEnergyLS->Fill( xproj, yproj );
    if( passENER ) fTRKxyEnergyRS->Fill( xproj, yproj );
     
    fMCPcorrE->Fill( -fMCP0->GetAmplitude(), -fMCP1->GetAmplitude() );
    fMCPcorr->Fill( fMCP0->GetTime(), fMCP1->GetTime() );
    fMCPdiff->Fill( fMCP0->GetTime() - fMCP1->GetTime() );
    fMCPta0->Fill( -fMCP0->GetAmplitude(), fMCP0->GetTime() - fMCP1->GetTime() );
    fMCPta1->Fill( -fMCP1->GetAmplitude(), fMCP0->GetTime() - fMCP1->GetTime() );

    if( !passENEL ) continue;
    if( !passENER ) continue;
    if( fEnergyL->GetAvgEne()<fMinEneL ) continue;
    if( fEnergyR->GetAvgEne()<fMinEneR ) continue;

    fEvents->Fill( 4 );
    if(iEvent4<25) {
      fEvent4DisplayEnergy->cd(iEvent4+1);
      TGraph *ret = DisplayWave(fEnergyL, fEnergyR, 2);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
    }
    iEvent4++;

    fEnergyLR->Fill( -fEnergyL->GetAmplitude(), -fEnergyR->GetAmplitude() );
    fEnergyAvgLR->Fill( fEnergyL->GetAvgEne(), fEnergyR->GetAvgEne() );
    fEnergyLX->Fill( xproj, -fEnergyL->GetAmplitude() );
    fEnergyRX->Fill( xproj, -fEnergyR->GetAmplitude() );

    fTimingL->SetThreshold(-100);
    fTimingR->SetThreshold(-100);
    bool passTIML = fTimingL->Process();
    bool passTIMR = fTimingR->Process();
    fTimingClipLineL->SetBaselineLimits(800,1000);
    fTimingClipLineR->SetBaselineLimits(800,1000);
    fTimingClipLineL->SetThreshold(-100);
    fTimingClipLineR->SetThreshold(-100);
    fTimingClipLineL->Process();
    fTimingClipLineR->Process();
    fEvents->Fill( 5 );
    if(iEvent5<25) {
      fEvent5DisplayTiming->cd(iEvent5+1);
      TGraph *ret = DisplayWave(fTimingL, fTimingR, 1, 0);
      if(ret) ret->SetTitle( Form("Event  %lld",iEvent) );
      ret = DisplayWave(fTimingClipLineL, fTimingClipLineR, 1, 1);
    }
    iEvent5++;
    
    //BOOKEEP ONE
    fTimingL->RefTime( fThresholds[0], true );
    fTimingR->RefTime( fThresholds[0], true );
    fTimingClipLineL->RefTime( fThresholds[0], true );
    fTimingClipLineR->RefTime( fThresholds[0], true );
    //
    for(int thr=0; thr!=7;++thr) {
      if( !fTimingL->RefTime( fThresholds[thr], false ) ) continue;
      if( !fTimingR->RefTime( fThresholds[thr], false ) ) continue;
      if( !fTimingClipLineL->RefTime( fThresholds[thr], false ) ) continue;
      if( !fTimingClipLineR->RefTime( fThresholds[thr], false ) ) continue;
      double sipmtimeLNC = fTimingL->GetRefTime() - fMCP0->GetTime() + fDeltaMCPLeft[thr];
      double sipmtimeRNC = fTimingR->GetRefTime() - fMCP0->GetTime() + fDeltaMCPRight[thr];
      double sipmtimeL = fTimingClipLineL->GetRefTime() - fMCP0->GetTime() + fDeltaMCPLeft[thr];
      double sipmtimeR = fTimingClipLineR->GetRefTime() - fMCP0->GetTime() + fDeltaMCPRight[thr];
      
      fTimingNCvsCL[thr]->Fill( sipmtimeLNC, sipmtimeL );
      fTimingNCvsCR[thr]->Fill( sipmtimeRNC, sipmtimeR );
      
      fTimingLE[thr]->Fill( fEnergyL->GetAvgEne(), sipmtimeL );
      fTimingRE[thr]->Fill( fEnergyR->GetAvgEne(), sipmtimeR );

      double avgeneL = fEnergyL->GetAvgEne();
      double avgeneR = fEnergyR->GetAvgEne();
      sipmtimeL -= fWalk_P0[0][thr];
      sipmtimeL -= fWalk_P1[0][thr]/avgeneL + fWalk_P2[0][thr]*TMath::Power(avgeneL,9)*1e-27;
      sipmtimeR -= fWalk_P0[1][thr];
      sipmtimeR -= fWalk_P1[1][thr]/avgeneR + fWalk_P2[1][thr]*TMath::Power(avgeneR,9)*1e-27;

      if( TMath::Abs(sipmtimeL) > 2 ) continue;
      if( TMath::Abs(sipmtimeR) > 2 ) continue;

      fTimingLEC[thr]->Fill( fEnergyL->GetAvgEne(), sipmtimeL );
      fTimingREC[thr]->Fill( fEnergyR->GetAvgEne(), sipmtimeR );
      fTimingLR[thr]->Fill( sipmtimeL, sipmtimeR );
      fTimingELEv[thr]->Fill( iEvent, fEnergyL->GetAvgEne() );
      fTimingEREv[thr]->Fill( iEvent, fEnergyR->GetAvgEne() );

      double delta = sipmtimeL-sipmtimeR;
      fTimingX[thr]->Fill( xproj, delta );
      delta -=  fLinearX_P0[thr] + fLinearX_P1[thr]*xproj;
      fTimingXC[thr]->Fill( xproj, delta );
      fTiming[thr]->Fill( delta );

      double plus = 0.5*(sipmtimeL+sipmtimeR);
      fTimingX_P[thr]->Fill( xproj, plus );
      fTimingXC_P[thr]->Fill( xproj, plus );
      fTiming_P[thr]->Fill( plus );
    }
    fAvgMPV->Fill( 0.5*(fEnergyL->GetAvgEne() + fEnergyL->GetAvgEne()) );
  }
  fTRKxyMCP0->Divide( fTRKxy0 );
  fTRKxyMCP1->Divide( fTRKxy0 );
  fTRKxyMCPI->Divide( fTRKxy0 );
  Terminate();
  fTree = NULL; //don't call me again
}
