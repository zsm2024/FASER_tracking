void allClusters() {
    TFile* f = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_4station_forward_seed_summary_circleFitTrackSeedTool.root");  // 替换为实际文件名
    TTree* t1 = (TTree*)f->Get("tree");

    // Declare variables
    int st0_l0, st0_l1, st0_l2;
    int st1_l0, st1_l1, st1_l2;
    int st2_l0, st2_l1, st2_l2;
    int st3_l0, st3_l1, st3_l2;

    // Set branch addresses
    t1->SetBranchAddress("nClusters_st0_l0", &st0_l0);
    t1->SetBranchAddress("nClusters_st0_l1", &st0_l1);
    t1->SetBranchAddress("nClusters_st0_l2", &st0_l2);
    t1->SetBranchAddress("nClusters_st1_l0", &st1_l0);
    t1->SetBranchAddress("nClusters_st1_l1", &st1_l1);
    t1->SetBranchAddress("nClusters_st1_l2", &st1_l2);
    t1->SetBranchAddress("nClusters_st2_l0", &st2_l0);
    t1->SetBranchAddress("nClusters_st2_l1", &st2_l1);
    t1->SetBranchAddress("nClusters_st2_l2", &st2_l2);
    t1->SetBranchAddress("nClusters_st3_l0", &st3_l0);
    t1->SetBranchAddress("nClusters_st3_l1", &st3_l1);
    t1->SetBranchAddress("nClusters_st3_l2", &st3_l2);

    const int nLayers = 12;
    int* vars[nLayers] = {
        &st0_l0, &st0_l1, &st0_l2,
        &st1_l0, &st1_l1, &st1_l2,
        &st2_l0, &st2_l1, &st2_l2,
        &st3_l0, &st3_l1, &st3_l2
    };

    // Step 1: Find max value per layer
    std::vector<int> maxPerLayer(nLayers, 0);
    Long64_t nentries = t1->GetEntries();

    for (Long64_t entry = 0; entry < nentries; ++entry) {
        t1->GetEntry(entry);
        for (int i = 0; i < nLayers; ++i) {
            if (*vars[i] > maxPerLayer[i]) {
                maxPerLayer[i] = *vars[i];
            }
        }
    }

    // Step 2: Create histograms with dynamic bin ranges
    TH1I* hists[nLayers];
    for (int i = 0; i < nLayers; ++i) {
        int maxVal = maxPerLayer[i];
        int nBins = std::max(10, std::min(100, maxVal + 5));  // 自动设定bin数（可调）
        //hists[i] = new TH1I(Form("layerTotal%d", i), "", nBins, 0, maxVal + 1);
        hists[i] = new TH1I(Form("layerTotal%d", i), "", 10, 0, 20);
    }

    // Step 3: Re-loop and fill histograms
    for (Long64_t entry = 0; entry < nentries; ++entry) {
        t1->GetEntry(entry);
        for (int i = 0; i < nLayers; ++i) {
            hists[i]->Fill(*vars[i]);
        }
    }

    // Step 4: Draw
    TCanvas* c2 = new TCanvas("c2", "Total cluster count per layer", 1600, 1200);
    c2->Divide(3, 4);

    for (int i = 0; i < nLayers; ++i) {
        c2->cd(i + 1);
        hists[i]->SetLineColor(kRed + 1);
        hists[i]->SetFillColorAlpha(kBlue + 7, 0.4);
        hists[i]->SetStats(0);
        hists[i]->GetXaxis()->SetTitle(Form("Station %d Layer %d", i / 3, i % 3));
        hists[i]->GetYaxis()->SetTitle("Entries");
//        hists[i]->SetTitle(Form("Station %d Layer %d", i / 3, i % 3));
        hists[i]->Draw("HIST");
    }

  //  c2->SaveAs("total_clusters_per_layer.pdf");
}

