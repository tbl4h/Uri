/** 
 * @file-Uri-cpp
 * 
 * This module contains the implementation of the Uri::Uri class
 * 
 */
 
#include "Uri.hpp"
#include <string>
#include <vector>

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
            impl_->host = rest.substr(2, authorityEnd - 2);
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
 }