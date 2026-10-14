void ckf_trackSeparation1() {
    gStyle->SetOptFit(0011);
    gStyle->SetOptStat(0000);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetTitleSize(0.04, "xy");
    gStyle->SetLineWidth(2);
    gStyle->SetLabelSize(0.04, "xy");
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.1, "y");
    gStyle->SetNdivisions(510, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    
    const Int_t nbins = 10;
    double bins[nbins + 1] = {0, 5, 10, 20, 30, 40, 50, 60, 80, 100, 220}; 

    //deal with mergeTarcks
    auto processFile_deltaR = [&](const char* filename,
                              TEfficiency* eff, int color
                              ) {

      TFile* file = TFile::Open(filename, "read");

      TTreeReader* reader = new TTreeReader("nt", file);
      TTree* t = reader->GetTree();

      std::vector<double>* v1 = nullptr; // truthd0_x
      std::vector<double>* v2 = nullptr; // truthd0_y
      std::vector<double>* v3 = nullptr; // truthd1_x
      std::vector<double>* v4 = nullptr; // truthd1_y
      std::vector<unsigned long>* v5 = nullptr; // Track_hitSet

      t->SetBranchAddress("truthd0_x", &v1);
      t->SetBranchAddress("truthd0_y", &v2);
      t->SetBranchAddress("truthd1_x", &v3);
      t->SetBranchAddress("truthd1_y", &v4);
      t->SetBranchAddress("Track_hitSet", &v5);

      size_t entries = t->GetEntries();
      std::cout << "events in " << filename << ": " << entries << std::endl;

      for(size_t ievent = 0; ievent < entries; ievent++) {
          t->GetEntry(ievent);

          if(v1->size() == 0 || v3->size() == 0) continue;

          int tracks = v5->size();
          if(tracks < 1) continue;

          double x1 = v1->at(3);
          double y1 = v2->at(3);
          double x2 = v3->at(3);
          double y2 = v4->at(3);

          double deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
          //if(deltaR > 60) continue;

          bool passTrack = (tracks >= 2);

          eff->Fill(passTrack, deltaR);
      }

      eff->SetMarkerColor(color);
      eff->SetLineColor(color);

      file->Close();
    };

    TEfficiency* eff_way1 = new TEfficiency("eff_way1", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
    TEfficiency* eff_way2 = new TEfficiency("eff_way2", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
    //
    processFile_deltaR(" beforeGeo_truthSeed_PHYS.root", eff_way1, kBlue);
    processFile_deltaR(" afterGeo_truthSeed_PHYS.root", eff_way2, kRed);


    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 600, 500);
    eff_way1->SetMarkerStyle(20);
    eff_way2->SetMarkerStyle(20);
    

    eff_way1->SetMarkerSize(0.8);
    eff_way2->SetMarkerSize(0.8);

    eff_way1->Draw("AP");
    eff_way2->Draw("P SAME");
   

    auto legend = new TLegend(0.65, 0.15, 0.85, 0.35);
    legend->SetBorderSize(1);
    legend->SetLineWidth(2);
    legend->SetFillStyle(0);
    legend->AddEntry(eff_way1, "beforeGeo_3st_forward_truthParams", "lp");
    legend->AddEntry(eff_way2, "afterGeo_3st_forward_truthParams", "lp");
    legend->Draw();

    //c1->SaveAs("ckf_trackSeparation.png");
}

