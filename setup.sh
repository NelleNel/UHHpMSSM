# go to home
cd $HOME

# make sure nothin is there yet
if [ -d UHHpMSSM ]; then
  echo "ERROR: the package UHHpMSSM already exist in $HOME"
  return
fi

# get the main analysis repository
git clone https://github.com/lveldere/UHHpMSSM
cd UHHpMSSM
maindir=$mylocation/UHHpMSSM

# create a software directory
swdir=$maindir/software
mkdir $swdir

# get analysis scripts (to automize submission of analysis jobs with grid-control)
cd $swdir
git clone https://github.com/lveldere/anascripts

# get the VBF analysis tools (used for histogram drawing)
git clone https://github.com/lveldere/VBF-LS-tau-tools

# install grid-contrl (for job-submission to the grid)
svn co https://ekptrac.physik.uni-karlsruhe.de/svn/grid-control/tags/stable/grid-control

# setup root environment
cd $swdir
module use -a /afs/desy.de/group/cms/modulefiles/
module load root/5.34

# get delphes
cd $swdir
delphes=Delphes-3.1.2
wget http://cp3.irmp.ucl.ac.be/downloads/$delphes.tar.gz
tar -xzf $delphes.tar.gz
rm $delphes.tar.gz
cd $delphes
# do some crazy shit to get delphes compiled
sed -i 's/Pythia.h/Pythia8\/Pythia.h/g' modules/PileUpMergerPythia8.cc
sed -i 's/Pythia.h/Pythia8\/Pythia.h/g' readers/DelphesPythia8.cpp
sed -i 's/lib -lpythia8/lib\/archive -lpythia8/' Makefile
sed -i 's/-lLHAPDF/-llhapdfdummy/' Makefile
temp_CMSSW_FWLITE_INCLUDE_PATH=$CMSSW_FWLITE_INCLUDE_PATH
unset CMSSW_FWLITE_INCLUDE_PATH
make -j 8
export CMSSW_FWLITE_INCLUDE_PATH=$temp_CMSSW_FWLITE_INCLUDE_PATH

# install HepMC package
cd $swdir
hepmc=HepMC-2.06.09 
mkdir $hepmc
mkdir temp
cd temp
wget http://lcgapp.cern.ch/project/simu/HepMC/download/$hepmc.tar.gz
tar -xzf $hepmc.tar.gz
cd $hepmc
./configure --prefix $swdir/$hepmc --with-momentum=GEV --with-length=CM
make -j 8
make check -j 8
make install
cd $swdir
rm -r temp

# get pythia
cd $swdir
pythia=pythia8186
mkdir temp
mkdir $pythia
cd temp
wget http://home.thep.lu.se/~torbjorn/pythia8/$pythia.tgz
tar -xzf $pythia.tgz
cd $pythia
./configure --prefix=$swdir/$pythia --with-hepmc2=$swdir/$hepmc
make -j 8
make install
cd $swdir
rm -rf temp
export PYTHIA8=$swdir/$pythia

# get back
cd $mylocation
