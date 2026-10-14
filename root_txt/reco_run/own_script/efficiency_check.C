#include <cmath>



void efficiency_check(){

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


  TFile* file1 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00090-00099-s0008_3station_forward_track_summary_ckf.root", "read");
  TFile* file2 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00090-00099-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root", "read");

  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();

  TTreeReader* Reader2 = new TTreeReader("tree", file2);
  TTree* t2 = Reader2->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr;
  std::vector<double> *v4 = nullptr;
  std::vector<uint32_t> *v5 = nullptr;
  std::vector<double>* v6 = nullptr;
  std::vector<double>* v7 = nullptr;
  std::vector<double>* v8 = nullptr;
  uint32_t event_nr = 0;


  std::vector<double> *v16 = nullptr;
  std::vector<double> *v17 = nullptr;
  std::vector<double> *v18 = nullptr;

  t2->SetBranchAddress("mea_x",&v6);
  t2->SetBranchAddress("mea_y",&v7);
  t2->SetBranchAddress("mea_z",&v8);


  t2->SetBranchAddress("truthd0_x",&v1);
  t2->SetBranchAddress("truthd0_y",&v2);
  t2->SetBranchAddress("truthd1_x",&v3);
  t2->SetBranchAddress("truthd1_y",&v4);
  t1->SetBranchAddress("track_nr",&v5);
  t1->SetBranchAddress("event_nr",&event_nr);



  const Int_t nbins = 19;
  double bins[nbins + 1] = {0, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 100, 200};
  TEfficiency* eff = new TEfficiency("eff", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
  TH2D* h1 = new TH2D("h1",";st1_posZ;x_y",100,-10,100,30,40,160);
  TH2D* h2 = new TH2D("h2",";st2_posZ;x_y",100,1180,1300,30,40,160);
  TH2D* h3 = new TH2D("h3",";st3_posZ;x_y",100,2380,2480,30,40,160);
  TH2D* h4 = new TH2D("h4",";Z;x_y",100,-10,2480,30,40,160);

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
    //for(size_t i = 0; i < v6->size();i++){
    //  auto x = v6->at(i);
    //  auto y = v7->at(i);
    //  auto z = v8->at(i);
    //  auto r = sqrt(x*x+y*y);
    //  if(z<500){
    //    
    //    h1->Fill(z,r);
    //  }
    //  if(500<z<2000){h2->Fill(z,r);}
    //  if(z>2000){h3->Fill(z,r);}

    //}
    double sum_x1 = 0, sum_y1 = 0, sum_z1 = 0;
    double sum_x2 = 0, sum_y2 = 0, sum_z2 = 0;
    double sum_x3 = 0, sum_y3 = 0, sum_z3 = 0;
    
    int count1 = 0, count2 = 0, count3 = 0;
    
    for (size_t i = 0; i < v6->size(); i++) {
        auto x = v6->at(i);
        auto y = v7->at(i);
        auto z = v8->at(i);
    
        if (z < 500) {
            sum_x1 += x;
            sum_y1 += y;
            sum_z1 += z;
            count1++;
        } else if (z<2000) {
            sum_x2 += x;
            sum_y2 += y;
            sum_z2 += z;
            count2++;
        } else {
            sum_x3 += x;
            sum_y3 += y;
            sum_z3 += z;
            count3++;
        }
    }
    std::cout<<count1<<";"<<count2<<";"<<count3<<std::endl;
    if(count1==0||count2==0||count3==0)continue;
    double r1 = sqrt((sum_x1/count1)*(sum_x1/count1)+(sum_y1/count1)*(sum_y1/count1));
    double r2 = sqrt((sum_x2/count2)*(sum_x2/count2)+(sum_y2/count2)*(sum_y2/count2));
    double r3 = sqrt((sum_x3/count3)*(sum_x3/count3)+(sum_y3/count3)*(sum_y3/count3));
    std::cout<<"z1"<<sum_z1<<";"<<"r1:"<<r1<<std::endl;
    std::cout<<"z2"<<sum_z2<<";"<<"r2:"<<r2<<std::endl;
    std::cout<<"z3"<<sum_z3<<";"<<"r3:"<<r3<<std::endl;
    h4->Fill(sum_z1/count1,r1);
    h4->Fill(sum_z2/count2,r2);
    h4->Fill(sum_z3/count3,r3);




    if(tracks < 1)continue;
    if(tracks >= 2){
      passTrack = true;

    }
    if(passTrack == false){
      std::cout<<"event:"<<ievent<<";"<<"event_nr: "<<event_nr<<std::endl;
    }
    std::cout<<"---------------------------------"<<std::endl;
    eff->Fill(passTrack,deltaR);
  }



  TCanvas* cs = new TCanvas("c1","efficiency",600,500);
  cs->Divide(3,2);

  cs->cd(1);
  eff->SetMarkerStyle(20);
  eff->SetMarkerSize(1.2);
  eff->SetLineWidth(3);
  eff->Draw();
  cs->cd(2);
  h4->SetLineWidth(4);
  h4->Draw("");
  cs->cd(4);
  h1->SetLineWidth(4);
  h1->Draw("colz");
  cs->cd(5);
  h2->SetLineWidth(4);
  h2->Draw("colz");
  cs->cd(6);
  h3->SetLineWidth(4);
  h3->Draw("colz");


}
                                                                                  

