#include "ResponseCacheControl.h"
#include <map>
#include <set>

namespace CesiumAsync {
    static std::string trimSpace(const std::string& str);

    ResponseCacheControl::ResponseCacheControl(bool mustRevalidate, 
        bool noCache, 
        bool noStore, 
        bool noTransform, 
        bool accessControlPublic, 
        bool accessControlPrivate, 
        bool proxyRevalidate, 
        int maxAge, 
        int sharedMaxAge)
        : _mustRevalidate{mustRevalidate}
        , _noCache{noCache}
        , _noStore{noStore}
        , _noTransform{noTransform}
        , _accessControlPublic{accessControlPublic}
        , _accessControlPrivate{accessControlPrivate}
        , _proxyRevalidate{proxyRevalidate}
        , _maxAge{maxAge}
        , _sharedMaxAge{sharedMaxAge}
    {}

    /*static*/ std::optional<ResponseCacheControl> ResponseCacheControl::parseFromResponseHeaders(const HttpHeaders& headers) 
    {
        std::map<std::string, std::string>::const_iterator cacheControlIter = headers.find("Cache-Control");
        if (cacheControlIter == headers.end()) {
            return std::nullopt;
        }

        const std::string& headerValue = cacheControlIter->second;
        std::map<std::string, std::string, CaseInsensitiveCompare> parameterizedDirectives;
        std::set<std::string, CaseInsensitiveCompare> directives;
        size_t last = 0;
        size_t next = 0;
        while ((next = headerValue.find(",", last)) != std::string::npos) {
            std::string directive = trimSpace(headerValue.substr(last, next - last));
            size_t equalSize = directive.find("=");
            if (equalSize != std::string::npos) {
                parameterizedDirectives.insert({ 
                    trimSpace(directive.substr(0, equalSize)), trimSpace(directive.substr(equalSize + 1)) 
                });
            }
            else {
                directives.insert(directive);
            }

            last = next + 1;
        }

        std::string directive = trimSpace(headerValue.substr(last));
        size_t equalSize = directive.find("=");
        if (equalSize != std::string::npos) {
            parameterizedDirectives.insert({ 
                trimSpace(directive.substr(0, equalSize)), trimSpace(directive.substr(equalSize + 1)) 
            });
        }
        else {
            directives.insert(directive);
        }

        bool mustRevalidate = directives.find("must-revalidate") != directives.end();
        bool noCache = directives.find("no-cache") != directives.end();
        bool noStore = directives.find("no-store") != directives.end();
        bool noTransform = directives.find("no-transform") != directives.end();
        bool accessControlPublic = directives.find("public") != directives.end();
        bool accessControlPrivate = directives.find("private") != directives.end();
        bool proxyRevalidate = directives.find("proxy-revalidate") != directives.end();

        int maxAge = 0;
        std::map<std::string, std::string, CaseInsensitiveCompare>::const_iterator maxAgeIter = parameterizedDirectives.find("max-age");
        if (maxAgeIter != parameterizedDirectives.end()) {
            maxAge = std::stoi(maxAgeIter->second);
        }

        int sharedMaxAge = 0;
        std::map<std::string, std::string, CaseInsensitiveCompare>::const_iterator sharedMaxAgeIter = parameterizedDirectives.find("s-maxage");
        if (sharedMaxAgeIter != parameterizedDirectives.end()) {
            sharedMaxAge = std::stoi(sharedMaxAgeIter->second);
        }

        return ResponseCacheControl(mustRevalidate, 
            noCache, 
            noStore, 
            noTransform, 
            accessControlPublic, 
            accessControlPrivate, 
            proxyRevalidate, 
            maxAge,
            sharedMaxAge);
    }

    std::string trimSpace(const std::string& str) {
        if (str.empty()) {
            return "";
        }

        size_t begin = 0;
        while (str[begin] == ' ') {
            ++begin;
        }

        size_t end = str.size() - 1;
        while (str[end] == ' ') {
            --end;
        }

        return str.substr(begin, end - begin + 1);
    }
}