# go to home
cd $HOME
cd UHHpMSSM
maindir=$PWD
swdir=$maindir/software
cd $swdir
rm -rf grid-control
svn co https://ekptrac.physik.uni-karlsruhe.de/public/grid-control/branches/stable-fixes/grid-control
cd $maindir