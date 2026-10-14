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
  float theta;
};


void rate() {
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
    gStyle->SetLineWidth(4);

    std::vector<std::string> filenames = {
        "total1_3station_forward_seed_summary_circleFitTrackSeedTool.root",
        "total1_3station_backward_seed_summary_circleFitTrackSeedTool.root",
        "total1_4station_forward_seed_summary_circleFitTrackSeedTool.root",
        "total1_4station_backward_seed_summary_circleFitTrackSeedTool.root"
    }; 
    std::vector<int> colors = {kRed, kBlue, kGreen+2, kMagenta};
    std::vector<std::string> labels = {"3station Forward", "3station Backward", "4station Forward", "4station Backward"};

    
    TLegend* leg = new TLegend(0.6, 0.2, 0.9, 0.4);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.03);
    
    TLegend* leg1 = new TLegend(0.6, 0.2, 0.9, 0.4);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);

    TH1I* h[4];
    TEfficiency* fakeRate_vs_theta[4];

    for (size_t i = 0; i < filenames.size(); i++) {
        TFile* f = TFile::Open(filenames[i].c_str(), "READ");
        if (!f || f->IsZombie()) {
            std::cerr << "Error opening file: " << filenames[i] << std::endl;
            continue;
        }
        TTree* t;
        f->GetObject("tree", t);
        if (!t) {
            std::cerr << "Error: Tree not found in " << filenames[i] << std::endl;
            continue;
        }

        std::vector<double>* v1 = nullptr;
        std::vector<uint64_t>* v4 = nullptr;
        std::vector<float>* v5 = nullptr;
        std::vector<float>* v6 = nullptr;
        std::vector<double>* v3 = nullptr;
        
        t->SetBranchAddress("t_truthHitRatio", &v1);
        t->SetBranchAddress("majorityParticleId", &v4);
        t->SetBranchAddress("t_theta", &v5);
        t->SetBranchAddress("truth_theta", &v6);
        t->SetBranchAddress("truth_barcode", &v3);
        
        fakeRate_vs_theta[i] = new TEfficiency(Form("eff_%lu", i), ";theta [rad]; Fake Rate", 10, 0, 0.05);
        fakeRate_vs_theta[i]->SetLineColor(colors[i]);
        fakeRate_vs_theta[i]->SetMarkerColor(colors[i]);
        fakeRate_vs_theta[i]->SetLineWidth(4);

        h[i] = new TH1I(Form("h_%lu", i), ";nDuplicates; Events", 5, -1, 4);
        h[i]->SetLineColor(colors[i]);
        h[i]->SetLineWidth(4);
        leg->AddEntry(fakeRate_vs_theta[i], labels[i].c_str(), "lep");
        leg1->AddEntry(h[i], labels[i].c_str(), "lep");

        size_t entries = t->GetEntries();
        for (size_t ievent = 0; ievent < entries; ievent++) {
            t->GetEntry(ievent);
            std::map<uint64_t, std::vector<track>> mapParticleIdMatchedTracks;
            
            if (v5->size() == 0) continue;
            for (size_t iSeed = 0; iSeed < v4->size(); ++iSeed) {
                if (std::isnan(v5->at(iSeed))) continue;
                //bool isFake = (v1->at(iSeed) < 0.5);
                //fakeRate_vs_theta[i]->Fill(isFake, v5->at(iSeed));
                //mapParticleIdMatchedTracks[v4->at(iSeed)]++;
                mapParticleIdMatchedTracks[v4->at(iSeed)].push_back({v1->at(iSeed),v5->at(iSeed)});
            }

            for (size_t iParticle = 0; iParticle < v3->size(); ++iParticle) {
               // int nSeeds = mapParticleIdMatchedTracks[v3->at(iParticle)];
               // h[i]->Fill(nSeeds - 1);
              auto ip = mapParticleIdMatchedTracks.find(v3->at(iParticle));
              if(ip != mapParticleIdMatchedTracks.end()){
                for(auto track : (*ip).second){
                  bool isFake = (track.truthHitsRatio < 0.5);
                  fakeRate_vs_theta[i]->Fill(isFake,track.theta);
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
    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 1200, 600);
    c1->Divide(1, 2);
    c1->cd(1);
    for (int i = 0; i < 4; i++) {
        if (i == 0) fakeRate_vs_theta[i]->Draw("AP");
        else fakeRate_vs_theta[i]->Draw("P SAME");
    }
    leg->Draw();
    
    c1->cd(2);
    for (int i = 0; i < 4; i++) {
        if (i == 0) h[i]->Draw();
        else h[i]->Draw("same");
    }
    leg1->Draw();
}

