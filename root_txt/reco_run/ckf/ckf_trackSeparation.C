#include <cmath>



void ckf_trackSeparation(){

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleSize(0.05, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.2, "x");
  gStyle->SetTitleOffset(1.2, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);


  TFile* file1 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_track_summary_ckf.root", "read");
  TFile* file2 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008-PHYS.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();

  TTreeReader* Reader2 = new TTreeReader("nt", file2);
  TTree* t2 = Reader2->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr; 
  std::vector<double> *v4 = nullptr;
  std::vector<uint32_t> *v5 = nullptr;




  t2->SetBranchAddress("truthd0_x",&v1);
  t2->SetBranchAddress("truthd0_y",&v2);
  t2->SetBranchAddress("truthd1_x",&v3);
  t2->SetBranchAddress("truthd1_y",&v4);
  t1->SetBranchAddress("track_nr",&v5);




  TEfficiency* eff = new TEfficiency("eff",";DeltaR1[mm];CKF Efficiency",20,0,20);

  size_t entries = t1->GetEntries();
  std::cout<<"events: "<<entries<<std::endl;

  for(size_t ievent = 0; ievent<entries; ievent++){
    t1->GetEntry(ievent);
    t2->GetEntry(ievent);
    int tracks = v5->size();
    if(tracks < 1)continue;

    bool passTrack = false;
    double deltaR;
    double x1 = v1->at(3);
    double y1 = v2->at(3);
    double x2 = v3->at(3);
    double y2 = v4->at(3);
    deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    
    if(tracks >= 2){
      passTrack = true;
    }
    eff->Fill(passTrack,deltaR);
  }


  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->cd();
  eff->Draw();

}
