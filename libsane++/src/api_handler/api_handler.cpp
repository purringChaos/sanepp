/*
 *  SaneAPI handling via libcURL.
 */

// Standard libraries.
#include <iostream>
#include <string>
#include <list>

// 3rd party libraries.
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Project specific libraries.
#include <api_handler/api_handler.hpp>
#include <entities/youtube_channel.hpp>
#include <db_handler/db_youtube_channels.hpp>

namespace sane {
    /**
     * Callback function to be called when receiving the http response from the server.
     *
     * You need to pass a function to handle the response stored in contents.
     * The CURLOPT_WRITEDATA is set the fourth param in the cb function.
     * You can write the response to this buffer and access it at your user context.
     *
     * @param contents
     * @param size
     * @param nmemb
     * @param userp
     * @return
     */
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    /**
     * Gets a SaneAPI response via cURL.
     *
     * @param url   A const string of the full API route URL.
     * @return      Response parsed as JSON or - if cURL failed - an explicitly expressed empty object.
     */
    nlohmann::json getSapiResponse(const std::string& url) {
        CURL *curl;
        std::string readBuffer;
        nlohmann::json jsonData = nlohmann::json::object();

        // Start a libcURL easy session and assign the returned handle.
        // NB: Implicitly calls curl_global_init, which is *NOT* thread-safe!
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            // Perform a blocking file transfer
            curl_easy_perform(curl);

            // Cleanup call REQUIRED by curl_easy_init, which closes the handle.
            //
            // This might close all connections this handle has used and possibly has kept open until
            // now - unless it was attached to a multi handle while doing the transfers.
            // Don't call this function if you intend to transfer more files,
            // re-using handles is a key to good performance with libcurl
            curl_easy_cleanup(curl);

            // Convert readBuffer to json
            jsonData = nlohmann::json::parse(readBuffer);
        }
        return jsonData;
    }

    void sapiGetSubscriptions() {
        std::list <std::shared_ptr<YoutubeChannel>> channels;

        std::cout << "Retrieving subscriptions from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_SUBSCRIPTIONS);
        std::cout << "Got response from SaneAPI, processing " << jsonData.size() << " channels..." << std::endl;

        // iterate the JSON array of multiple channels and append a YoutubeChannel.
        size_t warningsCount = 0;
        size_t errorsCount = 0;
        for (auto & subscriptionJson : jsonData) {
            // Create a new YoutubeChannel object for each channel.
            std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(subscriptionJson);
            if (channel->wasAborted()) {
                // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
                channel.reset();
                std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
                std::cerr << jsonData.dump(4);
            } else {
                warningsCount = channel->getWarnings().size();
                errorsCount = channel->getErrors().size();

                // Append channels list with the new YoutubeChannel object.
                channels.push_back(channel);
            }
        }

        std::string reportedProblems;
        if (warningsCount > 0 and errorsCount > 0) {
            reportedProblems = " with " + std::to_string(warningsCount) + " warnings and "
                    + std::to_string(errorsCount) + " errors";
        }
        else if (warningsCount > 0) {
            reportedProblems = " with " + std::to_string(warningsCount) + " warnings";
        }
        else if (errorsCount > 0) {
            reportedProblems = " with " + std::to_string(errorsCount) + " errors";
        }

        // Return the parsed channels (and report possible issues).
        std::cout << "Processing completed" << reportedProblems << "." << std::endl;

        // Clear the warnings and errors in the channel objects
        for (auto & channel : channels) {
            channel->clearErrorsAndWarnings();
        }

        // Store to Database
        std::cout << "Storing to database..." << std::endl;
        addChannelsToDB(channels, NO_ERROR_LOG);
        std::cout << "Storing to database successful!" << std::endl;
    }

    std::shared_ptr<YoutubeChannel> sapiGetChannelByUsername(const std::string &t_username) {
        size_t warningsCount = 0;
        size_t errorsCount = 0;

        std::cout << "Retrieving channel '" << t_username << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?username=" + t_username);
        std::cout << "Got response from SaneAPI, processing channel..." << std::endl;


        // Create a new YoutubeChannel object for the requested channel.
        std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(jsonData);

        if (channel->wasAborted()) {
            // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
            channel.reset();
            std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
            std::cerr << jsonData.dump(4);
            return nullptr;
        } else {
            warningsCount = channel->getWarnings().size();
            errorsCount = channel->getErrors().size();

            // Return the new YoutubeChannel object.
            return channel;
        }
    }

    std::shared_ptr<YoutubeChannel> sapiGetChannelById(const std::string &t_username) {
        size_t warningsCount = 0;
        size_t errorsCount = 0;

        std::cout << "Retrieving channel '" << t_username << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?id=" + t_username);
        std::cout << "Got response from SaneAPI, processing channel..." << std::endl;


        // Create a new YoutubeChannel object for the requested channel.
        std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(jsonData);

        if (channel->wasAborted()) {
            // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
            channel.reset();
            std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
            std::cerr << jsonData.dump(4);
            return nullptr;
        } else {
            warningsCount = channel->getWarnings().size();
            errorsCount = channel->getErrors().size();

            // Return the new YoutubeChannel object.
            return channel;
        }
    }
} // namespace sane.
