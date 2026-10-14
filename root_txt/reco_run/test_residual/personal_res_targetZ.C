#include <cmath>

void personal_res_targetZ() {
  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(1111);
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

  const std::vector<std::string> filenames = {
    //"110038-3st-backward-seed-summary.root",
    "/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/beforeGeo_mc24_newRecoScript_110051_3station_forward_seed_summary_circleFitTrackSeedTool.root",
    "/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/A_afterGeo/afterGeo_mc24_110051_3station_forward_seed_summary_circleFitTrackSeedTool.root  "
  
   // "/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/muonSample/FaserMC-MC24_PG_mupl_fasernu_5mrad_flukaE-100044-00000-00099-s0013_3station_forward_seed_summary_circleFitTrackSeedTool.root",
   // "/home/pub/FASER_tracking/root_txt/geoRotation/after_vs_before/test_geoMerge/afterGeo_afterModifyErr_test/check_darkPhoton_residual/muonSample/afterGeo/FaserMC-MC24_PG_mupl_fasernu_5mrad_flukaE-100044-00000-00099-s0013_3station_forward_seed_summary_circleFitTrackSeedTool.root"
  };

  const std::vector<int> colors = {600, 633, 418}; 
  const std::vector<std::string> labels = {
    //"m_{A'} = 100 Mev; #varepsilon = 10^{-5}",
    "m_{A'} = 10 Mev; #varepsilon = 10^{-4}; 2<seedNum<=5",
    "m_{A'} = 10 Mev; #varepsilon = 10^{-4} afterGeo",
   // "muon^{+}_beforeGeo",
   // "muon^{+}_afterGeo"
  };

  std::vector<TH1D*> hists;

  for (size_t i = 0; i < filenames.size(); ++i) {
    TFile* file = TFile::Open(filenames[i].c_str(), "READ");

    TTree* tree = (TTree*)file->Get("tree");
    std::vector<double>* v1 = nullptr;
    std::vector<double>* v2 = nullptr;
    tree->SetBranchAddress("track_QoverP", &v1);
    tree->SetBranchAddress("t_QoverP", &v2);

    TString histName = Form("h%d", (int)i);
    TH1D* h = new TH1D(histName, ";(P^{reco} - P^{truth})/P^{truth};Events", 50, -2, 2);
    h->SetLineColor(colors[i]);
    h->SetLineWidth(2);

    size_t entries = tree->GetEntries();
    for (size_t ievent = 0; ievent < entries; ++ievent) {
      tree->GetEntry(ievent);
      if (v1->size() <=5 && v1->size() == v2->size() && v1->size() > 2) {
        for (size_t j = 0; j < v1->size(); ++j) {
          if(std::isnan(v2->at(j))||std::isnan(v1->at(j)))continue;
          //h->Fill(1./abs(v2->at(j)) - 1./abs(v1->at(j)));
          h->Fill((1./abs(v1->at(j)) - 1./abs(v2->at(j)))/(1./abs(v2->at(j))));
        }
      }
    }

    hists.push_back(h);
  }
  TCanvas* c = new TCanvas("c", "", 700, 550);

  for (size_t i = 0; i < hists.size(); ++i) {
    hists[i]->Draw(i == 0 ? "" : "same"); 
  
  }
  for (size_t i = 0; i < hists.size(); ++i) {
    double mean = hists[i]->GetMean();
    double rms = hists[i]->GetRMS();
  
    TPaveText* pt = new TPaveText(0.60, 0.80 - i*0.14, 0.85, 0.90 - i*0.14, "NDC");
  //  pt->AddText(labels[i].c_str());
    pt->AddText(Form("Mean = %.6f", mean));
    pt->AddText(Form("Std Dev = %.6f", rms));
    pt->SetTextColor(colors[i]);
    pt->SetBorderSize(1);
    pt->SetFillStyle(0);
    pt->SetTextFont(42);
    pt->SetTextSize(0.03);
    pt->Draw("same");
  }
  TLegend* legend = new TLegend(0.20, 0.70, 0.52, 0.88);
  for (size_t i = 0; i < hists.size(); ++i) {
    legend->AddEntry(hists[i], labels[i].c_str(), "l");
  }
  legend->SetTextFont(42);
  legend->SetTextSize(0.028);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  legend->Draw();



}

 
