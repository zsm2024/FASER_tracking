#include<iostream>
#include<vector>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <cmath>

struct track{
  double truthHitsRatio;
  float theta;
};

void rate(){
  
  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
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

  TFile* f1 = TFile::Open("total_3station_forward_seed_summary_circleFitTrackSeedTool.root","READ");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr;
  std::vector<uint64_t> *v4 = nullptr;
  std::vector<float> *v5 = nullptr;
  std::vector<float> *v6 = nullptr;


  t1->SetBranchAddress("t_truthHitRatio",&v1);
  t1->SetBranchAddress("nMajorityHits",&v2);
  t1->SetBranchAddress("truth_barcode",&v3);
  t1->SetBranchAddress("majorityParticleId",&v4);
  t1->SetBranchAddress("t_theta",&v5);
  t1->SetBranchAddress("truth_theta",&v6);

  //get the distribution of nDuplicates
  TH1I* h1 = new TH1I("h1","",5,-1,4);
  TH2F* h2 = new TH2F("h2",";truth_theta[rad];nDuplicates",10,0,0.0005,5,-1,4);

  //get the fake rate
  TEfficiency* fakeRate_vs_theta = new TEfficiency("eff",";theta[rad];fakeRate",10, 0,0.05);



  size_t entries = t1->GetEntries();
  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);
    std::map<uint64_t, std::vector<track>> mapParticleIdMatchedTracks;
    
   //loop over all seeds matched majorityParticle

    if(v5->size()==0)continue;
    for(size_t iSeed = 0; iSeed<v4->size(); ++iSeed){
      if(std::isnan(v5->at(iSeed)))continue;
      mapParticleIdMatchedTracks[v4->at(iSeed)].push_back({v1->at(iSeed),v5->at(iSeed)});
    }
    for(size_t iParticle = 0; iParticle<v3->size(); ++iParticle){

      auto ip = mapParticleIdMatchedTracks.find(v3->at(iParticle));
      if(ip != mapParticleIdMatchedTracks.end()){
        for(auto track : (*ip).second){
          bool isFake = (track.truthHitsRatio < 0.5);
          fakeRate_vs_theta->Fill(isFake,track.theta);
        }


        //find seed matched this truthParticle
        //then get the number of seed, calculating nDuplicates
        int nSeeds = (*ip).second.size();
        h1->Fill(nSeeds-1);
        h2->Fill(v6->at(iParticle),nSeeds-1);
      }
      //don't have seed matched this truthParticle;
      else{
        h1->Fill(-1);
        h2->Fill(v6->at(iParticle),-1);
      }
    }
  }

  h1->GetXaxis()->SetTitle("nDuplicates");
  h1->GetYaxis()->SetTitle("Events");

  std::cout<<"All good.Now plotting..."<<std::endl;
  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->Divide(2,2);
  cs->cd(1);
  h1->SetLineWidth(3);
  h1->Draw();

  cs->cd(2);
  h2->SetLineWidth(3);
  h2->Draw();
  cs->cd(3);
  fakeRate_vs_theta->SetLineWidth(3);
  fakeRate_vs_theta->Draw();

}
