#include <string>
#include "TH1.h"
#include "SignalDistribution.h"


SignalDistribution::SignalDistribution(const std::string &name, const std::string &var, const TH1* h)
  : name_(name), var_(var), label_(name), color_(1) {
  hOrig_ = static_cast<TH1*>(h->Clone(("SigDist_"+var).c_str()));
}


SignalDistribution::~SignalDistribution() {
  delete hOrig_;
}

