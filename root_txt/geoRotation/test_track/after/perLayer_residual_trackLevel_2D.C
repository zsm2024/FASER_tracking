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
    float truthTheta     = -999.;
    float truthP     = -999.;
    float truthPt          = -999.;
    //double decayZ          = -999.;
};


// ============================================================
// Main function
// ============================================================

void perLayer_residual_trackLevel_2D(
    const char* stateFileName   = "FaserMC-MC24_FS_Aee_010MeV_1Em5-110033-00000-00001-master_dev_3station_forward_track_states_ckf.root",
    const char* summaryFileName = "FaserMC-MC24_FS_Aee_010MeV_1Em5-110033-00000-00001-master_dev_3station_forward_track_summary_ckf.root",
    std::string variable        = "truthTheta",
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


    // ========================================================
    // Get trees
    // ========================================================

    TTree* stateTree =
        dynamic_cast<TTree*>(stateFile->Get("tree"));

    TTree* summaryTree =
        dynamic_cast<TTree*>(summaryFile->Get("tree"));

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

    std::cout << "State entries   = "
              << stateTree->GetEntries() << std::endl;

    std::cout << "Summary entries = "
              << summaryTree->GetEntries() << std::endl;


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

    uint32_t summary_event_nr;
    std::vector<uint32_t> *summary_track_nr = nullptr;
    std::vector<float> *truthTheta = nullptr;
    std::vector<float> *truthP = nullptr;
    std::vector<float> *truthPt = nullptr;



    summaryTree->SetBranchAddress(
        "event_nr", &summary_event_nr);



    // --------------------------------------------------------
    // IMPORTANT:
    //
    // Modify these branch names according to your ntuple
    // --------------------------------------------------------

    summaryTree->SetBranchAddress(
        "track_nr", &summary_track_nr);

    summaryTree->SetBranchAddress(
        "t_theta", &truthTheta);

    summaryTree->SetBranchAddress(
        "t_p", &truthP);

    summaryTree->SetBranchAddress(
        "t_pT", &truthPt);



    // ========================================================
    // map:
    //
    // key = (event_nr, track_nr)
    //
    // value = TrackInfo
    // ========================================================

    using TrackKey = std::pair<uint32_t, uint32_t>;

    std::map<TrackKey, TrackInfo> trackMap;

    //get <event_nr, track_nr>
    std::vector<TrackKey> stateEntryMap;


    Long64_t nSummary = summaryTree->GetEntries();
    //std::cout<<"nSummary = "<<nSummary<<std::endl;

    for (Long64_t i = 0; i < nSummary; ++i) {

        summaryTree->GetEntry(i);


        for(unsigned track=0; track<summary_track_nr->size(); track++){
          stateEntryMap.push_back(std::make_pair(i+1, summary_track_nr->at(track)));

          if(std::isnan(truthTheta->at(track)))continue;
          TrackKey key = 
              std::make_pair(
                  i+1,
                  summary_track_nr->at(track)
              );


          TrackInfo info;
          info.truthTheta     = truthTheta->at(track);
          info.truthP          = truthP->at(track);
          info.truthPt          = truthPt->at(track);

          trackMap[key] = info;


        }
        
    }


    std::cout << "Track map size = "
              << trackMap.size() << std::endl;
    std::cout << "Tracks size = "
              << stateEntryMap.size() << std::endl;


    // ========================================================
    // 2. Read TrackState tree
    // ========================================================

    uint32_t track_nr;

    std::vector<int> *volume_id = nullptr;
    std::vector<int> *layer_id = nullptr;

    std::vector<float> *eQOP_flt = nullptr;
    std::vector<float> *eQOP_prt = nullptr;
    std::vector<float> *eQOP_smt = nullptr;

    std::vector<float> *t_eQOP = nullptr;



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
    double xmax = 20;
    


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

    Long64_t nTracks = stateTree->GetEntries();

    Long64_t nMatched = 0;
    Long64_t nMissing = 0;



    for(Long64_t iTrack = 0; iTrack < nTracks; ++iTrack){
      
      stateTree->GetEntry(iTrack);
      // ----------------------------------------------------
      // We only consider stations 1-3
      // ----------------------------------------------------


      TrackKey key = stateEntryMap.at(iTrack);
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
        x = info.truthTheta;

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
