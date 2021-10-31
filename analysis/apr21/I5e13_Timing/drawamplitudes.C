TF1 *gainenf;

void LoadGain() {
  TFile *rfile = new TFile("sipm_spec_input_HDR2-015-v2-1e13.root");
  rfile->ls();
  TF1 *gai = (TF1*) rfile->Get("fGain_vs_OV");
  cout << gai->GetFormula()->GetExpFormula() << endl;
  double p0G = gai->GetParameter(0);
  double p1G = gai->GetParameter(1);
  TF1 *enf = (TF1*) rfile->Get("fENF_vs_OV");
  double p0E = enf->GetParameter(0);
  double p1E = enf->GetParameter(1);
  gainenf = new TF1("gainenf",Form("(%f+x*%f)*(1+%f*x+%f*x*x)",p0G,p1G,p0E,p1E),0,10);
}

//==================
int drawamplitudes() {
  gStyle->SetOptStat(0);
  LoadGain();
  TFile *dcrfile = new TFile("DCR_HDR2-5e13.root");
  TCanvas *dcrcanvas = (TCanvas*) dcrfile->Get("c1_n2");
  dcrcanvas->Draw();
  
  const int nruns = 12;
  int runs[nruns] = {44688, 44694, 44695, 44697, 44702, 44704, 44705, 44706, 44707, 44708, 44711, 44713};
  float volts1[nruns] = {40.00, 38.50, 39.00, 39.40, 39.40, 39.40, 39.40, 39.60, 39.60, 40.10, 40.30, 40.50};
  float volts2[nruns] = {40.00, 38.60, 39.10, 39.50, 39.50, 39.50, 39.50, 39.70, 39.70, 40.20, 40.40, 40.60};
  float amp1[nruns] = { 1.95, 0.26, 0.55, 0.97, 1.00, 1.00, 1.00, 1.27, 1.27, 2.27, 2.82, 3.00};
  float amp2[nruns] = { 1.75, 0.26, 0.56, 0.97, 1.00, 1.00, 1.00, 1.29, 1.29, 2.30, 2.80, 3.00};

  float temp1[nruns] = { -32.0, -28.0, -32.0, -22.0, -20.0, -18.0, -17.0, -18.0, -17.0, -17.0, -17.0, -16.0 };
  float temp2[nruns] = { -28.0, -23.0, -29.0, -28.0, -22.0, -29.0, -19.0, -18.0, -18.0, -16.0, -14.0, -13.0};
  
  double tem[nruns];
  double mpv[nruns];
  double vol[nruns];
  double amp[nruns];
  double vov[nruns];
  double vov_corr[nruns];
  double tem_corr[nruns];
  double dcr[nruns];
  
  int color[8] = {kBlack, kRed-3, kBlue-3, kCyan-3, kGreen-3, kYellow-3, kOrange-3, kMagenta-3};

  ifstream fin;
  for(int irun=0; irun!=nruns; ++irun) {
    fin.open( Form("results/Run_%d_2_mpv.csv",runs[irun]) );
    fin >> mpv[irun] >> mpv[irun];
    fin.close();
    tem[irun] = 0.5*(temp1[irun]+temp2[irun]);
    vol[irun] = 0.5*(volts1[irun]+volts2[irun]);
    amp[irun] = 0.5*(amp1[irun]+amp2[irun]);
    vov[irun] = vol[irun];
    vov[irun] -= amp[irun]*141*1e-3;
    vov[irun] -= ( 38.55 + 0.0378*tem[irun] );

    vov_corr[irun] = 3./220.*mpv[irun];
    tem_corr[irun] = tem[irun] + (vov[irun]-vov_corr[irun])/0.0378;
    dcr[irun] = amp[irun] * 1e-3 / (1.6e-19) / gainenf->Eval( vov_corr[irun] ) * 1e-9;
  }

  TGraph *grTEM = new TGraph( nruns, tem, mpv );
  TGraph *grAMP = new TGraph( nruns, amp, mpv );
  TGraph *grVOL = new TGraph( nruns, vol, mpv );
  TGraph *grVOV = new TGraph( nruns, vov, mpv );
  TGraph *grDCR = new TGraph( nruns, vov, amp );
  TGraph *grVOVcorr = new TGraph( nruns, vov_corr, mpv );
  TGraph *grTEMcorr = new TGraph( nruns, tem_corr, mpv );
  TGraph *grDCRcorr = new TGraph( nruns, vov_corr, amp );
  TGraph *grDCRfin = new TGraph( nruns, vov_corr, dcr );

  TH2D *agrVOV = new TH2D("agrVOV",";VoV;MPV",100,-1,3.5,100,0,300);
  TH2D *agrTEM = new TH2D("agrTEM",";Temperature;MPV",100,-35,30,100,0,300);
  TH2D *agrDCR = new TH2D("agrDCR",";VoV;Current (mA)",100,0,3,100,0,4);
  TLine *lin = new TLine();

  new TCanvas();
  agrTEM->Draw();
  grTEMcorr->SetMarkerStyle(24);
  grTEMcorr->SetMarkerColor(kRed-3);
  grTEM->Draw("SAME*");
  grTEMcorr->Draw("SAMEP");

  new TCanvas();
  grAMP->Draw("A*");

  new TCanvas();
  grVOL->Draw("A*");

  new TCanvas();
  agrVOV->Draw();
  grVOV->Draw("SAME*");
  lin->DrawLine(0,0,3,220);
  grVOVcorr->SetMarkerStyle(24);
  grVOVcorr->SetMarkerColor(kRed-3);
  grVOVcorr->Draw("SAMEP");

  new TCanvas();
  agrDCR->Draw();
  grDCR->Draw("SAME*");
  grDCRcorr->SetMarkerStyle(24);
  grDCRcorr->SetMarkerColor(kRed-3);
  grDCRcorr->Draw("SAMEP");

  
  TLatex *tex = new TLatex();
  tex->SetTextSize(0.03);
  tex->SetTextColor(kRed-3);

  grTEM->GetXaxis()->SetTitle("Temperature");
  grTEM->GetYaxis()->SetTitle("MPV");
  for(int irun=0; irun!=nruns; ++irun) {
    tex->DrawLatex( vov_corr[irun], amp[irun], Form("%.1f",tem_corr[irun]) );
  }
  
  grAMP->GetXaxis()->SetTitle("Current");
  grAMP->GetYaxis()->SetTitle("MPV");

  grVOL->GetXaxis()->SetTitle("Volts");
  grVOL->GetYaxis()->SetTitle("MPV");

  dcrcanvas->cd();
  grDCRfin->Draw("PSAME");
  grDCRfin->SetMarkerStyle(20);
  grDCRfin->SetMarkerSize(2.5);
  grDCRfin->SetMarkerColorAlpha(kBlack,0.5);
  tex->SetTextColor(kBlack);
  tex->SetTextSize(0.01);
  for(int irun=0; irun!=nruns; ++irun) {
    tex->DrawLatex( vov_corr[irun]+0.02, dcr[irun], Form("R%d  %.1f C",runs[irun],tem_corr[irun]) );
  }

  return 0;
}
