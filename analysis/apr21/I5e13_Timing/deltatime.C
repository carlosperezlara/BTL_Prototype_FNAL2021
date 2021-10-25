int deltatime(int run=44734, int delta=10 ) {
  TList *list[7], *list2, *list3;
  TH2D     *LR[7];
  TH2D     *XT[7];
  TH2D     *XTC[7];
  TH1D     *T[7];
  TH2D     *XT_P[7];
  TH1D     *T_P[7];
  TProfile *profXTM[7];
  TProfile *profXTP[7];
  list2 = (TList*) TFile::Open( Form("results/AnalysisOutput_%d_D%d.root",run,delta) )->Get("output");
  list3 = (TList*) list2->FindObject( "timing" );
  for(int ithr=0; ithr!=7; ++ithr) {
    list[ithr] = (TList*) list3->FindObject( Form("thr_%d",ithr) );
    LR[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingLR_%d",ithr) );
    XT[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingX_%d",ithr) );
    XTC[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingXC_%d",ithr) );
    T[ithr] = (TH1D*) list[ithr]->FindObject( Form("fTiming_%d",ithr) );
    XT_P[ithr] = (TH2D*) list[ithr]->FindObject( Form("fTimingX_P_%d",ithr) );
    T_P[ithr] = (TH1D*) list[ithr]->FindObject( Form("fTiming_P_%d",ithr) );
    profXTM[ithr] = XT[ithr]->ProfileX( Form("profXTM_%d",ithr) );
    profXTP[ithr] = XT_P[ithr]->ProfileX( Form("profXTP_%d",ithr) );
  }

  TCanvas *main = new TCanvas();
  main->Divide(7,2);
  TLatex *tex = new TLatex();
  tex->SetTextSize(0.1);
  TF1 *fit[7];
  TF1 *fit_P[7];
  for(int ithr=0; ithr!=7; ++ithr) {
    main->cd(1+ithr);
    LR[ithr]->Draw("colz");
    tex->SetTextColor(kGray+2);
    tex->DrawLatexNDC(0.2,0.2, Form("r %.2f",LR[ithr]->GetCorrelationFactor()) );

    main->cd(8+ithr);
    T[ithr]->SetLineColor(kRed-3);
    T_P[ithr]->GetXaxis()->SetTitle("MIP TIMING  [ns]");
    T_P[ithr]->GetXaxis()->SetTitleSize(0.06);
    T_P[ithr]->GetXaxis()->SetTitleOffset(0.8);
    T_P[ithr]->GetXaxis()->SetLabelSize(0.05);
    T_P[ithr]->SetLineColor(kBlue-3);
    T_P[ithr]->Draw();
    double xce;
    xce = T_P[ithr]->GetBinCenter( T_P[ithr]->GetMaximumBin() );
    fit_P[ithr] = new TF1( Form("fit_P_%d",ithr), "gaus" );
    fit_P[ithr]->SetLineColor(kBlue-3);
    T_P[ithr]->Fit(fit_P[ithr],"R","",-0.4+xce,+0.4+xce);
    T_P[ithr]->Fit(fit_P[ithr],"R","",
                   -3*fit_P[ithr]->GetParameter(2),
                   +3*fit_P[ithr]->GetParameter(2));

    T[ithr]->Draw("same");
    xce = T[ithr]->GetBinCenter( T[ithr]->GetMaximumBin() );
    fit[ithr] = new TF1( Form("fit_%d",ithr), "gaus" );
    fit[ithr]->SetLineColor(kRed-3);
    T[ithr]->Fit(fit[ithr],"R","",-0.8+xce,+0.8+xce);
    T[ithr]->Fit(fit[ithr],"R","",
                 -3*fit[ithr]->GetParameter(2),
                 +3*fit[ithr]->GetParameter(2));

    tex->SetTextColor(kRed-3);
    tex->DrawLatexNDC( 0.15, 0.85, Form("#sigma^{-} %.0f ps",fit[ithr]->GetParameter(2)*1e3) );
    tex->SetTextColor(kBlue-3);
    tex->DrawLatexNDC( 0.15, 0.80, Form("#sigma^{+} %.0f ps",fit_P[ithr]->GetParameter(2)*1e3) );
  }

  double minus[7];
  double plus[7];
  for(int ithr=0; ithr!=7; ++ithr) {
    minus[ithr] = fit[ithr]->GetParameter(2)*1e3;
    cout << minus[ithr] << ",";
  }
  cout << endl;
  for(int ithr=0; ithr!=7; ++ithr) {
    plus[ithr] = fit_P[ithr]->GetParameter(2)*1e3;
    cout << plus[ithr] << ",";
  }
  cout << endl;

  TCanvas *main2 = new TCanvas();
  main2->SetLeftMargin(0.25);
  main2->SetBottomMargin(0.2);
  main2->Divide(7,2,0,0);
  TCanvas *main21 = new TCanvas();
  main21->SetLeftMargin(0.25);
  main21->SetBottomMargin(0.2);
  main21->Divide(7,2,0,0);
  int bX;
  TH1D *XT_PY0[7], *XTC_PY0[7], *XTP_PY0[7];
  TH1D *XT_PY1[7], *XTC_PY1[7], *XTP_PY1[7];
  
  TF1 *fitXTM[7];
  TF1 *fitXTP[7];

  for(int ithr=0; ithr!=7; ++ithr) {
    XT[ithr]->GetYaxis()->SetTitleSize(0.1);
    XT[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XT[ithr]->GetYaxis()->SetLabelSize(0.1);

    bX = XT[ithr]->GetXaxis()->FindBin(10);
    XT_PY0[ithr] = XT[ithr]->ProjectionY( Form("%s_Y0",XT[ithr]->GetName()), bX-5, bX+5  );
    bX = XT[ithr]->GetXaxis()->FindBin(18);
    XT_PY1[ithr] = XT[ithr]->ProjectionY( Form("%s_Y1",XT[ithr]->GetName()), bX-5, bX+5  );
    XT_PY1[ithr]->SetLineColor(kRed-3);
    
    XT_P[ithr]->GetYaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XT_P[ithr]->GetYaxis()->SetLabelSize(0.1);

    bX = XT_P[ithr]->GetXaxis()->FindBin(10);
    XTP_PY0[ithr] = XT_P[ithr]->ProjectionY( Form("%s_Y0",XT_P[ithr]->GetName()), bX-5, bX+5  );
    bX = XT_P[ithr]->GetXaxis()->FindBin(18);
    XTP_PY1[ithr] = XT_P[ithr]->ProjectionY( Form("%s_Y1",XT_P[ithr]->GetName()), bX-5, bX+5  );
    XTP_PY1[ithr]->SetLineColor(kRed-3);

    XT_P[ithr]->GetXaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetXaxis()->SetTitleOffset(0.85);
    XT_P[ithr]->GetXaxis()->SetLabelSize(0.1);
    XT_P[ithr]->GetXaxis()->SetNdivisions(505);

    tex->SetTextColor(kGray+2);

    main2->cd(1+ithr);
    profXTM[ithr]->SetLineColor(kRed-3);
    profXTM[ithr]->SetLineWidth(2);
    fitXTM[ithr] = new TF1( Form("fitXTM_%d",ithr), "[0]+[1]*x" );
    profXTM[ithr]->Fit( fitXTM[ithr] );
    XT[ithr]->Draw("colz");
    profXTM[ithr]->Draw("same");
    tex->DrawLatexNDC(0.4,0.1, Form("r %.2f",XT[ithr]->GetCorrelationFactor()) );

    main2->cd(8+ithr);
    profXTP[ithr]->SetLineColor(kRed-3);
    profXTP[ithr]->SetLineWidth(2);
    fitXTP[ithr] = new TF1( Form("fitXTP_%d",ithr), "[0]+[1]*x" );
    profXTP[ithr]->Fit( fitXTP[ithr] );
    XT_P[ithr]->Draw("colz");
    profXTP[ithr]->Draw("same");
    tex->DrawLatexNDC(0.4,0.4, Form("r %.2f",XT_P[ithr]->GetCorrelationFactor()) );

    main21->cd(1+ithr);
    XT_PY0[ithr]->DrawNormalized();
    XT_PY1[ithr]->DrawNormalized("same");
    main21->cd(8+ithr);
    XTP_PY0[ithr]->DrawNormalized();
    XTP_PY1[ithr]->DrawNormalized("same");

  }

  TCanvas *main3 = new TCanvas();
  main3->SetLeftMargin(0.25);
  main3->SetBottomMargin(0.2);
  main3->Divide(7,2,0,0);

  TCanvas *main4 = new TCanvas();
  main4->SetLeftMargin(0.25);
  main4->SetBottomMargin(0.2);
  main4->Divide(7,2,0,0);
  for(int ithr=0; ithr!=7; ++ithr) {
    XTC[ithr]->GetYaxis()->SetTitleSize(0.1);
    XTC[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XTC[ithr]->GetYaxis()->SetLabelSize(0.1);
    XTC[ithr]->GetYaxis()->SetTitle("L - R (CORR)  [ns]");

    bX = XTC[ithr]->GetXaxis()->FindBin(10);
    XTC_PY0[ithr] = XTC[ithr]->ProjectionY( Form("%s_Y0",XTC[ithr]->GetName()), bX-5, bX+5  );
    bX = XTC[ithr]->GetXaxis()->FindBin(18);
    XTC_PY1[ithr] = XTC[ithr]->ProjectionY( Form("%s_Y1",XTC[ithr]->GetName()), bX-5, bX+5  );
    XTC_PY1[ithr]->SetLineColor(kRed-3);
    
    XT_P[ithr]->GetYaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetYaxis()->SetTitleOffset(1.2);
    XT_P[ithr]->GetYaxis()->SetLabelSize(0.1);

    XT_P[ithr]->GetXaxis()->SetTitleSize(0.1);
    XT_P[ithr]->GetXaxis()->SetTitleOffset(0.85);
    XT_P[ithr]->GetXaxis()->SetLabelSize(0.1);
    XT_P[ithr]->GetXaxis()->SetNdivisions(505);


    main3->cd(1+ithr);
    XTC[ithr]->Draw("colz");
    tex->DrawLatexNDC(0.3,0.1, Form("r %.2f",XTC[ithr]->GetCorrelationFactor()) );
    main3->cd(8+ithr);
    XT_P[ithr]->Draw("colz");

    main4->cd(1+ithr);
    XTC_PY0[ithr]->DrawNormalized();
    XTC_PY1[ithr]->DrawNormalized("same");
    main4->cd(8+ithr);
    XTP_PY0[ithr]->DrawNormalized();
    XTP_PY1[ithr]->DrawNormalized("same");

  }

  // generate walk file
  ofstream fout( Form("RunConfig/Run_%d_%d_LIN.csv",run,delta) );
  for(int ithr=0; ithr!=7; ++ithr) {
    fout << fitXTM[ithr]->GetParameter(0) << " ";
    fout << fitXTM[ithr]->GetParameter(1) << endl;
  }
  for(int ithr=0; ithr!=7; ++ithr) {
    fout << fitXTP[ithr]->GetParameter(0) << " ";
    fout << fitXTP[ithr]->GetParameter(1) << endl;
  }
  fout.close();

  fout.open( Form("results/Run_%d_%d_THR.csv",run,delta) );
  for(int ithr=0; ithr!=7; ++ithr) {
    fout << fit[ithr]->GetParameter(1) << " ";
    fout << fit[ithr]->GetParameter(2) << endl;
  }
  for(int ithr=0; ithr!=7; ++ithr) {
    fout << fit_P[ithr]->GetParameter(1) << " ";
    fout << fit_P[ithr]->GetParameter(2) << endl;
  }
  fout.close();
  return 0;
}
