UHHpMSSM
========

## Ask for help in case of naf problems

Please systematically warn the naf-helpdesk in case naf causes problems:
naf-helpdesk__AT__desy.de

## News

### 8 Jan 14: update grid-control to 'stable-fixes'

in order to get rid of the issue that many jobs land on slc5 machines and crahs,
update grid-control as follows

```
cd $HOME/UHHpMSSM
git pull
source setup_fix_grid-control.sh
```

## Disclaimer

Probably not all of this works as expected.
Please report any malfunction you observe.

## Requirements

Use this recipe on one of the slc6 machines, e.g. naf-uhhcms06.desy.de

## Installation

Get the setup script and run it

```
wget https://raw.githubusercontent.com/lveldere/UHHpMSSM/master/setup.sh
source setup.sh
```

That's it!
This ends with an error, which has further no effect.

## How to set the environment

```
source UHHpMSSM/setenv.sh
```

## How to write analyzers

Ntuple analyzers are located in $MYPROJECTDIR/analyzers.

To enable submission of analyzers to the batch system,
they must be written in such a way that they can be run with two arguments:
```
./analyzer.sh arg1 arg2
```
Where arg1 is a txt file with a list of paths to the root files with the ntuples to be analyzed (one file per line),
and where arg2 is the path to the output ROOT file of the analyzer.

In your analyzer read the paths to the root files listed in the input file,
add the TTree objects in the files to a TChain (see ROOT manual).
Then, create an output file and initialise histograms (see ROOT manual).
Fill the histograms while looping over the TChain.
After the loop, write the histogrmas to the output file (see ROOT manual).
In the root directory of the output file, add a historam named "counts" 
and make sure that the first bin of the histogram has a value equal to the number of events processed.

The analyzer directory contains an example filelist.
Use it to test your analyzer.
```
./analyzer.sh filelist.txt test.root
```

## Prepare to run your analyzers over all samples

Check what's inside $MYPROJECTDIR/samples/.txt
On each line you find a directory on dcache and before that, a nickname for that directory.
Each directory contains the files from a particular background sample.
Check the contact of one of the directories
```
ls ...
```
Note that you don't need the full url of the directory to list it.
Also note that you can also add normal directories to this list.

Then create for each directory a list of files inside
```
updatefilelist.py
```
Now have a look at the files under $MYPROJECTDIR/data/filelists/....txt

Create a working directory on 'dust' and create a link in $MYPROJECTDIR
```
mkdir -p /nfs/dust/cms/user/$USER/UHHpMSSM/workdir
ln -s /nfs/dust/cms/user/$USER/UHHpMSSM/workdir $MYPROJECTDIR/workdir
```
On dust you have more storage, much more, than on afs, but nothing is backed up.
If you don't have access to dust yet, just use your afs directory for now.

## Run your analyzers over the event samples

Create a subdirectory in your workdir
```
cd $MYPROJECT/workdir
mkdir test
cd test
```

If you'd like to run with your "myAnalyzer" over the samples sample1,sample2,sample3,
prepare that like this
```
runanalyzer_batch.py myAnalyzer sample1,sample2,sample3
```
Have a look at the new files in the directory.
Also check the options of runanalyzer_batch.py with the --help option.
By default runanalyzer_batch.py will prepare such that you have one batch job per input file.
That can lead to huge numbers of jobs, therefore use the option --merge N,
to process N files per job. With this option you will receive one output file per root file in the samples.
If you'd like to have just one output file per job, use in addition the option --fullmerge.
So, the usually recommended way to prepare your batch jobs is
```
runanalyzer_batch.py myAnalyzer sample1,sample2,sample3 -merge N --fullmerge
```
typical values for N are 20-50.

Then, submit to the grid with grid-control:
```
go.py -icG job.cfg
```
Check the documentation of grid-control here: https://ekptrac.physik.uni-karlsruhe.de/trac/grid-control

Usually you want to change memory and cpu requirements in the grid-control cfg file (job.cfg) to fit your needs.
When you submit for the first time you want to submit just one job and therefore change the number of jobs to be submit to 1.

When jobs are done, find your output files in the subdirectories of the results directory.

## How to merge output files to one file per sample

Say the directory IDIR has several subdirectories each containting root files
```
multiHadd --idir IDIR --odir ODIR
```
will write for each subdirectory in IDIR a root file into ODIR 
that contains the merged content of the root files in the subdirectory.

This script was kindly provided by Denis

## How to draw plots

For a tutorial see the README file in software/VBF-LS-tau-tools/drawHist/tutorial/

## Consider moving your data directory to dust 

If you run out of storage on afs...

```
mkdir -p /nfs/dust/cms/user/$USER/UHHpMSSM/data
mv $MYPROJECTDIR/data/* /nfs/dust/cms/user/$USER/UHHpMSSM/data
rm -rf $MYPROJECTDIR/data
ln -s data
```


## Generating and simulating events

"Generation of events" = simulate proton-proton collisions w/o detector, using pythia8.1

"Simulation of events" = simulate the detector with Delphes3

cd into the analyzers directory and compile the code for event generation
```
cd $MYPROJECTDIR/analyzers
source makeGenSignal.sh
```
now you can generate events as follows
```
./genSignal <path to pythia8 configuration card> <path to slha file> <path to output hepmc file>
```

Two pythi8 configuration cards are provided:  py8card_pp8TeV_SUSY.txt and py8card_pp14TeV_SUSY.txt
to generate SUSY events for pp collisions with center of mass energies of respectively 8 and 14 TeV.
Have a look in one of the files and notice you can steer the number of events to be produced through the variable Main:numberOfEvents and the center of mass energy through the variable Beams:eCM.

