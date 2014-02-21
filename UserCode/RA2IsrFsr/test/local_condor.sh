#!/bin/bash
date
source /uscmst1/prod/sw/cms/shrc prod

basedir=/pnfs/cms/WAX/resilient/spadhi/CMS/spadhi/PhysicsProcesses_TopologyT2_38xFall10/PhysicsProcesses_TopologyT2_38xFall10/f959c379445f9d7540d41cf9ffa87a96

a=`expr $1 + 1`

test="reco_"$a"_1_*"
save=""
for file in $basedir/$test
do
  save=$file
done

echo $save

INPUTFILE=$save

CMSSW=/uscms_data/d2/mrenna/devel/CMSSW_3_8_4
directory=GeneratorInterface/Pythia6Interface


workerspace=$PWD

cd $CMSSW/src
eval `scramv1 runtime -sh`
#cd $directory

cd $workerspace

flag=$1

configfile=oset_cfg$flag.py

#cp add_cfg.py $configfile
mv oset_cfg.py $configfile

sed -i -e "s,ROOTFILE,$INPUTFILE," $configfile
sed -i -e "s/ROOTSUFFIX/$flag/" $configfile
sed -i -e "s/\#$flag//" $configfile

cmsRun $configfile

date
pwd
#---------------------------------------------------------------------------------
