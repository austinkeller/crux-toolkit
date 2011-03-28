#include "Barista.h"

double Barista :: check_gradients_hinge_one_net(int protind, int label)
{
  int num_pep = d.protind2num_pep(protind);
  int *pepinds = d.protind2pepinds(protind);
  max_psm_inds.erase(max_psm_inds.begin(),max_psm_inds.end());
  max_psm_scores.erase(max_psm_scores.begin(),max_psm_scores.end());
  for (int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];
      int num_psms = d.pepind2num_psm(pepind);
      int *psminds = d.pepind2psminds(pepind);
      double max_sc = -1000000.0;
      int max_ind = 0;
      for (int j = 0; j < num_psms; j++)
	{
	  double *feat = d.psmind2features(psminds[j]);
	  double *sc = net.fprop(feat);
	  if(sc[0] > max_sc)
	    {
	      max_sc = sc[0];
	      max_ind = j;
	    }
	}
      max_psm_inds.push_back(max_ind);
      max_psm_scores.push_back(max_sc);
    }
  assert((int)max_psm_inds.size() == num_pep);
  assert((int)max_psm_scores.size() == num_pep);
  
  double sm = 0.0;
  double n = pow(num_pep,alpha);
  for(unsigned int i = 0; i < max_psm_inds.size() ; i++)
    sm+= max_psm_scores[i];
  sm /= n;

  //if(sm < 1)
  // {
  net.clear_gradients();
  double *gc = new double[1];
  gc[0] = -label/n;
  for(int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];
      int *psminds = d.pepind2psminds(pepind);
      int max_psm_ind = psminds[max_psm_inds[i]];
      double *feat = d.psmind2features(max_psm_ind);
      net.fprop(feat);
      net.bprop(gc);
    }

  double h = 0.000001;
  double diff = -(1-sm*label);
  double err = 0.0;
  double *w = net.get_weights(1);
  double *dw = net.get_dweights(1);
  for (int k = 0; k < d.get_num_features(); k++)
    {
      w[k] += h;
      max_psm_inds.erase(max_psm_inds.begin(),max_psm_inds.end());
      max_psm_scores.erase(max_psm_scores.begin(),max_psm_scores.end());
      for (int i = 0; i < num_pep; i++)
	{
	  int pepind = pepinds[i];
	  int num_psms = d.pepind2num_psm(pepind);
	  int *psminds = d.pepind2psminds(pepind);
	  double max_sc = -1000000.0;
	  int max_ind = 0;
	  for (int j = 0; j < num_psms; j++)
	    {
	      double *feat = d.psmind2features(psminds[j]);
	      double *sc = net.fprop(feat);
	      if(sc[0] > max_sc)
		{
		  max_sc = sc[0];
		  max_ind = j;
		}
	    }
	  max_psm_inds.push_back(max_ind);
	  max_psm_scores.push_back(max_sc);
	}
      double sm1 = 0.0;
      double n = pow(num_pep,alpha);
      for(unsigned int i = 0; i < max_psm_inds.size() ; i++)
	sm1 += max_psm_scores[i];
      sm1 /= n;
      
      diff += (1-sm1*label);
      //cout << dw[k] << " " << diff/h << " " << dw[k]-diff/h << endl;
      err += dw[k]-diff/h;
      w[k]-=h;
      diff -= (1-sm1*label);
    }

  
  double *bias = net.get_bias(1);
  double *dbias = net.get_dbias(1);
  for (int k = 0; k < num_hu; k++)
    {
      bias[k] += h;
      max_psm_inds.erase(max_psm_inds.begin(),max_psm_inds.end());
      max_psm_scores.erase(max_psm_scores.begin(),max_psm_scores.end());
      for (int i = 0; i < num_pep; i++)
	{
	  int pepind = pepinds[i];
	  int num_psms = d.pepind2num_psm(pepind);
	  int *psminds = d.pepind2psminds(pepind);
	  double max_sc = -1000000.0;
	  int max_ind = 0;
	  for (int j = 0; j < num_psms; j++)
	    {
	      double *feat = d.psmind2features(psminds[j]);
	      double *sc = net.fprop(feat);
	      if(sc[0] > max_sc)
		{
		  max_sc = sc[0];
		  max_ind = j;
		}
	    }
	  max_psm_inds.push_back(max_ind);
	  max_psm_scores.push_back(max_sc);
	}
      double sm1 = 0.0;
      double n = pow(num_pep,alpha);
      for(unsigned int i = 0; i < max_psm_inds.size() ; i++)
	sm1 += max_psm_scores[i];
      sm1 /= n;

      diff += 1-sm1*label;
      //cout << dbias[k] << " " << diff/h << " " << dbias[k]-diff/h << endl;
      err += dbias[k]-diff/h;
      bias[k]-=h;
      diff -= 1-sm1*label;
    }
  
  w = net.get_weights(2);
  dw = net.get_dweights(2);
  for (int k = 0; k < num_hu; k++)
    {
      w[k] += h;
      max_psm_inds.erase(max_psm_inds.begin(),max_psm_inds.end());
      max_psm_scores.erase(max_psm_scores.begin(),max_psm_scores.end());
      for (int i = 0; i < num_pep; i++)
	{
	  int pepind = pepinds[i];
	  int num_psms = d.pepind2num_psm(pepind);
	  int *psminds = d.pepind2psminds(pepind);
	  double max_sc = -1000000.0;
	  int max_ind = 0;
	  for (int j = 0; j < num_psms; j++)
	    {
	      double *feat = d.psmind2features(psminds[j]);
	      double *sc = net.fprop(feat);
	      if(sc[0] > max_sc)
		{
		  max_sc = sc[0];
		  max_ind = j;
		}
	    }
	  max_psm_inds.push_back(max_ind);
	  max_psm_scores.push_back(max_sc);
	}
      double sm1 = 0.0;
      double n = pow(num_pep,alpha);
      for(unsigned int i = 0; i < max_psm_inds.size() ; i++)
	sm1 += max_psm_scores[i];
      sm1 /= n;

      diff += 1-sm1*label;
      //cout << dw[k] << " " << diff/h << " " << dw[k]-diff/h << endl;
      err += dw[k]-diff/h;
      w[k]-=h;
      diff -= 1-sm1*label;
    }

  bias = net.get_bias(2);
  dbias = net.get_dbias(2);
  for (int k = 0; k < 1; k++)
    {
      bias[k] += h;
      max_psm_inds.erase(max_psm_inds.begin(),max_psm_inds.end());
      max_psm_scores.erase(max_psm_scores.begin(),max_psm_scores.end());
      for (int i = 0; i < num_pep; i++)
	{
	  int pepind = pepinds[i];
	  int num_psms = d.pepind2num_psm(pepind);
	  int *psminds = d.pepind2psminds(pepind);
	  double max_sc = -1000000.0;
	  int max_ind = 0;
	  for (int j = 0; j < num_psms; j++)
	    {
	      double *feat = d.psmind2features(psminds[j]);
	      double *sc = net.fprop(feat);
	      if(sc[0] > max_sc)
		{
		  max_sc = sc[0];
		  max_ind = j;
		}
	    }
	  max_psm_inds.push_back(max_ind);
	  max_psm_scores.push_back(max_sc);
	}
      double sm1 = 0.0;
      double n = pow(num_pep,alpha);
      for(unsigned int i = 0; i < max_psm_inds.size() ; i++)
	sm1 += max_psm_scores[i];
      sm1 /= n;

      diff += 1-sm1*label;
      //cout << dbias[k] << " " << diff/h << " " << dbias[k]-diff/h << endl;
      err += dbias[k]-diff/h;
      bias[k]-=h;
      diff -= 1-sm1*label;
    }
  //}
  return err;
}

