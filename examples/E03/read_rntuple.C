//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/read_rntuple.C
/// \brief Macro for reading the E03d simulated data from Root file
///        written with kRNTuple mode

void read_rntuple()
{
/// Macro for reading the E03d simulated data from Root file
/// written with kRNTuple mode.
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4d.C.

  // MC application
  Int_t threadId = 1;
  Ex03dMCApplication* appl
    =  new Ex03dMCApplication("Example03", "The example03 MC application");

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i, TMCRootManager::kRNTuple, threadId);
    // appl->GetCalorimeterSD()->Print();
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }
}

// Reading tree without using the Ex03dMCApplication functions
void read_rntuple_raw()
{
  // Get a unique pointer to an empty RNTuple model
  auto model = ROOT::RNTupleModel::Create();

  // We only define the fields that are needed for reading
  std::shared_ptr<std::vector<Ex03CalorHit>> hits = model->MakeField<std::vector<Ex03CalorHit>>("hits");

  // Create an ntuple and attach the read model to it
  auto reader = ROOT::RNTupleReader::Open(std::move(model), "PRExample03", "PRExample03.root");

  // Quick overview of the ntuple and list of fields.
  reader->PrintInfo();

  // Loop over entries
  for (auto entryId : *reader) {
    if (entryId >= 5)
      break;
    reader->LoadEntry(entryId);
    std::cout << "Entry " << entryId << " has " << hits->size() << " hits." << std::endl;
    Ex03dCalorimeterSD::PrintTotal(hits.get());
  }

}
