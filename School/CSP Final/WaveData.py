from pydub import AudioSegment

class WaveData:
    def __init__(self, Data: AudioSegment = None, Path: str = None):
        self.__Data = Data
        self.__Path = Path

    @property
    def Data(self) -> AudioSegment:
        return self.__Data
    
    @Data.setter
    def Data(self, NewData: AudioSegment) -> None:
        self.__Data = NewData

    @property
    def Path(self) -> str:
        return self.__Path
    
    @Path.setter
    def Path(self, NewPath: str) -> None:
        self.__Path = NewPath