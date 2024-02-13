// IMPORT THIRD-PARTY LIBRARIES
#include <pxr/usd/ar/defaultResolver.h>
#include <pxr/usd/ar/defineResolver.h>
#include <iostream>
#include <filesystem>
#include <curl/curl.h>
#include <fstream>

// IMPORT LOCAL LIBRARIES
#include "resolver.h"

PXR_NAMESPACE_OPEN_SCOPE

AR_DEFINE_RESOLVER(HttpResolver, ArResolver)

HttpResolver::HttpResolver() : ArDefaultResolver() { curl_global_init(CURL_GLOBAL_ALL); }
HttpResolver::~HttpResolver() { curl_global_cleanup(); }

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

    static std::filesystem::path FetchAndDownloadAsset(std::string route) {
        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();

        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, route.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            else
                std::cout << readBuffer << std::endl;

            curl_easy_cleanup(curl);
        }

        std::filesystem::path systemPath = route;
        auto fileSystemPath = systemPath.filename();

        auto filePath = temp_dir.operator+=(fileSystemPath);
        std::ofstream outFile(filePath);
        if (outFile) { // Check if the file was successfully opened
            outFile << readBuffer; // Write the string to the file
            outFile.close(); // Close the file
            std::cout << "File written successfully." << filePath << std::endl;
        } else {
            std::cout << "Failed to open file for writing." << std::endl;
        }

        return filePath;
    }

void HttpResolver::setBaseUrl(const std::string& url) const {
    baseUrl = url;
}

ArResolvedPath HttpResolver::_Resolve(const std::string& assetPath) const {
    std::cout << "_Resolve: " << assetPath << std::endl;
    std::string myPath = assetPath;

    if (myPath.find("http") != std::string::npos){
        std::string githubName = "github.com";
        std::string rawGithubName = "raw.githubusercontent.com";
        std::string blob = "/blob";

        size_t pos = myPath.find(githubName);
        if (pos!= std::string::npos) {
            myPath.replace(pos, githubName.length(), rawGithubName);
        }

        size_t pos_blob = myPath.find(blob);
        if (pos_blob!= std::string::npos) {
            myPath.erase(pos_blob, blob.length());
        }

        std::cout << "http PATH: " << myPath << std::endl;

        std::filesystem::path fullHttpPath = myPath;

        std::filesystem::path rootHttpPath = fullHttpPath.parent_path();

        setBaseUrl(rootHttpPath.generic_string() + "/");

        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();

        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, myPath.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            else
                std::cout << readBuffer << std::endl;

            curl_easy_cleanup(curl);
        }

        std::filesystem::path systemPath = myPath;
        auto fileSystemPath = systemPath.filename();

        auto filePath = temp_dir.operator+=(fileSystemPath);
        std::ofstream outFile(filePath);
        if (outFile) { // Check if the file was successfully opened
            outFile << readBuffer; // Write the string to the file
            outFile.close(); // Close the file
            std::cout << "File written successfully." << filePath << std::endl;
        } else {
            std::cout << "Failed to open file for writing." << std::endl;
        }

        return ArResolvedPath(filePath);
    }
    else if (std::filesystem::exists(assetPath)){
        std::cout << "Already Exists: " << assetPath << std::endl;
        return ArResolvedPath(assetPath);
    }
    else {
        std::cout << "Does not exist, trying from baseUrl: " << baseUrl << std::endl;
        std::filesystem::path systemPath = myPath;

        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();

        std::filesystem::path relativePath = std::filesystem::relative(systemPath, temp_dir);

        auto totalAssetPath = baseUrl + relativePath.generic_string();

        auto resolvedPath = FetchAndDownloadAsset(totalAssetPath);

        std::cout << "Assumed to exist now, trying from baseUrl: " << resolvedPath << std::endl;

        return ArResolvedPath(resolvedPath);
    }

}



std::shared_ptr<ArAsset> HttpResolver::_OpenAsset(const ArResolvedPath &resolvedPath) const {
    std::cout << "_OpenAsset: " << resolvedPath.GetPathString() << std::endl;
    return ArDefaultResolver::_OpenAsset(resolvedPath);
}

ArResolvedPath HttpResolver::_ResolveForNewAsset(const std::string &assetPath) const {
    std::cout << "Resolve for new asset" << std::endl;
    return ArDefaultResolver::_ResolveForNewAsset(assetPath);
}

PXR_NAMESPACE_CLOSE_SCOPE
