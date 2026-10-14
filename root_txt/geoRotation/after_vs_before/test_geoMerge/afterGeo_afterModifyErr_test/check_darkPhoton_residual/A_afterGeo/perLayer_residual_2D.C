#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TLine.h>

#include <iostream>
#include <map>
#include <utility>
#include <cmath>
#include <string>


// ============================================================
// Structure containing track-level / event-level information
// ============================================================

struct TrackInfo {

    double trackSeparation = -999.;
    //double truthThetaX     = -999.;
    //double truthThetaY     = -999.;
    //double truthP          = -999.;
    //double decayZ          = -999.;
};


// ============================================================
// Main function
// ============================================================

void perLayer_residual_2D(
    const char* stateFileName   = "110051_mc24_afterGeo_3station_forward_track_states_ckf.root  ",
    const char* summaryFileName = "110051_mc24_afterGeo_3station_forward_track_summary_ckf.root    ",
    const char* seedFileName = "beforeGeo_mc24_newRecoScript_110051_3station_forward_seed_summary_circleFitTrackSeedTool.root",
    std::string variable        = "trackSeparation",
    std::string stateType       = "smt"
)
{

    gStyle->SetOptStat(0);
    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadTopMargin(0.08);

    // ========================================================
    // Open files
    // ========================================================

    TFile* stateFile = TFile::Open(stateFileName);

    if (!stateFile || stateFile->IsZombie()) {
        std::cerr << "Cannot open state file: "
                  << stateFileName << std::endl;
        return;
    }

    TFile* summaryFile = TFile::Open(summaryFileName);

    if (!summaryFile || summaryFile->IsZombie()) {
        std::cerr << "Cannot open summary file: "
                  << summaryFileName << std::endl;
        return;
    }

    TFile* seedFile = TFile::Open(seedFileName);

    if (!seedFile || seedFile->IsZombie()) {
        std::cerr << "Cannot open seed file: "
                  << seedFileName << std::endl;
        return;
    }

    // ========================================================
    // Get trees
    // ========================================================

    TTree* stateTree =
        dynamic_cast<TTree*>(stateFile->Get("tree"));

    TTree* summaryTree =
        dynamic_cast<TTree*>(summaryFile->Get("tree"));
    TTree* seedTree =
        dynamic_cast<TTree*>(seedFile->Get("tree"));

    if (!stateTree) {
        std::cerr << "Cannot find tree in state file."
                  << std::endl;
        return;
    }

    if (!summaryTree) {
        std::cerr << "Cannot find tree in summary file."
                  << std::endl;
        return;
    }
    if (!seedTree) {
        std::cerr << "Cannot find tree in seed file."
                  << std::endl;
        return;
    }

    std::cout << "State entries   = "
              << stateTree->GetEntries() << std::endl;

    std::cout << "Summary entries = "
              << summaryTree->GetEntries() << std::endl;
    
    std::cout << "Seed entries = "
              << seedTree->GetEntries() << std::endl;


    // ========================================================
    // 1. Read TrackSummary
    //
    // Build:
    //
    // (event_nr, track_nr)
    //        |
    //        ---> track information
    //
    // ========================================================

    uint32_t seed_event_nr;
    uint32_t seed_track_nr;

    double trackSeparation;
    //double truthThetaX;
    //double truthThetaY;
    //double truthP;
    //double decayZ;


    seedTree->SetBranchAddress(
        "event_number", &seed_event_nr);



    // --------------------------------------------------------
    // IMPORTANT:
    //
    // Modify these branch names according to your ntuple
    // --------------------------------------------------------
    std::vector<double> *v1 = nullptr, *v2 = nullptr, *v3 = nullptr, *v4 = nullptr;
        std::vector<uint32_t> *v5 = nullptr;

    seedTree->SetBranchAddress("truthd0_x", &v1);
    seedTree->SetBranchAddress("truthd0_y", &v2);
    seedTree->SetBranchAddress("truthd1_x", &v3);
    seedTree->SetBranchAddress("truthd1_y", &v4);




    // ========================================================
    // map:
    //
    // key = (event_nr, track_nr)
    //
    // value = TrackInfo
    // ========================================================

    std::map<uint32_t, TrackInfo> trackMap;


    Long64_t nSeed = seedTree->GetEntries();
    //std::cout<<"nSummary = "<<nSummary<<std::endl;

    for (Long64_t i = 0; i < nSeed; ++i) {

        seedTree->GetEntry(i);

        //TrackKey key = summary_event_nr;
            //std::make_pair(
            //    summary_event_nr,
            //    summary_track_nr
            //);

        TrackInfo info;
        if (v1->empty() || v3->empty()) continue;


        double x1 = v1->at(1), y1 = v2->at(1);
        double x2 = v3->at(1), y2 = v4->at(1);
        double trackSeparation = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

        info.trackSeparation = trackSeparation;
        //info.truthThetaX     = truthThetaX;
        //info.truthThetaY     = truthThetaY;
        //info.truthP          = truthP;
        //info.decayZ          = decayZ;

        trackMap[i+1] = info;
        //std::cout<<"store summary_event_nr = "<<summary_event_nr<<"; trackSeparation = "<<trackSeparation<<std::endl;
    }


    std::cout << "Track map size = "
              << trackMap.size() << std::endl;
    
    // ========================================================
    // 2. Read TrackSummary tree
    // ========================================================
    //


    uint32_t summary_event_nr;

    std::vector<uint32_t> *summary_track_nr = nullptr;

    summaryTree->SetBranchAddress(
        "event_nr", &summary_event_nr);
    summaryTree->SetBranchAddress(
        "track_nr", &summary_track_nr);

    //get <event_nr>
    std::vector<uint32_t> stateEntryMap;


    Long64_t nSummary = summaryTree->GetEntries();
    //std::cout<<"nSummary = "<<nSummary<<std::endl;

    for (Long64_t i = 0; i < nSummary; ++i) {

        summaryTree->GetEntry(i);


        for(unsigned track=0; track<summary_track_nr->size(); track++){

          stateEntryMap.push_back(i+1);
        }
    }
    std::cout << "stateEntry map size = "
              << stateEntryMap.size() << std::endl;


    // ========================================================
    // 2. Read TrackState tree
    // ========================================================

    uint32_t event_nr;
    uint32_t track_nr;

    std::vector<int> *volume_id = nullptr;
    std::vector<int> *layer_id = nullptr;

    std::vector<float> *eQOP_flt = nullptr;
    std::vector<float> *eQOP_prt = nullptr;
    std::vector<float> *eQOP_smt = nullptr;

    std::vector<float> *t_eQOP = nullptr;


    stateTree->SetBranchAddress(
        "event_nr", &event_nr);

    stateTree->SetBranchAddress(
        "track_nr", &track_nr);

    stateTree->SetBranchAddress(
        "volume_id", &volume_id);

    stateTree->SetBranchAddress(
        "layer_id", &layer_id);

    stateTree->SetBranchAddress(
        "eQOP_flt", &eQOP_flt);

    stateTree->SetBranchAddress(
        "eQOP_prt", &eQOP_prt);

    stateTree->SetBranchAddress(
        "eQOP_smt", &eQOP_smt);

    stateTree->SetBranchAddress(
        "t_eQOP", &t_eQOP);


    // ========================================================
    // 3. Create 9 TH2F
    //
    // [station][layer]
    // ========================================================

    TH2F* h2[3][3];


    // --------------------------------------------------------
    // X-axis range
    //
    // Change according to variable
    // --------------------------------------------------------

    double xmin = 0.;
    double xmax = 5;
    


    for (int st = 0; st < 3; ++st) {

        for (int layer = 0; layer < 3; ++layer) {
          h2[st][layer] =
            new TH2F(
                Form("h2_S%d_L%d", st + 1, layer),
                Form(
                    "S%d L%d;#Delta R [mm];(p^{reco}-p^{truth})/p^{truth}",
                    st + 1,
                    layer
                ),
                100,
                xmin,
                xmax,
                120,
                -1.2,
                1.2
            );





            //h2[st][layer]->GetYaxis()->SetTitle(
            //    "(p^{reco}-p^{truth})/p^{truth}"
            //);

            //h2[st][layer]->SetTitle(
            //    Form(
            //        "Station %d Layer %d",
            //        st + 1,
            //        layer
            //    )
            //);
        }
    }


    // ========================================================
    // 4. Loop over TrackState
    // ========================================================

    Long64_t nState = stateTree->GetEntries();

    Long64_t nMatched = 0;
    Long64_t nMissing = 0;


    for (Long64_t i = 0; i < nState; ++i) {

        stateTree->GetEntry(i);
        



        // ----------------------------------------------------
        // We only consider stations 1-3
        // ----------------------------------------------------
        for(unsigned state=0; state<volume_id->size(); state++){
          int station = -1;

          if (volume_id->at(state)   == 25)
              station = 0;

          else if (volume_id->at(state) == 27)
              station = 1;

          else if (volume_id->at(state) == 29)
              station = 2;

          else
              continue;  


          // ----------  ------------------------------------------
          // layer_id:
          //
          // your previ  ous code uses:
          //
          // layer = (j   + 1) * 2
          //
          // so:
          //
          // L0 -> 2
          // L1 -> 4
          // L2 -> 6
          // ----------  ------------------------------------------

          int layer = -  1;

          if (layer_id  ->at(state) == 2)
              layer = 0  ;

          else if (layer_id ->at(state) == 4)
              layer = 1  ;

          else if (layer_id->at(state) == 6)
              layer = 2  ;

          else
              continue;  


          // ----------  ------------------------------------------
          // Match Trac  kState to TrackSummary
          // ----------  ------------------------------------------

          //TrackKey key   = event_nr;
          uint32_t key = stateEntryMap.at(i);


          auto it = trackMap.find(key);

          if (it == trackMap.end()) {

              ++nMissing;

              continue;  
          }

          ++nMatched;


          const TrackInfo& info =
              it->second;


          // ----------  ------------------------------------------
          // Select X v  ariable
          // ----------  ------------------------------------------

          double x = -999.;


          //if (variable == "trackSeparation")
          x = info.trackSeparation;

        //else if (va  riable == "truthThetaX")
        //    x = inf  o.truthThetaX;

        //else if (va  riable == "truthThetaY")
        //    x = inf  o.truthThetaY;

        //else if (va  riable == "truthP")
        //    x = inf  o.truthP;

        //else if (va  riable == "decayZ")
        //    x = inf  o.decayZ;


        //if (!std::i  sfinite(x))
        //    continu  e;


          // ----------  ------------------------------------------
          // Calculate   residual
          // ----------  ------------------------------------------

          double eQOP =  -999.;


          if (stateType == "flt")
              eQOP = eQOP_flt->at(state);

          else if (stateType == "prt")
              eQOP = eQOP_prt->at(state);

          else if (stateType == "smt")
              eQOP = eQOP_smt->at(state);

          else {
              std::cerr
                  << "Unknown state type: "
                  << stateType
                  << std::endl;
              return;
          }


          if (!std::isfinite(eQOP) ||
              !std::isfinite(t_eQOP->at(state)))
              continue;


          if (eQOP == 0 ||
              t_eQOP->at(state) == 0)
              continue;


          // ----------------------------------------------------
          // Same definition as your existing code
          //
          // p = 1 / |q/p|
          //
          // t_eQOP is apparently in 1/MeV,
          // hence /1000 for GeV.
          // ----------------------------------------------------

          double pReco =
              1.0 / std::abs(eQOP);

          double pTruth =
              1.0 / std::abs(t_eQOP->at(state)) / 1000.;


          double residual =
              (pReco - pTruth) / pTruth;


          if (!std::isfinite(residual))
              continue;


          // ----------------------------------------------------
          // Optional:
          // only fill the physically interesting range
          // ----------------------------------------------------
          //std::cout<<"residual = "<<residual<<std::endl;

          //if (residual < 0.5 &&
          //    residual >  -0.5)
          //    continue;
          //std::cout<<"fill-------------------"<<std::endl;


          // ----------------------------------------------------
          // Fill
          // ----------------------------------------------------

          h2[station][layer]->Fill(
              x,
              residual
          );

        }

    }


    std::cout << std::endl;

    std::cout << "Matched states = "
              << nMatched
              << std::endl;

    std::cout << "Missing states = "
              << nMissing
              << std::endl;


    // ========================================================
    // 5. Draw 3 x 3
    // ========================================================

    TCanvas* c =
        new TCanvas(
            Form(
                "c_%s_%s",
                variable.c_str(),
                stateType.c_str()
            ),

            Form(
                "%s vs %s",
                stateType.c_str(),
                variable.c_str()
            ),

            1400,
            1000
        );


    c->Divide(3, 3);


    for (int st = 0; st < 3; ++st) {

        for (int layer = 0; layer < 3; ++layer) {

            int ipad =
                st * 3 + layer + 1;

            c->cd(ipad);

            gPad->SetRightMargin(0.13);

            h2[st][layer]->Draw("COLZ");

            TLatex latex;

            latex.SetNDC();
            latex.SetTextSize(0.055);

            latex.DrawLatex(
                0.16,
                0.87,
                Form(
                    "S%d L%d",
                    st + 1,
                    layer
                )
            );
        }
    }


    c->Update();
}
