#include <cmath>
#include <TEfficiency.h>

void trackSeparation1() {
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

  
    TFile* file_3st_forward = TFile::Open("total_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* file_3st_backward = TFile::Open("total_3station_backward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* file_4st_forward = TFile::Open("total_4station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* file_4st_backward = TFile::Open("total_4station_backward_seed_summary_circleFitTrackSeedTool.root", "read");

    if (!file_3st_forward || !file_3st_backward || !file_4st_forward || !file_4st_backward) {
        std::cerr << "Error: Cannot open one or more ROOT files!" << std::endl;
        return;
    }

    
    const Int_t nbins = 15;
    double bins[nbins + 1] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 100, 200};

  
    TEfficiency* eff_3st_forward = new TEfficiency("eff_3st_forward", ";#DeltaR1 [mm]; Seeding Efficiency", nbins, bins);
    TEfficiency* eff_3st_backward = new TEfficiency("eff_3st_backward", ";#DeltaR1 [mm]; Seeding Efficiency", nbins, bins);
    TEfficiency* eff_4st_forward = new TEfficiency("eff_4st_forward", ";#DeltaR1 [mm]; Seeding Efficiency", nbins, bins);
    TEfficiency* eff_4st_backward = new TEfficiency("eff_4st_backward", ";#DeltaR1 [mm]; Seeding Efficiency", nbins, bins);

    
    auto process_file = [](TFile* file, TEfficiency* eff, int marker, int color, int size) {
        TTree* t1 = (TTree*)file->Get("tree");
        if (!t1) {
            std::cerr << "Error: Tree not found in file " << file->GetName() << std::endl;
            return;
        }

        uint32_t nSeeds = 0;
        std::vector<double> *v1 = nullptr;
        std::vector<double> *v2 = nullptr;
        std::vector<double> *v3 = nullptr;
        std::vector<double> *v4 = nullptr;


        t1->SetBranchAddress("nSeeds", &nSeeds);
        t1->SetBranchAddress("truthd0_x", &v1);
        t1->SetBranchAddress("truthd0_y", &v2);
        t1->SetBranchAddress("truthd1_x", &v3);
        t1->SetBranchAddress("truthd1_y", &v4);

        size_t entries = t1->GetEntries();
        std::cout << "Processing file: " << file->GetName() << " | Entries: " << entries << std::endl;

        for (size_t ievent = 0; ievent < entries; ievent++) {
            t1->GetEntry(ievent);

            if (v1->empty() || v3->empty()) continue;

            bool passSeed = false;
            double x1 = v1->at(3), y1 = v2->at(3);
            double x2 = v3->at(3), y2 = v4->at(3);
            double deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

            if (nSeeds >= 2) passSeed = true;
            eff->Fill(passSeed, deltaR);
        }
      eff->SetMarkerStyle(marker);
      eff->SetMarkerSize(1.5);
      eff->SetLineWidth(size);
      eff->SetMarkerColor(color);
      eff->SetLineColor(color);
    };

    process_file(file_3st_forward, eff_3st_forward, 20, kRed, 2);
    process_file(file_3st_backward, eff_3st_backward, 21, kBlue, 3);
    process_file(file_4st_forward, eff_4st_forward, 22, kGreen, 1);
    process_file(file_4st_backward, eff_4st_backward, 23, kMagenta, 2);

    TCanvas* cs = new TCanvas("c1", "Efficiency Comparison", 1500, 600);

    eff_3st_forward->Draw("AP");
    gPad->Update();
    eff_3st_forward->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.8, 1.05);
    eff_3st_backward->Draw("P SAME");
    eff_4st_forward->Draw("P SAME");
    eff_4st_backward->Draw("P SAME");

    TLegend* leg = new TLegend(0.7, 0.2, 0.9, 0.4);
    leg->AddEntry(eff_3st_forward, "3-Station Forward", "p");
    leg->AddEntry(eff_3st_backward, "3-Station Backward", "p");
    leg->AddEntry(eff_4st_forward, "4-Station Forward", "p");
    leg->AddEntry(eff_4st_backward, "4-Station Backward", "p");
    leg->Draw();

    cs->SaveAs("seeding_trackSeparation.png");

    file_3st_forward->Close();
    file_3st_backward->Close();
    file_4st_forward->Close();
    file_4st_backward->Close();
}

