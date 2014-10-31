UHHpMSSM
========

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
./myAnalyzer filelist.txt outputFile.root
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
./myAnalyzer filelist.txt test.root
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
run_analyzer_batch.py myAnalyzer sample1,sample2,sample3
```
Have a look at the new files in the directory.
Also check the options of run_analyzer_batch.py with the --help option.
By default run_analyzer_batch.py will prepare such that you have one batch job per input file.
That can lead to huge numbers of jobs, therefore use the option --merge N,
to process N files per job. With this option you will receive one output file per root file in the samples.
If you'd like to have just one output file per job, use in addition the option --fullmerge.
So, the usually recommended way to prepare your batch jobs is
```
run_analyzer_batch.py myAnalyzer sample1,sample2,sample3 -merge N --fullmerge
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

use the ROOT command hadd

```
hadd mergedFile.root path/to/files/*.root 
```

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


 
