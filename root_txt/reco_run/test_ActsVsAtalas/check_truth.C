void check_truth() {


  TFile* f1 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00190-00199-s0008-PHYS.root","READ");
  TFile* f2 = TFile::Open("FaserMC-MC22_FS_Aee_010MeV_1Em5-110033-00190-00199-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root","READ");
  TTreeReader* Reader1 = new TTreeReader("nt",f1);
  TTreeReader* Reader2 = new TTreeReader("tree",f2);
  TTree* t1 = Reader1->GetTree();
  TTree* t2 = Reader2->GetTree();
  
  std::vector<int> *v1 = nullptr;
  std::vector<uint64_t> *v2 = nullptr;
  std::vector<bool> *v3 = nullptr;
  std::vector<double> *v4 = nullptr;
  std::vector<float> *v5 = nullptr;
  t1->SetBranchAddress("truth_barcode",&v1);
  t1->SetBranchAddress("truth_isFiducial",&v3);
  t1->SetBranchAddress("truth_px",&v4);
  t2->SetBranchAddress("truth_barcode",&v2);
  t2->SetBranchAddress("truth_px",&v5);


  TH1I* h1 = new TH1I("h1",";Ntuple_barcode;event",10,0,10);
  TH1I* h2 = new TH1I("h2",";seeding_barcode;event",10,0,10);

  TH1D* h3 = new TH1D("h3",";Ntuple_truthPx;event",50,-300,300);
  TH1D* h4 = new TH1D("h4",";seeding_truthPx;event",50,-300,300);




  size_t entries = t1->GetEntries();
  std::cout<<"entries: "<<entries<<std::endl;
  for(size_t ievent = 0; ievent<50; ievent++){
    t1->GetEntry(ievent);
    t2->GetEntry(ievent);
    std::cout<<"In Ntuple: ";
    for(size_t i = 0; i<v1->size();i++){
      if(v3->at(i) == false)continue;
      h1->Fill(v1->at(i));
      h3->Fill(v4->at(i));
      std::cout<<v1->at(i)<<", ";
    }
    std::cout<<std::endl;
    std::cout<<"In Seed: ";
    for(size_t j = 0; j<v2->size();j++){
      h2->Fill(v2->at(j));
      h4->Fill(v5->at(j));
      std::cout<<v2->at(j)<<", ";
    }
    std::cout<<std::endl;
    std::cout<<"---------------------------------"<<std::endl;
  }
  TCanvas* c1 = new TCanvas("c1","",600,500);
  c1->Divide(2,2);
  c1->cd(1);
  h1->Draw();
  c1->cd(2);
  h2->Draw();
  c1->cd(3);
  h3->Draw();
  c1->cd(4);
  h4->Draw();
  
}
