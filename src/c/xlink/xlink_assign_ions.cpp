#include "xhhc.h"
#include "xhhc_ion_series.h"
#include "xhhc_scorer.h"

#include "objects.h"
#include "scorer.h"
#include "SpectrumCollection.h"
#include "DelimitedFile.h"

#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <iostream>
#include <fstream>




void print_spectrum(Spectrum* spectrum, LinkedIonSeries& ion_series);
int main(int argc, char** argv){

  /* Verbosity level for set-up/command line reading */
  set_verbosity_level(CARP_ERROR);
  
  /* Define optional command line arguments */
  const char* option_list[] = {
    "verbosity",
    "version",
    "use-mgf",
    "fragment-mass",
    "max-ion-charge",
    "mz-bin-width",
    "precision"
  };
  int num_options = sizeof(option_list) / sizeof(char*);
  

  /* Define required command line arguments */
  const char* argument_list[] = {
    "peptide A",
    "peptide B",
    "pos A",
    "pos B",
    "link mass",
    "charge state",
    "scan number",
    "ms2 file"};
  int num_arguments = sizeof(argument_list) / sizeof(char*);
  
  /* for debugging of parameter processing */
  //set_verbosity_level( CARP_DETAILED_DEBUG );
  set_verbosity_level( CARP_ERROR );
  
  /* Set default values for parameters in parameter.c */
  initialize_parameters();

  /* Define optional and required command line arguments */
  select_cmd_line_options( option_list, num_options );
  select_cmd_line_arguments( argument_list, num_arguments);

  /* Parse the command line, including the optional params file */
  /* does sytnax, type, bounds checking and dies if neccessessary */
  parse_cmd_line_into_params_hash(argc, argv, "xlink-assign-ions");

  /* Set verbosity */
  set_verbosity_level(get_int_parameter("verbosity"));

  /* Get Arguments */
  char* peptideA = get_string_parameter("peptide A");
  char* peptideB = get_string_parameter("peptide B");
  
  int posA     = get_int_parameter("pos A");
  int posB     = get_int_parameter("pos B");
  int charge   = get_int_parameter("charge state"); 
  int scan_num = get_int_parameter("scan number"); 

  char* ms2_file = get_string_parameter("ms2 file");

  LinkedPeptide::linker_mass = get_double_parameter("link mass");
 
  // create new ion series
  
  // a single peptide linked to itself
  if (strcmp(peptideB, "NULL") == 0) {
    cout << "B is null" << endl; 
    peptideB = NULL;
  }

  // read ms2 file
  SpectrumCollection* collection = new SpectrumCollection(ms2_file);
  collection->parse();

  
  //cout << "lp " << lp << endl; 
  Spectrum* spectrum = NULL;
  Spectrum* current_spectrum = NULL;

  //TODO allow a binary search on both mgf and ms2 files.

  for (SpectrumIterator spectrum_iterator = collection->begin();
    spectrum_iterator != collection->end();
    ++spectrum_iterator) {

    current_spectrum = *spectrum_iterator;
    if (current_spectrum->getFirstScan() == scan_num) {
      spectrum = current_spectrum;
      break;
    }
  }

  // search for spectrum with correct scan number
  if (spectrum == NULL) {
    carp(CARP_ERROR, "failed to find spectrum with  scan_num: %d", scan_num);
    delete collection;
    exit(1);
  }

  //created linked peptide.
  LinkedPeptide lp = LinkedPeptide(peptideA, peptideB, posA-1, posB-1, charge);

  int max_ion_charge = get_max_ion_charge_parameter("max-ion-charge");

  int max_charge = min(max_ion_charge, charge);

  LinkedIonSeries ion_series(max_charge);
  ion_series.add_linked_ions(lp);
  print_spectrum(spectrum, ion_series);

  // free heap
  delete collection;
  //free_spectrum(spectrum);
}

