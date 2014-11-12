#!/bin/bash

# needed for perl...
export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8

shopt -s expand_aliases
module ()
{
    eval `/usr/bin/modulecmd bash $*`
}
module use -a /afs/desy.de/group/cms/modulefiles/
module load root/5.34

# variables used for job distribution
export MYPROJECT=UHHpMSSM
export MYPROJECTDIR=$HOME/$MYPROJECT
export SWDIR=$MYPROJECTDIR/software
export ANASCRIPTS=$SWDIR/anascripts
export PATH=$PATH:$ANASCRIPTS:$MYPROJECTDIR/scripts:$SWDIR/grid-control
export PYTHONPATH=$PYTHONPATH:$ANASCRIPTS
export ENVSCRIPT=$MYPROJECTDIR/setenv.sh

# to run pythia
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SWDIR/HepMC-2.06.09/lib/
export PYTHIA8DATA=$SWDIR/pythia8186/xmldoc
