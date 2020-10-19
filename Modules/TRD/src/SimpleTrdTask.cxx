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
/// \file   SimpleTrdTask.cxx
/// \author My Name
///

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include "TRDBase/Digit.h"
#include "DataFormatsTRD/Constants.h"

#include "QualityControl/QcInfoLogger.h"
#include "TRD/SimpleTrdTask.h"

namespace o2::quality_control_modules::trd
{

SimpleTrdTask::~SimpleTrdTask()
{
    if (mHistogram) {
      delete mHistogram;
    }
    if (mDataSize) {
      delete mDataSize;
    }
    if (mTotalDataVolume) {
      delete mTotalDataVolume;
    }
    if (mLME) {
      delete mLME;
    }
    if (mDet) {
      delete mDet;
    }
    if (mRow) {
      delete mRow;
    }
    if (mPad) {
      delete mPad;
    }
    if (mADC) {
      delete mADC;
    }
    if (mADCperTimeBinAllDetectors) {
      delete mADCperTimeBinAllDetectors;
    }
    if (mprofADCperTimeBinAllDetectors) {
      delete mprofADCperTimeBinAllDetectors;
    }
}

void SimpleTrdTask::initialize(o2::framework::InitContext& /*ctx*/)
{
  ILOG(Info) << "initialize SimpleTrdTask" << ENDM; // QcInfoLogger is used. FairMQ logs will go to there as well.

  // this is how to get access to custom parameters defined in the config file at qc.tasks.<task_name>.taskParameters
  if (auto param = mCustomParameters.find("myOwnKey"); param != mCustomParameters.end()) {
    ILOG(Info) << "Custom parameter - myOwnKey: " << param->second << ENDM;
  }

      mHistogram = new TH1F("trdPayloadSize", "trdPayloadSize", 300, 0, 30000000);
      getObjectsManager()->startPublishing(mHistogram);
      getObjectsManager()->addMetadata(mHistogram->GetName(), "custom", "34");

      mDataSize = new TH1F("trdDataSize", "trdDataSize", 300, 0, 30000000 );
      getObjectsManager()->startPublishing(mDataSize);
      getObjectsManager()->addMetadata(mDataSize->GetName(), "custom", "34");

      mTotalDataVolume = new TH1F("TotalDataVolume", "Total data volume", 300, 0, 30000000);
      getObjectsManager()->startPublishing(mTotalDataVolume);
      getObjectsManager()->addMetadata(mTotalDataVolume->GetName(), "custom", "34");

      mLME = new TH1F("trdLME", "trdLME", 300, 0, 30000000 );
      getObjectsManager()->startPublishing(mLME);
      getObjectsManager()->addMetadata(mLME->GetName(), "custom", "34");

      mDet = new TH1F("hDet", ";Detector number;Counts", 540, 0, 539);
      getObjectsManager()->startPublishing(mDet);
      getObjectsManager()->addMetadata(mDet->GetName(), "custom", "34");

      mRow = new TH1F("hRow", ";Row number;Counts", 16, 0, 15);
      getObjectsManager()->startPublishing(mRow);
      getObjectsManager()->addMetadata(mRow->GetName(), "custom", "34");

      mPad = new TH1F("hPad", ";Pad number;Counts", 144, 0, 143);
      getObjectsManager()->startPublishing(mPad);
      getObjectsManager()->addMetadata(mPad->GetName(), "custom", "34");

      mADC = new TH1F("hADC", ";ADC value;Counts", 1024, 0, 1023);
      getObjectsManager()->startPublishing(mADC);
      getObjectsManager()->addMetadata(mADC->GetName(), "custom", "34");

      mADCperTimeBinAllDetectors = new TH2F("ADCperTimeBinAllDetectors", "ADC distribution for all chambers for each time bin;Time bin;ADC", 31, -0.5, 30.5 , 1014, 0, 1023);
      getObjectsManager()->startPublishing(mADCperTimeBinAllDetectors);
      getObjectsManager()->addMetadata(mADCperTimeBinAllDetectors->GetName(), "custom", "34");

      mprofADCperTimeBinAllDetectors = new TProfile("profADCperTimeBinAllDetectors", "ADC distribution for all chambers for each time bin;Time bin;ADC", 31, -0.5, 30.5);
      getObjectsManager()->startPublishing(mprofADCperTimeBinAllDetectors);
      getObjectsManager()->addMetadata(mprofADCperTimeBinAllDetectors->GetName(), "custom", "34");


}

void SimpleTrdTask::startOfActivity(Activity& /*activity*/)
{
  ILOG(Info) << "startOfActivity" << ENDM;
  mHistogram->Reset();
  mDataSize->Reset();
  mTotalDataVolume->Reset();
  mLME->Reset();
  mDet->Reset();
  mRow->Reset();
  mPad->Reset();
  mADC->Reset();
  mADCperTimeBinAllDetectors->Reset();
  mprofADCperTimeBinAllDetectors->Reset();
} //set stats/stacs

void SimpleTrdTask::startOfCycle()
{
  ILOG(Info) << "startOfCycle" << ENDM;
}

void SimpleTrdTask::monitorData(o2::framework::ProcessingContext& ctx)
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
      ILOG(Info) << "payload size: " << (header->payloadSize) << ENDM;
      mHistogram->Fill(header->payloadSize);
      mTotalDataVolume->Fill(1., header->payloadSize);

