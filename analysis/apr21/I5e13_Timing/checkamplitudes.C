int checkamplitudes(int run=44734, int delta=2 ) {
  TH1D  *eneLP, *eneRP;
  TH2D  *eneL, *eneR;
  TList *list2, *list3;
  list2 = (TList*) TFile::Open( Form("results/AnalysisOutput_%d_D%d.root",run,delta) )->Get("output");
  list3 = (TList*) list2->FindObject( "energy" );
  eneL = (TH2D*) ((TList*) list3->FindObject( "ENERGYL" ))->FindObject("ENERGYL_Histo_AvgEneRefTime");
  eneR = (TH2D*) ((TList*) list3->FindObject( "ENERGYR" ))->FindObject("ENERGYR_Histo_AvgEneRefTime");

  eneLP = eneL->ProjectionX("eneLP");
  eneRP = eneR->ProjectionX("eneRP");
  eneLP->SetLineColor( kBlue-3 );
  eneRP->SetLineColor( kRed-3 );
  
  new TCanvas();
  eneLP->Draw();
  eneRP->Draw("same");
  
  return 0;
}
