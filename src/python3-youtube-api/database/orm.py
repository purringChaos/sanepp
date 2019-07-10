import os

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import scoped_session, sessionmaker

from handlers.config_handler import read_config
from handlers.log_handler import create_logger

logger = create_logger(__name__)


OS_PATH = os.path.dirname(__file__)
DB_URL = read_config('Database', 'url', literal_eval=False)

engine = create_engine(DB_URL, convert_unicode=True)
logger.info("Created DB engine: {}, convert_unicode=True".format(DB_URL))
db_session = scoped_session(sessionmaker(autocommit=False, autoflush=False, bind=engine))
logger.info("Created DB session")
PermanentBase = declarative_base()
PermanentBase.query = db_session.query_property()


def init_db():
    # import all modules here that might define models so that
    # they will be registered properly on the metadata.  Otherwise
    # you will have to import them first before calling init_db()
    import database.models
    import database.video
    import database.db_download_tile
    PermanentBase.metadata.create_all(bind=engine)
    logger.info("Initialised DB (PermanentBase.metadata.create_all(bind=engine))")


