#include <memory>
#include <string>
#include <vector>

namespace Uri {
    class Uri {
        public:
            ~Uri();
            Uri(const Uri&) = delete;
            Uri(Uri&&) = delete;
            Uri& operator=(const Uri&) = delete;
            Uri& operator=(Uri&&) = delete;
        public:
            Uri();
            /**
             * This method builds the URI from the elements parsed
             * from the given string rendering of a URI
             * 
             * @param[in] uriString
             *      This is the string rendering of the URI to parse.
             * 
             * @return
             *      An indication of whether or not the URI was 
             *      parsed successfully is returned.
             */
            bool ParseFromString(const std::string& uriString);
            
            std::string GetScheme() const;
            std::string GetHost() const;
            std::vector<std::string> GetPath() const;
            bool HasPort() const;
            uint16_t GetPort() const;
            bool IsRelativeReference() const;
            bool ContainRelativePath() const;
            std::string GetFragment() const;
            std::string GetQuery() const;
            std::string GetUserInfo() const;            
        // private method
        private:
            bool ParseFragment(std::string& uri);    
            bool ParseQuery(std::string& uri);
            bool ParseScheme(std::string& uri);
            bool ParseUserInfo(std::string& uri);
            void ParsePortHostAndAuthority(std::string& uri);
        private:
            struct Impl;
            std::unique_ptr<struct Impl > impl_; 
        
    };
}