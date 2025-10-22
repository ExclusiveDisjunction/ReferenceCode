import logging

Debug = logging.DEBUG
Info = logging.INFO
Warning = logging.WARNING
Error = logging.ERROR
Critical = logging.CRITICAL

def InitLog(logPath: str = "run.log", level = logging.INFO):
    logging.basicConfig(filename=logPath, encoding="utf-8", level=level, filemode="w", format="[%(asctime)s] %(levelname)s: %(message)s", datefmt="%m/%d/%Y %I:%M:%S %p")
    
    logging.info("Starting program log SPIDAM")

def LogEvent(EventString: str, level = logging.INFO):
    logging.log(level, EventString)