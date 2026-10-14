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
        "total_3station_forward_track_summary_ckf.root",
        "total_3station_backward_track_summary_ckf.root",
        //"total_4station_forward_track_summary_ckf.root",
        //"total_4station_backward_track_summary_ckf.root"
    };
    std::vector<std::string> filenames2 = {
        "total_3station_forward_seed_summary_circleFitTrackSeedTool.root",
        "total_3station_backward_seed_summary_circleFitTrackSeedTool.root",
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



      fakeRate_vs_theta[i] = new TEfficiency(Form("eff_%lu", i), ";prod_{#theta} [rad]; Fake Rate", nbins1, bins1);
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

    auto processFile_fakeRate = [&](const char* filename,
                                TH1I* h1,
                                TH2F* h2,
                                TEfficiency* fakeRate
                                ) {

      TFile* f = TFile::Open(filename,"READ");
      if(!f || f->IsZombie()){
          std::cerr << "Error opening file: " << filename << std::endl;
          return;
      }

      TTreeReader* Reader = new TTreeReader("nt",f);
      TTree* t = Reader->GetTree();

      std::vector<double>* v1 = nullptr;     // truthHitRatio
      std::vector<double>* v3 = nullptr;     // truth_barcode
      std::vector<uint64_t>* v4 = nullptr;   // majorityParticleId
      std::vector<int>* v5 = nullptr;        // truthParticle_Hits
      std::vector<float>* v6 = nullptr;      // truth_theta

      t->SetBranchAddress("t_truthHitRatio",&v1);
      t->SetBranchAddress("truth_barcode",&v3);
      t->SetBranchAddress("t_barcode",&v4);
      //t->SetBranchAddress("majorityParticleId",&v4);
      t->SetBranchAddress("truthParticle_Hits",&v5);
      t->SetBranchAddress("truth_theta",&v6);

      size_t entries = t->GetEntries();
      std::cout << "events in " << filename << ": " << entries << std::endl;

      for(size_t ievent = 0; ievent < entries; ievent++){
          t->GetEntry(ievent);

          std::map<uint64_t, std::vector<track>> mapParticleIdMatchedTracks;

          // loop over all seeds matched majorityParticle
          for(size_t iSeed = 0; iSeed < v4->size(); ++iSeed){
              mapParticleIdMatchedTracks[v4->at(iSeed)].push_back({v1->at(iSeed)});
          }

          // loop over all truth particles
          for(size_t iParticle = 0; iParticle < v3->size(); ++iParticle){

              if(v5->at(iParticle) < 18) continue;

              auto ip = mapParticleIdMatchedTracks.find(v3->at(iParticle));
              if(ip != mapParticleIdMatchedTracks.end()){
                  for(auto& trk : (*ip).second){
                      bool isFake = (trk.truthHitsRatio < 0.5);
                      fakeRate->Fill(isFake, v6->at(iParticle));
                  }

                  // number of seeds matched this truthParticle
                  int nSeeds = (*ip).second.size();
                  h1->Fill(nSeeds - 1);
                  h2->Fill(v6->at(iParticle), nSeeds - 1);
              }
              else {
                  h1->Fill(-1);
                  h2->Fill(v6->at(iParticle), -1);
              }
          }
      }

      f->Close();
    };

    //define hist and efficiency
    TH1I* h1_way1 = new TH1I("h1_way1",";nDuplicates;Events", 5, -1, 4);
    TH2F* h2_way1 = new TH2F("h2_way1",";prod_{#theta} [rad];nDuplicates", nbins1, bins1, 5, -1, 4);
    TEfficiency* fakeRate_way1 = new TEfficiency("eff_way1",";prod_{#theta} [rad];fakeRate", nbins1, bins1);
    
    TH1I* h1_way2 = new TH1I("h1_way2",";nDuplicates;Events", 5, -1, 4);
    TH2F* h2_way2 = new TH2F("h2_way2",";prod_{#theta} [rad];nDuplicates", nbins1, bins1, 5, -1, 4);
    TEfficiency* fakeRate_way2 = new TEfficiency("eff_way2",";prod_{#theta} [rad];fakeRate", nbins1, bins1);
    
    //deal with merge Tracks
    processFile_fakeRate("way1-MergeTracks-4st-PHYS.root", h1_way1, h2_way1, fakeRate_way1);
    processFile_fakeRate("way2-MergeTracks-4st-PHYS.root", h1_way2, h2_way2, fakeRate_way2);


    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 1500, 500);
    c1->Divide(2, 1);
    c1->cd(1);
    for (int i = 0; i < 2; i++) {
        if (i == 0) fakeRate_vs_theta[i]->Draw("AP");
        else fakeRate_vs_theta[i]->Draw("P SAME");

    }
    
    fakeRate_way1->SetLineColor(kGreen);
    fakeRate_way1->SetMarkerColor(kGreen);
    fakeRate_way1->SetMarkerStyle(20);
    fakeRate_way1->SetMarkerSize(0.8);
    fakeRate_way1->SetLineWidth(2);
    fakeRate_way1->Draw("same");
    leg->AddEntry(fakeRate_way1,"Merged", "lep");
    
    //fakeRate_way2->SetLineColor(kOrange);
    //fakeRate_way2->SetMarkerColor(kOrange);
    //fakeRate_way2->SetMarkerStyle(20);
    //fakeRate_way2->SetMarkerSize(0.8);
    //fakeRate_way2->SetLineWidth(2);
    //fakeRate_way2->Draw("same");
    //leg->AddEntry(fakeRate_way2,"merge using method2", "lep");
    leg->Draw();

    c1->cd(2);
    for (int i = 0; i < 2; i++) {
        if (i == 0) h[i]->Draw();
        else h[i]->Draw("same");
    }
    h1_way1->SetLineColor(kGreen);
    h1_way1->SetLineWidth(2);
    h1_way1->Draw("same");
    leg1->AddEntry(h1_way1, "Merged", "lep");
    //h1_way2->SetLineColor(kOrange);
    //h1_way2->SetLineWidth(2);
    //h1_way2->Draw("same");
    //leg1->AddEntry(h1_way2, "merge using method2", "lep");
    leg1->Draw();
}
