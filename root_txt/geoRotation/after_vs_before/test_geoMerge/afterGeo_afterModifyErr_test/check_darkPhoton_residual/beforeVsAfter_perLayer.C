#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <string>


// ============================================================
// Track information
// ============================================================

struct TrackInfo {

    double trackSeparation =
        std::numeric_limits<double>::quiet_NaN();

    float truthTheta =
        std::numeric_limits<float>::quiet_NaN();
    
    float truthPhi =
        std::numeric_limits<float>::quiet_NaN();

    float truthEta =
        std::numeric_limits<float>::quiet_NaN();
    
    float truthP =
        std::numeric_limits<float>::quiet_NaN();

    float truthPt =
        std::numeric_limits<float>::quiet_NaN();

    float trackP =
        std::numeric_limits<float>::quiet_NaN();

    float trackPurity =
        std::numeric_limits<float>::quiet_NaN();


    // S1 L0 L1 L2 | S2 L0 L1 L2 | S3 L0 L1 L2
    std::array<int, 9> stateCode{};

    // residual of each layer
    std::array<double, 9> residual;
    std::array<double, 9> pTruth;
    std::array<double, 9> pReco;


    TrackInfo()
    {
        residual.fill(
            std::numeric_limits<double>::quiet_NaN()
        );
    }
};


// ============================================================
// event_nr -> track_nr -> TrackInfo
// ============================================================

//using EventTrackMap =
//    std::map<
//        uint32_t,
//        std::map<uint32_t, TrackInfo>
//    >;


using TrackKey = std::pair<uint32_t, uint32_t>;
using EventTrackMap = std::map<TrackKey, TrackInfo>;


// ============================================================
// volume_id + layer_id -> 0 ... 8
// ============================================================

int getLayerIndex(int volume, int layer)
{
    int station = -1;
    int layerIndex = -1;

    if (volume == 25)
        station = 0;
    else if (volume == 27)
        station = 1;
    else if (volume == 29)
        station = 2;
    else
        return -1;


    if (layer == 2)
        layerIndex = 0;
    else if (layer == 4)
        layerIndex = 1;
    else if (layer == 6)
        layerIndex = 2;
    else
        return -1;


    return station * 3 + layerIndex;
}


// ============================================================
// Main
// ============================================================

