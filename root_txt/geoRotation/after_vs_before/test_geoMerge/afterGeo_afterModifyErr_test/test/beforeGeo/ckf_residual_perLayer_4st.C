#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "CommonUtils.h"
#include "SetStyle.hpp"

void anaHisto(TH1F *inputHist, TF1 *fitFunc, int j, TH1F *meanHist,
              TH1F *widthHist, bool doFit = false, double scale = 1) {
  // evaluate mean and width via the Gauss fit
  assert(inputHist != nullptr);
  if (inputHist->GetEntries() > 0) {
    if (doFit) {
      TFitResultPtr r = inputHist->Fit(fitFunc->GetName(), "SQ0");
      if (r.Get()) {
        // fill the mean and width into 'j'th bin of the meanHist and
        // widthHist
        meanHist->SetBinContent(j, r->Parameter(1));
        meanHist->SetBinError(j, r->ParError(1));
        widthHist->SetBinContent(j, r->Parameter(2) * scale);
        widthHist->SetBinError(j, r->ParError(2));
        // std::cout<<"bin "<< j  << " hist "<< widthHist->GetName() <<
        // " range " << inputHist->GetBinLowEdge(1) <<", "
        // <<inputHist->GetBinLowEdge(1)
        // +  inputHist->GetBinWidth(1)*inputHist->GetNbinsX() << ",
        // width = "<< r->Parameter(2)*scale << ", scale = " <<
        // scale/100. << std::endl;
      } else {
        std::cout << "Fitting failed " << std::endl;
      }
    } else {
      meanHist->SetBinContent(j, inputHist->GetMean());
      meanHist->SetBinError(j, inputHist->GetMeanError());
      widthHist->SetBinContent(j, inputHist->GetRMS() * scale);
      widthHist->SetBinError(j, inputHist->GetRMSError());
    }
  }
}

