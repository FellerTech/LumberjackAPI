/*
 * Copyright 2022 FellerTech LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License
 **/

#include <string>
#include <vector>
#include <syslog.h>

//JSON Parser
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace lumberjack {
  enum Severity{ CRITICAL, ERROR, WARNING, INFO, DEBUG, TRACE, ALL };
  enum class PayloadType { STRING, BINARY };
  enum Status{ OK, NO_INIT, ERR, INCOMPATIBLE};

  /**
   * \brief the lumberjack base class provides common functionality used by the
   * logging system and data interface applications.
   **/
  class Lumberjack {
    public:
      Lumberjack();
      ~Lumberjack();
      /**
       * \brief get API version
       * \return version in a major/minor/patch/release format
       **/
      std::string getVersion();
      Status getStatus();


      /**
       * \brief get the current timestamp
       * \return current timestamp as a double
       * 
       * This function provides a consistent timing reference for the logging
       * system.
       *
       **/
      double getTimestamp();

      /**
       * \brief  Function to insert a new string-based log message.
       * \param [in] level the enumerated Log level of the issue
       * \param [in] message text information about the event.
       * \param [in] tags vector of keywords related to the error
       * \return unique ID of the entry on success, empty string on failure
       **/
      std::string append( Severity level
          , std::string message
          );

      /**
       * \brief  Function to insert a new string-based log message.
       * \param [in] level the enumerated Log level of the issue
       * \param [in] message text information about the event.
       * \param [in] tags vector of keywords related to the error
       * \return unique ID of the entry on success, empty string on failure
       **/
      std::string append( Severity level
          , std::string message
          , std::vector<std::string> tags
          );

      /**
       * \brief  Function to insert a new string-based log message.
       * \param [in] level the enumerated Log level of the issue
       * \param [in] message text information about the event.
       * \param [in] module name of the module or function appending data
       * \param [in] tags vector of keywords related to the error
       * \return unique ID of the entry on success, empty string on failure
       **/
      std::string append( Severity level
          , std::string message
          , std::string module
          , std::vector<std::string> tags
          );

      /**
       * \brief function to append a tag to an entry
       * \param [in] id uid of the entry to add a tag to
       * \param [in] tag short text for the tag
       * \return true on success, false on failure
       **/
      bool appendTag( std::string id
            , std::string tag
          );

      /**
       * \brief sets the log level for the logging module
       * \param [in] level enumerated severity level
       * \return true on success, false on failure
       * 
       * Once the severity is set, all incoming messages at that serverity or
       * higher will be sent to the logging system.
       **/
      bool setLogLevel( Severity level );

      /**
       * \brief gets the current log level of the logging module
       * \return enumerated log level for the logging system.
       **/
      Severity getLogLevel( void );

      /**
       * \brief sets the print level for the logging module
       * \param [in] level enumerated severity level
       * \return true on success, false on failure
       * 
       * Once the severity is set, all incoming messages at that serverity or
       * higher will be printed in the system.
       **/
      bool setPrintLevel( Severity level );

      /**
       * \brief gets the current print level of the logging module
       * \return enumerated Severity level 
       **/
      Severity getPrintLevel( void );


    private:
      //pimpl setup
      //https://cpppatterns.com/patterns/pimpl.html
      class impl;
      std::unique_ptr<impl> pimpl;
  };
}



