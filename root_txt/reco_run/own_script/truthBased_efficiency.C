#include <iostream>
#include <tuple>
#include <vector>

#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>

struct ParticleInfo{
float theta;
float pT;

};

void truthBased_efficiency(){
  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetLineWidth(4);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);

  TFile* file1 = TFile::Open("total1_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();
  const Int_t nbins1 = 13;
  double bins1[nbins1 + 1] = {0, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.0175, 0.02, 0.0225, 0.025, 0.03, 0.04, 0.05};
  const Int_t nbins2 = 14;
  double bins2[nbins2 + 1] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.5};
  TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";theta[rad];Seeding Efficiency",nbins1, bins1);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";pT[Gev/c];Seeding Efficiency",nbins2, bins2);
 // TH1F* h1 = new TH1F("h1",";theta[rad];Events",10,0,0.1);
  //TH1F* h2 = new TH1F("h2",";pT[Gev/c];Events",10,0,2);

  TEfficiency* eff1 = new TEfficiency("eff",";theta[rad];Seeding Efficiency",nbins1, bins1);
  TEfficiency* eff2 = new TEfficiency("eff",";pT[Gev/c];Seeding Efficiency",nbins2, bins2);

  std::vector<uint64_t> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;

  std::vector<double> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;


  //All particles  
  t1->SetBranchAddress("truth_barcode",&v1);
  t1->SetBranchAddress("truth_theta",&v2);
  t1->SetBranchAddress("truth_pT",&v7);
  t1->SetBranchAddress("truthParticle_Hits",&v6);

  //All reco tracks
  t1->SetBranchAddress("majorityParticleId",&v3);
  t1->SetBranchAddress("t_truthHitRatio", &v4);
  t1->SetBranchAddress("nMeasurements", &v5);


  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;


  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);
    //store truthParticleBarcode into vector;
    std::vector<int> particleIdWithTracks; //majorityParticleId
    std::map<uint64_t,ParticleInfo>particles;
   
    // loop over all tracks
    for(int iTrack = 0; iTrack < v3->size(); ++iTrack){
  
      // set limit conditions(truthHitRatio)
//      if((v4->at(iTrack)) < 0.6) continue;
      //if((v5->at(iTrack)) < 12)continue;
      particleIdWithTracks.push_back(v3->at(iTrack));
    }


    // loop over all particles 
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){
      

      //Fill TEfficiency
      bool hasTrack = false;
      if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), v1->at(iparticle)) != particleIdWithTracks.end()){ 
        hasTrack = true;
      }
      trackEff_vs_theta->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT->Fill(hasTrack,v7->at(iparticle));

      if(v6->at(iparticle)<18) continue;
      particles[v1->at(iparticle)] = {v2->at(iparticle), v7->at(iparticle)};
    }
  
    
    for(const auto& [key,value] : particles){
     
      //Fill TEfficiency(limit condition)
      bool hasTrack = false;
      if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), key) != particleIdWithTracks.end()){ 
        hasTrack = true;
      }
      eff1->Fill(hasTrack,value.theta);
      eff2->Fill(hasTrack,value.pT);
    }
    //std::cout<<particles.size()<<"; "<<particleIdWithTracks.size()<<std::endl;
  }



  
  std::cout<<"All good.Now plotting..."<<std::endl;
  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->Divide(2,2);
  cs->cd(2);
  trackEff_vs_theta->SetLineWidth(3);
  trackEff_vs_theta->Draw();
  cs->cd(1);
  eff1->SetLineWidth(3);
  eff1->Draw();
  cs->cd(4);
  trackEff_vs_pT->SetLineWidth(3);
  trackEff_vs_pT->Draw();
  cs->cd(3);
  eff2->SetLineWidth(3);
  eff2->Draw();

  //cs->SaveAs("truthBased.pdf");
}
