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
/// \file   SimpleTrdTask.h
/// \author Tokozani Mtetwa
///

#ifndef QC_MODULE_TRD_TRDSIMPLETRDTASK_H
#define QC_MODULE_TRD_TRDSIMPLETRDTASK_H

#include "QualityControl/TaskInterface.h"
#include "DataFormatsTRD/RawData.h"
#include "TRDRaw/Cru2TrackletTranslator.h"
#include <cstdint>
#include <array>
#include <fstream>
#include <string>

class TH1F;
class TH2F;


using namespace o2::quality_control::core;

namespace o2::quality_control_modules::trd
{

/// \brief Example Quality Control DPL Task
/// \author My Name
class SimpleTrdTask final : public TaskInterface
{
 public:
  /// \brief Constructor
  SimpleTrdTask() = default;
  /// Destructor
  ~SimpleTrdTask() override;

  // Definition of the methods for the template method pattern
  void initialize(o2::framework::InitContext& ctx) override;
  void startOfActivity(Activity& activity) override;
  void startOfCycle() override;
  void monitorData(o2::framework::ProcessingContext& ctx) override;
  void endOfCycle() override;
  void endOfActivity(Activity& activity) override;
  void reset() override;

 private:
  TH1F* mHistogram = nullptr;
  TH1F* mLME = nullptr;
  TH1F* mDataSize = nullptr;
  TH1F* mTotalDataVolume = nullptr;
  std::array<uint32_t, 15> mCRULinkLengths;
  ::o2::trd::HalfCRUHeader mHalfCRUHeader;
  ::o2::trd::TrackletHCHeader mTracklet;
  //::o2::trd::getlinkdatasize mlinks;

};

} // namespace o2::quality_control_modules::trd

#endif // QC_MODULE_TRD_TRDSIMPLETRDTASK_H
