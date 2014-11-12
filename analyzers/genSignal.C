#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"

using namespace Pythia8;

int main(int argc,char * argv[]){
 
  if(!argc==2){
    cout << "expecting exactly 3 arguments: <path to pythia8 card> <path to intput slha file> <path to output file>" << std::endl;
    exit(1);
  }
  
  string cardfile = argv[1];
  string slhafile = argv[2];
  string ofile = argv[3];

  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;

  // Specify file where HepMC events will be stored.
  HepMC::IO_GenEvent ascii_io(ofile.c_str(), std::ios::out);

  // Initialization
  Pythia pythia;

  // swich on all susy production modes
  pythia.readFile(cardfile);
  // read from the slha file
  pythia.readString((string("SLHA:file = ") + slhafile).c_str());
  // set the random seed based on the clock time
  // => different events every time you run this code
  pythia.rndm.init(0); 
  
  pythia.init();
    
  int nevents = pythia.mode("Main:numberOfEvents");
    
  for (int iEvent=0; iEvent<nevents; iEvent++){
    pythia.next();

    // Construct new empty HepMC event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );
    
    // Write the HepMC event to file. Done with it.
    ascii_io << hepmcevt;
    delete hepmcevt;
  }
  //statistics
  pythia.stat();


  return 0;
    
}