double Barista :: check_gradients_hinge_clones(int protind, int label)
{
  double sm = get_protein_score(protind);
  net.clear_gradients();
  calc_gradients(protind,label);

  double h = 0.0000001;
  double diff = -(1-sm*label);
  double err = 0.0;
  double *w = net.get_weights(1);
  double *dw = net.get_dweights(1);
  for (int k = 0; k < d.get_num_features(); k++)
    {
      w[k] += h;
      double sm1 = get_protein_score(protind);
      diff += (1-sm1*label);
      //cout << dw[k] << " " << diff/h << " " << dw[k]-diff/h << endl;
      err += dw[k]-diff/h;
      w[k]-=h;
      diff -= (1-sm1*label);
    }
  
  double *bias = net.get_bias(1);
  double *dbias = net.get_dbias(1);
  for (int k = 0; k < num_hu; k++)
    {
      bias[k] += h;
      double sm1 = get_protein_score(protind);
      diff += 1-sm1*label;
      //cout << dbias[k] << " " << diff/h << " " << dbias[k]-diff/h << endl;
      err += dbias[k]-diff/h;
      bias[k]-=h;
      diff -= 1-sm1*label;
    }
  
  w = net.get_weights(2);
  dw = net.get_dweights(2);
  for (int k = 0; k < num_hu; k++)
    {
      w[k] += h;
      double sm1 = get_protein_score(protind);
      diff += 1-sm1*label;
      //cout << dw[k] << " " << diff/h << " " << dw[k]-diff/h << endl;
      err += dw[k]-diff/h;
      w[k]-=h;
      diff -= 1-sm1*label;
    }

  bias = net.get_bias(2);
  dbias = net.get_dbias(2);
  for (int k = 0; k < 1; k++)
    {
      bias[k] += h;
      double sm1 = get_protein_score(protind);
      diff += 1-sm1*label;
      //cout << dbias[k] << " " << diff/h << " " << dbias[k]-diff/h << endl;
      err += dbias[k]-diff/h;
      bias[k]-=h;
      diff -= 1-sm1*label;
    }
  
  net.update(mu);

  return err;
}

/**********************************************************/
int Barista :: getOverFDRPSM(PSMScores &s, NeuralNet &n,double fdr)
{
  double* featVec;
  int label = 0;
  for(int i = 0; i < s.size(); i++)
    {
      featVec = d.psmind2features(s[i].psmind);
      label = s[i].label;
      double *r = n.fprop(featVec);
      s[i].score = r[0];
    }

  int overFDR = s.calcOverFDR(fdr);
 
  if(verbose > 1)
    {
      cout << "psm over fdr: num psms " << overFDR << endl;
      int cn = 0;
      set<int> peptides;
      for(int i = 0; i < s.size(); i++)
	{
	  if(s[i].label == 1)
	    cn++;
	  peptides.insert(d.psmind2pepind(s[i].psmind));
	  if(cn >= overFDR)
	    break;
	}
      cout << "psm over fdr: num peptides " << peptides.size() << endl;
    }
  return overFDR;
}

