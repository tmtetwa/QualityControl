#include <DataSampling/DataSampling.h>
#include "QualityControl/InfrastructureGenerator.h"


using namespace o2;
using namespace o2::framework;
using namespace o2::utilities;

// The customize() functions are used to declare the executable arguments and to specify custom completion and channel
// configuration policies. They have to be above `#include "Framework/runDataProcessing.h"` - that header checks if
// these functions are defined by user and if so, it invokes them. It uses a trick with SFINAE expressions to do that.

void customize(std::vector<CompletionPolicy>& policies)
{
  DataSampling::CustomizeInfrastructure(policies);
  quality_control::customizeInfrastructure(policies);
}

void customize(std::vector<ChannelConfigurationPolicy>& policies)
{
  DataSampling::CustomizeInfrastructure(policies);
}

void customize(std::vector<ConfigParamSpec>& workflowOptions)
{
  workflowOptions.push_back(
    ConfigParamSpec{ "config-path", VariantType::String, "", { "SimpleTrd.json" } });
  workflowOptions.push_back(
    ConfigParamSpec{ "no-data-sampling", VariantType::Bool, false, { "Skips data sampling, connects directly the task to the producer." } });
}

#include <string>

#include <Framework/runDataProcessing.h>
#include <Configuration/ConfigurationFactory.h>

#include "QualityControl/CheckRunner.h"
#include "QualityControl/InfrastructureGenerator.h"
#include "QualityControl/runnerUtils.h"
#include "QualityControl/ExamplePrinterSpec.h"
#include "QualityControl/DataProducer.h"
#include "QualityControl/TaskRunner.h"

std::string getConfigPath(const ConfigContext& config);

using namespace o2;
using namespace o2::framework;
using namespace o2::quality_control::checker;
using namespace std::chrono;

WorkflowSpec defineDataProcessing(const ConfigContext& config)
{
  WorkflowSpec specs;

  // The producer to generate some data in the workflow
  DataProcessorSpec producer = getDataProducerSpec(1, 10000, 10);
  specs.push_back(producer);

  // Path to the config file
  std::string qcConfigurationSource = getConfigPath(config);
  LOG(INFO) << "Using config file '" << qcConfigurationSource << "'" << ENDM;

  // Generation of Data Sampling infrastructure
  DataSampling::GenerateInfrastructure(specs, qcConfigurationSource);

  // Generation of the QC topology (one task, one checker in this case)
  quality_control::generateRemoteInfrastructure(specs, qcConfigurationSource);

  // Finally the printer
  if (hasChecks(qcConfigurationSource)) {
    DataProcessorSpec printer{
      "printer",
      Inputs{
        { "checked-mo", "QC", CheckRunner::createCheckRunnerDataDescription(getFirstCheckName(qcConfigurationSource)), 0 } },
      Outputs{},
      adaptFromTask<o2::quality_control::example::ExampleQualityPrinterSpec>()
    };
    specs.push_back(printer);
  } else {
    DataProcessorSpec printer{
      "printer",
      Inputs{
        { "checked-mo", "QC", TaskRunner::createTaskDataDescription(getFirstTaskName(qcConfigurationSource)), 0 } },
      Outputs{},
      adaptFromTask<o2::quality_control::example::ExamplePrinterSpec>()
    };
    specs.push_back(printer);
  }

  return specs;
}

// TODO merge this with the one from runReadout.cxx
std::string getConfigPath(const ConfigContext& config)
{
  // Determine the default config file path and name (based on option no-data-sampling and the QC_ROOT path)
  bool noDS = config.options().get<bool>("no-data-sampling");
  std::string filename = !noDS ? "SimpleTrd.json" : "basic-no-sampling.json";
  std::string defaultConfigPath = getenv("QUALITYCONTROL_ROOT") != nullptr ? std::string(getenv("QUALITYCONTROL_ROOT")) + "/etc/" + filename : "$QUALITYCONTROL_ROOT undefined";
  // The the optional one by the user
  auto userConfigPath = config.options().get<std::string>("config-path");
  // Finally build the config path based on the default or the user-base one
  std::string path = std::string("json:/") + (userConfigPath.empty() ? defaultConfigPath : userConfigPath);
  return path;
 }
