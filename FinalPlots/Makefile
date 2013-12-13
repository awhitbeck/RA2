ROOTLIBS   = $(shell root-config --libs)
ROOTCFLAGS = $(shell root-config --cflags)

CFLAG      = -I $(ROOTCFLAGS)
LFLAG      = $(ROOTLIBS)

OBJ     = BackgroundDistribution.o FinalPlot.o InclusiveSearchBin.o SearchBin.o makeFinalPlots.o




makeFinalPlots: $(OBJ)		
	g++ $(OBJ) $(LFLAG) -o makeFinalPlots

BackgroundDistribution.o: BackgroundDistribution.h BackgroundDistribution.cc InclusiveSearchBin.h
	g++ $(CFLAG) -c BackgroundDistribution.cc

FinalPlot.o: FinalPlot.h FinalPlot.cc BackgroundDistribution.h
	g++ $(CFLAG) -c FinalPlot.cc

InclusiveSearchBin.o: InclusiveSearchBin.h InclusiveSearchBin.cc SearchBin.h
	g++ $(CFLAG) -c InclusiveSearchBin.cc

SearchBin.o: SearchBin.h SearchBin.cc StringParser.h
	g++ $(CFLAG) -c SearchBin.cc

makeFinalPlots.o: makeFinalPlots.cc BackgroundDistribution.h InclusiveSearchBin.h FinalPlot.h SearchBin.h Style.h HistogramReader.h
	g++ $(CFLAG) -c makeFinalPlots.cc



clean:
	@rm -f *.o 
	@rm -f makeFinalPlots
	@rm -f *~
	@rm -f *#
	@rm -f .#*


