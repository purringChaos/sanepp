#ifndef SANE_DB_YOUTUBE_CHANNELS_HPP
#define SANE_DB_YOUTUBE_CHANNELS_HPP

#include <iostream>
#include <vector>
#include <list>

#include <entities/youtube_channel.hpp>
#include <db_handler/db_handler.hpp>

namespace sane {
    int addChannelsToDB(const std::list <std::shared_ptr<YoutubeChannel>>& t_channels);
} // namespace sane

#endif //SANE_DB_YOUTUBE_CHANNELS_HPP