#include "CesiumAsync/DiskCache.h"
#include "CesiumAsync/IAssetResponse.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <utility>
#include <stdexcept>

namespace CesiumAsync {
    // Cache table column names
    static const std::string CACHE_TABLE = "CacheItemTable";
    static const std::string CACHE_TABLE_ID_COLUMN = "id";
    static const std::string CACHE_TABLE_EXPIRY_TIME_COLUMN = "expiryTime";
    static const std::string CACHE_TABLE_LAST_ACCESSED_TIME_COLUMN = "lastAccessedTime";
    static const std::string CACHE_TABLE_RESPONSE_HEADER_COLUMN = "responseHeaders";
    static const std::string CACHE_TABLE_RESPONSE_CONTENT_TYPE_COLUMN = "responseContentType";
    static const std::string CACHE_TABLE_RESPONSE_STATUS_CODE_COLUMN = "responseStatusCode";
    static const std::string CACHE_TABLE_RESPONSE_CACHE_CONTROL_COLUMN = "responseCacheControl";
    static const std::string CACHE_TABLE_RESPONSE_DATA_COLUMN = "responseData";
    static const std::string CACHE_TABLE_REQUEST_HEADER_COLUMN = "requestHeader";
    static const std::string CACHE_TABLE_REQUEST_METHOD_COLUMN = "requestMethod";
    static const std::string CACHE_TABLE_REQUEST_URL_COLUMN = "requestUrl";
    static const std::string CACHE_TABLE_KEY_COLUMN = "key";
    static const std::string CACHE_TABLE_VIRTUAL_TOTAL_ITEMS_COLUMN = "totalItems";