double Barista :: get_peptide_score(int pepind, NeuralNet &n)
{
  int num_psm = d.pepind2num_psm(pepind);
  int *psminds = d.pepind2psminds(pepind);
  double max_sc = -100000000.0;
  for(int i = 0; i < num_psm; i++)
    {
      int psmind = psminds[i];
      double *feat = d.psmind2features(psmind);
      double *sc = n.fprop(feat);
      if(max_sc < sc[0])
	max_sc = sc[0];
    }
  return max_sc;
}


int Barista :: getOverFDRPep(PepScores &s, NeuralNet &n,double fdr)
{
  int pepind = 0;
  int label = 0;
  for(int i = 0; i < s.size(); i++)
    {
      pepind = s[i].pepind;
      double sc = get_peptide_score(pepind,n);
      s[i].score = sc;
    }

  int overFDR = s.calcOverFDR(fdr);

  if(verbose > 1)
    {
      int cn = 0;
      set<int> proteins_pos;
      set<int> proteins_neg;
      int pep_pos = 0;
      int pep_neg = 0;
      for(int i = 0; i < s.size(); i++)
	{
	  label = s[i].label;
	  if(label == 1)
	    cn++;
      
	  if(label == 1)
	    pep_pos++;
	  else
	    pep_neg++;
	  
	  int pepind = s[i].pepind;
	  int num_prot = d.pepind2num_prot(pepind);
	  int *protinds = d.pepind2protinds(pepind);
	  for(int j=0; j < num_prot;j++)
	    {
	      if(label == 1)
		proteins_pos.insert(protinds[j]);
	  else
	    proteins_neg.insert(protinds[j]);
	    }
	  if(cn >= overFDR)
	    break;
	}
      cout << "over FDR peptides:\n";
      cout << "num pos proteins  " << proteins_pos.size() << " num neg proteins " << proteins_neg.size() << " num pos peptides " << pep_pos << " num neg peptides " << pep_neg  << endl;
    }
  return overFDR;
}

/*****************************************************/

double Barista :: get_protein_score_parsimonious(int protind, NeuralNet &n)
{
  int num_pep = d.protind2num_pep(protind);
  int num_all_pep = d.protind2num_all_pep(protind);
  int *pepinds = d.protind2pepinds(protind);
  double sm = 0.0;
  double div = pow(num_all_pep,alpha);

  for (int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];

      if(used_peptides[pepind] == 0)
	{
	  used_peptides[pepind] = 1;
	  int num_psms = d.pepind2num_psm(pepind);
	  int *psminds = d.pepind2psminds(pepind);
	  double max_sc = -1000000.0;
	  int max_ind = 0;
	  for (int j = 0; j < num_psms; j++)
	    {
	      double *feat = d.psmind2features(psminds[j]);
	      double *sc = n.fprop(feat);
	      if(sc[0] > max_sc)
		{
		  max_sc = sc[0];
		  max_ind = j;
		}
	    }
	  pepind_to_max_psmind[pepind] = psminds[max_ind];
	  sm += max_sc;
	}
    }
  
  sm /= div;
  return sm;
}

int Barista :: getOverFDRProtParsimonious(ProtScores &set, NeuralNet &n, double fdr)
{
  pepind_to_max_psmind.clear();
  pepind_to_max_psmind.resize(d.get_num_peptides(),-1);
  int total_num_pep = d.get_num_peptides();
  used_peptides.clear();
  used_peptides.resize(total_num_pep,0);
  double r = 0.0;
  for(int i = 0; i < set.size(); i++)
    {
      int protind = set[i].protind;
      r = get_protein_score_parsimonious(protind,n);
      set[i].score = r;
    }
  return set.calcOverFDR(fdr);
}

void Barista :: write_results_prot(string &out_dir, int fdr)
{
  ostringstream fn;
  fn << out_dir <<"/barista.target.proteins.txt";
  cout << "writing results to " << fn.str() << endl;
  ofstream f_res(fn.str().c_str());

  int cn = 0;
  for(int i = 0; i < trainset.size(); i++)
    {
      if(trainset[i].label == 1)
	{
	  //write out proteins
	  f_res << "protein group " << cn+1  << " q=" << trainset[i].q << endl;
	  int protind = trainset[i].protind;
	  f_res << d.ind2prot(protind) << " ";
	  for(unsigned int j = 0; j < trainset[i].subset_protinds.size(); j++)
	    {
	      int ind = trainset[i].subset_protinds[j];
	      if(d.protind2label(ind) == 1)
		f_res << d.ind2prot(ind) << " ";
	    }
	  f_res << endl;
	
	  //write out peptides
	  f_res << "peptides\n";
	  int num_pep = d.protind2num_pep(protind);
	  int *pepinds = d.protind2pepinds(protind);
	  for( int j = 0; j < num_pep; j++)
	    {
	      int pepind = pepinds[j];
	      f_res << d.ind2pep(pepind); 
	      int psmind = pepind_to_max_psmind[pepind];
	      if(psmind > -1)
		f_res << "-" << d.psmind2scan(psmind) << "." << d.psmind2charge(psmind);
	      else
		cout << "waning: did not assign peptide max psmind\n";
	      f_res << " ";
	    }
	  f_res << endl << endl;
	  cn++;
	}
      if(cn >= fdr)
	break;
    }
  f_res.close();
}


