void 3Samples_CKFTruthBasedEfficiency() {
  gStyle->SetOptStat(0);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetLineWidth(2);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  
  const std::vector<std::string> filenames = {
    "110038-4st-PHYS.root",
    "110033-4st-PHYS.root",
    "110051-4st-PHYS.root"
  };
  const std::vector<int> colors = {600, 633, 418};
  const std::vector<std::string> labels = {
    "m_{A'} = 100 MeV; #varepsilon = 10^{-5}",
    "m_{A'} = 10 MeV; #varepsilon = 10^{-5}",
    "m_{A'} = 10 MeV; #varepsilon = 10^{-4}"
  };

  const Int_t nbins1 = 6;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.001, 0.003, 0.005};
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};

  std::vector<TEfficiency*> eff_theta_list;
  std::vector<TEfficiency*> eff_pT_list;

  for (size_t ifile = 0; ifile < filenames.size(); ++ifile) {
    TFile* f = TFile::Open(filenames[ifile].c_str(), "read");
    TTree* t = (TTree*)f->Get("nt");

    std::vector<float> *truth_barcode = nullptr;
    std::vector<float> *truth_theta   = nullptr;
    std::vector<float> *truth_pT      = nullptr;
    std::vector<int>   *truthParticle_Hits = nullptr;
    std::vector<int>   *majorityParticleId = nullptr;

    t->SetBranchAddress("truth_barcode", &truth_barcode);
    t->SetBranchAddress("truth_theta", &truth_theta);
    t->SetBranchAddress("truth_pT", &truth_pT);
    t->SetBranchAddress("truthParticle_Hits", &truthParticle_Hits);
    t->SetBranchAddress("majorityParticleId", &majorityParticleId);

    TEfficiency* eff_theta = new TEfficiency(
        Form("eff_theta_%zu", ifile),
        ";prod_{#theta} [rad]; CKF Efficiency", nbins1, bins1);
    TEfficiency* eff_pT = new TEfficiency(
        Form("eff_pT_%zu", ifile),
        ";prod_{p_{T}} [MeV/c]; CKF Efficiency", nbins2, bins2);

    size_t entries = t->GetEntries();
    for (size_t iEvent = 0; iEvent < entries; iEvent++) {
      t->GetEntry(iEvent);

      std::vector<int> matchingParticleId;
      for (size_t iTrack = 0; iTrack < majorityParticleId->size(); iTrack++) {
        matchingParticleId.push_back(majorityParticleId->at(iTrack));
      }

      for (size_t ip = 0; ip < truth_barcode->size(); ++ip) {
        if (truthParticle_Hits->at(ip) < 18) continue;

        bool hasTrack = (std::find(matchingParticleId.begin(),
                                   matchingParticleId.end(),
                                   truth_barcode->at(ip)) != matchingParticleId.end());

        eff_theta->Fill(hasTrack, truth_theta->at(ip));
        eff_pT->Fill(hasTrack, truth_pT->at(ip));
      }
    }

    eff_theta->SetLineColor(colors[ifile]);
    eff_theta->SetLineWidth(2);
    eff_pT->SetLineColor(colors[ifile]);
    eff_pT->SetLineWidth(2);

    eff_theta_list.push_back(eff_theta);
    eff_pT_list.push_back(eff_pT);
  }

  
  TCanvas* c = new TCanvas("c","efficiency compare",1500,600);
  c->Divide(2,1);

  // theta
  c->cd(1);
  eff_theta_list[0]->Draw();
  for (size_t i=1; i<eff_theta_list.size(); ++i) {
    eff_theta_list[i]->Draw("same");
  }
  TLegend* leg1 = new TLegend(0.5,0.2,0.85,0.4);
  for (size_t i=0; i<labels.size(); ++i) {
    leg1->AddEntry(eff_theta_list[i], labels[i].c_str(), "l");
  }
  leg1->Draw();

  // pT
  c->cd(2);
  eff_pT_list[0]->Draw();
  for (size_t i=1; i<eff_pT_list.size(); ++i) {
    eff_pT_list[i]->Draw("same");
  }
  TLegend* leg2 = new TLegend(0.5,0.2,0.85,0.4);
  for (size_t i=0; i<labels.size(); ++i) {
    leg2->AddEntry(eff_pT_list[i], labels[i].c_str(), "l");
  }
  leg2->Draw();
}

