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
namespace Uri
{
    /**
      * This contains the private properties of a Uri instance
      */
    struct Uri::Impl
    {
        std::string scheme;
        std::string authority;
        std::string host;
        std::vector<std::string> path;
        uint16_t port;
        bool hasPort = false;
        std::string fragment;
        std::string query;
        std::string userInfo;
    };
    Uri::~Uri() = default;

    Uri::Uri()
        : impl_(new Impl)
    {
    }

    bool Uri::ParseFragment(std::string &uri)
    {
        const auto fragmentDelimiter = uri.find('#');
        if (fragmentDelimiter == std::string::npos)
        {
            impl_->fragment.clear();
            return false;
        }
        else
        {
            impl_->fragment = uri.substr(fragmentDelimiter + 1, uri.length());
            uri = uri.substr(0, fragmentDelimiter);
            return true;
        }
    };
    bool Uri::ParseQuery(std::string &uri)
    {
        auto queryDelimiter = uri.find('?');
        if (queryDelimiter == std::string::npos)
        {
            impl_->query.clear();
            return false;
        }
        else
        {
            impl_->query = uri.substr(queryDelimiter + 1, uri.length());
            uri = uri.substr(0, queryDelimiter);
            return true;
        }
    }
    bool Uri::ParseScheme(std::string &uri)
    {
        const auto schemeEnd = uri.find(':');
        if (schemeEnd == std::string::npos)
        {
            impl_->scheme.clear();
            return false;
        }
        else
        {
            impl_->scheme = uri.substr(0, schemeEnd);
            uri = uri.substr(schemeEnd + 1);
            if (uri.substr(0, 2) == "//")
                uri = uri.substr(2, uri.length());
            else
                impl_->host.clear();
            return true;
        }
    }
    bool Uri::ParseUserInfo(std::string &uri)
    {
        const auto userInfoDelimiter = uri.find('@');
        if (userInfoDelimiter == std::string::npos)
        {
            impl_->userInfo = "";
            return false;
        }
        else
        {
            impl_->userInfo = uri.substr(0,userInfoDelimiter);
            uri = uri.substr(userInfoDelimiter + 1, uri.length());
            return true;
        }
    }
    void Uri::ParsePortHostAndAuthority(std::string &uri)
    {
        auto authorityEnd = uri.find('/');
        const auto portDelimiter = uri.find(':');
        if (portDelimiter == std::string::npos)
        {   
            if(!impl_->scheme.empty())            
                impl_->host = uri.substr(0,authorityEnd);
            if (!impl_->scheme.empty())
                if (impl_->scheme == "http")
                    impl_->port = 80;
                else if (impl_->scheme == "https")
                    impl_->port = 443;
            /*TO DO
                     *Possible Other variant of uri scheme
                     */
        }
        else
        {
            impl_->hasPort = true;
            int portLength = 0;
            uint16_t isValidPort = 0;
            while (isdigit(uri[portDelimiter + portLength + 1]))
                portLength++;
            if (portLength > 0)
                isValidPort = std::stoi(uri.substr(portDelimiter + 1, portDelimiter + portLength));
            else
                impl_->hasPort = false;
            if (isValidPort <= 65535 && isValidPort > 0)
            {
                impl_->port = isValidPort;
            }
            else
            {
                impl_->hasPort = false;
                if (impl_->scheme == "http")
                    impl_->port = 80;
                else if (impl_->scheme == "https")
                    impl_->port = 443;
            }
            impl_->host = uri.substr(0,portDelimiter);
        }
        if ((!impl_->host.empty() && impl_->port == 80) || (!impl_->host.empty() && impl_->port == 443))
        {
            impl_->authority = uri.substr(0,authorityEnd);
            if(authorityEnd+1 != uri.length())
                uri = uri.substr(authorityEnd+1,uri.length());
            else
                uri = uri.substr(authorityEnd,uri.length());
        }
        if (authorityEnd == std::string::npos)
        {
            authorityEnd = uri.length();
        }        
    }
    bool Uri::ParseFromString(const std::string &uriString)
    {
        std::string uri = uriString;

        ParseFragment(uri);
        ParseQuery(uri);
        ParseScheme(uri);
        ParseUserInfo(uri);
        ParsePortHostAndAuthority(uri);

        bool isLast = false;
        bool isLastDelimeter = false;
        size_t pathDelimiter;
        if ((pathDelimiter = uri.find('/')) != std::string::npos)
        {
            if (pathDelimiter == (uri.length() - 1))
            {
                std::string isDelimeter = uri.substr(uri.find('/'), uri.length());
                if (isDelimeter == "/")
                    isLastDelimeter = true;
            }
        }
        if (!(uri.length() == 1 && isLastDelimeter == true))
        {
            while (!uri.empty())
            {
                pathDelimiter = uri.find('/');
                if (pathDelimiter == std::string::npos)
                {
                    pathDelimiter = uri.length();
                    isLast = true;
                }
                impl_->path.emplace_back(
                    uri.begin(),
                    uri.begin() + pathDelimiter);
                if (isLast == true)
                {
                    uri = uri.substr(pathDelimiter);
                }
                else
                    uri = uri.substr(pathDelimiter + 1);
            }
        }
        if (isLastDelimeter == true)
            impl_->path.push_back("");
        return true;
    };

    std::string Uri::GetScheme() const
    {
        return impl_->scheme;
    };
    std::string Uri::GetHost() const
    {
        return impl_->host;
    };
    std::vector<std::string> Uri::GetPath() const
    {
        return impl_->path;
    };
    bool Uri::HasPort() const
    {
        return impl_->hasPort;
    };
    uint16_t Uri::GetPort() const
    {
        return impl_->port;
    };
    bool Uri::IsRelativeReference() const
    {
        return impl_->authority.empty();
    };
    bool Uri::ContainRelativePath() const
    {
        if (impl_->path.empty())
            return true;
        else
        {
            return !impl_->path[0].empty();
        }
    };
    std::string Uri::GetFragment() const
    {
        return impl_->fragment;
    };
    std::string Uri::GetQuery() const
    {
        return impl_->query;
    };
    std::string Uri::GetUserInfo() const
    {
        return impl_->userInfo;
    };
}