void Barista :: report_all_results()
{

  trainset.clear();
  testset.clear();
  ProtScores::fillProteinsFull(trainset, d);
  cout << "finished training, making parsimonious protein set\n";
  trainset.make_meta_set(d);
  int fdr_trn = getOverFDRProtParsimonious(trainset,max_net_prot,selectionfdr);
  cout << "total proteins parsimonious at q<" << selectionfdr << ": " << fdr_trn << endl;
  int fdr_trn_psm = getOverFDRPSM(psmtrainset, max_net_psm, selectionfdr); 
  cout << "peptides at q< " << selectionfdr << ": " << getOverFDRPep(peptrainset, max_net_pep, selectionfdr) << endl;
  cout << "psms at q< " << selectionfdr << ": " << fdr_trn_psm << endl;
  write_results_prot(out_dir, fdr_trn);

}


void Barista :: get_pep_seq(string &pep, string &seq, string &n, string &c)
{
  string tmp;
  size_t pos;
  
  pos = pep.find(".");
  if(pos != string::npos)
    n = pep.at(pos-1); 
  tmp = pep.substr(pos+1, pep.size());

  pos = tmp.find(".");
  if(pos != string::npos)
    c = tmp.at(pos+1);
  seq = tmp.substr(0, pos);
}


void Barista :: write_results_prot_xml(ofstream &os)
{
  os << "<proteins>" <<endl;
  int cn = 0;
  for(int i = 0; i < trainset.size(); i++)
    {
      if(trainset[i].label == 1)
	{
	  cn++;
	  //write out proteins
	  int protind = trainset[i].protind;
	  os << " <protein_group p:group_id=" << cn << ">" << endl;
	  os << "  <q_value>" << trainset[i].q << "</q_value>" << endl;
	  os << "  <score>" << trainset[i].score << "</score>" << endl;
	  os << "  <protein_ids>" << endl;
	  os << "   <protein_id>" << d.ind2prot(protind) << "</protein_id>" <<endl;
	  for(unsigned int j = 0; j < trainset[i].subset_protinds.size(); j++)
	    {
	      int ind = trainset[i].subset_protinds[j];
	      if(d.protind2label(ind) == 1)
		os << "   <protein_id>" << d.ind2prot(ind) << "</protein_id> " << endl;;
	    }
	  os << "  </protein_ids>" << endl;
	
	  //write out peptides
	  os << "  <peptide_ids>" << endl;
	  int num_pep = d.protind2num_pep(protind);
	  int *pepinds = d.protind2pepinds(protind);
	  for( int j = 0; j < num_pep; j++)
	    {
	      int pepind = pepinds[j];
	      string pep = d.ind2pep(pepind);
	      string seq, n, c;
	      get_pep_seq(pep, seq, n, c);
	      os << "   <peptide_id>" << seq << "</peptide_id>"<< endl; 
	    }
	  os << "  </peptide_ids>" << endl;

	  os << " </protein_group>" << endl;
	}
      
    }
  os << "</proteins>" << endl;
}

void Barista :: write_results_peptides_xml(ofstream &os)
{
  os << "<peptides>" <<endl;
  int cn = 0;
  for(int i = 0; i < peptrainset.size(); i++)
    {
      if(peptrainset[i].label == 1)
	{
	  cn++;
	  //write out proteins
	  int pepind = peptrainset[i].pepind;
	  string pep = d.ind2pep(pepind);
	  string seq, n, c;
	  get_pep_seq(pep, seq, n, c);
	  os << " <peptide p:peptide_id=\"" << seq << "\">" << endl;
	  os << "  <q_value>" << peptrainset[i].q << "</q_value>" << endl;
	  os << "  <score>" << peptrainset[i].score << "</score>" << endl;
	  //write out peptides
	  int psmind = pepind_to_max_psmind[pepind];
	  if(psmind > -1)
	    os << "  <main_psm_id>"<< psmind << "</main_psm_id>" << endl;
	  else
	    cout << "waning: did not assign peptide max psmind\n";
	  
	  //print out all the psms in which this peptide is present
	  int num_psm = d.pepind2num_psm(pepind);
	  int *psminds = d.pepind2psminds(pepind);
	  os << "  <psm_ids>" << endl;
	  for(int j = 0; j < num_psm; j++)
	    os << "   <psm_id>" << psminds[j] << "</psm_id>" << endl;
	  os << "  </psm_ids>" << endl;
	  
	  //print out all the proteins
	  int num_prot = d.pepind2num_prot(pepind);
	  int *protinds = d.pepind2protinds(pepind);
	  os << "  <protein_ids>" << endl;
	  for(int j = 0; j < num_prot; j++)
	    os << "   <protein_id>" << d.ind2prot(protinds[j]) << "</protein_id>" << endl;
	  os << "  </protein_ids>" << endl;
	  os << " </peptide>" << endl;  
	}
      
    }
  os << "</peptides>" << endl;
}

