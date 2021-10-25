void format(TGraph *a, TGraph *b, int color){
  a->SetLineColor( color );
  a->SetMarkerColor( color );
  b->SetLineColor( color );
  b->SetMarkerColor( color );
  a->SetMarkerStyle(20);
  b->SetMarkerStyle(20);
}
//==================
int reso() {
  gStyle->SetOptStat(0);
  double x[7] = {5,10,15,20,30,50,100};

  double m34[7] = {116.154,97.6852,90.8371,94.9464,101.73,111.309,120.756};
  double p34[7] = {88.2264,75.4245,77.534,73.4038,74.6228,80.0848,85.1553};
  for(int i=0; i!=7; ++i) m34[i] /= 2;
  for(int i=0; i!=7; ++i) p34[i] = sqrt( p34[i]*p34[i]-50*50  ) ;
  TGraph *grm34 = new TGraph(7,x,m34);
  TGraph *grp34 = new TGraph(7,x,p34);
  format(grm34,grp34,kBlue-3);

  double m33[7] = {110.128,74.9833,71.9433,74.3464,72.4495,76.5814,89.6727};
  double p33[7] = {73.8205,72.1838,66.4298,67.1143,66.5194,67.1155,69.9206};
  for(int i=0; i!=7; ++i) m33[i] /= 2;
  for(int i=0; i!=7; ++i) p33[i] = sqrt( p33[i]*p33[i]-50*50  ) ;
  TGraph *grm33 = new TGraph(7,x,m33);
  TGraph *grp33 = new TGraph(7,x,p33);
  format(grm33,grp33,kCyan-3);

  double m31[7] = {92.5847,70.5116,67.1224,67.224,66.8045,71.5032,78.9993};
  double p31[7] = {80.9279,69.9033,67.4633,69.0634,63.7723,63.4369,67.3793};
  for(int i=0; i!=7; ++i) m31[i] /= 2;
  for(int i=0; i!=7; ++i) p31[i] = sqrt( p31[i]*p31[i]-50*50  ) ;
  TGraph *grm31 = new TGraph(7,x,m31);
  TGraph *grp31 = new TGraph(7,x,p31);
  format(grm31,grp31,kGreen-3);

  double m27[7] = {80.5899,69.839,67.0421,66.0623,66.5332,67.862,75.7893};
  double p27[7] = {80.9042,64.8224,59.7732,60.5441,61.5857,58.3342,57.9569};
  for(int i=0; i!=7; ++i) m27[i] /= 2;
  for(int i=0; i!=7; ++i) p27[i] = sqrt( p27[i]*p27[i]-50*50  ) ;
  TGraph *grm27 = new TGraph(7,x,m27);
  TGraph *grp27 = new TGraph(7,x,p27);
  format(grm27,grp27,kYellow-3);

  double m25[7] = {83.2739,63.9306,62.8566,60.6446,59.8795,60.4385,64.9335};
  double p25[7] = {80.2758,71.967,67.4865,60.6981,64.0389,59.5724,59.1083};
  for(int i=0; i!=7; ++i) m25[i] /= 2;
  for(int i=0; i!=7; ++i) p25[i] = sqrt( p25[i]*p25[i]-50*50  ) ;
  TGraph *grm25 = new TGraph(7,x,m25);
  TGraph *grp25 = new TGraph(7,x,p25);
  format(grm25,grp25,kOrange-3);

  double m30[7] = {79.293,68.0546,63.6827,62.2778,61.7924,62.5198,67.8764};
  double p30[7] = {79.1361,70.6351,63.2045,62.2014,63.2411,65.2639,63.4254};
  for(int i=0; i!=7; ++i) m30[i] /= 2;
  for(int i=0; i!=7; ++i) p30[i] = sqrt( p30[i]*p30[i]-50*50  ) ;
  TGraph *grm30 = new TGraph(7,x,m30);
  TGraph *grp30 = new TGraph(7,x,p30);
  format(grm30,grp30,kMagenta-3);

  double m28[7] = {72.276,60.0731,57.9438,57.0401,56.0422,57.4935,61.2038};
  double p28[7] = {80.1294,65.5924,58.471,60.4599,55.2213,64.6731,62.8954};
  for(int i=0; i!=7; ++i) m28[i] /= 2;
  for(int i=0; i!=7; ++i) p28[i] = sqrt( p28[i]*p28[i]-50*50  ) ;
  TGraph *grm28 = new TGraph(7,x,m28);
  TGraph *grp28 = new TGraph(7,x,p28);
  format(grm28,grp28,kRed-3);

  double m32[7] = {83.4132,64.2563,59.685,57.8714,56.4775,59.8568,64.3872};
  double p32[7] = {79.2541,72.9806,64.1843,64.8127,66.9955,67.4072,68.707};
  for(int i=0; i!=7; ++i) m32[i] /= 2;
  for(int i=0; i!=7; ++i) p32[i] = sqrt( p32[i]*p32[i]-50*50  ) ;
  TGraph *grm32 = new TGraph(7,x,m32);
  TGraph *grp32 = new TGraph(7,x,p32);
  format(grm32,grp32,kBlack);

  
  TH2D *axis1 = new TH2D("axis1","#sigma_{BAR} = #sigma^{-} / 2;Threshold  [mV];Resolution  [ps]",100,0,110,100,10,100);
  TH2D *axis2 = new TH2D("axis2","#sigma^{+} = #sqrt{#sigma^{2}_{BAR} + #sigma^{2}_{MCP}} ;Threshold  [mV];Resolution  [ps]",100,0,110,100,10,100);

  TCanvas *main = new TCanvas();
  main->Divide(2,1);

  TLegend * leg = new TLegend( 0.1, 0.7, 0.9, 0.9 );
  leg->AddEntry( grm34, "R44734 V_{op} 38.2" );
  leg->AddEntry( grm33, "R44733 V_{op} 39.2" );
  leg->AddEntry( grm31, "R44731 V_{op} 40.2" );
  leg->AddEntry( grm27, "R44727 V_{op} 41.4" );
  leg->AddEntry( grm25, "R44725 V_{op} 41.9" );
  leg->AddEntry( grm30, "R44730 V_{op} 42.2" );
  leg->AddEntry( grm28, "R44728 V_{op} 43.2" );
  leg->AddEntry( grm32, "R44732 V_{op} 44.2" );
  leg->SetNColumns(2);

  main->cd(1);
  axis1->Draw();
  grm34->Draw("PLSAME");
  grm33->Draw("PLSAME");
  grm31->Draw("PLSAME");
  grm27->Draw("PLSAME");
  grm25->Draw("PLSAME");
  grm30->Draw("PLSAME");
  grm28->Draw("PLSAME");
  grm32->Draw("PLSAME");
  leg->Draw();

  main->cd(2);
  axis2->Draw();
  grp34->Draw("PLSAME");
  grp33->Draw("PLSAME");
  grp31->Draw("PLSAME");
  grp27->Draw("PLSAME");
  grp25->Draw("PLSAME");
  grp30->Draw("PLSAME");
  grp28->Draw("PLSAME");
  grp32->Draw("PLSAME");

  return 0;
}
