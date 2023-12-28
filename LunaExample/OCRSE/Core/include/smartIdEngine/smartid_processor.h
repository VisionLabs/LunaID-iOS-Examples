/*
Copyright (c) 2012-2020, Smart Engines Ltd
All rights reserved.
*/

/**
 * @file smartid_processor.h
 * @brief Class for separate processing string fields in SmartIdEngine
 */

#ifndef SMARTID_ENGINE_SMARTID_PROCESSOR_H_INCLUDED_
#define SMARTID_ENGINE_SMARTID_PROCESSOR_H_INCLUDED_

#if defined _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4290)
#endif

#include <string>
#include <vector>

#include "smartid_common.h"
#include "smartid_result.h"

namespace se { namespace smartid {

class SMARTID_DLL_EXPORT ProcessorEngine {

public:
  /**
   * @brief ProcessorEngine ctor from configuration path
   * @param config_path - path to configuration file
   * @param lazy_configuration - whether to use engine's
   *    lazy component configuration capabilities
   *
   * @throws std::exception if configuration error occurs
   */
  ProcessorEngine(const std::string& config_path,
                  bool lazy_configuration = true) throw(std::exception);

  /**
   * @brief ProcessorEngine ctor from configuration buffer. Only for
   *        configuration from ZIP archive buffers.
   * @param config_data - pointer to configuration ZIP buffer start
   * @param data_length - size of the configuration ZIP buffer
   * @param lazy_configuration - whether to use engine's
   *    lazy component configuration capabilities
   *
   * @throws std::exception if configuration error occurs
   */
  ProcessorEngine(unsigned char* config_data,
                  size_t data_length,
                  bool lazy_configuration = true) throw(std::exception);

  /// ProcessorEngine dtor
  ~ProcessorEngine();

  /// Returns all processors key names
  std::vector<std::string> GetProcessorsNames() const;

  /// Checks if there is a processor with given key name
  bool HasProcessor(const std::string& name) const;

  /**
   * @brief Processes given string fields map by processor with given key name
   * @return string fields map
   */
  std::map<std::string, StringField> ProcessFields(
      const std::string& processor_name,
      std::map<std::string, StringField>& string_fields);

  /**
   * @brief Gets ProcessorEngine library version
   * @return std::string version representation
   */
  static std::string GetVersion();

  /**
   * @brief Auxiliary function - gets list of string fields collection keys
   * @param string_fields - collection of string fields
   * @return list of collection keys
   */
  static std::vector<std::string> GetKeys(
      const std::map<std::string, StringField>& string_fields);

private:
  /// Disabled copy constructor
  ProcessorEngine(const ProcessorEngine& copy);
  /// Disabled assignment operator
  void operator=(const ProcessorEngine& other);

private:
  class ProcessorEngineImpl* pimpl_; ///< pointer to internal implementation
};

} } // namespace se::smartid

#if defined _MSC_VER
#pragma warning(pop)
#endif

#endif // SMARTID_ENGINE_SMARTID_PROCESSOR_H_INCLUDED_
