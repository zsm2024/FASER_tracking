#include<cmath>
//struct triplet{
//  double d1;
//  double d2;
//  double d3;
//};


void test_p(){
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
  TFile* f1 = TFile::Open("total_3station_forward_track_states_ckf.root","Read");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();

  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<float>  *v3 = nullptr;
  std::vector<float>  *v4 = nullptr;
  std::vector<int>  *v5 = nullptr;


  t1->SetBranchAddress("eQOP_smt", &v1);
  t1->SetBranchAddress("t_eQOP", &v2);
  t1->SetBranchAddress("py_smt", &v3);
  t1->SetBranchAddress("pz_smt", &v4);
  t1->SetBranchAddress("volume_id", &v5);


  TH1D* h1 = new TH1D("h1",";loc1_{truth} - loc1_{reco} [mm];Events",50,-5,5);
  TH1D* h2 = new TH1D("h2",";deltaP/P;Events",100,-1,1);
  TH1D* h3 = new TH1D("h3",";tan{theta}_{y} ;Events",100,-4,4);
  TH2D* h4 = new TH2D("h4",";p_{truth} [Gev];deltaP/P",100,-1000,1000,100,-1.5,1.5);

  size_t entries = t1->GetEntries();
  for(size_t ievent = 0; ievent<entries; ++ievent){
    t1->GetEntry(ievent);

   
      for(size_t iState = 0; iState < v1->size(); ++iState){
        //std::cout<<"track_QOP = "<<v1->at(iState)<<std::endl;
        //std::cout<<"t_QOP = "<<v2->at(iState)<<std::endl;
        //std::cout<<"track_p = "<<(1./v1->at(iState))<<";   "<<"t_p = "<<1./v2->at(iState)<<std::endl;
        //std::cout<<"deltaP = "<<(1./v1->at(iState))-(1./v2->at(iState)/1000)<<std::endl;
        //std::cout<<"deltaP / P= "<<((1./v1->at(iState))-(1./v2->at(iState)/1000))/(1./v2->at(iState)/1000)<<std::endl;
        h4->Fill(1./v2->at(iState)/1000 , ((1./v1->at(iState))-(1./v2->at(iState)/1000))/(1./v2->at(iState)/1000));
        if(v5->at(iState) == 25){
          h3->Fill(tan(v3->at(iState))/tan(v4->at(iState)));
        }

      }
   
  }


  //h2->SetLineColor(596);
  //h2->SetLineWidth(3);
  //h3->SetLineColor(633);

  TCanvas* c1 = new TCanvas("c1","",600,500);
  c1->Divide(2,1);
  //h2->SetLineWidth(2);
  c1->cd(1);
  h4->Draw("colz");
  c1->cd(2);
  h3->Draw();

  //c1->cd(2);
  //h2->SetLineWidth(2);
  //h3->SetLineWidth(2);
  //h2->Draw();
  //h3->Draw("same");
  //TLegend *legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
  //legend2->AddEntry(h2, "Reco", "l");
  //legend2->AddEntry(h3, "Truth", "l");
  //legend2->SetTextFont(62);
  //legend2->Draw();

}
