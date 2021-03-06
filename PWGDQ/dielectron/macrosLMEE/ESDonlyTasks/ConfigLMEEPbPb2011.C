void InitHistograms(AliDielectron *die, Int_t cutDefinition);
void InitCF(AliDielectron* die, Int_t cutDefinition);
void EnableMC();

TString names=("noPairing;TPCTOFCentnoRej;TPCTOFSemiCentnoRej;TPCTOFPerinoRej;TPCTOFCent;TPCTOFSemiCent;TPCTOFPeri;TPCTOFCentnoRejTight;TPCTOFCentTight;TPCTOFCentPhiV;TPCTOFSemiCentPhiV;TPCTOFPeriPhiV;TPCTOFCentOA;TPCTOFSemiCentOA;TPCTOFPeriOA");
TObjArray *arrNames=names.Tokenize(";");
const Int_t nDie=arrNames->GetEntries();

Bool_t MCenabled=kFALSE;


AliDielectron* ConfigLMEEPbPb2011(Int_t cutDefinition, Bool_t withMC=kFALSE, Bool_t CFenable=kFALSE)
{

  Int_t selectedPID=-1;
  Int_t selectedCentrality=-1;
  Bool_t rejectionStep=kFALSE;
  LMEECutLib*  LMCL = new LMEECutLib();

  //
  // Setup the instance of AliDielectron
  //

  MCenabled=withMC;
  // create the actual framework object

  TString name=Form("%02d",cutDefinition);
  if ((cutDefinition)<arrNames->GetEntriesFast()){
	name=arrNames->At((cutDefinition))->GetName();
  }

  //thisCut only relevant for MC:
  AliDielectron *die =
	new AliDielectron(Form
		("%s",name.Data()),
		Form("Track cuts: %s",name.Data()));


  if (MCenabled)
	  die->SetHasMC(kTRUE);


  //Setup AnalysisSelection:
  if (cutDefinition==0) {
	//not yet implemented
  }
  else if (cutDefinition==1) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Central;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==2) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011SemiCentral;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==3) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Peripheral;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==4) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Central;
	rejectionStep = kTRUE;
  }
  else if (cutDefinition==5) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011SemiCentral;
	rejectionStep = kTRUE;
  }
  else if (cutDefinition==6) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Peripheral;
	rejectionStep = kTRUE;
  }

//Legacy cuts, check consistence w/ 1 & 4, then remove
  else if (cutDefinition==7) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOF;
	selectedCentrality = LMEECutLib::kPbPb2011Central;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==8) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOF;
	selectedCentrality = LMEECutLib::kPbPb2011Central;
	rejectionStep = kTRUE;
  }

  else if (cutDefinition==9) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Central;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==10) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011SemiCentral;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==11) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Peripheral;
	rejectionStep = kFALSE;
  }
//OA:
  else if (cutDefinition==12) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Central;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==13) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011SemiCentral;
	rejectionStep = kFALSE;
  }
  else if (cutDefinition==14) {
	selectedPID = LMEECutLib::kPbPb2011TPCandTOFwide;
	selectedCentrality = LMEECutLib::kPbPb2011Peripheral;
	rejectionStep = kFALSE;
  }

  else Semi{
	cout << " =============================== " << endl;
	cout << " ==== INVALID CONFIGURATION ==== " << endl;
	cout << " =============================== " << endl;
  }


  //Now configure task

  //Apply correct Pre-Filter Scheme, if necessary
  die->SetPreFilterAllSigns();

	if (rejectionStep) {
		//for Pb-Pb, only use tight PID (combinatorics) for rejection
		die->GetTrackFilter().AddCuts( LMCL->GetPIDCutsAna(selectedPID) );
		//die->GetTrackFilter().AddCuts(LMCL->GetPIDCutsPre(selectedPID) );
		die->GetPairPreFilterLegs().AddCuts(LMCL->GetPIDCutsAna(selectedPID) );
		die->GetPairPreFilter().AddCuts(LMCL->GetPairCuts(selectedPID) );
	}
	else { //No Prefilter, no Pairfilter
		die->GetTrackFilter().AddCuts( LMCL->GetPIDCutsAna(selectedPID) );
		if ((cutDefinition >=9) &&  (cutDefinition <=11)) {
		die->GetPairFilter().AddCuts(LMCL->GetPairCuts2(selectedPID,kFALSE));
		}
		if ((cutDefinition >=12) &&  (cutDefinition <=14)) {
		die->GetPairFilter().AddCuts(LMCL->GetPairCuts2(selectedPID,kTRUE));
		}
	}
	//Introduce NULL-check for pp?
	die->GetEventFilter().AddCuts(LMCL->GetCentralityCuts(selectedCentrality));




  AliDielectronTrackRotator *rot= 0x0;
  /*AliDielectronTrackRotator *rot= LMCL->GetTrackRotator(selectedPID);
  die->SetTrackRotator(rot);
   */
  AliDielectronMixingHandler *mix=LMCL->GetMixingHandler(selectedPID);
  die->SetMixingHandler(mix);

  // histogram setup
  // only if an AliDielectronHistos object is attached to the
  // dielectron framework histograms will be filled
  //
  InitHistograms(die,cutDefinition);

  // the last definition uses no cuts and only the QA histograms should be filled!
  if (CFenable) {
	  InitCF(die,cutDefinition);
  }
  return die;
}

