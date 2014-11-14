hepmcname=$2.hepmc
$MYPROJECTDIR/analyzers/genSignal $MYPROJECTDIR/analyzers/py8card_pp8TeV_SUSY.txt $1 $hepmcname
rm $2
$SWDIR/Delphes-3.1.2/DelphesHepMC $SWDIR/Delphes-3.1.2/examples/delphes_card_CMS.tcl $2 $hepmcname
