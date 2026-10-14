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


  TFile* file1 = TFile::Open("110033_officialNt_PHYS.root", "read");

  TTreeReader* Reader1 = new TTreeReader("nt",file1);
  TTree* t1 = Reader1->GetTree();

  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr;
  std::vector<double> *v4 = nullptr;
  std::vector<unsigned long> *v5 = nullptr;
  

  t1->SetBranchAddress("truthd0_x",&v1);
  t1->SetBranchAddress("truthd0_y",&v2);
  t1->SetBranchAddress("truthd1_x",&v3);
  t1->SetBranchAddress("truthd1_y",&v4);
  t1->SetBranchAddress("Track_hitSet",&v5);



  const Int_t nbins = 7;
  double bins[nbins + 1] = {0, 10, 20, 30, 40, 60, 100, 200}; 
  TEfficiency* eff = new TEfficiency("eff", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);

  size_t entries = t1->GetEntries();
  std::cout<<"events: "<<entries<<std::endl;

  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);

    int tracks = v5->size();
    //std::cout<<"number of tracks = "<<tracks<<std::endl;

    if(v1->size()==0||v3->size()==0)continue;
    bool passTrack = false;
    double deltaR;
    double x1 = v1->at(3);
    double y1 = v2->at(3);
    double x2 = v3->at(3);
    double y2 = v4->at(3);
    deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));


    if(tracks < 1)continue;
    if(tracks >= 2){
      passTrack = true;
        
    }
    eff->Fill(passTrack,deltaR);
  }


  TCanvas* cs = new TCanvas("c1","efficiency",600,500);

  cs->cd();
  eff->SetMarkerStyle(21);
  eff->SetMarkerSize(0.8);
  eff->SetLineWidth(2);
  eff->Draw();


}