    // Sql commands for setting up database
    static const std::string CREATE_CACHE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS " + CACHE_TABLE + "(" +
            CACHE_TABLE_ID_COLUMN + " INTEGER PRIMARY KEY NOT NULL," +
            CACHE_TABLE_EXPIRY_TIME_COLUMN + " DATETIME NOT NULL," +
            CACHE_TABLE_LAST_ACCESSED_TIME_COLUMN + " DATETIME NOT NULL," +
            CACHE_TABLE_RESPONSE_HEADER_COLUMN + " TEXT NOT NULL," +
            CACHE_TABLE_RESPONSE_CONTENT_TYPE_COLUMN + " TEXT NOT NULL," +
            CACHE_TABLE_RESPONSE_STATUS_CODE_COLUMN + " INTEGER NOT NULL," +
            CACHE_TABLE_RESPONSE_CACHE_CONTROL_COLUMN + " TEXT NOT NULL," +
            CACHE_TABLE_RESPONSE_DATA_COLUMN + " BLOB," +
            CACHE_TABLE_REQUEST_HEADER_COLUMN + " TEXT NOT NULL," +
            CACHE_TABLE_REQUEST_METHOD_COLUMN + " TEXT NOT NULL," +
            CACHE_TABLE_REQUEST_URL_COLUMN + " TEXT NOT NULL," +
            CACHE_TABLE_KEY_COLUMN + " TEXT NOT NULL)"; 

    static const std::string INDEX_KEY_COLUMN_SQL = "CREATE INDEX IF NOT EXISTS " + 
        CACHE_TABLE_KEY_COLUMN + "_index ON " + CACHE_TABLE + "(" + CACHE_TABLE_KEY_COLUMN + ")";

    static const std::string PRAGMA_WAL_SQL = "PRAGMA journal_mode=WAL";

    static const std::string PRAGMA_SYNC_SQL = "PRAGMA synchronous=OFF";

    static const std::string PRAGMA_PAGE_SIZE_SQL = "PRAGMA page_size=4096";

    // Sql commands for getting entry from database
    static const std::string GET_ENTRY_SQL = "SELECT " +
        CACHE_TABLE_ID_COLUMN + ", " +
        CACHE_TABLE_EXPIRY_TIME_COLUMN + ", " +
        CACHE_TABLE_LAST_ACCESSED_TIME_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_HEADER_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_CONTENT_TYPE_COLUMN + " , " +
        CACHE_TABLE_RESPONSE_STATUS_CODE_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_CACHE_CONTROL_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_DATA_COLUMN + ", " +
        CACHE_TABLE_REQUEST_HEADER_COLUMN + ", " +
        CACHE_TABLE_REQUEST_METHOD_COLUMN + ", " +
        CACHE_TABLE_REQUEST_URL_COLUMN +
        " FROM " + CACHE_TABLE + " WHERE " + CACHE_TABLE_KEY_COLUMN + "=?";

    static const std::string UPDATE_LAST_ACCESSED_TIME_SQL = "UPDATE " + CACHE_TABLE + 
        " SET " + CACHE_TABLE_LAST_ACCESSED_TIME_COLUMN + " = strftime('%s','now') WHERE " + CACHE_TABLE_KEY_COLUMN + " =?";

    // Sql commands for storing response
    static const std::string STORE_RESPONSE_SQL = "REPLACE INTO " + CACHE_TABLE + " (" +
        CACHE_TABLE_EXPIRY_TIME_COLUMN + ", " +
        CACHE_TABLE_LAST_ACCESSED_TIME_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_HEADER_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_CONTENT_TYPE_COLUMN + " , " +
        CACHE_TABLE_RESPONSE_STATUS_CODE_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_CACHE_CONTROL_COLUMN + ", " +
        CACHE_TABLE_RESPONSE_DATA_COLUMN + ", " +
        CACHE_TABLE_REQUEST_HEADER_COLUMN + ", " +
        CACHE_TABLE_REQUEST_METHOD_COLUMN + ", " +
        CACHE_TABLE_REQUEST_URL_COLUMN + ", " +
        CACHE_TABLE_KEY_COLUMN +
        ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    // Sql commands for prunning the database
    static const std::string TOTAL_ITEMS_QUERY_SQL = "SELECT COUNT(*) " + CACHE_TABLE_VIRTUAL_TOTAL_ITEMS_COLUMN + " FROM " + CACHE_TABLE;

    static const std::string DELETE_EXPIRED_ITEMS_SQL = "DELETE FROM " + CACHE_TABLE + " WHERE " + CACHE_TABLE_EXPIRY_TIME_COLUMN + " < strftime('%s','now')";

    static const std::string DELETE_LRU_ITEMS_SQL = "DELETE FROM " + CACHE_TABLE + " WHERE " + CACHE_TABLE_ID_COLUMN + 
        " IN (SELECT " + CACHE_TABLE_ID_COLUMN + " FROM " + CACHE_TABLE + " ORDER BY " + CACHE_TABLE_LAST_ACCESSED_TIME_COLUMN + " ASC " + " LIMIT ?)";

    // Sql commands for clean all items
    static const std::string CLEAR_ALL_SQL = "DELETE FROM " + CACHE_TABLE;

    static std::string convertHeadersToString(const HttpHeaders& headers);

    static std::string convertCacheControlToString(const ResponseCacheControl* cacheControl);

    static HttpHeaders convertStringToHeaders(const std::string& serializedHeaders);

    static std::optional<ResponseCacheControl> convertStringToResponseCacheControl(const char* serializedResponseCacheControl);

    DiskCache::DiskCache(const std::string &databaseName, uint64_t maxItems)
        : _pConnection{nullptr}
        , _maxItems{maxItems}
    {
        int status = sqlite3_open(databaseName.c_str(), &this->_pConnection);
        if (status != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errstr(status));
        }

        // create cache tables if not exist. Key -> Cache table: one-to-many relationship
        char* createTableError = nullptr;
        status = sqlite3_exec(this->_pConnection, CREATE_CACHE_TABLE_SQL.c_str(), nullptr, nullptr, &createTableError);
        if (status != SQLITE_OK) {
            std::string errorStr(createTableError);
            sqlite3_free(createTableError);
            throw std::runtime_error(errorStr);
        }

        // index Key column
        char* indexKeyError = nullptr;
        status = sqlite3_exec(this->_pConnection, INDEX_KEY_COLUMN_SQL.c_str(), nullptr, nullptr, &indexKeyError);
        if (status != SQLITE_OK) {
            std::string errorStr(indexKeyError);
            sqlite3_free(indexKeyError);
            throw std::runtime_error(errorStr);
        }
        
        // turn on WAL mode
        char* walError = nullptr;
        status = sqlite3_exec(this->_pConnection, PRAGMA_WAL_SQL.c_str(), nullptr, nullptr, &walError);
        if (status != SQLITE_OK) {
            std::string errorStr(walError);
            sqlite3_free(walError);
            throw std::runtime_error(errorStr);
        }

        // turn off synchronous mode
        char* syncError = nullptr;
        status = sqlite3_exec(this->_pConnection, PRAGMA_SYNC_SQL.c_str(), nullptr, nullptr, &syncError);
        if (status != SQLITE_OK) {
            std::string errorStr(syncError);
            sqlite3_free(syncError);
            throw std::runtime_error(errorStr);
        }

        // increase page size
        char* pageSizeError = nullptr;
        status = sqlite3_exec(this->_pConnection, PRAGMA_PAGE_SIZE_SQL.c_str(), nullptr, nullptr, &pageSizeError);
        if (status != SQLITE_OK) {
            std::string errorStr(pageSizeError);
            sqlite3_free(pageSizeError);
            throw std::runtime_error(errorStr);
        }

        // get entry based on key
        status = sqlite3_prepare_v2(_pConnection, GET_ENTRY_SQL.c_str(), -1, &this->_getEntryStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }

        // update last accessed for entry
        status = sqlite3_prepare_v2(
            this->_pConnection, UPDATE_LAST_ACCESSED_TIME_SQL.c_str(), -1, &this->_updateLastAccessedTimeStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }

        // store response 
        status = sqlite3_prepare_v2(this->_pConnection, STORE_RESPONSE_SQL.c_str(), -1, &this->_storeResponseStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }

        // query total items
        status = sqlite3_prepare_v2(this->_pConnection, TOTAL_ITEMS_QUERY_SQL.c_str(), -1, &this->_totalItemsQueryStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }

        // delete expired items
        status = sqlite3_prepare_v2(this->_pConnection, DELETE_EXPIRED_ITEMS_SQL.c_str(), -1, &this->_deleteExpiredStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }

        // delete expired items
        status = sqlite3_prepare_v2(this->_pConnection, DELETE_LRU_ITEMS_SQL.c_str(), -1, &this->_deleteLRUStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }

        // clear all items
        status = sqlite3_prepare_v2(this->_pConnection, CLEAR_ALL_SQL.c_str(), -1, &this->_clearAllStmtWrapper.stmt, nullptr);
        if (status != SQLITE_OK) {
            throw std::runtime_error(std::string(sqlite3_errstr(status)));
        }
    }

