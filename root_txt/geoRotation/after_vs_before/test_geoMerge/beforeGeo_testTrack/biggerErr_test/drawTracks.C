
//nominal bad events
//ievent = 17 //6 seeds
//ievent = 28 //8 seeds


// dashed circles are particle positions at station
// non-dashed circles are measurement positions 
// dots are measurements on reconstructed tracks 

void myText(Double_t x,Double_t y,Color_t color, float font, const char *text);


void drawTracks(int checkEvent = 25){
  std::map<int, std::string> particleNames;

  gStyle->SetOptFit(0011);
  gStyle->SetOptStat(0000);

  std::string sample = "FaserMC-MC24_FS_Aee_010MeV_1Em5-110033-00000-00001-master_dev";
  std::string trackingMode = "Forward tracking, 3 station [beforeGeometry]";

  //Just use it to retrieve the runnumber for provided event no.
  TFile *file1 = TFile::Open("FaserMC-MC24_FS_Aee_010MeV_1Em5-110033-00000-00001-master_dev_3station_forward_track_summary_ckf.root");
  TFile *file2 = TFile::Open("FaserMC-MC24_FS_Aee_010MeV_1Em5-110033-00000-00001-master_dev_3station_forward_track_states_ckf.root");
  TFile *file3 = TFile::Open("FaserMC-MC24_FS_Aee_010MeV_1Em5-110033-00000-00001-master_dev_3station_forward_seed_summary_circleFitTrackSeedTool.root");
  TTree* trackSummary = (TTree*)file1->Get("tree");
  TTree* trackState = (TTree*)file2->Get("tree");
  TTree* trackSeed = (TTree*)file3->Get("tree");
  
  
  uint32_t event_nr_1; 
  std::vector<double>* inputMeasurementLocX = new std::vector<double>;
  std::vector<double>* inputMeasurementLocY = new std::vector<double>;
  std::vector<double>* inputMeasurementGloX = new std::vector<double>;
  std::vector<double>* inputMeasurementGloY = new std::vector<double>;
  std::vector<double>* inputMeasurementGloZ = new std::vector<double>;
  std::vector<double>* inputMeasurementVolume = new std::vector<double>;
  std::vector<double>* inputMeasurementLayer = new std::vector<double>;
  std::vector<double>* inputMeasurementSensitive = new std::vector<double>;
  std::vector<double>* inputMeasurementIndex = new std::vector<double>;
  std::vector<uint64_t>* inputMeasurementParticleId = new std::vector<uint64_t>;
  std::vector<double>* inputMeasurementParticleP = new std::vector<double>;

  trackSummary->SetBranchAddress("event_nr", &event_nr_1);
  trackSummary->SetBranchAddress("inputMeasurementLocX", &inputMeasurementLocX);
  trackSummary->SetBranchAddress("inputMeasurementLocY", &inputMeasurementLocY);
  trackSummary->SetBranchAddress("inputMeasurementGloX", &inputMeasurementGloX);
  trackSummary->SetBranchAddress("inputMeasurementGloY", &inputMeasurementGloY);
  trackSummary->SetBranchAddress("inputMeasurementGloZ", &inputMeasurementGloZ);
  trackSummary->SetBranchAddress("inputMeasurementVolume", &inputMeasurementVolume);
  trackSummary->SetBranchAddress("inputMeasurementLayer", &inputMeasurementLayer);
  trackSummary->SetBranchAddress("inputMeasurementSensitive", &inputMeasurementSensitive);
  trackSummary->SetBranchAddress("inputMeasurementIndex", &inputMeasurementIndex);
  trackSummary->SetBranchAddress("inputMeasurementParticleId", &inputMeasurementParticleId);
  trackSummary->SetBranchAddress("inputMeasurementParticleP", &inputMeasurementParticleP);
 

  uint32_t event_nr_2;
  int majorityParticleId; 
  std::vector<float>*  g_x_hit = new  std::vector<float>;  
  std::vector<float>*  g_y_hit = new  std::vector<float>;  
  std::vector<float>*  g_z_hit = new  std::vector<float>;  
  std::vector<int>*  index_hit = new  std::vector<int>;  

  trackState->SetBranchAddress("event_nr", &event_nr_2);
  trackState->SetBranchAddress("majorityParticleId", &majorityParticleId);
  trackState->SetBranchAddress("g_x_hit", &g_x_hit);
  trackState->SetBranchAddress("g_y_hit", &g_y_hit);
  trackState->SetBranchAddress("g_z_hit", &g_z_hit);
  trackState->SetBranchAddress("index_hit", &index_hit);
 

  uint32_t event_nr_3;
  //Entries is the number of fiducial particles
  std::vector<double>*  truth_st0_x = new  std::vector<double>; 
  std::vector<double>*  truth_st0_y = new  std::vector<double>; 
  std::vector<double>*  truth_st0_z = new  std::vector<double>; 
  std::vector<double>*  truth_st1_x = new  std::vector<double>; 
  std::vector<double>*  truth_st1_y = new  std::vector<double>; 
  std::vector<double>*  truth_st1_z = new  std::vector<double>; 
  std::vector<double>*  truth_st2_x = new  std::vector<double>; 
  std::vector<double>*  truth_st2_y = new  std::vector<double>; 
  std::vector<double>*  truth_st2_z = new  std::vector<double>; 
  std::vector<double>*  truth_st3_x = new  std::vector<double>; 
  std::vector<double>*  truth_st3_y = new  std::vector<double>; 
  std::vector<double>*  truth_st3_z = new  std::vector<double>; 
  std::vector<uint64_t>* truth_barcode = new std::vector<uint64_t>;
  std::vector<int>* truth_pdg = new std::vector<int>;


  trackSeed->SetBranchAddress("event_number", &event_nr_3);
  trackSeed->SetBranchAddress("truth_barcode", &truth_barcode);
  trackSeed->SetBranchAddress("truth_pdg", &truth_pdg);
  trackSeed->SetBranchAddress("truth_st0_x", &truth_st0_x);
  trackSeed->SetBranchAddress("truth_st0_y", &truth_st0_y);
  trackSeed->SetBranchAddress("truth_st0_z", &truth_st0_z);
  trackSeed->SetBranchAddress("truth_st1_x", &truth_st1_x);
  trackSeed->SetBranchAddress("truth_st1_y", &truth_st1_y);
  trackSeed->SetBranchAddress("truth_st1_z", &truth_st1_z);
  trackSeed->SetBranchAddress("truth_st2_x", &truth_st2_x);
  trackSeed->SetBranchAddress("truth_st2_y", &truth_st2_y);
  trackSeed->SetBranchAddress("truth_st2_z", &truth_st2_z);
  trackSeed->SetBranchAddress("truth_st3_x", &truth_st3_x);
  trackSeed->SetBranchAddress("truth_st3_y", &truth_st3_y);
  trackSeed->SetBranchAddress("truth_st3_z", &truth_st3_z);


  struct Measurement{
    double volume;
    double layer;
    double sensor;
    double index;
    double particleP;
    TEllipse* hit; 
  };

  struct TrackHit{
    int index;
    TEllipse* hit;
  };


  std::map<int, std::vector<TEllipse*>> truthPos; //particle postion on different stations
  std::map<int, std::vector<Measurement>> hits; //input measurements
  std::map<int, std::vector<TrackHit>> tracks; //measurements on tracks

  double radius = 10;
 
  bool foundEvent = false; 
  for(int i=0; i<trackSummary->GetEntries(); ++i){
    trackSummary->GetEntry(i);
    if(foundEvent) break;
    if(event_nr_1!=checkEvent) continue; 
    foundEvent = true; 

    for(int ih =0; ih < inputMeasurementLocX->size(); ++ih){
      hits[inputMeasurementParticleId->at(ih)].push_back({inputMeasurementVolume->at(ih), inputMeasurementLayer->at(ih), inputMeasurementSensitive->at(ih), inputMeasurementIndex->at(ih), inputMeasurementParticleP->at(ih), new TEllipse(inputMeasurementGloZ->at(ih), hypot(inputMeasurementGloX->at(ih),inputMeasurementGloY->at(ih)), radius, radius*0.2)}); 
    }
  }



  int nTracks=0;
  bool thisEvent = false;
  for(int i=0; i<trackState->GetEntries(); ++i){
    trackState->GetEntry(i);
    if(event_nr_2==checkEvent) {
      thisEvent = true; 

      for(int ih =0; ih < g_x_hit->size(); ++ih){
        tracks[nTracks].push_back({index_hit->at(ih), new TEllipse(g_z_hit->at(ih), hypot(g_x_hit->at(ih), g_y_hit->at(ih)), radius, radius*0.2)});   
      }
      nTracks ++;
    }
 
    if(event_nr_2!=checkEvent and thisEvent){
      break;
    }
  }
  std::cout<<"There are "<< tracks.size()  <<" tracks" << std::endl;




  for(int i=0; i<trackSeed->GetEntries(); ++i){
    trackSeed->GetEntry(i); 
    if(event_nr_3==checkEvent) {
      std::array<std::array<double,2>,4> positions;
      for(int ip=0; ip < truth_st0_x->size(); ++ip){
        positions[0][0] = std::hypot(truth_st0_x->at(ip), truth_st0_y->at(ip));
        positions[0][1] = truth_st0_z->at(ip);
        positions[1][0] = std::hypot(truth_st1_x->at(ip), truth_st1_y->at(ip));
        positions[1][1] = truth_st1_z->at(ip);
        positions[2][0] = std::hypot(truth_st2_x->at(ip), truth_st2_y->at(ip));
        positions[2][1] = truth_st2_z->at(ip);
        positions[3][0] = std::hypot(truth_st3_x->at(ip), truth_st3_y->at(ip));
        positions[3][1] = truth_st3_z->at(ip);
	for(int i=0;i < 4; ++i){
          truthPos[truth_barcode->at(ip)].push_back(new TEllipse(positions[i][1], positions[i][0], radius, radius*0.2));        
        } 
        std::cout<<"particleId = " << truth_barcode->at(ip) <<" with pdg " << truth_pdg->at(ip) << std::endl; 
        if(truth_pdg->at(ip)==11) particleNames[truth_barcode->at(ip)]="e^{-}"; 
        if(truth_pdg->at(ip)==-11) particleNames[truth_barcode->at(ip)]="e^{+}"; 
        if(truth_pdg->at(ip)==13) particleNames[truth_barcode->at(ip)]="#mu^{-}"; 
        if(truth_pdg->at(ip)==-13) particleNames[truth_barcode->at(ip)]="#mu^{+}"; 
      } 
      break;
    }
  }

  TLegend *legend = new TLegend(0.5, 0.7, 0.9, 0.9);
  legend->SetLineStyle(0);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);



  //Negative charge (barcode == 2?) will be curved down
  TCanvas * c0 = new TCanvas("c0", "", 2000, 400);
  TH1F* h = new TH1F("h", "", 100, -2000, 2500);
  //h->GetXaxis()->SetRangeUser(-1000,1000);
  h->GetYaxis()->SetRangeUser(0,150);
  h->GetXaxis()->SetTitle("z [mm]");
  h->GetYaxis()->SetTitle("r [mm]");
  
  h->Draw("same");

  std::vector<int> colors = {814, 832, 896, 866, 796, 896, 862, 796, 832, 814, 852, 896};

  //draw truth tracks
  for(const auto& [particleId, poss] : truthPos){  
    for(const auto& pos : poss){
      if(particleId<colors.size()){ 
        pos->SetLineColor(colors[particleId]);
        pos->SetFillStyle(0); 
      } 
      //particleHit->SetLineColor(1); 
      pos->SetLineWidth(3); 
      pos->SetLineStyle(kDashed); 
      if(particleId==2 or particleId==3) pos->Draw("same");
    }
    //if(particleNames.find(particleId)!=particleNames.end()){
    //  legend->AddEntry(poss.front(), Form("Truth %s position", particleNames[particleId].c_str()), "P");
    //}
  }


  // draw measurements 
  for(const auto& [particleId, meass] : hits){
    std::cout<<"particle " << particleId << " index -- p -- volume/layer/module/ =  " << std::endl;
    for(const auto& meas : meass){
      if(particleId<colors.size()){ 
        meas.hit->SetLineColor(colors[particleId]);
      } 
      meas.hit->SetLineWidth(3); 
      meas.hit->SetFillStyle(0); 
      if(particleId==2 or particleId==3) meas.hit->Draw("same");

      std::cout<< meas.index << "--" << meas.particleP<<"--" << meas.volume<< "/" << meas.layer<<"/" << meas.sensor <<"  "; 
      //std::cout<< meas.index << "/" << meas.particleP<< "  "; 
    }
    //if(particleNames.find(particleId)!=particleNames.end()){
    //  legend->AddEntry(meass.front().hit, Form("measurements from %s", particleNames[particleId].c_str()), "P");
    //}
    std::cout<<std::endl;
    std::cout<<std::endl;
  }


  auto findParticleId = [&](int index){
    bool find = false;
    int particle_Id = -1;  
    for(const auto& [particleId, meass] : hits){
        for(const auto& meas: meass){
           if(meas.index == index) {
             find = true;
	     particle_Id = particleId;
	     break; 
	   }
	}
	if(find) break;
     }
     return particle_Id;
  };

  
  // draw reco tracks 
  for( auto& [iTrack, track] : tracks){
    std::cout<<"Track nMeasurements =  " << track.size() <<" with indices " << std::endl;
    for(const auto& trackHit : track){
      trackHit.hit->SetLineWidth(3);
     
      int particle_id = findParticleId(trackHit.index);
      int color = (particle_id>=colors.size()? 1 : colors[particle_id]); 
      trackHit.hit->SetFillColor(color);
      trackHit.hit->SetLineColor(color);
      trackHit.hit->Draw("same");
      
      std::cout<<trackHit.index <<" ";
    }

    std::cout<<std::endl;
    std::cout<<std::endl;

    for(int i=0; i< track.size()-1; ++i){
      TLine* line = new TLine(track[i].hit->GetX1(), track[i].hit->GetY1(), track[i+1].hit->GetX1(), track[i+1].hit->GetY1());
      line->SetLineColor(1); 
      line->SetLineStyle(kDashed); 
      line->Draw("same"); 
    }

  }


  myText(0.35, 0.75, 1, 0.04, Form("sample %s, event %d", sample.c_str(), checkEvent));
  myText(0.35, 0.65, 1, 0.04, Form("%s", trackingMode.c_str()));
  legend->Draw();
}


void myText(Double_t x,Double_t y,Color_t color, float font, const char *text) {

  Double_t tsize=0.05;
  TLatex l; //
  l.SetTextAlign(12);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextSize(font);
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}


