ROOTLIBS   = $(shell root-config --libs)
ROOTCFLAGS = $(shell root-config --cflags)

CFLAG      = $(ROOTCFLAGS)
LFLAG      = $(ROOTLIBS)

OBJS   = BackgroundDistribution.o FinalPlot.o InclusiveSearchBin.o SearchBin.o SearchBinManager.o SignalDistribution.o TwoColumnLegend.o


all: makeFinalPlots computeSignificance

makeFinalPlots: $(OBJS)	makeFinalPlots.o
	g++ $(OBJS) makeFinalPlots.o $(LFLAG) -o makeFinalPlots

computeSignificance: $(OBJS) computeSignificance.o
	g++ $(OBJS) computeSignificance.o $(LFLAG) -o computeSignificance

BackgroundDistribution.o: BackgroundDistribution.h BackgroundDistribution.cc InclusiveSearchBin.h
	g++ $(CFLAG) -c BackgroundDistribution.cc

FinalPlot.o: FinalPlot.h FinalPlot.cc BackgroundDistribution.h SignalDistribution.h Style.h TwoColumnLegend.h
	g++ $(CFLAG) -c FinalPlot.cc

InclusiveSearchBin.o: InclusiveSearchBin.h InclusiveSearchBin.cc SearchBin.h
	g++ $(CFLAG) -c InclusiveSearchBin.cc

SearchBin.o: SearchBin.h SearchBin.cc StringParser.h
	g++ $(CFLAG) -c SearchBin.cc

SearchBinManager.o: SearchBinManager.h SearchBinManager.cc SearchBin.h InclusiveSearchBin.h
	g++ $(CFLAG) -c SearchBinManager.cc

SignalDistribution.o: SignalDistribution.h SignalDistribution.cc
	g++ $(CFLAG) -c SignalDistribution.cc

TwoColumnLegend.o: TwoColumnLegend.h TwoColumnLegend.cc
	g++ $(CFLAG) -c TwoColumnLegend.cc

makeFinalPlots.o: makeFinalPlots.cc BackgroundDistribution.h InclusiveSearchBin.h FinalPlot.h SearchBin.h Style.h HistogramReader.h SearchBinManager.h SignalDistribution.h
	g++ $(CFLAG) -c makeFinalPlots.cc

computeSignificance.o: computeSignificance.cc SearchBin.h SearchBinManager.h
	g++ $(CFLAG) -c computeSignificance.cc


clean:
	@rm -f *.o 
	@rm -f makeFinalPlots
	@rm -f computeSignificance
	@rm -f *~
	@rm -f *#
	@rm -f .#*


