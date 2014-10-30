UHHpMSSM
========

# Installation

Get the setup script and run it

```
wget https://raw.githubusercontent.com/lveldere/UHHpMSSM/master/setup.sh
source setup.sh
```

That's it!
This ends with an error, which has further no effect.

# How to set the environment

```
source UHHpMSSM/setenv.sh
```

# How to write analyzers

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
 
