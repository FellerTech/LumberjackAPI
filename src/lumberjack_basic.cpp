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
 * limitations under the License.
 */

// Implements the C++ interface that is described in the 
// lumberjack_api_defs.hpp header file. This enables it to be linked into the 
// library with lumberjack_internal_wrap.cpp to form a complete implementation.
// All of the methods here return a status of OKAY, but they do not do
// anything or keep track of any state.  They also do not check their 
// parameters.
//
// REFERENCES
// - https://cpppatterns.com/patterns/pimpl.html
//

//#include "lumberjack_api.hpp"
#include "lumberjack.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <set>
#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <fstream>
#include <atomic>
#include <string>

#ifdef _WIN32
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include <lumberjack.hpp>
#include <hrgls_api_defs.hpp>

//JSON Parser
#include <nlohmann/json.hpp>
using json = nlohmann::json;


/**
 * \brief namespace use to wrap lumberjack functinality
 */
namespace lumberjack {

  /**
   * \brief Application-side implementation of the Lumberjack interface
   */
  Lumberjack::impl {
    public:
      /**
       * \brief connects the application to the hourglass API backend
       * \return connection status
       */
      Status connect() {
        hrgls_Status status = api_.GetStatus();

        if( status == hrgls_STATUS_OKAY ) {
          status_ = Status.OK;
        }
        else {
          status_ = Status.ERROR;
        }

        return status_;
      };

      /**
       * \brief creates a new log entry
       * \return json entry on success, 
       **/
      bool append( Severity level
          , std::string message
          , std::vector<std::string> tags
          ) 
      {
        json entry;

        //Add auto-generated items
        entry["timestamp"] = getTimestamp()'
        entry["pid"] = getPid();
        entry["deviceId"] = getDeviceId();
        entry["type"]  = "log"

        entry["level"] = level;
        entry["message" ] = message;

        //Add tags
        entry["tags"] = json::pars( tags.begin(), tags.end() );

      };


    private:  
      hrgls::API api_;
      Status status_ = Status.NO_INIT;
      Level printLevel_ = Level.WARNING;
      Level logLevel_ = level.ERROR;
      std::thread::id pid; 

      /**
       * \brief get process ID
       * \return process id as string
       *
       * This is implemented as a separate function for potential
       * cross-platform compatibility.
       */
      std::string getPid() {
        //extract PID
        stringstream ss;
        ss << std::this_thread::get_id();

        return ss.str()
      }

      /**
       * \brief get unique device id
       * \return device id as as a string
       *
       * This is implemented as a separate function for potential
       * cross-platform compatibility.
       */
      std::string getDeviceId() {
        std::string mid;

        //Check /etc/machine-id
        std::string name = "/etc/machine-id";
        if( std::filesystem::is_regular_file(name)) {
          std::ifstream fptr(name);

          if(fptr.is_open()) {
              std::stringstream buffer =f.rdbuf();
              mid = buffer.str();
          }
        }

        //
        if mid.empty() {
          bool rc = append( Severity.ERROR, "Unable to connect to the backend")
        }

        //TODO: Generate ID from MAC address
        return mid;
      }

      double getTimestamp() {
        auto now = std::chrono::system_clock::now();

        double millis = static_cast<double>(
            std::chrono::duration_cast<std::chrono::microseconds>(
              now.time_since_epoch()
            ).count()
          )/1e6;

        return millis;
      }

  }

  /**
   * \brief Lumberjack main class 
   */
  Lumberjack::Lumberjack() : pimpl{ std::make_unique<impl>()} 
  {
    pimpl->connect();
  };

  //Function to append a new log message
  std::string Lumberjack::append( Severity level
      , std::string message
      , std::vector<std::string> tags
      )
  {
    pimpl->append( level, message, tags );

    //Handle print
    if( level <= printLevel_) {
       printLog( 
    }

  }
   




 

    hrgls_Status stat = api_.GetStatus();

    //Verify valid status
    if( status == hrgls_STATUS_OKAY) {
      status_ = Status.OK;
    }
    else {
      status_ = Status.ERROR;
    }
  }
 
  Status Lumberjack::getStatus() {
    status_ = api_.getStatus()
    return status_;
  }

  