void ckf_residual_perLayer_4st(int station = 1, int layer = 0, int drawType = 1,
             bool doFit = false, double minY = 0, double maxY = 1) {

  gStyle->SetOptFit(0011);
  // gStyle->SetOptStat(1110);
  gStyle->SetOptStat(0000);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.02);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleSize(0.05, "xy");
  gStyle->SetLabelSize(0.05, "xy");
  gStyle->SetTitleOffset(1.2, "x");
  gStyle->SetTitleOffset(1.2, "y");
  gStyle->SetNdivisions(510, "xy");
  gStyle->SetErrorX(0);
  gStyle->SetStatX(0.98);
  gStyle->SetStatY(0.9);

  //std::string path1 = "./centos7/";
  //std::string path2 = "./alma9/WithoutDuplicateFix/";
  //std::string path3 = "./alma9/WithDuplicateFix/";

  //std::vector<std::string> files = {
  //    path3 +
  //        "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //        "3station_forward_track_states_ckf.root",

  //    path3 +
  //        "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //        "3station_backward_track_states_ckf.root",

  //    path3 +
  //        "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //        "4station_forward_track_states_ckf.root",

  //    path3 +
  //        "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //        "4station_backward_track_states_ckf.root",

  //    // path1 +
  //    // "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //    //   "4station_forward_track_states_ckf.root",
  //    //
  //    // path2 +
  //    // "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //    //   "4station_forward_track_states_ckf.root",
  //    //
  //    // path3 +
  //    // "FaserMC-MC22_PG_mumi_fasernu_5mrad_flukaE-100043-00000-00009-s0010_"
  //    //   "4station_forward_track_states_ckf.root",
  //};


  std::vector<std::string>files = {
    " FaserMC-MC24_PG_mupl_fasernu_5mrad_flukaE-100044-00000-00099-s0013_4station_forward_track_states_ckf.root",

  };

  // Deterine the actsVolumeIds from the fileNames
  std::vector<std::array<int, 4>> actsVolumeIds;
  for(int i = 0; i < 4; i++){
    actsVolumeIds.push_back({2,25,27,29});
  }

  // Deterine the stations from the fileNames
  std::vector<int> numStations;
  for (const auto &fileName : files) {

    TFile* file(TFile::Open(fileName.c_str()));
    if(!file||file->IsZombie()){
      std::cerr<<"Error opening file: "<< fileName << endl;
      exit(-1);
    }

    //if (fileName.find("centos") != std::string::npos) {
    //  actsVolumeIds.push_back({2, 11, 13, 15});
    //} else if (fileName.find("alma") != std::string::npos) {
    //  actsVolumeIds.push_back({2, 25, 27, 29});
    //} else {
    //  throw std::runtime_error(
    //      "Sorry. Can't determine the version from the file name");
    //}

    if (fileName.find("3station") != std::string::npos) {
      numStations.push_back(3);
    } else if (fileName.find("4station") != std::string::npos) {
      numStations.push_back(4);
    } else {
      throw std::runtime_error(
          "Sorry. Can't determine the number of stations from the file name");
    }
  }

  std::vector<std::string> legs = {
      "beforeGeo, W/O IFT, forward",
      "beforeGeo, W/O IFT, backward",
      "afterGeo, W/O IFT, forward",
      "afterGeo, W/O IFT, backward",

      //"centos7 (W IFT, forward)",
      //"alma9-W/O fix (W IFT, forward)",
      //"alma9-W/ fix (W IFT, forward)",
  };

  //std::vector<int> colors = {kBlue, kRed, kBlue, kRed};
  //std::vector<int> markers = {20, 20, 24, 24};
  std::vector<int> colors = {kBlue, kRed, kGreen};
  std::vector<int> markers = {20, 22, 24};

  std::vector<std::pair<std::string, std::string>> formats = {
      {"x_{l}^{fit} - x_{l}^{hit} [mm]", "eLOC0_smt-l_x_hit"},
      //{"QOP^{fit} - QOP^{truth} [1/Gev]", "eQOP_smt-t_eQOP*1000"},
      //{"(P^{fit} - P^{truth}) [Gev] ", "1./abs(eQOP_smt)-1./abs(t_eQOP)/1000"},
      {"(P^{reco} - P^{truth})/P^{truth} ", "(1./abs(eQOP_smt)-1./abs(t_eQOP)/1000)/(1./abs(t_eQOP)/1000)"},
      {"(P^{reco} - P^{truth})/P^{truth} ", "(1./abs(eQOP_flt)-1./abs(t_eQOP)/1000)/(1./abs(t_eQOP)/1000)"},
      {"(P^{reco} - P^{truth})/P^{truth} ", "(1./abs(eQOP_prt)-1./abs(t_eQOP)/1000)/(1./abs(t_eQOP)/1000)"},
      {"x_{l}^{hit} - x_{l}^{truth} [mm]", "l_x_hit-t_eLOC0"},
  };

  //int num = files.size();
  int num = 3;
  //std::pair<std::string, std::string> format = formats[drawType];
  std::vector<std::map<int, std::map<int, TH1F *>>> hists(num);
  std::vector<std::map<int, std::map<int, TF1 *>>> fits(num);
  std::vector<TH1F *> meanHists;
  std::vector<TH1F *> widthHists;
  for (int i = 0; i < num; ++i) {
    meanHists.push_back(new TH1F(Form("mean%i", i), "", 12, 0, 12));
    widthHists.push_back(new TH1F(Form("width%i", i), "", 12, 0, 12));
  }

  auto getPlots = [&](int ifile, std::map<int, std::map<int, TH1F *>> &hists,
                      std::map<int, std::map<int, TF1 *>> &fits, TH1F *meanHist,
                      std::pair<std::string, std::string> format,
                      TH1F *widthHist, int ss = 1, int color = 4,
                      int marker = 20) {
    TFile *tfile = TFile::Open(files[ifile].c_str());
    TTree *tree = (TTree *)tfile->Get("tree");
    std::cout<<"events  = "<<tree->GetEntries()<<std::endl;
    meanHist->GetXaxis()->SetTitle("");
    widthHist->GetXaxis()->SetTitle("");
    meanHist->GetYaxis()->SetTitle("Mean [mm]");
    if (doFit) {
      widthHist->GetYaxis()->SetTitle(
          Form("#sigma of %s", format.first.c_str()));
    } else {
      widthHist->GetYaxis()->SetTitle(
          Form("Std.Dev of %s", format.first.c_str()));
    }
    // widthHist->GetYaxis()->SetRangeUser(0.015,0.04);
    widthHist->GetYaxis()->SetRangeUser(minY, maxY);
    setThisHistStyle(widthHist, color, marker);
    setThisHistStyle(meanHist, color, marker);

    int nStations = 4;
    int nLayersPerStation = 3;
    int bindex = 1;
    for (int i = 1; i < nStations; i++) {
      for (int j = 0; j < nLayersPerStation; j++) {
        //meanHist->GetXaxis()->SetBinLabel(i * nLayersPerStation + j + 1,
        //                                  Form("Layer%i", j));
        //widthHist->GetXaxis()->SetBinLabel(i * nLayersPerStation + j + 1,
        //                                   Form("Layer%i", j));
        meanHist->GetXaxis()->SetBinLabel(bindex,
                                          Form("Layer%i", j));
        widthHist->GetXaxis()->SetBinLabel(bindex,
                                           Form("Layer%i", j));
        bindex++;
      }
    }

    int ih = 0;
    int bindex1 = 1;
    std::cout<<"nStations = "<<nStations<<std::endl;
    for (int i = ss; i < nStations; i++) {
      int station = actsVolumeIds[ifile][i];
      std::cout<<"ifile"<<ifile<<"; "<<"i = "<<i<<"; "<<"station = "<<station<<std::endl;
      std::cout << "looking at station = " << station << std::endl;
      for (int j = 0; j < nLayersPerStation; j++) {
        int layer = (j + 1) * 2;

        TH1F *hist =
            new TH1F(Form("hist_%i_%i", i, j),
                     Form("Station %i, layer %i", i, j), 100, -1.2,1.2);
        hist->GetXaxis()->SetTitle(Form("%s", format.first.c_str()));
        hist->GetYaxis()->SetTitle("Entries");
        hist->SetLineWidth(2);

        tree->Draw(Form("%s>>hist_%i_%i", format.second.c_str(), i, j),
                   Form("volume_id==%i&&layer_id==%i&&chi2Sum/"
                        "NDF<20&&rMax_smt<90&&NDF>9",
                        station, layer));
        

        //tree->Draw(Form("%s>>hist_%i_%i", format.second.c_str(), i, j),
        //           Form("volume_id==%i&&layer_id==%i"
        //                "&&(1./abs(t_eQOP)/1000)<500",
        //                station, layer));

        TF1 *fit = new TF1(
            Form("fit_%i_%i", i, j), "gaus(0)", hist->GetBinLowEdge(1),
            hist->GetBinLowEdge(1) + hist->GetBinWidth(1) * hist->GetNbinsX());
       // anaHisto(hist, fit, i * nLayersPerStation + j + 1, meanHist, widthHist,
       //          doFit);
        anaHisto(hist, fit, bindex1, meanHist, widthHist,
                 doFit);

        hists[ih][j] = hist;
        fits[ih][j] = fit;
        bindex1++;
      }
      ih++;
    }
  }; // end of lambda  function

  for (int ii = 0; ii < num; ++ii) {

    //int startS = (numStations[ii] == 3) ? 1 : 0;
    //getPlots(ii, hists[ii], fits[ii], meanHists[ii], widthHists[ii], startS,
    //         colors[ii], markers[ii]);
    int startS = 0;
    getPlots(0, hists[ii], fits[ii], meanHists[ii],formats[ii+1], widthHists[ii], startS,
             colors[ii], markers[ii]);
  }

  // ==========================================
  // Compare beforeGeo and afterGeo residuals
  // ==========================================
  
  TCanvas* cCompF =
      new TCanvas("cCompF",
                  "comparison(beforeGeo μ+ sample:110044",
                  1200,1200);
  
  cCompF->Divide(3,4);
  
  TLegend* legF = new TLegend(0.60,0.75,0.88,0.88);
  legF->SetBorderSize(0);
  legF->SetFillStyle(0);
  
  for(int ilay=0; ilay<12; ilay++){
  
      int ist = ilay/3;
      int il  = ilay%3;
  
      cCompF->cd(ilay+1);
  
      auto h1 = hists[0][ist][il];
      auto h2  = hists[1][ist][il];
      auto h3  = hists[2][ist][il];
  
      h1->SetLineColor(kBlue);
      h2 ->SetLineColor(kRed);
      h3 ->SetLineColor(kGreen);
  
      h1->SetLineWidth(2);
      h2 ->SetLineWidth(2);
      h3 ->SetLineWidth(2);
  
      double ymax =
          std::max(h1->GetMaximum(),
                   std::max(h2->GetMaximum(),
                   h3 ->GetMaximum())
                   );
  
      h1->SetMaximum(1.2*ymax);
  
      h1->Draw("hist");
      h2 ->Draw("hist same");
      h3 ->Draw("hist same");
  
      if(ilay==0){
          legF->AddEntry(h1,"beforeGeo_110044(μ+)_smt","l");
          legF->AddEntry(h2 ,"beforeGeo_110044(μ+)_flt","l");
          legF->AddEntry(h3 ,"beforeGeo_110044(μ+)_prt","l");
      }
      legF->SetLineWidth(2);
      legF->SetTextSize(0.05); 
  
      legF->Draw();
  
      TLatex latex;
      latex.SetNDC();
      latex.SetTextSize(0.05);
      latex.DrawLatex(
          0.15,0.85,
          Form("S%d L%d",ist+1,il));
  }

    TCanvas* cCompB =
      new TCanvas("cCompB",
                  "Backward comparison",
                  1200,1200);
  
  cCompB->Divide(3,4);
  
  TLegend* legB = new TLegend(0.60,0.75,0.88,0.88);
  legB->SetBorderSize(0);
  legB->SetFillStyle(0);
  
  for(int ilay=0; ilay<12; ilay++){
  
      int ist = ilay/3;
      int il  = ilay%3;
  
      cCompB->cd(ilay+1);
  
      auto hBefore = hists[1][ist][il];
      auto hAfter  = hists[3][ist][il];
  
      hBefore->SetLineColor(kRed);
      hAfter ->SetLineColor(kBlue);
  
      double ymax =
          std::max(hBefore->GetMaximum(),
                   hAfter ->GetMaximum());
  
      hBefore->SetMaximum(1.2*ymax);
  
      hBefore->Draw("hist");
      hAfter ->Draw("hist same");
  
      if(ilay==0){
          legB->AddEntry(hBefore,"beforeGeo_backward","l");
          legB->AddEntry(hAfter ,"afterGeo_backward","l");
      }
  
      legB->Draw();
  
      TLatex latex;
      latex.SetNDC();
      latex.SetTextSize(0.05);
      latex.DrawLatex(
          0.15,0.85,
          Form("S%d L%d",ist+1,il));
  }

  std::array<double, 4> statBoxPositions = {0.2, 0.4, 0.9, 0.95};

  std::vector<TCanvas *> cs;
  for (int ii = 0; ii < num; ++ii) {
    cs.push_back(new TCanvas(Form("cc%i", ii), legs[ii].c_str(), 1200, 800));
    int nStations = numStations[ii];
    int nLayersPerStation = 3;
    cs[ii]->Divide(nLayersPerStation, nStations);
    int ih = 0;
    for (int i = 0; i < nStations; i++) {
      for (int j = 0; j < nLayersPerStation; j++) {
        cs[ii]->cd(ih + 1);
        hists[ii][i][j]->Draw();
        fits[ii][i][j]->Draw("same");

        // Not sure why this does not work
        //  TPaveStats *ps = (TPaveStats *)gPad->GetPrimitive("stats");
        //  if (ps != nullptr) {
        //    std::cout<<"Resize the stat box"  << std::endl;
        //    ps->SetName(Form("stats_%s", hists[ii][i][j]->GetName()));
        //    ps->SetX1NDC(statBoxPositions[0]);
        //    ps->SetX2NDC(statBoxPositions[1]);
        //    ps->SetY1NDC(statBoxPositions[2]);
        //    ps->SetY2NDC(statBoxPositions[3]);
        //    gPad->Update();
        //  } else {
        //    std::cout<<"Didn't find the stat box"  << std::endl;
        //  }

        ih++;
      }
    }
    cs[ii]->Update();
  }

  TLegend *legend = new TLegend(0.5, 0.7, 0.9, 0.9);
  legend->SetLineStyle(0);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  TCanvas *c1 = new TCanvas("c1", "", 700, 500);
  c1->cd();
  gPad->SetLeftMargin(0.18);
  gPad->SetRightMargin(0.1);
  for (int ii = 0; ii < num; ++ii) {
    widthHists[ii]->Draw("same");
    legend->AddEntry(widthHists[ii], legs[ii].c_str(), "AP");
  }
  TLine *line0 = new TLine(3, minY, 3, minY + (maxY - minY) * 0.7);
  TLine *line1 = new TLine(6, minY, 6, minY + (maxY - minY) * 0.7);
  //TLine *line2 = new TLine(9, minY, 9, minY + (maxY - minY) * 0.7);
  line0->SetLineStyle(kDashed);
  line1->SetLineStyle(kDashed);
  //line2->SetLineStyle(kDashed);
  line0->SetLineWidth(2);
  line1->SetLineWidth(2);
  //line2->SetLineWidth(2);
  line0->Draw();
  line1->Draw();
  //line2->Draw();
  legend->Draw();
  myText(0.25, 0.65, 1, 0.04, "Station 1");
  myText(0.50, 0.65, 1, 0.04, "Station 2");
  myText(0.75, 0.65, 1, 0.04, "Station 3");
//  myText(0.75, 0.65, 1, 0.04, "Station 3");
}
