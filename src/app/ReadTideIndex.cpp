#include "ReadTideIndex.h"

#include "TideMatchSet.h"
#include "io/carp.h"
#include "parameter.h"
#include "app/tide/records_to_vector-inl.h"
#include "app/tide/peptide.h"
#include <vector>

using namespace std;

ReadTideIndex::ReadTideIndex() {
}

ReadTideIndex::~ReadTideIndex() {
}

int ReadTideIndex::main(int argc, char** argv) {

  const char* option_list[] = {
    "skip-decoys"
  };
  int num_options = 0;
  const char* arg_list[] = {
    "tide database index"
  };
  int num_args = sizeof(arg_list) / sizeof(char*);
  initialize(arg_list, num_args, option_list, num_options, argc, argv);

  carp(CARP_INFO, "Running read-tide-index...");

  string index_dir = get_string_parameter("tide database index");
  string peptides_file = index_dir + "/pepix";
  string proteins_file = index_dir + "/protix";
  string auxlocs_file = index_dir + "/auxlocs";

  // Read proteins index file
  carp(CARP_INFO, "Reading proteins...");
  ProteinVec proteins;
  pb::Header protein_header;
  if (!ReadRecordsToVector<pb::Protein, const pb::Protein>(&proteins,
      proteins_file, &protein_header)) {
    carp(CARP_FATAL, "Error reading index (%s)", proteins_file.c_str());
  }
  carp(CARP_DEBUG, "Read %d proteins", proteins.size());

  // Read auxlocs index file
  carp(CARP_INFO, "Reading auxiliary locations...");
  vector<const pb::AuxLocation*> locations;
  if (!ReadRecordsToVector<pb::AuxLocation>(&locations, auxlocs_file)) {
    carp(CARP_FATAL, "Error reading index (%s)", auxlocs_file.c_str());
  }
  carp(CARP_DEBUG, "Read %d auxlocs", locations.size());

  // Read peptides index file
  carp(CARP_INFO, "Reading peptides...");
  pb::Header peptides_header;
  HeadedRecordReader peptide_reader(peptides_file, &peptides_header);
  if (!peptides_header.file_type() == pb::Header::PEPTIDES ||
      !peptides_header.has_peptides_header()) {
    carp(CARP_FATAL, "Error reading index (%s)", peptides_file.c_str());
  }

  const pb::Header::PeptidesHeader& pepHeader = peptides_header.peptides_header();
  MassConstants::Init(&pepHeader.mods(),
                      get_double_parameter("mz-bin-width"),
                      get_double_parameter("mz-bin-offset"));

  // Set up output file
  string output_file = make_file_path(getName() + ".peptides.txt");
  ofstream* output_stream = create_stream_in_path(
    output_file.c_str(), NULL, get_boolean_parameter("overwrite"));
	*output_stream << get_column_header(SEQUENCE_COL) << '\t'
                 << get_column_header(PROTEIN_ID_COL) << endl;

  RecordReader* reader = peptide_reader.Reader();
  while (!reader->Done()) {
    // Read peptide
    pb::Peptide pb_peptide;
    reader->Read(&pb_peptide);
    if (get_boolean_parameter("skip-decoys") && pb_peptide.is_decoy()) {
      continue;
    }
    Peptide peptide(pb_peptide, proteins);

    // Output to file
    *output_stream << peptide.SeqWithMods() << '\t'
                   << proteins[peptide.FirstLocProteinId()]->name();
    if (peptide.HasAuxLocationsIndex()) {
      const pb::AuxLocation* aux_loc = locations[peptide.AuxLocationsIndex()];
      for (int i = 0; i < aux_loc->location_size(); i++) {
        const pb::Protein* protein = proteins[aux_loc->location(i).protein_id()];
        if (protein->has_name()) {
          *output_stream << ';' << protein->name();
        }
      }
    }
    *output_stream << endl;
  }

  output_stream->close();
  delete output_stream;

  return 0;
}

string ReadTideIndex::getName() {
  return "read-tide-index";
}

string ReadTideIndex::getDescription() {
  return "Reads an index generated by tide-index and outputs information about "
         "the peptides it contains.";
}

bool ReadTideIndex::needsOutputDirectory() {
  return true;
}

COMMAND_T ReadTideIndex::getCommand() {
  return READ_SPECTRUMRECORDS_COMMAND;
}

bool ReadTideIndex::hidden() {
  return true; 
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 2
 * End:
 */
