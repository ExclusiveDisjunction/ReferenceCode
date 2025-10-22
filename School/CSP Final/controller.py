import model
import view

import conf.Configuration as Conf
import conf.Log as Log

import DataAna
import WaveGrabber
import WaveTool

class Controller:
    def __init__(self, model: model.Model, view : view.View):
        self.model = model
        self.view = view

        Log.LogEvent("Controller initiated.")

    def LoadAndCalculateFile(self):
        if self.model and self.view:
            # First we need to grab the wave file.
            WaveGrabber.GrabWaveFile(self.model.Data)

            if self.model.Data.Data is None:
                return
        
            # Now that the file is loaded, render graphs
            TotalOutputImg = WaveTool.GraphWave(self.model.Data.Data)            
            DataAna.calculate_rt60(self.model.Data)
            DataAna.generateRT60Plot()

            # Next fill the RT60 values and graphs.
            RT60Diff = DataAna.getRT60Difference()
            RT60Low = float(DataAna.getRT60("low"))
            RT60Mid = float(DataAna.getRT60("mid"))
            RT60High = float(DataAna.getRT60("high"))

            FreqGraphTot = "output/all.png"
            FreqGraphLow = "output/rt60plot-250Hz.png"
            FreqGraphMid = "output/rt60plot-1000Hz.png"
            FreqGraphHigh = "output/rt60plot-10000Hz.png"
            OtherChart = "output/rt60bar.png"

            self.model.ImageFreqData = [ 
                ("Total Wave Output", TotalOutputImg, ""),
                ("All Frequencies", FreqGraphTot, f"RT60 Difference is {round(RT60Diff, 2)} seconds"),
                ("Low Frequencies", FreqGraphLow, f"RT60 is {round(RT60Low, 2)} seconds"),
                ("Mid Frequencies", FreqGraphMid, f"RT60 is {round(RT60Mid, 2)} seconds"),
                ("High Frequencies", FreqGraphHigh, f"RT60 is {round(RT60High, 2)} seconds"),
                ("Frequencies Comparison", OtherChart, "")
            ]
            self.model.CurrentImage = 0
            self.UpdateImageOnCurrentVal()

            # Update Stats
            self.view.FilePathVar.set(self.model.Data.Path)
            self.view.FileLenVar.set(f"{round(self.model.Data.Data.duration_seconds, 2)} seconds")
            resStr = str(DataAna.ComputeHighestResonance(self.model.Data)).format(".0e")
            self.view.ResonantFreqVar.set(value = f"{resStr} Hz")
            self.view.RT60DiffVar.set(value = f"{round(RT60Diff, 2)} seconds")

    def CycleNextFreqImage(self):
        if self.model and self.view:
            # First increment the index
            currentIndex = self.model.CurrentImage
            currentIndex = (currentIndex + 1) % len(self.model.ImageFreqData)

            # Update data and then select data.
            self.model.CurrentImage = currentIndex
            self.UpdateImageOnCurrentVal()            
    def CyclePrevFreqImage(self):
        if self.model and self.view:
            # First increment the index
            currentIndex = self.model.CurrentImage - 1
            currentIndex = len(self.model.ImageFreqData) - 1 if currentIndex < 0 else currentIndex

            # Update data and then select data.
            self.model.CurrentImage = currentIndex
            self.UpdateImageOnCurrentVal()  
    def UpdateImageOnCurrentVal(self):  
        if self.model and self.view:
            currentData = self.model.ImageFreqData[self.model.CurrentImage]

            # Update View.
            self.view.GraphTitleVar.set(currentData[0])
            self.view.GraphCaptionVar.set(currentData[2])
            self.view.LoadImageFromPath(self.view.CurrentImage, currentData[1])