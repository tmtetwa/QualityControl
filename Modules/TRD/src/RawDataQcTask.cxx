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
/// \file   RawDataQcTask.cxx
/// \author My Name
///

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include "TRDRaw/Cru2TrackletTranslator.h"
#include "DataFormatsTRD/RawData.h"
#include "DetectorsRaw/RDHUtils.h"
//#include "Headers/RAWDataHeader.h"
#include "Headers/RDHAny.h"

#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <iostream>

#include "QualityControl/QcInfoLogger.h"
#include "TRD/RawDataQcTask.h"

namespace o2::quality_control_modules::trd
{

RawDataQcTask::~RawDataQcTask()
{
  if (mHistogram) {
    delete mHistogram;
  }
}

void RawDataQcTask::initialize(o2::framework::InitContext& /*ctx*/)
{
  ILOG(Info) << "initialize RawDataQcTask" << ENDM; // QcInfoLogger is used. FairMQ logs will go to there as well.

  // o2::trd::getlinkdatasizes(mCurrentHalfCRUHeader,mCurrentHalfCRULinkLengths);
  // o2::trd::getlinkerrorflags(mCurrentHalfCRUHeader,mCurrentHalfCRULinkErrorFlags);

  // this is how to get access to custom parameters defined in the config file at qc.tasks.<task_name>.taskParameters
  if (auto param = mCustomParameters.find("myOwnKey"); param != mCustomParameters.end()) {
    ILOG(Info) << "Custom parameter - myOwnKey: " << param->second << ENDM;
  }
     mHistogram = new TH1F("Adc", "ADC", 1024,-0.5, 1023.5);

  // mHistogram = new TH2F("hmcm", "hmcm;hc;mcm",
  //                       1080, -0.5, 1079.5,
  //                       128, -0.5, 127.5););
  getObjectsManager()->startPublishing(mHistogram);
  try {
    getObjectsManager()->addMetadata(mHistogram->GetName(), "custom", "34");
  } catch (...) {
    // some methods can throw exceptions, it is indicated in their doxygen.
    // In case it is recoverable, it is recommended to catch them and do something meaningful.
    // Here we don't care that the metadata was not added and just log the event.
    ILOG(Warning) << "Metadata could not be added to " << mHistogram->GetName() << ENDM;
  }
}

void RawDataQcTask::startOfActivity(Activity& activity)
{
  ILOG(Info) << "startOfActivity : " << activity.mId << ENDM;
  mHistogram->Reset();
}

void RawDataQcTask::startOfCycle()
{
  ILOG(Info) << "startOfCycle" << ENDM;
}

void RawDataQcTask::monitorData(o2::framework::ProcessingContext& ctx)
{
  // In this function you can access data inputs specified in the JSON config file, for example:
  //   "query": "random:ITS/RAWDATA/0"
  // which is correspondingly <binding>:<dataOrigin>/<dataDescription>/<subSpecification
  // One can also access conditions from CCDB, via separate API (see point 3)

  // Use Framework/DataRefUtils.h or Framework/InputRecord.h to access and unpack inputs (both are documented)
  // One can find additional examples at:
  // https://github.com/AliceO2Group/AliceO2/blob/dev/Framework/Core/README.md#using-inputs---the-inputrecord-api

  // Some examples:

  // 1. In a loop
  for (auto&& input : ctx.inputs()) {
    // get message header
    if (input.header != nullptr && input.payload != nullptr) {
      const auto* header = header::get<header::DataHeader*>(input.header);
      // get payload of a specific input, which is a char array.
      // const char* payload = input.payload;

      // for the sake of an example, let's fill the histogram with payload sizes
      mHistogram->Fill(header->payloadSize);
    }
  }

  // 2. Using get("<binding>")

  // get the payload of a specific input, which is a char array. "random" is the binding specified in the config file.
  //   auto payload = ctx.inputs().get("random").payload;

  // get payload of a specific input, which is a structure array:
  //  const auto* header = header::get<header::DataHeader*>(ctx.inputs().get("random").header);
  //  struct s {int a; double b;};
  //  auto array = ctx.inputs().get<s*>("random");
  //  for (int j = 0; j < header->payloadSize / sizeof(s); ++j) {
  //    int i = array.get()[j].a;
  //  }

  // get payload of a specific input, which is a root object
  //   auto h = ctx.inputs().get<TH1F*>("histos");
  //   Double_t stats[4];
  //   h->GetStats(stats);
  //   auto s = ctx.inputs().get<TObjString*>("string");
  //   LOG(INFO) << "String is " << s->GetString().Data();

  // 3. Access CCDB. If it is enough to retrieve it once, do it in initialize().
  // Remember to delete the object when the pointer goes out of scope or it is no longer needed.
  //   TObject* condition = TaskInterface::retrieveCondition("QcTask/example"); // put a valid condition path here
  //   if (condition) {
  //     LOG(INFO) << "Retrieved " << condition->ClassName();
  //     delete condition;
  //   }
}

void RawDataQcTask::endOfCycle()
{
  ILOG(Info) << "endOfCycle" << ENDM;
}

void RawDataQcTask::endOfActivity(Activity& /*activity*/)
{
  ILOG(Info) << "endOfActivity" << ENDM;
}

void RawDataQcTask::reset()
{
  // clean all the monitor objects here

  ILOG(Info) << "Resetting the histogram" << ENDM;
  mHistogram->Reset();
}

} // namespace o2::quality_control_modules::trd
