#include <cmath>

void ckf_trackSeparation1_mergedTracks(){

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
    gStyle->SetLineWidth(2);
    gStyle->SetLabelSize(0.04, "xy");
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.1, "y");
    gStyle->SetNdivisions(510, "y");
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
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
      ";#DeltaR1 [mm];CKF Efficiency",
      nbins,
      bins
  );

  TEfficiency* eff2 = new TEfficiency(
      "eff2",
      ";#DeltaR1 [mm];CKF Efficiency",
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
      "110033_officialNt_PHYS.root ",
      eff1
  );

  processFile(
      "afterModifyRefit_modifySmallErr_PHYS.root ",
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

  auto makeRatioGraph =
[](TEfficiency* num,
   TEfficiency* den)
{
    auto gNum =
        (TGraphAsymmErrors*)
        num->CreateGraph();

    auto gDen =
        (TGraphAsymmErrors*)
        den->CreateGraph();

    int n = gNum->GetN();

    auto gr =
        new TGraphErrors();

    for(int i=0;i<n;i++){

        double xNum,yNum;
        double xDen,yDen;

        gNum->GetPoint(i,xNum,yNum);
        gDen->GetPoint(i,xDen,yDen);

        if(yDen<=0) continue;

        double ratio = yNum/yDen;

        double errNum =
            0.5*(gNum->GetErrorYhigh(i)
               + gNum->GetErrorYlow(i));

        double errDen =
            0.5*(gDen->GetErrorYhigh(i)
               + gDen->GetErrorYlow(i));

        double errRatio = 0;

        if(yNum>0){
            errRatio =
                ratio*
                sqrt(
                    pow(errNum/yNum,2)
                  + pow(errDen/yDen,2)
                );
        }

        gr->SetPoint(
            gr->GetN(),
            xNum,
            ratio
        );

        gr->SetPointError(
            gr->GetN()-1,
            0,
            errRatio
        );
    }

    return gr;
};
auto grRatio =
    makeRatioGraph(
        eff2,
        eff1
        );
TCanvas* c1 =
new TCanvas(
    "c1",
    "comparison",
    700,
    600
);   

TPad* pad1 =
new TPad(
    "pad1",
    "",
    0,
    0.30,
    1,
    1
);

TPad* pad2 =
new TPad(
    "pad2",
    "",
    0,
    0,
    1,
    0.30
);

pad1->SetBottomMargin(0.02);

pad2->SetTopMargin(0.05);
pad2->SetBottomMargin(0.30);

pad1->Draw();
pad2->Draw();
pad1->cd();

eff1->Draw("AP");
eff2->Draw("P SAME");
  TLegend* leg = new TLegend(
      0.55,
      0.20,
      0.85,
      0.35
  );

  leg->AddEntry(eff1, "beforeGeo_mergedTracks", "lep");
  leg->AddEntry(eff2, "afterGeo_mergedTracks", "lep");

  leg->SetBorderSize(0);


leg->Draw();
pad2->cd();

TH1D* frame =
new TH1D(
    "frame",
    "",
    nbins,
    bins
);

frame->GetYaxis()
     ->SetTitle("afterGeo / beforeGeo");

frame->GetXaxis()
     ->SetTitle("#DeltaR1 [mm]");

frame->GetYaxis()
     ->SetRangeUser(
         0.8,
         1.2
     );

frame->GetYaxis()
     ->SetTitleSize(0.08);

frame->GetYaxis()
     ->SetLabelSize(0.08);

frame->GetXaxis()
     ->SetTitleSize(0.10);

frame->GetXaxis()
     ->SetLabelSize(0.08);

frame->GetYaxis()
     ->SetNdivisions(505);

frame->Draw();
grRatio->SetMarkerStyle(20);
grRatio->SetMarkerColor(kBlack);
grRatio->SetLineColor(kBlack);
grRatio->SetLineWidth(2);

grRatio->Draw("P SAME");

TLine* line =
new TLine(
    bins[0],
    1.0,
    bins[nbins],
    1.0
);

line->SetLineStyle(2);

line->Draw();


 //TCanvas* c1 = new TCanvas(
 //     "c1",
 //     "comparison",
 //     700,
 //     600
 // );

 // eff1->Draw("AP");
 // eff2->Draw("P SAME");

 // //----------------------------------------
 // // legend
 // //----------------------------------------

}
