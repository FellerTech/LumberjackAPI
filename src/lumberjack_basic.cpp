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
#include <iostream>
#include <sstream>
#include <filesystem>
#include <memory>
#include <stdlib.h>

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

#include <functional>

#ifdef _WIN32
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include <FTTimer.hpp>

#include <lumberjack.hpp>
#include <hrgls_api_defs.hpp>

//JSON Parser
#include <nlohmann/json.hpp>
using json = nlohmann::json;


/**
 * \brief make_unique replacement
 */
namespace FT {
  template<typename   T, typename... Args>
  std::unique_ptr<T>   make_unique(Args&&... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

/**
 * \brief namespace use to wrap lumberjack functinality
 */
namespace lumberjack {

  void HandleCallback( hrgls::Message &message
      , void * userData
      )
  {
    std::cout << "received message";
  }


  /**
   * \brief internal implementation class
   */
  class Lumberjack::impl {
    public:
      impl() {
        version_ = LJ_VERSION;
        hash_ = LJ_HASH;


      }

      /**
       * \brief callback for receiving hourglass messages
       * \param [in] message payload of the function
       * \param [in] userData pointer to data passed throug
       */
      static void  HGMessageCallback(hrgls::Message &message
          , void * userData 
          )
      {
         size_t * count = static_cast<size_t *>(userData );

         std::cout << *count <<": "<< message.Value() <<std::endl;
      }

      /**
       * \brief callback for receiving hourglass messages
       */
      static void  HGStreamCallback(hrgls::datablob::DataBlob &blob
          , void * userData 
          )
      {
        std::cout << "message received"<<std::endl;
      }



      /**
       * \brief connects the application to the hourglass API backend
       * \return connection status
       */
      Status connect() {
        hrgls_Status status = api_.GetStatus();

        if( status == hrgls_STATUS_OKAY ) {
          status_ = OK;
        }
        else {
          status_ = ERR;
          return status_;
        }

        //Create a point to the given stream
        streamPtr_ = new hrgls::datablob::DataBlobSource ( api_
            , streamProperties_
            );
        if( streamPtr_ == NULL ) {
          status_ = ERR;
          return status_;
        }

        //Set the stream callback
        streamPtr_->SetStreamCallback( &lumberjack::Lumberjack::impl::HGStreamCallback); 
       
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
        entry["timestamp"] = getTimestamp();
        entry["pid"] = getPid();
        entry["deviceId"] = getDeviceId();
        entry["type"]  = "log";

        entry["level"] = level;
        entry["message" ] = message;

        //Add tags
        //entry["tags"] = json::parse( tags.begin(), tags.end() );

        return true;

      };

      Status getAPIStatus( void ) 
      {
        return status_;
      };

      std::string getVersion( void )
      {
        std::stringstream ss;
        ss << "version: "<<version_<<", hash: "<<hash_;;;

        //return std::string(LJVERSION);
        return ss.str();
      };


      double getTimestamp() {
        return FTTimer::getTimestamp();
      };

      /////////////////////////////////////////////
      // returns the Log entry as a stringl
      /////////////////////////////////////////////
      std::string getLogStringById( std::string id ) {
        std::string event;
        //Get json represnetation of log

        //json entry = pimpl->getLocStringById( level, message, tags );
        //convert json to string
        //
        //
        return event;

      }



    private:  
      std::string version_;
      std::string hash_;
      hrgls::API api_;
      hrgls::StreamProperties streamProperties_;
      hrgls::datablob::DataBlobSource * streamPtr_ = NULL; 

      Status status_ = NO_INIT;
      Severity printLevel_ = WARNING;
      Severity logLevel_ = ERROR;
      std::thread::id pid; 

      std::function<void(hrgls::datablob::DataBlob, void * )> callback_;

      /**
       * \brief get process ID
       * \return process id as string
       *
       * This is implemented as a separate function for potential
       * cross-platform compatibility.
       */
      std::string getPid() {
        //extract PID
        std::stringstream ss;
        ss << std::this_thread::get_id();

        return ss.str();
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
        std::string name = "/etc/machine-id";

        //Check /etc/machine-id
        std::stringstream buffer;
        std::ifstream fptr(name);
        if(fptr.is_open()) {
          buffer  << fptr.rdbuf();
          mid = buffer.str();
        }

        //
        if( mid.empty() ) {
          //TODO generate error
          //append( ERROR, "Unable to connect to the backend");
        }

        //TODO: Generate ID from MAC address
        return mid;
      }

      /*
      double getTimestamp() {
        return FTTimer::getTimestamp();

        auto now = std::chrono::system_clock::now();

        double millis = static_cast<double>(
            std::chrono::duration_cast<std::chrono::microseconds>(
              now.time_since_epoch()
            ).count()
          )/1e6;


        return millis;
      }
        */

  };

  /**
   * \brief Lumberjack main class 
   *
   * This class connects to the hrgls API on construction
   */
  Lumberjack::Lumberjack() : pimpl { FT::make_unique<impl>()} 
  {
    pimpl->connect();
  };

  Lumberjack::~Lumberjack() = default;


  /////////////////////////////////////////////
  //Function to append a new log message
  /////////////////////////////////////////////
  std::string Lumberjack::append( Severity level
      , std::string message
      , std::vector<std::string> tags
      )
  {
    pimpl->append( level, message, tags );
    return "test";
  }

  /////////////////////////////////////////////
  // Function to append a new log message
  /////////////////////////////////////////////
  std::string Lumberjack::append( Severity level
      , std::string message
      )
  {
    std::vector<std::string> tags;
    pimpl->append( level, message, tags );
    return "test";
  }

 
    
  /////////////////////////////////////////////
  // Function to get api status
  /////////////////////////////////////////////
  Status Lumberjack::getAPIStatus() {
    return pimpl->getAPIStatus();
  }

  std::string Lumberjack::getVersion( void )
  {
    return pimpl->getVersion();
  };

  double Lumberjack::getTimestamp() {
    return pimpl->getTimestamp();
  }
}


  