void Barista :: write_results_psm_xml(ofstream &os)
{
  os << "<psms>" <<endl;
  int cn = 0;
  for(int i = 0; i < psmtrainset.size(); i++)
    {
      if(psmtrainset[i].label == 1)
	{
	  cn++;
	  //write out proteins
	  int psmind = psmtrainset[i].psmind;
	  os << " <psm p:psm_id=" << psmind << ">" << endl;
	  os << "  <q_value>" << psmtrainset[i].q << "</q_value>" << endl;
	  os << "  <score>" << psmtrainset[i].score << "</score>" << endl;
	  os << "  <scan>" << d.psmind2scan(psmind) << "</scan>" << endl;
	  os << "  <charge>" << d.psmind2charge(psmind) << "</charge>" << endl;
	  int pepind = d.psmind2pepind(psmind);
	  string pep = d.ind2pep(pepind);
	  string seq, n,c;
	  get_pep_seq(pep,seq,n,c);
	  os << "  <peptide_seq n =\"" << n << "\" c=\"" << c << "\" seq=\"" << seq << "\"/>" << endl;
	  os << " </psm>" << endl;  
	}
    }
  os << "</psms>" << endl;
}


void Barista :: report_all_results_xml()
{

  trainset.clear();
  testset.clear();
  ProtScores::fillProteinsFull(trainset, d);
  cout << "finished training, making parsimonious protein set\n";
  trainset.make_meta_set(d);
  int fdr_trn = getOverFDRProtParsimonious(trainset,max_net_prot,selectionfdr);
  cout << "total proteins parsimonious at q<" << selectionfdr << ": " << fdr_trn << endl;
  int fdr_trn_psm = getOverFDRPSM(psmtrainset, max_net_psm, selectionfdr); 
  cout << "peptides at q< " << selectionfdr << ": " << getOverFDRPep(peptrainset, max_net_pep, selectionfdr) << endl;
  cout << "psms at q< " << selectionfdr << ": " << fdr_trn_psm << endl;

  ostringstream fname;
  fname << out_dir << "/barista_output.xml";
  ofstream of(fname.str().c_str());
  of << "<barista_output>" << endl;
  of << endl;
  write_results_prot_xml(of);
  write_results_peptides_xml(of);
  write_results_psm_xml(of);
  of << endl;
  of << "</barista_output>" << endl;
  of.close();

}

/*************************************************************************/
void Barista :: report_prot_fdr_counts(vector<double> &qvals, ofstream &of)
{
  for(unsigned int count = 0; count < qvals.size(); count++)
    {
      double q = qvals[count];
      if(trainset.size() > 0)
	{
	  int fdr_trn = getOverFDRProtParsimonious(trainset,max_net_prot,q);
	  of << q << " " << fdr_trn;
	  cout << q << " " << fdr_trn;
	}
      
      if(testset.size() > 0)
	{
	  int fdr_tst = getOverFDRProtParsimonious(testset,max_net_prot,q);
	  of << " " << fdr_tst;
	  cout << " " << fdr_tst;
	}
      of << endl;
      cout << endl;
    }
} 

void Barista :: report_psm_fdr_counts(vector<double> &qvals, ofstream &of)
{
  for(unsigned int count = 0; count < qvals.size(); count++)
    {
      double q = qvals[count];
      if(psmtrainset.size() > 0)
	{
	  int fdr_trn = getOverFDRPSM(psmtrainset,max_net_prot,q);
	  of << q << " " << fdr_trn;
	  cout << q << " " << fdr_trn;
	}
      
      if(psmtestset.size() > 0)
	{
	  int fdr_tst = getOverFDRPSM(psmtestset,max_net_prot,q);
	  of << " " << fdr_tst;
	  cout << " " << fdr_tst;
	}
      of << endl;
      cout << endl;
    }
} 


void Barista :: report_pep_fdr_counts(vector<double> &qvals, ofstream &of)
{
  for(unsigned int count = 0; count < qvals.size(); count++)
    {
      double q = qvals[count];
      if(peptrainset.size() > 0)
	{
	  int fdr_trn = getOverFDRPep(peptrainset,max_net_prot,q);
	  of << q << " " << fdr_trn;
	  cout << q << " " << fdr_trn;
	}
      
      if(psmtestset.size() > 0)
	{
	  int fdr_tst = getOverFDRPep(peptestset,max_net_prot,q);
	  of << " " << fdr_tst;
	  cout << " " << fdr_tst;
	}
      of << endl;
      cout << endl;
    }
} 



void Barista :: report_all_fdr_counts()
{
  int num_qvals = 14;
  vector<double> qvals;
  qvals.resize(num_qvals,0.0);
  double q = 0.0;
  for(int count = 0; count < num_qvals; count++)
    {
      qvals[count] = q;
      if(q < 0.01)
	q+=0.0025;
      else
	q+=0.01;
    }
    
  ostringstream fname;
  fname << out_dir << "/barista_prot_m7" << ".txt";
  ofstream of(fname.str().c_str());
  if(trainset.size() > 0)
    trainset.make_meta_set(d);
  if(testset.size() > 0)
    testset.make_meta_set(d);
  report_prot_fdr_counts(qvals,of);
  of.close();

  fname.str("");
  fname << out_dir << "/barista_psm_m7" << ".txt";
  ofstream ofpsm(fname.str().c_str());
  report_psm_fdr_counts(qvals,ofpsm);
  ofpsm.close();

  fname.str("");
  fname << out_dir << "/barista_pep_m7" << ".txt";
  ofstream ofpep(fname.str().c_str());
  report_pep_fdr_counts(qvals,ofpep);
  ofpep.close();


}



/*******************************************************************************/


