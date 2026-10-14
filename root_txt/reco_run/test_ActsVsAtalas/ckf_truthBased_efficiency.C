void ckf_truthBased_efficiency(){
  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetLineWidth(2);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);

  TFile* f1 = TFile::Open("3st_forwad_Ntuple.root", "read");
  TFile* f2 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00190-00199-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("nt",f1);
  TTree* t1 = Reader1->GetTree();
  
  TTreeReader* Reader2 = new TTreeReader("tree",f2);
  TTree* t2 = Reader2->GetTree();
  //const Int_t nbins1 = 6;
  //double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.004};
  //const Int_t nbins2 = 7;
  //double bins2[nbins2 + 1] = {0, 100, 200, 300, 400, 500, 600, 1600};
  
  const Int_t nbins1 = 7;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.002, 0.004};
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};
  //const Int_t nbins2 = 10;
  //double bins2[nbins2 + 1] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1600};
  TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";prod_{#theta}[rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";prod_{p_{T}}[Mev/c];CKF Efficiency",nbins2,bins2);
  
  std::vector<float> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<int> *v3 = nullptr;

  std::vector<double> *v4 = nullptr;
  std::vector<bool> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;


  //All particles
  t2->SetBranchAddress("truth_barcode",&v1);
  t2->SetBranchAddress("truth_theta",&v2);
  t2->SetBranchAddress("truth_pT",&v7);
  t2->SetBranchAddress("truthParticle_Hits",&v6);

  //All reco tracks
  t1->SetBranchAddress("t_barcode",&v3);
  t1->SetBranchAddress("truthHitRatio", &v4);
  t1->SetBranchAddress("isFiducial", &v5);
  
  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;

  for(size_t iEvent = 0; iEvent<entries; iEvent++){
  //  std::cout<<"iEvent: "<<iEvent<<std::endl;
    t1->GetEntry(iEvent);
    t2->GetEntry(iEvent);
  
    std::vector<int>matchingParticleId;

    //loop over all tracks
    //std::cout<<"matching truthParticle: ";
    for(size_t iTrack = 0; iTrack<v3->size(); iTrack++){
      //std::cout<<v3->at(iTrack)<<"; ";
      matchingParticleId.push_back(v3->at(iTrack));
    }
    //std::cout<<std::endl;
    
    
    // loop over all particles
    //std::cout<<"truthParticle: ";
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){


      //std::cout<<v1->at(iparticle)<<", ";
      if(v6->at(iparticle)<18){
        continue;
      }

      //Fill TEfficiency
      bool hasTrack = false;
      if(std::find(matchingParticleId.begin(), matchingParticleId.end(), v1->at(iparticle)) != matchingParticleId.end()){
        hasTrack = true;
      }
      trackEff_vs_theta->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT->Fill(hasTrack,v7->at(iparticle));

    }
    //std::cout<<std::endl;
    //std::cout<<"--------------------------------"<<std::endl;
    
  }

  TCanvas* cs = new TCanvas("c1", "efficiency", 1500, 1200);
  cs->Divide(2,2);
  cs->cd(1);
  trackEff_vs_theta->SetLineWidth(2);
  trackEff_vs_theta->Draw();
  cs->cd(2);
  trackEff_vs_pT->SetLineWidth(2);
  trackEff_vs_pT->Draw();
  
}
