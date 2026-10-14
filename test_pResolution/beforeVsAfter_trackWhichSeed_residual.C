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
struct seedInfo{
  float seedLoc0;
  float seedLoc1;
  float seedTheta;
  float seedPhi;
  float seedQoverP;
  float tLoc0;
  float tLoc1;
  float tTheta;
  float tPhi;
  float tQoverP;
};

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

    unsigned int nMeasurements;
    unsigned int nSharedHits;
    unsigned int nOutliers ;
    unsigned int nHoles;
    float chi2OverNDF = -999.;

    unsigned int seedNr;
    seedInfo seedInfo;


    // S1 L0 L1 L2 | S2 L0 L1 L2 | S3 L0 L1 L2
    std::array<int, 9> stateCode{};

    // residual of each layer
    std::array<double, 9> residual;
    std::array<double, 9> pTruth;
    std::array<double, 9> pReco;
    std::array<int, 9> qMatch;


    TrackInfo()
    {
        residual.fill(
            std::numeric_limits<double>::quiet_NaN()
        );
    }
};
int sign(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}


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

void beforeVsAfter_trackWhichSeed_residual(
    const char* beforeStateFileName =
        "beforeGeo_mc24_newRecoScript_110051_3station_forward_track_states_ckf.root",

    const char* beforeSummaryFileName =
        "beforeGeo_addTrackWhichSeed_3station_forward_track_summary_ckf.root",

    const char* afterStateFileName =
        "110051_mc24_afterGeo_3station_forward_track_states_ckf.root",

    const char* afterSummaryFileName =
        "afterGeo_addTrackWhichSeed_3station_forward_track_summary_ckf.root",
    
    const char* seedFileName = "beforeGeo_addTrackWhichSeed_3station_forward_seed_summary_circleFitTrackSeedTool.root",
    const char* seedFileName1 = "afterGeo_addTrackWhichSeed_3station_forward_seed_summary_circleFitTrackSeedTool.root",

    std::string variable =
        "chi2OverNDF",

    std::string stateType =
        "flt"
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
    
    TFile* seedFile = TFile::Open(seedFileName);
    TFile* seedFile1 = TFile::Open(seedFileName1);

    if (!seedFile || seedFile->IsZombie()) {
        std::cerr << "Cannot open seed file: "
                  << seedFileName << std::endl;
        return;
    }

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

    TTree* seedTree =
        dynamic_cast<TTree*>(seedFile->Get("tree"));
    TTree* seedTree1 =
        dynamic_cast<TTree*>(seedFile1->Get("tree"));
    
    if (!beforeState ||
        !beforeSummary ||
        !afterState ||
        !afterSummary) {

        std::cerr
            << "Cannot find tree."
            << std::endl;

        return;
    }

    // ========================================================
    // Read SeedSummary
    // ========================================================

    std::vector<float>* seedLoc0=nullptr;
    std::vector<float>* seedLoc1=nullptr;
    std::vector<float>* seedTheta=nullptr;
    std::vector<float>* seedPhi=nullptr;
    std::vector<float>* seedQoverP=nullptr;

    std::vector<float>* tLoc0=nullptr;
    std::vector<float>* tLoc1=nullptr;
    std::vector<float>* tTheta=nullptr;
    std::vector<float>* tPhi=nullptr;
    std::vector<float>* tQoverP=nullptr;

    seedTree->SetBranchAddress(
        "track_loc0", &seedLoc0);
    seedTree->SetBranchAddress(
        "track_loc1", &seedLoc1);
    seedTree->SetBranchAddress(
        "track_theta", &seedTheta);
    seedTree->SetBranchAddress(
        "track_phi", &seedPhi);
    seedTree->SetBranchAddress(
        "track_QoverP", &seedQoverP);
    seedTree->SetBranchAddress(
        "t_loc0", &tLoc0);
    seedTree->SetBranchAddress(
        "t_loc1", &tLoc1);
    seedTree->SetBranchAddress(
        "t_theta", &tTheta);
    seedTree->SetBranchAddress(
        "t_phi", &tPhi);
    seedTree->SetBranchAddress(
        "t_QoverP", &tQoverP);


    std::map<TrackKey, seedInfo> seedMap;


    Long64_t entries = seedTree->GetEntries();

    for (Long64_t i = 0; i < entries; ++i) {

        seedTree->GetEntry(i);
        for(size_t seed = 0; seed < seedQoverP->size(); seed++){
          
          //only focus on tracks of matching truthParticle
          if(std::isnan(tLoc0->at(seed)))continue;
          TrackKey seedKey = std::make_pair(i+1, seed);
          seedMap[seedKey].seedLoc0 = seedLoc0->at(seed);
          seedMap[seedKey].seedLoc1 = seedLoc1->at(seed);
          seedMap[seedKey].seedTheta = seedTheta->at(seed);
          seedMap[seedKey].seedPhi = seedPhi->at(seed);
          seedMap[seedKey].seedQoverP = seedQoverP->at(seed);
          seedMap[seedKey].tLoc0 = tLoc0->at(seed);
          seedMap[seedKey].tLoc1 = tLoc1->at(seed);
          seedMap[seedKey].tTheta = tTheta->at(seed);
          seedMap[seedKey].tPhi = tPhi->at(seed);
          seedMap[seedKey].tQoverP = tQoverP->at(seed);

        }
    }

    std::vector<float>* AseedLoc0=nullptr;
    std::vector<float>* AseedLoc1=nullptr;
    std::vector<float>* AseedTheta=nullptr;
    std::vector<float>* AseedPhi=nullptr;
    std::vector<float>* AseedQoverP=nullptr;

    std::vector<float>* AtLoc0=nullptr;
    std::vector<float>* AtLoc1=nullptr;
    std::vector<float>* AtTheta=nullptr;
    std::vector<float>* AtPhi=nullptr;
    std::vector<float>* AtQoverP=nullptr;

    seedTree1->SetBranchAddress(
        "track_loc0", &AseedLoc0);
    seedTree1->SetBranchAddress(
        "track_loc1", &AseedLoc1);
    seedTree1->SetBranchAddress(
        "track_theta", &AseedTheta);
    seedTree1->SetBranchAddress(
        "track_phi", &AseedPhi);
    seedTree1->SetBranchAddress(
        "track_QoverP", &AseedQoverP);
    seedTree1->SetBranchAddress(
        "t_loc0", &AtLoc0);
    seedTree1->SetBranchAddress(
        "t_loc1", &AtLoc1);
    seedTree1->SetBranchAddress(
        "t_theta", &AtTheta);
    seedTree1->SetBranchAddress(
        "t_phi", &AtPhi);
    seedTree1->SetBranchAddress(
        "t_QoverP", &AtQoverP);


    std::map<TrackKey, seedInfo> AseedMap;


    Long64_t entries1 = seedTree1->GetEntries();

    for (Long64_t i = 0; i < entries1; ++i) {

        seedTree1->GetEntry(i);
        for(size_t seed = 0; seed < AseedQoverP->size(); seed++){
          
          //only focus on tracks of matching truthParticle
          if(std::isnan(AtLoc0->at(seed)))continue;
          TrackKey seedKey = std::make_pair(i+1, seed);
          AseedMap[seedKey].seedLoc0 = AseedLoc0->at(seed);
          AseedMap[seedKey].seedLoc1 = AseedLoc1->at(seed);
          AseedMap[seedKey].seedTheta = AseedTheta->at(seed);
          AseedMap[seedKey].seedPhi = AseedPhi->at(seed);
          AseedMap[seedKey].seedQoverP = AseedQoverP->at(seed);
          AseedMap[seedKey].tLoc0 = AtLoc0->at(seed);
          AseedMap[seedKey].tLoc1 = AtLoc1->at(seed);
          AseedMap[seedKey].tTheta = AtTheta->at(seed);
          AseedMap[seedKey].tPhi = AtPhi->at(seed);
          AseedMap[seedKey].tQoverP = AtQoverP->at(seed);

        }
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
    std::vector<unsigned int> *beforeNOutliers = nullptr;
    std::vector<unsigned int> *beforeNHoles = nullptr;
    std::vector<float> *beforeChi2Sum = nullptr;
    std::vector<float> *beforeNDF = nullptr;

    std::vector<unsigned int>* beforeNMajorityHits =
        nullptr;
    //matched seed
    std::vector<unsigned int> *beforeSeedNr = nullptr;


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
        "nHoles",
        &beforeNHoles);
    
    beforeSummary->SetBranchAddress(
        "nOutliers",
        &beforeNOutliers);
    
    beforeSummary->SetBranchAddress(
        "chi2Sum",
        &beforeChi2Sum);
    
    beforeSummary->SetBranchAddress(
        "NDF",
        &beforeNDF);

    beforeSummary->SetBranchAddress(
        "nMajorityHits",
        &beforeNMajorityHits);
    
    beforeSummary->SetBranchAddress(
        "seed_nr",
        &beforeSeedNr);


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
    //TH1F* h5 = new TH1F("h1", "; residualP; events",100,-1.1, 1.1);


    //std::set<std::pair<unsigned int, unsigned int>> fillSeeds;
    for (Long64_t i = 0;
         i < beforeSummary->GetEntries();
         ++i) {

        beforeSummary->GetEntry(i);


        for (size_t track = 0;
             track < beforeTrackNr->size();
             ++track) {

          
          TrackInfo info;

            uint32_t trk =
                beforeTrackNr->at(track);
          
          //info.nMeasurements = beforeNMeasurements->at(track);
          //info.nOutliers = beforeNOutliers->at(track);
          //info.nHoles = beforeNHoles->at(track);
          //info.chi2OverNDF = (1.0 * (beforeChi2Sum->at(track))) / (beforeNDF->at(track));
          TrackKey key = std::make_pair(i+1, trk);

          unsigned int seedNr = beforeSeedNr->at(track);
          TrackKey seedKey = std::make_pair(i+1, seedNr);
          auto it = seedMap.find(seedKey);
          if (it == seedMap.end()) {
              continue;
          }
      //    if(!fillSeeds.insert({i+1, seedNr}).second)continue;
          
    
          const seedInfo& seedinfo = it->second;
          float residualP = (1.0/std::abs(seedinfo.seedQoverP)-1.0/std::abs(seedinfo.tQoverP)) / (1.0/std::abs(seedinfo.tQoverP));
          //std::cout<<residualP<<std::endl;
          //if((sign(seedinfo.seedQoverP) != sign(seedinfo.seedQoverP)))continue;
          //if(std::abs(residualP)>0.1)continue;
          //if(std::abs(seedinfo.seedTheta - seedinfo.tTheta)>0.0001)continue;
          //if(residualP>-0.8)continue;
          info.seedNr = seedNr;
          info.seedInfo = seedinfo;
          beforeEvents[key] = info;


          beforeStateEntryMap.push_back(
              {i+1, trk}
          );
        }

        //for (size_t track = 0; track < beforeTruthTheta->size(); track++){


        //    TrackInfo info;

        //    //only store track having majorityParticle and fitted parms

        //    if(std::isnan(beforeTruthTheta->at(track)))continue;
        //    if(std::isnan(beforeTrackP->at(track)))continue;


        //    info.truthTheta =
        //        beforeTruthTheta->at(track);
        //    
        //    info.truthPhi =
        //        beforeTruthPhi->at(track);
        //    
        //    info.truthEta =
        //        beforeTruthEta->at(track);

        //    info.truthP =
        //        beforeTruthP->at(track);

        //    info.truthPt =
        //        beforeTruthPt->at(track);


        //    info.trackP =
        //        std::abs(
        //            1.0 / beforeTrackP->at(track)
        //        );



        //    info.trackPurity =
        //        1.0 *
        //        beforeNMajorityHits->at(track) /
        //        beforeNMeasurements->at(track);

        //    TrackKey key = std::make_pair(i+1, beforeTrackNr->at(track));

        //    beforeEvents[key] =
        //        info;
        //}
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

            if(sign(eQOP) == sign(tQOP)){
              info.qMatch[idx] = 1;
            }else{
              info.qMatch[idx] = 0;
            }
            
            info.residual[idx] =
                residual;


            //set limit range
            //if (std::abs(residual) < 0.5)
            //if (residual < -0.9)
            //    info.stateCode[idx] = 1;
        }
    }


    // ==========================================================
    // 2. Find special events
    //
    // S1 L0 L1 L2 = 111
    // ==========================================================


    //key <event, seedNr> 
    std::map<std::pair<uint32_t, uint32_t>, TrackInfo> special_tracks;

    //key<event, trackNr>
    std::map<std::pair<uint32_t, uint32_t>, TrackInfo> before_special_tracks;
    
    for (const auto& [trackKey, info] : beforeEvents) {
    
        uint32_t eventNr = trackKey.first;
    
    
        // ====================================================
        // 检查当前 track 是否满足 special 条件
        // ====================================================
        //if (info.stateCode[0] == 1 &&
        //    info.stateCode[1] == 1 &&
        //    info.stateCode[2] == 1  
        //    //info.stateCode[3] == 1 &&
        //    //info.stateCode[4] == 1 &&
        //    //info.stateCode[5] == 1 &&
        //    //info.stateCode[6] == 1 &&
        //    //info.stateCode[7] == 1 &&
        //    //info.stateCode[8] == 1 
        //   ){
   
          unsigned int seedNr = info.seedNr;
          TrackKey key = std::make_pair(eventNr, seedNr);
          special_tracks[key] = info;
          before_special_tracks[trackKey] = info;

    
        //}
    }
    


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
    std::vector<unsigned int> *afterNOutliers = nullptr;
    std::vector<unsigned int> *afterNHoles = nullptr;
    std::vector<float> *afterChi2Sum = nullptr;
    std::vector<float> *afterNDF = nullptr;

    std::vector<unsigned int>* afterNMajorityHits =
        nullptr;
    std::vector<unsigned int>* afterSeedNr =
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
        "nHoles",
        &afterNHoles);
    
    afterSummary->SetBranchAddress(
        "nOutliers",
        &afterNOutliers);
    
    afterSummary->SetBranchAddress(
        "chi2Sum",
        &afterChi2Sum);
    
    afterSummary->SetBranchAddress(
        "NDF",
        &afterNDF);

    afterSummary->SetBranchAddress(
        "nMajorityHits",
        &afterNMajorityHits);
    afterSummary->SetBranchAddress(
        "seed_nr",
        &afterSeedNr);


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


        for (size_t track = 0;
             track < afterTrackNr->size();
             ++track) {

            uint32_t trk =
                afterTrackNr->at(track);
            
            TrackInfo info;
            //info.nMeasurements = afterNMeasurements->at(track);
            //info.nOutliers = afterNOutliers->at(track);
            //info.nHoles = afterNHoles->at(track);
            //info.chi2OverNDF = (1.0 * (afterChi2Sum->at(track))) / (afterNDF->at(track));
            TrackKey key = std::make_pair(i+1, trk);

            unsigned int seedNr = afterSeedNr->at(track);
            TrackKey seedKey1 = std::make_pair(i+1, seedNr);
            auto seedit = AseedMap.find(seedKey1);
            if (seedit == AseedMap.end()) {
                continue;
            }
            const seedInfo& seedinfo = seedit->second;
            info.seedNr = seedNr;
            info.seedInfo = seedinfo;

            //Only tracks selected by seed(beforeGeo:bad tracks from which seed)
            TrackKey seedKey = std::make_pair(i+1, seedNr);
            auto it = special_tracks.find(seedKey);
            if (it == special_tracks.end()) {
                continue;
            }
            

            

             afterEvents[key] = info;


            afterStateEntryMap.push_back(
                {i+1, trk}
            );
        }



        //for(size_t track = 0; track < afterTruthTheta->size(); track++){

        //  TrackInfo info;
        //  if(std::isnan(afterTruthTheta->at(track)))continue;
        //  if(std::isnan(afterTrackP->at(track)))continue;

        //  info.truthTheta =
        //      afterTruthTheta->at(track);

        //  info.truthPhi =
        //      afterTruthPhi->at(track);
        //  
        //  info.truthEta =
        //      afterTruthEta->at(track);
        //  info.truthP =
        //      afterTruthP->at(track);

        //  info.truthPt =
        //      afterTruthPt->at(track);


        //  info.trackP =
        //      std::abs(
        //          1.0 / afterTrackP->at(track)
        //      );



        //  info.trackPurity =
        //      1.0 *
        //      afterNMajorityHits->at(track) /
        //      afterNMeasurements->at(track);

        //  TrackKey key = std::make_pair(i+1, afterTrackNr->at(track));


        //    afterEvents[key] =
        //        info;
        //}
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
            
            if(sign(eQOP) == sign(tQOP)){
              info.qMatch[idx] = 1;
            }else{
              info.qMatch[idx] = 0;
            }
            info.residual[idx] =
                residual;


            //if (residual < -0.9)
            //    info.stateCode[idx] = 1;
        }
    }


    // ==========================================================
    // 5. Create 9 TH2F
    //
    // x = afterGeo track variable
    // y = afterGeo residual
    // ==========================================================

    TH2F* h2[3][3];
    TH1F* h4[3][3];
    TH3F* h5[3][3];


    double xmin = 0.5;
    double xmax = 2.5;

