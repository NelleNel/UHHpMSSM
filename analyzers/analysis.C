#include <TMath.h>
//------------------------------------------------------------------------------

void analysis(const char *inputFile,char * outputFile)
{
  string myprojectdir = getenv("MYPROJECTDIR");

  gSystem->Load((myprojectdir + "/software/Delphes-3.1.2/libDelphes").c_str());// put back


  // Create chain of root trees
  TChain chain("Delphes");
  std::ifstream file(inputFile);
  std::string str; 
  while (std::getline(file, str))
    {
      if(str.size()==0)
	continue;
      cout << "adding file: " << str << endl;
      chain.Add(str.c_str());
    }

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  
  // Get pointers to branches used in this analysis

  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");

  
  // Book histograms
  TFile * ofile = TFile::Open(outputFile,"RECREATE"); // by lv
  ofile->cd();
  TH1 *counts = new TH1F("counts","counts",0,0,0);
  counts->SetBit(TH1::kCanRebin);
  TH1 *histMHT = new TH1F("MHT", "MHT", 250, 0.0, 1800.0);
  TH1 *histHT = new TH1F("HT", "HT", 250, 0.0, 1800.0);
  TH1 *histNJets = new TH1F("NJets", "NJets", 12, 0.0, 12.0);
  TH1 *histNElec = new TH1F("NElec", "NElec", 12, 0.0, 12.0);
  TH1 *histNMuon = new TH1F("NMuon", "NMuon", 12, 0.0, 12.0);
  TH1 *histNBJets = new TH1F("NBJets", "NBJets", 12, 0.0, 12.0);
  TH1 *histMHT_N1 = new TH1F("MHT N-1 plot", "MHT N-1 plot", 250, 0.0, 1800.0);
  TH1 *histHT_N1 = new TH1F("HT N-1 plot", "HT N-1 plot", 250, 0.0, 1800.0);
  TH1 *histNJets_N1 = new TH1F("NJets N-1 plot", "NJets N-1 plot", 12, 0.0, 12.0);
  
  counts->Fill("NoCuts",0.);
  counts->Fill("NJets3",0.);
  counts->Fill("MHT200",0.);
  counts->Fill("HT500",0.);

  cout << "!!!!" << chain->GetEntries() << endl;
  
  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
    {
      // Load selected branches with data from specified event
      treeReader->ReadEntry(entry);
     
      counts->Fill("NoCuts",1.);
      
     
      Double_t weight = 1.;
      
      // Histogram for the number of jets
      Int_t N_Jets = 0.;
      Int_t N_BJets = 0.;
      for(Int_t j = 0; j < branchJet->GetEntries(); j++)
	{
	  Jet *jet = (Jet*) branchJet->At(j);
	  if (jet->PT > 50 && sqrt((jet->Eta)*(jet->Eta)) < 2.5){N_Jets = N_Jets + 1;}
	  if (jet->PT > 50 && sqrt((jet->Eta)*(jet->Eta)) < 2.5 && jet->BTag == 1){N_BJets = N_BJets + 1;}
	}
      Double_t MHT = 0.;
      Double_t HT = 0;
      // If event contains at least 1 jet
      if(branchJet->GetEntries() > 0);
      {
	// Calculate HT
	for(Int_t i = 0; i < branchJet->GetEntries(); i++)
	  {
	    Jet *jet = (Jet*) branchJet->At(i);
	    double jetPT = jet->PT;
	    double jetEta = jet->Eta;
	    if (jetPT > 50 && sqrt((jetEta)*(jetEta)) < 2.5)
	      {
		HT = HT + jetPT;
	      }
	  }
      

	// Calculate MHT
	TLorentzVector V_MHT(0., 0., 0., 0.);
	for(Int_t j = 0; j < branchJet->GetEntries(); j++)
	  {
	  
	    Jet *jet = (Jet*) branchJet->At(j);
	    if(jet->PT >30 && sqrt((jet->Eta)*(jet->Eta)) <5.)
	      {
		TLorentzVector V_PT;
		V_PT.SetPtEtaPhiM(jet->PT, jet->Eta, jet->Phi, jet->Mass);
		V_MHT = V_MHT + V_PT;
	      }
	  }
	V_MHT = -V_MHT;
	MHT = V_MHT*V_MHT;
	MHT = sqrt(MHT);


	

      }

   
      // Calculate N_Electron
      Int_t NElec = 0;
      for(Int_t j = 0; j < branchElectron->GetEntries(); j++)
	{
	  Electron *electron = (Electron*) branchElectron->At(j);
	  if(electron->PT >10){NElec = NElec + 1;}
	}

      // Calculate N_Muon
      Int_t NMuon = 0;
      for(Int_t j = 0; j < branchMuon->GetEntries(); j++)
	{
	  Muon *muon = (Muon*) branchMuon->At(j);
	  if(muon->PT >10){NMuon = NMuon + 1;}
	}
 
     
      // Fill histograms
      histHT->Fill(HT, weight);
      histMHT->Fill(MHT,weight);
      histNElec->Fill(NElec, weight);
      histNMuon->Fill(NMuon, weight);
      histNJets->Fill(N_Jets, weight);
      histNBJets->Fill(N_BJets, weight);
      if(MHT>200 && HT>500){histNJets_N1->Fill(N_Jets, weight);}
      if(MHT>200 && N_Jets>2){histHT_N1->Fill(HT, weight);}
      if(HT>500 && N_Jets>2){histMHT_N1->Fill(MHT, weight);}
	  
      if(!(   N_Jets >= 3     )){ continue;}
      else counts->Fill("NJets3",1);

      if(!(   MHT > 200       )){ continue;}
      else counts->Fill("MHT200",1);

      if(!(   HT > 500        )){ continue;}
      else counts->Fill("HT500",1);
    }
  ofile->Write();

  
}
