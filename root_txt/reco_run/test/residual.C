#include<cmath>
//struct triplet{
//  double d1;
//  double d2;
//  double d3;
//};


void residual(){
  gStyle->SetOptStat(1110);
    
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
  TFile* f1 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_4station_backward_seed_summary_circleFitTrackSeedTool.root","Read");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;


  t1->SetBranchAddress("track_loc1", &v1);
  t1->SetBranchAddress("t_loc1", &v2);


  TH1D* h1 = new TH1D("h1",";loc1_{truth} - loc1_{reco} [mm];Events",50,-5,5);
  TH1D* h2 = new TH1D("h2",";loc1_{targetZ} [mm];Events",100,-150,150);
  TH1D* h3 = new TH1D("h3",";loc1_{targetZ} [mm];Events",100,-150,150);

  size_t entries = t1->GetEntries();
  for(size_t ievent = 0; ievent<entries; ++ievent){
    t1->GetEntry(ievent);
    TGraph* graph = new TGraph();
    if (v1->size() == v2->size() && v1->size() > 0){
      for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
        h1->Fill((v2->at(iTrack))-(v1->at(iTrack)));
        h2->Fill(v1->at(iTrack));
        h3->Fill(v2->at(iTrack));

      }
    }
    else{
  //    std::cerr <<"event"<<ievent<< ": Warning: No valid data points in this event (v1 or v2 is empty)!" << std::endl;
      continue;
    }
  }


  //TF1 *fit = new TF1("f1","gaus",-3,3);
  //fit->SetLineWidth(3);
  //h1->Fit(fit,"R");
  //double mean = fit->GetParameter(1);
  //double meanError = fit->GetParError(1);
  //double sigma = fit->GetParameter(2);
  //double sigmaError = fit->GetParError(2);
  

  h2->SetLineColor(596);
  h2->SetLineWidth(3);
  h3->SetLineColor(633);
 
  TCanvas* c1 = new TCanvas("c1","",600,500);
  c1->Divide(2,1);
  c1->cd(1);
  h1->SetLineWidth(2);
  h1->Draw();
  //TLegend *legend1 = new TLegend(0.6, 0.6, 0.9, 0.9); 
  //legend1->AddEntry(fit, TString::Format("#mu = %f #pm %f", mean, meanError), "");
  //legend1->AddEntry(fit, TString::Format("#sigma = %f #pm %f", sigma, sigmaError),"");
  //legend1->SetTextFont(62);
  //legend1->SetTextSize(0.02);
  //legend1->Draw();
  
  c1->cd(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  h2->Draw();
  h3->Draw("same");
  TLegend *legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend2->AddEntry(h2, "Reco", "l");
  legend2->AddEntry(h3, "Truth", "l");
  legend2->SetTextFont(62);
  legend2->Draw();
   
}