double Barista :: get_protein_score(int protind, NeuralNet &n)
{
  int num_pep = d.protind2num_pep(protind);
  int num_all_pep = d.protind2num_all_pep(protind);
  int *pepinds = d.protind2pepinds(protind);
  double sm = 0.0;
  double div = pow(num_all_pep,alpha);

  for (int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];
      int num_psms = d.pepind2num_psm(pepind);
      int *psminds = d.pepind2psminds(pepind);
      double max_sc = -1000000.0;
      int max_ind = 0;
      for (int j = 0; j < num_psms; j++)
	{
	  double *feat = d.psmind2features(psminds[j]);
	  double *sc = n.fprop(feat);
	  if(sc[0] > max_sc)
	    {
	      max_sc = sc[0];
	      max_ind = j;
	    }
	}
      sm += max_sc;
    }
  sm /= div;
  return sm;
}


double Barista :: get_protein_score_max(int protind, NeuralNet &n)
{
  int num_pep = d.protind2num_pep(protind);
  int *pepinds = d.protind2pepinds(protind);
  double sm = 0.0;
  double div = 1;
  vector<double> scores;

  for (int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];
      double pep_sc = get_peptide_score(pepind,n);
      scores.push_back(pep_sc);
    }
  sort(scores.begin(),scores.end());
  reverse(scores.begin(),scores.end());
  sm += scores[0];
  sm /= div;
  return sm;
}

int Barista :: getOverFDRProtMax(ProtScores &set, NeuralNet &n, double fdr)
{
  double r = 0.0;
  for(int i = 0; i < set.size(); i++)
    {
      int protind = set[i].protind;
      r = get_protein_score_max(protind,n);
      set[i].score = r;
    }
  return set.calcOverFDR(fdr);
  
}

int Barista :: getOverFDRProt(ProtScores &set, NeuralNet &n, double fdr)
{
  double r = 0.0;
  for(int i = 0; i < set.size(); i++)
    {
      int protind = set[i].protind;
      r = get_protein_score(protind,n);
      set[i].score = r;
    }
  return set.calcOverFDR(fdr);
  
}



double Barista :: get_protein_score(int protind)
{
  int num_pep = d.protind2num_pep(protind);
  int num_all_pep = d.protind2num_all_pep(protind);
  int *pepinds = d.protind2pepinds(protind);
  max_psm_inds.erase(max_psm_inds.begin(),max_psm_inds.end());
  max_psm_scores.erase(max_psm_scores.begin(),max_psm_scores.end());
  int psm_count = 0;
  for (int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];
      int num_psms = d.pepind2num_psm(pepind);
      int *psminds = d.pepind2psminds(pepind);
      double max_sc = -1000000.0;
      int max_ind = 0;
      for (int j = 0; j < num_psms; j++)
	{
	  double *feat = d.psmind2features(psminds[j]);
	  double *sc = net_clones[psm_count].fprop(feat);
	  if(sc[0] > max_sc)
	    {
	      max_sc = sc[0];
	      max_ind = j;
	    }
	  psm_count++;
	}
      max_psm_inds.push_back(max_ind);
      max_psm_scores.push_back(max_sc);
    }
  assert((int)max_psm_inds.size() == num_pep);
  assert((int)max_psm_scores.size() == num_pep);
  
  double sm = 0.0;
  double n = pow(num_all_pep,alpha);
  for(unsigned int i = 0; i < max_psm_inds.size() ; i++)
    sm+= max_psm_scores[i];
  sm /= n;
  return sm;
}

void Barista :: calc_gradients(int protind, int label)
{
  int num_pep = d.protind2num_pep(protind);
  int num_all_pep = d.protind2num_all_pep(protind);
  int *pepinds = d.protind2pepinds(protind);
  double n = pow(num_all_pep,alpha);

  double *gc = new double[1];
  gc[0] = -label/n;
  int psm_count = 0;
  for(int i = 0; i < num_pep; i++)
    {
      int pepind = pepinds[i];
      int num_psms = d.pepind2num_psm(pepind);
      int clone_ind = psm_count+max_psm_inds[i];
      net_clones[clone_ind].bprop(gc);
      psm_count += num_psms;
    }
  delete[] gc;
}

double Barista :: train_hinge(int protind, int label)
{
  double sm = get_protein_score(protind);
  double err = max(0.0,1.0-sm*label);

  if(sm*label < 1)
    {
      net.clear_gradients();
      calc_gradients(protind,label);
      net.update(mu);
    }
  return err;
}


double Barista :: train_hinge_psm(int psmind, int label)
{
  double *x = d.psmind2features(psmind);
  double *c = net.fprop(x);
  double err = 1.0-c[0]*label;

  if(c[0]*label < 1)
    {
      double *gc = new double[1];
      gc[0] = -1*label;
      net.clear_gradients();
      net.bprop(gc);
      net.update(mu);
      delete[] gc;
    }
  return err;
}



