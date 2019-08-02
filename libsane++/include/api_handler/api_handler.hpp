/*
 *  SaneAPI handling via libcURL -- Headers.
 */
#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <list>

#include <entities/youtube_channel.hpp>

// SaneAPI (SAPI) REST API URLs, see SaneAPI documentation
// for the meaning of Local, Remote and YouTube.

// Local: SAPI Internal or local operations.
#define SAPI_LOCAL_ADD_SUBSCRIPTION             "http://127.0.0.1:5002/api/v1/local/add_subscription"

// Remote: Requests to the YouTube API with some extra functionality added on.
#define SAPI_REMOTE_GET_SUBSCRIPTIONS           "http://127.0.0.1:5002/api/v1/remote/subscriptions"
// TODO: Port SAPI_REMOTE_GET_SUBFEED functionality to native C++
#define SAPI_REMOTE_GET_SUBFEED                 "http://127.0.0.1:5002/api/v1/remote/subfeed"
#define SAPI_REMOTE_GET_CHANNEL                 "http://127.0.0.1:5002/api/v1/remote/channel"

// YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
#define SAPI_YT_ACTIVITIES_LIST                 "http://127.0.0.1:5002/api/v1/youtube/activities/list"
#define SAPI_YT_ACTIVITIES_INSERT               "http://127.0.0.1:5002/api/v1/youtube/activities/insert"
#define SAPI_YT_CAPTIONS_LIST                   "http://127.0.0.1:5002/api/v1/youtube/captions/list"
#define SAPI_YT_CAPTIONS_INSERT                 "http://127.0.0.1:5002/api/v1/youtube/captions/insert"
#define SAPI_YT_CAPTIONS_UPDATE                 "http://127.0.0.1:5002/api/v1/youtube/captions/update"
#define SAPI_YT_CAPTIONS_DOWNLOAD               "http://127.0.0.1:5002/api/v1/youtube/captions/download"
#define SAPI_YT_CAPTIONS_DELETE                 "http://127.0.0.1:5002/api/v1/youtube/captions/delete"
#define SAPI_YT_CHANNEL_BANNERS_INSERT          "http://127.0.0.1:5002/api/v1/youtube/channelBanners/insert"
#define SAPI_YT_CHANNELS_LIST                   "http://127.0.0.1:5002/api/v1/youtube/channels/list"
#define SAPI_YT_CHANNEL_SECTION_LIST            "http://127.0.0.1:5002/api/v1/youtube/channelSection/list"
#define SAPI_YT_CHANNEL_SECTION_INSERT          "http://127.0.0.1:5002/api/v1/youtube/channelSection/insert"
#define SAPI_YT_CHANNEL_SECTION_UPDATE          "http://127.0.0.1:5002/api/v1/youtube/channelSection/update"
#define SAPI_YT_CHANNEL_SECTION_DELETE          "http://127.0.0.1:5002/api/v1/youtube/channelSection/delete"
#define SAPI_YT_COMMENT_LIST                    "http://127.0.0.1:5002/api/v1/youtube/comment/list"
#define SAPI_YT_COMMENT_INSERT                  "http://127.0.0.1:5002/api/v1/youtube/comment/insert"
#define SAPI_YT_COMMENT_UPDATE                  "http://127.0.0.1:5002/api/v1/youtube/comment/update"
#define SAPI_YT_COMMENT_MARK_AS_SPAM            "http://127.0.0.1:5002/api/v1/youtube/comment/markAsSpam"
#define SAPI_YT_COMMENT_SET_MODERATION_STATUS   "http://127.0.0.1:5002/api/v1/youtube/comment/setModerationStatus"
#define SAPI_YT_COMMENT_DELETE                  "http://127.0.0.1:5002/api/v1/youtube/comment/delete"
#define SAPI_YT_COMMENT_THREAD_LIST             "http://127.0.0.1:5002/api/v1/youtube/commentThread/list"
#define SAPI_YT_COMMENT_THREAD_INSERT           "http://127.0.0.1:5002/api/v1/youtube/commentThread/insert"
#define SAPI_YT_COMMENT_THREAD_UPDATE           "http://127.0.0.1:5002/api/v1/youtube/commentThread/update"
#define SAPI_YT_GUIDE_CATEGORIES_LIST           "http://127.0.0.1:5002/api/v1/youtube/guideCategories/list"
#define SAPI_YT_IL18N_LANGUAGES_LIST            "http://127.0.0.1:5002/api/v1/youtube/i18nLanguages/list"
#define SAPI_YT_IL8N_REGIONS_LIST               "http://127.0.0.1:5002/api/v1/youtube/i18nRegions/list"
#define SAPI_YT_PLAYLIST_ITEMS_LIST             "http://127.0.0.1:5002/api/v1/youtube/playlistItems/list"
#define SAPI_YT_PLAYLIST_ITEMS_INSERT           "http://127.0.0.1:5002/api/v1/youtube/playlistItems/insert"
#define SAPI_YT_PLAYLIST_ITEMS_UPDATE           "http://127.0.0.1:5002/api/v1/youtube/playlistItems/update"
#define SAPI_YT_PLAYLIST_ITEMS_DELETE           "http://127.0.0.1:5002/api/v1/youtube/playlistItems/delete"
#define SAPI_YT_PLAYLISTS_LIST                  "http://127.0.0.1:5002/api/v1/youtube/playlists/list"
#define SAPI_YT_PLAYLISTS_INSERT                "http://127.0.0.1:5002/api/v1/youtube/playlists/insert"
#define SAPI_YT_PLAYLISTS_UPDATE                "http://127.0.0.1:5002/api/v1/youtube/playlists/update"
#define SAPI_YT_PLAYLISTS_DELETE                "http://127.0.0.1:5002/api/v1/youtube/playlists/delete"
#define SAPI_YT_SEARCH                          "http://127.0.0.1:5002/api/v1/youtube/search"
#define SAPI_YT_SUBSCRIPTIONS_LIST              "http://127.0.0.1:5002/api/v1/youtube/subscriptions/list"
#define SAPI_YT_THUMBNAILS_SET                  "http://127.0.0.1:5002/api/v1/youtube/thumbnails/set"
#define SAPI_YT_VIDEO_ABUSE_REPORT_REASONS_LIST "http://127.0.0.1:5002/api/v1/youtube/videoAbuseReportReasons/list"
#define SAPI_YT_VIDEO_CATEGORIES_LIST           "http://127.0.0.1:5002/api/v1/youtube/videoCategories/list"
#define SAPI_YT_VIDEOS_LIST                     "http://127.0.0.1:5002/api/v1/youtube/videos/list"
#define SAPI_YT_VIDEOS_INSERT                   "http://127.0.0.1:5002/api/v1/youtube/videos/insert"
#define SAPI_YT_VIDEOS_UPDATE                   "http://127.0.0.1:5002/api/v1/youtube/videos/update"
#define SAPI_YT_VIDEOS_RATE                     "http://127.0.0.1:5002/api/v1/youtube/videos/rate"
#define SAPI_YT_VIDEOS_GET_RATING               "http://127.0.0.1:5002/api/v1/youtube/videos/getRating"
#define SAPI_YT_VIDEOS_REPORT_ABUSE             "http://127.0.0.1:5002/api/v1/youtube/videos/reportAbuse"
#define SAPI_YT_VIDEOS_DELETE                   "http://127.0.0.1:5002/api/v1/youtube/videos/delete"
#define SAPI_YT_WATERMARKS_SET                  "http://127.0.0.1:5002/api/v1/youtube/watermarks/set"
#define SAPI_YT_WATERMARKS_UNSET                "http://127.0.0.1:5002/api/v1/youtube/watermarks/unset"

namespace sane {
    void sapiGetSubscriptions();

    std::shared_ptr<YoutubeChannel> sapiGetChannelByUsername(const std::string &t_username);

    std::shared_ptr<YoutubeChannel> sapiGetChannelById(const std::string &t_id);
} // namespace sane.
#endif // Header guards.