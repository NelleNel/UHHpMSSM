g++ genSignal.C -o genSignal -I $SWDIR/pythia8186/include/ -I $SWDIR/HepMC-2.06.09/include/ -L $SWDIR/pythia8186/lib/archive/ -lpythia8 -llhapdfdummy -lpythia8tohepmc -L $SWDIR/HepMC-2.06.09/lib -lHepMC