//    double xmin = 0;
//    double xmax = 0.001;

    // Default y range for residual
    double ymin = -1.1;
    double ymax = 1.1;
    TH1F* h1 = new TH1F("h1",";trackMatchedSeed_(seedP-tP)/tP;Events",100,-1.1,1.1);
    //TH1F* h1 = new TH1F("h1",";trackMatchedSeed_(QOP^{seed}-QOP^{truth});Events",100,-0.2,0.2);
    //TH1F* h1 = new TH1F("h1",";trackMatchedSeed_(#theta^{seed}-#theta^{truth});Events",100,-0.01,0.01);
    TH2F* h3 = new TH2F("h3",";trackMatchedSeed_tP;trackMatchedSeed_seedP",100,0,1500,100,0,1500);


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
                    //";p_{Truth}[Gev];p_{Reco}[Gev]",
                    ";p_{seedingReco}[Gev];p_{CKFReco}[Gev]",
                    //";qMatch(0/1);P^{reco}-P^{truth} / p^{truth}",
                    //";seeding_residualP;P^{reco}-P^{truth} / p^{truth}",
                    100,
                    0,
                    1500,

                    100,
                    0,
                    1500
                    

                    //50,
                    //xmin,
                    //xmax,

                    //100,
                    //ymin,
                    //ymax
                );
            h4[st][layer] = new TH1F(Form(
                        "h4_S%d_L%d",
                        st + 1,
                        layer
                    ),";P^{reco}-P^{truth} / p^{truth};Events",100,-1.1,1.1);
            h5[st][layer] = new TH3F(Form("h5_S%d_L%d", st+1, layer),";seeding_residualTheta; seeding_residualP; P^{reco}-P^{truth} / p^{truth}", 100,-0.001,0.006, 100, -1.1, 1.1, 100, -1.1, 1.1);
        }
    }


    // ==========================================================
    // 6. Fill histograms
    //
    // ONLY afterGeo special events
    // ==========================================================

    //loop over events;(before_special_tracks / afterEvents)
    std::set<std::pair<unsigned int, unsigned int>> filledSeeds;


    for (auto& [trackKey, info]
         : afterEvents) {



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
            
            else if (variable == "nMeasurements")
                x = info.nMeasurements;
            
            else if (variable == "nOutliers")
                x = info.nOutliers;
            
            else if (variable == "nHoles")
                x = info.nHoles;
            
            else if (variable == "chi2OverNDF")
                x = info.chi2OverNDF;

            else {

                std::cerr
                    << "Unknown variable: "
                    << variable
                    << std::endl;

                return;
            }


            if (!std::isfinite(x))
                continue;

            //unsigned int eventNr = trackKey.first;
            //unsigned int seedNr = info.seedNr;

            ////only fill the seed once if this seed produce more tracks;
            //if(!filledSeeds.insert({eventNr, seedNr}).second){
            //
            //  //std::cout<<"have yet filled"<<std::endl;
            //  continue;
            //}

            //float seed_P = 1.0 / std::abs(info.seedInfo.seedQoverP);
            //float t_P = 1.0 / std::abs(info.seedInfo.tQoverP);
            //////h1->Fill(std::abs(info.seedInfo.seedQoverP)-std::abs(info.seedInfo.tQoverP));
            //////h1->Fill(info.seedInfo.seedTheta - info.seedInfo.tTheta);
           // h1->Fill((1.0/std::abs(info.seedInfo.seedQoverP) - 1.0/std::abs(info.seedInfo.tQoverP)) / (1.0/std::abs(info.seedInfo.tQoverP)));

            //////h1->Fill(info.seedInfo.seedTheta - info.seedInfo.tTheta);

            //h3->Fill(t_P, seed_P);

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


                //std::cout<<info.residual[idx]<<std::endl;
                //h4[station][layer]->Fill(info.residual[idx]);
                 //h2[station][layer]->Fill(
                 //   info.qMatch[idx],
                 //   info.residual[idx]
                 //h2[station][layer]->Fill(
                 //   (1.0/std::abs(info.seedInfo.seedQoverP) - 1.0/std::abs(info.seedInfo.tQoverP)) / (1.0/std::abs(info.seedInfo.tQoverP)),
                 //   info.residual[idx]
                 //h5[station][layer]->Fill(
                 //    info.seedInfo.seedTheta - info.seedInfo.tTheta,
                 //   (1.0/std::abs(info.seedInfo.seedQoverP) - 1.0/std::abs(info.seedInfo.tQoverP)) / (1.0/std::abs(info.seedInfo.tQoverP)),
                 //   info.residual[idx]
                h2[station][layer]->Fill(
                    1.0/std::abs(info.seedInfo.seedQoverP),
                    info.pReco[idx]
                );


            }
    }




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

            //1400,
            //1000
            1200,
            700
        );
    //c->cd();
    //h1->Draw();
    //h3->Draw("COLZ");


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

            h2[st][layer]->Draw("colz");
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
