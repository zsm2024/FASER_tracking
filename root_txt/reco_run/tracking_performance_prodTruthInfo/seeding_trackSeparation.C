#include <cmath>



void seeding_trackSeparation(){

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLineWidth(2);
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.1, "x");
  gStyle->SetTitleOffset(1.1, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);


  TFile* file1 = TFile::Open("total_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();
  uint32_t nSeeds = 0;
  double targetZ = 0;

  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr; 
  std::vector<double> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<double> *v6 = nullptr;
  std::vector<double> *v7 = nullptr;
  std::vector<double> *v8 = nullptr;
  


  t1->SetBranchAddress("nSeeds",&nSeeds);
  t1->SetBranchAddress("truthd0_x",&v1);
  t1->SetBranchAddress("truthd0_y",&v2);
  t1->SetBranchAddress("truthd1_x",&v3);
  t1->SetBranchAddress("truthd1_y",&v4);
  const Int_t nbins = 7;
  double bins[nbins + 1] = {0, 5, 10, 20, 30, 40, 50, 60};
  TEfficiency* eff = new TEfficiency("eff", ";#DeltaR1 [mm];Seeding Efficiency", nbins, bins);

  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;
  
  for(size_t ievent = 0; ievent<entries; ievent++){
    
    t1->GetEntry(ievent);

    if(v1->size()==0 || v3->size()==0)continue;

    bool passSeed = false;
    double deltaR;
    double x1 = v1->at(3);
    double y1 = v2->at(3);
    double x2 = v3->at(3);
    double y2 = v4->at(3);
    deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

    if(nSeeds < 1) continue;
    if(deltaR > 60) continue;
    if(nSeeds >= 2){
      passSeed = true;
    }
    eff->Fill(passSeed,deltaR);

  }



  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->cd();
  eff->SetMarkerStyle(20);
  eff->SetMarkerSize(0.8);
  eff->SetLineWidth(2);
  eff->Draw();



}
