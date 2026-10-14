void actsVsAtlas() {

  TFile* f1 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00190-00199-s0008-PHYS.root","read");
  TTreeReader* Reader1 = new TTreeReader("nt",f1);
  TTree* t1 = Reader1->GetTree();
  
  TFile* f2 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00190-00199-s0008_3station_forward_track_summary_ckf.root","read");
  TTreeReader* Reader2 = new TTreeReader("tree",f2);
  TTree* t2 = Reader2->GetTree();
  

  std::vector<unsigned long> *v1 = nullptr;
  std::vector<unsigned int> *v2 = nullptr;
  std::vector<uint64_t> *v3 = nullptr;
  std::vector<uint32_t> *v4 = nullptr;
  std::vector<unsigned int> *v5 = nullptr;
  std::vector<unsigned int> *v6 = nullptr;

  t1->SetBranchAddress("Track_hitSet",&v1);
  t1->SetBranchAddress("Track_nMeasurements",&v2);
  t1->SetBranchAddress("hitCounts",&v3);
  
  t2->SetBranchAddress("track_nr",&v4);
  t2->SetBranchAddress("nMeasurements",&v5);
  t2->SetBranchAddress("nMajorityHits",&v6);

  TH1I* h1 = new TH1I("h1",";nTrack;Events",10,0,10);
  TH1I* h2 = new TH1I("h2",";nMeasurements;Events",50,0,50);
  TH1I* h3 = new TH1I("h3",";nMajorityHits;Events",50,0,50);
  
  TH1I* h4 = new TH1I("h4",";nTrack;Events",10,0,10);
  TH1I* h5 = new TH1I("h5",";nMeasurements;Events",50,0,50);
  TH1I* h6 = new TH1I("h6",";nMajorityHits;Events",50,0,50);


  std::vector<TH1I*> vs1 = {h1, h2, h3};
  std::vector<TH1I*> vs2 = {h4, h5, h6};
  size_t entries = t1->GetEntries();
  std::cout<<"entries = "<<entries<<std::endl;
  for(size_t iEvent = 0; iEvent<entries; iEvent++){
    t1->GetEntry(iEvent);
    t2->GetEntry(iEvent);

   //if(v1->size()!=v2->size()){
   //  std::cout<<"12"<<std::endl;
   //}

      h1->Fill(v1->size());
    for(size_t i = 0; i<v1->size(); i++){
      
      h2->Fill(v2->at(i));
    }
    for(size_t j = 0; j<v3->size(); j++){
      h3->Fill(v3->at(j));
    }
    
   // if(v4->size()!=v5->size()){
   //   std::cout<<"45"<<std::endl;
   // }
   // if(v4->size()!=v6->size()){
   //   std::cout<<"46"<<std::endl;
   // }
      h4->Fill(v4->size());
      
    for(size_t k = 0; k<v4->size(); k++){
      h5->Fill(v5->at(k));
    }
    for(size_t a = 0; a<v6->size(); a++){
      if(!std::isnan(v6->at(a))){
        h6->Fill(v6->at(a));
      }

    }
   
  }
  for(int n =0; n<3; n++){
    vs1.at(n)->SetLineColor(kRed);
    vs1.at(n)->SetLineWidth(2);
    vs2.at(n)->SetLineColor(kBlue);
    vs2.at(n)->SetLineWidth(2);
  }
  
  TCanvas* c1 = new TCanvas("c1","",1500,600);
  c1->Divide(2,2);
  c1->cd(1);
  h1->Draw();
  h4->Draw("same");
  c1->cd(2);
  h2->Draw();
  h5->Draw("same");
  c1->cd(3);
  h3->Draw();
  h6->Draw("same");





}
