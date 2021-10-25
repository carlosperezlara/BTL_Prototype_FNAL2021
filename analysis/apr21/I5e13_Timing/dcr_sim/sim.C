Double_t aphotoelectron(Double_t *xx, Double_t *par) {
  Float_t x = xx[0];
  Float_t sg1 = 1.5;
  Float_t mu = par[0]+3*sg1;
  Float_t sg2 = 5;
  Double_t f;
  f = -TMath::Gaus(x,mu,sg1,1);
  if(x>mu+sg1) f += +0.8*TMath::Gaus(x,mu+sg2,sg2,1);
  f *= 5;
  return f;
}
//===============
TF1 *onephoton = new TF1("onephoton",aphotoelectron,0,200,1);
//===============
void FillOnePhoton(TGraph *gr, Double_t arrival) {
  onephoton->SetParameter(0,arrival);
  for(int i=0; i!=1024; ++i) {
    Double_t x = gr->GetPointX(i);
    Double_t y = gr->GetPointY(i);
    gr->SetPointY( i, y+onephoton->Eval(x) );
  }
}
//===============
void FillMIP(TGraph *gr, Double_t arrival) {
  int number_of_photons = 4000;
  for( int iph=0; iph!=number_of_photons; ++iph ) {
    Double_t smeared = arrival + (gRandom->Rndm()-0.5)*0;
    onephoton->SetParameter(0,smeared);
    for(int i=0; i!=1024; ++i) {
      Double_t x = gr->GetPointX(i);
      Double_t y = gr->GetPointY(i);
      gr->SetPointY( i, y+onephoton->Eval(x) );
    }
  }
}
//===============
void SimulateDCR(TGraph *gr, Double_t dcr /*GHz*/) {
  int photons_in_window = dcr/5.0*200;
  for( int iph=0; iph!=photons_in_window; ++iph ) {
    Double_t arrival = gRandom->Rndm()*200;
    onephoton->SetParameter(0,arrival);
    for(int i=0; i!=1024; ++i) {
      Double_t x = gr->GetPointX(i);
      Double_t y = gr->GetPointY(i);
      gr->SetPointY( i, y+onephoton->Eval(x) );
    }
  }
}
//===============
void clip(TGraph *gr) {
  for(int i=0; i!=1024; ++i) {
    Double_t y = gr->GetPointY(i);
    if(y>300) y = 300;
    if(y<-800) y = -800;
    gr->SetPointY( i, y );
  }
}

//===============
int sim() {
  Double_t x[1024], y[1024];
  for(int i=0; i!=1024; ++i) {
    x[i] = 0.2*i;
    y[i] = 0;
  }

  TGraph *wave1 = new TGraph( 1024, x, y );
  FillMIP(wave1,80);
  TGraph *wave2 = new TGraph( 1024, wave1->GetX(), wave1->GetY() );
  //FillOnePhoton(wave,80);
  SimulateDCR(wave2,100);
  clip(wave1);
  clip(wave2);
  wave2->Draw("AP");
  wave1->Draw("PSAME");
  wave1->SetMarkerColor(kBlue-3);
  wave2->SetMarkerColor(kRed-3);
  wave1->SetMarkerStyle(20);
  wave2->SetMarkerStyle(20);

  /*
  for(int i=0; i!=1024; ++i) {
    Double_t y = wave1->GetPointY(i);
    cout << y << ", ";
  }
   */
  cout << endl << endl;
  for(int i=0; i!=1024; ++i) {
    Double_t y = wave2->GetPointY(i);
    cout << y << ", ";
  }

  return 0;
}
