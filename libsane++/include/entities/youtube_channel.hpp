#ifndef SANE_YOUTUBE_CHANNEL_HPP
#define SANE_YOUTUBE_CHANNEL_HPP

#include <map>
#include <list>
#include <unicode/unistr.h>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#define MISSING_VALUE "N/A"
#define YOUTUBE_CHANNEL_PREFIX "UC"
#define YOUTUBE_UPLOADS_PLAYLIST_PREFIX "UU"
#define YOUTUBE_FAVOURITES_PLAYLIST_PREFIX "FL"
#define YOUTUBE_LIKES_PLAYLIST_PREFIX "LL"

namespace sane {
    /**
     * Notes:
       channel id and uploads playlist id are identical if you remove their static prefixes:

       channel = UC, uploads playlist = UU, favourites playlist = FL
     */
    class YoutubeChannel {
    public:
        // Create an empty instance, to be populated later.
        explicit YoutubeChannel();

        // Create an instance and feed it values through a JSON.
        explicit YoutubeChannel(nlohmann::json &t_data) {
            addFromJson(t_data);
        }

        // Create an instance and feed it values through a list of strings.
        explicit YoutubeChannel(std::list<std::string> &t_values) {
            addFromStringList(t_values);
        }

        // Create an instance and feed it values through a map of strings.
        explicit YoutubeChannel(std::map<std::string, std::string> &t_map) {
            addFromMap(t_map);
        }

        // Create an instance and feed it values through a map of unsigned chars.
        explicit YoutubeChannel(std::map<std::string, const unsigned char*> &t_map) {
            addFromMap(t_map);
        }

        void assignJsonStringValue(std::string &stringToAssignValue,
                                   nlohmann::json &unknownJsonTypeValue, nlohmann::json &t_json);

        void assignJsonStringValue(icu::UnicodeString &stringToAssignValue,
                                   nlohmann::json &unknownJsonTypeValue, nlohmann::json &t_json);

        void addFromJson(nlohmann::json t_json);

        void addFromStringList(const std::list<std::string>& t_values);

        void addFromMap(std::map<std::string, std::string>& t_map);

        void addFromMap(std::map<std::string, const unsigned char*>& t_map);

        void print(int indentationSpacing);

        // Each error/warning map consists of ["error"] and ["json"]
        void addError(const std::string &t_errorMsg, nlohmann::json &t_json);

        void addWarning(const std::string &t_warningMsg, nlohmann::json &t_json);

        std::list<std::map<std::string, nlohmann::json>> getErrors();

        std::list<std::map<std::string, nlohmann::json>> getWarnings();

        void clearWarnings();

        void clearErrors();

        void clearErrorsAndWarnings();

        bool wasAborted();

        const std::string getUploadsPlaylist();

        const std::string getLikesPlaylist();

        const std::string getFavouritesPlaylist();

        const std::string getId();

        const char *getIdAsCString();

        const std::string getChannelId();

        const std::string getDescriptionAsString();

        const icu::UnicodeString getDescriptionAsUnicode();

        const char* getDescriptionAsCString();

        const std::string getPublishedAt();

        const char* getPublishedAtAsCString();

        const std::map<std::string, std::string> getThumbnails();

        const std::string getThumbnailDefault();

        const char* getThumbnailDefaultAsCString();

        const std::string getThumbnailHigh();

        const char* getThumbnailHighAsCString();

        const std::string getThumbnailMedium();

        const char* getThumbnailMediumAsCString();

        const std::string getTitleAsString();

        const icu::UnicodeString getTitleAsUnicode();

        const char* getTitleAsCString();

        bool hasFavouritesPlaylist();

        bool hasUploadsPlaylist();

        bool hasLikesPlaylist();
    private:
        // Relevant JSON response values.

        // The value that YouTube uses to uniquely identify the channel that the user subscribed to.
        // NB: Grab the one inside resourceId, the outer one is *yours*.
        std::string m_id;

        // Playlists (only need bool as they are supersets of ID)
        bool m_hasFavouritesPlaylist = false;
        bool m_hasUploadsPlaylist = false;
        bool m_hasLikesPlaylist = false;

        // The subscription's details.
        icu::UnicodeString m_description;

        // The date and time that the subscription was created.
        // The value is specified in ISO 8601 (YYYY-MM-DDThh:mm:ss.sZ) format.
        std::string m_publishedAt;

        // A map of thumbnail images associated with the subscription.
        // Should in most cases contain keys: "default", "high" and "medium".
        std::map<std::string, std::string> m_thumbnails;

        // Subscription/Channel title.
        icu::UnicodeString m_title;

        // Track errors that occur
        std::list<std::map<std::string, nlohmann::json>> m_errors;

        // Track warnings that occur
        std::list<std::map<std::string, nlohmann::json>> m_warnings;

        // Indicate whether the operation was aborted
        bool m_aborted = false;

    };

} // namespace sane

#endif //SANE_YOUTUBE_CHANNEL_HPP
