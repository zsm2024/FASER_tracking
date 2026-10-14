#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>

  TGraphErrors* makeRatioGraph(TEfficiency* eff1,
                             TEfficiency* eff2){

  TGraphAsymmErrors* g1 = eff1->CreateGraph();
  TGraphAsymmErrors* g2 = eff2->CreateGraph();

  int n = g1->GetN();

  TGraphErrors* ratio = new TGraphErrors(n);

  for(int i=0;i<n;i++){

    double x1,y1;
    double x2,y2;

    g1->GetPoint(i,x1,y1);
    g2->GetPoint(i,x2,y2);

    if(y1 <= 0) continue;

    double r = y2/y1;

    ratio->SetPoint(i,x1,r);

    //double err1 = g1->GetErrorY(i);
    //double err2 = g2->GetErrorY(i);

    //double err = r * sqrt(
    //    (err1*err1)/(y1*y1)
    //  + (err2*err2)/(y2*y2)
    //);

    //ratio->SetPointError(i,0,err);
  }

  return ratio;
}
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



  //const char* filenames1[4] = {
  //  "afterModify_3station_forward_seed_summary_circleFitTrackSeedTool.root",
  //  "afterModify_3station_forward_seed_summary_circleFitTrackSeedTool.root",
  // //"total_4station_forward_seed_summary_circleFitTrackSeedTool.root",
  //  //"total_4station_backward_seed_summary_circleFitTrackSeedTool.root"
  //};
  //const char* filenames2[4] = {
  //  "afterModify_3station_forward_track_summary_ckf.root",
  //  "afterModify_3station_backward_track_summary_ckf.root",
  //  //"total_4station_forward_track_summary_ckf.root",
  //  //"total_4station_backward_track_summary_ckf.root"
  //};
 
  //const char* labels[2] = {"Forward", "Backward"};
  //int colors[2] = {kRed, kBlue};

  //set different width of bins (according to range of theta and pT).
  const Int_t nbins1 = 7;
  double bins1[nbins1 + 1] = {0, 0.0002, 0.0004, 0.0006, 0.0008, 0.001, 0.002, 0.004};
  const Int_t nbins2 = 8;
  double bins2[nbins2 + 1] = {0, 50, 100, 150, 200, 300, 400, 600, 1000};
  

  ////according to number of files.
  //TEfficiency* eff_theta[2];
  //TEfficiency* eff_pT[2];
  //TEfficiency* Leff_theta[2];
  //TEfficiency* Leff_pT[2];
  //
  ////four plots in total, forward and backward in one plot
  //std::array<std::array<TEfficiency*, 2>, 4> ve;

  //for (int i = 0; i < 2; i++) {
  //  
  //  //deal with i file.
  //  eff_theta[i] = new TEfficiency(Form("eff_theta_%d", i), ";prod_{#theta} [rad];CKF Efficiency", nbins1, bins1);
  //  eff_pT[i] = new TEfficiency(Form("eff_pT_%d", i), ";prod_{p_{T}} [Mev/c];CKF Efficiency", nbins2, bins2);
  //  Leff_theta[i] = new TEfficiency(Form("Leff_theta_%d", i), ";prod_{#theta} [rad];CKF Efficiency", nbins1, bins1);
  //  Leff_pT[i] = new TEfficiency(Form("Leff_pT_%d", i), ";prod_{p_{T}} [Mev/c];CKF Efficiency", nbins2, bins2);
  //  
  //}

  //for (int i = 0; i < 2; i++) {
  //  
  //  TFile* file1 = TFile::Open(filenames1[i], "READ");
  //  TFile* file2 = TFile::Open(filenames2[i], "READ");
  //  TTree* tree1;
  //  TTree* tree2;
  //  file1->GetObject("tree", tree1);
  //  file2->GetObject("tree", tree2);
  //  
  //  std::vector<float> *v1 = nullptr;
  //  std::vector<float> *v2 = nullptr;
  //  std::vector<uint64_t> *v3 = nullptr;

  //  std::vector<double> *v4 = nullptr;
  //  std::vector<unsigned int> *v5 = nullptr;
  //  std::vector<int> *v6 = nullptr;
  //  std::vector<float> *v7 = nullptr;


  //  //All particles
  //  tree1->SetBranchAddress("truth_barcode",&v1);
  //  tree1->SetBranchAddress("truth_theta",&v2);
  //  tree1->SetBranchAddress("truth_pT",&v7);
  //  tree1->SetBranchAddress("truthParticle_Hits",&v6);

  //  //All reco tracks
  //  tree2->SetBranchAddress("majorityParticleId",&v3);
  //  //tree2->SetBranchAddress("nMajorityHits", &v4);
  //  //tree2->SetBranchAddress("nMeasurements", &v5);
  //  
  //  for (size_t j = 0; j < tree1->GetEntries(); j++) {
  //    tree1->GetEntry(j);
  //    tree2->GetEntry(j);

  //    //store truthParticleBarcode into vector;
  //    std::vector<int> particleIdWithTracks; //majorityParticleId

  //    // loop over all tracks
  //    for(int iTrack = 0; iTrack < v3->size(); ++iTrack){

  //      particleIdWithTracks.push_back(v3->at(iTrack));
  //    }


  //    // loop over all particles
  //    for(int iparticle = 0; iparticle < v1->size(); ++iparticle){


  //      //Fill TEfficiency
  //      bool hasTrack = false;
  //      if(std::find(particleIdWithTracks.begin(), particleIdWithTracks.end(), v1->at(iparticle)) != particleIdWithTracks.end()){
  //        hasTrack = true;
  //      }
  //      eff_theta[i]->Fill(hasTrack,v2->at(iparticle));
  //      eff_pT[i]->Fill(hasTrack,v7->at(iparticle));

  //      if(v6->at(iparticle)<18){
  //        continue;
  //      }
  //      Leff_theta[i]->Fill(hasTrack,v2->at(iparticle));
  //      Leff_pT[i]->Fill(hasTrack,v7->at(iparticle));

  //    }
  //  }
  //  ve[0][i] = eff_theta[i];
  //  ve[1][i] = eff_pT[i];
  //  ve[2][i] = Leff_theta[i];
  //  ve[3][i] = Leff_pT[i];

  //  file1->Close();
  //  file2->Close();
  //}
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

    for(size_t iEvent = 0; iEvent < 20; iEvent++){
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
  
  TEfficiency* trackEff_vs_theta = new TEfficiency("eff1",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT = new TEfficiency("eff2",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta1 = new TEfficiency("eff3",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT1 = new TEfficiency("eff4",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta2 = new TEfficiency("eff5",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT2 = new TEfficiency("eff6",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);
  TEfficiency* trackEff_vs_theta3 = new TEfficiency("eff7",";prod_{#theta} [rad];CKF Efficiency",nbins1,bins1);
  TEfficiency* trackEff_vs_pT3 = new TEfficiency("eff8",";prod_{p_{T}} [Mev/c];CKF Efficiency",nbins2,bins2);

  //processFile("110033_officialNt_PHYS.root", trackEff_vs_theta, trackEff_vs_pT);
  //processFile("110033_modifyRefit_PHYS.root", trackEff_vs_theta1, trackEff_vs_pT1);
  //processFile("110033_afterModifyRefit_xaiErr_PHYS.root  ", trackEff_vs_theta2, trackEff_vs_pT2);
  processFile("afterModifyRefit_modifySmallErr_PHYS.root  ", trackEff_vs_theta3, trackEff_vs_pT3);


  TGraphErrors* ratio_theta =
    makeRatioGraph(trackEff_vs_theta,
                   trackEff_vs_theta1);
  TGraphErrors* ratio_theta1 =
    makeRatioGraph(trackEff_vs_theta,
                   trackEff_vs_theta3);

TGraphErrors* ratio_pT =
    makeRatioGraph(trackEff_vs_pT,
                   trackEff_vs_pT1);
TGraphErrors* ratio_pT1 =
    makeRatioGraph(trackEff_vs_pT,
                   trackEff_vs_pT3);

  TCanvas* c = new TCanvas("c1", "Tracking Efficiency Comparison", 1500, 400);
  c->Divide(2,2);
  std::vector<TLegend*>legend;
  for(int j = 0; j < 4; j++){
    TLegend* l = new TLegend(0.6, 0.2, 0.9, 0.4);
    legend.push_back(l);

  }
  for(int k = 0; k < 4; k++){
    c->cd(k+1);
    //for (int i = 0; i < 2; i++) {
    //  
    //  
    //  ve[k][i]->SetLineColor(colors[i]);
    //  ve[k][i]->SetMarkerColor(colors[i]);
    //  ve[k][i]->SetMarkerStyle(20);
    //  ve[k][i]->SetLineWidth(2);
    //  if (i == 0) ve[k][i]->Draw();
    //  else ve[k][i]->Draw("same");
    //  legend[k]->AddEntry(ve[k][i], labels[i], "lep");
    //  
    //}
    if(k==0){
      trackEff_vs_theta->Draw();
      trackEff_vs_theta->SetLineColor(kRed);
      trackEff_vs_theta->SetMarkerColor(kRed);
      trackEff_vs_theta->SetMarkerStyle(20);
      trackEff_vs_theta->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta,"beforeGeo_mergedTracks", "lep");
      
      trackEff_vs_theta1->Draw("same");
      trackEff_vs_theta1->SetLineColor(kBlue);
      trackEff_vs_theta1->SetMarkerColor(kBlue);
      trackEff_vs_theta1->SetMarkerStyle(20);
      trackEff_vs_theta1->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta1,"afterGeo_mergedTracks", "lep");
      
      trackEff_vs_theta2->Draw("same");
      trackEff_vs_theta2->SetLineColor(kBlack);
      trackEff_vs_theta2->SetMarkerColor(kBlack);
      trackEff_vs_theta2->SetMarkerStyle(20);
      trackEff_vs_theta2->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta2,"afterGeo_mergedTracks_bigErr", "lep");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();
      
      trackEff_vs_theta3->Draw("same");
      trackEff_vs_theta3->SetLineColor(kGreen);
      trackEff_vs_theta3->SetMarkerColor(kGreen);
      trackEff_vs_theta3->SetMarkerStyle(20);
      trackEff_vs_theta3->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_theta3,"afterGeo_mergedTracks_smallErr", "lep");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();
      
    }
    if(k==1){
      trackEff_vs_pT->Draw();
      trackEff_vs_pT->SetLineColor(kRed);
      trackEff_vs_pT->SetMarkerColor(kRed);
      trackEff_vs_pT->SetMarkerStyle(20);
      trackEff_vs_pT->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT,"beforeGeo_mergedTracks", "lep");
      
      trackEff_vs_pT1->Draw("same");
      trackEff_vs_pT1->SetLineColor(kBlue);
      trackEff_vs_pT1->SetMarkerColor(kBlue);
      trackEff_vs_pT1->SetMarkerStyle(20);
      trackEff_vs_pT1->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT1,"afterGeo_mergedTracks", "lep");
      
      trackEff_vs_pT2->Draw("same");
      trackEff_vs_pT2->SetLineColor(kBlack);
      trackEff_vs_pT2->SetMarkerColor(kBlack);
      trackEff_vs_pT2->SetMarkerStyle(20);
      trackEff_vs_pT2->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT2,"afterGeo_mergedTracks_bigErr", "lep");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();
      
      trackEff_vs_pT3->Draw("same");
      trackEff_vs_pT3->SetLineColor(kGreen);
      trackEff_vs_pT3->SetMarkerColor(kGreen);
      trackEff_vs_pT3->SetMarkerStyle(20);
      trackEff_vs_pT3->SetLineWidth(2);
      legend[k]->AddEntry(trackEff_vs_pT2,"afterGeo_mergedTracks_smallErr", "lep");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();
      
    }
    if(k==2){
      ratio_theta->SetTitle(
          ";prod_{#theta} [rad];Ratio"
      );
      ratio_theta->SetMarkerStyle(20);
      ratio_theta->SetMarkerColor(kBlue);
      ratio_theta->GetXaxis()->SetRangeUser(0,0.004);
      ratio_theta->Draw("AP");
      ratio_theta1->SetMarkerStyle(20);
      ratio_theta1->SetMarkerColor(kGreen);
      ratio_theta1->Draw("same");
      legend[k]->AddEntry(ratio_theta,"afterGeo/beforeGeo", "p");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();

      legend[k]->AddEntry(ratio_theta1,"afterGeo_smallerErr/beforeGeo", "p");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();
      TLine* line1 = new TLine(
          0,
          1.0,
          0.0033,
          1.0
      );
      
      line1->SetLineStyle(2);
      line1->Draw("same");
    }
    if(k==3){
      ratio_pT->SetTitle(
          ";prod_{p_{T}} [Mev/c];Ratio"
      );
      ratio_pT->GetXaxis()->SetRangeUser(0,1000);
      ratio_pT->SetMarkerStyle(20);
      ratio_pT->SetMarkerColor(kBlue);
      ratio_pT->Draw("AP");
      
      ratio_pT1->SetMarkerStyle(20);
      ratio_pT1->SetMarkerColor(kGreen);
      ratio_pT1->Draw("same");
      legend[k]->AddEntry(ratio_pT,"afterGeo/beforeGeo", "p");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();

      legend[k]->AddEntry(ratio_pT1,"afterGeo_smallerErr/beforeGeo", "p");
      legend[k]->SetTextSize(0.03);
      legend[k]->Draw();
      TLine* line2 = new TLine(
          0,
          1.0,
          890,
          1.0
      );
      
      line2->SetLineStyle(2);
      line2->Draw("same");
    }


  }
  //c->SaveAs("truthBased.pdf");
}

