$MYPROJECTDIR/analyzers/genSignal $MYPROJECTDIR/analyzers/py8card_pp14TeV_SUSY.txt $1 $2.hepmc
rm $2.root
$SWDIR/Delphes-3.1.2/DelphesHepMC $SWDIR/Delphes-3.1.2/examples/delphes_card_CMS.tcl $2.root $2.hepmc
