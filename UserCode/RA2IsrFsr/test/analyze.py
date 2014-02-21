import ROOT
import sys,glob


#ROOT.gEnv.SetValue("TFile.Recover",0)
# important!  This doesn't work.  Set in local .rootrc

for filename in glob.glob("runT1_400*.root"):

	f = ROOT.TFile(filename, "READ")
	
	if f.IsZombie(): continue
		

	dirlist = f.GetListOfKeys()
	f.cd("missingPt")

	t=ROOT.gDirectory.GetListOfKeys()

	for k in t:
		tp=k.GetName()
		if tp.find("missing_pT")>-1:
			hnew_=ROOT.TH1F()
			ROOT.gDirectory.GetObject(tp,hnew_)
			print filename,hnew_.Integral()/100000.0


	f.Close()
