//
// All cuts for single track in D0 analysis,
// based on track quality and particle identification
// with TPC and TOF.
// Author: Massimo Venaruzzo
//
//

#include <Riostream.h>

#include "AliPID.h"
#include "AliPIDResponse.h"
#include "AliRsnCutDaughterD0.h"

ClassImp(AliRsnCutDaughterD0)

//__________________________________________________________________________________________________
AliRsnCutDaughterD0::AliRsnCutDaughterD0(const char *name, AliPID::EParticleType pid) :
AliRsnCut(name, AliRsnTarget::kDaughter),
  fNoPID(kFALSE),
  fPID(pid),
  fCutQuality(Form("%sQuality", name)),
  fPionTPCPIDCut(3.0),
  fKaonTPCPIDCut(3.0),
  fPionTOFPIDCut(3.0),
  fKaonTOFPIDCut(3.0),
  fPtDepPIDCut(kFALSE) 
{
  //
  // Constructor
  // Initialize track quality cuts to 2010 defaults
  //
  fCutQuality.SetPtRange(0.15, 1E+20);
  fCutQuality.SetEtaRange(-0.8, 0.8);
  fCutQuality.SetDCARPtFormula("0.0105+0.0350/pt^1.1");
  fCutQuality.SetDCARmin(0.0);
  fCutQuality.SetDCAZmax(2.0);
  fCutQuality.SetSPDminNClusters(1);
  fCutQuality.SetITSminNClusters(0);
  fCutQuality.SetITSmaxChi2(1E+20);
  fCutQuality.SetTPCminNClusters(70);
  fCutQuality.SetTPCmaxChi2(4.0);
  fCutQuality.SetRejectKinkDaughters();
  fCutQuality.SetAODTestFilterBit(5);
}

//__________________________________________________________________________________________________
Bool_t AliRsnCutDaughterD0::IsSelected(TObject *obj)
{
  //
  // Global check
  //

  // coherence check
  if (!TargetOK(obj)) return kFALSE;

  // check track
  AliVTrack *track = dynamic_cast<AliVTrack *>(fDaughter->GetRef());
  if (!track) return kFALSE;
   
  AliDebugClass(2, "Checking status...");
  // check flags
  if ((track->GetStatus() & AliESDtrack::kTPCin   ) == 0) return kFALSE;
  if ((track->GetStatus() & AliESDtrack::kTPCrefit) == 0) return kFALSE;
  if ((track->GetStatus() & AliESDtrack::kITSrefit) == 0) return kFALSE;
  AliDebugClass(2, "...passed");
  
  // quality
  AliDebugClass(2, "Checking quality cuts...");
  if (!fCutQuality.IsSelected(obj)) return kFALSE;
  AliDebugClass(2, "...passed");

  // if no PID is required, accept
  if (fNoPID) return kTRUE;
  
  // check initialization of PID object
  AliPIDResponse *pid = fEvent->GetPIDResponse();
  if (!pid) {
    AliFatal("NULL PID response");
    return kFALSE;
  }
  
  AliDebugClass(2, "Checking TOF Matching..."); 
  // check if TOF is matched
  // and computes all values used in the PID cut
  Bool_t   isTOF  = MatchTOF(track);   
  AliDebugClass(2, "...passed");
   
  Double_t pTPC   = track->GetTPCmomentum();
  Double_t p      = track->P();
  Double_t nsTPC  = TMath::Abs(pid->NumberOfSigmasTPC(track, fPID));
  Double_t nsTOF  = isTOF ? TMath::Abs(pid->NumberOfSigmasTOF(track, fPID)) : 1E20;
  Double_t maxTPC = 1E20;
  Double_t maxTOF = 1E20;
  AliDebugClass(2, "Checking PID...");

  if(!fPtDepPIDCut){
    // applies the cut differently depending on the PID and the momentum
    if (isTOF) {
      if (fPID == AliPID::kPion) {maxTPC = fPionTPCPIDCut; maxTOF = fPionTOFPIDCut;}
      if (fPID == AliPID::kKaon) {maxTPC = fKaonTPCPIDCut; maxTOF = fKaonTOFPIDCut;}
      return (nsTPC <= maxTPC && nsTOF <= maxTOF);
    } else {
      if (fPID == AliPID::kPion) maxTPC = fPionTPCPIDCut;
      if (fPID == AliPID::kKaon) maxTPC = fKaonTPCPIDCut;
      return (nsTPC <= maxTPC); 
    }
  } else {
    // applies the cut differently depending on the PID and the momentum
    if (isTOF) {
      // TPC: 5sigma cut for all
      if (nsTPC > 5.0) return kFALSE;
      // TOF: 3sigma below 1.5 GeV, 2sigma above
      if (p < 1.5) maxTOF = 3.0; else maxTOF = 2.0;
      return (nsTOF <= maxTOF);
    } else {
      // TPC:
      // all   below   350         MeV: 5sigma
      // all   between 350 and 500 MeV: 3sigma
      // pions above   500         MeV: 2sigma
      // kaons between 500 and 700 MeV: 2sigma
      // kaons above   700         MeV: rejected
      if (pTPC <= 0.35)
	maxTPC = 5.0;
      else if (pTPC > 0.35 && pTPC <= 0.5)
	maxTPC = 3.0;
      else {
	if (fPID == AliPID::kPion)
	  maxTPC = 2.0;
	else if (fPID == AliPID::kKaon) {
	  if (pTPC <= 0.7)
	    maxTPC = 2.0;
	  else
	    return kFALSE;
	}
      }
      return (nsTPC <= maxTPC);
    } 
  }    
  
  AliDebugClass(2, "...passed"); 
  // if we reach this point, all checks were successful
  AliDebugClass(2, "Good Pion/Kaon Candidate Found!!");
}
