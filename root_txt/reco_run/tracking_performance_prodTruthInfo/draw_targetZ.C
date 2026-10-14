void draw_targetZ() {

  gStyle->SetOptStat(0000);

  std::vector<std::string> filenames= {
    "total_3station_forward_seed_summary_circleFitTrackSeedTool.root",
    "total_3station_backward_seed_summary_circleFitTrackSeedTool.root",
   // "total_4station_backward_seed_summary_circleFitTrackSeedTool.root",
   // "total_4station_forward_seed_summary_circleFitTrackSeedTool.root"
  };

  const char* labels[2] = {
    "Forward",
    "Backward"
  };

    int colors[2] = {kRed, kBlue};

    TH1F* hists[2];

    TCanvas* c1 = new TCanvas("c1", "", 900, 600);

    for (int i = 0; i < filenames.size(); ++i) {
        TFile* file = TFile::Open(filenames[i].c_str());
        TTree* tree = (TTree*)file->Get("tree");

        TString histName = Form("hist_%d", i);
        hists[i] = new TH1F(histName, ";targetZ [mm];Entries", 100, -100, 2600);
        hists[i]->SetLineColor(colors[i]);
        hists[i]->SetLineWidth(2);


        if (i == 0) {
        //    hists[i]->Draw("HIST");
         
          //tree->Draw("var >> hist", "cut", "options").
          tree->Draw(Form("targetZPosition >> %s", histName.Data()), "", "HIST");
        } else {
            tree->Draw(Form("targetZPosition >> %s", histName.Data()), "", "HIST SAME");
            //hists[i]->Draw("HIST SAME");
        }

    }

    TLegend* legend = new TLegend(0.65, 0.68, 0.88, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    for (int i = 0; i < filenames.size(); ++i) {
      legend->AddEntry(hists[i], labels[i], "l");
    }
    legend->Draw();

}

