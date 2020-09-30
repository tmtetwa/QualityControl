#include <memory>
#include <random>

// ROOT includes
#include <TH1F.h>
#include <TH2F.h>

// o2 included
#include "Framework/runDataProcessing.h"
#include "DPLUtils/RootTreeReader.h"

// qc includes
#include "QualityControl/CheckRunner.h"
#include "QualityControl/runnerUtils.h"

WorkflowSpec defineDataProcessing(const ConfigContext& config)
{
  WorkflowSpec specs;

  // ===| workflow options |====================================================
  //
  auto inputFile = config.options().get<std::string>("input-file");
  auto treeName = config.options().get<std::string>("tree-name");
  auto branchName = config.options().get<std::string>("branch-name");

  // ===| tree reader |=========================================================
  //
  // The tree reader will read tpc tracks from file crated via the o2 sim/rec
  // workflow
  DataProcessorSpec producer{
    "trd-raw-reader",
    Inputs{},
    Outputs{
      { "TRD", "Raw", 0, Lifetime::Timeframe } },
    AlgorithmSpec{
      (AlgorithmSpec::InitCallback)[inputFile, treeName, branchName](InitContext&){

        // root tree reader
        //constexpr auto persistency = Lifetime::Transient;
        constexpr auto persistency = Lifetime::Timeframe;

  auto reader = std::make_shared<RootTreeReader>(treeName.data(),                      // tree name
                                                 inputFile.data(),                     // input file name
                                                 RootTreeReader::PublishingMode::Loop, // loop over
                                                 Output{ "TRD", "Raw", 0, persistency },
                                                 branchName.data() // name of the branch
  );

  return (AlgorithmSpec::ProcessCallback)[reader](ProcessingContext & processingContext) mutable
  {
    //(++(*reader))(processingContext);
    if (reader->next()) {
      (*reader)(processingContext);
      //LOG(INFO) << "Call producer AlgorithmSpec::ProcessCallback:  has data " << reader->getCount();
    } else {
      //LOG(INFO) << "Call producer AlgorithmSpec::ProcessCallback:  no next data" << reader->getCount();
    }
  };
}
}
}
;

specs.push_back(producer);

return specs;
}