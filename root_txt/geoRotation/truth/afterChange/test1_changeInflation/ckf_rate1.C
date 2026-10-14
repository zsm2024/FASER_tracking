#include<iostream>
#include<vector>
#include<map>
#include<TCanvas.h>
#include<TEfficiency.h>
#include<TFile.h>
#include<TTree.h>
#include<TStyle.h>
#include<TH1I.h>
#include<TH2F.h>
#include<TLegend.h>
#include<cmath>
struct track{
  double truthHitsRatio;
};

void ckf_rate1() {
    gStyle->SetOptFit(0011);
    gStyle->SetOptStat(0000);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetTitleSize(0.05, "xy");
    gStyle->SetLabelSize(0.04, "xy");
    gStyle->SetTitleOffset(1.2, "x");
    gStyle->SetTitleOffset(1.2, "y");
    gStyle->SetNdivisions(510, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetLineWidth(2);

    std::vector<std::string> filenames1 = {
        //"total_3station_forward_track_summary_ckf.root",
        //"total_3station_backward_track_summary_ckf.root",
        "9000_3station_forward_track_summary_ckf.root",
        "9000_3station_backward_track_summary_ckf.root",
        //"total_4station_forward_track_summary_ckf.root",
        //"total_4station_backward_track_summary_ckf.root"
    };
    std::vector<std::string> filenames2 = {
        //"total_3station_forward_seed_summary_circleFitTrackSeedTool.root",
        //"total_3station_backward_seed_summary_circleFitTrackSeedTool.root",
        "9000_3station_forward_seed_summary_circleFitTrackSeedTool.root",
        "9000_3station_forward_seed_summary_circleFitTrackSeedTool.root",
        //"total_4station_forward_seed_summary_circleFitTrackSeedTool.root",
        //"total_4station_backward_seed_summary_circleFitTrackSeedTool.root"
    };

    std::vector<int> colors = {kRed, kBlue};
    std::vector<std::string> labels = {"Forward", "Backward"};


    TLegend* leg = new TLegend(0.6, 0.2, 0.9, 0.4);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.03);

    TLegend* leg1 = new TLegend(0.6, 0.2, 0.9, 0.4);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);

    TH1I* h[2];
    TEfficiency* fakeRate_vs_theta[2];
    
    const Int_t nbins1 = 7;
    double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.002, 0.004};

    for (size_t i = 0; i < filenames1.size(); i++) {
      
      TFile* f1 = TFile::Open(filenames1[i].c_str(), "READ");
      TFile* f2 = TFile::Open(filenames2[i].c_str(), "READ");
      TTree* t1;
      TTree* t2;
      f1->GetObject("tree", t1);
      f2->GetObject("tree", t2);

      std::vector<unsigned int> *v1 = nullptr;
      std::vector<unsigned int> *v2 = nullptr;
      std::vector<double> *v3 = nullptr;
      std::vector<uint64_t> *v4 = nullptr;
      std::vector<float> *v5 = nullptr;
      std::vector<float> *v6 = nullptr;
      std::vector<int> *v7 = nullptr;


      t1->SetBranchAddress("nMeasurements",&v1);
      t1->SetBranchAddress("nMajorityHits",&v2);

      t2->SetBranchAddress("truth_barcode",&v3);
      t1->SetBranchAddress("majorityParticleId",&v4);
      t2->SetBranchAddress("truth_theta",&v6);
      t2->SetBranchAddress("truthParticle_Hits",&v7);



      fakeRate_vs_theta[i] = new TEfficiency(Form("eff_%lu", i), ";prod_{#theta} [rad]; Fake Rate", 10, 1.5705, 1.571);
      fakeRate_vs_theta[i]->SetLineColor(colors[i]);
      fakeRate_vs_theta[i]->SetMarkerColor(colors[i]);
      fakeRate_vs_theta[i]->SetMarkerStyle(20);
      fakeRate_vs_theta[i]->SetMarkerSize(0.8);
      fakeRate_vs_theta[i]->SetLineWidth(2);

      h[i] = new TH1I(Form("h_%lu", i), ";nDuplicates; Events", 5, -1, 4);
      h[i]->SetLineColor(colors[i]);
      h[i]->SetLineWidth(2);
      leg->AddEntry(fakeRate_vs_theta[i], labels[i].c_str(), "lep");
      leg1->AddEntry(h[i], labels[i].c_str(), "lep");

      size_t entries = t1->GetEntries();
      for (size_t ievent = 0; ievent < entries; ievent++) {
        t1->GetEntry(ievent);
        t2->GetEntry(ievent);

        std::map<uint64_t, std::vector<track>> mapParticleIdMatchedTracks;

        for(size_t iTrack = 0; iTrack<v4->size(); ++iTrack){

          double truthHitRatio = (v2->at(iTrack))*1.0/(v1->at(iTrack));
          mapParticleIdMatchedTracks[v4->at(iTrack)].push_back({truthHitRatio});
           
        }

        for(size_t iParticle = 0; iParticle<v3->size(); ++iParticle){
          
          
          if(v7->at(iParticle) < 18)continue;

          auto ip = mapParticleIdMatchedTracks.find(v3->at(iParticle));
          if(ip != mapParticleIdMatchedTracks.end()){
            for(auto track : (*ip).second){
              bool isFake = (track.truthHitsRatio < 0.5);
              fakeRate_vs_theta[i]->Fill(isFake,v6->at(iParticle));
            }


            //find seed matched this truthParticle
            //then get the number of seed, calculating nDuplicates
            int nSeeds = (*ip).second.size();
            h[i]->Fill(nSeeds-1);
          }
          //don't have seed matched this truthParticle;
          else{
            h[i]->Fill(-1);
          }
        } 
      }
    }


    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 1500, 500);
    c1->Divide(2, 1);
    c1->cd(1);
    for (int i = 0; i < 2; i++) {
        if (i == 0) fakeRate_vs_theta[i]->Draw("AP");
        else fakeRate_vs_theta[i]->Draw("P SAME");

    }
    
    leg->Draw();

    c1->cd(2);
    for (int i = 0; i < 2; i++) {
        if (i == 0) h[i]->Draw();
        else h[i]->Draw("same");
    }
    leg1->Draw();
}
