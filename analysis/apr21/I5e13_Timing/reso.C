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
    cout << sigmaM[i] << endl;
  }
  for(int i=0; i!=7; ++i) {
    sigmaP[i] = 0;
    fin >> sigmaP[i] >> sigmaP[i];
    sigmaP[i] *= 1e3;
    sigmaP[i] = sqrt(sigmaP[i]*sigmaP[i]-50*50);
  }
}
//==================
int reso() {
  gStyle->SetOptStat(0);
  double fRun = 44734;
  double fMinEneL, fMinEneR;
  double fDeltaMCPLeft[7];
  double fDeltaMCPRight[7];
  double fThresholds[7];
  double fDelay = 2;

  int runs[8] = {44734, 44733, 44731, 44727, 44725, 44730, 44728, 44732};
  int color[8] = {kBlack, kRed-3, kBlue-3, kCyan-3, kGreen-3, kYellow-3, kOrange-3, kMagenta-3};
  TGraph *grm34[8];
  TGraph *grp34[8];
  #include "RunConfig/Run44734.cpp"
  for(int ithr=0; ithr!=7; ++ithr) fThresholds[ithr] *= -1;
  for(int irun=0; irun!=8; ++irun) {
    loadarrays( Form("results/Run_%d_2_THR.csv",runs[irun]) );
    grm34[irun] = new TGraph(7,fThresholds,sigmaM);
    grp34[irun] = new TGraph(7,fThresholds,sigmaP);
    format(grm34[irun],grp34[irun],color[irun]);
  }
  
  TH2D *axis1 = new TH2D("axis1","#sigma_{BAR} = #sigma^{-} / 2;Threshold  [mV];Resolution  [ps]",100,0,50,100,80,160);
  TH2D *axis2 = new TH2D("axis2","#sigma^{+} = #sqrt{#sigma^{2}_{BAR} + #sigma^{2}_{MCP}} ;Threshold  [mV];Resolution  [ps]",100,0,50,100,80,160);

  TCanvas *main = new TCanvas();
  main->Divide(2,1);

  TLegend * leg = new TLegend( 0.1, 0.7, 0.9, 0.9 );
  leg->AddEntry( grm34[0], "R44734  -18.1 C" );
  leg->AddEntry( grm34[1], "R44733  -17.7 C" );
  leg->AddEntry( grm34[2], "R44731  -17.7 C" );
  leg->AddEntry( grm34[3], "R44727  -16.5 C" );
  leg->AddEntry( grm34[4], "R44725  -16.5 C" );
  leg->AddEntry( grm34[5], "R44730  -18.1 C" );
  leg->AddEntry( grm34[6], "R44728  -17.9 C" );
  leg->AddEntry( grm34[7], "R44732  -17.8 C" );
  leg->SetNColumns(2);

  main->cd(1);
  axis1->Draw();
  for(int irun=0; irun!=8; ++irun)
    grm34[irun]->Draw("PLSAME");
  leg->Draw();

  TLatex *tex = new TLatex();
  tex->SetTextColor(kGray+1);
  main->cd(2);
  axis2->Draw();
  for(int irun=0; irun!=8; ++irun)
    grp34[irun]->Draw("PLSAME");

  tex->DrawLatexNDC(0.15,0.85,"Apr'21");
  tex->DrawLatexNDC(0.15,0.80,"Irradiated. Fluence 5e13");
  tex->DrawLatexNDC(0.15,0.75,"V_{op} LEFT 39.4 V  RIGHT 39.5 V");
  tex->DrawLatexNDC(0.15,0.70,"Approx temperature -18 C");
  tex->DrawLatexNDC(0.15,0.65,"Approx #sigma_{MCP} 15 ps");
  
  return 0;
}