void beforeVsAfter_perLayer(
    const char* beforeStateFileName =
        "beforeGeo_mc24_newRecoScript_110051_3station_forward_track_states_ckf.root",

    const char* beforeSummaryFileName =
        "beforeGeo_mc24_newRecoScript_110051_3station_forward_track_summary_ckf.root",

    const char* afterStateFileName =
        "110051_mc24_afterGeo_3station_forward_track_states_ckf.root",

    const char* afterSummaryFileName =
        "110051_mc24_afterGeo_3station_forward_track_summary_ckf.root",

    std::string variable =
        "truthPhi",

    std::string stateType =
        "smt"
)
{
    gStyle->SetOptStat(0);

    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadTopMargin(0.08);


    // ==========================================================
    // Open files
    // ==========================================================

    TFile* fBeforeState =
        TFile::Open(beforeStateFileName);

    TFile* fBeforeSummary =
        TFile::Open(beforeSummaryFileName);

    TFile* fAfterState =
        TFile::Open(afterStateFileName);

    TFile* fAfterSummary =
        TFile::Open(afterSummaryFileName);


    if (!fBeforeState || fBeforeState->IsZombie() ||
        !fBeforeSummary || fBeforeSummary->IsZombie() ||
        !fAfterState || fAfterState->IsZombie() ||
        !fAfterSummary || fAfterSummary->IsZombie()) {

        std::cerr
            << "Error opening input files."
            << std::endl;

        return;
    }


    // ==========================================================
    // Get trees
    // ==========================================================

    TTree* beforeState =
        dynamic_cast<TTree*>(
            fBeforeState->Get("tree"));

    TTree* beforeSummary =
        dynamic_cast<TTree*>(
            fBeforeSummary->Get("tree"));

    TTree* afterState =
        dynamic_cast<TTree*>(
            fAfterState->Get("tree"));

    TTree* afterSummary =
        dynamic_cast<TTree*>(
            fAfterSummary->Get("tree"));


    if (!beforeState ||
        !beforeSummary ||
        !afterState ||
        !afterSummary) {

        std::cerr
            << "Cannot find tree."
            << std::endl;

        return;
    }


    // ==========================================================
    // ==========================================================
    // 1. BEFORE GEO
    // ==========================================================
    // ==========================================================


    // ----------------------------------------------------------
    // Summary branches
    // ----------------------------------------------------------


    std::vector<uint32_t>* beforeTrackNr = nullptr;

    std::vector<float>* beforeTruthTheta = nullptr;
    std::vector<float>* beforeTruthPhi = nullptr;
    std::vector<float>* beforeTruthEta = nullptr;
    std::vector<float>* beforeTruthP = nullptr;
    std::vector<float>* beforeTruthPt = nullptr;
    std::vector<float>* beforeTrackP = nullptr;

    std::vector<unsigned int>* beforeNMeasurements =
        nullptr;

    std::vector<unsigned int>* beforeNMajorityHits =
        nullptr;



    beforeSummary->SetBranchAddress(
        "track_nr",
        &beforeTrackNr);

    beforeSummary->SetBranchAddress(
        "t_theta",
        &beforeTruthTheta);
    
    beforeSummary->SetBranchAddress(
        "t_phi",
        &beforeTruthPhi);
    
    beforeSummary->SetBranchAddress(
        "t_eta",
        &beforeTruthEta);

    beforeSummary->SetBranchAddress(
        "t_p",
        &beforeTruthP);

    beforeSummary->SetBranchAddress(
        "t_pT",
        &beforeTruthPt);

    beforeSummary->SetBranchAddress(
        "eQOP_fit",
        &beforeTrackP);

    beforeSummary->SetBranchAddress(
        "nMeasurements",
        &beforeNMeasurements);

    beforeSummary->SetBranchAddress(
        "nMajorityHits",
        &beforeNMajorityHits);


    // ----------------------------------------------------------
    // state entry -> (event_nr, track_nr)
    //
    // This follows your existing stateEntryMap logic:
    // summary entry i -> i+1
    // ----------------------------------------------------------

    std::vector<
        std::pair<uint32_t, uint32_t>
    > beforeStateEntryMap;


    // ----------------------------------------------------------
    // Main beforeGeo map
    //
    // event_nr -> track_nr -> TrackInfo
    // ----------------------------------------------------------

    EventTrackMap beforeEvents;


    for (Long64_t i = 0;
         i < beforeSummary->GetEntries();
         ++i) {

        beforeSummary->GetEntry(i);


        for (size_t track = 0;
             track < beforeTrackNr->size();
             ++track) {

            uint32_t trk =
                beforeTrackNr->at(track);


            beforeStateEntryMap.push_back(
                {i+1, trk}
            );
        }

        for (size_t track = 0; track < beforeTruthTheta->size(); track++){


            TrackInfo info;

            //only store track having majorityParticle and fitted parms

            if(std::isnan(beforeTruthTheta->at(track)))continue;
            if(std::isnan(beforeTrackP->at(track)))continue;


            info.truthTheta =
                beforeTruthTheta->at(track);
            
            info.truthPhi =
                beforeTruthPhi->at(track);
            
            info.truthEta =
                beforeTruthEta->at(track);

            info.truthP =
                beforeTruthP->at(track);

            info.truthPt =
                beforeTruthPt->at(track);


            info.trackP =
                std::abs(
                    1.0 / beforeTrackP->at(track)
                );



            info.trackPurity =
                1.0 *
                beforeNMajorityHits->at(track) /
                beforeNMeasurements->at(track);

            TrackKey key = std::make_pair(i+1, beforeTrackNr->at(track));

            beforeEvents[key] =
                info;
        }
    }


    // ----------------------------------------------------------
    // State branches
    // ----------------------------------------------------------

    uint32_t beforeStateTrackNr = 0;

    std::vector<int>* volume_id = nullptr;
    std::vector<int>* layer_id = nullptr;

    std::vector<float>* eQOP_flt = nullptr;
    std::vector<float>* eQOP_prt = nullptr;
    std::vector<float>* eQOP_smt = nullptr;

    std::vector<float>* t_eQOP = nullptr;


    beforeState->SetBranchAddress(
        "track_nr",
        &beforeStateTrackNr);

    beforeState->SetBranchAddress(
        "volume_id",
        &volume_id);

    beforeState->SetBranchAddress(
        "layer_id",
        &layer_id);

    beforeState->SetBranchAddress(
        "eQOP_flt",
        &eQOP_flt);

    beforeState->SetBranchAddress(
        "eQOP_prt",
        &eQOP_prt);

    beforeState->SetBranchAddress(
        "eQOP_smt",
        &eQOP_smt);

    beforeState->SetBranchAddress(
        "t_eQOP",
        &t_eQOP);


    // ----------------------------------------------------------
    // Fill beforeGeo residual + stateCode
    // ----------------------------------------------------------

    for (Long64_t i = 0;
         i < beforeState->GetEntries();
         ++i) {

        beforeState->GetEntry(i);


        if (i >=
            (Long64_t)beforeStateEntryMap.size())
            continue;



        //uint32_t evt =
        //    beforeStateEntryMap[i].first;

        //uint32_t trk =
        //    beforeStateEntryMap[i].second;


        //auto eit =
        //    beforeEvents.find(evt);

        //if (eit == beforeEvents.end())
        //    continue;


        //auto tit =
        //    eit->second.find(trk);

        //if (tit == eit->second.end())
        //    continue;

        TrackKey key = beforeStateEntryMap.at(i);

        auto it  = beforeEvents.find(key);
        if(it == beforeEvents.end())continue;

        TrackInfo& info = it -> second;



        for (size_t state = 0;
             state < volume_id->size();
             ++state) {

            if (state >= layer_id->size() ||
                state >= eQOP_flt->size() ||
                state >= eQOP_prt->size() ||
                state >= eQOP_smt->size() ||
                state >= t_eQOP->size())
                continue;


            int idx =
                getLayerIndex(
                    volume_id->at(state),
                    layer_id->at(state)
                );


            if (idx < 0)
                continue;


            double eQOP = -999.;


            if (stateType == "flt")
                eQOP = eQOP_flt->at(state);

            else if (stateType == "prt")
                eQOP = eQOP_prt->at(state);

            else if (stateType == "smt")
                eQOP = eQOP_smt->at(state);

            else {
                std::cerr
                    << "Unknown stateType: "
                    << stateType
                    << std::endl;

                return;
            }


            double tQOP =
                t_eQOP->at(state);


            if (!std::isfinite(eQOP) ||
                !std::isfinite(tQOP))
                continue;


            if (eQOP == 0 ||
                tQOP == 0)
                continue;


            double pReco =
                1.0 / std::abs(eQOP);

            double pTruth =
                1.0 /
                std::abs(tQOP) /
                1000.;


            double residual =
                (pReco - pTruth) /
                pTruth;


            if (!std::isfinite(residual))
                continue;


            info.pTruth[idx] =
                pTruth;
            
            info.pReco[idx] =
                pReco;
            
            info.residual[idx] =
                residual;


            //set limit range
            if (residual <= -0.9)
            //if (residual > -0.5)
                info.stateCode[idx] = 1;
        }
    }


    // ==========================================================
    // 2. Find special events
    //
    // S1 L0 L1 L2 = 111
    // ==========================================================



    //for (const auto& [trackKey, info]
    //     : beforeEvents) {


    //  if (info.stateCode[0] == 1 &&
    //      info.stateCode[1] == 1 &&
    //      info.stateCode[2] == 1) {
    //    

    //      specialEvents.insert(trackKey.first);

    //      break;
    //  }
    //}

    std::set<uint32_t> specialEvents;

    std::map<std::pair<uint32_t, uint32_t>, TrackInfo> specialEvents_tracks;
    
    uint32_t currentEvent = std::numeric_limits<uint32_t>::max();
    
    bool isSpecialEvent = false;
    
    // 当前 event 的所有 track 临时保存
    std::map<std::pair<uint32_t, uint32_t>, TrackInfo> currentEventTracks;
    
    for (const auto& [trackKey, info] : beforeEvents) {
    
        uint32_t eventNr = trackKey.first;
    
        // ====================================================
        // 进入新的 event
        // ====================================================
        if (eventNr != currentEvent) {
    
            // 如果上一个 event 是 special event，
            // 把上一个 event 的所有 track 保存下来
            if (currentEvent != std::numeric_limits<uint32_t>::max() &&
                isSpecialEvent) {
    
                specialEvents.insert(currentEvent);
    
                specialEvents_tracks.insert(
                    currentEventTracks.begin(),
                    currentEventTracks.end()
                );
            }
    
            // 开始新的 event
            currentEvent = eventNr;
            isSpecialEvent = false;
            currentEventTracks.clear();
        }
    
        // ====================================================
        // 保存当前 event 的所有 track
        // ====================================================
        currentEventTracks[trackKey] = info;
    
        // ====================================================
        // 检查当前 track 是否满足 special 条件
        // ====================================================
        if (info.stateCode[0] == 1 &&
            info.stateCode[1] == 1 &&
            info.stateCode[2] == 1 &&
            info.stateCode[3] == 1 &&
            info.stateCode[4] == 1 &&
            info.stateCode[5] == 1 &&
            info.stateCode[6] == 1 &&
            info.stateCode[7] == 1 &&
            info.stateCode[8] == 1 
            ) {
    
            isSpecialEvent = true;
        }
    }
    
    // ========================================================
    // 最后一个 event 需要单独处理
    // ========================================================
    if (currentEvent != std::numeric_limits<uint32_t>::max() &&
        isSpecialEvent) {
    
        specialEvents.insert(currentEvent);
    
        specialEvents_tracks.insert(
            currentEventTracks.begin(),
            currentEventTracks.end()
        );
    }

    std::cout
        << "Special beforeGeo events = "
        << specialEvents.size()
        << std::endl;


    // ==========================================================
    // ==========================================================
    // 3. AFTER GEO
    // ==========================================================
    // ==========================================================


    // ----------------------------------------------------------
    // Summary branches
    // ----------------------------------------------------------


    std::vector<uint32_t>* afterTrackNr = nullptr;

    std::vector<float>* afterTruthTheta = nullptr;
    std::vector<float>* afterTruthPhi = nullptr;
    std::vector<float>* afterTruthEta = nullptr;
    std::vector<float>* afterTruthP = nullptr;
    std::vector<float>* afterTruthPt = nullptr;
    std::vector<float>* afterTrackP = nullptr;

    std::vector<unsigned int>* afterNMeasurements =
        nullptr;

    std::vector<unsigned int>* afterNMajorityHits =
        nullptr;



    afterSummary->SetBranchAddress(
        "track_nr",
        &afterTrackNr);

    afterSummary->SetBranchAddress(
        "t_theta",
        &afterTruthTheta);
    
    afterSummary->SetBranchAddress(
        "t_phi",
        &afterTruthPhi);
    
    afterSummary->SetBranchAddress(
        "t_eta",
        &afterTruthEta);

    afterSummary->SetBranchAddress(
        "t_p",
        &afterTruthP);

    afterSummary->SetBranchAddress(
        "t_pT",
        &afterTruthPt);

    afterSummary->SetBranchAddress(
        "eQOP_fit",
        &afterTrackP);

    afterSummary->SetBranchAddress(
        "nMeasurements",
        &afterNMeasurements);

    afterSummary->SetBranchAddress(
        "nMajorityHits",
        &afterNMajorityHits);


    // ----------------------------------------------------------
    // after state entry -> event + track
    // ----------------------------------------------------------

    std::vector<
        std::pair<uint32_t, uint32_t>
    > afterStateEntryMap;


    // ----------------------------------------------------------
    // Main afterGeo map
    //
    // event_nr -> track_nr -> TrackInfo
    // ----------------------------------------------------------

    EventTrackMap afterEvents;


    for (Long64_t i = 0;
         i < afterSummary->GetEntries();
         ++i) {

        afterSummary->GetEntry(i);


        // Only events selected by beforeGeo
        if (!specialEvents.count(i+1))
            continue;


        for (size_t track = 0;
             track < afterTrackNr->size();
             ++track) {

            uint32_t trk =
                afterTrackNr->at(track);


            afterStateEntryMap.push_back(
                {i+1, trk}
            );
        }



        for(size_t track = 0; track < afterTruthTheta->size(); track++){

          TrackInfo info;
          if(std::isnan(afterTruthTheta->at(track)))continue;
          if(std::isnan(afterTrackP->at(track)))continue;

          info.truthTheta =
              afterTruthTheta->at(track);

          info.truthPhi =
              afterTruthPhi->at(track);
          
          info.truthEta =
              afterTruthEta->at(track);
          info.truthP =
              afterTruthP->at(track);

          info.truthPt =
              afterTruthPt->at(track);


          info.trackP =
              std::abs(
                  1.0 / afterTrackP->at(track)
              );



          info.trackPurity =
              1.0 *
              afterNMajorityHits->at(track) /
              afterNMeasurements->at(track);

          TrackKey key = std::make_pair(i+1, afterTrackNr->at(track));


            afterEvents[key] =
                info;
        }
    }


    // ==========================================================
    // 4. Fill afterGeo state
    // ==========================================================

    uint32_t afterStateTrackNr = 0;


    afterState->SetBranchAddress(
        "track_nr",
        &afterStateTrackNr);

    afterState->SetBranchAddress(
        "volume_id",
        &volume_id);

    afterState->SetBranchAddress(
        "layer_id",
        &layer_id);

    afterState->SetBranchAddress(
        "eQOP_flt",
        &eQOP_flt);

    afterState->SetBranchAddress(
        "eQOP_prt",
        &eQOP_prt);

    afterState->SetBranchAddress(
        "eQOP_smt",
        &eQOP_smt);

    afterState->SetBranchAddress(
        "t_eQOP",
        &t_eQOP);


    // ----------------------------------------------------------
    // Important:
    //
    // afterStateEntryMap contains ONLY special events.
    // Therefore we cannot use the original state entry index
    // directly.
    //
    // Here we use the same ordering of the selected summary
    // entries.
    // ----------------------------------------------------------

    for (Long64_t i = 0;
         i < afterState->GetEntries();
         ++i) {

        afterState->GetEntry(i);


        if (i >=
            (Long64_t)afterStateEntryMap.size())
            continue;


        //uint32_t evt =
        //    afterStateEntryMap[i].first;

        //uint32_t trk =
        //    afterStateEntryMap[i].second;


        //auto eit =
        //    afterEvents.find(evt);

        //if (eit == afterEvents.end())
        //    continue;


        //auto tit =
        //    eit->second.find(trk);

        //if (tit == eit->second.end())
        //    continue;


        TrackKey key = afterStateEntryMap.at(i);
        auto it = afterEvents.find(key);
        if(it == afterEvents.end())continue;
        TrackInfo& info =
            it->second;


        for (size_t state = 0;
             state < volume_id->size();
             ++state) {

            if (state >= layer_id->size() ||
                state >= eQOP_flt->size() ||
                state >= eQOP_prt->size() ||
                state >= eQOP_smt->size() ||
                state >= t_eQOP->size())
                continue;


            int idx =
                getLayerIndex(
                    volume_id->at(state),
                    layer_id->at(state)
                );


            if (idx < 0)
                continue;


            double eQOP = -999.;


            if (stateType == "flt")
                eQOP = eQOP_flt->at(state);

            else if (stateType == "prt")
                eQOP = eQOP_prt->at(state);

            else if (stateType == "smt")
                eQOP = eQOP_smt->at(state);


            double tQOP =
                t_eQOP->at(state);


            if (!std::isfinite(eQOP) ||
                !std::isfinite(tQOP))
                continue;


            if (eQOP == 0 ||
                tQOP == 0)
                continue;


            double pReco =
                1.0 / std::abs(eQOP);

            double pTruth =
                1.0 /
                std::abs(tQOP) /
                1000.;


            double residual =
                (pReco - pTruth) /
                pTruth;


            if (!std::isfinite(residual))
                continue;


            info.pTruth[idx] =
                pTruth;
            info.pReco[idx] =
                pReco;
            info.residual[idx] =
                residual;


            if (residual <= -0.9)
                info.stateCode[idx] = 1;
        }
    }


    // ==========================================================
    // 5. Create 9 TH2F
    //
    // x = afterGeo track variable
    // y = afterGeo residual
    // ==========================================================

    TH2F* h2[3][3];


    double xmin = -0.001;
    double xmax = 0.001;

//    double xmin = 0;
//    double xmax = 0.001;

    // Default y range for residual
    double ymin = -1.1;
    double ymax = 1.1;


    for (int st = 0;
         st < 3;
         ++st) {

        for (int layer = 0;
             layer < 3;
             ++layer) {

            h2[st][layer] =
                new TH2F(
                    Form(
                        "h2_S%d_L%d",
                        st + 1,
                        layer
                    ),

                    //Form(
                    //    "S%d L%d;%s;Residual",
                    //    st + 1,
                    //    layer,
                    //    variable.c_str()
                    //),
                    ";p_{Truth}[Gev];p_{Reco}[Gev]",

                    100,
                    0,
                    5000,

                    120,
                    0,
                    5000
                );
        }
    }


    // ==========================================================
    // 6. Fill histograms
    //
    // ONLY afterGeo special events
    // ==========================================================

    //loop over events;(specialEvents_tracks / afterEvents)
    Long64_t nFilled = 0;


    for (auto& [trackKey, info]
         : specialEvents_tracks) {



            // --------------------------------------------------
            // Select x variable
            // --------------------------------------------------

            double x =
                std::numeric_limits<double>::quiet_NaN();


            if (variable == "trackSeparation")
                x = info.trackSeparation;

            else if (variable == "truthTheta")
                x = info.truthTheta;
            
            else if (variable == "truthPhi")
                x = info.truthPhi;
            
            else if (variable == "truthEta")
                x = info.truthEta;

            else if (variable == "truthP")
                x = info.truthP;

            else if (variable == "truthPt")
                x = info.truthPt;

            else if (variable == "trackP")
                x = info.trackP;

            else if (variable == "trackPurity")
                x = info.trackPurity;

            else {

                std::cerr
                    << "Unknown variable: "
                    << variable
                    << std::endl;

                return;
            }


            if (!std::isfinite(x))
                continue;


            // --------------------------------------------------
            // Fill 9 layers
            // --------------------------------------------------

            for (int idx = 0;
                 idx < 9;
                 ++idx) {

                if (!std::isfinite(
                        info.residual[idx]))
                    continue;


                int station =
                    idx / 3;

                int layer =
                    idx % 3;


                //h2[station][layer]->Fill(
                //    x,
                //    info.residual[idx]
                h2[station][layer]->Fill(
                    info.pTruth[idx],
                    info.pReco[idx]
                );


                ++nFilled;
            }
    }


    std::cout
        << "Filled entries = "
        << nFilled
        << std::endl;


    // ==========================================================
    // 7. Draw 3 x 3
    // ==========================================================

    TCanvas* c =
        new TCanvas(
            Form(
                "c_%s_%s",
                variable.c_str(),
                stateType.c_str()
            ),

            Form(
                "AfterGeo special events: %s vs residual",
                variable.c_str()
            ),

            1400,
            1000
        );


    c->Divide(3, 3);


    for (int st = 0;
         st < 3;
         ++st) {

        for (int layer = 0;
             layer < 3;
             ++layer) {

            c->cd(
                st * 3 + layer + 1
            );

            h2[st][layer]->Draw("COLZ");
        }
    }


   // c->SaveAs(
   //     Form(
   //         "afterGeo_special_%s_%s_2D.png",
   //         variable.c_str(),
   //         stateType.c_str()
   //     )
   // );
}
