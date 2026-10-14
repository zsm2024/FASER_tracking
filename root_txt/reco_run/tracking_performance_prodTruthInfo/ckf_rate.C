#include<iostream>
#include<vector>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <cmath>

struct track{
  double truthHitRatio;
};



void ckf_rate(){

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
  gStyle->SetLineWidth(3);

  TFile* f1 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_track_summary_ckf.root","READ");
  TFile* f2 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root","READ");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  TTreeReader* Reader2 = new TTreeReader("tree",f2);
  TTree* t2 = Reader2->GetTree();
 
  std::vector<unsigned int> *v1 = nullptr;
  std::vector<unsigned int> *v2 = nullptr;
  std::vector<double> *v3 = nullptr;
  std::vector<uint64_t> *v4 = nullptr;
  std::vector<int> *v5 = nullptr;
  std::vector<float> *v6 = nullptr;


  t1->SetBranchAddress("nMeasurements",&v1);
  t1->SetBranchAddress("nMajorityHits",&v2);
  
  t2->SetBranchAddress("truth_barcode",&v3);
  t1->SetBranchAddress("majorityParticleId",&v4);
  t2->SetBranchAddress("truthParticle_Hits",&v5);
  t2->SetBranchAddress("truth_theta",&v6);


  t1->AddFriend(t2);
  
  //get the distribution of nDuplicates
  TH1I* h1 = new TH1I("h1",";nDuplicates;Events",5,-1,4);
  TH2F* h2 = new TH2F("h2",";prod_{#theta} [rad];nDuplicates",10,0,0.001,5,-1,4);

  //get the fake rate
  TEfficiency* fakeRate_vs_theta = new TEfficiency("eff",";prod_{#theta} [rad];fakeRate",10, 0,0.001);


  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;
  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);
    std::map<uint64_t, std::vector<track>> mapParticleIdMatchedTracks;


   //loop over all tracks matched majorityParticle    
    for(size_t iTrack = 0; iTrack<v4->size(); ++iTrack){

      double truthHitRatio = (v2->at(iTrack))*1.0/(v1->at(iTrack));
      mapParticleIdMatchedTracks[v4->at(iTrack)].push_back({truthHitRatio});

    }
    
    for(size_t iParticle = 0; iParticle<v3->size(); ++iParticle){

      //truthParticle_Hits >= 18 .
      if(v5->at(iParticle)<18)continue;

      //find the fake track(truthHitRatio<0.5) from matched tracks.
      auto ip = mapParticleIdMatchedTracks.find(v3->at(iParticle));
      if(ip != mapParticleIdMatchedTracks.end()){
        for(auto track : (*ip).second){
          bool isFake = (track.truthHitRatio < 0.5);
          fakeRate_vs_theta->Fill(isFake,v6->at(iParticle));
        }


        //find track matched this truthParticle
        //then get the number of track, calculating nDuplicates
        int nTracks = (*ip).second.size();
        
        h1->Fill(nTracks-1);
        h2->Fill(v6->at(iParticle),nTracks-1);
      }
      //don't have track matched this truthParticle;
      else{
        h1->Fill(-1);
        h2->Fill(v6->at(iParticle),-1);
      }
    }
  }


  std::cout<<"All good.Now plotting..."<<std::endl;
  TCanvas* cs = new TCanvas("c1","efficiency",1500,1200);
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
