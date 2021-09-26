/** 
 * @file-Uri-cpp
 * 
 * This module contains the implementation of the Uri::Uri class
 * 
 */
 
#include "Uri.hpp"
#include <string>
#include <vector>
#include <stdint.h>
 namespace Uri {
     /**
      * This contains the private properties of a Uri instance
      */
     struct Uri::Impl {
         std::string scheme;
         std::string authority;
         std::string host;
         std::vector<std::string> path;
         std::string pathDelimiter = "/";
         uint16_t port;
         bool hasPort = false;
     };
     Uri::~Uri() = default;

     Uri::Uri() 
        : impl_ (new Impl)
    {            
    }
    
    void Uri::setPathDelimiter(const std::string & newPathDelimiter){
        impl_->pathDelimiter = newPathDelimiter;
    };
    
    bool Uri::ParseFromString(const std::string& uriString){
        const auto schemeEnd = uriString.find(':');        
        impl_->scheme = uriString.substr(0,schemeEnd);
        auto rest = uriString.substr(schemeEnd + 1);
        if (rest.substr(0,2) == "//"){
            const auto authorityEnd = rest.find(impl_->pathDelimiter,2);
            const auto portDelimiter = rest.find(':');
            if(portDelimiter == std::string::npos){
                impl_->host = rest.substr(2, authorityEnd - 2);
            }else{
                impl_->hasPort = true;
                int portLength = 0;
                uint16_t isValidPort = 0;
                while(isdigit(rest[portDelimiter+portLength+1]))
                    portLength++;
                if(portLength > 0)
                    isValidPort = std::stoi(rest.substr(portDelimiter+1,portDelimiter+portLength));
                else 
                    impl_->hasPort = false;
                if (isValidPort <= 65535){
                    impl_->port = isValidPort;
                }else{ 
                    impl_->hasPort = false;
                    impl_->port = 0;
                }

                impl_->host = rest.substr(2, portDelimiter - 2);                
            }
            rest = rest.substr(authorityEnd );
        } else {
             impl_->host.clear();
        }
        bool isLast = false;        
        bool isLastDelimeter = false;
        size_t pathDelimiter;
        if((pathDelimiter = rest.find(impl_->pathDelimiter)) != std::string::npos){
            if(pathDelimiter == (rest.length() - impl_->pathDelimiter.length()))
            {
                std::string isDelimeter = rest.substr(rest.find(impl_->pathDelimiter), rest.length());
                if (isDelimeter == impl_->pathDelimiter)
                    isLastDelimeter = true;
            }
        }
        if(!(rest.length() == impl_->pathDelimiter.length() && isLastDelimeter == true)){
            while(!rest.empty() ){
                pathDelimiter = rest.find(impl_->pathDelimiter);
                if(pathDelimiter == std::string::npos){
                    pathDelimiter = rest.length();
                    isLast = true;
                }
                impl_->path.emplace_back(
                    rest.begin(),
                    rest.begin() + pathDelimiter 
                );
                if(isLast == true)
                    rest = rest.substr(pathDelimiter);
                else
                    rest = rest.substr(pathDelimiter + impl_->pathDelimiter.length());
            }
        }
        if(isLastDelimeter == true)
            impl_->path.push_back("");
        return true;
    };
            
    std::string Uri::GetScheme() const {
        return impl_->scheme;
    };
    std::string Uri::GetHost() const {
        return impl_->host;
    };
    std::vector<std::string> Uri::GetPath() const {
        return impl_->path;
    };
    bool Uri::HasPort() const {
        return impl_->hasPort;
    };
    uint16_t Uri::GetPort() const {
        return impl_->port;
    };
 }