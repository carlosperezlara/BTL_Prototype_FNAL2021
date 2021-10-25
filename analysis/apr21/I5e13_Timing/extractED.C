int extractED() {
  TFile *file = new TFile("results/AnalysisOutput_44734_D10.root");
  TList *list;

  list = (TList*) ((TList*) file->Get("output"))->FindObject("timing");
  TCanvas *multTime = (TCanvas*) list->FindObject("ED5_Timing");
  TPad *pad = (TPad*) multTime->GetListOfPrimitives()->FindObject("ED5_Timing_19");
  //pad->GetListOfPrimitives()->ls();
  TGraph *gr0 = (TGraph*) pad->GetListOfPrimitives()->At(1);
  TGraph *gr1 = (TGraph*) pad->GetListOfPrimitives()->At(2);
  TGraph *gr3 = (TGraph*) pad->GetListOfPrimitives()->At(5);
  TGraph *gr4 = (TGraph*) pad->GetListOfPrimitives()->At(6);
  
  new TCanvas();
  gr0->Draw("AP");
  gr1->Draw("PSAME");
  gr3->Draw("PSAME");
  gr4->Draw("PSAME");

  gr0->GetYaxis()->SetTitle("mV");
  gr0->GetXaxis()->SetTitle("ns");

  TLegend *leg = new TLegend(0.6,0.1,0.9,0.5);
  leg->AddEntry( gr0, "SiPM left" );
  leg->AddEntry( gr1, "SiPM right" );
  leg->AddEntry( gr3, "SiPM left (DL 1ns)" );
  leg->AddEntry( gr4, "SiPM right (DL 1ns)" );
  leg->Draw();

  list = (TList*) ((TList*) file->Get("output"))->FindObject("energy");
  TCanvas *multEnergy = (TCanvas*) list->FindObject("ED4_Energy");
  TPad *pad2 = (TPad*) multEnergy->GetListOfPrimitives()->FindObject("ED4_Energy_19");
  //pad2->GetListOfPrimitives()->ls();
  TGraph *gr10 = (TGraph*) pad2->GetListOfPrimitives()->At(1);
  TGraph *gr11 = (TGraph*) pad2->GetListOfPrimitives()->At(2);

  new TCanvas();
  gr10->Draw("AP");
  gr11->Draw("PSAME");

  gr10->GetYaxis()->SetTitle("mV");
  gr10->GetXaxis()->SetTitle("ns");

  TLegend *leg2 = new TLegend(0.1,0.1,0.4,0.20);
  leg2->AddEntry( gr0, "SiPM left" );
  leg2->AddEntry( gr1, "SiPM right" );
  leg2->Draw();
  
  return 0;

}
