void draw_variable_comparison(const char* varName, const char* title, const char* hist_title, const char* filename_suffix) {
    

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

    TString canvasName = Form("c_%s", varName);
    TCanvas* c1 = new TCanvas(canvasName, title, 800, 600);
    c1->SetGrid(0, 0);
    c1->SetTicks(1, 1);

    TLegend* leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.04);

    TH1F* hists[3];

    for (int i = 0; i < 3; ++i) {
        TFile* file = TFile::Open(files[i]);
        TTree* tree = nullptr;
        file->GetObject("tree", tree);

        TString histName = Form("hist_%s_%d", varName, i);

        
        //tree->Draw("var >> hist","cut","options"). "var >> hist(100): means the number of bin is 100" and of course we can make var >> hist(100,0,50)
        tree->Draw(Form("%s >> %s(100)", varName, histName.Data()), "", "goff");

        hists[i] = (TH1F*)gDirectory->Get(histName);
        hists[i]->SetTitle(hist_title);

        hists[i]->SetLineColor(colors[i]);
        hists[i]->SetLineWidth(2);

        if (i == 0) {
            hists[i]->Draw("HIST");
        } else {
            hists[i]->Draw("HIST SAME");
        }

        leg->AddEntry(hists[i], labels[i], "l");
    }

    leg->Draw();
}


void draw_varInfo() {
    draw_variable_comparison("truth_nHits_st1_l0", "", ";truthHits_st1_l0;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st1_l1", "", ";truthHits_st1_l1;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st1_l2", "", ";truthHits_st1_l2;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st2_l0", "", ";truthHits_st2_l0;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st2_l1", "", ";truthHits_st2_l1;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st2_l2", "", ";truthHits_st2_l2;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st3_l0", "", ";truthHits_st3_l0;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st3_l1", "", ";truthHits_st3_l1;Events",         "Hits_comparison");
    draw_variable_comparison("truth_nHits_st3_l2", "", ";truthHits_st3_l2;Events",         "Hits_comparison");
    //draw_variable_comparison("nSegments", "", ";segments;Events",         "segments_comparison");
   // draw_variable_comparison("truth_theta", "",     ";prod_theta [rad];Events",   "truth_theta_comparison");
    //draw_variable_comparison("truth_pT", "",   ";prod_P_{T} [Mev/c];Events",      "truth_P_{T}_comparison");
}