void Barista :: train_net(double selectionfdr, int interval)
{
  for (int k = 0; k < nepochs; k++)
    {
      if(verbose > 0)
	cout << "epoch " << k << endl;
      double err_sum = 0.0;
      for(int i = 0; i < trainset.size(); i++)
	{
	  int ind = rand()%interval;
	  int protind = trainset[ind].protind;
	  int label = d.protind2label(protind);
	  err_sum += train_hinge(protind,label);
	}
      int fdr_trn = getOverFDRProt(trainset,net,selectionfdr);
      if(verbose > 0)
	{
	  if(interval == trainset.size())
	    cout << "err " << err_sum << "  ";
	  cout << selectionfdr << " " << fdr_trn;
	  if(testset.size() > 0)
	    cout << " " << getOverFDRProt(testset,net,selectionfdr);
	  cout << endl;
	}
	if(fdr_trn > max_fdr)
	{
	  max_net_prot = net;
	  max_fdr = fdr_trn;
	  if(verbose == 0)
	    {
	      cout << "q< " << selectionfdr << ": max non-parsimonious so far " << max_fdr;
	      if(testset.size() > 0)
		cout << " " << getOverFDRProt(testset,max_net_prot,selectionfdr);
	      cout << endl;
	    }
	}
      if(verbose > 0)
	{
	  cout << "q< " << selectionfdr << ": max non-parsimonious so far " << max_fdr;
	  if(testset.size() > 0)
	    cout << " " << getOverFDRProt(testset,max_net_prot,selectionfdr);
	  cout << endl;
	}
      //cout << "max prot " << getOverFDRProtMax(trainset,max_net_prot,selectionfdr) << endl;
      if(1)
	{
	  int fdr_trn_psm = getOverFDRPSM(psmtrainset, net, selectionfdr); 
	  if(fdr_trn_psm > max_fdr_psm)
	    {
	      max_net_psm = net;
	      max_fdr_psm = fdr_trn_psm;
	    }
	}
      if(1)
	{
	  int fdr_trn_pep = getOverFDRPep(peptrainset, net, selectionfdr); 
	  if(fdr_trn_pep > max_fdr_pep)
	    {
	      max_net_pep = net;
	      max_fdr_pep = fdr_trn_pep;
	    }
	}
    }
  if(verbose > 0)
    {
      cout << "max peptides so far at q< " << selectionfdr << ": " << getOverFDRPep(peptrainset, max_net_psm, selectionfdr) << endl;
      cout << "max psms so far at q<" << selectionfdr << ": " << max_fdr_psm << endl;
    }
}


void Barista :: train_net_multi_task(double selectionfdr, int interval)
{
  for (int k = 0; k < nepochs; k++)
    {
      if(verbose > 0)
	cout << "epoch " << k << endl;
      double err_sum = 0.0;
      for(int i = 0; i < trainset.size(); i++)
	{
	  int ind = rand()%trainset.size();
	  int protind = trainset[ind].protind;
	  int label = d.protind2label(protind);
	  err_sum += train_hinge(protind,label);

	  ind = rand()%interval;
	  int psmind = psmtrainset[ind].psmind;
	  label = d.psmind2label(psmind);
	  train_hinge_psm(psmind,label);

	}
      
      int fdr_trn = getOverFDRProt(trainset,net,selectionfdr);
      if(verbose > 0)
	{
	  if(interval == trainset.size())
	    cout << "err " << err_sum << "  ";
	  cout << selectionfdr << " " << fdr_trn;
	  if(testset.size() > 0)
	    cout << " " << getOverFDRProt(testset,net,selectionfdr);
	  cout << endl;
	}
	if(fdr_trn > max_fdr)
	{
	  max_net_prot = net;
	  max_fdr = fdr_trn;
	  if(verbose == 0)
	    {
	      cout << "q< " << selectionfdr << ": max non-parsimonious so far " << max_fdr;
	      if(testset.size() > 0)
		cout << " " << getOverFDRProt(testset,max_net_prot,selectionfdr);
	      cout << endl;
	    }
	}
      if(verbose > 0)
	{
	  cout << "q< " << selectionfdr << ": max non-parsimonious so far " << max_fdr;
	  if(testset.size() > 0)
	    cout << " " << getOverFDRProt(testset,max_net_prot,selectionfdr);
	  cout << endl;
	}
      //cout << "max prot " << getOverFDRProtMax(trainset,max_net_prot,selectionfdr) << endl;
      if(1)
	{
	  int fdr_trn_psm = getOverFDRPSM(psmtrainset, net, selectionfdr); 
	  if(fdr_trn_psm > max_fdr_psm)
	    {
	      max_net_psm = net;
	      max_fdr_psm = fdr_trn_psm;
	    }
	}
      if(1)
	{
	  int fdr_trn_pep = getOverFDRPep(peptrainset, net, selectionfdr); 
	  if(fdr_trn_pep > max_fdr_pep)
	    {
	      max_net_pep = net;
	      max_fdr_pep = fdr_trn_pep;
	    }
	}
    }
  if(verbose > 0)
    {
      cout << "max peptides so far at q< " << selectionfdr << ": " << getOverFDRPep(peptrainset, max_net_psm, selectionfdr) << endl;
      cout << "max psms so far at q<" << selectionfdr << ": " << max_fdr_psm << endl;
    }
}



