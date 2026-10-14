void ckf_trackSeparation1() {
    gStyle->SetOptFit(0011);
    gStyle->SetOptStat(0000);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetTitleSize(0.04, "xy");
    gStyle->SetLineWidth(4);
    gStyle->SetLabelSize(0.04, "xy");
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.1, "y");
    gStyle->SetNdivisions(510, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    
    TFile* file1 = TFile::Open("total1_3station_forward_track_summary_ckf.root", "read");
    TFile* file2 = TFile::Open("total1_3station_backward_track_summary_ckf.root", "read");
    TFile* file3 = TFile::Open("total1_4station_forward_track_summary_ckf.root", "read");
    TFile* file4 = TFile::Open("total1_4station_backward_track_summary_ckf.root", "read");

    TFile* seedFile1 = TFile::Open("total1_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* seedFile2 = TFile::Open("total1_3station_backward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* seedFile3 = TFile::Open("total1_4station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* seedFile4 = TFile::Open("total1_4station_backward_seed_summary_circleFitTrackSeedTool.root", "read");

    
    const Int_t nbins = 19;
    double bins[nbins + 1] = {0, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 100, 200};

    TEfficiency* eff_3F = new TEfficiency("eff_3F", ";#DeltaR1 [mm]; CKF Efficiency", nbins, bins);
    TEfficiency* eff_3B = new TEfficiency("eff_3B", ";#DeltaR1 [mm]; CKF Efficiency", nbins, bins);
    TEfficiency* eff_4F = new TEfficiency("eff_4F", ";#DeltaR1 [mm]; CKF Efficiency", nbins, bins);
    TEfficiency* eff_4B = new TEfficiency("eff_4B", ";#DeltaR1 [mm]; CKF Efficiency", nbins, bins);

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

            bool passTrack = false;
            double x1 = v1->at(3), y1 = v2->at(3);
            double x2 = v3->at(3), y2 = v4->at(3);
            double deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            if(tracks ≥ 2){
              passTrack = true;
            }

            eff->Fill(passTrack, deltaR);
        }

        eff->SetMarkerColor(color);
        eff->SetLineColor(color);
    };

    process_file(file1, seedFile1, eff_3F, kRed);      
    process_file(file2, seedFile2, eff_3B, kBlue);     
    process_file(file3, seedFile3, eff_4F, kGreen+2);  
    process_file(file4, seedFile4, eff_4B, kMagenta);

    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 1500, 600);
    eff_3F->SetMarkerStyle(21);
    eff_3B->SetMarkerStyle(22);
    eff_4F->SetMarkerStyle(23);
    eff_4B->SetMarkerStyle(33);

    eff_3F->SetMarkerSize(1.0);
    eff_3B->SetMarkerSize(1.0);
    eff_4F->SetMarkerSize(1.0);
    eff_4B->SetMarkerSize(1.0);

    eff_3F->Draw("AP");
    eff_3B->Draw("P SAME");
    eff_4F->Draw("P SAME");
    eff_4B->Draw("P SAME");

    auto legend = new TLegend(0.65, 0.15, 0.85, 0.35);
    legend->SetBorderSize(1);
    legend->SetFillStyle(0);
    legend->AddEntry(eff_3F, "3-Station Forward", "p");
    legend->AddEntry(eff_3B, "3-Station Backward", "p");
    legend->AddEntry(eff_4F, "4-Station Forward", "p");
    legend->AddEntry(eff_4B, "4-Station Backward", "p");
    legend->Draw();

    c1->SaveAs("ckf_trackSeparation.png");
}

