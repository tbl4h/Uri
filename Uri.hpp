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
            void setPathDelimiter(const std::string & newPathDelimiter);
            
        private:
            struct Impl;

            std::unique_ptr<struct Impl > impl_; 
    };
}