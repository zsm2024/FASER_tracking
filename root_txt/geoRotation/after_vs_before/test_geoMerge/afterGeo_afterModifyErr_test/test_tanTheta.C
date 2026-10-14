#include<cmath>
//struct triplet{
//  double d1;
//  double d2;
//  double d3;
//};


void test_tanTheta(){
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
  TFile* f1 = TFile::Open("110033_ntOut_officialRec_3st_forward_new_PHYS.root","Read");
  TTreeReader* Reader1 = new TTreeReader("nt",f1);
  TTree* t1 = Reader1->GetTree();
  TFile* f2 = TFile::Open("110033_smallerCov_seedParams_3stForward_new_PHYS.root","Read");
  TTreeReader* Reader2 = new TTreeReader("nt",f2);
  TTree* t2 = Reader2->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr;
  std::vector<double> *v4 = nullptr;
  std::vector<int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<double> *v7 = nullptr;
  std::vector<double> *v8 = nullptr;
  std::vector<int> *v9 = nullptr;
  std::vector<int> *v10 = nullptr;
  std::vector<double> *v11 = nullptr;
  std::vector<double> *v12 = nullptr;
  std::vector<int> *v13 = nullptr;
  std::vector<int> *v14 = nullptr;
  std::vector<double> *v15 = nullptr;
  std::vector<double> *v16 = nullptr;
  std::vector<double> *v17 = nullptr;
  std::vector<double> *v18 = nullptr;


  t1->SetBranchAddress("Track_ThetaX_atVetoNu", &v1);
  t1->SetBranchAddress("Track_ThetaY_atVetoNu", &v2);
  t1->SetBranchAddress("Track_charge", &v5);
  t1->SetBranchAddress("t_st1_py", &v7);
  t1->SetBranchAddress("t_st1_pz", &v8);
  t1->SetBranchAddress("t_pdg", &v9);
  t1->SetBranchAddress("t_barcode", &v10);
  t1->SetBranchAddress("Track_pz0", &v15);
  t1->SetBranchAddress("t_p", &v17);
  
  t2->SetBranchAddress("Track_X_atVetoNu", &v3);
  t2->SetBranchAddress("Track_Y_atVetoNu", &v4);
  t2->SetBranchAddress("Track_charge", &v6);
  t2->SetBranchAddress("t_st1_py", &v11);
  t2->SetBranchAddress("t_st1_pz", &v12);
  t2->SetBranchAddress("t_pdg", &v13);
  t2->SetBranchAddress("t_barcode", &v14);
  t2->SetBranchAddress("Track_p0", &v16);
  t2->SetBranchAddress("t_p", &v18);


  TH1D* h1 = new TH1D("h1",";tan#theta_{y}_beforeGeo ;Events",100,-0.01,0.01);
  TH1D* h3 = new TH1D("h3",";tan#theta_{y}_beforeGeo ;Events",50,-0.003,0.003);
  TH1D* h5 = new TH1D("h5",";tan#theta_{y}_beforeGeo ;Events",50,-0.003,0.003);
  TH1D* h6 = new TH1D("h6",";tan#theta_{y}_beforeGeo ;Events",50,-0.003,0.003);
  
  TH1D* h2 = new TH1D("h2",";tan#theta_{y}_afterGeo ;Events",50,-0.01,0.01);
  TH1D* h4 = new TH1D("h4",";tan#theta_{y}_afterGeo ;Events",50,-0.003,0.003);
  TH1D* h7 = new TH1D("h7",";tan#theta_{y}_afterGeo ;Events",50,-0.003,0.003);
  TH1D* h8 = new TH1D("h8",";tan#theta_{y}_afterGeo ;Events",50,-0.003,0.003);

  size_t entries1 = t1->GetEntries();
  for(size_t ievent = 0; ievent<entries1; ++ievent){
    t1->GetEntry(ievent);

    
    for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
      if(!(100<(v15->at(iTrack)/1000) && (v15->at(iTrack)/1000)<250) )continue;
      //if(v5->at(iTrack) == 1){
        h1->Fill(v1->at(iTrack));
      //}else{
      //  h3->Fill(v1->at(iTrack)/v2->at(iTrack));
      //}

    }
    //for (size_t iParticle = 0; iParticle < v7->size(); iParticle++){
    //  if(!(100<(v17->at(iParticle)/1000) && (v17->at(iParticle)/1000)<250) )continue;
    //  //track truth 
    //  if(v10->at(iParticle) == 1 || v10->at(iParticle) == 2 || v10->at(iParticle) == 3){
    //    if(v9->at(iParticle) == -11){
    //      h5->Fill(v7->at(iParticle)/v8->at(iParticle));
    //    }else{
    //      h6->Fill(v7->at(iParticle)/v8->at(iParticle));
    //    }
    //  }
    //}



  }
  size_t entries2 = t2->GetEntries();
  for(size_t ievent = 0; ievent<entries2; ++ievent){
    t2->GetEntry(ievent);
    for(size_t iTrack = 0; iTrack < v3->size(); ++iTrack){
      //if(!(100<(v16->at(iTrack)/1000) && (v16->at(iTrack)/1000)<250) )continue;
      //if(v6->at(iTrack) == 1){
        //h2->Fill(v3->at(iTrack)/v4->at(iTrack));
        double Track_Z_atVetoNu = sqrt(v3->at(iTrack)*v3->at(iTrack) + v4->at(iTrack)*v4->at(iTrack));
        h2->Fill(v4->at(iTrack)/Track_Z_atVetoNu);
      //}else{
      //  h4->Fill(v3->at(iTrack)/v4->at(iTrack));
      //}

    }
    //for (size_t iParticle = 0; iParticle < v11->size(); iParticle++){
    //  //track truth 
    //  if(v14->at(iParticle) == 1 || v14->at(iParticle) == 2 || v14->at(iParticle) == 3){
    //  if(!(100<(v18->at(iParticle)/1000) && (v18->at(iParticle)/1000)<250) )continue;
    //    if(v13->at(iParticle) == -11){
    //      h7->Fill(v11->at(iParticle)/v12->at(iParticle));
    //    }else{
    //      h8->Fill(v11->at(iParticle)/v12->at(iParticle));
    //    }
    //  }
    //}
  }



  //TF1 *fit = new TF1("f1","gaus",-3,3);
  //fit->SetLineWidth(3);
  //h1->Fit(fit,"R");
  //double mean = fit->GetParameter(1);
  //double meanError = fit->GetParError(1);
  //double sigma = fit->GetParameter(2);
  //double sigmaError = fit->GetParError(2);
  
  //reco_e+
  h1->SetLineColor(kBlue);
  h1->SetLineWidth(2);
  h2->SetLineColor(kRed);
  h2->SetLineWidth(2);
  
  //reco_e-
  h3->SetLineColor(kRed);
  h3->SetLineWidth(2);
  h4->SetLineColor(kRed);
  h4->SetLineWidth(2);
  
  //truth_e+
  h5->SetLineColor(kBlack);
  h5->SetLineWidth(2);
  h7->SetLineColor(kBlack);
  h7->SetLineWidth(2);
  
  //truth_e-
  h6->SetLineColor(kGreen);
  h6->SetLineWidth(2);
  h8->SetLineColor(kGreen);
  h8->SetLineWidth(2);

 
  TCanvas* c1 = new TCanvas("c1","",400,500);
  //c1->Divide(1,2);
  c1->cd();
  h1->Draw();
  //h2->Draw("same");
  //h3->Draw("same");
  //h5->Draw("same");
  //h6->Draw("same");
 // TLegend *legend1 = new TLegend(0.6, 0.6, 0.9, 0.9); 
 // legend1->AddEntry(h1, "beforeGeo", "l");
 // legend1->AddEntry(h2, "afterGeo","l");
 // //legend1->AddEntry(h5, "truth_e+","l");
 // //legend1->AddEntry(h6, "truth_e-","l");
 // legend1->SetTextFont(62);
 // legend1->SetTextSize(0.05);
 // legend1->Draw();
  
  //c1->cd(2);
  //h2->Draw();
  ////h4->Draw("same");
  ////h7->Draw("same");
  ////h8->Draw("same");
  //TLegend *legend2 = new TLegend(0.6, 0.6, 0.9, 0.9); 
  //legend2->AddEntry(h2, "afterGeo_e+", "l");
  //legend2->AddEntry(h4, "afterGeo_e-","l");
  //legend2->AddEntry(h7, "truth_e+","l");
  //legend2->AddEntry(h8, "truth_e-","l");
  //legend2->SetTextFont(62);
  //legend2->SetTextSize(0.05);
  //legend2->Draw();
  
  
   
}
