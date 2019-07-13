import datetime
import threading
from flask import Flask, jsonify, render_template, request, redirect
from flask_restful import Resource, Api
from json import dumps

# Sane legacy
#   Database
from database.orm import init_db
from database.write_operations import DBUpdateVideo
#   YouTube
from handlers.log_handler import create_logger
import remote
from resources.youtube_auth import load_key, load_youtube_resource_oauth
from youtube.youtube_dl_handler import YoutubeDownload

# Create logger instance
logger = create_logger(__name__)

# Create the application instance
app = Flask(__name__, template_folder="templates")
api = Api(app)

API_VERSION = 'v1'

"""
API Internal
"""


@app.route('/')
def home():
    """
    This function just responds to the browser URL
    localhost:<port>/

    :return:        the rendered template 'home.html'
    """
    return render_template('home.html')


"""
Local: Internal or local operations.
"""


@app.route('/api/v1/local/add_subscription')
def add_subscription():
    """
    Add a YouTube subscription (Local).
    Takes a channel ID or username as argument.

    :return: Returns the youtube#channel resource response.
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    if 'id' in request.args:
        channel = youtube_auth.channels().list(part='contentDetails,snippet', id=request.args['id']).execute()
    elif 'username' in request.args:
        channel = youtube_auth.channels().list(part='contentDetails,snippet',
                                               forUsername=request.args['username']).execute()
    else:
        return jsonify("Error: no id or username field provided. Please specify one.")

    channel_title = channel['items'][0]['snippet']['localized']['title']
    channel_id = channel['items'][0]['id']

    logger.info("Adding subscription (Local): {} / {}".format(channel_id, channel_title))

    return jsonify(channel)


"""
Remote: Requests to the YouTube API with some extra functionality added on.
"""


@app.route('/api/v1/remote/subscriptions')
def youtube_subscriptions():
    """
    Returns subscriptions from YouTube API.

    :return: A list of youtube#subscription resources with <contentDetails> added on.
    """
    logger.info("Getting subscriptions from DB.")

    # Get an authenticated OAuth2 resource (since this is authenticated user scope)
    youtube_oauth = load_youtube_resource_oauth()

    try:
        subscriptions = remote.get_subscriptions(youtube_oauth)
    except ValueError as exc_ve:
        return jsonify(str(exc_ve))

    return jsonify(subscriptions)


@app.route('/api/v1/remote/subfeed')
def youtube_subfeed():
    """
    !! NB: This is *NOT* the regular YouTube subscription feed !!

    This function gets a list of every channel the user is subscribed to
    and combines the "Uploaded videos" playlists of all the channel into
    a single one.

    :return: JSONified list of subscribed channels' uploaded videos.
    """
    pass  # FIXME: Implement


@app.route('/video')
def youtube_video_remote(id):
    pass  # FIXME: Implement


@app.route('/api/v1/remote/channel')
def youtube_channel_remote():
    """
    Takes either a channel <id> or <username> and passes it as kwargs to YouTube API pass-through.

    :return: A youtube#channel JSON
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    if 'id' in request.args:
        channel = youtube_auth.channels().list(part='contentDetails,snippet', id=request.args['id']).execute()

    elif 'username' in request.args:
        channel = youtube_auth.channels().list(part='contentDetails,snippet',
                                               forUsername=request.args['username']).execute()

    else:
        return jsonify("Error: no id or username field provided. Please specify one.")

    return jsonify(channel['items'][0])  # Send full relevant response since id is outside of snippet


"""
# FIXME: find a name/category for youtube-dl. 
"""


@app.route('/api/v1/download', methods=['GET', 'PUT', 'CREATE'])
def youtube_download(video_id, db_update_listeners=None, youtube_dl_finished_listener=None, wait=False):
    # Vital parts unimplemented, return error for now.

    raise Exception("Not Implemented")

    # FIXME: get a video obj from id
    video = None

    # Set properties related to downloading.
    video.downloaded = True
    video.date_downloaded = datetime.datetime.utcnow()

    # Update Video information in Database.
    DBUpdateVideo(video, update_existing=True,
                  finished_listeners=db_update_listeners).start()

    # Create a threading event and set it if ready.
    event = threading.Event()

    if not wait:
        event.set()

    # FIXME: Find a way to handle "emitting" progress, finished and failed downloads.
    # Create a signal to track download progress. (was: PyQt signals/slots)
    # download_progress_signal = DownloadProgressSignals(video, event)
    download_progress_signal = None

    # Create a download instance.
    YoutubeDownload(video, event, download_progress_listener=download_progress_signal,
                    finished_listeners=youtube_dl_finished_listener).start()

    # return the signal
    return download_progress_signal


"""
YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
"""


def remove_empty_kwargs(**kwargs):
    """
    Remove keyword arguments that are not set.
    :param kwargs:
    :return:
    """
    good_kwargs = {}
    if kwargs is not None:
        for key, value in kwargs.items():
            if value:
                good_kwargs[key] = value

    return good_kwargs


@app.route('/api/v1/youtube/channels/list')
def youtube_api_channels_list():
    """
    Passes on any kwargs directly to the YouTube API: Channels.list().

    :return: youtube#channelListResponse JSON

    https://developers.google.com/youtube/v3/docs/channels/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.channels().list(**kwargs).execute()

    # Pass on kwargs and return (JSONified) result
    return jsonify(response)


@app.route('/api/v1/youtube/search')
def youtube_api_search():
    """
    Passes on any kwargs directly to the YouTube API: Search().

    https://developers.google.com/youtube/v3/docs/search/list
    :return:
    """
    # Get an authenticated API key object
    youtube_auth = load_key()

    # Strip out empty kwargs.
    kwargs = remove_empty_kwargs(**request.args)

    response = youtube_auth.search().list(**kwargs).execute()

    return jsonify(response)


# If we're running in stand alone mode, run the application
if __name__ == '__main__':
    # Initialize database
    init_db()

    # Run API
    app.run(port="5002", debug=True)
