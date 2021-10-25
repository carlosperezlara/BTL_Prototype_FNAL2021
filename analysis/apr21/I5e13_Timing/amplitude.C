//==================
int amplitude(int run=44734, int delay=2) {
  gStyle->SetOptStat(0);
  TFile *file = new TFile( Form("results/AnalysisOutput_%d_D%d.root",run,delay) );
  TList *list = (TList*) ( (TList*) file->Get("output") )->FindObject("timing");
  TH1D *hist = (TH1D*) list->FindObject( "fAvgMPV" );

  TCanvas *canvas = new TCanvas();
  hist->Draw();
  TF1 *fit = new TF1("fit","landau");
  hist->Fit(fit,"R","",0,400);

  ofstream fout( Form("results/Run_%d_%d_mpv.csv",run,delay) );
  fout << run << " " << fit->GetParameter(1) << endl;
  fout.close();
  
  return 0;
}
