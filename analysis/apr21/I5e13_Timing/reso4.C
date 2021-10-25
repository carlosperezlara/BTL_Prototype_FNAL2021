double sigmaM[7];
double sigmaP[7];

void format(TGraph *a, TGraph *b, int color){
  a->SetLineColor( color );
  a->SetMarkerColor( color );
  b->SetLineColor( color );
  b->SetMarkerColor( color );
  a->SetMarkerStyle(20);
  b->SetMarkerStyle(20);
}
//==================
void loadarrays(TString file) {
  ifstream fin( file.Data() );
  for(int i=0; i!=7; ++i) {
    sigmaM[i] = 0;
    fin >> sigmaM[i] >> sigmaM[i];
    sigmaM[i] *= 1e3;
    sigmaM[i] /= 2;
    //cout << sigmaM[i] << endl;
  }
  for(int i=0; i!=7; ++i) {
    sigmaP[i] = 0;
    fin >> sigmaP[i] >> sigmaP[i];
    sigmaP[i] *= 1e3;
    sigmaP[i] = sqrt(sigmaP[i]*sigmaP[i]-50*50);
  }
}
//==================
int reso4() {
  gStyle->SetOptStat(0);
  double fRun = 44734;
  double fMinEneL, fMinEneR;
  double fDeltaMCPLeft[7];
  double fDeltaMCPRight[7];
  double fThresholds[7];
  double fDelay = 2;

  const int nruns = 10;
  int runs[nruns] = {44612, 44615, 44619, 44620, 44621, 44622, 44623, 44624, 44626, 44628};
  float volts1[nruns] = {39.60, 39.60, 39.40, 40.30, 40.10, 40.00, 39.50, 39.10, 38.90, 39.10};
  float volts2[nruns] = {39.60, 39.60, 39.40, 20.40, 40.20, 40.10, 39.60, 39.20, 38.80, 39.00};
  float amp1[nruns] = {1.27, 1.27, 1.00, 2.80, 2.30, 2.09, 1.15, 0.65, 0.50, 0.70};
  float amp2[nruns] = {1.10, 1.10, 0.85, 2.75, 2.30, 2.10, 1.13, 0.66, 0.35, 0.50};
  
  float temps[nruns] = {};
  int color[8] = {kBlack, kRed-3, kBlue-3, kCyan-3, kGreen-3, kYellow-3, kOrange-3, kMagenta-3};
  TGraph *grm34[nruns];
  TGraph *grp34[nruns];
  #include "RunConfig/Run44734.cpp"
  for(int ithr=0; ithr!=7; ++ithr) fThresholds[ithr] *= -1;
  for(int irun=0; irun!=nruns; ++irun) {
    loadarrays( Form("results/Run_%d_2_THR.csv",runs[irun]) );
    grm34[irun] = new TGraph(7,fThresholds,sigmaM);
    grp34[irun] = new TGraph(7,fThresholds,sigmaP);
    format(grm34[irun],grp34[irun],color[irun%8]);
  }
  
  TH2D *axis1 = new TH2D("axis1","#sigma_{BAR} = #sigma^{-} / 2;Threshold  [mV];Resolution  [ps]",100,0,50,100,0,300);
  TH2D *axis2 = new TH2D("axis2","#sigma^{+} = #sqrt{#sigma^{2}_{BAR} + #sigma^{2}_{MCP}} ;Threshold  [mV];Resolution  [ps]",100,0,50,100,0,300);

  TCanvas *main = new TCanvas();
  main->Divide(2,1);

  TLegend * leg = new TLegend( 0.1, 0.7, 0.9, 0.9 );
  for(int irun=0; irun!=nruns; ++irun) {
    leg->AddEntry( grm34[irun], Form("R%d %.1f C %0.1f mA", runs[irun],
                                     0.5*(volts1[irun] + volts2[irun]),
                                     0.5*(amp1[irun] + amp2[irun])) );
    cout << Form("R%d %.1f C %0.1f mA", runs[irun],
                 0.5*(volts1[irun] + volts2[irun]),
                 0.5*(amp1[irun] + amp2[irun])) << endl;
  }
  leg->SetNColumns(1);

  main->cd(1);
  axis1->Draw();
  for(int irun=0; irun!=nruns; ++irun)
    grm34[irun]->Draw("PLSAME");
  leg->Draw();

  TLatex *tex = new TLatex();
  tex->SetTextColor(kGray+1);
  main->cd(2);
  axis2->Draw();
  for(int irun=0; irun!=nruns; ++irun)
    grp34[irun]->Draw("PLSAME");

  tex->DrawLatexNDC(0.15,0.85,"Apr'21");
  tex->DrawLatexNDC(0.15,0.80,"Irradiated. Fluence 5e13");
  tex->DrawLatexNDC(0.15,0.75,"Approx #sigma_{MCP} 15 ps");
  
  return 0;
}
