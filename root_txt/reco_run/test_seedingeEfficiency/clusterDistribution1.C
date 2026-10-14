#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void clusterDistribution1() {

  gStyle->SetLineWidth(2);
    // Open ROOT file and retrieve TTree
    TFile* file = TFile::Open("FaserMC-MC22_FS_Aee_100MeV_1Em5-110038-00000-00009-s0008_4station_forward_seed_summary_circleFitTrackSeedTool.root");
    TTree* tree = (TTree*)file->Get("tree");

    // Define pointers to branches
    std::vector<int>* seedIndex = nullptr;
    std::vector<int>* clusterIndex = nullptr;
    std::vector<int>* clusterStation = nullptr;
    std::vector<int>* clusterLayer = nullptr;

    tree->SetBranchAddress("seedIndex", &seedIndex);
    tree->SetBranchAddress("clusterIndex", &clusterIndex);
    tree->SetBranchAddress("clusterStation", &clusterStation);
    tree->SetBranchAddress("clusterLayer", &clusterLayer);

    const int nLayers = 12;
    std::vector<int> maxPerLayer(nLayers, 0);  // First pass to find max per layer

    Long64_t nentries = tree->GetEntries();

    // First pass: find max cluster count per layer (to auto-set histogram range)
    for (Long64_t entry = 0; entry < nentries; ++entry) {
        tree->GetEntry(entry);
        for (size_t s = 0; s < seedIndex->size(); ++s) {
            int seed_id = seedIndex->at(s);
            std::vector<int> layer_counts(nLayers, 0);

            for (size_t i = 0; i < clusterIndex->size(); ++i) {
                if (clusterIndex->at(i) != seed_id) continue;
                int st = clusterStation->at(i);
                int ly = clusterLayer->at(i);
                int idx = st * 3 + ly;
                if (idx >= 0 && idx < nLayers) layer_counts[idx]++;
            }

            for (int l = 0; l < nLayers; ++l) {
                maxPerLayer[l] = std::max(maxPerLayer[l], layer_counts[l]);
            }
        }
    }

    // Create histograms with dynamic bin range
    TH1I* hists[nLayers];
    for (int i = 0; i < nLayers; ++i) {
        int xmax = std::max(10, maxPerLayer[i] + 2);  // safety margin
        hists[i] = new TH1I(Form("layer%d", i), "", xmax, 0, xmax);
    }

    // Second pass: fill histograms
    for (Long64_t entry = 0; entry < nentries; ++entry) {
        tree->GetEntry(entry);
        for (size_t s = 0; s < seedIndex->size(); ++s) {
            int seed_id = seedIndex->at(s);
            std::vector<int> layer_counts(nLayers, 0);

            for (size_t i = 0; i < clusterIndex->size(); ++i) {
                if (clusterIndex->at(i) != seed_id) continue;
                int st = clusterStation->at(i);
                int ly = clusterLayer->at(i);
                int idx = st * 3 + ly;
                if (idx >= 0 && idx < nLayers) layer_counts[idx]++;
            }

            for (int l = 0; l < nLayers; ++l) {
                hists[l]->Fill(layer_counts[l]);
            }
        }
    }

    // Draw histograms
    TCanvas* c = new TCanvas("c", "Cluster counts per layer", 1600, 1200);
    c->Divide(3, 4);  // 3 columns × 4 rows

    for (int i = 0; i < nLayers; ++i) {
        c->cd(i + 1);
        hists[i]->SetLineColor(kBlue + 1);
        hists[i]->SetFillColorAlpha(kAzure + 7, 0.4);
        hists[i]->SetStats(0);
        hists[i]->GetXaxis()->SetTitle(Form("Station %d Layer %d", i / 3, i % 3));
        hists[i]->GetYaxis()->SetTitle("Entries");
       // hists[i]->SetTitle(Form("Station %d Layer %d", i / 3, i % 3));
        hists[i]->Draw("HIST");
    }

   // c->SaveAs("cluster_distribution_per_layer.pdf");
}

