#include <cmath>
#include <TEfficiency.h>

void residual1() {
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


    TFile* file_3st_forward = TFile::Open("total1_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* file_3st_backward = TFile::Open("total1_3station_backward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* file_4st_forward = TFile::Open("total1_4station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    TFile* file_4st_backward = TFile::Open("total1_4station_backward_seed_summary_circleFitTrackSeedTool.root", "read");

    if (!file_3st_forward || !file_3st_backward || !file_4st_forward || !file_4st_backward) {
        std::cerr << "Error: Cannot open one or more ROOT files!" << std::endl;
        return;
    }



    TH1D* h1_3st_forward = new TH1D("h1_3st_forward",";QoverP;Events",50,-0.1,0.1);
    TH1D* h1_3st_backward = new TH1D("h1_3st_backward",";QoverP;Events",50,-0.1,0.1);
    TH1D* h1_4st_forward = new TH1D("h1_4st_forward",";QoverP;Events",50,-0.1,0.1);
    TH1D* h1_4st_backward = new TH1D("h1_4st_backward",";QoverP;Events",50,-0.1,0.1);


    auto process_file = [](TFile* file, TH1D* h1, int color, int size) {
      TTree* t1 = (TTree*)file->Get("tree");
      if (!t1) {
          std::cerr << "Error: Tree not found in file " << file->GetName() << std::endl;
          return;
      }
      
      std::vector<double> *v1 = nullptr;
      std::vector<double> *v2 = nullptr;

      t1->SetBranchAddress("track_QoverP", &v1);
      t1->SetBranchAddress("t_QoverP", &v2);

      size_t entries = t1->GetEntries();
      for(size_t ievent = 0; ievent<entries; ++ievent){
        t1->GetEntry(ievent);
        if (v1->size() == v2->size() && v1->size() > 0){
          for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
            h1->Fill((v2->at(iTrack))-(v1->at(iTrack)));
          }
        }
        else{
          continue;
        }
      }

      TF1 *fit = new TF1("f1","gaus",-0.1,0.1);
      fit->SetLineWidth(2);
      h1->Fit(fit,"R0");
      double mean = fit->GetParameter(1);
      double meanError = fit->GetParError(1);
      double sigma = fit->GetParameter(2);
      double sigmaError = fit->GetParError(2);
      h1->SetLineColor(color);
      h1->SetLineWidth(size);
    };
    process_file(file_3st_forward,h1_3st_forward,kRed,2);
    process_file(file_3st_backward,h1_3st_backward,kBlue,3);
    process_file(file_4st_forward,h1_4st_forward,kGreen,1);
    process_file(file_4st_backward,h1_4st_backward,kMagenta,2);

    TCanvas* c1 = new TCanvas("c1","",600,500);
    h1_3st_forward->Draw();
    //gPad->Update();
   // eff_3st_forward->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.8, 1.05);
    h1_3st_backward->Draw("SAME");
    h1_4st_forward->Draw("SAME");
    h1_4st_backward->Draw("SAME");


    TLegend* leg = new TLegend(0.7, 0.2, 0.9, 0.4);
    leg->AddEntry(h1_3st_forward, "3-Station Forward", "l");
    leg->AddEntry(h1_3st_backward, "3-Station Backward", "l");
    leg->AddEntry(h1_4st_forward, "4-Station Forward", "l");
    leg->AddEntry(h1_4st_backward, "4-Station Backward", "l");
    leg->Draw();

    c1->SaveAs("seeding_residual_QoverP.png");

    file_3st_forward->Close();
    file_3st_backward->Close();
    file_4st_forward->Close();
    file_4st_backward->Close();
}
