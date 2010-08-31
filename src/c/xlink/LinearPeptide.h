#ifndef LINEARPEPTIDE_H_
#define LINEARPEPTIDE_H_

#include "objects.h"
#include "utils.h"

#include "MatchCandidate.h"
#include "XLinkBondMap.h"
#include "XLinkablePeptide.h"

#include <vector>

class LinearPeptide : public MatchCandidate {
 protected:

  PEPTIDE_T* peptide_;
  char* sequence_;
  BOOLEAN_T is_decoy_;
 public:
  
  LinearPeptide();
  LinearPeptide(char* sequence_);
  LinearPeptide(PEPTIDE_T* peptide);

  virtual ~LinearPeptide();

  static void addCandidates(FLOAT_T precursor_mz, int charge, 
			    INDEX_T* index, DATABASE_T* database,
			    PEPTIDE_MOD_T** peptide_modes,
			    int num_peptide_mods,
			    MatchCandidateVector& candidates,
			    BOOLEAN_T use_decoy_window);

  virtual MATCHCANDIDATE_TYPE_T getCandidateType();
  virtual std::string getSequenceString();
  virtual FLOAT_T getMass();
  virtual MatchCandidate* shuffle();
  virtual void predictIons(ION_SERIES_T* ion_series, int charge);
  std::string getIonSequence(ION_T* ion);
};



#endif
