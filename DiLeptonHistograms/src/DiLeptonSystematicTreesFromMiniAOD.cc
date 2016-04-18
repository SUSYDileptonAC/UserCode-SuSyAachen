// -*- C++ -*-
//
// Package:    Histograms
// Class:      DiLeptonSystematicTreesFromMiniAOD
// 
/**\class DiLeptonSystematicTreesFromMiniAOD DiLeptonSystematicTreesFromMiniAOD.cc brot/DiLeptonSystematicTreesFromMiniAOD/src/DiLeptonSystematicTreesFromMiniAOD.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  matthias edelhoff
//         Created:  Tue Oct 27 13:50:40 CET 2009
// $Id: DiLeptonSystematicTreesFromMiniAOD.cc,v 1.31 2012/09/17 17:38:58 sprenger Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <DataFormats/Candidate/interface/Candidate.h>
#include <DataFormats/PatCandidates/interface/Lepton.h>

#include <DataFormats/PatCandidates/interface/Electron.h>
#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/Tau.h>
#include <DataFormats/PatCandidates/interface/Jet.h>
#include <DataFormats/PatCandidates/interface/MET.h>

#include <DataFormats/Provenance/interface/EventID.h>

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

// For JES
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"


#include <SuSyAachen/DiLeptonHistograms/interface/WeightFunctor.h>
#include <SuSyAachen/DiLeptonHistograms/interface/PdgIdFunctor.h>
#include <SuSyAachen/DiLeptonHistograms/interface/VertexWeightFunctor.h>
#include <SuSyAachen/TagAndProbeTreeWriter/interface/IsolationFunctor.h>
#include <SuSyAachen/DiLeptonHistograms/interface/TriggerMatchFunctorMiniAOD.h>
#include <SuSyAachen/DiLeptonHistograms/interface/BTagCalibrationStandalone.h>
#include <SuSyAachen/DiLeptonHistograms/interface/BTagEffMapFunctor.h>
#include <SuSyAachen/DiLeptonHistograms/interface/LeptonFullSimScaleFactorMapFunctor.h>
#include <SuSyAachen/DiLeptonHistograms/interface/LeptonFastSimScaleFactorMapFunctor.h>


//ROOT
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

using namespace std;

//
// class decleration
//

class DiLeptonSystematicTreesFromMiniAOD : public edm::EDAnalyzer {
public:
  explicit DiLeptonSystematicTreesFromMiniAOD(const edm::ParameterSet&);
  ~DiLeptonSystematicTreesFromMiniAOD();

private:
  //  typedef reco::Candidate candidate;
  typedef pat::Lepton<reco::Candidate> candidate;
  typedef edm::View<candidate> collection;

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  void initFloatBranch( const std::string &name);
  void initIntBranch( const std::string &name);
  void initTLorentzVectorBranch( const std::string &name);
  template <class aT, class bT> void makeCombinations( const std::string &treeName, const std::vector<aT> &a, const std::vector<bT >&b, const std::vector<pat::PackedCandidate>&pfCands, const edm::Event &ev, const pat::MET &patMet, const pat::MET &patMetPuppi, const TLorentzVector &MHT, const TLorentzVector &MHTLoose, const TLorentzVector &MHTPuppi, const TLorentzVector &MHTLoosePuppi, const edm::Handle<reco::VertexCollection> &vertices, const float &rho, const std::map<std::string, int> &intEventProperties, const  std::map<std::string, float> &floatEventProperties, const  std::map<std::string, TLorentzVector> &tLorentzVectorEventProperties);
  template <class aT> void makeCombinations( const std::string &treeName, const std::vector<aT> &a , const std::vector<pat::PackedCandidate>&pfCands,   const edm::Event &ev, const pat::MET &patMet, const pat::MET &patMetPuppi, const TLorentzVector &MHT, const TLorentzVector &MHTLoose,const TLorentzVector &MHTPuppi, const TLorentzVector &MHTLoosePuppi, const edm::Handle<reco::VertexCollection> &vertices, const float &rho, const std::map<std::string, int> &intEventProperties, const  std::map<std::string, float> &floatEventProperties, const  std::map<std::string, TLorentzVector> &tLorentzVectorEventProperties);
  template<class aT, class bT> void fillTree( const std::string &treeName, const aT &a, const bT &b, const std::vector<pat::PackedCandidate>&pfCands,  const pat::MET &patMet, const pat::MET &patMetPuppi, const TLorentzVector &MHT, const TLorentzVector &MHTLoose, const TLorentzVector &MHTPuppi, const TLorentzVector &MHTLoosePuppi, const edm::Handle<reco::VertexCollection> &vertices, const float &rho);
  //~ std::pair<double, double> calcPZeta(const TLorentzVector& p1,const TLorentzVector& p2, const TLorentzVector& met);
  void fillPdfUncert(const edm::Handle< std::vector<double> >& weightHandle, const std::string& pdfIdentifier, const std::string& treeName);

  const TLorentzVector getMomentum(const  pat::Electron &e);
  const TLorentzVector getMomentum(const  pat::Muon &mu);
  const TLorentzVector getMomentum(const  pat::Tau &tau);
  void fillLeptonIDs(const std::string &treeName, const  pat::Electron &ele1, const  pat::Electron &ele2, const edm::Handle<reco::VertexCollection> &vertices);
  void fillLeptonIDs(const std::string &treeName, const  pat::Muon &mu1, const  pat::Muon &mu2, const edm::Handle<reco::VertexCollection> &vertices);
  void fillLeptonIDs(const std::string &treeName, const  pat::Electron &ele1, const  pat::Muon &mu2, const edm::Handle<reco::VertexCollection> &vertices);
  void fillLeptonIDs(const std::string &treeName, const  pat::Electron &ele1, const  pat::Tau &tau2, const edm::Handle<reco::VertexCollection> &vertices);
  void fillLeptonIDs(const std::string &treeName, const  pat::Muon &mu1, const  pat::Tau &tau2, const edm::Handle<reco::VertexCollection> &vertices);
  void fillLeptonIDs(const std::string &treeName, const  pat::Tau &tau1, const  pat::Tau &tau2, const edm::Handle<reco::VertexCollection> &vertices);
  float topPtWeightBen(double topPt);
  float topPtWeightTOP(double topPt);
  float getIso(const  pat::Electron &e, const std::string &method);
  float getIso(const  pat::Muon &mu, const std::string &method);
  float getIso(const  pat::Tau &tau, const std::string &method);
  float getDeltaB(const  pat::Electron &e);
  float getDeltaB(const  pat::Muon &mu);
  float getDeltaB(const  pat::Tau &tau);
  float transverseMass(const TLorentzVector& p, const TLorentzVector& met);
  float getAEffEle(double eta);
  float getAEffMu(double eta);
  std::string convertInputTag(const edm::InputTag tag);

  edm::InputTag eTag_;
  edm::InputTag muTag_;
  edm::InputTag tauTag_;
  edm::InputTag jetTag_;
  edm::InputTag jet2Tag_;
  edm::InputTag jetTagPuppi_;
  edm::InputTag bJetTag_;
  edm::InputTag bJetTagPuppi_;
  edm::InputTag metTag_;
  edm::InputTag metTagPuppi_;
  edm::InputTag metNoHFTag_;  
  edm::InputTag vertexTag_;
  edm::InputTag vertexTagUp_;
  edm::InputTag vertexTagDown_;
  edm::InputTag pfCandTag_;
  edm::InputTag genParticleTag_;
  edm::InputTag genEventInfoTag_;
  edm::InputTag LHEEventTag_;  
  edm::InputTag rhoTag_;
  std::vector<edm::ParameterSet> susyVars_;
  std::vector<edm::InputTag> pdfs_;
  std::string tauId_;


  std::map<double, double> electronCorrections_;
  //data
  std::map<std::string, TTree*> trees_;  
  std::map<std::string, std::map< std::string, float*> > floatBranches_; 
  std::map<std::string, std::map< std::string, unsigned int*> > intBranches_; 
  std::map<std::string, std::map< std::string, TLorentzVector*> > tLorentzVectorBranches_;

  edm::Handle< std::vector< pat::Jet > > jets_;
  
  edm::Handle< std::vector< pat::Jet > > jetsPuppi_;
  
  BTagEffMapFunctor fctBTagEff_;
  
  BTagCalibration fctBTagCalibFullSim_;
  BTagCalibrationReader fctBTagCalibReaderFullSimBJets_;
  BTagCalibrationReader fctBTagCalibReaderFullSimCJets_;
  BTagCalibrationReader fctBTagCalibReaderFullSimLightJets_;
  BTagCalibrationReader fctBTagCalibReaderFullSimBJetsUp_;
  BTagCalibrationReader fctBTagCalibReaderFullSimCJetsUp_;
  BTagCalibrationReader fctBTagCalibReaderFullSimLightJetsUp_;
  BTagCalibrationReader fctBTagCalibReaderFullSimBJetsDown_;
  BTagCalibrationReader fctBTagCalibReaderFullSimCJetsDown_;
  BTagCalibrationReader fctBTagCalibReaderFullSimLightJetsDown_;
  
  BTagCalibration fctBTagCalibFastSim_;
  BTagCalibrationReader fctBTagCalibReaderFastSim_;
  BTagCalibrationReader fctBTagCalibReaderFastSimUp_;
  BTagCalibrationReader fctBTagCalibReaderFastSimDown_;
  
  LeptonFullSimScaleFactorMapFunctor fctLeptonFullSimScaleFactors_;
  LeptonFastSimScaleFactorMapFunctor fctLeptonFastSimScaleFactors_;

  WeightFunctor fakeRates_;
  WeightFunctor efficiencies_;
  VertexWeightFunctor fctVtxWeight_;
  VertexWeightFunctor fctVtxWeightUp_;
  VertexWeightFunctor fctVtxWeightDown_;
  IsolationFunctor fctIsolation_;
  TriggerMatchFunctorMiniAOD fctTrigger_;  
  PdgIdFunctor getPdgId_;
 

  bool debug;
  bool useJets2_;
  bool useTaus_;
  bool writeID_;
  bool metUncert_;  
  bool triggerMatches_;
};

// constructors and destructor
DiLeptonSystematicTreesFromMiniAOD::DiLeptonSystematicTreesFromMiniAOD(const edm::ParameterSet& iConfig):
  fctBTagEff_    (iConfig.getParameter<edm::ParameterSet>("bTagEfficiencies") ),
  
  fctBTagCalibFullSim_    (iConfig.getParameter<edm::ParameterSet>("BTagCalibration").getParameter<std::string>("CSVFullSimTagger"),iConfig.getParameter<edm::ParameterSet>("BTagCalibration").getParameter<std::string>("CSVFullSimFileName") ),
  fctBTagCalibReaderFullSimBJets_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_bJets"),"central" ),
  fctBTagCalibReaderFullSimCJets_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_cJets"),"central" ),
  fctBTagCalibReaderFullSimLightJets_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_lightJets"),"central" ),
  fctBTagCalibReaderFullSimBJetsUp_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_bJets"),"up" ),
  fctBTagCalibReaderFullSimCJetsUp_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_cJets"),"up" ),
  fctBTagCalibReaderFullSimLightJetsUp_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_lightJets"),"up" ),
  fctBTagCalibReaderFullSimBJetsDown_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_bJets"),"down" ),
  fctBTagCalibReaderFullSimCJetsDown_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_cJets"),"down" ),
  fctBTagCalibReaderFullSimLightJetsDown_    (&fctBTagCalibFullSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_lightJets"),"down" ),
  
  fctBTagCalibFastSim_    (iConfig.getParameter<edm::ParameterSet>("BTagCalibration").getParameter<std::string>("CSVFastSimTagger"),iConfig.getParameter<edm::ParameterSet>("BTagCalibration").getParameter<std::string>("CSVFastSimFileName") ),
  fctBTagCalibReaderFastSim_    (&fctBTagCalibFastSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_fastSim"),"central" ),
  fctBTagCalibReaderFastSimUp_    (&fctBTagCalibFastSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_fastSim"),"up" ),
  fctBTagCalibReaderFastSimDown_    (&fctBTagCalibFastSim_,BTagEntry::OP_MEDIUM,iConfig.getParameter<edm::ParameterSet>("BTagCalibrationReader").getParameter<std::string>("measurementType_fastSim"),"down" ),
  
  fctLeptonFullSimScaleFactors_ (iConfig.getParameter<edm::ParameterSet>("LeptonFullSimScaleFactors") ),
  fctLeptonFastSimScaleFactors_ (iConfig.getParameter<edm::ParameterSet>("LeptonFastSimScaleFactors") ),
  
  fctVtxWeight_    (iConfig.getParameter<edm::ParameterSet>("vertexWeights") ),
  fctVtxWeightUp_    (iConfig.getParameter<edm::ParameterSet>("vertexWeightsUp") ),
  fctVtxWeightDown_    (iConfig.getParameter<edm::ParameterSet>("vertexWeightsDown") ),
  fctIsolation_  (iConfig.getParameter<edm::ParameterSet>("isolationDefinitions")),
  fctTrigger_  (iConfig.getParameter<edm::ParameterSet>("triggerDefinitions")),  
  getPdgId_( iConfig.getParameter< edm::ParameterSet>("pdgIdDefinition") )
{
  debug = false;
  useTaus_ = iConfig.existsAs<edm::InputTag>("taus");
  useJets2_ = iConfig.existsAs<edm::InputTag>("jets2");
  writeID_ = iConfig.existsAs<edm::InputTag>("baseTrees");
  triggerMatches_ = iConfig.existsAs<edm::InputTag>("triggerSummaryTag");  
  metUncert_ = iConfig.existsAs<edm::InputTag>("doMETUncert");  
  // read config
  eTag_ = iConfig.getParameter<edm::InputTag>("electrons");
  muTag_ = iConfig.getParameter<edm::InputTag>("muons");
  if(useTaus_)tauTag_ = iConfig.getParameter<edm::InputTag>("taus");
  jetTag_ = iConfig.getParameter<edm::InputTag>("jets");
  jetTagPuppi_ = iConfig.getParameter<edm::InputTag>("jetsPuppi");
  if(useJets2_) jet2Tag_ = iConfig.getParameter<edm::InputTag>("jets2");
  bJetTag_ = iConfig.getParameter<edm::InputTag>("bJets");
  bJetTagPuppi_ = iConfig.getParameter<edm::InputTag>("bJetsPuppi");
  metTag_ = iConfig.getParameter<edm::InputTag>("met");
  metTagPuppi_ = iConfig.getParameter<edm::InputTag>("metPuppi");
  metNoHFTag_ = iConfig.getParameter<edm::InputTag>("metNoHF");  
  vertexTag_ = iConfig.getParameter<edm::InputTag>("vertices");
  pfCandTag_ = iConfig.getParameter<edm::InputTag>("pfCands");
  susyVars_ = iConfig.getParameter< std::vector<edm::ParameterSet> >("susyVars");
  genParticleTag_ = iConfig.getParameter<edm::InputTag>("genParticles");
  LHEEventTag_ = iConfig.getParameter<edm::InputTag>("LHEInfo");
  genEventInfoTag_ = iConfig.getParameter<edm::InputTag>("pdfInfo");
  rhoTag_ = iConfig.getParameter<edm::InputTag>("rho");
  pdfs_ = iConfig.getParameter<std::vector<edm::InputTag> > ("pdfWeightTags");
	
  tauId_ = iConfig.getParameter<std::string >("tauId");
  fakeRates_.SetSource(iConfig,"fakeRates");// TODO use these and add mcInfo flag to choose right rates...
  efficiencies_.SetSource(iConfig,"efficiencies");// TODO use these and add mcInfo flag to choose right rates...
  
  if(iConfig.existsAs<edm::VParameterSet>("electronCorrections")){
    edm::VParameterSet bins = iConfig.getParameter<edm::VParameterSet>("electronCorrections");
    for(edm::VParameterSet::const_iterator it = bins.begin(); it != bins.end(); ++it){
      float absEta = (*it).getParameter<double>("absEta");
      electronCorrections_[absEta] = (*it).getParameter<double>("correction");
    }
  }

  // init trees
  edm::Service<TFileService> file;
  trees_["EE"] = file->make<TTree>("EEDileptonTree", "EE DileponTree");
  trees_["EMu"] = file->make<TTree>("EMuDileptonTree", "EMu DileponTree");
  trees_["MuMu"] = file->make<TTree>("MuMuDileptonTree", "MuMu DileponTree");
  if(useTaus_){
    trees_["ETau"] = file->make<TTree>("ETauDileptonTree", "ETau DileponTree");
    trees_["MuTau"] = file->make<TTree>("MuTauDileptonTree", "MuTau DileponTree");
    trees_["TauTau"] = file->make<TTree>("TauTauDileptonTree", "TauTau DileponTree");
  }
  initFloatBranch( "genWeight" );  
  initFloatBranch( "genWeightAbsValue" );    
  initFloatBranch( "weight" );
  initFloatBranch( "weightUp" );
  initFloatBranch( "weightDown" );
  initFloatBranch( "bTagWeight" );
  initFloatBranch( "bTagWeightErrHeavy" );
  initFloatBranch( "bTagWeightErrLight" );
  initFloatBranch( "bTagWeightPuppi" );
  initFloatBranch( "bTagWeightErrHeavyPuppi" );
  initFloatBranch( "bTagWeightErrLightPuppi" );
  initFloatBranch( "leptonFullSimScaleFactor1" );
  initFloatBranch( "leptonFullSimScaleFactor2" );
  initFloatBranch( "leptonFastSimScaleFactor1" );
  initFloatBranch( "leptonFastSimScaleFactor2" );
  initFloatBranch( "ISRUncertainty" );
  initFloatBranch( "genPtDiSbottom" );
  initFloatBranch( "genPtTop1" );
  initFloatBranch( "genPtTop2" );
  initFloatBranch( "chargeProduct" );
  initFloatBranch( "charge1" );
  initFloatBranch( "charge2" );
  initFloatBranch( "mSbottom" );
  initFloatBranch( "mNeutralino2" );
  initTLorentzVectorBranch( "p4" );
  initTLorentzVectorBranch( "p4Gen" );
  initTLorentzVectorBranch( "lepton1" );
  initTLorentzVectorBranch( "lepton2" );
  initTLorentzVectorBranch( "genLepton1" );
  initTLorentzVectorBranch( "genLepton2" );
  initTLorentzVectorBranch( "jet1" );
  initTLorentzVectorBranch( "jet2" );
  initTLorentzVectorBranch( "jet3" );
  initTLorentzVectorBranch( "jet4" );
  initTLorentzVectorBranch( "jet1Puppi" );
  initTLorentzVectorBranch( "jet2Puppi" );
  initTLorentzVectorBranch( "jet3Puppi" );
  initTLorentzVectorBranch( "jet4Puppi" );
  initTLorentzVectorBranch( "bJet1" );
  initTLorentzVectorBranch( "bJet2" );
  initTLorentzVectorBranch( "bJet1Puppi" );
  initTLorentzVectorBranch( "bJet2Puppi" );
  initTLorentzVectorBranch( "vMet" ); 
  initTLorentzVectorBranch( "vMetPuppi" );  
  initTLorentzVectorBranch( "vMetUncorrected" );
  initTLorentzVectorBranch( "vMetNoHF" );  
  initTLorentzVectorBranch( "vMetNoHFUncorrected" );   
  initTLorentzVectorBranch( "vGenMet" ); 
  initTLorentzVectorBranch( "vMHT" );   
  initTLorentzVectorBranch( "vMHTLoose" );
  initTLorentzVectorBranch( "vMHTPuppi" );   
  initTLorentzVectorBranch( "vMHTLoosePuppi" );
  initFloatBranch( "rho" );
  initFloatBranch( "pt1" );
  initFloatBranch( "pt2" );
  initFloatBranch( "eta1" );
  initFloatBranch( "eta2" );
  initFloatBranch( "isoEffArea1" );
  initFloatBranch( "isoEffArea2" );
  initFloatBranch( "isoDeltaBeta1" );
  initFloatBranch( "isoDeltaBeta2" );
  initFloatBranch( "miniIsoEffArea1" );
  initFloatBranch( "miniIsoEffArea2" );
  initFloatBranch( "miniIsoDeltaBeta1" );
  initFloatBranch( "miniIsoDeltaBeta2" );
  initFloatBranch( "miniIsoConeSize1" );
  initFloatBranch( "miniIsoConeSize2" );  
  initFloatBranch( "miniIsoPF1" );
  initFloatBranch( "miniIsoPF2" );
    
  initFloatBranch( "chargedIso1");
  initFloatBranch( "neutralIso1");
  initFloatBranch( "photonIso1");
  initFloatBranch( "puIso1");
  initFloatBranch( "effectiveArea1");
  
  initFloatBranch( "chargedIso2");
  initFloatBranch( "neutralIso2");
  initFloatBranch( "photonIso2");
  initFloatBranch( "puIso2");
  initFloatBranch( "effectiveArea2");
  
  initFloatBranch( "dB1" );
  initFloatBranch( "dB2" );
  initFloatBranch( "mt1" );
  initFloatBranch( "mt2" );
  initFloatBranch( "fakeWeight1" );
  initFloatBranch( "fakeWeight2" );
  initFloatBranch( "deltaPhi" );
  initFloatBranch( "deltaR" );
  initFloatBranch( "jzbResponse" );
  initFloatBranch( "jzbResponseUncorr" );  
  initFloatBranch( "jzbResponsePuppi" );  
  initFloatBranch( "jzb" );
  initFloatBranch( "jzbUncorr" );  
  initFloatBranch( "jzbPuppi" );  
  initFloatBranch( "ht" );
  initFloatBranch( "htPuppi" );
  initFloatBranch( "htJESUp" );
  initFloatBranch( "htJESDown" );
  initFloatBranch( "mht" );
  initFloatBranch( "mhtPuppi" );
  initFloatBranch( "met" );
  initFloatBranch( "metPuppi" );
  initFloatBranch( "uncorrectedMet" );
  initFloatBranch( "metNoHF" );
  initFloatBranch( "uncorrectedMetNoHF" );  
  initFloatBranch( "genMet" );
  initFloatBranch( "metJESUp" );
  initFloatBranch( "metJESDown" );
  //~ initFloatBranch( "pZeta" );
  //~ initFloatBranch( "pZetaVis" );
  initIntBranch( "nJets" );
  initIntBranch( "nJetsPuppi" );
  initIntBranch( "nJets30" );  
  //initIntBranch( "nJetsNoPULoose" );
  //initIntBranch( "nJetsNoPUMedium" );
  //initIntBranch( "nJetsNoPUTight" );      
  initIntBranch( "nJetsOld" );
  initIntBranch( "nBJets" );
  initIntBranch( "nBJetsPuppi" );
  initIntBranch( "nShiftedJetsJESUp" );
  initIntBranch( "nShiftedJetsJESDown" );
  initIntBranch( "nVertices" );
  initIntBranch( "nGenVertices" );
  initIntBranch( "nLightLeptons" );
  initFloatBranch( "jet1pt" );
  initFloatBranch( "jet2pt" );
  initFloatBranch( "jet3pt" );
  initFloatBranch( "jet4pt" );
  initFloatBranch( "jet1ptPuppi" );
  initFloatBranch( "jet2ptPuppi" );
  initFloatBranch( "jet3ptPuppi" );
  initFloatBranch( "jet4ptPuppi" );
  initFloatBranch( "bjet1pt" );
  initFloatBranch( "bjet2pt" );
  initFloatBranch( "bjet1ptPuppi" );
  initFloatBranch( "bjet2ptPuppi" );
  initFloatBranch( "genHT" );
  initFloatBranch( "sqrts" ); 
  initIntBranch( "runNr" );
  initIntBranch( "lumiSec" );
  initIntBranch( "eventNr" );
  initIntBranch( "pdgId1" );
  initIntBranch( "pdgId2" );
  initIntBranch( "matched" );
  initIntBranch( "motherPdgId1" );
  initIntBranch( "motherPdgId2" );
  initIntBranch( "grandMotherPdgId1" );
  initIntBranch( "grandMotherPdgId2" );  
  initIntBranch( "isPrompt1" );
  initIntBranch( "isPrompt2" );
  initIntBranch( "isFromTau1" );
  initIntBranch( "isFromTau2" );
  initIntBranch( "isPromptHardProcess1" );
  initIntBranch( "isPromptHardProcess2" );
  initIntBranch( "isFromTauHardProcess1" );
  initIntBranch( "isFromTauHardProcess2" );
      
      
  if(useJets2_) {
    initFloatBranch( "ht2" );
    initIntBranch( "nJets2" );    
  }
  if (writeID_){
 
 
	  initFloatBranch( "deltaEtaSuperClusterTrackAtVtx1");
	  initFloatBranch( "deltaPhiSuperClusterTrackAtVtx1");
	  initFloatBranch( "sigmaIetaIeta1");
	  initFloatBranch( "hadronicOverEm1");
	  initFloatBranch( "eOverP1");
	  initFloatBranch( "missingHits1");
	  initFloatBranch( "d01");
	  initFloatBranch( "dZ1");
	  initFloatBranch( "globalMuon1");
	  initFloatBranch( "trackerMuon1");
	  initFloatBranch( "pfMuon1");
	  initFloatBranch( "trackChi21");
	  initFloatBranch( "numberOfValidMuonHits1");
	  initFloatBranch( "numberOfMatchedStations1");
	  initFloatBranch( "numberOfValidPixelHits1");
	  initFloatBranch( "trackerLayersWithMeasurement1");
	  initFloatBranch( "chargedIso1");
	  initFloatBranch( "neutralIso1");
	  initFloatBranch( "photonIso1");
	  initFloatBranch( "puIso1");
	  initFloatBranch( "effectiveArea1");
	  initFloatBranch( "passConversion1");

	  initFloatBranch( "deltaEtaSuperClusterTrackAtVtx2");
	  initFloatBranch( "deltaPhiSuperClusterTrackAtVtx2");
	  initFloatBranch( "sigmaIetaIeta2");
	  initFloatBranch( "hadronicOverEm2");
	  initFloatBranch( "eOverP2");
	  initFloatBranch( "missingHits2");
	  initFloatBranch( "d02");
	  initFloatBranch( "dZ2");
	  initFloatBranch( "globalMuon2");
	  initFloatBranch( "trackerMuon2");
	  initFloatBranch( "pfMuon2");
	  initFloatBranch( "trackChi22");
	  initFloatBranch( "numberOfValidMuonHits2");
	  initFloatBranch( "numberOfMatchedStations2");
	  initFloatBranch( "numberOfValidPixelHits2");
	  initFloatBranch( "trackerLayersWithMeasurement2");
	  initFloatBranch( "chargedIso2");
	  initFloatBranch( "neutralIso2");
	  initFloatBranch( "photonIso2");
	  initFloatBranch( "puIso2");
	  initFloatBranch( "effectiveArea2");
	  initFloatBranch( "passConversion2"); 
  
  }
  
  if (triggerMatches_){
  		
	  initIntBranch( "matchesSingleElectron1" );
	  initIntBranch( "matchesSingleElectron2" );
	  initIntBranch( "matchesSingleMuon1" );
	  initIntBranch( "matchesSingleMuon2" );
	  initIntBranch( "matchesDoubleElectronTrailing1" );
	  initIntBranch( "matchesDoubleElectronTrailing2" );
	  initIntBranch( "matchesDoubleElectronTrailingNonIso1" );
	  initIntBranch( "matchesDoubleElectronTrailingNonIso2" );	  
	  initIntBranch( "matchesDoubleElectronLeading1" );
	  initIntBranch( "matchesDoubleElectronLeading2" );	 
	  initIntBranch( "matchesDoubleElectronLeadingNonIso1" );
	  initIntBranch( "matchesDoubleElectronLeadingNonIso2" );		  
	  initIntBranch( "matchesDoubleMuonLeading1" );
	  initIntBranch( "matchesDoubleMuonLeading2" );
	  initIntBranch( "matchesDoubleMuonLeadingNonIso1" );
	  initIntBranch( "matchesDoubleMuonLeadingNonIso2" );	  
	  initIntBranch( "matchesDoubleMuonLeadingBoth1" );
	  initIntBranch( "matchesDoubleMuonLeadingBoth2" );	  
	  initIntBranch( "matchesDoubleMuonLeadingTk1" );
	  initIntBranch( "matchesDoubleMuonLeadingTk2" );
	  initIntBranch( "matchesEMuLeading1" );
	  initIntBranch( "matchesEMuLeading2" );
	  initIntBranch( "matchesMuELeading1" );
	  initIntBranch( "matchesMuELeading2" );
	  initIntBranch( "matchesDoubleMuonTrailing1" );
	  initIntBranch( "matchesDoubleMuonTrailing2" );
	  initIntBranch( "matchesDoubleMuonTrailingNonIso1" );
	  initIntBranch( "matchesDoubleMuonTrailingNonIso2" );	  
	  initIntBranch( "matchesDoubleMuonTrailingBoth1" );
	  initIntBranch( "matchesDoubleMuonTrailingBoth2" );	  
	  initIntBranch( "matchesDoubleMuonTrailingTk1" );
	  initIntBranch( "matchesDoubleMuonTrailingTk2" );
	  initIntBranch( "matchesEMuTrailing1" );
	  initIntBranch( "matchesEMuTrailing2" );
	  initIntBranch( "matchesMuETrailing1" );
	  initIntBranch( "matchesMuETrailing2" );  
	  initIntBranch( "matchesMuEGMuonNonIso1" );
	  initIntBranch( "matchesMuEGMuonNonIso2" );
	  initIntBranch( "matchesMuEGElectronNonIso1" );
	  initIntBranch( "matchesMuEGElectronNonIso2" );  
  
  }
  
  
  if (metUncert_){
  		
	  initFloatBranch( "metJetEnUp");
	  initFloatBranch( "metJetEnDown");
	  initFloatBranch( "metJetResUp");
	  initFloatBranch( "metJetResDown");
	  initFloatBranch( "metMuonEnUp");
	  initFloatBranch( "metMuonEnDown");
	  initFloatBranch( "metElectronEnUp");
	  initFloatBranch( "metElectronEnDown");
	  initFloatBranch( "metTauEnUp");
	  initFloatBranch( "metTauEnDown");
	  initFloatBranch( "metUnclusteredEnUp");
	  initFloatBranch( "metUnclusteredEnDown");	  	  
  
  
  }  
  
  for ( std::vector<edm::ParameterSet>::iterator susyVar_i = susyVars_.begin(); susyVar_i != susyVars_.end(); ++susyVar_i ) {
    edm::InputTag var = susyVar_i->getParameter<edm::InputTag>( "var" );
    std::string type = susyVar_i->getParameter<std::string>( "type" );
    if(debug) std::cout << var << " of type " << type << std::endl;
    if (type=="int") initIntBranch( convertInputTag(var) );
    else if (type=="float") initFloatBranch( convertInputTag(var) );
    else throw cms::Exception("Unrecognized type") << 
      "Unknown type " << type << " for variable" << var << " found\n";
  }
  for ( std::vector<edm::InputTag>::iterator pdf_i = pdfs_.begin(); pdf_i != pdfs_.end(); ++pdf_i ) {
     std::string pdfIdentifier = (*pdf_i).instance();
     std::string up = "Up";
     std::string down = "Down";
     initFloatBranch( pdfIdentifier );
     initFloatBranch( pdfIdentifier+up );
     initFloatBranch( pdfIdentifier+down );
  }
}

void 
DiLeptonSystematicTreesFromMiniAOD::initTLorentzVectorBranch(const std::string &name)
{
  for( std::map<std::string, TTree*>::const_iterator it = trees_.begin();
       it != trees_.end(); ++it){
    if(debug) std::cout << (*it).first <<" - "<< name << std::endl;
    tLorentzVectorBranches_[(*it).first][name] = new TLorentzVector;
    (*it).second->Branch(name.c_str(), "TLorentzVector" ,&tLorentzVectorBranches_[(*it).first][name]);
  }
}

void 
DiLeptonSystematicTreesFromMiniAOD::initFloatBranch(const std::string &name)
{
  for( std::map<std::string, TTree*>::const_iterator it = trees_.begin();
       it != trees_.end(); ++it){
    if(debug) std::cout << (*it).first <<" - "<< name << std::endl;
    floatBranches_[(*it).first][name] = new float;
    (*it).second->Branch(name.c_str(), floatBranches_[(*it).first][name], (name+"/F").c_str());
  }
}

void 
DiLeptonSystematicTreesFromMiniAOD::initIntBranch(const std::string &name)
{
  for( std::map<std::string, TTree*>::const_iterator it = trees_.begin();
       it != trees_.end(); ++it){
    if(debug) std::cout << (*it).first <<" - "<< name << std::endl;
    intBranches_[(*it).first][name] = new unsigned int;
    (*it).second->Branch(name.c_str(), intBranches_[(*it).first][name], (name+"/I").c_str());
  }
}

DiLeptonSystematicTreesFromMiniAOD::~DiLeptonSystematicTreesFromMiniAOD()
{ 
  for( std::map<std::string, std::map< std::string, float*> >::const_iterator it = floatBranches_.begin();
       it != floatBranches_.end(); ++it){
    for( std::map< std::string, float*>::const_iterator it2 = (*it).second.begin();
	 it2 != (*it).second.end(); ++it2){
      if(debug)std::cout << "deleting: " << (*it).first << " - "<< (*it2).first << std::endl;
      delete (*it2).second;
    }
  }
  for( std::map<std::string, std::map< std::string, unsigned int*> >::const_iterator it = intBranches_.begin();
       it != intBranches_.end(); ++it){
    for( std::map< std::string, unsigned int*>::const_iterator it2 = (*it).second.begin();
	 it2 != (*it).second.end(); ++it2){
      if(debug) std::cout << "deleting: " << (*it).first << " - "<< (*it2).first << std::endl;
      delete (*it2).second;
    }
  }


}


// member functions
// ------------ method called to for each event  ------------
void
DiLeptonSystematicTreesFromMiniAOD::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle< std::vector< pat::Electron > > electrons;
  iEvent.getByLabel(eTag_, electrons);

  edm::Handle< std::vector< pat::Muon > > muons;
  iEvent.getByLabel(muTag_, muons);

  edm::Handle< std::vector<pat::PackedCandidate>  > pfCands;
  iEvent.getByLabel(pfCandTag_, pfCands); 



  edm::Handle< std::vector< pat::Tau > > taus;
  if(useTaus_)
    iEvent.getByLabel(tauTag_, taus);
  
  //edm::Handle< std::vector< pat::Jet > > jets;
  iEvent.getByLabel(jetTag_, jets_);


  iEvent.getByLabel(jetTagPuppi_, jetsPuppi_);


	
  edm::Handle< std::vector< pat::Jet > > bJets;
  iEvent.getByLabel(bJetTag_, bJets);
  
  edm::Handle< std::vector< pat::Jet > > bJetsPuppi;
  iEvent.getByLabel(bJetTagPuppi_, bJetsPuppi);
  
  edm::Handle< std::vector< pat::MET > > mets;
  iEvent.getByLabel(metTag_, mets);
  
  edm::Handle< std::vector< pat::MET > > metsPuppi;
  iEvent.getByLabel(metTagPuppi_, metsPuppi);
  
  edm::Handle< std::vector< pat::MET > > metsNoHF;
  iEvent.getByLabel(metNoHFTag_, metsNoHF);


  edm::Handle< std::vector< reco::GenParticle > > genParticles;
  iEvent.getByLabel(genParticleTag_, genParticles);


  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertexTag_, vertices);

  getPdgId_.loadGenParticles(iEvent);
  fctIsolation_.init(iEvent);
  fctTrigger_.loadTrigger(iEvent);
  std::map<std::string, int> intEventProperties;
  std::map<std::string, float> floatEventProperties;
  std::map<std::string, TLorentzVector> tLorentzVectorEventProperties;


  edm::Handle<GenEventInfoProduct> genInfoProduct;
  iEvent.getByLabel(genEventInfoTag_, genInfoProduct);	
  if (genInfoProduct.isValid()){
  	floatEventProperties["genWeightAbsValue"] = (*genInfoProduct).weight();
   	if ((*genInfoProduct).weight() < 0.0){
   	
   		floatEventProperties["genWeight"] = -1;
   	}
   	else{
   		floatEventProperties["genWeight"] = 1;   	
   	}
  }
  else{
 
 	floatEventProperties["genWeight"] = 1; 
 	floatEventProperties["genWeightAbsValue"] = 1;
  
  }	  
  
	// stolen from https://github.com/Aachen-3A/PxlSkimmer/blob/master/Skimming/src/PxlSkimmer_miniAOD.cc#L590
    edm::Handle<LHEEventProduct> lheInfoHandle;
    iEvent.getByLabel(LHEEventTag_ , lheInfoHandle);

    if (lheInfoHandle.isValid()) {
        lhef::HEPEUP lheParticleInfo = lheInfoHandle->hepeup();
        // get the five vector
        // (Px, Py, Pz, E and M in GeV)
        std::vector<lhef::HEPEUP::FiveVector> allParticles = lheParticleInfo.PUP;
        std::vector<int> statusCodes = lheParticleInfo.ISTUP;

        double ht = 0;
        for (unsigned int i = 0; i < statusCodes.size(); i++) {
            if (statusCodes[i] == 1) {
                if (abs(lheParticleInfo.IDUP[i]) < 11 || abs(lheParticleInfo.IDUP[i]) > 16) {
                    ht += sqrt(pow(allParticles[i][0], 2) + pow(allParticles[i][1], 2));
                }
            }
        }
        floatEventProperties["genHT"] = ht;
    }
	
	else floatEventProperties["genHT"] = -999.;
      	

	
  intEventProperties["nVertices"] = vertices->size();
  
  int nGenVertices = -1;
  
  if (genParticles.isValid()){
	  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
	  iEvent.getByLabel(edm::InputTag("slimmedAddPileupInfo"), PupInfo);
	
	  std::vector<PileupSummaryInfo>::const_iterator PVI;

  
	  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
		
		int BX = PVI->getBunchCrossing();
		
		if(BX == 0) { 
		  nGenVertices = PVI->getTrueNumInteractions();
		  continue;
		}
	  }
  }
  
  intEventProperties["nGenVertices"] = nGenVertices;
    
    

  edm::Handle<double> rho;
  iEvent.getByLabel(rhoTag_,rho);
  floatEventProperties["rho"] = (float)(*rho);
  const float Rho = *rho;


  intEventProperties["nBJets"] = bJets->size();
  intEventProperties["nBJetsPuppi"] = bJetsPuppi->size();
  intEventProperties["nLightLeptons"] = electrons->size() + muons->size();
  intEventProperties["runNr"] = iEvent.id().run();
  intEventProperties["lumiSec"] = iEvent.id().luminosityBlock();
  intEventProperties["eventNr"] = iEvent.id().event();



  pat::MET met = mets->front();
  TLorentzVector metVector(mets->front().px(), mets->front().py(), mets->front().pz(), mets->front().energy());
  TLorentzVector uncorrectedMetVector;
  uncorrectedMetVector.SetPtEtaPhiE(mets->front().uncorPt(), 0,	mets->front().uncorPhi(), mets->front().uncorPt());
  
  floatEventProperties["met"] = metVector.Pt();
  tLorentzVectorEventProperties["vMet"] = metVector; 
  
  tLorentzVectorEventProperties["vMetUncorrected"] = uncorrectedMetVector;
  floatEventProperties["uncorrectedMet"] = uncorrectedMetVector.Pt();
  
  
  pat::MET metPuppi = metsPuppi->front();
  TLorentzVector metVectorPuppi(metsPuppi->front().px(), metsPuppi->front().py(), metsPuppi->front().pz(), metsPuppi->front().energy());
  
  floatEventProperties["metPuppi"] = metVectorPuppi.Pt();
  tLorentzVectorEventProperties["vMetPuppi"] = metVectorPuppi; 
  
  
  pat::MET metNoHF = metsNoHF->front();
  TLorentzVector metVectorNoHF(metsNoHF->front().px(), metsNoHF->front().py(), metsNoHF->front().pz(), metsNoHF->front().energy());
  TLorentzVector uncorrectedMetVectorNoHF;
  uncorrectedMetVectorNoHF.SetPtEtaPhiE(metsNoHF->front().uncorPt(), 0,	metsNoHF->front().uncorPhi(), metsNoHF->front().uncorPt());
  
  floatEventProperties["metNoHF"] = metVectorNoHF.Pt();
  tLorentzVectorEventProperties["vMetNoHF"] = metVectorNoHF; 
  
  tLorentzVectorEventProperties["vMetNoHFUncorrected"] = uncorrectedMetVectorNoHF;
  floatEventProperties["uncorrectedMetNoHF"] = uncorrectedMetVectorNoHF.Pt();  
  

  pat::METCollection const& metsForUncert = *mets;	
  floatEventProperties["met"] =  metsForUncert[0].pt();	
  if (metUncert_){

 	floatEventProperties["metJetEnUp"] = metsForUncert[0].shiftedPt(pat::MET::JetEnUp); 
 	floatEventProperties["metJetEnDown"] = metsForUncert[0].shiftedPt(pat::MET::JetEnDown); 
 	floatEventProperties["metJetResUp"] = metsForUncert[0].shiftedPt(pat::MET::JetResUp);
 	floatEventProperties["metJetResDown"] = metsForUncert[0].shiftedPt(pat::MET::JetResDown);
 	floatEventProperties["metMuonEnUp"] = metsForUncert[0].shiftedPt(pat::MET::MuonEnUp);
 	floatEventProperties["metMuonEnDown"] = metsForUncert[0].shiftedPt(pat::MET::MuonEnDown);
 	floatEventProperties["metElectronEnUp"] = metsForUncert[0].shiftedPt(pat::MET::ElectronEnUp);
 	floatEventProperties["metElectronEnDown"] = metsForUncert[0].shiftedPt(pat::MET::ElectronEnDown);
 	floatEventProperties["metTauEnUp"] = metsForUncert[0].shiftedPt(pat::MET::TauEnUp);
 	floatEventProperties["metTauEnDown"] = metsForUncert[0].shiftedPt(pat::MET::TauEnDown);
 	floatEventProperties["metUnclusteredEnUp"] = metsForUncert[0].shiftedPt(pat::MET::UnclusteredEnUp);
 	floatEventProperties["metUnclusteredEnDown"] = metsForUncert[0].shiftedPt(pat::MET::UnclusteredEnDown);  
  
  }

  TLorentzVector genMetVector(0.,0.,0.,0.);
  TLorentzVector vGenParticle(0.,0.,0.,0.);
  
  TLorentzVector genSbottom1(0.,0.,0.,0.);
  TLorentzVector genSbottom2(0.,0.,0.,0.);
  TLorentzVector genDiSbottom(0.,0.,0.,0.);
  
  floatEventProperties["mSbottom"] = -1;
  floatEventProperties["mNeutralino2"] = -1;
  floatEventProperties["genPtDiSbottom"] = -1;
  float ISRUncertainty = 0.;
  
  floatEventProperties["genPtTop1"] = -1;
  floatEventProperties["genPtTop2"] = -1;
  if (genParticles.isValid()){
	
	for (std::vector<reco::GenParticle>::const_iterator itGenParticle = genParticles->begin(); itGenParticle != genParticles->end(); itGenParticle++) {
		
		vGenParticle.SetPxPyPzE((*itGenParticle).px(), (*itGenParticle).py(), (*itGenParticle).pz(), (*itGenParticle).energy());
		genMetVector -= vGenParticle;		

		if (abs((*itGenParticle).pdgId())== 6){

			if ((*itGenParticle).pdgId()== 6){
				floatEventProperties["genPtTop1"] = (*itGenParticle).pt();
			}
			else if ((*itGenParticle).pdgId()== -6){
				floatEventProperties["genPtTop2"] = (*itGenParticle).pt();
			}

		}
		
		if (abs((*itGenParticle).pdgId())== 1000005){
			if ((*itGenParticle).pdgId()== 1000005){
				genSbottom1.SetPxPyPzE((*itGenParticle).px(), (*itGenParticle).py(), (*itGenParticle).pz(), (*itGenParticle).energy());
				floatEventProperties["mSbottom"] = (*itGenParticle).mass();
			}
			else if ((*itGenParticle).pdgId()== -1000005){
				genSbottom2.SetPxPyPzE((*itGenParticle).px(), (*itGenParticle).py(), (*itGenParticle).pz(), (*itGenParticle).energy());
			}

		}
		if (abs((*itGenParticle).pdgId())== 1000023 ){
			floatEventProperties["mNeutralino2"] = (*itGenParticle).mass();
		}


	}

  }
  
  floatEventProperties["genMet"] = genMetVector.Pt();
  tLorentzVectorEventProperties["vGenMet"] = genMetVector;
  
  genDiSbottom = genSbottom1 + genSbottom2;
  if (genDiSbottom.Pt() > 1){
  	floatEventProperties["genPtDiSbottom"] = genDiSbottom.Pt();
  }
  
  if (genDiSbottom.Pt() < 400) {
	ISRUncertainty = 0.;
  }
  else {
	if (genDiSbottom.Pt() < 600){
		ISRUncertainty = 0.15;
	}
	else {
		ISRUncertainty = 0.3;
	}
  }
  floatEventProperties["ISRUncertainty"] = ISRUncertainty;


  TLorentzVector MHT;
  TLorentzVector MHTLoose;  
  TLorentzVector MHTPuppi;
  TLorentzVector MHTLoosePuppi;  
  TLorentzVector tempMHT;
  TLorentzVector tempMHTLoose;
  
  floatEventProperties["ht"] = 0.0;
  floatEventProperties["htPuppi"] = 0.0;
  floatEventProperties["htJESUp"] = 0.0;
  floatEventProperties["htJESDown"] = 0.0;
  floatEventProperties["mht"] = 0.0;
  floatEventProperties["mhtPuppi"] = 0.0;

  edm::FileInPath fip("SuSyAachen/DiLeptonHistograms/data/GR_P_V40_AN1::All_Uncertainty_AK5PF.txt");
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(fip.fullPath());

  //edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
  //iSetup.get<JetCorrectionsRecord>().get("AK4PF",JetCorParColl); 
  //JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
  //JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar);


  TLorentzVector changeUp(0,0,0,0);
  TLorentzVector changeDown(0,0,0,0);
  TLorentzVector tempChangeUp;
  TLorentzVector tempChangeDown;
  // loop over jets
  std::vector<pat::Jet> * shiftedJetsJESUp = new std::vector<pat::Jet>(); 
  std::vector<pat::Jet> * shiftedJetsJESDown = new std::vector<pat::Jet>(); 



  for (std::vector<pat::Jet>::const_iterator itJet = jets_->begin(); itJet != jets_->end(); itJet++) {
	
	

    pat::Jet ajetUp( *itJet );
    pat::Jet ajetDown( *itJet );
    jecUnc->setJetEta(itJet->eta());
    jecUnc->setJetPt(itJet->pt()); // IMPORTANT: the uncertainty is a function of the CORRECTED pt
    double correction = jecUnc->getUncertainty(true);
    //double correction = itJet->relCorrUncert(dir_); 
    
    //use pat::Jet::relCorrUncert
    if (correction==0.0){
	correction = 0.045; 
    }
    //    std::cout << "jet pt = "<<itJet->p4()<<", uncertainty = "<<correction<<std::endl;
    ajetUp.setP4( itJet->p4() * (1.0 + correction) );
    ajetDown.setP4( itJet->p4() * (1.0 - correction) );

    //keep track of the change, in order to correct JEC-corrected MET, too.
    tempChangeUp.SetPxPyPzE((ajetUp.px()-(*itJet).px()), (ajetUp.py()-(*itJet).py()), (ajetUp.pz()-(*itJet).pz()), (ajetUp.energy()-(*itJet).energy()));	
    tempChangeDown.SetPxPyPzE((ajetDown.px()-itJet->px()), (ajetDown.py()-itJet->py()), (ajetDown.pz()-itJet->pz()), (ajetDown.energy()-itJet->energy()));	
    changeUp +=tempChangeUp;
    changeDown += tempChangeDown;

    shiftedJetsJESUp->push_back(ajetUp);
    shiftedJetsJESDown->push_back(ajetDown);
  }
  int nJets=0;
  int nJetsPuppi=0;
  int nJets30=0;
  //int nJetsNoPULoose = 0;
  //int nJetsNoPUMedium = 0;
  //int nJetsNoPUTight = 0;
  
  //float puJetID = 0.;	
  
  for(std::vector<pat::Jet>::const_iterator it = jets_->begin(); it != jets_->end() ; ++it){
	if ((*it).pt() >=35.0 && fabs((*it).eta())<2.4){
		nJets++;
		
		//puJetID =  it->userFloat("pileupJetId:fullIdLoose");
		//std::cout << puJetID << endl;
      	//if( PileupJetIdentifier::passJetId( puJetID, PileupJetIdentifier::kLoose )) {
        //   nJetsNoPULoose++;
      	//}
     	//if( PileupJetIdentifier::passJetId( puJetID, PileupJetIdentifier::kMedium )) {
        //   nJetsNoPUMedium++;
        //}
        //if( PileupJetIdentifier::passJetId( puJetID, PileupJetIdentifier::kTight )) {
        //   nJetsNoPUTight++;		
		//}
		tempMHT.SetPxPyPzE((*it).px(), (*it).py(), (*it).pz(), (*it).energy());	
		MHT = MHT + tempMHT;
		floatEventProperties["ht"] += (*it).pt();
	}
	
	if ((*it).pt() >=20.0 && fabs((*it).eta())<5.0){

		tempMHTLoose.SetPxPyPzE((*it).px(), (*it).py(), (*it).pz(), (*it).energy());	
		MHTLoose = MHTLoose + tempMHTLoose;

	}	
	
  }
  intEventProperties["nJets"] = nJets;
  
  for(std::vector<pat::Jet>::const_iterator it = jetsPuppi_->begin(); it != jetsPuppi_->end() ; ++it){
	if ((*it).pt() >=35.0 && fabs((*it).eta())<2.4){
		nJetsPuppi++;
		
		tempMHT.SetPxPyPzE((*it).px(), (*it).py(), (*it).pz(), (*it).energy());	
		MHTPuppi = MHTPuppi + tempMHT;
		floatEventProperties["htPuppi"] += (*it).pt();
	}
	
	if ((*it).pt() >=20.0 && fabs((*it).eta())<5.0){

		tempMHTLoose.SetPxPyPzE((*it).px(), (*it).py(), (*it).pz(), (*it).energy());	
		MHTLoosePuppi = MHTLoosePuppi + tempMHTLoose;

	}	
	
  }
  intEventProperties["nJetsPuppi"] = nJetsPuppi;
  
  nJets30 = 0;
  for(std::vector<pat::Jet>::const_iterator it = jets_->begin(); it != jets_->end() ; ++it){
	if ((*it).pt() >=30.0 && fabs((*it).eta())<2.4){
		nJets30++;
	}
  }
  intEventProperties["nJets30"] = nJets30;  
  
  //intEventProperties["nJetsNoPULoose"] = nJetsNoPULoose;
  //intEventProperties["nJetsNoPUMedium"] = nJetsNoPUMedium;
  //intEventProperties["nJetsNoPUTight"] = nJetsNoPUTight;    
  int nJetsOld=0;
  for(std::vector<pat::Jet>::const_iterator it = jets_->begin(); it != jets_->end() ; ++it){
	if ((*it).pt() >=40.0){
		nJetsOld++;
	}
  }
  intEventProperties["nJetsOld"] = nJetsOld;


  TLorentzVector jet1Vector(0.,0.,0.,0.);
  TLorentzVector jet2Vector(0.,0.,0.,0.);
  TLorentzVector jet3Vector(0.,0.,0.,0.);
  TLorentzVector jet4Vector(0.,0.,0.,0.); 


  if (nJets > 0)
    jet1Vector.SetPxPyPzE(jets_->at(0).px(),jets_->at(0).py(),jets_->at(0).pz(),jets_->at(0).energy());
  if (nJets > 1)
    jet2Vector.SetPxPyPzE(jets_->at(1).px(),jets_->at(1).py(),jets_->at(1).pz(),jets_->at(1).energy());
  if (nJets > 2)
    jet3Vector.SetPxPyPzE(jets_->at(2).px(),jets_->at(2).py(),jets_->at(2).pz(),jets_->at(2).energy());
  if (nJets > 3)
    jet4Vector.SetPxPyPzE(jets_->at(3).px(),jets_->at(3).py(),jets_->at(3).pz(),jets_->at(3).energy());
  tLorentzVectorEventProperties["jet1"] = jet1Vector;
  tLorentzVectorEventProperties["jet2"] = jet2Vector;
  tLorentzVectorEventProperties["jet3"] = jet3Vector;
  tLorentzVectorEventProperties["jet4"] = jet4Vector;


  TLorentzVector jet1VectorPuppi(0.,0.,0.,0.);
  TLorentzVector jet2VectorPuppi(0.,0.,0.,0.);
  TLorentzVector jet3VectorPuppi(0.,0.,0.,0.);
  TLorentzVector jet4VectorPuppi(0.,0.,0.,0.); 


  if (nJetsPuppi > 0)
    jet1VectorPuppi.SetPxPyPzE(jetsPuppi_->at(0).px(),jetsPuppi_->at(0).py(),jetsPuppi_->at(0).pz(),jetsPuppi_->at(0).energy());
  if (nJetsPuppi > 1)
    jet2VectorPuppi.SetPxPyPzE(jetsPuppi_->at(1).px(),jetsPuppi_->at(1).py(),jetsPuppi_->at(1).pz(),jetsPuppi_->at(1).energy());
  if (nJetsPuppi > 2)
    jet3VectorPuppi.SetPxPyPzE(jetsPuppi_->at(2).px(),jetsPuppi_->at(2).py(),jetsPuppi_->at(2).pz(),jetsPuppi_->at(2).energy());
  if (nJetsPuppi > 3)
    jet4VectorPuppi.SetPxPyPzE(jetsPuppi_->at(3).px(),jetsPuppi_->at(3).py(),jetsPuppi_->at(3).pz(),jetsPuppi_->at(3).energy());
  tLorentzVectorEventProperties["jet1Puppi"] = jet1VectorPuppi;
  tLorentzVectorEventProperties["jet2Puppi"] = jet2VectorPuppi;
  tLorentzVectorEventProperties["jet3Puppi"] = jet3VectorPuppi;
  tLorentzVectorEventProperties["jet4Puppi"] = jet4VectorPuppi;




  int nJetsJESUp=0;
  for(std::vector<pat::Jet>::const_iterator it = shiftedJetsJESUp->begin(); it != shiftedJetsJESUp->end() ; ++it){
	if ((*it).pt() >=35.0){
        	nJetsJESUp++;
		floatEventProperties["htJESUp"] += (*it).pt();
	}
  }
  intEventProperties["nShiftedJetsJESUp"] = nJetsJESUp;
  int nJetsJESDown=0;
  for(std::vector<pat::Jet>::const_iterator it = shiftedJetsJESDown->begin(); it != shiftedJetsJESDown->end() ; ++it){
	if ((*it).pt() >=35.0){	
        	nJetsJESDown++;
		floatEventProperties["htJESDown"] += (*it).pt();
	}
  }
  intEventProperties["nShiftedJetsJESDown"] = nJetsJESDown;
  TLorentzVector metVectorJESUp = metVector + changeUp;
  TLorentzVector metVectorJESDown = metVector + changeDown;
  floatEventProperties["metJESUp"] = metVectorJESUp.Pt();


  floatEventProperties["metJESDown"] = metVectorJESDown.Pt();

//   floatEventProperties["metJESUp"] = 0.;
// 
// 
//   floatEventProperties["metJESDown"] = 0.;


//   floatEventProperties["mht"] = MHT.Pt();

  // Jet pt
  floatEventProperties["jet1pt"] = -1.0;
  floatEventProperties["jet2pt"] = -1.0;
  floatEventProperties["jet3pt"] = -1.0;
  floatEventProperties["jet4pt"] = -1.0;
  if (jets_->size() > 0)
    floatEventProperties["jet1pt"] = jets_->at(0).pt();
  if (jets_->size() > 1)
    floatEventProperties["jet2pt"] = jets_->at(1).pt();
  if (jets_->size() > 2)
    floatEventProperties["jet3pt"] = jets_->at(2).pt();
  if (jets_->size() > 3)
    floatEventProperties["jet4pt"] = jets_->at(3).pt();

  floatEventProperties["jet1ptPuppi"] = -1.0;
  floatEventProperties["jet2ptPuppi"] = -1.0;
  floatEventProperties["jet3ptPuppi"] = -1.0;
  floatEventProperties["jet4ptPuppi"] = -1.0;
  if (jetsPuppi_->size() > 0)
    floatEventProperties["jet1ptPuppi"] = jetsPuppi_->at(0).pt();
  if (jetsPuppi_->size() > 1)
    floatEventProperties["jet2ptPuppi"] = jetsPuppi_->at(1).pt();
  if (jetsPuppi_->size() > 2)
    floatEventProperties["jet3ptPuppi"] = jetsPuppi_->at(2).pt();
  if (jetsPuppi_->size() > 3)
    floatEventProperties["jet4ptPuppi"] = jetsPuppi_->at(3).pt();

  // bjet pt

  TLorentzVector bJet1Vector(0.,0.,0.,0.);
  TLorentzVector bJet2Vector(0.,0.,0.,0.);



  floatEventProperties["bjet1pt"] = -1.0;
  floatEventProperties["bjet2pt"] = -1.0;
  if (bJets->size() > 0){
    floatEventProperties["bjet1pt"] = bJets->at(0).pt();
    bJet1Vector.SetPxPyPzE(bJets->at(0).px(),bJets->at(0).py(),bJets->at(0).pz(),bJets->at(0).energy());
  }
  if (bJets->size() > 1){
    floatEventProperties["bjet2pt"] = bJets->at(1).pt();
    bJet2Vector.SetPxPyPzE(bJets->at(1).px(),bJets->at(1).py(),bJets->at(1).pz(),bJets->at(1).energy());
  }
 
  tLorentzVectorEventProperties["bJet1"] = bJet1Vector;
  tLorentzVectorEventProperties["bJet2"] = bJet2Vector;
  
  
  TLorentzVector bJet1VectorPuppi(0.,0.,0.,0.);
  TLorentzVector bJet2VectorPuppi(0.,0.,0.,0.);

  floatEventProperties["bjet1ptPuppi"] = -1.0;
  floatEventProperties["bjet2ptPuppi"] = -1.0;
  if (bJetsPuppi->size() > 0){
    floatEventProperties["bjet1ptPuppi"] = bJetsPuppi->at(0).pt();
    bJet1VectorPuppi.SetPxPyPzE(bJetsPuppi->at(0).px(),bJetsPuppi->at(0).py(),bJetsPuppi->at(0).pz(),bJetsPuppi->at(0).energy());
  }
  if (bJetsPuppi->size() > 1){
    floatEventProperties["bjet2ptPuppi"] = bJetsPuppi->at(1).pt();
    bJet2VectorPuppi.SetPxPyPzE(bJetsPuppi->at(1).px(),bJetsPuppi->at(1).py(),bJetsPuppi->at(1).pz(),bJetsPuppi->at(1).energy());
  }
 
  tLorentzVectorEventProperties["bJet1Puppi"] = bJet1VectorPuppi;
  tLorentzVectorEventProperties["bJet2Puppi"] = bJet2VectorPuppi;
  
  floatEventProperties["bTagWeight"] = -9999.;
  floatEventProperties["bTagWeightErrHeavy"] = -9999.;
  floatEventProperties["bTagWeightErrLight"] = -9999.;
  
  // Calculate btagging weights
  if (genParticles.isValid()){
	  
	  float P_MC = 1.;
	  float P_Data = 1.;
	  float err1 = 0.;
	  float err2 = 0.;
	  float err3 = 0.;
	  float err4 = 0.;
	  
	  for(std::vector<pat::Jet>::const_iterator it = jets_->begin(); it != jets_->end() ; ++it){
		  if ((*it).pt() >=35.0 && fabs((*it).eta())<2.4){
			  
			  int jetFlavor;
			  float eff;
			  float SF, SF_up, SF_down;
			  float SF_FullSim, SF_FullSim_up, SF_FullSim_down;
			  float SF_FastSim, SF_FastSim_up, SF_FastSim_down;
			  float SF_err;
			  
			  jetFlavor = (*it).hadronFlavour();
			  if (jetFlavor == 5)
			  {
				  SF_FullSim = fctBTagCalibReaderFullSimBJets_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FullSim_up = fctBTagCalibReaderFullSimBJetsUp_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FullSim_down = fctBTagCalibReaderFullSimBJetsDown_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  
				  SF_FastSim = fctBTagCalibReaderFastSim_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FastSim_up = fctBTagCalibReaderFastSimUp_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FastSim_down = fctBTagCalibReaderFastSimDown_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  
				  SF = SF_FullSim * SF_FastSim;
				  SF_up = SF_FullSim_up * SF_FastSim_up;
				  SF_down = SF_FullSim_down * SF_FastSim_down;
				  
				  eff = fctBTagEff_(jetFlavor, (*it).pt(), fabs((*it).eta()));
			  }
			  else if (jetFlavor == 4)
			  {
				  SF_FullSim = fctBTagCalibReaderFullSimCJets_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FullSim_up = fctBTagCalibReaderFullSimCJetsUp_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FullSim_down = fctBTagCalibReaderFullSimCJetsDown_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  
				  SF_FastSim = fctBTagCalibReaderFastSim_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FastSim_up = fctBTagCalibReaderFastSimUp_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FastSim_down = fctBTagCalibReaderFastSimDown_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  
				  SF = SF_FullSim * SF_FastSim;
				  SF_up = SF_FullSim_up * SF_FastSim_up;
				  SF_down = SF_FullSim_down * SF_FastSim_down;
				  
				  eff = fctBTagEff_(jetFlavor, (*it).pt(), fabs((*it).eta()));
			  }
			  else
			  {
				  SF_FullSim = fctBTagCalibReaderFullSimLightJets_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FullSim_up = fctBTagCalibReaderFullSimLightJetsUp_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FullSim_down = fctBTagCalibReaderFullSimLightJetsDown_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  
				  SF_FastSim = fctBTagCalibReaderFastSim_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FastSim_up = fctBTagCalibReaderFastSimUp_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FastSim_down = fctBTagCalibReaderFastSimDown_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  
				  SF = SF_FullSim * SF_FastSim;
				  SF_up = SF_FullSim_up * SF_FastSim_up;
				  SF_down = SF_FullSim_down * SF_FastSim_down;
				  
				  eff = fctBTagEff_(jetFlavor, (*it).pt(), fabs((*it).eta()));
			  }
			  
			  SF_err = std::max(fabs(SF_up-SF),fabs(SF_down-SF));
				  
			  // check if jet is btagged
			  bool tagged = false;
			  if (bJets->size() > 0){
				  for (unsigned int i = 0; i < bJets->size(); ++i){
					  if ((*it).pt()==bJets->at(i).pt()){
						  tagged = true;
						  break;
					  }
				  }
			  }
			  
			  if (tagged){
				  P_MC = P_MC * eff;
				  P_Data = P_Data * SF * eff;
				  if (jetFlavor == 5 || jetFlavor == 4) err1 += SF_err/SF;
				  else err3 += SF_err/SF;
			  }
			  else{
				  P_MC = P_MC * (1 - eff);
				  P_Data = P_Data  * (1 - SF * eff);
				  if (jetFlavor == 5 || jetFlavor == 4) err2 += (-eff*SF_err) / (1-eff*SF);
				  else err4 += (-eff*SF_err) / (1-eff*SF);
			  }
		  }
	  }
	  floatEventProperties["bTagWeight"] = P_Data/P_MC;
	  floatEventProperties["bTagWeightErrHeavy"] = (err1+err2)*P_Data/P_MC;
	  floatEventProperties["bTagWeightErrLight"] = (err3+err4)*P_Data/P_MC;
  }
  
  floatEventProperties["bTagWeightPuppi"] = -9999.;
  floatEventProperties["bTagWeightErrHeavyPuppi"] = -9999.;
  floatEventProperties["bTagWeightErrLightPuppi"] = -9999.;
  
  // Calculate btagging weights
  if (genParticles.isValid()){
	  
	  float P_MC = 1.;
	  float P_Data = 1.;
	  float err1 = 0.;
	  float err2 = 0.;
	  float err3 = 0.;
	  float err4 = 0.;
	  
	  for(std::vector<pat::Jet>::const_iterator it = jetsPuppi_->begin(); it != jetsPuppi_->end() ; ++it){
		  if ((*it).pt() >=35.0 && fabs((*it).eta())<2.4){
			  
			  int jetFlavor;
			  float eff;
			  float SF, SF_up, SF_down;
			  float SF_FullSim, SF_FullSim_up, SF_FullSim_down;
			  float SF_FastSim, SF_FastSim_up, SF_FastSim_down;
			  float SF_err;
			  
			  jetFlavor = (*it).hadronFlavour();
			  if (jetFlavor == 5)
			  {
				  SF_FullSim = fctBTagCalibReaderFullSimBJets_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FullSim_up = fctBTagCalibReaderFullSimBJetsUp_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FullSim_down = fctBTagCalibReaderFullSimBJetsDown_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  
				  SF_FastSim = fctBTagCalibReaderFastSim_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FastSim_up = fctBTagCalibReaderFastSimUp_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  SF_FastSim_down = fctBTagCalibReaderFastSimDown_.eval(BTagEntry::FLAV_B, (*it).eta(), (*it).pt());
				  
				  SF = SF_FullSim * SF_FastSim;
				  SF_up = SF_FullSim_up * SF_FastSim_up;
				  SF_down = SF_FullSim_down * SF_FastSim_down;
				  
				  eff = fctBTagEff_(jetFlavor, (*it).pt(), fabs((*it).eta()));
			  }
			  else if (jetFlavor == 4)
			  {
				  SF_FullSim = fctBTagCalibReaderFullSimCJets_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FullSim_up = fctBTagCalibReaderFullSimCJetsUp_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FullSim_down = fctBTagCalibReaderFullSimCJetsDown_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  
				  SF_FastSim = fctBTagCalibReaderFastSim_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FastSim_up = fctBTagCalibReaderFastSimUp_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  SF_FastSim_down = fctBTagCalibReaderFastSimDown_.eval(BTagEntry::FLAV_C, (*it).eta(), (*it).pt());
				  
				  SF = SF_FullSim * SF_FastSim;
				  SF_up = SF_FullSim_up * SF_FastSim_up;
				  SF_down = SF_FullSim_down * SF_FastSim_down;
				  
				  eff = fctBTagEff_(jetFlavor, (*it).pt(), fabs((*it).eta()));
			  }
			  else
			  {
				  SF_FullSim = fctBTagCalibReaderFullSimLightJets_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FullSim_up = fctBTagCalibReaderFullSimLightJetsUp_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FullSim_down = fctBTagCalibReaderFullSimLightJetsDown_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  
				  SF_FastSim = fctBTagCalibReaderFastSim_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FastSim_up = fctBTagCalibReaderFastSimUp_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  SF_FastSim_down = fctBTagCalibReaderFastSimDown_.eval(BTagEntry::FLAV_UDSG, (*it).eta(), (*it).pt());
				  
				  SF = SF_FullSim * SF_FastSim;
				  SF_up = SF_FullSim_up * SF_FastSim_up;
				  SF_down = SF_FullSim_down * SF_FastSim_down;
				  
				  eff = fctBTagEff_(jetFlavor, (*it).pt(), fabs((*it).eta()));
			  }
			  
			  SF_err = std::max(fabs(SF_up-SF),fabs(SF_down-SF));
				  
			  // check if jet is btagged
			  bool tagged = false;
			  if (bJetsPuppi->size() > 0){
				  for (unsigned int i = 0; i < bJetsPuppi->size(); ++i){
					  if ((*it).pt()==bJetsPuppi->at(i).pt()){
						  tagged = true;
						  break;
					  }
				  }
			  }
			  
			  if (tagged){
				  P_MC = P_MC * eff;
				  P_Data = P_Data * SF * eff;
				  if (jetFlavor == 5 || jetFlavor == 4) err1 += SF_err/SF;
				  else err3 += SF_err/SF;
			  }
			  else{
				  P_MC = P_MC * (1 - eff);
				  P_Data = P_Data  * (1 - SF * eff);
				  if (jetFlavor == 5 || jetFlavor == 4) err2 += (-eff*SF_err) / (1-eff*SF);
				  else err4 += (-eff*SF_err) / (1-eff*SF);
			  }
		  }
	  }
	  floatEventProperties["bTagWeightPuppi"] = P_Data/P_MC;
	  floatEventProperties["bTagWeightErrHeavyPuppi"] = (err1+err2)*P_Data/P_MC;
	  floatEventProperties["bTagWeightErrLightPuppi"] = (err3+err4)*P_Data/P_MC;
  }
  
  
				  
    	

  floatEventProperties["weight"] = fctVtxWeight_( iEvent );
  floatEventProperties["weightUp"] = fctVtxWeightUp_( iEvent );
  floatEventProperties["weightDown"] = fctVtxWeightDown_( iEvent );
  
  if(useJets2_) {
    edm::Handle< std::vector< pat::Jet > > jets2;
    iEvent.getByLabel(jet2Tag_, jets2);
    intEventProperties["nJets2"] = jets2->size();
    floatEventProperties["ht2"] = 0.0;
    for(std::vector<pat::Jet>::const_iterator it = jets2->begin(); it != jets2->end() ; ++it){
      floatEventProperties["ht2"] += (*it).pt();
    }
  }

  makeCombinations< pat::Electron >("EE", *electrons,*pfCands, iEvent, met,metPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices, Rho, intEventProperties, floatEventProperties,tLorentzVectorEventProperties);
  makeCombinations< pat::Electron, pat::Muon >("EMu", *electrons, *muons,*pfCands, iEvent, met, metPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices, Rho, intEventProperties, floatEventProperties,tLorentzVectorEventProperties);
  makeCombinations< pat::Muon >("MuMu", *muons,*pfCands, iEvent, met, metPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices, Rho, intEventProperties, floatEventProperties,tLorentzVectorEventProperties);
  if(useTaus_){
    makeCombinations< pat::Electron, pat::Tau >("ETau", *electrons, *taus,*pfCands,iEvent, met, metPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices,Rho, intEventProperties, floatEventProperties,tLorentzVectorEventProperties);
    makeCombinations< pat::Muon, pat::Tau>("MuTau", *muons, *taus,*pfCands, iEvent, met, metPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices,Rho, intEventProperties, floatEventProperties,tLorentzVectorEventProperties);
    makeCombinations< pat::Tau >("TauTau", *taus,*pfCands, iEvent, met, metPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices, Rho, intEventProperties, floatEventProperties,tLorentzVectorEventProperties);
  }
  //  if( nMu != 2) std::cout << "-------! "<<nMu<<std::endl;

  delete jecUnc;
  delete shiftedJetsJESUp;
  delete shiftedJetsJESDown;


}

template <class aT, class bT> void 
DiLeptonSystematicTreesFromMiniAOD::makeCombinations ( const std::string &treeName, const std::vector<aT> &a, const std::vector<bT> &b,const std::vector<pat::PackedCandidate>&pfCands, const edm::Event &ev, const pat::MET &patMet, const pat::MET &patMetPuppi, const TLorentzVector &MHT, const TLorentzVector &MHTLoose , const TLorentzVector &MHTPuppi, const TLorentzVector &MHTLoosePuppi , const edm::Handle<reco::VertexCollection> &vertices, const float &rho , const std::map<std::string, int> &intEventProperties, const  std::map<std::string, float> &floatEventProperties, const  std::map<std::string, TLorentzVector> &tLorentzVectorEventProperties)
{
  TLorentzVector met(patMet.px(), patMet.py(), patMet.pz(), patMet.energy());
  TLorentzVector uncorrectedMet2;
  uncorrectedMet2.SetPtEtaPhiE(patMet.uncorPt(), 0,\
                              patMet.uncorPhi(), patMet.uncorPt());
  *(tLorentzVectorBranches_[treeName]["vMet"]) = met;
  for(std::map<std::string, int>::const_iterator it = intEventProperties.begin(); it != intEventProperties.end(); ++it){
    assert(intBranches_[treeName].find((*it).first) != intBranches_[treeName].end());
    *(intBranches_[treeName][(*it).first]) = (*it).second;
  }
  for(std::map<std::string, float>::const_iterator it = floatEventProperties.begin(); it != floatEventProperties.end(); ++it){
    assert(floatBranches_[treeName].find((*it).first) != floatBranches_[treeName].end());
    *(floatBranches_[treeName][(*it).first]) = (*it).second;
  }

 for(std::map<std::string, TLorentzVector>::const_iterator it = tLorentzVectorEventProperties.begin(); it != tLorentzVectorEventProperties.end(); ++it){
    assert(tLorentzVectorBranches_[treeName].find((*it).first) != tLorentzVectorBranches_[treeName].end());
    *(tLorentzVectorBranches_[treeName][(*it).first]) = (*it).second;
  }


  for ( std::vector<edm::ParameterSet>::iterator susyVar_i = susyVars_.begin(); susyVar_i != susyVars_.end(); ++susyVar_i ) {
        edm::InputTag var = susyVar_i->getParameter<edm::InputTag>( "var" );
        std::string type = susyVar_i->getParameter<std::string>( "type" );
        edm::Handle< double > var_;
        ev.getByLabel(var, var_);
        if (type=="float") *(floatBranches_[treeName][convertInputTag(var)]) = float(*var_);
        else if (type=="int") *(intBranches_[treeName][convertInputTag(var)]) = int(*var_);
  }
  //std::cout << std::endl;
  for ( std::vector<edm::InputTag>::iterator pdf_i = pdfs_.begin(); pdf_i != pdfs_.end(); ++pdf_i ) {
     const std::string pdfIdentifier = (*pdf_i).instance();
     edm::Handle<std::vector<double> > weightHandle;
     ev.getByLabel((*pdf_i), weightHandle);
     fillPdfUncert(weightHandle,pdfIdentifier,treeName);
  }
  for( typename std::vector<aT>::const_iterator itA = a.begin(); itA != a.end(); ++itA){
    for( typename std::vector<bT>::const_iterator itB = b.begin(); itB != b.end(); ++itB){
//      std::cout << treeName <<": "<< fakeRates_(*itA) << std::endl;
//      weight *= fakeRates_();
      fillTree<aT,bT>( treeName, *itA, *itB,pfCands, patMet, patMetPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices,rho); 
    }
  }
}

template <class aT> void 
DiLeptonSystematicTreesFromMiniAOD::makeCombinations ( const std::string &treeName, const std::vector<aT> &a,const std::vector<pat::PackedCandidate>&pfCands,const edm::Event &ev, const pat::MET &patMet , const pat::MET &patMetPuppi , const TLorentzVector &MHT, const TLorentzVector &MHTLoose, const TLorentzVector &MHTPuppi, const TLorentzVector &MHTLoosePuppi, const edm::Handle<reco::VertexCollection> &vertices, const float &rho , const std::map<std::string, int> &intEventProperties, const  std::map<std::string, float> &floatEventProperties, const  std::map<std::string, TLorentzVector> &tLorentzVectorEventProperties)
{
  TLorentzVector met(patMet.px(), patMet.py(), patMet.pz(), patMet.energy());
  TLorentzVector uncorrectedMet;
  uncorrectedMet.SetPtEtaPhiE(patMet.uncorPt(), 0,\
                              patMet.uncorPhi(), patMet.uncorPt());
  *(tLorentzVectorBranches_[treeName]["vMet"]) = met;
  for(std::map<std::string, int>::const_iterator it = intEventProperties.begin(); it != intEventProperties.end(); ++it){
    assert(intBranches_[treeName].find((*it).first) != intBranches_[treeName].end());
    *(intBranches_[treeName][(*it).first]) = (*it).second;  
  }
  for(std::map<std::string, float>::const_iterator it = floatEventProperties.begin(); it != floatEventProperties.end(); ++it){
    assert(floatBranches_[treeName].find((*it).first) != floatBranches_[treeName].end());
    *(floatBranches_[treeName][(*it).first]) = (*it).second;
  }

 for(std::map<std::string, TLorentzVector>::const_iterator it = tLorentzVectorEventProperties.begin(); it != tLorentzVectorEventProperties.end(); ++it){
    assert(tLorentzVectorBranches_[treeName].find((*it).first) != tLorentzVectorBranches_[treeName].end());
    *(tLorentzVectorBranches_[treeName][(*it).first]) = (*it).second;
  }

  for ( std::vector<edm::ParameterSet>::iterator susyVar_i = susyVars_.begin(); susyVar_i != susyVars_.end(); ++susyVar_i ) {
        edm::InputTag var = susyVar_i->getParameter<edm::InputTag>( "var" );
        std::string type = susyVar_i->getParameter<std::string>( "type" );
        edm::Handle< double > var_;
        ev.getByLabel(var, var_);
        if (type=="int") *(intBranches_[treeName][convertInputTag(var)]) = int(*var_);
        else if (type=="float") *(floatBranches_[treeName][convertInputTag(var)]) = float(*var_);
  }
  for ( std::vector<edm::InputTag>::iterator pdf_i = pdfs_.begin(); pdf_i != pdfs_.end(); ++pdf_i ) {
     std::string pdfIdentifier = (*pdf_i).instance();
     edm::Handle<std::vector<double> > weightHandle;
     ev.getByLabel((*pdf_i), weightHandle);
     fillPdfUncert(weightHandle,pdfIdentifier,treeName);
  }

  for( typename std::vector<aT>::const_iterator itA = a.begin(); itA != a.end(); ++itA){
    for( typename std::vector<aT>::const_iterator itB = a.begin(); itB != itA; ++itB){
      //std::cout << treeName<<"("<<(*itA).pt()<<", "<<(*itA).eta() <<"): "<< fakeRates_(*itA) << std::endl;
//      weight *= fakeRates_();
      fillTree<aT, aT>( treeName, *itA, *itB,pfCands, patMet, patMetPuppi,MHT,MHTLoose,MHTPuppi,MHTLoosePuppi,vertices,rho); 
    }
  }
}

template <class aT, class bT> void 
DiLeptonSystematicTreesFromMiniAOD::fillTree( const std::string &treeName, const aT& a, const bT& b,const std::vector<pat::PackedCandidate>&pfCands, const pat::MET &patMet,  const pat::MET &patMetPuppi, const TLorentzVector &MHT, const TLorentzVector &MHTLoose, const TLorentzVector &MHTPuppi, const TLorentzVector &MHTLoosePuppi, const edm::Handle<reco::VertexCollection> &vertices,const float &rho)
{
  if(debug) std::cout << treeName << "- pts:"<< a.pt() << " " << b.pt();
  TLorentzVector aVec = getMomentum(a);//( a.px(), a.py(), a.pz(), a.energy() );
  TLorentzVector bVec = getMomentum(b); //( b.px(), b.py(), b.pz(), b.energy() );
  TLorentzVector met(patMet.px(), patMet.py(), patMet.pz(), patMet.energy());
  TLorentzVector metPuppi(patMetPuppi.px(), patMetPuppi.py(), patMetPuppi.pz(), patMetPuppi.energy());
  TLorentzVector uncorrectedMet; 
  uncorrectedMet.SetPtEtaPhiE(patMet.uncorPt(), 0, \
			      patMet.uncorPhi(), patMet.uncorPt());  

  //  std::cout << "met: "<<met.Et()<< ", unCorr met: "<< uncorrectedMet.Et()
  //<< "=> "<< met.Et()* 1./uncorrectedMet.Et()<< " (xCheck: "<< patMet.corSumEt()*1./patMet.uncorrectedPt(pat::MET::uncorrALL) <<")"<<std::endl;
  double r_iso_min = 0.05;
  double r_iso_max = 0.2;
  double kt_scale = 10;


  TLorentzVector comb = aVec+bVec;
  TLorentzVector MHT2 = comb + MHT;
  TLorentzVector MHT2Loose = comb + MHTLoose;
  TLorentzVector MHT2Puppi = comb + MHTPuppi;
  TLorentzVector MHT2LoosePuppi = comb + MHTLoosePuppi;
  
  //~ std::pair<double, double> pZeta = calcPZeta(aVec, bVec, met);
  *(floatBranches_[treeName]["chargeProduct"]) = a.charge()*b.charge();
  *(tLorentzVectorBranches_[treeName]["p4"]) = comb;
  *(tLorentzVectorBranches_[treeName]["lepton1"]) = aVec;
  *(tLorentzVectorBranches_[treeName]["lepton2"]) = bVec;
  *(tLorentzVectorBranches_[treeName]["vMHT"]) = MHT2;
  *(tLorentzVectorBranches_[treeName]["vMHTLoose"]) = MHT2Loose;
    *(floatBranches_[treeName]["mht"]) = MHT2.Pt();
  *(tLorentzVectorBranches_[treeName]["vMHTPuppi"]) = MHT2Puppi;
  *(tLorentzVectorBranches_[treeName]["vMHTLoosePuppi"]) = MHT2LoosePuppi;
    *(floatBranches_[treeName]["mhtPuppi"]) = MHT2Puppi.Pt();
  *(floatBranches_[treeName]["pt1"]) = aVec.Pt();
  *(floatBranches_[treeName]["pt2"]) = bVec.Pt();
  *(floatBranches_[treeName]["miniIsoConeSize1"]) = max(r_iso_min,min(r_iso_max, kt_scale/aVec.Pt()));
  *(floatBranches_[treeName]["miniIsoConeSize2"]) = max(r_iso_min,min(r_iso_max, kt_scale/bVec.Pt()));  
  *(floatBranches_[treeName]["charge1"]) = a.charge();
  *(floatBranches_[treeName]["charge2"]) = b.charge();
  *(floatBranches_[treeName]["eta1"]) = aVec.Eta();
  *(floatBranches_[treeName]["eta2"]) = bVec.Eta();
  *(floatBranches_[treeName]["leptonFullSimScaleFactor1"]) = fctLeptonFullSimScaleFactors_(a, a.pt(), fabs(a.eta()), *(intBranches_[treeName]["nVertices"]) );
  *(floatBranches_[treeName]["leptonFullSimScaleFactor2"]) = fctLeptonFullSimScaleFactors_(b, b.pt(), fabs(b.eta()), *(intBranches_[treeName]["nVertices"]) );
  *(floatBranches_[treeName]["leptonFastSimScaleFactor1"]) = fctLeptonFastSimScaleFactors_(a, a.pt(), fabs(a.eta()), *(intBranches_[treeName]["nVertices"]) );
  *(floatBranches_[treeName]["leptonFastSimScaleFactor2"]) = fctLeptonFastSimScaleFactors_(b, b.pt(), fabs(b.eta()), *(intBranches_[treeName]["nVertices"]) );
  *(floatBranches_[treeName]["isoEffArea1"]) = getIso(a,"effectiveArea");
  *(floatBranches_[treeName]["isoEffArea2"]) = getIso(b,"effectiveArea");
  *(floatBranches_[treeName]["isoDeltaBeta1"]) = getIso(a,"deltaBeta");
  *(floatBranches_[treeName]["isoDeltaBeta2"]) = getIso(b,"deltaBeta");
  *(floatBranches_[treeName]["miniIsoEffArea1"]) = getIso(a,"miniIsoEA");
  *(floatBranches_[treeName]["miniIsoEffArea2"]) = getIso(b,"miniIsoEA");
  *(floatBranches_[treeName]["miniIsoDeltaBeta1"]) = getIso(a,"miniIsoDB");
  *(floatBranches_[treeName]["miniIsoDeltaBeta2"]) = getIso(b,"miniIsoDB");
  *(floatBranches_[treeName]["miniIsoPF1"]) = getIso(a,"miniIsoPFWeight");
  *(floatBranches_[treeName]["miniIsoPF2"]) = getIso(b,"miniIsoPFWeight");
  *(floatBranches_[treeName]["dB1"]) = getDeltaB(a);
  *(floatBranches_[treeName]["dB2"]) = getDeltaB(b);
  *(floatBranches_[treeName]["mt1"]) = transverseMass(aVec, met);
  *(floatBranches_[treeName]["mt2"]) = transverseMass(bVec, met);
  *(floatBranches_[treeName]["fakeWeight1"]) = fakeRates_(a);
  *(floatBranches_[treeName]["fakeWeight2"]) = fakeRates_(b);
  *(floatBranches_[treeName]["deltaPhi"]) = aVec.DeltaPhi( bVec );
  *(floatBranches_[treeName]["deltaR"]) = aVec.DeltaR( bVec );
  *(floatBranches_[treeName]["jzb"]) = (met+comb).Pt() - comb.Pt();
  *(floatBranches_[treeName]["jzbUncorr"]) = (uncorrectedMet+comb).Pt() - comb.Pt();  
  *(floatBranches_[treeName]["jzbPuppi"]) = (metPuppi+comb).Pt() - comb.Pt();  
  *(floatBranches_[treeName]["jzbResponse"]) = (met+comb).Pt() / comb.Pt();
  *(floatBranches_[treeName]["jzbResponseUncorr"]) = (uncorrectedMet+comb).Pt() / comb.Pt();    
  *(floatBranches_[treeName]["jzbResponsePuppi"]) = (metPuppi+comb).Pt() / comb.Pt();    
  //~ *(floatBranches_[treeName]["pZeta"]) = pZeta.first;
  //~ *(floatBranches_[treeName]["pZetaVis"]) = pZeta.second;

  if (jets_->size() >= 2){
    TLorentzVector vJet1 = TLorentzVector(jets_->at(0).p4().x(), jets_->at(0).p4().y(), jets_->at(0).p4().z(), jets_->at(0).p4().t());
    TLorentzVector vJet2 = TLorentzVector(jets_->at(1).p4().x(), jets_->at(1).p4().y(), jets_->at(1).p4().z(), jets_->at(1).p4().t());
    TLorentzVector sub = aVec + bVec + vJet1 + vJet2;
    *(floatBranches_[treeName]["sqrts"]) = std::sqrt(TMath::Power((std::sqrt(sub.M2() + sub.Perp2()) + met.Et()), 2.0) - (sub.Vect().XYvector() + met.Vect().XYvector())*(sub.Vect().XYvector() + met.Vect().XYvector()));
  } else {
    *(floatBranches_[treeName]["sqrts"]) = -1.0;
  }
  
  fillLeptonIDs(treeName, a , b , vertices );

  if(debug) std::cout << "dB1: "<< *(floatBranches_[treeName]["dB1"]) 
		      << "dB2: "<< *(floatBranches_[treeName]["dB2"])<< std::endl;

  int matched = 0;
  //ETH style genMatching
  *(intBranches_[treeName]["pdgId1"]) = -9999;
  *(intBranches_[treeName]["pdgId2"]) = -9999;  
  *(intBranches_[treeName]["motherPdgId1"]) =-9999;
  *(intBranches_[treeName]["motherPdgId2"]) =-9999;  
  *(intBranches_[treeName]["grandMotherPdgId1"]) = -9999;
  *(intBranches_[treeName]["grandMotherPdgId2"]) = -9999; 
  *(intBranches_[treeName]["isPrompt1"]) = 0;
  *(intBranches_[treeName]["isPrompt2"]) = 0;
  *(intBranches_[treeName]["isFromTau1"]) = 0;
  *(intBranches_[treeName]["isFromTau2"]) = 0;
  *(intBranches_[treeName]["isPromptHardProcess1"]) = 0;
  *(intBranches_[treeName]["isPromptHardProcess2"]) = 0;  
  *(intBranches_[treeName]["isFromTauHardProcess1"]) = 0;
  *(intBranches_[treeName]["isFromTauHardProcess2"]) = 0;          
  std::vector<int> pdgIds1 = getPdgId_.operator()<aT>(a); 
  std::vector<int> pdgIds2 = getPdgId_.operator()<bT>(b); 	
  *(intBranches_[treeName]["pdgId1"]) = pdgIds1[0];
  *(intBranches_[treeName]["pdgId2"]) = pdgIds2[0];  
  *(intBranches_[treeName]["motherPdgId1"]) = pdgIds1[1];
  *(intBranches_[treeName]["motherPdgId2"]) = pdgIds2[1];  
  *(intBranches_[treeName]["grandMotherPdgId1"]) = pdgIds1[2];
  *(intBranches_[treeName]["grandMotherPdgId2"]) = pdgIds2[2]; 
  *(intBranches_[treeName]["isPrompt1"]) = pdgIds1[3];
  *(intBranches_[treeName]["isPrompt2"]) = pdgIds2[3];
  *(intBranches_[treeName]["isFromTau1"]) = pdgIds1[4];
  *(intBranches_[treeName]["isFromTau2"]) = pdgIds2[4];
  *(intBranches_[treeName]["isPromptHardProcess1"]) = pdgIds1[5];
  *(intBranches_[treeName]["isPromptHardProcess2"]) = pdgIds2[5]; 
  *(intBranches_[treeName]["isFromTauHardProcess1"]) = pdgIds1[6];
  *(intBranches_[treeName]["isFromTauHardProcess2"]) = pdgIds2[6];

  TLorentzVector genVec( 0., 0., 0., 0. );
  if(a.genLepton() != NULL){
    matched |= 1;
  }
  if(b.genLepton() != NULL){
    matched |= 2;
  }
  TLorentzVector genLepton1;
  TLorentzVector genLepton2;
  if(a.genLepton() != NULL && b.genLepton() != NULL){
      genLepton1.SetPxPyPzE(a.genLepton()->px(),a.genLepton()->py(),a.genLepton()->pz(),a.genLepton()->energy());
      genLepton2.SetPxPyPzE(b.genLepton()->px(),b.genLepton()->py(),b.genLepton()->pz(),b.genLepton()->energy());

      genVec.SetPxPyPzE(a.genLepton()->px()+b.genLepton()->px(),a.genLepton()->py()+b.genLepton()->py(),a.genLepton()->pz()+b.genLepton()->pz(),a.genLepton()->energy()+b.genLepton()->energy());
  }
  if( matched == 3 && pdgIds1[1] == pdgIds2[1] ) {
    matched |= 4;
  }
  *(intBranches_[treeName]["matched"]) = matched; 
  *(tLorentzVectorBranches_[treeName]["p4Gen"]) = genVec;
  *(tLorentzVectorBranches_[treeName]["genLepton1"]) = genLepton1;
  *(tLorentzVectorBranches_[treeName]["genLepton2"]) = genLepton2;
  if(debug) std::cout << ", matched = "<<matched<<", motherId = "<<pdgIds1[1];
  if(debug) std::cout<<", M = "<< comb.M() <<", chargeProduct = "<< a.charge()*b.charge() <<std::endl;
  
  
  if (triggerMatches_){

		std::map<string,int> triggerMatches1 = fctTrigger_.operator()<aT>(a);   
 		std::map<string,int> triggerMatches2 = fctTrigger_.operator()<bT>(b);  

		*(intBranches_[treeName]["matchesSingleElectron1"]) = triggerMatches1["matchesSingleElectron"];
		*(intBranches_[treeName]["matchesSingleMuon1"]) = triggerMatches1["matchesSingleMuon"];
		*(intBranches_[treeName]["matchesDoubleElectronLeading1"]) = triggerMatches1["matchesDoubleElectronLeading"];
		*(intBranches_[treeName]["matchesDoubleElectronTrailing1"]) = triggerMatches1["matchesDoubleElectronTrailing"];	
		*(intBranches_[treeName]["matchesDoubleMuonLeading1"]) = triggerMatches1["matchesDoubleMuonLeading"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailing1"]) = triggerMatches1["matchesDoubleMuonTrailing"];
		*(intBranches_[treeName]["matchesDoubleMuonLeadingBoth1"]) = triggerMatches1["matchesDoubleMuonLeadingBoth"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailingBoth1"]) = triggerMatches1["matchesDoubleMuonTrailingBoth"];		
		*(intBranches_[treeName]["matchesDoubleMuonLeadingTk1"]) = triggerMatches1["matchesDoubleMuonLeadingTk"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailingTk1"]) = triggerMatches1["matchesDoubleMuonTrailingTk"];
		*(intBranches_[treeName]["matchesMuELeading1"]) = triggerMatches1["matchesMuELeading"];
		*(intBranches_[treeName]["matchesMuETrailing1"]) = triggerMatches1["matchesMuETrailing"];
		*(intBranches_[treeName]["matchesEMuLeading1"]) = triggerMatches1["matchesMuETrailing"];
		*(intBranches_[treeName]["matchesEMuTrailing1"]) = triggerMatches1["matchesEMuTrailing"];
		
		*(intBranches_[treeName]["matchesDoubleElectronLeadingNonIso1"]) = triggerMatches1["matchesDoubleElectronLeadingNonIso"];
		*(intBranches_[treeName]["matchesDoubleElectronTrailingNonIso1"]) = triggerMatches1["matchesDoubleElectronTrailingNonIso"];			
		*(intBranches_[treeName]["matchesDoubleMuonLeadingNonIso1"]) = triggerMatches1["matchesDoubleMuonLeadingNonIso"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailingNonIso1"]) = triggerMatches1["matchesDoubleMuonTrailingNonIso"];		
		*(intBranches_[treeName]["matchesMuEGElectronNonIso1"]) = triggerMatches1["matchesMuEGElectronNonIso"];
		*(intBranches_[treeName]["matchesMuEGMuonNonIso1"]) = triggerMatches1["matchesMuEGMuonNonIso"];
		
		*(intBranches_[treeName]["matchesSingleElectron2"]) = triggerMatches2["matchesSingleElectron"];
		*(intBranches_[treeName]["matchesSingleMuon2"]) = triggerMatches2["matchesSingleMuon"];
		*(intBranches_[treeName]["matchesDoubleElectronLeading2"]) = triggerMatches2["matchesDoubleElectronLeading"];
		*(intBranches_[treeName]["matchesDoubleElectronTrailing2"]) = triggerMatches2["matchesDoubleElectronTrailing"];		
		*(intBranches_[treeName]["matchesDoubleMuonLeading2"]) = triggerMatches2["matchesDoubleMuonLeading"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailing2"]) = triggerMatches2["matchesDoubleMuonTrailing"];
		*(intBranches_[treeName]["matchesDoubleMuonLeadingBoth2"]) = triggerMatches2["matchesDoubleMuonLeadingBoth"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailingBoth2"]) = triggerMatches2["matchesDoubleMuonTrailingBoth"];		
		*(intBranches_[treeName]["matchesDoubleMuonLeadingTk2"]) = triggerMatches2["matchesDoubleMuonLeadingTk"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailingTk2"]) = triggerMatches2["matchesDoubleMuonTrailingTk"];
		*(intBranches_[treeName]["matchesMuELeading2"]) = triggerMatches2["matchesMuELeading"];
		*(intBranches_[treeName]["matchesMuETrailing2"]) = triggerMatches2["matchesMuETrailing"];
		*(intBranches_[treeName]["matchesEMuLeading2"]) = triggerMatches2["matchesEMuLeading"];
		*(intBranches_[treeName]["matchesEMuTrailing2"]) = triggerMatches2["matchesEMuTrailing"];

 		*(intBranches_[treeName]["matchesDoubleElectronLeadingNonIso2"]) = triggerMatches2["matchesDoubleElectronLeadingNonIso"];
		*(intBranches_[treeName]["matchesDoubleElectronTrailingNonIso2"]) = triggerMatches2["matchesDoubleElectronTrailingNonIso"];			
		*(intBranches_[treeName]["matchesDoubleMuonLeadingNonIso2"]) = triggerMatches2["matchesDoubleMuonLeadingNonIso"];
		*(intBranches_[treeName]["matchesDoubleMuonTrailingNonIso2"]) = triggerMatches2["matchesDoubleMuonTrailingNonIso"];		
		*(intBranches_[treeName]["matchesMuEGElectronNonIso2"]) = triggerMatches2["matchesMuEGElectronNonIso"];
		*(intBranches_[treeName]["matchesMuEGMuonNonIso2"]) = triggerMatches2["matchesMuEGMuonNonIso"]; 
  }

  trees_[treeName]->Fill();
}



//from DQM/Physics/src/EwkTauDQM.cc
//~ std::pair<double, double>
//~ DiLeptonSystematicTreesFromMiniAOD::calcPZeta(const TLorentzVector& p1,const TLorentzVector& p2, const TLorentzVector& met)
//~ {
	//~ double cosPhi1 = cos(p1.Phi());
	//~ double sinPhi1 = sin(p1.Phi());
	//~ double cosPhi2 = cos(p2.Phi());
	//~ double sinPhi2 = sin(p2.Phi());
	//~ double zetaX = cosPhi1 + cosPhi2;
	//~ double zetaY = sinPhi1 + sinPhi2;
	//~ double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
	//~ if ( zetaR > 0. ) {
		//~ zetaX /= zetaR;
		//~ zetaY /= zetaR;
	//~ }
//~ 
	//~ double pxVis = p1.Px() + p2.Px();
	//~ double pyVis = p1.Py() + p2.Py();
	//~ double pZetaVis = pxVis*zetaX + pyVis*zetaY;
//~ 
	//~ double px = pxVis + met.Px();
	//~ double py = pyVis + met.Py();
	//~ double pZeta = px*zetaX + py*zetaY;
//~ 
	//~ return std::pair<double, double>(pZeta, pZetaVis);
//~ }

void DiLeptonSystematicTreesFromMiniAOD::fillPdfUncert(const edm::Handle< std::vector<double> >& weightHandle, const std::string& pdfIdentifier, const std::string& treeName){
     std::string up = "Up";
     std::string down = "Down";
     bool nnpdfFlag = (pdfIdentifier.substr(0,5)=="NNPDF");
     double centralValue = (*weightHandle)[0];
     *(floatBranches_[treeName][pdfIdentifier]) = float(centralValue);
     if(debug) std::cout << "Cen" << treeName << ": " << centralValue << std::endl;
     unsigned int nmembers = weightHandle->size();
     double wminus = 0.;
     double wplus = 0.;
     unsigned int nplus = 0;
     unsigned int nminus = 0;
     for (unsigned int j=1; j<nmembers; j+=2) {
        float wa = ((*weightHandle)[j]-centralValue)/centralValue;
        float wb = ((*weightHandle)[j+1]-centralValue)/centralValue;
	if (nnpdfFlag) {
	  if (wa>0.) {
	    wplus += wa*wa; 
	    nplus++;
	  } else {
	    wminus += wa*wa;
	    nminus++;
	  }
	  if (wb>0.) {
	    wplus += wb*wb; 
	    nplus++;
	  } else {
	    wminus += wb*wb;
	    nminus++;
	  }
	} else {
	  
	  if (wa>wb) {
            if (wa<0.) wa = 0.;
            if (wb>0.) wb = 0.;
            wplus += wa*wa;
            wminus += wb*wb;
	  } else {
            if (wb<0.) wb = 0.;
            if (wa>0.) wa = 0.;
            wplus += wb*wb;
            wminus += wa*wa;
	  }
	}
    }
    if (wplus>0.) wplus = sqrt(wplus);
    if (wminus>0.) wminus = sqrt(wminus);
    if (nnpdfFlag) {
      if (nplus>0) wplus /= sqrt(nplus);
      if (nminus>0) wminus /= sqrt(nminus);
    }
    *(floatBranches_[treeName][pdfIdentifier+down]) = float(wminus);
    *(floatBranches_[treeName][pdfIdentifier+up]) = float(wplus);
}

const TLorentzVector DiLeptonSystematicTreesFromMiniAOD::getMomentum(const  pat::Electron &e)
{
  double corr = 1.;
  double lowEdge = 0.;
  for(std::map<double, double>::iterator it = electronCorrections_.begin(); 
      it != electronCorrections_.end(); ++it){
    if(lowEdge <= fabs(e.superCluster()->eta()) && fabs(e.superCluster()->eta()) < (*it).first ){
      corr = (*it).second;
    }
    lowEdge = (*it).second;
  }

  const TLorentzVector result = TLorentzVector(corr*e.px(), corr*e.py(), corr*e.pz(), corr*e.energy());
  if(debug)std::cout << "correction: "<< corr << ", pt = "<< result.Pt()<<std::endl;
  return result;
}

const TLorentzVector DiLeptonSystematicTreesFromMiniAOD::getMomentum(const  pat::Muon &mu)
{
  const TLorentzVector result = TLorentzVector(mu.px(), mu.py(), mu.pz(), mu.energy());
  return result;
}

const TLorentzVector DiLeptonSystematicTreesFromMiniAOD::getMomentum(const  pat::Tau &tau)
{
  const TLorentzVector result = TLorentzVector(tau.px(), tau.py(), tau.pz(), tau.energy());
  return result;
}

float DiLeptonSystematicTreesFromMiniAOD::getIso(const  pat::Electron &e, const std::string &method)
{
  //  if (e.isEE())
  //  return (e.dr03HcalTowerSumEt() + e.dr03EcalRecHitSumEt() + e.dr03TkSumPt())/e.pt();
  // else
  //  return (e.dr03HcalTowerSumEt() + std::max(0.0, e.dr03EcalRecHitSumEt() - 1.0) + e.dr03TkSumPt())/e.pt();

  //  std::cout<<"electron " << (lepton.trackIso()+lepton.ecalIso()+lepton.hcalIso())/lepton.pt() << std::endl;
  // return (lepton.trackIso()+lepton.ecalIso()+lepton.hcalIso())/lepton.pt() ;
    //  return (e.chargedHadronIso() + e.photonIso() + e.neutralHadronIso()) / e.pt();
  return fctIsolation_(e,method)* 1./e.pt();
}

float DiLeptonSystematicTreesFromMiniAOD::getIso(const  pat::Muon &mu, const std::string &method)
{
  //  std::cout<<"muon " << (lepton.trackIso()+lepton.ecalIso()+lepton.hcalIso())/lepton.pt() << std::endl;
  //  return (mu.isolationR03().hadEt + mu.isolationR03().emEt + mu.isolationR03().sumPt) / mu.pt();
  //  return (mu.chargedHadronIso() + mu.photonIso() + mu.neutralHadronIso()) / mu.pt();
  return fctIsolation_(mu,method)* 1./mu.pt();
}

float DiLeptonSystematicTreesFromMiniAOD::getIso(const  pat::Tau &tau, const std::string &method)
{
  float result = fctIsolation_(tau,method);
  if(tau.tauID(tauId_) < 0.5)
    result *= -1.0;
  return result;
}


void DiLeptonSystematicTreesFromMiniAOD::fillLeptonIDs(const std::string &treeName, const  pat::Electron &ele1, const  pat::Electron &ele2, const edm::Handle<reco::VertexCollection> &vertices)
{

  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffEle(ele1.eta());
  *(floatBranches_[treeName]["chargedIso1"]) = ele1.pfIsolationVariables().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso1"]) = ele1.pfIsolationVariables().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso1"]) = ele1.pfIsolationVariables().sumPhotonEt;
  *(floatBranches_[treeName]["puIso1"]) = ele1.pfIsolationVariables().sumPUPt;

  *(floatBranches_[treeName]["effectiveArea2"]) = getAEffEle(ele2.eta());
  *(floatBranches_[treeName]["chargedIso2"]) = ele2.pfIsolationVariables().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso2"]) = ele2.pfIsolationVariables().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso2"]) = ele2.pfIsolationVariables().sumPhotonEt;
  *(floatBranches_[treeName]["puIso2"]) = ele2.pfIsolationVariables().sumPUPt;
  
  if (writeID_){
  
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx1"]) = ele1.deltaEtaSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx1"]) = ele1.deltaPhiSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["sigmaIetaIeta1"]) = ele1.sigmaIetaIeta();
  *(floatBranches_[treeName]["hadronicOverEm1"]) = ele1.hadronicOverEm();
  *(floatBranches_[treeName]["eOverP1"]) = abs(1.0/ele1.ecalEnergy() - ele1.eSuperClusterOverP()/ele1.ecalEnergy());
  *(floatBranches_[treeName]["missingHits1"]) = ele1.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffEle(ele1.eta());
  *(floatBranches_[treeName]["passConversion1"]) = ele1.passConversionVeto();
  *(floatBranches_[treeName]["d01"]) = ele1.gsfTrack()->dxy(vertices->at(0).position());
  *(floatBranches_[treeName]["dZ1"]) = fabs(ele1.gsfTrack()->dz(vertices->at(0).position())); 

  *(floatBranches_[treeName]["globalMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon1"]) = -999.;
  *(floatBranches_[treeName]["pfMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackChi21"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations1"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits1"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = -999.;

  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx2"]) = ele2.deltaEtaSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx2"]) = ele2.deltaPhiSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["sigmaIetaIeta2"]) = ele2.sigmaIetaIeta();
  *(floatBranches_[treeName]["hadronicOverEm2"]) = ele2.hadronicOverEm();
  *(floatBranches_[treeName]["eOverP2"]) = abs(1.0/ele2.ecalEnergy() - ele2.eSuperClusterOverP()/ele2.ecalEnergy());
  *(floatBranches_[treeName]["missingHits2"]) = ele2.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  *(floatBranches_[treeName]["effectiveArea2"]) = getAEffEle(ele2.eta());
  *(floatBranches_[treeName]["passConversion2"]) = ele2.passConversionVeto();
  *(floatBranches_[treeName]["d02"]) = ele2.gsfTrack()->dxy(vertices->at(0).position());
  *(floatBranches_[treeName]["dZ2"]) = fabs(ele2.gsfTrack()->dz(vertices->at(0).position()));
  
   *(floatBranches_[treeName]["globalMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon2"]) = -999.;
  *(floatBranches_[treeName]["pfMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackChi22"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations2"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits2"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = -999.; 
  
  }

}


void DiLeptonSystematicTreesFromMiniAOD::fillLeptonIDs(const std::string &treeName, const  pat::Electron &ele1, const  pat::Tau &tau2, const edm::Handle<reco::VertexCollection> &vertices)
{

  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffEle(ele1.eta());
  *(floatBranches_[treeName]["chargedIso1"]) = ele1.pfIsolationVariables().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso1"]) = ele1.pfIsolationVariables().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso1"]) = ele1.pfIsolationVariables().sumPhotonEt;
  *(floatBranches_[treeName]["puIso1"]) = ele1.pfIsolationVariables().sumPUPt;

  *(floatBranches_[treeName]["effectiveArea2"]) = -999.;
  *(floatBranches_[treeName]["chargedIso2"]) = -999.;
  *(floatBranches_[treeName]["neutralIso2"]) = -999.;
  *(floatBranches_[treeName]["photonIso2"]) = -999.;
  *(floatBranches_[treeName]["puIso2"]) = -999.;
  
  
  if (writeID_){
  
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx1"]) = ele1.deltaEtaSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx1"]) = ele1.deltaPhiSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["sigmaIetaIeta1"]) = ele1.sigmaIetaIeta();
  *(floatBranches_[treeName]["hadronicOverEm1"]) = ele1.hadronicOverEm();
  *(floatBranches_[treeName]["eOverP1"]) = abs(1.0/ele1.ecalEnergy() - ele1.eSuperClusterOverP()/ele1.ecalEnergy());
  *(floatBranches_[treeName]["missingHits1"]) = ele1.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffEle(ele1.eta());
  *(floatBranches_[treeName]["passConversion1"]) = ele1.passConversionVeto();
  *(floatBranches_[treeName]["d01"]) = ele1.gsfTrack()->dxy(vertices->at(0).position());
  *(floatBranches_[treeName]["dZ1"]) = fabs(ele1.gsfTrack()->dz(vertices->at(0).position())); 
 
   *(floatBranches_[treeName]["globalMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon1"]) = -999.;
  *(floatBranches_[treeName]["pfMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackChi21"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations1"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits1"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = -999.;

  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta2"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm2"]) = -999.;
  *(floatBranches_[treeName]["eOverP2"]) = -999.;
  *(floatBranches_[treeName]["missingHits2"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea2"]) = -999.;
  *(floatBranches_[treeName]["passConversion2"]) = -999.;
  *(floatBranches_[treeName]["d02"]) = -999.;
  *(floatBranches_[treeName]["dZ2"]) = -999.;
  
  
  *(floatBranches_[treeName]["globalMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon2"]) = -999.;
  *(floatBranches_[treeName]["pfMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackChi22"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations2"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits2"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = -999.;
  
  
  }
  

}


void DiLeptonSystematicTreesFromMiniAOD::fillLeptonIDs(const std::string &treeName, const  pat::Tau &tau1, const  pat::Tau &tau2, const edm::Handle<reco::VertexCollection> &vertices)
{

  *(floatBranches_[treeName]["effectiveArea1"]) = -999.;
  *(floatBranches_[treeName]["chargedIso1"]) = -999.;
  *(floatBranches_[treeName]["neutralIso1"]) = -999.;
  *(floatBranches_[treeName]["photonIso1"]) = -999.;
  *(floatBranches_[treeName]["puIso1"]) = -999.;

  *(floatBranches_[treeName]["effectiveArea2"]) = -999.;
  *(floatBranches_[treeName]["chargedIso2"]) = -999.;
  *(floatBranches_[treeName]["neutralIso2"]) = -999.;
  *(floatBranches_[treeName]["photonIso2"]) = -999.;
  *(floatBranches_[treeName]["puIso2"]) = -999.;
  
  if (writeID_){
 
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx1"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx1"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta1"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm1"]) = -999.;
  *(floatBranches_[treeName]["eOverP1"]) = -999.;
  *(floatBranches_[treeName]["missingHits1"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea1"]) = -999.;
  *(floatBranches_[treeName]["passConversion1"]) = -999.;
  *(floatBranches_[treeName]["d01"]) = -999.;
  *(floatBranches_[treeName]["dZ1"]) = -999.; 
 
  *(floatBranches_[treeName]["globalMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon1"]) = -999.;
  *(floatBranches_[treeName]["pfMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackChi21"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations1"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits1"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = -999.;

  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta2"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm2"]) = -999.;
  *(floatBranches_[treeName]["eOverP2"]) = -999.;
  *(floatBranches_[treeName]["missingHits2"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea2"]) = -999.;
  *(floatBranches_[treeName]["passConversion2"]) = -999.;
  *(floatBranches_[treeName]["d02"]) = -999.;
  *(floatBranches_[treeName]["dZ2"]) = -999.;
  
  *(floatBranches_[treeName]["globalMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon2"]) = -999.;
  *(floatBranches_[treeName]["pfMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackChi22"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations2"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits2"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = -999.; 
  
  
  }

}



void DiLeptonSystematicTreesFromMiniAOD::fillLeptonIDs(const std::string &treeName, const  pat::Electron &ele1, const  pat::Muon &mu2, const edm::Handle<reco::VertexCollection> &vertices)
{
	
  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffEle(ele1.eta());
  *(floatBranches_[treeName]["chargedIso1"]) = ele1.pfIsolationVariables().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso1"]) = ele1.pfIsolationVariables().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso1"]) = ele1.pfIsolationVariables().sumPhotonEt;
  *(floatBranches_[treeName]["puIso1"]) = ele1.pfIsolationVariables().sumPUPt;

  *(floatBranches_[treeName]["effectiveArea2"]) = getAEffMu(mu2.eta());
  *(floatBranches_[treeName]["chargedIso2"]) = mu2.pfIsolationR03().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso2"]) = mu2.pfIsolationR03().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso2"]) = mu2.pfIsolationR03().sumPhotonEt;
  *(floatBranches_[treeName]["puIso2"]) = mu2.pfIsolationR03().sumPUPt;
  
  if (writeID_){
  
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx1"]) = ele1.deltaEtaSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx1"]) = ele1.deltaPhiSuperClusterTrackAtVtx();
  *(floatBranches_[treeName]["sigmaIetaIeta1"]) = ele1.sigmaIetaIeta();
  *(floatBranches_[treeName]["hadronicOverEm1"]) = ele1.hadronicOverEm();
  *(floatBranches_[treeName]["eOverP1"]) = abs(1.0/ele1.ecalEnergy() - ele1.eSuperClusterOverP()/ele1.ecalEnergy());
  *(floatBranches_[treeName]["missingHits1"]) = ele1.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffEle(ele1.eta());
  *(floatBranches_[treeName]["passConversion1"]) = ele1.passConversionVeto();
  *(floatBranches_[treeName]["d01"]) = ele1.gsfTrack()->dxy(vertices->at(0).position());
  *(floatBranches_[treeName]["dZ1"]) = fabs(ele1.gsfTrack()->dz(vertices->at(0).position())); 
 
  *(floatBranches_[treeName]["globalMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon1"]) = -999.;
  *(floatBranches_[treeName]["pfMuon1"]) = -999.;
  *(floatBranches_[treeName]["trackChi21"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations1"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits1"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = -999.;
  
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta2"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm2"]) = -999.;
  *(floatBranches_[treeName]["eOverP2"]) = -999.;
  *(floatBranches_[treeName]["missingHits2"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea2"]) = getAEffMu(mu2.eta());
  *(floatBranches_[treeName]["passConversion2"]) = -999.;
  
  *(floatBranches_[treeName]["d02"]) = mu2.dB();
  *(floatBranches_[treeName]["dZ2"]) = abs(mu2.muonBestTrack()->dz(vertices->at(0).position())); 
   if (mu2.isGlobalMuon()){
	  *(floatBranches_[treeName]["trackChi22"]) = mu2.globalTrack()->normalizedChi2();
	  *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = mu2.globalTrack()->hitPattern().numberOfValidMuonHits();
  }
  else{
	 *(floatBranches_[treeName]["trackChi22"]) = -999.;
         *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = -999.;
  }
  *(floatBranches_[treeName]["numberOfMatchedStations2"]) =mu2.numberOfMatchedStations();
  if (mu2.isTrackerMuon()){
  	*(floatBranches_[treeName]["numberOfValidPixelHits2"]) = mu2.innerTrack()->hitPattern().numberOfValidPixelHits();
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = mu2.track()->hitPattern().trackerLayersWithMeasurement();
  }
  else{
  	*(floatBranches_[treeName]["numberOfValidPixelHits2"]) = -999.;
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = -999.;
  }    
  
  }

}

void DiLeptonSystematicTreesFromMiniAOD::fillLeptonIDs(const std::string &treeName, const  pat::Muon &mu1, const  pat::Muon &mu2, const edm::Handle<reco::VertexCollection> &vertices)
{

  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffMu(mu1.eta());
  *(floatBranches_[treeName]["chargedIso1"]) = mu1.pfIsolationR03().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso1"]) = mu1.pfIsolationR03().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso1"]) = mu1.pfIsolationR03().sumPhotonEt;
  *(floatBranches_[treeName]["puIso1"]) = mu1.pfIsolationR03().sumPUPt;

  
  *(floatBranches_[treeName]["effectiveArea2"]) = getAEffMu(mu2.eta());
  *(floatBranches_[treeName]["chargedIso2"]) = mu2.pfIsolationR03().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso2"]) = mu2.pfIsolationR03().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso2"]) = mu2.pfIsolationR03().sumPhotonEt;
  *(floatBranches_[treeName]["puIso2"]) = mu2.pfIsolationR03().sumPUPt;
  
  if (writeID_){
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx1"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx1"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta1"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm1"]) = -999.;
  *(floatBranches_[treeName]["eOverP1"]) = -999.;
  *(floatBranches_[treeName]["missingHits1"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffMu(mu1.eta());
  *(floatBranches_[treeName]["passConversion1"]) = -999.;

  *(floatBranches_[treeName]["d01"]) = mu1.dB();
  *(floatBranches_[treeName]["dZ1"]) = abs(mu1.muonBestTrack()->dz(vertices->at(0).position()));


  *(floatBranches_[treeName]["globalMuon1"]) = mu1.isGlobalMuon();
  *(floatBranches_[treeName]["trackerMuon1"]) = mu1.isTrackerMuon();
  *(floatBranches_[treeName]["pfMuon1"]) = mu1.isPFMuon();
  if (mu1.isGlobalMuon()){
	  *(floatBranches_[treeName]["trackChi21"]) = mu1.globalTrack()->normalizedChi2();
	  *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = mu1.globalTrack()->hitPattern().numberOfValidMuonHits();
  }
  else{
	 *(floatBranches_[treeName]["trackChi21"]) = -999.;
         *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = -999.;
  }
  *(floatBranches_[treeName]["numberOfMatchedStations1"]) =mu1.numberOfMatchedStations();
  if (mu1.isTrackerMuon()){
  	*(floatBranches_[treeName]["numberOfValidPixelHits1"]) = mu1.innerTrack()->hitPattern().numberOfValidPixelHits();
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = mu1.track()->hitPattern().trackerLayersWithMeasurement();
  }
  else{
  	*(floatBranches_[treeName]["numberOfValidPixelHits1"]) = -999.;
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = -999.;
  }

  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta2"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm2"]) = -999.;
  *(floatBranches_[treeName]["eOverP2"]) = -999.;
  *(floatBranches_[treeName]["missingHits2"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea2"]) = getAEffMu(mu2.eta());
  *(floatBranches_[treeName]["passConversion2"]) = -999.;

  *(floatBranches_[treeName]["d02"]) = mu2.dB();
  *(floatBranches_[treeName]["dZ2"]) = abs(mu2.muonBestTrack()->dz(vertices->at(0).position()));



  *(floatBranches_[treeName]["globalMuon2"]) = mu2.isGlobalMuon();
  *(floatBranches_[treeName]["trackerMuon2"]) = mu2.isTrackerMuon();
  *(floatBranches_[treeName]["pfMuon2"]) = mu2.isPFMuon();
  if (mu2.isGlobalMuon()){
	  *(floatBranches_[treeName]["trackChi22"]) = mu2.globalTrack()->normalizedChi2();
	  *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = mu2.globalTrack()->hitPattern().numberOfValidMuonHits();
  }
  else{
	 *(floatBranches_[treeName]["trackChi22"]) = -999.;
         *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = -999.;
  }
  *(floatBranches_[treeName]["numberOfMatchedStations2"]) =mu2.numberOfMatchedStations();
  if (mu2.isTrackerMuon()){
  	*(floatBranches_[treeName]["numberOfValidPixelHits2"]) = mu2.innerTrack()->hitPattern().numberOfValidPixelHits();
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = mu2.track()->hitPattern().trackerLayersWithMeasurement();
  }
  else{
  	*(floatBranches_[treeName]["numberOfValidPixelHits2"]) = -999.;
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = -999.;
  }
  
  
  }


}


void DiLeptonSystematicTreesFromMiniAOD::fillLeptonIDs(const std::string &treeName, const  pat::Muon &mu1, const  pat::Tau &tau2, const edm::Handle<reco::VertexCollection> &vertices)
{

  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffMu(mu1.eta());
  *(floatBranches_[treeName]["chargedIso1"]) = mu1.pfIsolationR03().sumChargedHadronPt;
  *(floatBranches_[treeName]["neutralIso1"]) = mu1.pfIsolationR03().sumNeutralHadronEt;
  *(floatBranches_[treeName]["photonIso1"]) = mu1.pfIsolationR03().sumPhotonEt;
  *(floatBranches_[treeName]["puIso1"]) = mu1.pfIsolationR03().sumPUPt;

  *(floatBranches_[treeName]["effectiveArea2"]) = -999.;
  *(floatBranches_[treeName]["chargedIso2"]) = -999.;
  *(floatBranches_[treeName]["neutralIso2"]) = -999.;
  *(floatBranches_[treeName]["photonIso2"]) = -999.;
  *(floatBranches_[treeName]["puIso2"]) = -999.;

  if (writeID_){
  
  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx1"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx1"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta1"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm1"]) = -999.;
  *(floatBranches_[treeName]["eOverP1"]) = -999.;
  *(floatBranches_[treeName]["missingHits1"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea1"]) = getAEffMu(mu1.eta());
  *(floatBranches_[treeName]["passConversion1"]) = -999.;

  *(floatBranches_[treeName]["d01"]) = mu1.dB();
  *(floatBranches_[treeName]["dZ1"]) = abs(mu1.muonBestTrack()->dz(vertices->at(0).position()));

  *(floatBranches_[treeName]["globalMuon1"]) = mu1.isGlobalMuon();
  *(floatBranches_[treeName]["trackerMuon1"]) = mu1.isTrackerMuon();
  *(floatBranches_[treeName]["pfMuon1"]) = mu1.isPFMuon();
  if (mu1.isGlobalMuon()){
	  *(floatBranches_[treeName]["trackChi21"]) = mu1.globalTrack()->normalizedChi2();
	  *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = mu1.globalTrack()->hitPattern().numberOfValidMuonHits();
  }
  else{
	 *(floatBranches_[treeName]["trackChi21"]) = -999.;
         *(floatBranches_[treeName]["numberOfValidMuonHits1"]) = -999.;
  }
  *(floatBranches_[treeName]["numberOfMatchedStations1"]) =mu1.numberOfMatchedStations();
  if (mu1.isTrackerMuon()){
  	*(floatBranches_[treeName]["numberOfValidPixelHits1"]) = mu1.innerTrack()->hitPattern().numberOfValidPixelHits();
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = mu1.track()->hitPattern().trackerLayersWithMeasurement();
  }
  else{
  	*(floatBranches_[treeName]["numberOfValidPixelHits1"]) = -999.;
  	*(floatBranches_[treeName]["trackerLayersWithMeasurement1"]) = -999.;
  }

  *(floatBranches_[treeName]["deltaEtaSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["deltaPhiSuperClusterTrackAtVtx2"]) = -999.;
  *(floatBranches_[treeName]["sigmaIetaIeta2"]) = -999.;
  *(floatBranches_[treeName]["hadronicOverEm2"]) = -999.;
  *(floatBranches_[treeName]["eOverP2"]) = -999.;
  *(floatBranches_[treeName]["missingHits2"]) = -999.;
  *(floatBranches_[treeName]["effectiveArea2"]) = -999.;
  *(floatBranches_[treeName]["passConversion2"]) = -999.;
  *(floatBranches_[treeName]["d02"]) = -999.;
  *(floatBranches_[treeName]["dZ2"]) = -999.;

  *(floatBranches_[treeName]["globalMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackerMuon2"]) = -999.;
  *(floatBranches_[treeName]["pfMuon2"]) = -999.;
  *(floatBranches_[treeName]["trackChi22"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidMuonHits2"]) = -999.;
  *(floatBranches_[treeName]["numberOfMatchedStations2"]) = -999.;
  *(floatBranches_[treeName]["numberOfValidPixelHits2"]) = -999.;
  *(floatBranches_[treeName]["trackerLayersWithMeasurement2"]) = -999.; 
  
  }

}


float DiLeptonSystematicTreesFromMiniAOD::topPtWeightBen(double topPt){
  if( topPt<0 ) return 1;

  float p0 = 1.18246e+00;
  float p1 = 4.63312e+02;
  float p2 = 2.10061e-06;

  if( topPt>p1 ) topPt = p1;

  float result = p0 + p2 * topPt * ( topPt - 2 * p1 );
  return result;
}

float DiLeptonSystematicTreesFromMiniAOD::topPtWeightTOP(double topPt){
  if( topPt<0 ) return 1;

  float p0 = 0.156;
  float p1 = 0.00137;

  float result = exp(p0 - p1 * topPt);
  return result;
}


float DiLeptonSystematicTreesFromMiniAOD::getDeltaB(const  pat::Electron &e)
{
  float result = e.dB(pat::Electron::PV3D);
  return result;
}

float DiLeptonSystematicTreesFromMiniAOD::getDeltaB(const  pat::Muon &mu)
{
  float result = mu.dB(pat::Muon::PV3D);
  return result;
}

float DiLeptonSystematicTreesFromMiniAOD::getDeltaB(const  pat::Tau &tau)
{
  float result = -1; // not available for pat::Tau could use ip of leading ch. Hadr if needed.
  return result;
}

float DiLeptonSystematicTreesFromMiniAOD::getAEffEle(double eta)
{
    //from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/src/EGammaCutBasedEleId.cc
    // but gamma + neutral hadrons values...
    double etaAbs = fabs(eta);
    double AEff = 0.1013;
    if (etaAbs > 0.8 && etaAbs <= 1.3) AEff = 0.0988;
    if (etaAbs > 1.3 && etaAbs <= 2.0) AEff = 0.0572;
    if (etaAbs > 2.0 && etaAbs <= 2.2) AEff = 0.0842;
    if (etaAbs > 2.2) AEff = 0.153;
    return AEff;
}

float DiLeptonSystematicTreesFromMiniAOD::getAEffMu(double eta)
{
    //from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/src/EGammaCutBasedEleId.cc
    // but gamma + neutral hadrons values...
    double etaAbs = fabs(eta);
    double AEff = 0.0913;
    if (etaAbs > 0.8 && etaAbs <= 1.3) AEff = 0.0765;
    if (etaAbs > 1.3 && etaAbs <= 2.0) AEff = 0.0546;
    if (etaAbs > 2.0 && etaAbs <= 2.2) AEff = 0.0728;
    if (etaAbs > 2.2) AEff = 0.1177;
    return AEff;
}


float DiLeptonSystematicTreesFromMiniAOD::transverseMass(const TLorentzVector& p, const TLorentzVector& met)
{
  reco::Candidate::LorentzVector otherMet(met.Px(),met.Py(),met.Pz(),met.E());
  reco::Candidate::LorentzVector leptonT(p.Px(),p.Py(),0.,p.E()*sin(p.Theta()));
  reco::Candidate::LorentzVector sumT=leptonT+otherMet;

  return std::sqrt(sumT.M2());
}

std::string DiLeptonSystematicTreesFromMiniAOD::convertInputTag(const edm::InputTag tag)
{
  std::string result = tag.label();
  if(tag.instance().length() > 0)
    result = tag.instance();
  //  std::cerr << "'"<<tag.label() << "', '"<< tag.instance()<<"' = '"<< result<<"'"<<std::endl;
  return result;
}


// ------------ Method called once each job just before starting event loop  ------------
void 
DiLeptonSystematicTreesFromMiniAOD::beginJob()
{
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiLeptonSystematicTreesFromMiniAOD::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiLeptonSystematicTreesFromMiniAOD);
