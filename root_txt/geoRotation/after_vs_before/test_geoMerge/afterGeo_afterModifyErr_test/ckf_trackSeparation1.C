void ckf_trackSeparation1() {
    gStyle->SetOptFit(0011);
    gStyle->SetOptStat(0000);
    gStyle->SetPadLeftMargin(0.18);
    gStyle->SetPadRightMargin(0.05);
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

    
    const Int_t nbins = 10;
    double bins[nbins + 1] = {0, 5, 10, 20, 30, 40, 50, 60, 80, 100, 220}; 

    //deal with mergeTarcks
    auto processFile_deltaR = [&](const char* filename,
                              TEfficiency* eff, int color
                              ) {

      TFile* file = TFile::Open(filename, "read");

      TTreeReader* reader = new TTreeReader("nt", file);
      TTree* t = reader->GetTree();

      std::vector<double>* v1 = nullptr; // truthd0_x
      std::vector<double>* v2 = nullptr; // truthd0_y
      std::vector<double>* v3 = nullptr; // truthd1_x
      std::vector<double>* v4 = nullptr; // truthd1_y
      std::vector<unsigned long>* v5 = nullptr; // Track_hitSet

      t->SetBranchAddress("truthd0_x", &v1);
      t->SetBranchAddress("truthd0_y", &v2);
      t->SetBranchAddress("truthd1_x", &v3);
      t->SetBranchAddress("truthd1_y", &v4);
      t->SetBranchAddress("Track_hitSet", &v5);

      size_t entries = t->GetEntries();
      std::cout << "events in " << filename << ": " << entries << std::endl;

      for(size_t ievent = 0; ievent < entries; ievent++) {
          t->GetEntry(ievent);

          if(v1->size() == 0 || v3->size() == 0) continue;

          int tracks = v5->size();
          if(tracks < 1) continue;

          double x1 = v1->at(3);
          double y1 = v2->at(3);
          double x2 = v3->at(3);
          double y2 = v4->at(3);

          double deltaR = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
          //if(deltaR > 60) continue;

          bool passTrack = (tracks >= 2);

          eff->Fill(passTrack, deltaR);
      }

      eff->SetMarkerColor(color);
      eff->SetLineColor(color);

      file->Close();
    };

    TEfficiency* eff_way1 = new TEfficiency("eff_way1", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
    TEfficiency* eff_way2 = new TEfficiency("eff_way2", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
    TEfficiency* eff_way3 = new TEfficiency("eff_way3", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
    TEfficiency* eff_way4 = new TEfficiency("eff_way4", ";#DeltaR1 [mm];CKF Efficiency", nbins, bins);
    //
    processFile_deltaR("110033_ntOut_officialRec_3st_forward_new_PHYS.root", eff_way1, kBlue);
    processFile_deltaR("beforeGeo_mc24_110033_biggerErr_3st_forward_PHYS.root", eff_way2, kRed);
    processFile_deltaR("110033_smallerCov_seedParams_3stForward_new_PHYS.root", eff_way3, kGreen);
    processFile_deltaR("afterGeo_mc24_110033_oldErr_3st_forward_PHYS.root", eff_way4, kBlack);

//    TGraphAsymmErrors* g_beforeF =
//(TGraphAsymmErrors*)eff_way1->CreateGraph();
//
//TGraphAsymmErrors* g_afterF =
//(TGraphAsymmErrors*)eff_way2->CreateGraph();
//
//TGraphAsymmErrors* g_beforeB =
//(TGraphAsymmErrors*)eff_way3->CreateGraph();
//
//TGraphAsymmErrors* g_afterB =
//(TGraphAsymmErrors*)eff_way4->CreateGraph();
//
//auto makeRatioGraph =
//[](TGraphAsymmErrors* num,
//   TGraphAsymmErrors* den)
//{
//    int n = num->GetN();
//
//    auto gr = new TGraphErrors(n);
//
//    for(int i=0;i<n;i++){
//
//        double x1,y1;
//        double x2,y2;
//
//        num->GetPoint(i,x1,y1);
//        den->GetPoint(i,x2,y2);
//
//        if(y2<=0) continue;
//
//        double ratio = y1/y2;
//
//        double err1 =
//            0.5*(num->GetErrorYhigh(i)+
//                 num->GetErrorYlow(i));
//
//        double err2 =
//            0.5*(den->GetErrorYhigh(i)+
//                 den->GetErrorYlow(i));
//
//        double ratioErr = ratio *
//            sqrt(pow(err1/y1,2)+
//                 pow(err2/y2,2));
//
//        gr->SetPoint(i,x1,ratio);
//        gr->SetPointError(i,0,ratioErr);
//    }
//
//    return gr;
//};
//auto grRatioF =
//makeRatioGraph(g_afterF,g_beforeF);
//
//auto grRatioB =
//makeRatioGraph(g_afterB,g_beforeB);
//
TCanvas* c = new TCanvas("c","",700,700);

//TPad* p1 =
//new TPad("p1","",0,0.30,1,1);
//
//TPad* p2 =
//new TPad("p2","",0,0,1,0.30);
//
//p1->SetBottomMargin(0.02);
//p2->SetTopMargin(0.05);
//p2->SetBottomMargin(0.30);
//
//p1->Draw();
//p2->Draw();

c->cd();

eff_way1->Draw("AP");
eff_way2->Draw("P SAME");
eff_way3->Draw("P SAME");
eff_way4->Draw("P SAME");
    auto legend = new TLegend(0.65, 0.15, 0.85, 0.35);
    legend->SetBorderSize(1);
    legend->SetLineWidth(2);
    legend->SetFillStyle(0);
    legend->AddEntry(eff_way1, "beforeGeo_oldErr", "lp");
    legend->AddEntry(eff_way2, "beforeGeo_biggerErr", "lp");
    legend->AddEntry(eff_way3, "afterGeo_biggerErr", "lp");
    legend->AddEntry(eff_way4, "afterGeo_oldErr", "lp");

legend->Draw();
//p2->cd();
//
//TH1D* frame =
//new TH1D("frame","",
//         nbins,bins);
//
//frame->GetYaxis()
//     ->SetTitle("afterGeo/beforeGeo");
//
//frame->GetYaxis()
//     ->SetRangeUser(0.8,1.2);
//
//frame->GetXaxis()
//     ->SetTitle("#DeltaR1 [mm]");
//
//frame->Draw();
//grRatioF->SetMarkerColor(kRed);
//grRatioF->SetLineColor(kRed);
//grRatioF->SetLineWidth(2);
//
//grRatioB->SetMarkerColor(kBlack);
//grRatioB->SetLineColor(kBlack);
//grRatioB->SetLineWidth(2);
//    auto legend1 = new TLegend(0.65, 0.15, 0.85, 0.35);
//    legend1->SetBorderSize(1);
//    legend1->SetLineWidth(2);
//    legend1->SetFillStyle(0);
//    legend1->AddEntry(grRatioF, "forward", "lp");
//    legend1->AddEntry(grRatioB, "backward", "lp");
//
//legend1->Draw();
//
//grRatioF->Draw("P SAME");
//grRatioB->Draw("P SAME");
//
//TLine* line =
//new TLine(0,1,220,1);
//
//line->SetLineStyle(2);
//line->Draw();

    //TCanvas* c1 = new TCanvas("c1", "Efficiency Comparison", 600, 500);
    //eff_way1->SetMarkerStyle(20);
    //eff_way2->SetMarkerStyle(20);
    //

    //eff_way1->SetMarkerSize(0.8);
    //eff_way2->SetMarkerSize(0.8);

    //eff_way1->Draw("AP");
    //eff_way2->Draw("P SAME");
    //eff_way3->Draw("P SAME");
    //eff_way4->Draw("P SAME");
   

    //auto legend = new TLegend(0.65, 0.15, 0.85, 0.35);
    //legend->SetBorderSize(1);
    //legend->SetLineWidth(2);
    //legend->SetFillStyle(0);
    //legend->AddEntry(eff_way1, "beforeGeo_3st_forward", "lp");
    //legend->AddEntry(eff_way2, "afterGeo_3st_forward", "lp");
    //legend->AddEntry(eff_way3, "beforeGeo_3st_backward", "lp");
    //legend->AddEntry(eff_way4, "afterGeo_3st_backward", "lp");
    //legend->Draw();

    //c1->SaveAs("ckf_trackSeparation.png");
}

