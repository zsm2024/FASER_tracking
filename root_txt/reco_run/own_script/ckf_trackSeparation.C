#include <cmath>



void ckf_trackSeparation(){

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLineWidth(4);
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetLabelSize(0.03, "Z");


  TFile* file1 = TFile::Open("total1_3station_backward_track_summary_ckf.root", "read");
  TFile* file2 = TFile::Open("total1_3station_backward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();

  TTreeReader* Reader2 = new TTreeReader("tree", file2);
  TTree* t2 = Reader2->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr; 
  std::vector<double> *v4 = nullptr;
  std::vector<uint32_t> *v5 = nullptr;
  std::vector<unsigned int>* v6 = nullptr;
  std::vector<double>* v7 = nullptr;
  std::vector<unsigned int>* v8 = nullptr;
  std::vector<unsigned int>* v9 = nullptr;
  std::vector<unsigned int>* v10 = nullptr;
  std::vector<unsigned int>* v11 = nullptr;
  std::vector<std::vector<double>>* v12 = nullptr;
  std::vector<std::vector<double>>* v13 = nullptr;
  std::vector<float>* v14 = nullptr;
  std::vector<unsigned int>* v15 = nullptr;
  uint32_t nSeeds = 0;
  uint32_t event_nr = 0;


  std::vector<double> *v16 = nullptr;
  std::vector<double> *v17 = nullptr;
  std::vector<double> *v18 = nullptr;
  
  t2->SetBranchAddress("Positionx",&v16);
  t2->SetBranchAddress("Positiony",&v17);
  t2->SetBranchAddress("Positionz",&v18);
  t2->SetBranchAddress("nSeeds",&nSeeds);


  t2->SetBranchAddress("truthd0_x",&v1);
  t2->SetBranchAddress("truthd0_y",&v2);
  t2->SetBranchAddress("truthd1_x",&v3);
  t2->SetBranchAddress("truthd1_y",&v4);
  t1->SetBranchAddress("track_nr",&v5);
  t1->SetBranchAddress("nMeasurements",&v6);
  t1->SetBranchAddress("rMax_hit",&v7);
  t1->SetBranchAddress("nOutliers",&v8);
  t1->SetBranchAddress("nHoles",&v9);
  t1->SetBranchAddress("nSharedHits",&v10);
  t1->SetBranchAddress("nStates",&v11);
  t1->SetBranchAddress("measurementLayer",&v12);
  t1->SetBranchAddress("measurementVolume",&v13);
  t1->SetBranchAddress("chi2Sum",&v14);
  t1->SetBranchAddress("NDF",&v15);
  t1->SetBranchAddress("event_nr",&event_nr);



  const Int_t nbins = 19;
  double bins[nbins + 1] = {0, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 100, 200};
  TH1F* h1 = new TH1F("h_total", ";#DeltaR1 [mm]; Events", nbins, bins);
  TEfficiency* eff = new TEfficiency("eff", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
  //TEfficiency* eff = new TEfficiency("eff",";DeltaR1[mm];CKF Efficiency",20,0,200);
//  TH1F* h1 = new TH1F("h1",";DeltaR1[mm];Events",20,0,200);
  TH2F* h2 = new TH2F("h2",";#DeltaR1[mm];nMeasurements",88,0,220,30,0,30);
  TH2F* h3 = new TH2F("h3",";#DeltaR1[mm];nTracks",88,0,220,20,0,20);
  TH2F* h4 = new TH2F("h4",";#DeltaR1[mm];rMaxHit",88,0,220,200,50,250);
  TH2F* h5 = new TH2F("h5",";#DeltaR1[mm];nOutliers",88,0,220,20,0,20);
  TH2F* h6 = new TH2F("h6",";#DeltaR1[mm];nHoles",88,0,220,20,0,20);
  TH2F* h7 = new TH2F("h7",";#DeltaR1[mm];#chi^2/NDF",88,0,220,20,0,20);
  TH2F* h8 = new TH2F("h8",";#DeltaR1[mm];nStates",88,0,220,20,0,20);
  TH2D* h9 = new TH2D("h9",";separation;volume&layer",22,0,220,10,250,300);
  TH2D* h10 = new TH2D("h10",";DeltaR;r",88,0,220,20,0,200);
  //TProfile* p = new TProfile("p", ";DeltaR1[mm];nMeasurements", 10, 0, 200);

  size_t entries = t1->GetEntries();
  std::cout<<"events: "<<entries<<std::endl;

  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);
    t2->GetEntry(ievent);
    int tracks = v5->size();

    if(v1->size()==0||v3->size()==0)continue;
    bool passTrack = false;
    double deltaR;
    double x1 = v1->at(3);
    double y1 = v2->at(3);
    double x2 = v3->at(3);
    double y2 = v4->at(3);
    deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    if(deltaR<180) continue;
    h3->Fill(deltaR,tracks);
    h8->Fill(deltaR,v11->size());

    for(size_t iTrack = 0; iTrack<v6->size(); iTrack++){
      h2->Fill(deltaR,v6->at(iTrack));
      h4->Fill(deltaR,v7->at(iTrack));
      h5->Fill(deltaR,v8->at(iTrack));
      h6->Fill(deltaR,v9->at(iTrack));
      h7->Fill(deltaR,v14->at(iTrack)/v15->at(iTrack));

      for(size_t k = 0; k<v12->at(iTrack).size(); k++){
      //  std::cout<<v13->at(iTrack).at(k)<<v12->at(iTrack).at(k)<<"; ";
        h9->Fill(deltaR,(v13->at(iTrack).at(k))*10+(v12->at(iTrack).at(k)));
      }
      //std::cout<<std::endl;

    }
//    h1->Fill(deltaR);

    if(tracks < 1)continue;
    if(tracks >= 2){
      passTrack = true;
     // for(size_t iTrack = 0; iTrack<v6->size(); iTrack++){

     //   for(size_t k = 0; k<v12->at(iTrack).size(); k++){
     //   //  std::cout<<v13->at(iTrack).at(k)<<v12->at(iTrack).at(k)<<"; ";
     //     h10->Fill(v13->at(iTrack).at(k),v12->at(iTrack).at(k));
     //   }
     //   //std::cout<<std::endl;

     // }
        
    }
    if(passTrack == false){
     // for(size_t j = 0; j<nSeeds;j++){
     //   for(size_t k = 0; k<v18->size(); k++){

     //     //h3->Fill(deltaR,v8->at(k));
     //     double x = v16->at(k);
     //     double y = v17->at(k);
     //     double z = v18->at(k);
     //     double r = sqrt(x*x+y*y);
     //     std::cout<<z<<","<<r<<";";
     //     h10->Fill(deltaR,r);

     //   }
     //   std::cout<<std::endl;
     // }

      std::cout<<"event:"<<ievent<<";"<<"event_nr: "<<event_nr<<std::endl;
    }
    std::cout<<"---------------------------------"<<std::endl;
    eff->Fill(passTrack,deltaR);
  }
  //TF1* f1 = new TF1("f1", "pol1", 0, 200);
  //p->Fit(f1, "R");
  for (int i = 13; i <= nbins; ++i) {
    int passed = eff->GetPassedHistogram()->GetBinContent(i);
    int total  = eff->GetTotalHistogram()->GetBinContent(i);
    double eff_val = eff->GetEfficiency(i);

   // std::cout << "Bin " << i
   //           << ": passed = " << passed
   //           << ", total = " << total
   //           << ", efficiency = " << eff_val
   //           << std::endl;
}


  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->Divide(4,2);

  cs->cd(1);
  eff->SetMarkerStyle(20);
  eff->SetMarkerSize(1.2);
  eff->SetLineWidth(3);
  eff->Draw();
  cs->cd(2);
  h2->SetLineWidth(4);
  h2->Draw("colz");
  cs->cd(3);
  h3->SetLineWidth(4);
  h3->Draw("colz");
  cs->cd(4);
  h4->SetLineWidth(4);
  h4->Draw("colz");
  cs->cd(5);
  h5->SetLineWidth(4);
  h5->Draw("colz");
  cs->cd(6);
  h6->SetLineWidth(4);
  h6->Draw("colz");
  cs->cd(7);
  h7->SetLineWidth(4);
  h7->Draw("colz TEXT");
  cs->cd(8);
  h8->SetLineWidth(4);
  h8->Draw("colz ");
  //p->Fit("pol2","R");


  /*TLegend* legend = new TLegend(0.8, 0.2, 0.95, 0.4);
  legend->AddEntry(trackEff1, "trackeff", "lp");
  legend->AddEntry(trackEff2, "trackeff(non 0 nSeeds)", "lp");
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.03);
  legend->Draw();*/


}