//______________________________________________________________________________________

void InitHistograms(AliDielectron *die, Int_t cutDefinition)
{
  //
  // Initialise the histograms
  //

  //Setup histogram Manager
  AliDielectronHistos *histos=
	new AliDielectronHistos(die->GetName(),
		die->GetTitle());
  //Initialise histogram classes
  histos->SetReservedWords("Track;Pair;Pre;RejTrack;RejPair");

  //Event class
//  if (cutDefinition==nDie-1) 
  histos->AddClass("Event");

  //Track classes
  //to fill also track info from 2nd event loop until 2
  for (Int_t i=0; i<2; ++i){
	histos->AddClass(Form("Track_%s",AliDielectron::TrackClassName(i)));
  }

  //Pair classes
  // to fill also mixed event histograms loop until 10
  for (Int_t i=0; i<3; ++i){
	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(i)));
  }

  //ME and track rot
  if (die->GetMixingHandler()) {
	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(3)));
	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(4)));
	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(6)));
	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(7)));
  }
  if (die->GetTrackRotator()) {
	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(10)));
  }

  //PreFilter Classes
  //to fill also track info from 2nd event loop until 2
  for (Int_t i=0; i<2; ++i){
	histos->AddClass(Form("Pre_%s",AliDielectron::TrackClassName(i)));
  }


  //Create Classes for Rejected Tracks/Pairs:
  for (Int_t i=0; i<2; ++i){
	histos->AddClass(Form("RejTrack_%s",AliDielectron::TrackClassName(i)));
  }
  for (Int_t i=0; i<3; ++i){
	histos->AddClass(Form("RejPair_%s",AliDielectron::PairClassName(i)));
  }

  /*
  //track rotation

  histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(AliDielectron::kEv1PMRot)));
  histos->AddClass(Form("Track_Legs_%s",AliDielectron::PairClassName(AliDielectron::kEv1PMRot)));
  */
	//add histograms to event class
	histos->UserHistogram("Event","nEvents","Number of processed events after cuts;Number events",
		1,0.,1.,AliDielectronVarManager::kNevents);
	histos->UserHistogram("Event","Centrality","Centrality;Centrality [%]","0,10,20,40,80,100,101",
		AliDielectronVarManager::kCentrality);


  //add histograms to Track classes, also fills RejTrack
  histos->UserHistogram("Track","Pt","Pt;Pt [GeV];#tracks",200,0,20.,AliDielectronVarManager::kPt);
  histos->UserHistogram("Track","NclsSFracTPC","NclsSFracTPC; NclsSFracTPC;#tracks",200,0,10.,AliDielectronVarManager::kNclsSFracTPC);
  histos->UserHistogram("Track","TPCclsDiff","TPCclsDiff; TPCclsDiff;#tracks",200,0,10.,AliDielectronVarManager::kTPCclsDiff);

  histos->UserHistogram("Track","ITS_dEdx_P","ITS_dEdx;P [GeV];ITS signal (arb units);#tracks",
	  400,0.0,20.,1000,0.,1000.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kITSsignal,kTRUE);

  histos->UserHistogram("Track","dEdx_P","dEdx;P [GeV];TPC signal (arb units);#tracks",
	  400,0.0,20.,200,0.,200.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Track","TPCnSigmaEle_P","TPC number of sigmas Electrons;P [GeV];TPC number of sigmas Electrons;#tracks",
	  400,0.0,20.,100,-5.,5.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCnSigmaEle,kTRUE);
  histos->UserHistogram("Track","TPCnSigmaKao_P","TPC number of sigmas Kaons;P [GeV];TPC number of sigmas Kaons;#tracks",
	  400,0.0,20.,100,-5.,5.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCnSigmaKao,kTRUE);
  histos->UserHistogram("Track","TPCnSigmaPio_P","TPC number of sigmas Pions;P [GeV];TPC number of sigmas Pions;#tracks",
	  400,0.0,20.,100,-5.,5.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCnSigmaPio,kTRUE);

  histos->UserHistogram("Track","TRDpidPobEle_P","TRD PID probability Electrons;P [GeV];TRD prob Electrons;#tracks",
	  400,0.0,20.,100,0.,1.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTRDprobEle,kTRUE);
  histos->UserHistogram("Track","TRDpidPobPio_P","TRD PID probability Pions;P [GeV];TRD prob Pions;#tracks",
	  400,0.0,20.,100,0.,1.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTRDprobPio,kTRUE);

  histos->UserHistogram("Track","TOFnSigmaKao_P","TOF number of sigmas Kaons;P [GeV];TOF number of sigmas Kaons;#tracks",
	  400,0.0,20.,100,-5.,5.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTOFnSigmaKao,kTRUE);
  histos->UserHistogram("Track","TOFnSigmaPro_P","TOF number of sigmas Protons;P [GeV];TOF number of sigmas Protons;#tracks",
	  400,0.0,20.,100,-5.,5.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTOFnSigmaPro,kTRUE);

  histos->UserHistogram("Track","Eta_Phi","Eta Phi Map; Eta; Phi;#tracks",
	  200,-2,2,200,0,3.15,AliDielectronVarManager::kEta,AliDielectronVarManager::kPhi);

  histos->UserHistogram("Track","dXY","dXY;dXY [cm];#tracks",200,-2.,2.,AliDielectronVarManager::kImpactParXY);

  histos->UserHistogram("Track","TPCnCls","Number of Clusters TPC;TPC number clusteres;#tracks",159,0.,159.,AliDielectronVarManager::kNclsTPC);

  histos->UserHistogram("Track","TPCnCls_kNFclsTPCr","nTPC vs nTPCr;nTPC vs nTPCr;#tracks",159,0.,159.,159,0.,159.,AliDielectronVarManager::kNclsTPC,AliDielectronVarManager::kNFclsTPCr);

  histos->UserHistogram("Track","kNFclsTPCr_pT","nTPCr vs pt;nTPCr vs pt;#tracks",159,0.,159.,200,0.,20.,AliDielectronVarManager::kNFclsTPCr,AliDielectronVarManager::kPt);

  //add histograms to Pair classes, also fills RejPair
  histos->UserHistogram("Pair","InvMass","Inv.Mass;Inv. Mass [GeV];#pairs",
	  500,0.0,5.00,AliDielectronVarManager::kM);
  histos->UserHistogram("Pair","Rapidity","Rapidity;Rapidity;#pairs",
	  100,-2.,2.,AliDielectronVarManager::kY);
  histos->UserHistogram("Pair","DeltaEta","DeltaEta;DeltaEta",
	  100,0.,3.15,AliDielectronVarManager::kDeltaEta);
  histos->UserHistogram("Pair","DeltaPhi","DeltaPhi;DeltaPhi",
	  100,0.,3.15,AliDielectronVarManager::kDeltaPhi);
  histos->UserHistogram("Pair","PhivPair","PhivPair;angle",
	  100,0.,3.15,AliDielectronVarManager::kPhivPair);
  histos->UserHistogram("Pair","PsiPair","PsiPair;angle",
	  100,0.,3.15,AliDielectronVarManager::kPsiPair);
  histos->UserHistogram("Pair","OpeningAngle","Opening angle;angle",
	  100,0.,3.15,AliDielectronVarManager::kOpeningAngle);
  //2D Histo Plot
  histos->UserHistogram("Pair","InvMassPairPt","Inv.Mass vs PairPt;Inv. Mass [GeV], pT [GeV];#pairs",
	  500,0.0,5.0,500,0.,50.,AliDielectronVarManager::kM,AliDielectronVarManager::kPt);


  histos->UserHistogram("Pair","MPhivPair","PhivPair vs M;M ;PhiV",
	  500,0.0,5.0,200,0.,6.3,AliDielectronVarManager::kM,AliDielectronVarManager::kPhivPair);

  histos->UserHistogram("Pair","PsiPairPhivPair","PhivPair vs PsiPair;PsiPair ;#pairs",
	  200,0.0,6.3,200,0.,6.3,AliDielectronVarManager::kPsiPair,AliDielectronVarManager::kPhivPair);
 
  histos->UserHistogram("Pair","PhivPairOpeningAngle","Opening Angle vs PhivPair;PhivPair [GeV];#pairs",
	  200,0.0,6.3,200,0.,6.3,AliDielectronVarManager::kPhivPair,AliDielectronVarManager::kOpeningAngle);

  histos->UserHistogram("Pair","PsiPairOpeningAngle","Opening Angle vs PsiPair;PsiPair [GeV];#pairs",
	  200,0.0,6.3,200,0.,6.3,AliDielectronVarManager::kPsiPair,AliDielectronVarManager::kOpeningAngle);
 
  histos->UserHistogram("Pair","InvMassOpeningAngle","Opening Angle vs Inv.Mass;Inv. Mass [GeV];#pairs",
	  500,0.0,5.0,200,0.,6.3,AliDielectronVarManager::kM,AliDielectronVarManager::kOpeningAngle);

  //add histograms to Track classes
  histos->UserHistogram("Pre","Pt","Pt;Pt [GeV];#tracks",200,0,20.,AliDielectronVarManager::kPt);

  histos->UserHistogram("Pre","ITS_dEdx_P","ITS_dEdx;P [GeV];ITS signal (arb units);#tracks",
	  400,0.0,20.,1000,0.,1000.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kITSsignal,kTRUE);

  histos->UserHistogram("Pre","dEdx_P","dEdx;P [GeV];TPC signal (arb units);#tracks",
	  400,0.0,20.,200,0.,200.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Pre","Eta_Phi","Eta Phi Map; Eta; Phi;#tracks",
	  200,-2,2,200,0,3.15,AliDielectronVarManager::kEta,AliDielectronVarManager::kPhi);

  histos->UserHistogram("Pre","dXY","dXY;dXY [cm];#tracks",200,-2.,2.,AliDielectronVarManager::kImpactParXY);

  histos->UserHistogram("Pre","ZVertex ","ZVertex ;ZVertex[cm];#tracks",20,-20,20,AliDielectronVarManager::kZv);
  histos->UserHistogram("Pre","XVertex ","XVertex ;XVertex[cm];#tracks",20,-20,20,AliDielectronVarManager::kXv);
  histos->UserHistogram("Pre","YVertex ","YVertex ;YVertex[cm];#tracks",20,-20,20,AliDielectronVarManager::kYv);