      ::o2::trd::Cru2TrackletTranslator ibj;

      auto head = ibj.mhalfcruheader;
      auto link = ibj.mCurrentHalfCRULinkLengths;
      auto flag = ibj.mCurrentHalfCRULinkErrorFlags;

      ILOG(Info) << "\ncru header : " << head  << ENDM;
      for (int i=0; i<16; i++){
            ILOG(Info) << "cru data errors : " << flag[i]  << ENDM;
            ILOG(Info) << "cru data links : " << link[i]  << ENDM;
            mLME->Fill(flag[i]);
            mDataSize->Fill(link[i]);
       }
       mLME->Draw("COLZ");
       mDataSize->Draw("LEGO2");

       const auto inputDigits = ctx.inputs().get<gsl::span<o2::trd::Digit>>("random");
       std::vector<o2::trd::Digit> msgDigits(inputDigits.begin(), inputDigits.end());
       //std::vector<unsigned int> msgDigitsIndex;
        for(auto digit : msgDigits )
        {
          int det = digit.getDetector();
          int row = digit.getRow();
          int pad = digit.getPad();
          auto adcs = digit.getADC();

          ILOG(Info) << " Detectors" << det <<ENDM;

          for (int tb = 0; tb < o2::trd::constants::TIMEBINS; ++tb) {
              int adc = adcs[tb];
              mADC->Fill(adc);
              mADCperTimeBinAllDetectors->Fill(tb, adc);
            }

          mDet->Fill(det);
          mRow->Fill(row);
          mPad->Fill(pad);
          //mADC[det]->Fill(adcs);
         }
         //vbvb->draw("mrow::mpad", "mdetector")

         mDet->Draw("lego");
         mPad->Draw();
         mRow->Draw();
         mADC->Draw();
         //mADC->SetLogy();
         //mADCperTimeBinAllDetectors->Draw("HIST*");
         //mprofADCperTimeBinAllDetectors->SetLineColor();
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

void SimpleTrdTask::endOfCycle()
{
  ILOG(Info) << "endOfCycle" << ENDM;
}

void SimpleTrdTask::endOfActivity(Activity& /*activity*/)
{
  ILOG(Info) << "endOfActivity" << ENDM;
}

void SimpleTrdTask::reset()
{
  // clean all the monitor objects here

  ILOG(Info) << "Resetting the histogram" << ENDM;
  mHistogram->Reset();
  mDataSize->Reset();
  mTotalDataVolume->Reset();
  mLME->Reset();
  mDet->Reset();
  mRow->Reset();
  mPad->Reset();
  mADC->Reset();
  mADCperTimeBinAllDetectors->Reset();
  mprofADCperTimeBinAllDetectors->Reset();
}

} // namespace o2::quality_control_modules::trd
