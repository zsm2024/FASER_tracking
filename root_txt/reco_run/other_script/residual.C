//struct triplet{
//  double d1;
//  double d2;
//  double d3;
//};


void residual(){

  gStyle->SetOptStat(1110);

  TFile* f1 = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_3station_forward_seed_summary_circleFitTrackSeedTool.root","Read");
  TTreeReader* Reader1 = new TTreeReader("tree",f1);
  TTree* t1 = Reader1->GetTree();
  std::vector<double> *v1 = nullptr;
  std::vector<double> *v2 = nullptr;
  std::vector<double> *v3 = nullptr;
  std::vector<double> *v4 = nullptr;

  t1->SetBranchAddress("track_theta", &v1);
  t1->SetBranchAddress("t_theta", &v2);
  t1->SetBranchAddress("t_QoverP", &v3);
  t1->SetBranchAddress("track_QoverP", &v4);

  TH1D* h1 = new TH1D("h1","",50,-0.0035,0.0035);
  TH1D* h2 = new TH1D("h2","",100,0,0.08);
  TH1D* h3 = new TH1D("h3","",100,0,0.08);
  TH1D* h4 = new TH1D("h4","",50,-0.0035,0.0035);
  TH1D* h5 = new TH1D("h5","",50,-0.0035,0.0035);

  size_t entries = t1->GetEntries();
  //std::vector<triplet> info;
  TMultiGraph* multiGraph = new TMultiGraph();
  for(size_t ievent = 0; ievent<entries; ++ievent){
    t1->GetEntry(ievent);
    TGraph* graph = new TGraph();
    if (v1->size() == v2->size() && v1->size() > 0){
    for(size_t iTrack = 0; iTrack < v1->size(); ++iTrack){
      h1->Fill((v2->at(iTrack))-(v1->at(iTrack)));
      h2->Fill(v1->at(iTrack));
      h3->Fill(v2->at(iTrack));
      graph->SetPoint(iTrack,v2->at(iTrack),(v2->at(iTrack))-(v1->at(iTrack)));
      if(v4->at(iTrack) > 0){
        h4->Fill((v2->at(iTrack))-(v1->at(iTrack)));
      }
      else{
        h5->Fill((v2->at(iTrack))-(v1->at(iTrack)));
      }
    //  info.push_back({v1->at(iTrack),v2->at(iTrack),(v2->at(iTrack))-(v1->at(iTrack))});
    }
    graph->SetMarkerStyle(20);  // 圆形标记
    graph->SetMarkerColor(ievent + 1);  // 每个事件使用不同的颜色
    graph->SetMarkerSize(1.0);  //
    multiGraph->Add(graph);
    }
    else{
      std::cerr <<"event"<<ievent<< ": Warning: No valid data points in this event (v1 or v2 is empty)!" << std::endl;
      continue;
    }
  }

 // size_t n = info.size();
 // double x,y,z;
 // auto g1 = new TGraph2D();
 // for(int i = 0; i<n;++i){
 //   x = info.at(i).d1;
 //   y = info.at(i).d2;
 //   z = info.at(i).d3;
 //   g1->SetPoint(i,x,y,z);
 // }

   //g1->SetTitle("; track_theta [rad]; t_theta [rad]; residual");
  
  h1->GetXaxis()->SetTitle("residual.theta [rad]");
  h1->GetYaxis()->SetTitle("Events");
  h2->GetXaxis()->SetTitle("theta [rad]");
  h2->GetYaxis()->SetTitle("Events");
  h4->GetXaxis()->SetTitle("(+e)theta [rad]");
  h4->GetYaxis()->SetTitle("Events");
  h5->GetXaxis()->SetTitle("(-e)theta [rad]");
  h5->GetYaxis()->SetTitle("Events");

  TF1 *fit = new TF1("f1","gaus",-3,3);
  h1->Fit(fit,"R");
  double mean = fit->GetParameter(1);
  double meanError = fit->GetParError(1);
  double sigma = fit->GetParameter(2);
  double sigmaError = fit->GetParError(2);



  h2->SetLineColor(596);
  h2->SetLineWidth(3);
  h3->SetLineColor(633);
 // g1->SetMarkerColor(596);
 // g1->SetMarkerStyle(21);
 
  TCanvas* c1 = new TCanvas("c1","",500,600);
  c1->Divide(2,3);
  c1->cd(1);
  h1->Draw();
  TLegend *legend1 = new TLegend(0.7, 0.7, 0.9, 0.9); // 创建一个图例
  legend1->AddEntry(h1, "Data", "l");  // 为直方图添加图例项，"l"表示线型
  //legend1->AddEntry(fit, TString::Format("Gaussian Fit:\n#mu = %f #pm %f\n#sigma = %f #pm %f", mean, meanError, sigma, sigmaError), "l");
  legend1->AddEntry(fit, TString::Format("Gaussian Fit: #mu = %f #pm %f", mean, meanError), "l");
  legend1->AddEntry(fit, TString::Format("#sigma = %f #pm %f", sigma, sigmaError),"");
  legend1->SetTextSize(0.03);
  legend1->Draw();
  c1->cd(2);
  h2->Draw();
  h3->Draw("same");
  TLegend *legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend2->AddEntry(h2, "Reco", "l");
  legend2->AddEntry(h3, "Truth", "l");
  legend2->Draw();
  c1->cd(3);
  //gStyle->SetPalette(1);
  multiGraph->Draw("AP");
 // gStyle->SetPalette(1)
 // g1->SetMarkerStyle(20);
 // g1->Draw("pcol");
  c1->cd(4);
  h4->Draw();
  c1->cd(5);
  h5->Draw();
}
