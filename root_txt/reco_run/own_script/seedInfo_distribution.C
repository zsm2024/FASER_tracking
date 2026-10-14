void seedInfo_distribution(){
  TFile* file1= new TFile("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00190-00199-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root","read");
  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();
  uint32_t nSeeds = 0;
  std::vector<int> *v2 = nullptr;
  std::vector<int> *v3 = nullptr;
  std::vector<float> *v4 = nullptr;
  std::vector<double> *v5 = nullptr;


  t1->SetBranchAddress("nSeeds",&nSeeds);
  t1->SetBranchAddress("nClusters",&v2);
  t1->SetBranchAddress("nSegments",&v3);
  t1->SetBranchAddress("t_theta",&v4);
  t1->SetBranchAddress("Positionx",&v5);

  TH1I* h1 = new TH1I("h1","TH_nSeeds",10,0,10);
  TH1I* h2 = new TH1I("h2","TH_nClusters",30,0,30);
  TH1I* h3 = new TH1I("h3","TH_nSegments",4,0,4);
  TH1F* h4 = new TH1F("h4","TH_cos(theta)",10,0.99999990,1);
  

  h1->GetXaxis()->SetTitle("nSeeds");
  h1->GetYaxis()->SetTitle("Events");
  h2->GetXaxis()->SetTitle("nClusters");
  h2->GetYaxis()->SetTitle("Events");
  h3->GetXaxis()->SetTitle("nSegments");
  h3->GetYaxis()->SetTitle("Events");
  h3->GetXaxis()->SetTitle("cos(theta)");
  h3->GetYaxis()->SetTitle("Events");
  
  size_t entries = t1->GetEntries();
  for(size_t ievent = 0;ievent < entries; ++ievent){
    t1->GetEntry(ievent);
   // std::cout<<"nSeeds.size: "<<nSeeds<<std::endl;
    h1->Fill(nSeeds);
    std::cout<<"seeds: "<<nSeeds<<std::endl;
    for(size_t iTrack = 0; iTrack < (v2->size()); ++iTrack){
     // std::cout<<"nSegments.size: "<<v3->at(iTrack)<<std::endl;
      std::cout<<"seed "<<iTrack<<"has position: "<<v5->size()<<std::endl;
      h2->Fill(v2->at(iTrack));
      h3->Fill(v3->at(iTrack));
    }
    std::cout<<"————————————————————————————————————————————"<<std::endl;
    for(size_t i = 0; i<(v4->size()); ++i){
      h4->Fill(cos(v4->at(i)));
    }
  }
  TCanvas* c1 = new TCanvas("c1","",500,600);
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
