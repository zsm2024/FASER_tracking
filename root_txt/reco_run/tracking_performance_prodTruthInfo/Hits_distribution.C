void Hits_distribution(){

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  //gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleSize(0.05, "xy");
  gStyle->SetLabelSize(0.04, "xy");
  gStyle->SetTitleOffset(1.2, "x");
  gStyle->SetTitleOffset(1.2, "y");
  gStyle->SetNdivisions(510, "y");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPalette(kBird);
  gStyle->SetNumberContours(100);
  gStyle->SetLineWidth(3);
  TFile* file1= new TFile("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root","read");
  TTreeReader* Reader1 = new TTreeReader("tree",file1);
  TTree* t1 = Reader1->GetTree();

  std::vector<int> *v1 = nullptr;
  std::vector<int> *v2 = nullptr;
  std::vector<int> *v3 = nullptr;
  std::vector<int> *v4 = nullptr;
  std::vector<int> *v5 = nullptr;
  std::vector<int> *v6 = nullptr;
  std::vector<int> *v7 = nullptr;
  std::vector<int> *v8 = nullptr;
  std::vector<int> *v9 = nullptr;
  std::vector<int> *v10 = nullptr;
  std::vector<int> *v11 = nullptr;
  std::vector<int> *v12 = nullptr;
  std::vector<uint64_t> *v13 = nullptr;
  std::vector<float> *v14 = nullptr;
  std::vector<float> *v15 = nullptr;
  std::vector<int> *v16 = nullptr;
  


  t1->SetBranchAddress("st0_l0",&v1);
  t1->SetBranchAddress("st0_l1",&v2);
  t1->SetBranchAddress("st0_l2",&v3);
  t1->SetBranchAddress("st1_l0",&v4);
  t1->SetBranchAddress("st1_l1",&v5);
  t1->SetBranchAddress("st1_l2",&v6);
  t1->SetBranchAddress("st2_l0",&v7);
  t1->SetBranchAddress("st2_l1",&v8);
  t1->SetBranchAddress("st2_l2",&v9);
  t1->SetBranchAddress("st3_l0",&v10);
  t1->SetBranchAddress("st3_l1",&v11);
  t1->SetBranchAddress("st3_l2",&v12);
  t1->SetBranchAddress("truth_barcode",&v13);
  t1->SetBranchAddress("truth_theta",&v14);
  t1->SetBranchAddress("truth_pdg",&v15);
  t1->SetBranchAddress("truthParticle_Hits",&v16);

  TH1D* h1 = new TH1D("h1",";0_0_nHits;Events",15,0,15);
  TH1D* h2 = new TH1D("h2",";0_1_nHits;Events",15,0,15);
  TH1D* h3 = new TH1D("h3",";0_2_nHits;Events",15,0,15);
  TH1D* h4 = new TH1D("h4",";1_0_nHits;Events",15,0,15);
  TH1D* h5 = new TH1D("h5",";1_1_nHits;Events",15,0,15);
  TH1D* h6 = new TH1D("h6",";1_2_nHits;Events",15,0,15);
  TH1D* h7 = new TH1D("h7",";2_0_nHits;Events",15,0,15);
  TH1D* h8 = new TH1D("h8",";2_1_nHits;Events",15,0,15);
  TH1D* h9 = new TH1D("h9",";2_2_nHits;Events",15,0,15);
  TH1D* h10 = new TH1D("h10",";3_0_nHits;Events",15,0,15);
  TH1D* h11 = new TH1D("h11",";3_1_nHits;Events",15,0,15);
  TH1D* h12 = new TH1D("h12",";3_2_nHits;Events",15,0,15);
 // TH1F* h13 = new TH1F("h13",";nonHits_theta;Events",10,0,0.05);
 // TH1I* h14 = new TH1I("h14",";nonHits_pdg;Events",20,-20,20);
 // TH2F* h15 = new TH2F("h15",";truth_theta;nHits",10,0,0.05,50,0,50);


  size_t entries = t1->GetEntries();
  int event = 0;
  for(size_t ievent = 0;ievent < entries; ++ievent){
    t1->GetEntry(ievent);
    for(size_t iParticle = 0; iParticle < (v13->size()); ++iParticle){
      
      h15->Fill(v14->at(iParticle),v16->at(iParticle));
     // if(v1->at(iParticle) == 0||v2->at(iParticle) == 0||v3->at(iParticle) == 0||v4->at(iParticle) == 0||v5->at(iParticle) == 0||v6->at(iParticle) == 0||v7->at(iParticle) == 0||v8->at(iParticle) == 0||v9->at(iParticle) == 0||v10->at(iParticle) == 0||v11->at(iParticle) == 0||v12->at(iParticle) == 0){
     //   std::cout<<"ievent: "<<ievent<<"; "<<"barcode: "<<v13->at(iParticle)<<std::endl;
     //   std::cout<<v1->at(iParticle)<<";"<<v2->at(iParticle)<<";"<<v3->at(iParticle)<<";"<<v4->at(iParticle)<<";"<<v5->at(iParticle)<<";"<<v6->at(iParticle)<<";"<<v7->at(iParticle)<<";"<<v8->at(iParticle)<<";"<<v9->at(iParticle)<<";"<<v10->at(iParticle)<<";"<<v11->at(iParticle)<<";"<<v12->at(iParticle)<<";"<<std::endl;
     // }

        if(v1->at(iParticle) == 0&&v2->at(iParticle) == 0&&v3->at(iParticle) == 0&&v4->at(iParticle) == 0&&v5->at(iParticle) == 0&&v6->at(iParticle) == 0&&v7->at(iParticle) == 0&&v8->at(iParticle) == 0&&v9->at(iParticle) == 0&&v10->at(iParticle) == 0&&v11->at(iParticle) == 0&&v12->at(iParticle) == 0){
      //std::cout<<"ievent: "<<ievent<<"; "<<"number: "<<v13->size()<<"; "<<"barcode: "<<v13->at(iParticle)<<std::endl;
      //  std::cout<<v1->at(iParticle)<<";"<<v2->at(iParticle)<<";"<<v3->at(iParticle)<<";"<<v4->at(iParticle)<<";"<<v5->at(iParticle)<<";"<<v6->at(iParticle)<<";"<<v7->at(iParticle)<<";"<<v8->at(iParticle)<<";"<<v9->at(iParticle)<<";"<<v10->at(iParticle)<<";"<<v11->at(iParticle)<<";"<<v12->at(iParticle)<<";"<<std::endl;
      event++;
      //std::cout<<v14->at(iParticle)<<std::endl;
      h13->Fill(v14->at(iParticle));
      h14->Fill(v15->at(iParticle));
       }
   //
  //   if(v16->at(iParticle)<18){
  //     event++;

  //     h13->Fill(v14->at(iParticle));
  //     h14->Fill(v15->at(iParticle));
  //   }
     
    // h1->Fill(v1->at(iParticle));
    // h2->Fill(v2->at(iParticle));
    // h3->Fill(v3->at(iParticle));
    // h4->Fill(v4->at(iParticle));
    // h5->Fill(v5->at(iParticle));
    // h6->Fill(v6->at(iParticle));
    // h7->Fill(v7->at(iParticle));
    // h8->Fill(v8->at(iParticle));
    // h9->Fill(v9->at(iParticle));
    // h10->Fill(v10->at(iParticle));
    // h11->Fill(v11->at(iParticle));
    // h12->Fill(v12->at(iParticle));
    }
    //std::cout<<"______________________________"<<std::endl;
  }
  //std::cout<<"-------------------------------------"<<std::endl;
  std::cout<<"event: "<<event<<std::endl;
  TCanvas* c1 = new TCanvas("c1","",500,600);
  c1->Divide(1,3);
  c1->cd(1);
  h13->SetLineWidth(3);
  h13->Draw();
  c1->cd(2);
  h14->SetLineWidth(3);
  h14->Draw();
  c1->cd(3);
  h15->SetLineWidth(3);
  h15->Draw("COLZ");
  //c1->Divide(3,4);
  //c1->cd(1);
  //h1->SetLineWidth(3);
  //h1->Draw();
  //c1->cd(2);
  //h2->SetLineWidth(3);
  //h2->Draw();
  //c1->cd(3);
  //h3->SetLineWidth(3);
  //h3->Draw();
  //c1->cd(4);
  //h4->SetLineWidth(3);
  //h4->Draw();
  //c1->cd(5);
  //h5->SetLineWidth(3);
  //h5->Draw();
  //c1->cd(6);
  //h6->SetLineWidth(3);
  //h6->Draw();
  //c1->cd(7);
  //h7->SetLineWidth(3);
  //h7->Draw();
  //c1->cd(8);
  //h8->SetLineWidth(3);
  //h8->Draw();
  //c1->cd(9);
  //h9->SetLineWidth(3);
  //h9->Draw();
  //c1->cd(10);
  //h10->SetLineWidth(3);
  //h10->Draw();
  //c1->cd(11);
  //h11->SetLineWidth(3);
  //h11->Draw();
  //c1->cd(12);
  //h12->SetLineWidth(3);
  //h12->Draw();

}
