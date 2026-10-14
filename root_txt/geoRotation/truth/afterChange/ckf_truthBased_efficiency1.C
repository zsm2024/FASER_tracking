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

  const char* filenames1[4] = {
    "FaserMC-MC22_FS_Aee_010MeV_1Em4-110051-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root",
    "FaserMC-MC22_FS_Aee_010MeV_1Em4-110051-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root",
   //"total_4station_forward_seed_summary_circleFitTrackSeedTool.root",
    //"total_4station_backward_seed_summary_circleFitTrackSeedTool.root"
  };
  const char* filenames2[4] = {
    "FaserMC-MC22_FS_Aee_010MeV_1Em4-110051-00000-00009-s0008_3station_forward_track_summary_ckf.root",
    "FaserMC-MC22_FS_Aee_010MeV_1Em4-110051-00000-00009-s0008_3station_backward_track_summary_ckf.root",
    //"total_4station_forward_track_summary_ckf.root",
    //"total_4station_backward_track_summary_ckf.root"
  };
 
  const char* labels[2] = {"Forward", "Backward"};
  int colors[2] = {kRed, kBlue};

  //set different width of bins (according to range of theta and pT).
  const Int_t nbins1 = 4;
  double bins1[nbins1 + 1] = {1.5704, 1.5708,  1.5709,  1.571, 1.5715};
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};
  

  //according to number of files.
  TEfficiency* eff_theta[2];
  TEfficiency* eff_pT[2];
  TEfficiency* Leff_theta[2];
  TEfficiency* Leff_pT[2];
  
  //four plots in total, forward and backward in one plot
  std::array<std::array<TEfficiency*, 2>, 4> ve;

  for (int i = 0; i < 2; i++) {
    
    //deal with i file.
    eff_theta[i] = new TEfficiency(Form("eff_theta_%d", i), ";prod_{#theta} [rad];CKF Efficiency", 10, 1.5705, 1.571);
    eff_pT[i] = new TEfficiency(Form("eff_pT_%d", i), ";prod_{p_{T}} [Mev/c];CKF Efficiency", 10, 0, 1000);
    Leff_theta[i] = new TEfficiency(Form("Leff_theta_%d", i), ";prod_{#theta} [rad];CKF Efficiency", 10, 1.5705, 1.571);
    Leff_pT[i] = new TEfficiency(Form("Leff_pT_%d", i), ";prod_{p_{T}} [Mev/c];CKF Efficiency", 10, 0, 1000);
    //eff_theta[i] = new TEfficiency(Form("eff_theta_%d", i), ";prod_{#theta} [rad];CKF Efficiency", nbins1, bins1);
    //eff_pT[i] = new TEfficiency(Form("eff_pT_%d", i), ";prod_{p_{T}} [Mev/c];CKF Efficiency", nbins2, bins2);
    //Leff_theta[i] = new TEfficiency(Form("Leff_theta_%d", i), ";prod_{#theta} [rad];CKF Efficiency", nbins1, bins1);
    //Leff_pT[i] = new TEfficiency(Form("Leff_pT_%d", i), ";prod_{p_{T}} [Mev/c];CKF Efficiency", nbins2, bins2);
    
  }

  for (int i = 0; i < 2; i++) {
    
    TFile* file1 = TFile::Open(filenames1[i], "READ");
    TFile* file2 = TFile::Open(filenames2[i], "READ");
    TTree* tree1;
    TTree* tree2;
    file1->GetObject("tree", tree1);
    file2->GetObject("tree", tree2);
    
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
    //tree2->SetBranchAddress("t_truthHitRatio", &v4);
    tree2->SetBranchAddress("nMeasurements", &v5);
    
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
    ve[0][i] = eff_theta[i];
    ve[1][i] = eff_pT[i];
    ve[2][i] = Leff_theta[i];
    ve[3][i] = Leff_pT[i];

    file1->Close();
    file2->Close();
  }
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
    t->SetBranchAddress("truthParticle_Hits",&v6);
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
  
 // TEfficiency* trackEff_vs_theta = new TEfficiency("eff",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
 // TEfficiency* trackEff_vs_pT = new TEfficiency("eff",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
 // TEfficiency* trackEff_vs_theta1 = new TEfficiency("eff",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
 // TEfficiency* trackEff_vs_pT1 = new TEfficiency("eff",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);

 // processFile("way1-MergeTracks-4st-PHYS.root", trackEff_vs_theta, trackEff_vs_pT);
 // processFile("way2-MergeTracks-4st-PHYS.root", trackEff_vs_theta1, trackEff_vs_pT1);


  TCanvas* c = new TCanvas("c1", "Tracking Efficiency Comparison", 1500, 1200);
  c->Divide(2,2);
  std::vector<TLegend*>legend;
  for(int j = 0; j < 4; j++){
    TLegend* l = new TLegend(0.6, 0.2, 0.9, 0.4);
    legend.push_back(l);

  }
  for(int k = 2; k < 4; k++){
    c->cd(k+1);
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
    //if(k==2){
    //  trackEff_vs_theta->Draw("same");
    //  trackEff_vs_theta->SetLineColor(kGreen);
    //  trackEff_vs_theta->SetMarkerColor(kGreen);
    //  trackEff_vs_theta->SetMarkerStyle(20);
    //  trackEff_vs_theta->SetLineWidth(2);
    //  legend[k]->AddEntry(trackEff_vs_theta,"Merged", "lep");
    //  
    //  //trackEff_vs_theta1->Draw("same");
    //  //trackEff_vs_theta1->SetLineColor(kOrange);
    //  //trackEff_vs_theta1->SetMarkerColor(kOrange);
    //  //trackEff_vs_theta1->SetMarkerStyle(20);
    //  //trackEff_vs_theta1->SetLineWidth(2);
    //  //legend[k]->AddEntry(trackEff_vs_theta1,"merge using method2", "lep");
    //}
    //if(k==3){
    //  trackEff_vs_pT->Draw("same");
    //  trackEff_vs_pT->SetLineColor(kGreen);
    //  trackEff_vs_pT->SetMarkerColor(kGreen);
    //  trackEff_vs_pT->SetMarkerStyle(20);
    //  trackEff_vs_pT->SetLineWidth(2);
    //  legend[k]->AddEntry(trackEff_vs_pT,"Merged", "lep");
      
      //trackEff_vs_pT1->Draw("same");
      //trackEff_vs_pT1->SetLineColor(kOrange);
      //trackEff_vs_pT1->SetMarkerColor(kOrange);
      //trackEff_vs_pT1->SetMarkerStyle(20);
      //trackEff_vs_pT1->SetLineWidth(2);
      //legend[k]->AddEntry(trackEff_vs_pT1,"merge using method2", "lep");
    //}
    legend[k]->Draw();
  }
}

