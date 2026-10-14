#include<cmath>
//struct triplet{
//  double d1;
//  double d2;
//  double d3;
//};


void residual(){
  //gStyle->SetOptStat(1110);
    
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
  //TFile* f1 = TFile::Open("/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/muonSample/FaserMC-MC24_PG_mupl_fasernu_5mrad_flukaE-100044-00000-00099-s0013_3station_forward_seed_summary_circleFitTrackSeedTool.root","Read");
  //TFile* f1 = TFile::Open("/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/beforeGeo_mc24_110033_newRecoScipt_3station_forward_seed_summary_circleFitTrackSeedTool.root","Read");
  //TFile* f1 = TFile::Open("/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/muonSample/afterGeo/FaserMC-MC24_PG_mupl_fasernu_5mrad_flukaE-100044-00000-00099-s0013_3station_forward_seed_summary_circleFitTrackSeedTool.root");

 TFile* f1 = TFile::Open("/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/A_afterGeo/110033_mc24_afterGeo_3station_forward_seed_summary_circleFitTrackSeedTool.root");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;


  t1->SetBranchAddress("track_QoverP", &v1);
  t1->SetBranchAddress("t_QoverP", &v2);


  TH1D* h1 = new TH1D("h1",";(p^{reco} - p^{truth})/p^{truth};Events",100,-1.5,1.5);
  TH1D* h2 = new TH1D("h2",";loc0_{targetZ} [mm];Events",100,-150,150);
  TH1D* h3 = new TH1D("h3",";loc0_{targetZ} [mm];Events",100,-150,150);
  TH2D* h4 = new TH2D("h4",";QOP_{reco};QOP_{truth}",100,-0.01,0.01,100,-0.01,0.01);

  size_t entries = t1->GetEntries();
  for(size_t ievent = 0; ievent<entries; ++ievent){
    t1->GetEntry(ievent);
    TGraph* graph = new TGraph();
    if (v1->size() == v2->size() && v1->size() > 0){
      for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
        //if(((1./abs(v1->at(iTrack)) - 1./abs(v2->at(iTrack)))/(1./abs(v2->at(iTrack))))<=-0.9){
          //std::cout<<"μ+ sample seed momentum ::truth QOP = "<<v2->at(iTrack)<<"; reco QOP = "<<v1->at(iTrack)<<std::endl;
          h1->Fill((1./abs(v1->at(iTrack)) - 1./abs(v2->at(iTrack)))/(1./abs(v2->at(iTrack))));
          h4->Fill(v1->at(iTrack),v2->at(iTrack));
        //}
        //h1->Fill((1/abs((v1->at(iTrack)))/1000-1/abs((v2->at(iTrack)))/1000)/1/abs((v2->at(iTrack)))/1000);
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
  c1->Divide(2,2);
  c1->cd(1);
  h1->SetLineWidth(2);
  h1->Draw();
  c1->cd(2);
  h4->Draw("COLZ");
  //TLegend *legend1 = new TLegend(0.6, 0.6, 0.9, 0.9); 
  //legend1->AddEntry(fit, TString::Format("#mu = %f #pm %f", mean, meanError), "");
  //legend1->AddEntry(fit, TString::Format("#sigma = %f #pm %f", sigma, sigmaError),"");
  //legend1->SetTextFont(62);
  //legend1->SetTextSize(0.02);
  //legend1->Draw();
  
  c1->cd(3);
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
