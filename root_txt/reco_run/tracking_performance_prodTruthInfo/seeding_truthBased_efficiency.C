#include <iostream>
#include <tuple>
#include <vector>

#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>


void seeding_truthBased_efficiency(){
  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetLineWidth(2);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);

  TFile* file1 = TFile::Open("total_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();
  
  const Int_t nbins1 = 6;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.001, 0.003, 0.005};
  
  //const Int_t nbins1 = 8;
  //double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.0015, 0.002, 0.004};
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};
  TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";prod_theta[rad];Seeding Efficiency",nbins1, bins1);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";prod_pT[Mev/c];Seeding Efficiency",nbins2, bins2);

  TEfficiency* eff1 = new TEfficiency("eff",";prod_theta[rad];Seeding Efficiency",nbins1, bins1);
  TEfficiency* eff2 = new TEfficiency("eff",";prod_pT[Mev/c];Seeding Efficiency",nbins2, bins2);
  
 // TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";prod_theta[rad];Seeding Efficiency",20,0,0.004);
 // TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";prod_pT[Mev/c];Seeding Efficiency",20,0,1600);

 // TEfficiency* eff1 = new TEfficiency("eff",";prod_theta[rad];cut_Seeding Efficiency",20,0,0.004);
 // TEfficiency* eff2 = new TEfficiency("eff",";prod_pT[Mev/c];cut_Seeding Efficiency",20,0,1600);

  std::vector<uint64_t> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;

  std::vector<double> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;
  std::vector<float> *v8 = nullptr;


  //All particles  
  t1->SetBranchAddress("truth_barcode",&v1);
  t1->SetBranchAddress("truth_theta",&v2);
  t1->SetBranchAddress("truth_pT",&v7);
  t1->SetBranchAddress("truthParticle_Hits",&v6);
  t1->SetBranchAddress("truth_prod_x",&v8);

  //All reco tracks
  t1->SetBranchAddress("majorityParticleId",&v3);
  t1->SetBranchAddress("t_truthHitRatio", &v4);
  t1->SetBranchAddress("nMeasurements", &v5);


  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;


  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);

    //store majorityParticleId into vector;
    std::vector<int> particleIdWithTracks; 
   
    // loop over all tracks
    for(int iTrack = 0; iTrack < v3->size(); ++iTrack){
  
      //set limit conditions(truthHitRatio)
      //if((v4->at(iTrack)) < 0.6) continue;
      //if((v5->at(iTrack)) < 12)continue;
      particleIdWithTracks.push_back(v3->at(iTrack));
    }


    // loop over all particles 
    
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){

      ////only see particles having prod_vertex
      //if(std::isnan(v8->at(iparticle))){
      //  std::cout<<"ievent: "<<ievent<<"particle: "<<iparticle<<std::endl;
      //  continue;
      //}


      //Fill TEfficiency
      bool hasTrack = false;
      if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), v1->at(iparticle)) != particleIdWithTracks.end()){ 
        hasTrack = true;
      }
      trackEff_vs_theta->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT->Fill(hasTrack,v7->at(iparticle));

      if(v6->at(iparticle)<18) continue;
      eff1->Fill(hasTrack,v2->at(iparticle));
      eff2->Fill(hasTrack,v7->at(iparticle));
      
    }
  }



  
  std::cout<<"All good.Now plotting..."<<std::endl;
  TCanvas* cs = new TCanvas("c1","efficiency",1500,1200);
  cs->Divide(2,2);
  cs->cd(1);
  trackEff_vs_theta->SetLineWidth(2);
  trackEff_vs_theta->Draw();
  cs->cd(2);
  trackEff_vs_pT->SetLineWidth(2);
  trackEff_vs_pT->Draw();
  cs->cd(3);
  eff1->SetLineWidth(2);
  eff1->Draw();
  cs->cd(4);
  eff2->SetLineWidth(2);
  eff2->Draw();

  //cs->SaveAs("truthBased.pdf");
}
