//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2014 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \ingroup E03
/// \file E03/read_ttree.C
/// \brief Macro for reading the E03d simulated data from Root file
///        written with kTTree mode

void read_ttree()
{
/// Macro for reading the E03d simulated data from Root file
/// written with kTTree mode.
/// Note that since Root 6 the libraries have to be loaded first
/// via load_g4d.C.

  // MC application
  Int_t threadId = 1;
  Ex03dMCApplication* appl
    =  new Ex03dMCApplication("Example03", "The example03 MC application");

  for (Int_t i=0; i<5; i++) {
    cout << "   Event no " << i+1 << ":" << endl;
    appl->ReadEvent(i, TMCRootManager::kTTree, threadId);
    // appl->GetCalorimeterSD()->Print();
    appl->GetCalorimeterSD()->PrintTotal();
    cout << endl;
  }
}

// Reading tree without using the Ex03dMCApplication functions
void read_ttree_raw()
{
    // Open the file and get the TTree
    TFile* rfile = TFile::Open("TExample03_1.root", "READ");
    TTree* tree = nullptr;
    rfile->GetObject("TExample03", tree);

    // Initialize the pointer to nullptr before reading
    std::vector<Ex03CalorHit>* hits = nullptr;

    // Set the branch address (pass the address of your pointer variable)
    tree->SetBranchAddress("hits", &hits);
    // std::cout << "hits: " << hits << std::endl;

    // Loop over entries
    Long64_t nEntries = tree->GetEntries();
    std::cout << "3" << std::endl;

    // for (Long64_t i = 0; i < nEntries; ++i) {
    for (Long64_t i = 0; i < 5; ++i) {
        tree->GetEntry(i);
        std::cout << "Entry " << i << " has " << hits->size() << " hits." << std::endl;
        Ex03dCalorimeterSD::PrintTotal(hits);
    }

    // Cleanup
    // ROOT automatically allocated the vector; delete it when completely done
    delete hits;
    rfile->Close();
}