void Barista :: setup_for_training(int trn_to_tst)
{
  string summary_fn = "summary.txt";
  string psm_fn = "psm.txt";

  d.load_data(summary_fn, psm_fn);
  d.normalize_psms();
  d.load_prot_data();
  if(trn_to_tst)
    ProtScores::fillProteinsSplit(trainset, testset, d, trn_to_tst);
  else
    ProtScores::fillProteinsFull(trainset, d);
  thresholdset = trainset;
  
  if(trn_to_tst)
    PSMScores::fillFeaturesSplit(psmtrainset, psmtestset, d, 0.5);
  else
    PSMScores::fillFeaturesFull(psmtrainset, d);
  if(trn_to_tst)
    PepScores::fillFeaturesSplit(peptrainset, peptestset, d, 0.5);
  else
    PepScores::fillFeaturesFull(peptrainset, d);
  

  num_features = d.get_num_features();
  int has_bias = 1;
  int is_lin = 0;
  if(is_lin)
    num_hu = 1;
  net.initialize(num_features, num_hu, is_lin, has_bias);
  max_net_prot = net;
  max_net_psm = net;
  max_net_pep = net;

  
  //get the max num peptides
  max_peptides = 0;
  max_psms_in_prot = 0;
  for (int i = 0; i < d.get_num_proteins();i++)
    {
      int num_pep = d.protind2num_pep(i);
      if(num_pep > max_peptides)
	max_peptides = num_pep;
      int *pepinds = d.protind2pepinds(i);
      int sum_num_psms = 0;
      for(int j = 0; j < num_pep; j++)
	{
	  int pepind = pepinds[j];
	  sum_num_psms += d.pepind2num_psm(pepind);
	}
      if(max_psms_in_prot < sum_num_psms)
	max_psms_in_prot = sum_num_psms;
    }
  max_psm_inds.reserve(max_peptides);
  max_psm_scores.reserve(max_peptides);
 
  //create the net clones
  net_clones = new NeuralNet[max_psms_in_prot];
  for (int i = 0; i < max_psms_in_prot;i++)
    net_clones[i].clone(net);
}


int Barista :: run()
{
  setup_for_training(2);
  srand(seed);
  train_net(selectionfdr, trainset.size());

  net.copy(max_net_prot);
  int interval= getOverFDRProt(trainset,max_net_prot,0.1)*2;
  if(interval > trainset.size())
    interval=trainset.size()/2;
  train_net(selectionfdr, interval);
  
  report_all_results();

  return 0;

}


int Barista :: run_tries()
{
  setup_for_training(2);
  srand(seed);
  
  int tries = 3;
  vector<double> mu_choices;
  mu_choices.resize(3,0.0);
  mu_choices[0] = 0.005;
  mu_choices[1] = 0.01;
  mu_choices[2] = 0.05;
  for(int k = 0; k < tries; k++)
    {
      mu = mu_choices[k];
      net.make_random();
      train_net(selectionfdr, trainset.size());
    }
    
  report_all_fdr_counts();

   return 0;

}


int Barista :: run_tries_multi_task()
{
  setup_for_training(2);
  srand(seed);
  
  int tries = 3;
  vector<double> mu_choices;
  mu_choices.resize(3,0.0);
  mu_choices[0] = 0.005;
  mu_choices[1] = 0.01;
  mu_choices[2] = 0.05;
  for(int k = 1; k < tries; k++)
    {
      mu = mu_choices[k];
      net.make_random();
      train_net_multi_task(selectionfdr, psmtrainset.size());
    }
    
  //net.copy(max_net_prot);
  int interval= getOverFDRPSM(psmtrainset,max_net_psm,0.01)*2;
  if(interval > psmtrainset.size() || interval < 10)
  interval/=2;
  train_net_multi_task(selectionfdr, interval);
  
  //report_all_results_xml();
  report_all_fdr_counts();

   return 0;

}



int Barista :: set_command_line_options(int argc, char *argv[])
{
  string db_source;
  string sqt_source;
  string ms2_source;

  int arg = 1;
  while (arg < argc)
    {
      
      string str = argv[arg];
      //parse the options
      if(str.find("--") != string::npos)
	{
	  //found enzyme
	  if(str.find("enzyme") != string::npos)
	    {
	      size_t pos = str.find("=");
	      if(pos == string::npos)
		cout << "warning: wrong option format: --enzyme=<enzyme>, will assume trypsin\n";
	      string enzyme = str.substr(pos+1,str.size());
	      cout << "enzyme: " << enzyme << endl;
	      //sqtp.set_decoy_prefix(prefix);
	    }
	  //found decoy prefix
	  if(str.find("decoy") != string::npos)
	    {
	      size_t pos = str.find("=");
	      if(pos == string::npos)
		cout << "warning: wrong option format: --decoy_prefix=<decoy prefix>, will assume random_\n";
	      string prefix = str.substr(pos+1,str.size());
	      cout << "decoy prefix: " << prefix << endl;
	      sqtp.set_decoy_prefix(prefix);
	    }
	}
      else
	{
	  break;
	  
	}
      arg++;
    }
  if(argc-arg < 3)
    {
      cout << "Incorrect input format: to run barista\n barista database-source sqt-file-source ms2-file-source" << endl;
      return 0;
    }
  db_source = argv[arg];
  arg++;
  sqt_source = argv[arg];
  arg++;
  ms2_source = argv[arg];
  cout << "database source " << db_source << " sqt source " << sqt_source << " ms2 source " << ms2_source << endl;
  if(!sqtp.set_input_sources(db_source, sqt_source, ms2_source))
    return 0;
  sqtp.set_output_dir("crux-output");

  return 1;
  
}



int Barista::main(int argc, char **argv) {
 //int main(int argc, char **argv){
  
  if(!set_command_line_options(argc,argv))
    return 0;
  //num of spec features
  sqtp.set_num_spec_features(0);
  
  if(!sqtp.run())
    return 0;
  sqtp.clear();
  string dir = sqtp.get_output_dir();
  
  set_input_dir(dir);
  set_output_dir(dir);
  run_tries_multi_task();
  sqtp.clean_up(dir);
  
  return 1;
}   

string Barista::getName() {
  return "barista";
}

string Barista::getDescription() {
  return "Program for protein inferrence using .....";
}



