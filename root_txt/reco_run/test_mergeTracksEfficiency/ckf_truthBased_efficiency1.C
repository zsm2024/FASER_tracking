#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>

void ckf_truthBased_efficiency1(){
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
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

  const char* filenames1[1] = {
    //"FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008-3stMerged-PHYS.root",
    "FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root",
   //"total_4station_forward_seed_summary_circleFitTrackSeedTool.root",
    //"total_4station_backward_seed_summary_circleFitTrackSeedTool.root"
  };
  const char* filenames2[2] = {
    "FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_track_summary_ckf.root",
    "FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_backward_track_summary_ckf.root",
    //"total_4station_forward_track_summary_ckf.root",
    //"total_4station_backward_track_summary_ckf.root"
  };
 // const char* labels[4] = {"3-Station Forward", "3-Station Backward", "4-Station Forward", "4-Station Backward"};
 // int colors[4] = {kRed, kBlue, kGreen+2, kMagenta};
  
  const char* labels[2] = {"Forward", "Backward"};
  int colors[2] = {kRed, kBlue};

  const Int_t nbins1 = 5;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.001, 0.004};
  const Int_t nbins2 = 5;
  double bins2[nbins2 + 1] = {0, 200, 400, 600, 800, 1600}; 
  
  //const Int_t nbins1 = 5;
  //double bins1[nbins1 + 1] = {0, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005};
  //const Int_t nbins2 = 6;
  //double bins2[nbins2 + 1] = {0, 100, 200, 300, 400, 500, 600};

  TEfficiency* eff_theta[2];
  TEfficiency* eff_pT[2];
  TEfficiency* Leff_theta[2];
  TEfficiency* Leff_pT[2];
  std::array<std::array<TEfficiency*, 2>, 4> ve;

  for (int i = 0; i < 2; i++) {
    eff_theta[i] = new TEfficiency(Form("eff_%d", i), ";prod_theta[rad];Tracking Efficiency", nbins1, bins1);
    eff_pT[i] = new TEfficiency(Form("eff_%d", i), ";prod_pT[Mev/c];Tracking Efficiency", nbins2, bins2);
    Leff_theta[i] = new TEfficiency(Form("eff_%d", i), ";prod_theta[rad];Tracking Efficiency", nbins1, bins1);
    Leff_pT[i] = new TEfficiency(Form("eff_%d", i), ";prod_pT[Mev/c];Tracking Efficiency", nbins2, bins2);
    
    //eff_theta[i] = new TEfficiency(Form("eff_%d", i), ";prod_theta[rad];Tracking Efficiency",10,0,0.001);
    //eff_pT[i] = new TEfficiency(Form("eff_%d", i), ";prod_pT[Mev/c];Tracking Efficiency", 10,0,1500);
    //Leff_theta[i] = new TEfficiency(Form("eff_%d", i), ";prod_theta[rad];Tracking Efficiency",10,0,0.001);
    //Leff_pT[i] = new TEfficiency(Form("eff_%d", i), ";prod_pT[Mev/c];Tracking Efficiency",10,0,1500);
  }

  for (int i = 0; i < 2; i++) {
    TFile* file1 = TFile::Open(filenames1[0], "READ");
    TFile* file2 = TFile::Open(filenames2[i], "READ");
    if (!file1 || file1->IsZombie()) {
      std::cerr << "Error opening file: " << filenames1[i] << std::endl;
      continue;
    }
    if (!file2 || file2->IsZombie()) {
      std::cerr << "Error opening file: " << filenames2[i] << std::endl;
      continue;
    }
    std::cout<<"open file------------"<<std::endl;
    TTree* tree1;
    TTree* tree2;
    file1->GetObject("tree", tree1);
    file2->GetObject("tree", tree2);
    if (!tree1) {
      std::cerr << "Tree not found in " << filenames1[i] << std::endl;
      continue;
    }
    if (!tree2) {
      std::cerr << "Tree not found in " << filenames2[i] << std::endl;
      continue;
    }
    std::cout<<"get the tree---------"<<std::endl;
    std::vector<float> *v1 = nullptr;
    std::vector<float> *v2 = nullptr;
    std::vector<uint64_t> *v3 = nullptr;

    //std::vector<double> *v4 = nullptr;
    //std::vector<unsigned int> *v5 = nullptr;
    std::vector<int> *v6 = nullptr;
    std::vector<float> *v7 = nullptr;


    //All particles
    tree1->SetBranchAddress("truth_barcode",&v1);
    tree1->SetBranchAddress("truth_theta",&v2);
    tree1->SetBranchAddress("truth_pT",&v7);
    tree1->SetBranchAddress("truthParticle_Hits",&v6);

    //All reco tracks
    tree2->SetBranchAddress("majorityParticleId",&v3);
    //tree2->SetBranchAddress("t_truthHitRatio", &v4);
    //tree2->SetBranchAddress("nMeasurements", &v5);
    for (size_t j = 0; j < tree1->GetEntries(); j++) {
      tree1->GetEntry(j);
      tree2->GetEntry(j);

      //store truthParticleBarcode into vector;
      std::vector<int> particleIdWithTracks; //majorityParticleId

      // loop over all tracks
      for(int iTrack = 0; iTrack < v3->size(); ++iTrack){

        particleIdWithTracks.push_back(v3->at(iTrack));
      }


      // loop over all particles
      for(int iparticle = 0; iparticle < v1->size(); ++iparticle){


        //Fill TEfficiency
        bool hasTrack = false;
        if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), v1->at(iparticle)) != particleIdWithTracks.end()){
          hasTrack = true;
        }
        eff_theta[i]->Fill(hasTrack,v2->at(iparticle));
        eff_pT[i]->Fill(hasTrack,v7->at(iparticle));

        if(v6->at(iparticle)<18){
          continue;
        }
        Leff_theta[i]->Fill(hasTrack,v2->at(iparticle));
        Leff_pT[i]->Fill(hasTrack,v7->at(iparticle));

      }
    }
    std::cout<<"fill efficiency-------------------------"<<std::endl;
    ve[0][i] = eff_theta[i];
    ve[1][i] = eff_pT[i];
    ve[2][i] = Leff_theta[i];
    ve[3][i] = Leff_pT[i];

    file1->Close();
    file2->Close();
  }

  TCanvas* c = new TCanvas("c1", "Tracking Efficiency Comparison", 1500, 1200);
  c->Divide(2,2);
  std::vector<TLegend*>legend;
  for(int j = 0; j < 4; j++){
    TLegend* l = new TLegend(0.6, 0.2, 0.9, 0.4);
    legend.push_back(l);

  }
  for(int k = 0; k < 4; k++){
    c->cd(k+1);
    std::cout<<"deal  "<<k+1<<" canvas"<<std::endl;
    //c->cd();
    for (int i = 0; i < 2; i++) {
      ve[k][i]->SetLineColor(colors[i]);
      ve[k][i]->SetMarkerColor(colors[i]);
      ve[k][i]->SetMarkerStyle(20);
      ve[k][i]->SetLineWidth(2);
      if (i == 0) ve[k][i]->Draw();
      else ve[k][i]->Draw("same");
      legend[k]->AddEntry(ve[k][i], labels[i], "lep");
    }
    legend[k]->Draw();
  }
}

