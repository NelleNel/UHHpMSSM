#!/bin/bash

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
