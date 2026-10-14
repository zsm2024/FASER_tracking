#include<cmath>
//struct triplet{
//  double d1;
//  double d2;
//  double d3;
//};


void draw(){
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
  
  //TFile* f1 = TFile::Open("/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/muonSample/afterGeo/FaserMC-MC24_PG_mupl_fasernu_5mrad_flukaE-100044-00000-00099-s0013_3station_forward_track_summary_ckf.root");
  TFile* f1 = TFile::Open("/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/A_afterGeo/110051_mc24_afterGeo_3station_forward_track_summary_ckf.root");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  std::vector<unsigned int> *v1 = nullptr;


//  t1->SetBranchAddress("inputMeasurementNumParticles", &v1);
  t1->SetBranchAddress("inputMeasurementRDOSize", &v1);



  TH1D* h1 = new TH1D("h1",";inputMeasurementNumParticles;Events",10,0,10);

  size_t entries = t1->GetEntries();
  for(size_t ievent = 0; ievent<entries; ++ievent){
    t1->GetEntry(ievent);
      for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
        //if(v1->at(iTrack)!=1 && v1->at(iTrack)!=0){

        //  std::cout<<"inputMeasurementNumParticles = "<<v1->at(iTrack)<<std::endl;
        //}
         // std::cout<<"inputMeasurementNumParticles = "<<v1->at(iTrack)<<std::endl;
        h1->Fill(v1->at(iTrack));

      }
  }
  TCanvas* c1 = new TCanvas("c1","",600,500);
  //c1->Divide(2,1);
  c1->cd();
  h1->SetLineWidth(2);
  h1->Draw();
  //TLegend *legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
  //legend2->AddEntry(h2, "Reco", "l");
  //legend2->AddEntry(h3, "Truth", "l");
  //legend2->SetTextFont(62);
  //legend2->Draw();
}
