#include <iostream>
#include <tuple>
#include <vector>

#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>

struct ParticleInfo{
double id;
double theta;
//double phi;
//double pT;
//double p;

};

void truthBased_efficiency(){
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

  TFile* file1 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");
  TFile* file2 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_track_summary_ckf.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();
  TTreeReader* Reader2 = new TTreeReader("tree",file2);
  TTree* t2 = Reader2->GetTree();


  t1->AddFriend(t2);

  TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";theta[rad];Tracking Efficiency",10, 0,0.0005);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";pT[Gev/c];Tracking Efficiency",10, 0,1);


  std::vector<float> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;

  std::vector<unsigned int> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<bool> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;


  //All particles
  t1->SetBranchAddress("truth_barcode",&v1);
  t1->SetBranchAddress("truth_theta",&v2);
  t1->SetBranchAddress("truth_pT",&v7);
  t1->SetBranchAddress("truth_isFiducial",&v6);

  //All reco tracks
  t2->SetBranchAddress("majorityParticleId",&v3);
  t2->SetBranchAddress("nMajorityHits", &v4);
  t2->SetBranchAddress("nMeasurements", &v5);


  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;
  std::vector<ParticleInfo> particles;


  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);

    //store truthParticleBarcode into vector;
    std::vector<int> particleIdWithTracks; //majorityParticleId

    // loop over all tracks
    for(int iTrack = 0; iTrack < v3->size(); ++iTrack){

      // set limit conditions(truthHitRatio)
      if(std::isnan(v3->at(iTrack)))continue;
      double truthHitRatio = (v4->at(iTrack))*1.0/(v5->at(iTrack));
      if(truthHitRatio < 0.5) continue;
      if((v5->at(iTrack)) < 12)continue;
      particleIdWithTracks.push_back(v3->at(iTrack));
    }

    // loop over all particles
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){
    //  particles.push_back(  {v1->at(iparticle), v2->at(iparticle)} );

      if((v6->at(iparticle)) != true) continue;


      //Fill TEfficiency
        bool hasTrack = false;
        if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), v1->at(iparticle)) != particleIdWithTracks.end()){
          hasTrack = true;
        }
        trackEff_vs_theta->Fill(hasTrack,v2->at(iparticle));
        trackEff_vs_pT->Fill(hasTrack,v7->at(iparticle));
    }
  }

  std::cout<<"All good.Now plotting..."<<std::endl;
  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->Divide(2,2);
  cs->cd(1);
  trackEff_vs_theta->Draw();
  cs->cd(3);
  trackEff_vs_pT->Draw();
}
