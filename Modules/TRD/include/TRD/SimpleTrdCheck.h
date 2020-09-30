///
/// \file   SimpleTrdCheck.h
/// \author My Name
///

#ifndef QC_MODULE_TRD_SIMPLETRDCHECK_H
#define QC_MODULE_TRD_SIMPLETRDCHECK_H

#include "QualityControl/CheckInterface.h"
#include <TFile.h>

namespace o2::quality_control_modules::trd
{

/// \brief  TRD Check
/// \author My Name
class SimpleTrdCheck : public o2::quality_control::checker::CheckInterface
{
 public:
  /// Default constructor
  SimpleTrdCheck() = default;
  /// Destructor
  ~SimpleTrdCheck() override = default;

  // Override interface
  void configure(std::string name) override;
  Quality check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap) override;
  void beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult = Quality::Null) override;
  std::string getAcceptedType() override;

  ClassDefOverride(SimpleTrdCheck, 1);
};

} // namespace o2::quality_control_modules::trd

#endif // QC_MODULE_TRD_SIMPLETRDCHECK_H
