#include <cmath>

void ckf_trackSeparation_mergedTracks(){

  //----------------------------------------
  // style
  //----------------------------------------

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);

  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);

  gStyle->SetTitleSize(0.04, "xy");
  gStyle->SetLabelSize(0.04, "xy");

  //----------------------------------------
  // binning
  //----------------------------------------

  const Int_t nbins = 7;
  double bins[nbins + 1] = {0, 10, 20, 30, 40, 60, 100, 200};

  //----------------------------------------
  // efficiencies
  //----------------------------------------

  TEfficiency* eff1 = new TEfficiency(
      "eff1",
      ";#DeltaR [mm];CKF Efficiency",
      nbins,
      bins
  );

  TEfficiency* eff2 = new TEfficiency(
      "eff2",
      ";#DeltaR [mm];CKF Efficiency",
      nbins,
      bins
  );

  //----------------------------------------
  // lambda function
  //----------------------------------------

  auto processFile = [&](const char* filename,
                         TEfficiency* eff){

    TFile* file = TFile::Open(filename, "read");

    TTree* t = (TTree*)file->Get("nt");

    std::vector<double> *v1 = nullptr;
    std::vector<double> *v2 = nullptr;
    std::vector<double> *v3 = nullptr;
    std::vector<double> *v4 = nullptr;

    std::vector<unsigned long> *v5 = nullptr;

    t->SetBranchAddress("truthd0_x",&v1);
    t->SetBranchAddress("truthd0_y",&v2);
    t->SetBranchAddress("truthd1_x",&v3);
    t->SetBranchAddress("truthd1_y",&v4);

    t->SetBranchAddress("Track_hitSet",&v5);

    size_t entries = t->GetEntries();

    std::cout<<"Processing "
             <<filename
             <<" entries = "
             <<entries
             <<std::endl;

    for(size_t ievent = 0; ievent < entries; ievent++){

      t->GetEntry(ievent);

      //----------------------------------------
      // protection
      //----------------------------------------

      if(v1->size() < 4) continue;
      if(v3->size() < 4) continue;

      //----------------------------------------
      // deltaR
      //----------------------------------------

      double x1 = v1->at(3);
      double y1 = v2->at(3);

      double x2 = v3->at(3);
      double y2 = v4->at(3);

      double deltaR = sqrt(
          (x1-x2)*(x1-x2)
        + (y1-y2)*(y1-y2)
      );

      //----------------------------------------
      // track selection
      //----------------------------------------

      int tracks = v5->size();

      bool passTrack = false;

      if(tracks >= 2){
        passTrack = true;
      }

      eff->Fill(passTrack, deltaR);
    }

    file->Close();
  };

  //----------------------------------------
  // process files
  //----------------------------------------

  processFile(
      "official.root",
      eff1
  );

  processFile(
      "newGeometry.root",
      eff2
  );

  //----------------------------------------
  // style
  //----------------------------------------

  eff1->SetLineColor(kRed+1);
  eff1->SetLineWidth(2);
  eff1->SetMarkerColor(kRed+1);
  eff1->SetMarkerStyle(20);

  eff2->SetLineColor(kBlue+1);
  eff2->SetLineWidth(2);
  eff2->SetMarkerColor(kBlue+1);
  eff2->SetMarkerStyle(21);

  //----------------------------------------
  // draw
  //----------------------------------------

  TCanvas* c1 = new TCanvas(
      "c1",
      "comparison",
      700,
      600
  );

  eff1->Draw("AP");
  eff2->Draw("P SAME");

  //----------------------------------------
  // legend
  //----------------------------------------

  TLegend* leg = new TLegend(
      0.55,
      0.20,
      0.85,
      0.35
  );

  leg->AddEntry(eff1, "Official", "lep");
  leg->AddEntry(eff2, "GeoRotation", "lep");

  leg->SetBorderSize(0);

  leg->Draw();
}