    DiskCache::DiskCache(DiskCache&& other) noexcept {
        this->_pConnection = other._pConnection;
        this->_maxItems = other._maxItems;

        other._pConnection = nullptr;
        other._maxItems = 0;
    }

    DiskCache& DiskCache::operator=(DiskCache&& other) noexcept {
        if (&other != this) {
            this->_pConnection = other._pConnection;
            this->_maxItems = other._maxItems;

            other._pConnection = nullptr;
            other._maxItems = 0;
        }

        return *this;
    }

    DiskCache::~DiskCache() noexcept {
        if (this->_pConnection) {
            sqlite3_close(this->_pConnection);
        }
    }

    bool DiskCache::getEntry(const std::string& key, 
            std::function<bool(CacheItem)> predicate, 
            std::string& error) const 
    {
        std::lock_guard<std::mutex> guard(this->_mutex);

        // get entry based on key
        int status = sqlite3_reset(this->_getEntryStmtWrapper.stmt);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_clear_bindings(this->_getEntryStmtWrapper.stmt);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_bind_text(this->_getEntryStmtWrapper.stmt, 1, key.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        while ((status = sqlite3_step(this->_getEntryStmtWrapper.stmt)) == SQLITE_ROW) {
            int64_t itemIndex = sqlite3_column_int64(this->_getEntryStmtWrapper.stmt, 0);

            // parse cache item metadata
            std::time_t expiryTime = sqlite3_column_int64(this->_getEntryStmtWrapper.stmt, 1);

            std::time_t lastAccessedTime = sqlite3_column_int64(this->_getEntryStmtWrapper.stmt, 2);

            // parse response cache 
            std::string serializedResponseHeaders = reinterpret_cast<const char*>(sqlite3_column_text(this->_getEntryStmtWrapper.stmt, 3));
            HttpHeaders responseHeaders = convertStringToHeaders(serializedResponseHeaders);

            std::string responseContentType = reinterpret_cast<const char*>(sqlite3_column_text(this->_getEntryStmtWrapper.stmt, 4));

            uint16_t statusCode = static_cast<uint16_t>(sqlite3_column_int(this->_getEntryStmtWrapper.stmt, 5));

            const char* serializedResponseCacheControl = reinterpret_cast<const char*>(sqlite3_column_text(this->_getEntryStmtWrapper.stmt, 6));
            std::optional<ResponseCacheControl> responseCacheControl = convertStringToResponseCacheControl(serializedResponseCacheControl);

            const void* rawResponseData = sqlite3_column_blob(this->_getEntryStmtWrapper.stmt, 7);
            size_t responseDataSize = static_cast<size_t>(sqlite3_column_bytes(this->_getEntryStmtWrapper.stmt, 7));
            std::vector<uint8_t> responseData(responseDataSize);
            if (responseDataSize != 0) {
                std::memcpy(responseData.data(), rawResponseData, responseDataSize);
            }

            CacheResponse cacheResponse(statusCode, 
                std::move(responseContentType), 
                std::move(responseHeaders), 
                std::move(responseCacheControl), 
                std::move(responseData));
            
            // parse request
            std::string serializedRequestHeaders = reinterpret_cast<const char*>(sqlite3_column_text(this->_getEntryStmtWrapper.stmt, 8));
            HttpHeaders requestHeaders = convertStringToHeaders(serializedRequestHeaders);

            std::string requestMethod = reinterpret_cast<const char*>(sqlite3_column_text(this->_getEntryStmtWrapper.stmt, 9));

            std::string requestUrl = reinterpret_cast<const char*>(sqlite3_column_text(this->_getEntryStmtWrapper.stmt, 10));

            CacheRequest cacheRequest(std::move(requestHeaders), 
                std::move(requestMethod), 
                std::move(requestUrl));

            CacheItem item{ expiryTime,
                lastAccessedTime,
                std::move(cacheRequest),
                std::move(cacheResponse) };

            if (predicate(std::move(item))) {
                // update the last accessed time
                int updateStatus = sqlite3_reset(this->_updateLastAccessedTimeStmtWrapper.stmt);
                if (updateStatus != SQLITE_OK) {
                    error = std::string(sqlite3_errstr(updateStatus));
                    return false;
                }

                updateStatus = sqlite3_clear_bindings(this->_updateLastAccessedTimeStmtWrapper.stmt);
                if (updateStatus != SQLITE_OK) {
                    error = std::string(sqlite3_errstr(updateStatus));
                    return false;
                }

                updateStatus = sqlite3_bind_int64(this->_updateLastAccessedTimeStmtWrapper.stmt, 1, itemIndex);
                if (updateStatus != SQLITE_OK) {
                    error = std::string(sqlite3_errstr(updateStatus));
                    return false;
                }

                updateStatus = sqlite3_step(this->_updateLastAccessedTimeStmtWrapper.stmt);
                if (updateStatus != SQLITE_DONE) {
                    error = std::string(sqlite3_errstr(updateStatus));
                    return false;
                }

                break;
            }
        }

        if (status != SQLITE_DONE && status != SQLITE_ROW) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        return true;
    }

    bool DiskCache::storeResponse(const std::string& key, 
        std::time_t expiryTime,
        const IAssetRequest& request,
        std::string& error) 
    {
        const IAssetResponse* response = request.response();
        if (response == nullptr) {
            error = std::string("Request needs to have a response");
            return false;
        }

        gsl::span<const uint8_t> responseData = response->data();

        std::lock_guard<std::mutex> guard(this->_mutex);

        // cache the request with the key
        int status = sqlite3_reset(this->_storeResponseStmtWrapper.stmt);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_clear_bindings(this->_storeResponseStmtWrapper.stmt);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_bind_int64(this->_storeResponseStmtWrapper.stmt, 1, static_cast<int64_t>(expiryTime));
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_bind_int64(this->_storeResponseStmtWrapper.stmt, 2, static_cast<int64_t>(std::time(0)));
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        std::string responseHeader = convertHeadersToString(response->headers());
        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 3, responseHeader.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }
        
        const std::string& responseContentType = response->contentType();
        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 4, responseContentType.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_bind_int(this->_storeResponseStmtWrapper.stmt, 5, static_cast<int>(response->statusCode()));
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        std::string responseCacheControl = convertCacheControlToString(response->cacheControl());
        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 6, responseCacheControl.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_bind_blob(this->_storeResponseStmtWrapper.stmt, 7, responseData.data(), static_cast<int>(responseData.size()), SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        std::string requestHeader = convertHeadersToString(request.headers());
        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 8, requestHeader.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        const std::string& requestMethod = request.method();
        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 9, requestMethod.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        const std::string& requestUrl = request.url();
        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 10, requestUrl.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_bind_text(this->_storeResponseStmtWrapper.stmt, 11, key.c_str(), -1, SQLITE_STATIC);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_step(this->_storeResponseStmtWrapper.stmt);
        if (status != SQLITE_DONE) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        return true;
    }

    bool DiskCache::prune(std::string& error)
    {
        std::lock_guard<std::mutex> guard(this->_mutex);

        int64_t totalItems = 0;

        // query total size of response's data
        {
            int totalItemsQueryStatus = sqlite3_reset(this->_totalItemsQueryStmtWrapper.stmt);
            if (totalItemsQueryStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(totalItemsQueryStatus));
                return false;
            }

            totalItemsQueryStatus = sqlite3_clear_bindings(this->_totalItemsQueryStmtWrapper.stmt);
            if (totalItemsQueryStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(totalItemsQueryStatus));
                return false;
            }

            totalItemsQueryStatus = sqlite3_step(this->_totalItemsQueryStmtWrapper.stmt);
            if (totalItemsQueryStatus == SQLITE_DONE) {
                return true;
            }
            else if (totalItemsQueryStatus != SQLITE_ROW) {
                error = std::string(sqlite3_errstr(totalItemsQueryStatus));
                return false;
            }

            // prune the rows if over maximum
            totalItems = sqlite3_column_int64(this->_totalItemsQueryStmtWrapper.stmt, 0);
            if (totalItems > 0 && totalItems <= static_cast<int64_t>(_maxItems)) {
                return true;
            }
        }

