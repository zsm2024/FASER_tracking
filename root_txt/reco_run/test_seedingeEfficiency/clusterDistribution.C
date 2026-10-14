#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

void clusterDistribution() {
    // 打开文件和获取TTree
    TFile* file = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_4station_forward_seed_summary_circleFitTrackSeedTool.root");
    TTree* tree = (TTree*)file->Get("tree");

    // 定义指针
    std::vector<int>* seedIndex = nullptr;
    std::vector<int>* clusterIndex = nullptr;
    std::vector<int>* clusterStation = nullptr;
    std::vector<int>* clusterLayer = nullptr;

    // 设置分支地址
    tree->SetBranchAddress("seedIndex", &seedIndex);
    tree->SetBranchAddress("clusterIndex", &clusterIndex);
    tree->SetBranchAddress("clusterStation", &clusterStation);
    tree->SetBranchAddress("clusterLayer", &clusterLayer);
    // 初始化 12 个 histogram，例如 st0_l0, st0_l1, ..., st3_l2
    TH1I* hists[12];
    for (int i = 0; i < 12; ++i) {
        hists[i] = new TH1I(Form("layer%d", i),";;" , 20, 0, 20);
    }
    Long64_t nentries = tree->GetEntries();
    for (Long64_t entry = 0; entry < nentries; ++entry) {
      tree->GetEntry(entry);  // <---- 关键：读取
    
      // 遍历每个 seed
      for (size_t s = 0; s < seedIndex->size(); ++s) {
          int seed_id = seedIndex->at(s);
      
          // 记录这个 seed 在 12 层上各自的 cluster 数
          std::vector<int> layer_counts(12, 0);
      
          // 遍历所有 clusters
          for (size_t i = 0; i < clusterIndex->size(); ++i) {
              if (clusterIndex->at(i) != seed_id) continue;
      
              int st = clusterStation->at(i);
              int ly = clusterLayer->at(i);
              int idx = st * 3 + ly;
      
              if (idx >= 0 && idx < 12){
                  layer_counts[idx]++;
              }
          }
      
          // 对每层填 histogram
          for (int l = 0; l < 12; ++l){
              hists[l]->Fill(layer_counts[l]);
          }
      }
    }
    
    TCanvas* c = new TCanvas("c", "Cluster counts per layer", 1600, 1200);
    c->Divide(3, 4); // 4列×3行，12张子图
    
    for (int i = 0; i < 12; ++i) {
        c->cd(i + 1); // 第 i 个 pad
    
        hists[i]->SetLineColor(kBlue+1);
        hists[i]->SetFillColorAlpha(kAzure+7, 0.4);
        hists[i]->SetStats(0);
        
        hists[i]->GetXaxis()->SetTitle(Form("Station %d Layer %d", i/3, i%3));
        hists[i]->GetYaxis()->SetTitle("Entries");
    
        hists[i]->Draw("HIST");
    }
    
}

