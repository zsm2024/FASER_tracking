void ckf_residual_truthMinusFit (){
    
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat(0000);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.1);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetTitleSize(0.05, "xy");
    gStyle->SetLineWidth(2);
    gStyle->SetLabelSize(0.04, "xy");
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.9, "y");
    gStyle->SetNdivisions(510, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    TFile* file1= new TFile("total_3station_forward_track_summary_ckf.root","read");
    TTreeReader* Reader1 = new TTreeReader("tree",file1);
    TTree* t1 = Reader1->GetTree();
    
    std::vector<float> *v1 = nullptr; 
    std::vector<float> *v2 = nullptr; 
    std::vector<float> *v3 = nullptr; 
    std::vector<float> *v4 = nullptr; 
    std::vector<float> *v5 = nullptr; 
    std::vector<float> *v6 = nullptr; 
    t1->SetBranchAddress("res_eTHETA_fit",&v1);
    t1->SetBranchAddress("res_ePHI_fit",&v2);
    t1->SetBranchAddress("res_eLOC0_fit",&v3);
    t1->SetBranchAddress("res_eLOC1_fit",&v4);
    t1->SetBranchAddress("res_eQOP_fit",&v5);
    t1->SetBranchAddress("res_eT_fit",&v6);

    TH1F* h1 = new TH1F("h1",";theta_{truth-reco};Event",50,-0.01,0.01);
    TH1F* h2 = new TH1F("h2",";phi_{truth-reco};Event",50,-6,6);
    TH1F* h3 = new TH1F("h3",";loc0_{truth-reco};Event",50,-1,1);
    TH1F* h4 = new TH1F("h4",";loc1_{truth-reco};Event",50,-5,5);
    TH1F* h5 = new TH1F("h5",";QoverP_{truth-reco};Event",50,-0.1,0.1);
    //TH1F* h6 = new TH1F("h6",";time_{truth-reco};Event",50,-0.01,0.01);
    size_t entries = t1->GetEntries();
    for(size_t iEvent = 0; iEvent<entries; iEvent++){
      t1->GetEntry(iEvent);
      for(size_t iTrack = 0; iTrack<v1->size(); iTrack++){
        h1->Fill(-(v1->at(iTrack)));
        h2->Fill(-(v2->at(iTrack)));
        h3->Fill(-(v3->at(iTrack)));
        h4->Fill(-(v4->at(iTrack)));
        h5->Fill(-(v5->at(iTrack)));
        //h6->Fill(-(v6->at(iTrack)));
      }

    }
    TF1 *f1 = new TF1("f1","gaus",-0.006,0.006);
    f1->SetLineWidth(2);
    h1->Fit(f1,"R");
    
    TF1 *f2 = new TF1("f2","gaus",-2,2);
    f2->SetLineWidth(2);
    h2->Fit(f2,"R");
    
    TF1 *f3 = new TF1("f3","gaus",-0.4,0.4);
    f3->SetLineWidth(2);
    h3->Fit(f3,"R");
    
    TF1 *f4 = new TF1("f4","gaus",-2,2);
    f4->SetLineWidth(2);
    h4->Fit(f4,"R");
    
    TF1 *f5 = new TF1("f5","gaus",-0.04,0.04);
    f5->SetLineWidth(2);
    h5->Fit(f5,"R");
    
    std::vector<TH1F*> h={h1,h2,h3,h4,h5};
    std::vector<TLegend*> t;
    for(int i = 0; i<5; i++){
      TLegend* leg = new TLegend(0.7, 0.2, 0.9, 0.4);
      leg->AddEntry(h.at(i),"ckf_forward","l");
      leg->SetBorderSize(0);
      t.push_back(leg);
    }
    
    TCanvas* c1 = new TCanvas("c1","",1500,1400);
    c1->Divide(2,3);
    for(int j=0; j<5; j++){
      c1->cd(j+1);
      h[j]->SetMinimum(0);
      h[j]->SetMaximum(60000);
      h[j]->Draw();
      t[j]->Draw();
    }
}
