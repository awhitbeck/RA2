import os,sys,re,fileinput,string,shutil

def frange(start,stop,step):
    step *= 2*((stop>start)^(step<0))-1
    return [start+i*step for i in range(int((stop-start)/step))]


topologies=["T1","T2"]
mothermasses=frange(300.0,1000.0,100.0)

largeMass="10000.0"

for topo in topologies:
	for motherMass in mothermasses:
		if topo.find("T1")>-1:
			newMSquark=largeMass
			newMGluino=str(motherMass)
		elif topo.find("T2")>-1:
			newMGluino=largeMass
			newMSquark=str(motherMass)

		dauMin=100.0
		dauMax=motherMass-50.0
		daughtermasses=frange(dauMin,dauMax,50.0)
		for dauMass in daughtermasses:
			newMChi10=str(dauMass)

			modellist=[topo,newMSquark,newMGluino,newMChi10]

			modelFilename = string.join(modellist,"_")+".slha"

			shutil.copyfile("template.slha",modelFilename)

			for line in fileinput.FileInput(modelFilename, inplace=1):
				line=line.replace("MSQUARK",newMSquark)
				line=line.replace("MGLUINO",newMGluino)
				line=line.replace("MCHI10",newMChi10)
				print line.rstrip()
