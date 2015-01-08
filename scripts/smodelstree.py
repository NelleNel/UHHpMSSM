#!/usr/bin/env python

#
# run me as follows
# python smodelstree.py idir ofile.root
# with idir the directory with smodels output txt files (MUST have extension .txt)"
# ofile.root the output root file with tree
#

import sys,glob,re
import ROOT as rt

idir = sys.argv[1]
ofile = sys.argv[2]
files = sorted(glob.glob(idir + "/*.txt"))

# helper class to store element information
class Element:
    def __init__(self):
        self.particles = None
        self.branch0_mass = None
        self.branch1_mass = None
        self.sqrts = None
    def __str__(self):
        _list = [
            "----------",
            " Element  ",
            "----------",
            " " + self.particles,
            " " + str(self.branch0_mass),
            " " + str(self.branch1_mass),
            " " + str(self.sqrts),
            "----------"
            ]
        return "\n".join(_list)

# a regular expression for file parsing
re_sqrts = re.compile(".*Weight:(.*)\[pb\].*")

# a function to read elements from a txt file
def readElements(filePath):

    FILE = open(filePath)
    lines = FILE.read().strip().split("\n")
    FILE.close()

    elements = []
    active_element = None
    
    for line in lines:
        line = line.strip()
        # at line "Element:" do
        # fail if prev element active, start element
        if line == "Element:":
            if not active_element is None:
                print "Error: encountered line 'Element:...' while still in previous element"
                sys.exit()
            active_element = Element()

        # at line 'Particles in element:' do
        # fail if no element active, add particles to active element
        if line.find("Particles in element:") == 0:
            if active_element is None:
                print "Error: encountered line 'Particles in element:...' while not in element"
                sys.exit()
            line = line.split(":")[1].strip()
            active_element.particles = line
            

        # at line 'Branch 0:' do
        # fail if no element active, add masses to active element
        if line.find("Branch 0:") == 0:
            if active_element is None:
                print "Error: encountered line 'Branch 0: ...' while not in element"
                sys.exit()
            masses = line.split(":")[1].strip().replace("[GeV]","")
            active_element.branch0_mass = masses

        # at line 'Branch 1:' do
        # fail if no element active, add masses to active element
        if line.find("Branch 1:") == 0:
            if active_element is None:
                print "Error: encountered line 'Branch 1: ...' while not in element"
                sys.exit()
            masses = line.split(":")[1].strip().replace("[GeV]","")
            active_element.branch1_mass = masses

        # at line 'Sqrts:' do 
        # if no element active ignore (topology sqrt s)
        # else add sqrts to active element
        if line.find("Sqrts:") == 0:
            if active_element is None:
                continue
            else:
                matches = re_sqrts.match(line)
                active_element.sqrts = float(matches.group(1))

        # if all ok, add element to list of elements and de-activate element
        all_ok = (
            active_element is not None
            and active_element.particles is not None
            and active_element.branch0_mass is not None
            and active_element.branch1_mass is not None
            and active_element.sqrts is not None
                )
        if all_ok:
            elements.append(active_element)
            active_element = None


    return elements

# initialise a tree
tfile = rt.TFile.Open(ofile,"RECREATE")
tree = rt.TTree("smodels","smodels")
particles = rt.std.vector(rt.std.string)()
branch0_mass = rt.std.vector(rt.std.string)()
branch1_mass = rt.std.vector(rt.std.string)()
sqrts = rt.std.vector(float)()
tree.Branch("particles",particles)
tree.Branch("branch0_mass_gev",branch0_mass)
tree.Branch("branch1_mass_gev",branch0_mass)
tree.Branch("sqrts_pb",sqrts)

# loop over input files
for _file in files:

    elements = readElements(_file)

    particles.clear()
    for element in elements:
        particles.push_back(element.particles)
        branch0_mass.push_back(element.branch0_mass)
        branch1_mass.push_back(element.branch1_mass)
        sqrts.push_back(element.sqrts)
    tree.Fill()


tree.Write()
tfile.Close()
