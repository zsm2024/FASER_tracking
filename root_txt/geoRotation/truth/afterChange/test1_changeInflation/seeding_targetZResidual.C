#include <cmath>
#include <TEfficiency.h>

void seeding_targetZResidual() {
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(0000);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetTitleSize(0.05, "xy");
    gStyle->SetLineWidth(2);
    gStyle->SetLabelSize(0.04, "xy");
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.9, "y");
    gStyle->SetNdivisions(510, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);


    TFile* file_3st_forward = TFile::Open("9000_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    //TFile* file_3st_backward = TFile::Open("total_3station_backward_seed_summary_circleFitTrackSeedTool.root", "read");
    //TFile* file_4st_forward = TFile::Open("total_4station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
    //TFile* file_4st_backward = TFile::Open("total_4station_backward_seed_summary_circleFitTrackSeedTool.root", "read");

    //if (!file_3st_forward || !file_3st_backward || !file_4st_forward || !file_4st_backward) {
    //    std::cerr << "Error: Cannot open one or more ROOT files!" << std::endl;
    //    return;
    //}



    TH1D* st3_forward = new TH1D("st3_forward",";theta[rad];Events",50,-0.004,0.004);
    TH1D* st3_backward = new TH1D("st3_backward",";theta[rad];Events",50,-0.1,0.1);
    TH1D* st4_forward = new TH1D("st4_forward",";phi[rad];Events",50,-6,6);
    TH1D* st4_backward = new TH1D("st4_backward",";phi[rad];Events",50,-6,6);
    std::vector<TH1D*> v1 = {st3_forward,st3_backward,st4_forward,st4_backward};
    std::vector<TLegend*>v2;
    for(int j=0; j<2; j++){
      TLegend* leg = new TLegend(0.7, 0.2, 0.9, 0.4);
      v2.push_back(leg);
    }


    auto process_file = [](TFile* file, TH1D* h1, int size) {
      TTree* t1 = (TTree*)file->Get("tree");
      
      std::vector<double> *v1 = nullptr;
      std::vector<double> *v2 = nullptr;

      t1->SetBranchAddress("track_theta", &v1);
      t1->SetBranchAddress("t_theta", &v2);

      size_t entries = t1->GetEntries();
      for(size_t ievent = 0; ievent<entries; ++ievent){
        t1->GetEntry(ievent);
          for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
            if(std::isnan(v1->at(iTrack))||std::isnan(v2->at(iTrack))){
              continue;
            }
            h1->Fill((v2->at(iTrack))-(v1->at(iTrack)));
          }
      }

      TF1 *fit = new TF1("f1","gaus",-0.004,0.004);
      fit->SetLineWidth(2);
      h1->Fit(fit,"R");
      double mean = fit->GetParameter(1);
      double meanError = fit->GetParError(1);
      double sigma = fit->GetParameter(2);
      double sigmaError = fit->GetParError(2);
      //h1->SetLineColor(color);
      h1->SetLineWidth(size);
    };


    process_file(file_3st_forward,st3_forward,2);
    //process_file(file_3st_backward,st3_backward,2);

    TCanvas* c1 = new TCanvas("c1","",1500,1200);
    c1->Divide(2,2);
    for(int i = 0; i<1; i++){
      c1->cd(i+1);
      v1.at(i)->SetMinimum(0);
      v1.at(i)->SetMaximum(6000);
      v1.at(i)->Draw();
      v2.at(i)->AddEntry(v1.at(i),v1.at(i)->GetName(),"l");
      v2.at(i)->SetBorderSize(0);
      v2.at(i)->Draw();
    }
   // h1_3st_forward->Draw();
   // h1_3st_backward->Draw("SAME");
   // h1_4st_forward->Draw("SAME");
   // h1_4st_backward->Draw("SAME");


    //TLegend* leg = new TLegend(0.7, 0.2, 0.9, 0.4);
    //leg->AddEntry(h1_3st_forward, "3-Station Forward", "l");
    //leg->AddEntry(h1_3st_backward, "3-Station Backward", "l");
    //leg->AddEntry(h1_4st_forward, "4-Station Forward", "l");
    //leg->AddEntry(h1_4st_backward, "4-Station Backward", "l");
    //leg->Draw();
    
   // TLegend* leg2 = new TLegend(0.15, 0.5, 0.55, 0.87);
   // leg2->AddEntry((TObject*)0, "Fit Results:", "");
   // leg2->AddEntry((TObject*)0, Form("3st-F: #mu=%.1f#pm%.1f, #sigma=%.1f#pm%.1f", mean1, meanErr1, sigma1, sigmaErr1), "");
   // leg2->AddEntry((TObject*)0, Form("3st-B: #mu=%.1f#pm%.1f, #sigma=%.1f#pm%.1f", mean2, meanErr2, sigma2, sigmaErr2), "");
   // leg2->AddEntry((TObject*)0, Form("4st-F: #mu=%.1f#pm%.1f, #sigma=%.1f#pm%.1f", mean3, meanErr3, sigma3, sigmaErr3), "");
   // leg2->AddEntry((TObject*)0, Form("4st-B: #mu=%.1f#pm%.1f, #sigma=%.1f#pm%.1f", mean4, meanErr4, sigma4, sigmaErr4), "");
   // leg2->SetTextFont(42);
   // leg2->SetTextSize(0.01);
   // leg2->SetBorderSize(0);
   // leg2->Draw();
    
    c1->SaveAs("seeding_residual_phi.pdf");

    file_3st_forward->Close();
    //file_3st_backward->Close();
    //file_4st_forward->Close();
    //file_4st_backward->Close();
}
