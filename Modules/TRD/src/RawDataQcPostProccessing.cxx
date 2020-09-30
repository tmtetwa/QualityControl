// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   PostProcessingInterface.cxx
/// \author My Name
///

#include "TRD/RawDataQcPostProccessing.h"
#include "QualityControl/QcInfoLogger.h"

using namespace o2::quality_control::postprocessing;

namespace o2::quality_control_modules::trd
{

RawDataQcPostProccessing::~RawDataQcPostProccessing()
{
}

void RawDataQcPostProccessing::initialize(Trigger, framework::ServiceRegistry&)
{
}

void RawDataQcPostProccessing::update(Trigger t, framework::ServiceRegistry&)
{
  ILOG(Info) << "Trigger type is: " << t.triggerType << ", the timestamp is " << t.timestamp << ENDM;
}

void RawDataQcPostProccessing::finalize(Trigger, framework::ServiceRegistry&)
{
}

} // namespace o2::quality_control_modules::trd
