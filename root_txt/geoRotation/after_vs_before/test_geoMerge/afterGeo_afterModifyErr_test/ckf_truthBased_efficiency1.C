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

 

  //set different width of bins (according to range of theta and pT).
  const Int_t nbins1 = 7;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.002, 0.004};
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};
  

  auto processFile = [&](const char* filename,
                       TEfficiency* eff_theta,
                       TEfficiency* eff_pT) {

    TFile* f = TFile::Open(filename, "read");
    if(!f || f->IsZombie()){
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    TTreeReader* reader = new TTreeReader("nt", f);
    TTree* t = reader->GetTree();

    std::vector<float>* v1 = nullptr;  // truth_barcode
    std::vector<float>* v2 = nullptr;  // truth_theta
    std::vector<int>* v3 = nullptr;    // majorityParticleId
    std::vector<double>* v4 = nullptr; // truthHitRatio
    std::vector<int>* v6 = nullptr;    // truthParticle_Hits
    std::vector<float>* v7 = nullptr;  // truth_pT

    t->SetBranchAddress("truth_barcode",&v1);
    t->SetBranchAddress("truth_theta",&v2);
    t->SetBranchAddress("truth_pT",&v7);
    t->SetBranchAddress("truth_nHits",&v6);
    //t->SetBranchAddress("majorityParticleId",&v3);
    //t->SetBranchAddress("truthHitRatio",&v4);
    t->SetBranchAddress("t_barcode",&v3);
    t->SetBranchAddress("t_truthHitRatio",&v4);

    size_t entries = t->GetEntries();

    for(size_t iEvent = 0; iEvent < entries; iEvent++){
        t->GetEntry(iEvent);
        std::vector<int> matchingParticleId;

        // loop over reco tracks
        for(size_t iTrack = 0; iTrack < v3->size(); iTrack++){
            matchingParticleId.push_back(v3->at(iTrack));
        }

        // loop over truth particles
        for(int iparticle = 0; iparticle < v1->size(); ++iparticle){
            if(v6->at(iparticle) < 18) continue;

            bool hasTrack = (std::find(matchingParticleId.begin(),
                                       matchingParticleId.end(),
                                       v1->at(iparticle)) != matchingParticleId.end());

            eff_theta->Fill(hasTrack, v2->at(iparticle));
            eff_pT->Fill(hasTrack, v7->at(iparticle));
        }
    }

    f->Close();
  };
  
  TEfficiency* trackEff_vs_theta1 = new TEfficiency("eff1",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT1 = new TEfficiency("eff2",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta2 = new TEfficiency("eff3",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT2 = new TEfficiency("eff4",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta3 = new TEfficiency("eff5",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT3 = new TEfficiency("eff6",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta4 = new TEfficiency("eff7",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT4 = new TEfficiency("eff8",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);

  processFile("110033_ntOut_officialRec_3st_forward_new_PHYS.root", trackEff_vs_theta1, trackEff_vs_pT1);
  processFile("beforeGeo_mc24_110033_biggerErr_3st_forward_PHYS.root", trackEff_vs_theta2, trackEff_vs_pT2);
  processFile("110033_smallerCov_seedParams_3stForward_new_PHYS.root", trackEff_vs_theta3, trackEff_vs_pT3);
  processFile("afterGeo_mc24_110033_oldErr_3st_forward_PHYS.root", trackEff_vs_theta4, trackEff_vs_pT4);


  TCanvas* c = new TCanvas("c1", "Tracking Efficiency Comparison", 1500, 1200);
  c->Divide(1,2);
  std::vector<TLegend*>legend;
  for(int j = 0; j < 2; j++){
    TLegend* l = new TLegend(0.6, 0.2, 0.9, 0.4);
    legend.push_back(l);

  }
  for(int k = 0; k < 4; k++){
    c->cd(k+1);
    //c->cd();
    if(k==0){
      trackEff_vs_theta1->Draw();
      trackEff_vs_theta1->SetLineColor(kBlue);
      trackEff_vs_theta1->SetMarkerColor(kBlue);
      trackEff_vs_theta1->SetMarkerStyle(20);
      trackEff_vs_theta1->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta1,"beforeGeo_oldErr", "lep");
      
      trackEff_vs_theta2->Draw("same");
      trackEff_vs_theta2->SetLineColor(kGreen);
      trackEff_vs_theta2->SetMarkerColor(kGreen);
      trackEff_vs_theta2->SetMarkerStyle(20);
      trackEff_vs_theta2->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta2,"beforeGeo_biggerErr", "lep");
      
      trackEff_vs_theta3->Draw("same");
      trackEff_vs_theta3->SetLineColor(kRed);
      trackEff_vs_theta3->SetMarkerColor(kRed);
      trackEff_vs_theta3->SetMarkerStyle(20);
      trackEff_vs_theta3->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta3,"afterGeo_biggerErr", "lep");
      
      trackEff_vs_theta4->Draw("same");
      trackEff_vs_theta4->SetLineColor(kBlack);
      trackEff_vs_theta4->SetMarkerColor(kBlack);
      trackEff_vs_theta4->SetMarkerStyle(20);
      trackEff_vs_theta4->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta4,"afterGeo_oldErr", "lep");
    }
    if(k==1){
      trackEff_vs_pT1->Draw();
      trackEff_vs_pT1->SetLineColor(kBlue);
      trackEff_vs_pT1->SetMarkerColor(kBlue);
      trackEff_vs_pT1->SetMarkerStyle(20);
      trackEff_vs_pT1->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT1,"beforeGeo_oldErr", "lep");
      
      trackEff_vs_pT2->Draw("same");
      trackEff_vs_pT2->SetLineColor(kGreen);
      trackEff_vs_pT2->SetMarkerColor(kGreen);
      trackEff_vs_pT2->SetMarkerStyle(20);
      trackEff_vs_pT2->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT2,"beforeGeo_biggerErr", "lep");
      
      trackEff_vs_pT3->Draw("same");
      trackEff_vs_pT3->SetLineColor(kRed);
      trackEff_vs_pT3->SetMarkerColor(kRed);
      trackEff_vs_pT3->SetMarkerStyle(20);
      trackEff_vs_pT3->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT3,"afterGeo_biggerErr", "lep");
      
      trackEff_vs_pT4->Draw("same");
      trackEff_vs_pT4->SetLineColor(kBlack);
      trackEff_vs_pT4->SetMarkerColor(kBlack);
      trackEff_vs_pT4->SetMarkerStyle(20);
      trackEff_vs_pT4->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT4,"afterGeo_oldErr", "lep");
    }
    legend[k]->Draw();
  }
}

