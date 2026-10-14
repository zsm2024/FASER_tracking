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

    
    TFile* file1 = TFile::Open("9000_3station_forward_track_summary_ckf.root", "read");
    TFile* file2 = TFile::Open("9000_3station_backward_track_summary_ckf.root", "read");

    TFile* seedFile1 = TFile::Open("9000_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* seedFile2 = TFile::Open("9000_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

    
    const Int_t nbins = 10;
    double bins[nbins + 1] = {0, 5, 10, 20, 30, 40, 50, 60, 80, 100, 220};

    TEfficiency* eff_3F = new TEfficiency("eff_3F", ";#DeltaR1 [mm]; CKF Efficiency", nbins, bins);
    TEfficiency* eff_3B = new TEfficiency("eff_3B", ";#DeltaR1 [mm]; CKF Efficiency", nbins, bins);

    //deal with CKF forward and backward
    auto process_file = [](TFile* fileTrack, TFile* fileSeed, TEfficiency* eff, int color) {
        TTreeReader ReaderTrack("tree", fileTrack);
        TTree* t1 = ReaderTrack.GetTree();

        TTreeReader ReaderSeed("tree", fileSeed);
        TTree* t2 = ReaderSeed.GetTree();

        std::vector<double> *v1 = nullptr, *v2 = nullptr, *v3 = nullptr, *v4 = nullptr;
        std::vector<uint32_t> *v5 = nullptr;

        t2->SetBranchAddress("truthd0_x", &v1);
        t2->SetBranchAddress("truthd0_y", &v2);
        t2->SetBranchAddress("truthd1_x", &v3);
        t2->SetBranchAddress("truthd1_y", &v4);
        t1->SetBranchAddress("track_nr", &v5);

        size_t entries = t1->GetEntries();
        std::cout << "Processing file: " << fileTrack->GetName() << " | Entries: " << entries << std::endl;

        for (size_t ievent = 0; ievent < entries; ievent++) {
            t1->GetEntry(ievent);
            t2->GetEntry(ievent);
            int tracks = v5->size();

            if (v1->empty() || v3->empty()) continue;
            if(tracks < 1) continue;

            bool passTrack = (tracks >= 2);
            double x1 = v1->at(3), y1 = v2->at(3);
            double x2 = v3->at(3), y2 = v4->at(3);
            double deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            //if(deltaR > 60) continue;


            eff->Fill(passTrack, deltaR);
        }

        eff->SetMarkerColor(color);
        eff->SetLineColor(color);
    };

    process_file(file1, seedFile1, eff_3F, kRed);      // 3 站前向 (红色)
    process_file(file2, seedFile2, eff_3B, kBlue);     // 3 站后向 (蓝色)


    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 600, 500);
    eff_3F->SetMarkerStyle(20);
    eff_3B->SetMarkerStyle(20);

    eff_3F->SetMarkerSize(0.8);
    eff_3B->SetMarkerSize(0.8);

    eff_3F->Draw("AP");
    eff_3B->Draw("P SAME");
   

    auto legend = new TLegend(0.65, 0.15, 0.85, 0.35);
    legend->SetBorderSize(1);
    legend->SetLineWidth(2);
    legend->SetFillStyle(0);
    legend->AddEntry(eff_3F, "Forward", "lp");
    legend->AddEntry(eff_3B, "Backward", "lp");
    legend->Draw();

}