For testing, set in py8card_pp8TeV_SUSY.txt the number of events to generate to 7, and run on the example slha file mb_250__mz2_150__edge70.slha
```
./genSignal py8card_pp8TeV_SUSY.txt mb_250__mz2_150__edge70.slha test.hepmc
```
You should see a new file test.hepmc with generated events stored in hepmc format.

Two scripts are provided to run the event generation and the detector simulation one after another: genSimSignal_8TeV.sh and genSimSignal_14TeV.sh for sparticle production in respectively 8 TeV and 14 TeV pp events.

Run Them as follows: 
```
./genSimSignal_*TeV.sh <path to slha file> <path to output root file>
```
e.g.
```
./genSimSignal_8TeV.sh mb_250__mz2_150__edge70.slha test.root
```

Now you should see two new files, test.root.hepmc and test.root, with respectively the generated events in hepmc format and the simulated events in Delphes tree format.

### Mass generation of signal events for on or multiple slha files

Say you have one or more slha files for which you want to generate signal events.
Put your slha files in a directory, and register this directory as a sample.
There is an example directory /nfs/dust/cms/user/lveldere/UHHpMSSM/data/slha/T6bbEdge70/
and the procedure is illustrated with that directory.

First add a line to the file $MYPROJECTDIR/data/samples/slha.txt.
The line for our example is actually already there, check it out
```
$ more $MYPROJECTDIR/data/samples/slha.txt
T6bbEdge70 /nfs/dust/cms/user/lveldere/UHHpMSSM/data/slha/T6bbEdge70
```
The first element is the nickname, the next the path to the directory.

Then update the filelists, and check out the result
```
updatefilelists.py
more $MYPROJECTDIR/data/filelists/slha/T6bbEdge70.txt
```
The last command should list you all slha files in the T6bbEdge70 directory.

Then, create a subdirectory in the workdir and prepare to run over all slha files
```
cd $MYPROJECTDIR/workdir
mkdir testsignal
cd testsignal
runanalyzer_batch.py genSimSignal_8TeV.sh slha/T6bbEdge70 --repeat 10
```
This last command prepares a configuration to run on the naf cluster.
The first argument defines the script inside $MYPROJECTDIR/analyzers to be used for generation.
The second argument defines which slha files to process.
The third argument defines how many jobs per slha file will be submitted.
The latter argument allows you to parallelize the event generation.
Note that the number of events  per job is defined in the pythia8 card used by the generation script,
in this case $MYPROJECTDIR/analyzers/py8card_pp8TeV_SUSY.txt.

Then, run over naf
```
go.py -icG job.cfg
```

When done, check the results
```
ls results/genSimSignal_8TeV.sh/slha/T6bbEdge70/
```

You will find .root files with the Delphes output,
and .root.hepmc files with the pythia8 output.
The slha file name is contained in the output filenames.
The job number is included in the end of the output filenames, in order to have unique output file names. 

## How to propagate changes to and from the central repository


get other peoples changes
```
cd $MYPROJECTDIR/analyzers
git pull
```

to put your changes in the repository
```
cd $MYPROJECTDIR/analyzers
git pull       # always first synchronise your area with the central repository
git add <A FILE>
git add <AN OTHER FILE>
...
git commit -m "<SOME EXPLANATION>"
git push
```

if git push leads to an error like
```
error: The requested URL returned error: 403 Forbidden while accessing
```

first create and register a public ssh key as explained here:
https://help.github.com/articles/generating-ssh-keys/#step-1-check-for-ssh-keys

then edit the git configuration
```
cd $MYPROJECTDIR
emacs -nw .git/config
```
change in the section [remote "origin"] the url to something like
```
url = ssh://git@github.com/lveldere/UHHpMSSM.git
```
(it might be you need your own github username there)

for more info, see the git and github manuals

## How to make your code produce an error that can be catched by grid-control

In the script to be run by grid-control.

```
./YourCommand || exit 666
```

Will cause an error to be trown with code 666 in case YourCommand fails.
Grid-control will recognise this error and flag the job as unsuccessful.

## Old News

### 27 Nov 14: add Denis' script to apply hadd on the content of the subdirectories of a given directory

```
cd $HOME/UHHpMSSM/software/anascripts
git pull
```
see "how to merge histograms per sample"


### 27 Nov 14: add drawHist tools to the path

```
cd $HOME/UHHpMSSM/
git pull
source setenv.sh
```

Now all drawHist tools (drawHist.py, weightHist.py) are available in whatever directory you are

### 27 Nov 14: Allow grid-control to recognise failures during the setup of the root environment

```
cd $HOME/UHHpMSSM/
git pull
```

The environment setup will now crash with exit code 666 if the root environment could not be set correctly.

### 27 Nov 14: fix to previous news item

```
cd $HOME/UHHpMSSM/software/anascripts
git pull
```

### 14 Nov 14: pull in the ability to generate signal events for multiple slha files over naf

```
cd $HOME/UHHpMSSM
git pull
cd analyzers
source makeGenSignal.sh
updatefilelist.py
```

The way to run genSimSignal_*TeV.sh has slightly changed,
see section "Generating and simulating events"

Learn how to use the new tools in the section "Generating and simulating events"

### 12 Nov 14: fix the pythia installation

fix the pythia installation
```
cd $HOME/UHHpMSSM
git pull
source setup_fixpythia8.sh
```














 
