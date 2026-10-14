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
  gStyle->SetLineWidth(4);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

  const char* filenames1[4] = {
    "total_3station_forward_seed_summary_circleFitTrackSeedTool.root",
    "total_3station_backward_seed_summary_circleFitTrackSeedTool.root",
    "total_4station_forward_seed_summary_circleFitTrackSeedTool.root",
    "total_4station_backward_seed_summary_circleFitTrackSeedTool.root"
  };
  const char* filenames2[4] = {
    "total_3station_forward_track_summary_ckf.root",
    "total_3station_backward_track_summary_ckf.root",
    "total_4station_forward_track_summary_ckf.root",
    "total_4station_backward_track_summary_ckf.root"
  };
  const char* labels[4] = {"3-Station Forward", "3-Station Backward", "4-Station Forward", "4-Station Backward"};
  int colors[4] = {kRed, kBlue, kGreen+2, kMagenta};

  const Int_t nbins1 = 13;
  double bins1[nbins1 + 1] = {0, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.0175, 0.02, 0.0225, 0.025, 0.03, 0.04, 0.05};
  const Int_t nbins2 = 14;
  double bins2[nbins2 + 1] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.5};
  TEfficiency* eff_theta[4];
  TEfficiency* eff_pT[4];
  TEfficiency* Leff_theta[4];
  TEfficiency* Leff_pT[4];
  std::array<std::array<TEfficiency*, 4>, 4> ve;

  for (int i = 0; i < 4; i++) {
    eff_theta[i] = new TEfficiency(Form("eff_%d", i), ";theta[rad];Tracking Efficiency", nbins1, bins1);
    eff_pT[i] = new TEfficiency(Form("eff_%d", i), ";pT[Gev/c];Tracking Efficiency", nbins2, bins2);
    Leff_theta[i] = new TEfficiency(Form("eff_%d", i), ";theta[rad];Tracking Efficiency", nbins1, bins1);
      Leff_pT[i] = new TEfficiency(Form("eff_%d", i), ";pT[Gev/c];Tracking Efficiency", nbins2, bins2);
  }

  for (int i = 0; i < 4; i++) {
    TFile* file1 = TFile::Open(filenames1[i], "READ");
    TFile* file2 = TFile::Open(filenames2[i], "READ");
    if (!file1 || file1->IsZombie()) {
      std::cerr << "Error opening file: " << filenames1[i] << std::endl;
      continue;
    }
    if (!file2 || file2->IsZombie()) {
      std::cerr << "Error opening file: " << filenames2[i] << std::endl;
      continue;
    }
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
    std::vector<float> *v1 = nullptr;
    std::vector<float> *v2 = nullptr;
    std::vector<uint64_t> *v3 = nullptr;

    std::vector<double> *v4 = nullptr;
    std::vector<unsigned int> *v5 = nullptr;
    std::vector<int> *v6 = nullptr;
    std::vector<float> *v7 = nullptr;


    //All particles
    tree1->SetBranchAddress("truth_barcode",&v1);
    tree1->SetBranchAddress("truth_theta",&v2);
    tree1->SetBranchAddress("truth_pT",&v7);
    tree1->SetBranchAddress("truthParticle_Hits",&v6);

    //All reco tracks
    tree2->SetBranchAddress("majorityParticleId",&v3);
    tree2->SetBranchAddress("t_truthHitRatio", &v4);
    tree2->SetBranchAddress("nMeasurements", &v5);
    for (size_t j = 0; j < tree1->GetEntries(); j++) {
      tree1->GetEntry(j);
      tree2->GetEntry(j);

      //store truthParticleBarcode into vector;
      std::vector<int> particleIdWithTracks; //majorityParticleId

      // loop over all tracks
      for(int iTrack = 0; iTrack < v3->size(); ++iTrack){

        // set limit conditions(truthHitRatio)
        //if((v4->at(iTrack)) < 0.6) continue;
        //if((v5->at(iTrack)) < 12)continue;
        particleIdWithTracks.push_back(v3->at(iTrack));
      }


      // loop over all particles
      for(int iparticle = 0; iparticle < v1->size(); ++iparticle){
      //  particles.push_back(  {v1->at(iparticle), v2->at(iparticle)} );


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
    ve[0][i] = eff_theta[i];
    ve[1][i] = eff_pT[i];
    ve[2][i] = Leff_theta[i];
    ve[3][i] = Leff_pT[i];

    file1->Close();
    file2->Close();
  }

  TCanvas* c = new TCanvas("c1", "Tracking Efficiency Comparison", 800, 600);
  c->Divide(2,2);
  std::vector<TLegend*>legend;
  for(int j = 0; j < 4; j++){
    TLegend* l = new TLegend(0.6, 0.2, 0.9, 0.4);
    legend.push_back(l);

  }
  for(int k = 0; k < 4; k++){
    c->cd(k+1);
    for (int i = 0; i < 4; i++) {
      ve[k][i]->SetLineColor(colors[i]);
      ve[k][i]->SetMarkerColor(colors[i]);
      ve[k][i]->SetMarkerStyle(20);
      ve[k][i]->SetLineWidth(3);
      if (i == 0) ve[k][i]->Draw();
      else ve[k][i]->Draw("same");
      legend[k]->AddEntry(ve[k][i], labels[i], "lep");
    }
    legend[k]->Draw();
  }
}

