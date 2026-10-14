#include <iostream>
#include <tuple>
#include <vector>

#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>

struct particleInfo{
  int clusterCounts;
  int cl_st0_l0;
  int cl_st0_l1;
  int cl_st0_l2;
  int cl_st1_l0;
  int cl_st1_l1;
  int cl_st1_l2;
  int cl_st2_l0;
  int cl_st2_l1;
  int cl_st2_l2;
  int cl_st3_l0;
  int cl_st3_l1;
  int cl_st3_l2;
};


void truthBased_efficiency(){
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

  TFile* file1 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_4station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();
  
  const Int_t nbins1 = 6;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.001, 0.003, 0.005};
  
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};
  
  //TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";prod_theta[rad];Seeding Efficiency",nbins1, bins1);
  //TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";prod_pT[Mev/c];Seeding Efficiency",nbins2, bins2);

  //TEfficiency* eff1 = new TEfficiency("eff",";prod_theta[rad];Seeding Efficiency",nbins1, bins1);
  //TEfficiency* eff2 = new TEfficiency("eff",";prod_pT[Mev/c];Seeding Efficiency",nbins2, bins2);
  
  //TEfficiency* trackEff_vs_theta1 = new TEfficiency("eff1",";prod_{#theta}[rad];Seeding Efficiency",5,0,0.0005);
  //TEfficiency* trackEff_vs_pT1 = new TEfficiency("effp1",";prod_{p_{T}}[Mev/c];Seeding Efficiency",10,0,1600);

  //TEfficiency* trackEff_vs_theta2 = new TEfficiency("eff2",";prod_{#theta}[rad];Seeding Efficiency",5,0,0.0005);
  //TEfficiency* trackEff_vs_pT2 = new TEfficiency("effp2",";prod_{p_{T}}[Mev/c];Seeding Efficiency",10,0,1600);

  //TEfficiency* trackEff_vs_theta3 = new TEfficiency("eff3",";prod_{#theta}[rad];Seeding Efficiency",5,0,0.0005);
  //TEfficiency* trackEff_vs_pT3 = new TEfficiency("effp3",";prod_{p_{T}}[Mev/c];Seeding Efficiency",10,0,1600);
  
  TEfficiency* trackEff_vs_theta1 = new TEfficiency("eff1",";prod_{#theta}[rad];Seeding Efficiency",5,0,0.0005);
  TEfficiency* trackEff_vs_pT1 = new TEfficiency("effp1",";prod_{p_{T}}[Mev/c];Seeding Efficiency",10,0,1000);

  TEfficiency* trackEff_vs_theta2 = new TEfficiency("eff2",";prod_{#theta}[rad];Seeding Efficiency",5,0,0.0005);
  TEfficiency* trackEff_vs_pT2 = new TEfficiency("effp2",";prod_{p_{T}}[Mev/c];Seeding Efficiency",10,0,1000);

  TEfficiency* trackEff_vs_theta3 = new TEfficiency("eff3",";prod_{#theta}[rad];Seeding Efficiency",5,0,0.0005);
  TEfficiency* trackEff_vs_pT3 = new TEfficiency("effp3",";prod_{p_{T}}[Mev/c];Seeding Efficiency",10,0,1000);
  
  trackEff_vs_theta1->SetLineColor(kBlue);
  trackEff_vs_theta1->SetMarkerColor(kBlue);
  trackEff_vs_pT1->SetLineColor(kBlue);
  trackEff_vs_pT1->SetMarkerColor(kBlue);
  trackEff_vs_theta2->SetLineColor(kRed);
  trackEff_vs_theta2->SetMarkerColor(kRed);
  trackEff_vs_pT2->SetLineColor(kRed);
  trackEff_vs_pT2->SetMarkerColor(kRed);
  trackEff_vs_theta3->SetLineColor(kGreen);
  trackEff_vs_theta3->SetMarkerColor(kGreen);
  trackEff_vs_pT3->SetLineColor(kGreen);
  trackEff_vs_pT3->SetMarkerColor(kGreen);
  
  std::vector<int>* st0_l0 = nullptr;
  std::vector<int>* st0_l1 = nullptr;
  std::vector<int>* st0_l2 = nullptr;
  std::vector<int>* st1_l0 = nullptr;
  std::vector<int>* st1_l1 = nullptr;
  std::vector<int>* st1_l2 = nullptr;
  std::vector<int>* st2_l0 = nullptr;
  std::vector<int>* st2_l1 = nullptr;
  std::vector<int>* st2_l2 = nullptr;
  std::vector<int>* st3_l0 = nullptr;
  std::vector<int>* st3_l1 = nullptr;
  std::vector<int>* st3_l2 = nullptr;

  std::vector<uint64_t> *v1 = nullptr;
  std::vector<float> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;

  std::vector<double> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<float> *v7 = nullptr;
  std::vector<float> *v8 = nullptr;
  
  std::vector<uint64_t> *v9 = nullptr;
  std::vector<bool> *v10 = nullptr;
  std::vector<int> *v11 = nullptr;


  //All particles  
  t1->SetBranchAddress("truth_barcode",&v1);
  t1->SetBranchAddress("truth_theta",&v2);
  t1->SetBranchAddress("truth_pT",&v7);
  t1->SetBranchAddress("truthParticle_Hits",&v6);
  t1->SetBranchAddress("truth_prod_x",&v8);

  //All reco Seeds
  t1->SetBranchAddress("majorityParticleId",&v3);
  t1->SetBranchAddress("t_truthHitRatio", &v4);
  t1->SetBranchAddress("nMeasurements", &v5);
  t1->SetBranchAddress("clusterContributionParticleId", &v9);
  t1->SetBranchAddress("clusterContributionParticleIsFiducial", &v10);
  t1->SetBranchAddress("clusterCounts", &v11);
  
  t1->SetBranchAddress("nClusters_st0_l0", &st0_l0);
  t1->SetBranchAddress("nClusters_st0_l1", &st0_l1);
  t1->SetBranchAddress("nClusters_st0_l2", &st0_l2);
  t1->SetBranchAddress("nClusters_st1_l0", &st1_l0);
  t1->SetBranchAddress("nClusters_st1_l1", &st1_l1);
  t1->SetBranchAddress("nClusters_st1_l2", &st1_l2);
  t1->SetBranchAddress("nClusters_st2_l0", &st2_l0);
  t1->SetBranchAddress("nClusters_st2_l1", &st2_l1);
  t1->SetBranchAddress("nClusters_st2_l2", &st2_l2);
  t1->SetBranchAddress("nClusters_st3_l0", &st3_l0);
  t1->SetBranchAddress("nClusters_st3_l1", &st3_l1);
  t1->SetBranchAddress("nClusters_st3_l2", &st3_l2);


  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;



  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);
    

    //store majorityParticleId into vector;
    std::vector<int> particleIdWithTracks; 
   
    // loop over all tracks
    for(int iTrack = 0; iTrack < v3->size(); ++iTrack){
  
      particleIdWithTracks.push_back(v3->at(iTrack));
    }


    //loop over all particeIdWithClusters
    std::map<uint64_t, particleInfo>particleIdWithClusters;
    for(int ip = 0; ip < v9->size(); ip++){
      //if(v10->at(ip)==false)continue;
      particleIdWithClusters[v9->at(ip)] = {v11->at(ip),st0_l0->at(ip),st0_l1->at(ip),st0_l2->at(ip),st1_l0->at(ip),st1_l1->at(ip),st1_l2->at(ip),st2_l0->at(ip),st2_l1->at(ip),st2_l2->at(ip),st3_l0->at(ip),st3_l1->at(ip),st3_l2->at(ip)};
    }


    // loop over all particles 
    
    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){


      //Fill TEfficiency
      bool hasTrack = false;
      if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), v1->at(iparticle)) != particleIdWithTracks.end()){ 
        hasTrack = true;
      }
      trackEff_vs_theta1->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT1->Fill(hasTrack,v7->at(iparticle));

      if(v6->at(iparticle)<18) continue;
      trackEff_vs_theta2->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT2->Fill(hasTrack,v7->at(iparticle));

      //if(!(st1_l0>=2 && st1_l1>=2 && st1_l2>=2 && st2_l0>=2 && st2_l1>=2 && st2_l2>=2 && st3_l0>=2 && st3_l1>=2 && st3_l2>=2)){
      //  continue;
      //}
      auto it = particleIdWithClusters.find(v1->at(iparticle));
      if(it != particleIdWithClusters.end()){
        //if(particleIdWithClusters[v1->at(iparticle)].clusterCounts<18){
        //  continue;
        //}
        particleInfo p = particleIdWithClusters[v1->at(iparticle)];
        //if(!(p.cl_st1_l0>=2 && p.cl_st1_l1>=2 && p.cl_st1_l2>=2 && p.cl_st2_l0>=2 && p.cl_st2_l1>=2 && p.cl_st2_l2>=2 && p.cl_st3_l0>=2 && p.cl_st3_l1>=2 && p.cl_st3_l2>=2)){
        //  continue;
        //}
        int st1_nClusters = p.cl_st1_l0 + p.cl_st1_l1 + p.cl_st1_l2;
        int st2_nClusters = p.cl_st2_l0 + p.cl_st2_l1 + p.cl_st2_l2;
        int st3_nClusters = p.cl_st3_l0 + p.cl_st3_l1 + p.cl_st3_l2;
        if (!( (st1_nClusters >= 4 && st1_nClusters <= 36) ||
           (st2_nClusters >= 4 && st2_nClusters <= 36) ||
           (st3_nClusters >= 4 && st3_nClusters <= 36) )){
          std::cout<<"the number of clusters at one station per truthParticle is too little or too much"<<std::endl;
          std::cout<<"ievent = "<<ievent<<"; "<<"truthParticleId = "<<v1->at(iparticle)<<"; "<<"the number of clusters at per station for this truthParticle: "<<st1_nClusters<<"; "<<st2_nClusters<<"; "<<st3_nClusters<<std::endl;
          std::cout<<"******************************************************************************"<<std::endl;
            continue;
           }

      }else{
        std::cout<<"The truthParticle did not match any clusters"<<std::endl;
        std::cout<<"ievent = "<<ievent<<"; "<<"truthParticleId = "<<v1->at(iparticle)<<std::endl;
        std::cout<<"the particleIdWithClusters of this event: "<<std::endl;
        for(auto& [key,value] : particleIdWithClusters){
          std::cout<<"particleId = "<<key<<"; "<<"clusterCounts = "<<value.clusterCounts<<std::endl;
        }
        std::cout<<"------------------------------------"<<std::endl;
        continue;
      }
      trackEff_vs_theta3->Fill(hasTrack,v2->at(iparticle));
      trackEff_vs_pT3->Fill(hasTrack,v7->at(iparticle));

      

      
    }
  }



  
  std::cout<<"All good.Now plotting..."<<std::endl;
  TCanvas* cs = new TCanvas("c1","efficiency",1500,1200);
  cs->Divide(2,2);
  cs->cd(1);
  trackEff_vs_theta1->Draw();
  trackEff_vs_theta2->Draw("same");
  trackEff_vs_theta3->Draw("same");
  TLegend* l = new TLegend(0.6, 0.2, 0.9, 0.4);
  l->AddEntry(trackEff_vs_theta1,"origin","lep");
  l->AddEntry(trackEff_vs_theta2,"truthParticle_nHits>=18","lep");
  l->AddEntry(trackEff_vs_theta3,"truthParticle_nClusters>=4 at least at  one station","lep");
  l->Draw();
  cs->cd(2);
  trackEff_vs_pT1->Draw();
  trackEff_vs_pT2->Draw("same");
  trackEff_vs_pT3->Draw("same");
  TLegend* l1 = new TLegend(0.6, 0.2, 0.9, 0.4);
  l1->AddEntry(trackEff_vs_pT1,"origin","lep");
  l1->AddEntry(trackEff_vs_pT2,"truthParticle_nHits>=18","lep");
  l1->AddEntry(trackEff_vs_pT3,"truthParticle_nClusters>=4 at least at one station","lep");
  l1->Draw();
  //cs->cd(1);
  //trackEff_vs_theta->SetLineWidth(2);
  //trackEff_vs_theta->Draw();
  //cs->cd(2);
  //trackEff_vs_pT->SetLineWidth(2);
  //trackEff_vs_pT->Draw();
  //cs->cd(3);
  //eff1->SetLineWidth(2);
  //eff1->Draw();
  //cs->cd(4);
  //eff2->SetLineWidth(2);
  //eff2->Draw();

  //cs->SaveAs("truthBased.pdf");
}