/*
void HandleCallback(hrgls::datablob::DataBlob &blob, void *userData)
{
  // Set *done when it is time to quit.
  bool *done = static_cast<bool*>(userData);

  // Do whatever we want with the blob.
  /// @todo Replace this code with whatever is desired.
  static size_t count = 0;
  if (++count >= 10) {
    *done = true;
  }

  // Release the data from the blob.
  blob.ReleaseData();
  hrgls_Status status = blob.GetStatus();
  if (status != hrgls_STATUS_OKAY) {
    std::cerr << "Could not release blob data in callback: "
      << hrgls_ErrorMessage(status) << std::endl;
    *done = true;
  }
}

int main(int argc, const char *argv[])
{
  // Get a base API object and DataBlobSource, specifying default parameters.
  hrgls::API api;
  hrgls_Status status = api.GetStatus();
  if (status != hrgls_STATUS_OKAY) {
    std::cerr << "Could not Open API: "
      << hrgls_ErrorMessage(status) << std::endl;
    return 1;
  }

  // Create a stream using default stream properties.
  hrgls::StreamProperties sp;
  status = sp.GetStatus();
  if (status != hrgls_STATUS_OKAY) {
    std::cerr << "Could not create stream properties: "
      << hrgls_ErrorMessage(status) << std::endl;
    return 8;
  }
  hrgls::datablob::DataBlobSource *stream = new hrgls::datablob::DataBlobSource(
    api, sp);
  status = stream->GetStatus();
  if (status != hrgls_STATUS_OKAY) {
    std::cerr << "Could not create stream: "
      << hrgls_ErrorMessage(status) << std::endl;
    return 9;
  }

  { /// This shows how to read the blobs using a callback handler.
    std::cout << "Callback-based blob reading" << std::endl;

    // Set a callback handler for incoming blobs and then start streaming.
    bool done = false;
    if (stream->SetStreamCallback(HandleBlobCallback, &done) != hrgls_STATUS_OKAY) {
      std::cerr << "Could not set callback handler: "
        << hrgls_ErrorMessage(status) << std::endl;
      return 11;
    }
    if (stream->SetStreamingState(true) != hrgls_STATUS_OKAY) {
      std::cerr << "Could not set streaming state on: "
        << hrgls_ErrorMessage(status) << std::endl;
      return 12;
    }

    // Run until the callback handler sets done.
    while (!done) {};

    // Unhook the callback handler after stopping the stream.
    if (stream->SetStreamingState(false) != hrgls_STATUS_OKAY) {
      std::cerr << "Could not set streaming state off: "
        << hrgls_ErrorMessage(status) << std::endl;
      return 13;
    }
    if (stream->SetStreamCallback(nullptr, nullptr) != hrgls_STATUS_OKAY) {
      std::cerr << "Could not reset callback handler: "
        << hrgls_ErrorMessage(status) << std::endl;
      return 14;
    }
  }

  { /// This shows how to read the blobs without a callback handler.
    std::cout << "Get-based blob reading" << std::endl;

    // Start streaming
    if (stream->SetStreamingState(true) != hrgls_STATUS_OKAY) {
      std::cerr << "Could not set streaming state on: "
        << hrgls_ErrorMessage(status) << std::endl;
      return 15;
    }

    size_t count = 0;
    do {
      hrgls::datablob::DataBlob blob = stream->GetNextBlob();
      status = stream->GetStatus();
      if (hrgls_STATUS_OKAY == status) {

        // Do whatever we want with the blob.
        /// @todo Replace the print and increment with the work we want to do.
        if (blob.Size() >= 2) {
          std::cout << " first character = " << static_cast<int>(blob.Data()[0]) << std::endl;
          std::cout << " second character = " << static_cast<int>(blob.Data()[1]) << std::endl;
        }
        count++;

        // Release the data from the blob.
        blob.ReleaseData();
        hrgls_Status status = blob.GetStatus();
        if (status != hrgls_STATUS_OKAY) {
          std::cerr << "Could not release blob data in main program: "
            << hrgls_ErrorMessage(status) << std::endl;
          return 17;
        }
      } else if (status != hrgls_STATUS_TIMEOUT) {
        std::cerr << "Bad blob received: " << hrgls_ErrorMessage(status) << std::endl;
        return 18;
      }

    } while (count < 10);

    // Stop streaming
    if (stream->SetStreamingState(false) != hrgls_STATUS_OKAY) {
      std::cerr << "Could not set streaming state off: "
        << hrgls_ErrorMessage(status) << std::endl;
      return 19;
    }
  }

  delete stream;

  return 0;
}
*/
