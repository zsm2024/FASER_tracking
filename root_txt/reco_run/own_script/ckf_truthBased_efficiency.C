void ckf_truthBased_efficiency(){
  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetLineWidth(4);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);

  TFile* f1 = TFile::Open("total1_3station_forward_track_summary_ckf.root", "read");
  TFile* f2 = TFile::Open("total1_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  
  TTreeReader* Reader2 = new TTreeReader("tree",f2);
  TTree* t2 = Reader2->GetTree();
  const Int_t nbins1 = 11;
  double bins1[nbins1 + 1] = {0, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.02, 0.025, 0.03, 0.04, 0.05};
  const Int_t nbins2 = 9;
  double bins2[nbins2 + 1] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.6};
  const Int_t nbins3 = 19;
  double bins3[nbins3 + 1] = {0, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 100, 200};

  TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";theta[rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";pT[Gev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_deltaR = new TEfficiency("eff",";deltaR[mm];CKF Efficiency",nbins3,bins3);
  TH2D* h1 = new TH2D("h1",";deltaR;1st_hits_r",22,0,220,22,0,220);
  TH2D* h2 = new TH2D("h2",";deltaR;1st_hits_r",22,0,220,22,0,220);
  
  std::vector<float> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;

  std::vector<double> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;
  std::vector<double> *v8 = nullptr;
  std::vector<double> *v9 = nullptr;
  std::vector<double> *v10 = nullptr;
  std::vector<double> *v11 = nullptr;
  std::vector<double> *v12 = nullptr;
  std::vector<double> *v13 = nullptr;


  //All particles
  t2->SetBranchAddress("truth_barcode",&v1);
  t2->SetBranchAddress("truth_theta",&v2);
  t2->SetBranchAddress("truth_pT",&v7);
  t2->SetBranchAddress("truthParticle_Hits",&v6);
  t2->SetBranchAddress("truthd0_x",&v8);
  t2->SetBranchAddress("truthd0_y",&v9);
  t2->SetBranchAddress("truthd1_x",&v10);
  t2->SetBranchAddress("truthd1_y",&v11);
  t2->SetBranchAddress("truth_st1_x",&v12);
  t2->SetBranchAddress("truth_st1_y",&v13);

  //All reco tracks
  t1->SetBranchAddress("majorityParticleId",&v3);
  t1->SetBranchAddress("t_truthHitRatio", &v4);
  t1->SetBranchAddress("nMeasurements", &v5);
  
  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;
  std::map<int,int>particleIdWithTracks;

  for(size_t iEvent = 0; iEvent<entries; iEvent++){
    t1->GetEntry(iEvent);
    t2->GetEntry(iEvent);
    if(v8->size()==0||v10->size()==0)continue;
    double deltaR;
    double x1 = v8->at(3);
    double y1 = v9->at(3);
    double x2 = v10->at(3);
    double y2 = v11->at(3);
    deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

    //loop over all tracks
    for(size_t iTrack = 0; iTrack<v3->size(); iTrack++){
      particleIdWithTracks[iTrack]++;
    }

    // loop over all particles
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){
      if(v6->at(iparticle)<18){
        continue;
      }
      double x1 = v12->at(iparticle);
      double y1 = v13->at(iparticle);
      double r = sqrt(x1*x1+y1*y1);

      //Fill TEfficiency
      bool hasTrack = false;
      if (particleIdWithTracks.find(v1->at(iparticle)) != particleIdWithTracks.end()) {
        hasTrack = true;
      }
      h2->Fill(r,deltaR);
      if(hasTrack == false){
        h1->Fill(r,deltaR);
      }
      trackEff_vs_theta->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT->Fill(hasTrack,v7->at(iparticle));
      trackEff_vs_deltaR->Fill(hasTrack,deltaR);

    }
  }

  TCanvas* cs = new TCanvas("c1", "efficiency", 600, 500);
  cs->Divide(2,3);
  cs->cd(1);
  trackEff_vs_theta->SetLineWidth(3);
  trackEff_vs_theta->Draw();
  cs->cd(2);
  trackEff_vs_pT->SetLineWidth(3);
  trackEff_vs_pT->Draw();
  cs->cd(3);
  trackEff_vs_deltaR->SetLineWidth(3);
  trackEff_vs_deltaR->Draw();
  cs->cd(5);
  h1->SetLineWidth(3);
  h1->Draw("colz");
  cs->cd(6);
  h2->SetLineWidth(3);
  h2->Draw("colz");
  
}