//  histos->UserHistogram("Pre","TPCnCls","Number of Clusters TPC;TPC number clusteres;#tracks",159,0.,159.,AliDielectronVarManager::kNclsTPC);


  die->SetHistogramManager(histos);
}


void InitCF(AliDielectron* die, Int_t cutDefinition)
{
  //
  // Setupd the CF Manager if needed
  //
  AliDielectronCF *cf=new AliDielectronCF(die->GetName(),die->GetTitle());

  //pair variables
  cf->AddVariable(AliDielectronVarManager::kP,200,0,20);
  cf->AddVariable(AliDielectronVarManager::kPt,200,0,20);
  cf->AddVariable(AliDielectronVarManager::kM,201,-0.01,4.01); //20Mev Steps
  cf->AddVariable(AliDielectronVarManager::kY,100,-2.,2.);
  cf->AddVariable(AliDielectronVarManager::kDeltaEta,200,-2.,2.);
  cf->AddVariable(AliDielectronVarManager::kDeltaPhi,100,0.,3.2);
  cf->AddVariable(AliDielectronVarManager::kPairType,10,0,10);

  cf->AddVariable(AliDielectronVarManager::kCentrality,"0.,10.0,30.0,40.0,60.,80.,100.");
  cf->AddVariable(AliDielectronVarManager::kOpeningAngle,320,0.,3.2);
  cf->AddVariable(AliDielectronVarManager::kPsiPair,320,0.,3.2);
  //leg variables
  cf->AddVariable(AliDielectronVarManager::kP,200,0.,20.,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kPt,200,0.,20.,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kITSsignal,1000,0.0.,1000.,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kTPCsignal,500,0.0.,500.,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kY,100,-2.,2.,kTRUE);
  //only in this case write MC truth info
  if (MCenabled) {
	cf->SetStepForMCtruth();
	cf->SetStepsForMCtruthOnly();
	cf->AddVariable(AliDielectronVarManager::kHaveSameMother,5,-2,2);
	cf->AddVariable(AliDielectronVarManager::kPdgCode,10000,-5000.5,4999.5,kTRUE);
	cf->AddVariable(AliDielectronVarManager::kPdgCodeMother,10000,-5000.5,4999.5,kTRUE);
  }

//Added after train commit
  cf->SetStepsForEachCut();
  cf->SetStepForAfterAllCuts();


  cf->SetStepsForSignal();
  die->SetCFManagerPair(cf);

/*
 AliDielectronSignalMC* lowMassDiele=new
    AliDielectronSignalMC("lowMassDiele","low mass dielectron pairs");
  lowMassDiele->SetLegPDGs(11,-11);
  lowMassDiele->SetCheckBothChargesLegs(kTRUE,kTRUE);
  lowMassDiele->SetLegSources(AliDielectronSignalMC::kPrimary,
      AliDielectronSignalMC::kPrimary);
  lowMassDiele->SetFillPureMCStep(kTRUE);
  die->AddSignalMC(lowMassDiele);

  AliDielectronSignalMC* secondary=new
    AliDielectronSignalMC("secondary","secondary electrons pairs");
  secondary->SetLegPDGs(11,-11);
  secondary->SetCheckBothChargesLegs(kTRUE,kTRUE);
  secondary->SetLegSources(AliDielectronSignalMC::kSecondary,
      AliDielectronSignalMC::kSecondary);
  die->AddSignalMC(secondary);


  AliDielectronSignalMC* finalState=new
    AliDielectronSignalMC("finalState","finalState electrons pairs");
  finalState->SetLegPDGs(11,-11);
  finalState->SetCheckBothChargesLegs(kTRUE,kTRUE);
  finalState->SetLegSources(AliDielectronSignalMC::kFinalState,
      AliDielectronSignalMC::kFinalState);
  die->AddSignalMC(finalState);
*/

  AliDielectronSignalMC* eleFromConversions=new
	AliDielectronSignalMC("eleFromConversions","conversion electrons");
  eleFromConversions->SetLegPDGs(11,-11);
  eleFromConversions->SetCheckBothChargesLegs(kTRUE,kTRUE);
  eleFromConversions->SetLegSources(AliDielectronSignalMC::kSecondary,
	  AliDielectronSignalMC::kSecondary);
  eleFromConversions->SetMotherPDGs(22,22);    // 22- photon
  die->AddSignalMC(eleFromConversions);


  AliDielectronSignalMC* dalitzDecays=new
	AliDielectronSignalMC("dalitzDecays","dalitz Pairs");
  dalitzDecays->SetLegPDGs(11,-11);
  dalitzDecays->SetCheckBothChargesLegs(kTRUE,kTRUE);
  dalitzDecays->SetLegSources(AliDielectronSignalMC::kSecondary,
	  AliDielectronSignalMC::kSecondary);
  dalitzDecays->SetMotherPDGs(111,111);
  dalitzDecays->SetFillPureMCStep(kTRUE);
  die->AddSignalMC(dalitzDecays);


  AliDielectronSignalMC* PhiDecays=new
        AliDielectronSignalMC("PhiDecays","Phi Pairs");
  PhiDecays->SetLegPDGs(11,-11);
  PhiDecays->SetCheckBothChargesLegs(kTRUE,kTRUE);
  PhiDecays->SetLegSources(AliDielectronSignalMC::kFinalState,
                AliDielectronSignalMC::kFinalState);
  PhiDecays->SetMotherPDGs(333,333);
  PhiDecays->SetMothersRelation(AliDielectronSignalMC::kSame);
  PhiDecays->SetFillPureMCStep(kTRUE);
  die->AddSignalMC(PhiDecays);

  AliDielectronSignalMC* OmegaDecays=new
        AliDielectronSignalMC("OmegaDecays","Omega Pairs");
  OmegaDecays->SetLegPDGs(11,-11);
  OmegaDecays->SetCheckBothChargesLegs(kTRUE,kTRUE);
  OmegaDecays->SetLegSources(AliDielectronSignalMC::kFinalState,
                AliDielectronSignalMC::kFinalState);
  OmegaDecays->SetMotherPDGs(223,223);
  OmegaDecays->SetMothersRelation(AliDielectronSignalMC::kSame);
  OmegaDecays->SetFillPureMCStep(kTRUE);
  die->AddSignalMC(OmegaDecays);

							 

}

//--------------------------------------
void EnableMC() {
  MCenabled=kTRUE;
}

