void truth_nHits_3Samples() {
    gStyle->SetOptStat(0000);

    const char* files[3] = {
        "total_110038_4st_forward_seed.root",
        "total_110033_4st_forward_seed.root",
        "total_110051_4st_forward_seed.root"
    };

    const char* labels[3] = {
        "m_{A'} = 100 Mev; #varepsilon = 10^{-5}",
        "m_{A'} = 10 Mev; #varepsilon = 10^{-5}",
        "m_{A'} = 10 Mev; #varepsilon = 10^{-4}",
    };

    int colors[3] = { kRed, kBlue, kGreen + 2 };

    const char* variables[] = {
        "truth_nHits_st1_l0",
        "truth_nHits_st1_l1",
        "truth_nHits_st1_l2",
        "truth_nHits_st2_l0",
        "truth_nHits_st2_l1",
        "truth_nHits_st2_l2",
        "truth_nHits_st3_l0",
        "truth_nHits_st3_l1",
        "truth_nHits_st3_l2"
    };

    const char* axisTitles[] = {
        ";truthHits_st1_l0;Events",
        ";truthHits_st1_l1;Events",
        ";truthHits_st1_l2;Events",
        ";truthHits_st2_l0;Events",
        ";truthHits_st2_l1;Events",
        ";truthHits_st2_l2;Events",
        ";truthHits_st3_l0;Events",
        ";truthHits_st3_l1;Events",
        ";truthHits_st3_l2;Events"
    };

    const int nVars = sizeof(variables) / sizeof(variables[0]);

    TCanvas* c = new TCanvas("c_multi", "Truth Hit Comparison", 1200, 1000);
    c->Divide(3, 3); 

    for (int v = 0; v < nVars; ++v) {
        c->cd(v + 1); 

        TLegend* leg = new TLegend(0.55, 0.70, 0.88, 0.88);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        leg->SetTextSize(0.035);

        for (int i = 0; i < 3; ++i) {
            TFile* file = TFile::Open(files[i]);

            TTree* tree = nullptr;
            file->GetObject("tree", tree);

            TString histName = Form("hist_%s_%d", variables[v], i);
            tree->Draw(Form("%s >> %s(100)", variables[v], histName.Data()), "", "goff");

            TH1F* h = (TH1F*)gDirectory->Get(histName);
            h->SetTitle(axisTitles[v]);
            h->SetLineColor(colors[i]);
            h->SetLineWidth(1);

            if (i == 0) {
                h->Draw("HIST");
            } else {
                h->Draw("HIST SAME");
            }

            leg->AddEntry(h, labels[i], "l");
        }

        leg->Draw();
    }

}

