double sigmaM[7];
double sigmaP[7];

void format(TGraph *a, TGraph *b, int color){
  a->SetLineColor( color );
  a->SetMarkerColor( color );
  b->SetLineColor( color );
  b->SetMarkerColor( color );
  a->SetMarkerStyle(20);
  b->SetMarkerStyle(20);
  a->SetMarkerSize(2.0);
  b->SetMarkerSize(2.0);
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
int reso2() {
  gStyle->SetOptStat(0);
  double fRun = 44734;
  double fMinEneL, fMinEneR;
  double fDeltaMCPLeft[7];
  double fDeltaMCPRight[7];
  double fThresholds[7];

  int delays[9] = {1,2,5,10,15,20,30,50,100};
  int color[9] = {kBlack, kGray, kRed-3, kBlue-3, kCyan-3, kGreen-3, kYellow-3, kOrange-3, kMagenta-3};
  TGraph *grm34[9];
  TGraph *grp34[9];
  for(int idel=0; idel!=9; ++idel) {
    double fDelay = delays[idel];
    #include "RunConfig/Run44734.cpp"
    loadarrays( Form("results/Run_44734_%d_THR.csv",delays[idel]) );
    for(int ithr=0; ithr!=7; ++ithr) fThresholds[ithr] *= -1;
    grm34[idel] = new TGraph(7,fThresholds,sigmaM);
    grp34[idel] = new TGraph(7,fThresholds,sigmaP);
    format(grm34[idel],grp34[idel],color[idel]);
  }

  TH2D *axis1 = new TH2D("axis1","#sigma_{BAR} = #sigma^{-} / 2;Threshold  [mV];Resolution  [ps]",100,1,500,100,80,320);
  TH2D *axis2 = new TH2D("axis2","#sigma^{+} = #sqrt{#sigma^{2}_{BAR} + #sigma^{2}_{MCP}} ;Threshold  [mV];Resolution  [ps]",100,0.1,500,100,80,410);

  TCanvas *main = new TCanvas();
  //main->Divide(2,1);

  TLegend * leg = new TLegend( 0.1, 0.28, 0.33, 0.9 );
  for(int i=0; i!=9; ++i) {
    leg->AddEntry( grm34[i], Form("Ch%d (%.1fns)",delays[i],delays[i]*0.2) );
  }
  //leg->SetNColumns(2);

  main->SetLogx(1);
  //main->cd(1);//->SetLogx(1);
  axis1->Draw();
  axis1->GetXaxis()->SetMoreLogLabels();
  for(int i=0; i!=9; ++i) {
    grm34[i]->Draw("PLSAME");
  }
  leg->Draw();

  grm34[0]->RemovePoint(0);
  grm34[0]->RemovePoint(1);

  TLatex *tex = new TLatex();
  tex->SetTextColor(kGray+1);
  //main->cd(2);//->SetLogx(1);
  //axis2->Draw();
  //axis2->GetXaxis()->SetMoreLogLabels();
  //for(int i=0; i!=9; ++i) {
  //  grp34[i]->Draw("PLSAME");
  //}
  //leg->Draw();
  tex->DrawLatexNDC(0.35,0.85,"Run 44734  Apr'21");
  tex->DrawLatexNDC(0.35,0.80,"Irradiated. Fluence 5e13");
  tex->DrawLatexNDC(0.35,0.75,"Temp approx -18 C");
  tex->DrawLatexNDC(0.35,0.70,"I_{drawn} 1 mA per SiPM");
  tex->DrawLatexNDC(0.35,0.65,"V_{op} LEFT 39.4 V  RIGHT 39.5 V");
  //tex->DrawLatexNDC(0.15,0.60,"Approx #sigma_{MCP} 15 ps");
  
  return 0;
}
