// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// StaticNavigationEngine.h, ACTS project
///////////////////////////////////////////////////////////////////

#ifndef ACTS_EXTRAPOLATION_STATICNAVIGATIONENGINE_H
#define ACTS_EXTRAPOLATION_STATICNAVIGATIONENGINE_H 1

#include "ACTS/EventData/NeutralParameters.hpp"
#include "ACTS/EventData/TrackParameters.hpp"
#include "ACTS/Extrapolation/ExtrapolationCell.hpp"
#include "ACTS/Extrapolation/IMaterialEffectsEngine.hpp"
#include "ACTS/Extrapolation/INavigationEngine.hpp"
#include "ACTS/Extrapolation/IPropagationEngine.hpp"
#include "ACTS/Extrapolation/detail/ExtrapolationMacros.hpp"
#include "ACTS/Utilities/Logger.hpp"
#include "ACTS/Volumes/BoundarySurfaceT.hpp"

namespace Acts {

class TrackingGeometry;

/// @class StaticNavigationEntine
///
/// The static navigation engine for finding the next volume,
/// propagate to the boundary, can be shared with other engines that have a
/// static frame.
///
class StaticNavigationEngine : virtual public INavigationEngine
{
public:
  /// @struct Config
  /// NEsted configuration struct for the StaticNavigationEngine
  struct Config
  {
    /// the used propagation engine
    std::shared_ptr<const IPropagationEngine> propagationEngine = nullptr;
    /// the material effects updator
    std::shared_ptr<IMaterialEffectsEngine> materialEffectsEngine = nullptr;
    /// the tracking geometry cache
    std::shared_ptr<const TrackingGeometry> trackingGeometry = nullptr;
    /// output prefix
    std::string prefix = "[SN] - ";
    /// output postfix
    std::string postfix = " - ";
  };

  /// Constructor
  ///
  /// @param snConfig is the configuration struct to steer behaviour
  /// @param logger logging instance
  StaticNavigationEngine(const Config&           snConfig,
                         std::unique_ptr<Logger> logger
                         = getDefaultLogger("StaticNavigationEngine",
                                            Logging::INFO));

  /// Destructor
  ~StaticNavigationEngine();

  /// avoid method shaddowing
  using INavigationEngine::resolveBoundary;
  using INavigationEngine::resolvePosition;

  /// Resolve the boundary situation - for charged particles
  ///
  /// @param ecCell is the charged extrapolation cell
  /// @param dir is the additional direction prescription
  ///
  /// @return is a extrapolation code indication
  ExtrapolationCode
  resolveBoundary(ExCellCharged& ecCell,
                  PropDirection  dir = alongMomentum) const final;

  /// Resolve the boundary situation - for neutral particles
  ///
  /// @param enCell is the neutral extrapolation cell
  /// @param dir is the additional direction prescription
  ///
  /// @return is a extrapolation code indication
  ExtrapolationCode
  resolveBoundary(ExCellNeutral& enCell,
                  PropDirection  dir = alongMomentum) const final;

  /// Resolve the boundary situation - for charged particles
  ///
  /// @param ecCell is the charged extrapolation cell
  /// @param dir is the additional direction prescription
  /// @param noLoop is a loop protection @todo check with ST                
  ///
  /// @return is a extrapolation code indication
  ExtrapolationCode
  resolvePosition(ExCellCharged& ecCell,
                  PropDirection  dir    = alongMomentum,
                  bool           noLoop = false) const final;

  /// Resolve the boundary situation - for neutral particles
  ///
  /// @param enCell is the neutral extrapolation cell
  /// @param dir is the additional direction prescription
  /// @param noLoop is a loop protection @todo check with ST                
  ///
  /// @return is a extrapolation code indication
  ExtrapolationCode
  resolvePosition(ExCellNeutral& enCell,
                  PropDirection  dir    = alongMomentum,
                  bool           noLoop = false) const final;

  /// Set configuration method
  ///
  /// @param snConfig the configuration object to be set
  void
  setConfiguration(const Config& snConfig);

  /// Get configuration method
  Config
  getConfiguration() const;

  /// Set logging instance
  ///
  /// @param logger the logging instance to be seet
  void
  setLogger(std::unique_ptr<Logger> logger);

protected:
  /// the configuration member of the static navigation engine
  Config m_cfg;

private:
  /// Private access to the logging instance
  const Logger&
  logger() const
  {
    return *m_logger;
  }

  std::unique_ptr<Logger> m_logger;

  /// Resolve the boundary situation
  ///
  /// @param eCell the extrapolation
  /// @param dir the propagation direction
  ///
  /// @return is a extrapolation code indication
  template <class T>
  ExtrapolationCode
  resolveBoundaryT(ExtrapolationCell<T>& eCell,
                   PropDirection         dir = alongMomentum) const;

  /// Resolve position
  ///
  /// @param eCell the extrapolation
  /// @param dir the propagation direction
  /// @param noLoop
  /// @todo check with sharka
  ///
  /// @return is a extrapolation code indication
  template <class T>
  ExtrapolationCode
  resolvePositionT(ExtrapolationCell<T>& eCell,
                   PropDirection         dir    = alongMomentum,
                   bool                  noLoop = false) const;

  /// Deal with the boundary Surface - called by resolveBoundary
  ///
  /// @param eCell the extrapolation
  /// @param bSurfaceTV the boundary surface
  /// @param dir the propagation direction
  /// @param stepout  is a prescription to step out the volume
  ///
  /// @return is a extrapolation code indication
  template <class T>
  ExtrapolationCode
  handleBoundaryT(ExtrapolationCell<T>&                   eCell,
                  const BoundarySurfaceT<TrackingVolume>& bSurfaceTV,
                  PropDirection                           dir = alongMomentum,
                  bool stepout                             = false) const;
};

inline StaticNavigationEngine::Config
StaticNavigationEngine::getConfiguration() const
{
  return m_cfg;
}

}  // end of namespace

#include "ACTS/Extrapolation/detail/StaticNavigationEngine.ipp"

#endif  // ACTS_EXTRAPOLATION_STATICNAVIGATIONENGINE_H
