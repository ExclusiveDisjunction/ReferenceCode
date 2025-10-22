import tkinter as tk
from WaveData import WaveData

import conf.Log as Log

class Model:
    def __init__(self, Data: WaveData):
        self.Data = Data
        self.Data.Data = None
        #                       Title, Path, Caption
        self.ImageFreqData = { ("Total Wave Output", "", "No File Loaded")}
        self.CurrentImage = 0
        Log.LogEvent("Model initiated")

    @property
    def Data(self):
        return self.__data
    
    @Data.setter
    def Data(self, value: WaveData):
        self.__data = value
        Log.LogEvent("Data in Model was changed.", Log.Debug)
