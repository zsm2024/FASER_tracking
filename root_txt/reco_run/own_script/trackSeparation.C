#include <cmath>



void trackSeparation(){

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


  TFile* file1 = TFile::Open("total1_4station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

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
  t1->SetBranchAddress("targetZPosition",&targetZ);
  t1->SetBranchAddress("nMeasurements",&v5);
  t1->SetBranchAddress("truthd0_x",&v1);
  t1->SetBranchAddress("truthd0_y",&v2);
  t1->SetBranchAddress("truthd1_x",&v3);
  t1->SetBranchAddress("truthd1_y",&v4);
  t1->SetBranchAddress("Positionx",&v6);
  t1->SetBranchAddress("Positiony",&v7);
  t1->SetBranchAddress("Positionz",&v8);
  const Int_t nbins = 15;
  double bins[nbins + 1] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 100, 200};
  //TH1F* h1 = new TH1F("h_total", ";#DeltaR1 [mm]; Events", nbins, bins);
  TEfficiency* eff = new TEfficiency("eff", ";#DeltaR1 [mm];Seeding Efficiency", nbins, bins);
  TH2F* h2 = new TH2F("h2",";#DeltaR1[mm];nMeasurements",88,0,220,30,0,30);
  TH2D* h3 = new TH2D("h3",";#DeltaR1[mm];PositionZ",88,0,220,28,-0,2800);
  TH2D* h4 = new TH2D("h4",";#DeltaR1[mm];r",88,0,220,20,0,200);
  TH1D* h5 = new TH1D("h5",";targetZ;Events",20,-5,15);

  //TEfficiency* eff = new TEfficiency("eff",";DeltaR1[mm];Seeding Efficiency",20,0,200);
  //TH1F* h1 = new TH1F("h1",";DeltaR1[mm];Events",50,0,200);

  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;
  

  for(size_t ievent = 0; ievent<entries; ievent++){
    
    t1->GetEntry(ievent);
    if(targetZ>1000)continue;
    h5->Fill(targetZ);

    if(v1->size()==0 || v3->size()==0)continue;

    bool passSeed = false;
    double deltaR;
    double x1 = v1->at(3);
    double y1 = v2->at(3);
    double x2 = v3->at(3);
    double y2 = v4->at(3);
    deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    //h1->Fill(deltaR);
    for(size_t i = 0; i < v5->size(); i++){
      h2->Fill(deltaR,v5->at(i));
    }
    for(size_t j = 0; j<nSeeds;j++){
      for(size_t k = 0; k<v8->size(); k++){

        h3->Fill(deltaR,v8->at(k));
        double x = v6->at(k);
        double y = v7->at(k);
        double r = sqrt(x*x+y*y);
        h4->Fill(deltaR,r);

      }
    }

    if(nSeeds < 1) continue;
    if(nSeeds >= 2){
      passSeed = true;
    }
    eff->Fill(passSeed,deltaR);

  }
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
//  cs->Divide(1,3);
//  cs->cd(1);
//  eff->SetMarkerStyle(20);
//  eff->SetMarkerSize(1.2);
//  eff->SetLineWidth(3);
//  eff->Draw();
//  cs->cd(2);
//  h3->SetLineWidth(4);
//  h3->Draw("colz");
//  cs->cd(3);
//  h5->SetLineWidth(4);
//  h5->Draw();
   cs->Divide(1);
   cs->cd();
   eff->SetMarkerStyle(20);
   eff->SetMarkerSize(1.2);
   eff->SetLineWidth(3);
   eff->Draw();



}
