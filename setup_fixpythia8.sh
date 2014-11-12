# get pythia
cd $HOME
source UHHpMSSM/setenv.sh
cd $SWDIR
export swdir=$SWDIR
pythia=pythia8186
hepmc=HepMC-2.06.09 
rm -rf $pythia
rm -rf $pythia.tgz
rm -rf temp
mkdir temp
mkdir $pythia
cd temp
wget http://home.thep.lu.se/~torbjorn/pythia8/$pythia.tgz
tar -xzf $pythia.tgz
cd $pythia
./configure --prefix=$swdir/$pythia --with-hepmc=$swdir/$hepmc
make -j 8
make install
cd $swdir
rm -rf temp
