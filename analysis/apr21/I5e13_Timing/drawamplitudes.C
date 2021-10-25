//==================
int drawamplitudes() {
  gStyle->SetOptStat(0);

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
  }

  TGraph *grTEM = new TGraph( nruns, tem, mpv );
  TGraph *grAMP = new TGraph( nruns, amp, mpv );
  TGraph *grVOL = new TGraph( nruns, vol, mpv );
  TGraph *grVOV = new TGraph( nruns, vov, mpv );


  new TCanvas();
  grTEM->Draw("A*");
  new TCanvas();
  grAMP->Draw("A*");
  new TCanvas();
  grVOL->Draw("A*");
  new TCanvas();
  grVOV->Draw("A*");

  TLatex *tex = new TLatex();
  grTEM->GetXaxis()->SetTitle("Temperature");
  grTEM->GetYaxis()->SetTitle("MPV");
  for(int irun=0; irun!=nruns; ++irun) {
  }
  
  grAMP->GetXaxis()->SetTitle("Current");
  grAMP->GetYaxis()->SetTitle("MPV");

  grVOL->GetXaxis()->SetTitle("Volts");
  grVOL->GetYaxis()->SetTitle("MPV");

  grVOV->GetXaxis()->SetTitle("VoV");
  grVOV->GetYaxis()->SetTitle("MPV");

  return 0;
}
