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

  TFile* f1 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00000-00009-s0008_3station_backward_track_summary_ckf.root", "read");
  TFile* f2 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  
  TTreeReader* Reader2 = new TTreeReader("tree",f2);
  TTree* t2 = Reader2->GetTree();
  //const Int_t nbins1 = 11;
  //double bins1[nbins1 + 1] = {0, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.02, 0.025, 0.03, 0.04, 0.05};
  //const Int_t nbins2 = 9;
  //double bins2[nbins2 + 1] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.6};
  //const Int_t nbins1 = 6;
  //double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.004};
  //const Int_t nbins2 = 7;
  //double bins2[nbins2 + 1] = {0, 100, 200, 300, 400, 500, 600, 1600};
  //TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";theta[rad];CKF Efficiency",nbins1,bins1);
  //TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";pT[Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";theta[rad];CKF Efficiency",10,0,0.001);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";pT[Mev/c];CKF Efficiency",10,0,100);
  
  std::vector<float> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;

  std::vector<double> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;


  //All particles
  t2->SetBranchAddress("truth_barcode",&v1);
  t2->SetBranchAddress("truth_theta",&v2);
  t2->SetBranchAddress("truth_pT",&v7);
  t2->SetBranchAddress("truthParticle_Hits",&v6);

  //All reco tracks
  t1->SetBranchAddress("majorityParticleId",&v3);
  
  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;

  for(size_t iEvent = 0; iEvent<entries; iEvent++){
    t1->GetEntry(iEvent);
    t2->GetEntry(iEvent);
    std::map<int,int>particleIdWithTracks;

    //loop over all tracks
    for(size_t iTrack = 0; iTrack<v3->size(); iTrack++){
      particleIdWithTracks[v3->at(iTrack)]++;
    }

    // loop over all particles
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){
      

      if(v6->at(iparticle)<18){
        continue;
      }


      //Fill TEfficiency
      bool hasTrack = false;
      if (particleIdWithTracks.find(v1->at(iparticle)) != particleIdWithTracks.end()) {
        hasTrack = true;
      }
      trackEff_vs_theta->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT->Fill(hasTrack,v7->at(iparticle));

    }
  }

  TCanvas* cs = new TCanvas("c1", "efficiency", 600, 500);
  cs->Divide(2,2);
  cs->cd(1);
  trackEff_vs_theta->SetLineWidth(3);
  trackEff_vs_theta->Draw();
  cs->cd(2);
  trackEff_vs_pT->SetLineWidth(3);
  trackEff_vs_pT->Draw();
  
}
