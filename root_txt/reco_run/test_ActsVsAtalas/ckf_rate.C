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

void ckf_rate() {
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
      


    TFile* f1 = TFile::Open("3st_forwad_Ntuple.root", "READ");
    TFile* f2 = TFile::Open("total_3station_forward_seed_summary_circleFitTrackSeedTool.root", "READ");
    TTree* t1;
    TTree* t2;
    f1->GetObject("nt", t1);
    f2->GetObject("tree", t2);

    std::vector<double> *v1 = nullptr;
    std::vector<int> *v2 = nullptr;
    std::vector<double> *v3 = nullptr;
    std::vector<float>*v4 = nullptr;
    std::vector<bool>*v5 = nullptr;
    std::vector<int>*v6 = nullptr;


    t1->SetBranchAddress("t_truthHitRatio",&v1);
    t1->SetBranchAddress("t_barcode",&v2);
    t1->SetBranchAddress("truthParticleIsFiducial",&v5);
    t1->SetBranchAddress("t_barcode",&v6);

    t2->SetBranchAddress("truth_barcode",&v3);
    t2->SetBranchAddress("truth_theta",&v4);

    const Int_t nbins = 7;
    double bins[nbins + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.002, 0.004};
    TEfficiency* fakeRate_vs_theta = new TEfficiency("t1",";prod_theta[rad];fakeRate", nbins,bins);
    fakeRate_vs_theta->SetLineWidth(2);
    TH1I* h1 = new TH1I("h1",";nDuplicates;Events",5,-1,4);
    
    TH1I* h2 = new TH1I("h2",";truthBarcode;Events",10,0,10);


    size_t entries = t1->GetEntries();
    for(size_t ievent = 0; ievent<50; ievent++){
      t1->GetEntry(ievent);
      t2->GetEntry(ievent);
      std::cout<<"event = "<<ievent<<std::endl;
      for(size_t i=0; i<v6->size(); i++){
        //if(v5->at(i) == false)continue;
        std::cout<<v6->at(i);
        h2->Fill(v6->at(i));
      }
      std::cout<<std::endl;
      std::cout<<"-------------------------"<<std::endl;

      std::map<uint64_t, std::vector<track>> mapParticleIdMatchedTracks;

      //loop over all seeds matched majorityParticle
      for(size_t iSeed = 0; iSeed<v2->size(); ++iSeed){
        mapParticleIdMatchedTracks[v2->at(iSeed)].push_back({v1->at(iSeed)});
      }

      //loop over all particle
      for(size_t iParticle = 0; iParticle<v3->size(); ++iParticle){


        auto ip = mapParticleIdMatchedTracks.find(v3->at(iParticle));
        if(ip != mapParticleIdMatchedTracks.end()){
          for(auto track : (*ip).second){
            bool isFake = (track.truthHitsRatio < 0.5);
            fakeRate_vs_theta->Fill(isFake,v4->at(iParticle));
          }


          //find seed matched this truthParticle
          //then get the number of seed, calculating nDuplicates
          int nSeeds = (*ip).second.size();
          h1->Fill(nSeeds-1);
        }
        else{
          h1->Fill(-1);
        }
      }
    }
    TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 1200, 600);
    c1->Divide(2, 2);
    c1->cd(1);
    fakeRate_vs_theta->Draw();
    c1->cd(2);
    h1->Draw();
    c1->cd(3);
    h2->Draw();
}