        // delete expired rows first
        {
            int deleteExpiredStatus = sqlite3_reset(this->_deleteExpiredStmtWrapper.stmt);
            if (deleteExpiredStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(deleteExpiredStatus));
                return false;
            }

            deleteExpiredStatus = sqlite3_clear_bindings(this->_deleteExpiredStmtWrapper.stmt);
            if (deleteExpiredStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(deleteExpiredStatus));
                return false;
            }

            deleteExpiredStatus = sqlite3_step(this->_deleteExpiredStmtWrapper.stmt);
            if (deleteExpiredStatus != SQLITE_DONE) {
                error = std::string(sqlite3_errstr(deleteExpiredStatus));
                return false;
            }
        }

        // check if we should delete more
        int deletedRows = sqlite3_changes(this->_pConnection);
        if (totalItems - deletedRows < static_cast<int>(this->_maxItems)) {
            return true;
        }

        totalItems -= deletedRows;

        // delete rows LRU if we are still over maximum
        {
            int deleteLLRUStatus = sqlite3_reset(this->_deleteLRUStmtWrapper.stmt);
            if (deleteLLRUStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(deleteLLRUStatus));
                return false;
            }

            deleteLLRUStatus = sqlite3_clear_bindings(this->_deleteLRUStmtWrapper.stmt);
            if (deleteLLRUStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(deleteLLRUStatus));
                return false;
            }

            deleteLLRUStatus = sqlite3_bind_int64(this->_deleteLRUStmtWrapper.stmt, 1, totalItems - static_cast<int64_t>(this->_maxItems));
            if (deleteLLRUStatus != SQLITE_OK) {
                error = std::string(sqlite3_errstr(deleteLLRUStatus));
                return false;
            }

            deleteLLRUStatus = sqlite3_step(this->_deleteLRUStmtWrapper.stmt);
            if (deleteLLRUStatus != SQLITE_DONE) {
                error = std::string(sqlite3_errstr(deleteLLRUStatus));
                return false;
            }
        }

        return true;
    }

    bool DiskCache::clearAll(std::string& error)
    {
        std::lock_guard<std::mutex> guard(this->_mutex);

        int status = sqlite3_reset(this->_clearAllStmtWrapper.stmt);
        if (status != SQLITE_OK) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        status = sqlite3_step(this->_clearAllStmtWrapper.stmt);
        if (status != SQLITE_DONE) {
            error = std::string(sqlite3_errstr(status));
            return false;
        }

        return true;
    }

    std::string convertHeadersToString(const HttpHeaders& headers) {
        rapidjson::Document document;
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value root(rapidjson::kObjectType);
        rapidjson::Value key(rapidjson::kStringType);
        rapidjson::Value value(rapidjson::kStringType);
        for (const std::pair<const std::string, std::string>& header : headers) 
        {
            key.SetString(header.first.c_str(), allocator);
            value.SetString(header.second.c_str(), allocator);
            root.AddMember(key, value, allocator);
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        root.Accept(writer);
        return buffer.GetString();
    }

    std::string convertCacheControlToString(const ResponseCacheControl* cacheControl) {
        if (!cacheControl) {
            return "";
        }

        rapidjson::Document document;
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value root(rapidjson::kObjectType);
        root.AddMember("mustRevalidate", cacheControl->mustRevalidate(), allocator);
        root.AddMember("noCache", cacheControl->noCache(), allocator);
        root.AddMember("noStore", cacheControl->noStore(), allocator);
        root.AddMember("noTransform", cacheControl->noTransform(), allocator);
        root.AddMember("accessControlPublic", cacheControl->accessControlPublic(), allocator);
        root.AddMember("accessControlPrivate", cacheControl->accessControlPrivate(), allocator);
        root.AddMember("proxyRevalidate", cacheControl->proxyRevalidate(), allocator);
        root.AddMember("maxAge", cacheControl->maxAge(), allocator);
        root.AddMember("sharedMaxAge", cacheControl->sharedMaxAge(), allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        root.Accept(writer);
        return buffer.GetString();
    }

    HttpHeaders convertStringToHeaders(const std::string& serializedHeaders) {
        rapidjson::Document document;
        document.Parse(serializedHeaders.c_str());

        HttpHeaders headers;
        for (rapidjson::Document::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); ++it) {
            headers.insert({ it->name.GetString(), it->value.GetString() });
        }

        return headers;
    }

    std::optional<ResponseCacheControl> convertStringToResponseCacheControl(const char* serializedResponseCacheControl) {
        if (serializedResponseCacheControl == nullptr) {
            return std::nullopt;
        }

        rapidjson::Document document;
        document.Parse(serializedResponseCacheControl);
        return ResponseCacheControl{
            document["mustRevalidate"].GetBool(),
            document["noCache"].GetBool(),
            document["noStore"].GetBool(),
            document["noTransform"].GetBool(),
            document["accessControlPublic"].GetBool(),
            document["accessControlPrivate"].GetBool(),
            document["proxyRevalidate"].GetBool(),
            document["maxAge"].GetInt(),
            document["sharedMaxAge"].GetInt()
        };
    }
}