void print_spectrum(Spectrum* spectrum, LinkedIonSeries& ion_series) {

      int total_by_ions = ion_series.get_total_by_ions();
      int matched_by_ions = Scorer::get_matched_by_ions(spectrum, ion_series);
      FLOAT_T frac_by_ions = (double)matched_by_ions / (double) total_by_ions;

      carp(CARP_INFO, "total theoretical ions:%d",total_by_ions);
      carp(CARP_INFO,"theoretical ions matched:%d",matched_by_ions);
      carp(CARP_INFO,"frac theoretical ions matched:%f",frac_by_ions);
      carp(CARP_INFO,"npeaks:%d",spectrum->getNumPeaks());

      FLOAT_T bin_width = get_double_parameter("mz-bin-width");
      vector<LinkedPeptide>& ions = ion_series.ions();
      
      map<PEAK_T*, LinkedPeptide> matched;
      double matched_intensity = 0;
      for (vector<LinkedPeptide>::iterator ion = ions.begin();
	   ion != ions.end(); 
	   ++ion) {
        
	//if (ion -> get_mz(MONO) >= 400 && ion -> get_mz(MONO) <= 1400) {
	  if (ion -> type() == B_ION || ion -> type() == Y_ION) {
	    PEAK_T* peak = spectrum->getNearestPeak(ion->get_mz(MONO), 
                                                    bin_width);
	    if (peak != NULL) {
              if (matched.find(peak) == matched.end()) {
                matched_intensity += get_peak_intensity(peak);
              }
	      matched[peak] = *ion;
	    } else {
              carp(CARP_DETAILED_DEBUG,"Ion clash!");
            }
	  }
	//}
      }

      double total_intensity = spectrum->getTotalEnergy();
      double frac_intensity = matched_intensity / total_intensity;

      carp(CARP_INFO,"matched intensity:%lf",matched_intensity);
      carp(CARP_INFO,"total intensity:%lf",total_intensity);
      carp(CARP_INFO,"frac intensity:%lf",frac_intensity);
      

      //now print out the spectrum

      DelimitedFile result_file;

      unsigned int mz_obs_col = result_file.addColumn("m/z obs");
      unsigned int int_col = result_file.addColumn("intensity");
      unsigned int mz_calc_col = result_file.addColumn("m/z calc");
      unsigned int mass_obs_col = result_file.addColumn("mass obs");
      unsigned int mass_calc_col = result_file.addColumn("mass calc");
      unsigned int ppm_col = result_file.addColumn("ppm(Mass)");
      unsigned int ion_col = result_file.addColumn("ion");
      unsigned int seq_col = result_file.addColumn("sequence");
      
      for (PeakIterator peak_iter = spectrum->begin();
        peak_iter != spectrum->end();
        ++peak_iter) {

	PEAK_T* peak = *peak_iter;
        
	//if (get_peak_location(peak) >= 400 && get_peak_location(peak) <= 1400) {
          unsigned int row_idx = result_file.addRow();
          result_file.setValue(mz_obs_col, row_idx, get_peak_location(peak));
          result_file.setValue(int_col, row_idx, get_peak_intensity(peak));

          

	  if (matched.find(peak) != matched.end()) {
              LinkedPeptide& ion = matched[peak];
              double mz_calc = ion.get_mz(MONO);
              double mz_obs = get_peak_location(peak);
              int charge = ion.charge();
              double mass_calc = ion.mass(MONO);
              double mass_obs = (mz_obs - MASS_PROTON) * (double)charge;
              double ppm =  fabs(mass_calc - mass_obs) / mass_calc * 1e6;
              
              result_file.setValue(mz_calc_col, row_idx, mz_calc);
              result_file.setValue(mass_obs_col, row_idx, mass_obs);
              result_file.setValue(mass_calc_col, row_idx, mass_calc);
              result_file.setValue(ppm_col, row_idx, ppm);
              
              ostringstream ion_string_oss;
           


              if (ion.type() == B_ION) {
                ion_string_oss << "b";
              } else if (ion.type() == Y_ION) {
                ion_string_oss << "y";
              } else {
                ion_string_oss << "u";
              }
              ion_string_oss << "(+" << charge << ")";

              result_file.setValue(ion_col, row_idx, ion_string_oss.str());
              result_file.setValue(seq_col, row_idx, ion);
          }
        //}
      }

      cout << result_file;

}