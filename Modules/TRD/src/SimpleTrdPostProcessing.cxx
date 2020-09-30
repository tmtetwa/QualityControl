///
/// \file   PostProcessingInterface.cxx
/// \author My Name
///

#include "TRD/SimpleTrdPostProcessing.h"

using namespace o2::quality_control::postprocessing;

namespace o2::quality_control_modules::trd
{

SimpleTrdPostProcessing::~SimpleTrdPostProcessing()
{
}

void SimpleTrdPostProcessing::initialize(Trigger, framework::ServiceRegistry&)
{
}

void SimpleTrdPostProcessing::update(Trigger, framework::ServiceRegistry&)
{
}

void SimpleTrdPostProcessing::finalize(Trigger, framework::ServiceRegistry&)
{
}

} // namespace o2::quality_control_modules::trd
