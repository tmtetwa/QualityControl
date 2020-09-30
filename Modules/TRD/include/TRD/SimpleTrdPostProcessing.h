
#ifndef QUALITYCONTROL_SIMPLETRDPOSTPROCESSING_H
#define QUALITYCONTROL_SIMPLETRDPOSTPROCESSING_H

#include "QualityControl/PostProcessingInterface.h"

class TH1F;

namespace o2::quality_control_modules::trd
{

/// \brief Example Quality Control Postprocessing Task
/// \author My Name
class SimpleTrdPostProcessing final : public quality_control::postprocessing::PostProcessingInterface
{
 public:
  /// \brief Constructor
  SimpleTrdPostProcessing() = default;
  /// \brief Destructor
  ~SimpleTrdPostProcessing() override;

  /// \brief Initialization of a post-processing task.
  /// Initialization of a post-processing task. User receives a Trigger which caused the initialization and a service
  /// registry with singleton interfaces.
  /// \param trigger  Trigger which caused the initialization, for example Trigger::SOR
  /// \param services Interface containing optional interfaces, for example DatabaseInterface
  void initialize(quality_control::postprocessing::Trigger, framework::ServiceRegistry&) override;
  /// \brief Update of a post-processing task.
  /// Update of a post-processing task. User receives a Trigger which caused the update and a service
  /// registry with singleton interfaces.
  /// \param trigger  Trigger which caused the initialization, for example Trigger::Period
  /// \param services Interface containing optional interfaces, for example DatabaseInterface
  void update(quality_control::postprocessing::Trigger, framework::ServiceRegistry&) override;
  /// \brief Finalization of a post-processing task.
  /// Finalization of a post-processing task. User receives a Trigger which caused the finalization and a service
  /// registry with singleton interfaces.
  /// \param trigger  Trigger which caused the initialization, for example Trigger::EOR
  /// \param services Interface containing optional interfaces, for example DatabaseInterface
  void finalize(quality_control::postprocessing::Trigger, framework::ServiceRegistry&) override;

// private:
//  TH1F* mHistogram = nullptr;
};

} // namespace o2::quality_control_modules::trd

#endif //QUALITYCONTROL_SIMPLETRDPOSTPROCESSING_H
