import tkinter as tk
import conf.Log as Log

from PIL import Image, ImageTk

class View(tk.Frame):
    def __init__(self, parent):
        super().__init__(parent)
    
        self.grid_columnconfigure(0, weight=1)

        self.LoadHeader()
        self.LoadStatsGrid()
        self.LoadGraphsGrids()

        self.HeaderGrid.grid(column = 0, row = 0, pady=20, sticky='ew')
        self.StatsGrid.grid(column = 0, row = 1, pady=20, sticky='ew')   
        self.GraphsGrid.grid(column = 0, row = 2, pady=20, sticky='ew') 

        self.MessageLabel = tk.Label(self, text="Ready!", anchor=tk.W).grid(row=3, column=0, sticky='ew')

    def LoadHeader(self):
        self.HeaderGrid = tk.Frame(self)

        self.TitleLbl = tk.Label(self.HeaderGrid, text="SPIDAM Program", font=('Arial 20 bold'))
        self.TitleLbl.grid(row = 0, column= 0)
        self.SubTitleLbl = tk.Label(self.HeaderGrid, text="Please select a file to begin")
        self.SubTitleLbl.grid(row = 1, column = 0)
        # GrabWaveFile sets the data attribute inside waveData obj
        self.SelectFileBttn = tk.Button(self.HeaderGrid, text="Select", command=self.OpenFileCommand)
        self.SelectFileBttn.grid(row = 2, column = 0)

    def LoadStatsGrid(self):
        self.StatsGrid = tk.Frame(self) #Note positioning is handled later.

        currentRow = 0
        tk.Label(self.StatsGrid, text="File Information", font='Arial 16 bold').grid(row=currentRow, column=0, columnspan=2)
        currentRow += 1

        self.FilePathVar = tk.StringVar(value = "[No File Loaded]")
        self.FileLenVar = tk.StringVar(value = "0.00 seconds")
        self.ResonantFreqVar = tk.StringVar(value = "0 Hz")
        self.RT60DiffVar = tk.StringVar(value = "0 seconds")

        tk.Label(self.StatsGrid, text="File Path: ").grid(row=currentRow, column=0)
        tk.Label(self.StatsGrid, textvariable=self.FilePathVar).grid(row=currentRow, column=1)
        currentRow += 1

        tk.Label(self.StatsGrid, text="Length: ").grid(row=currentRow, column=0)
        tk.Label(self.StatsGrid, textvariable=self.FileLenVar).grid(row=currentRow, column=1)
        currentRow += 1

        tk.Label(self.StatsGrid, text="Resonant Frequency: ").grid(row = currentRow, column = 0)
        tk.Label(self.StatsGrid, textvariable=self.ResonantFreqVar).grid(row = currentRow, column = 1)
        currentRow += 1

        tk.Label(self.StatsGrid, text="RT60 Difference: ").grid(row = currentRow, column = 0)
        tk.Label(self.StatsGrid, textvariable=self.RT60DiffVar).grid(row = currentRow, column = 1)

    def LoadGraphsGrids(self):
        self.GraphsGrid = tk.Frame(self)
        tk.Label(self.GraphsGrid, text="Audio Data", font='Arial 16 bold').grid(row = 0, column = 0, columnspan = 2)

        currentRow = 1
        currentColumn = 0

        # Column 1
        self.GraphTitleVar = tk.StringVar(value = "No Info")
        self.GraphCaptionVar = tk.StringVar(value ="No File Loaded")

        self.GraphTitleLbl = tk.Label(self.GraphsGrid, textvariable=self.GraphTitleVar)
        self.GraphTitleLbl.grid(row = currentRow, column = currentColumn)
        currentRow += 1

        self.GraphsButtonsGrid = tk.Frame(self.GraphsGrid)
        self.GraphsButtonsGrid.grid(row = currentRow, column = currentColumn)
        self.GraphsBackImageBttn = tk.Button(self.GraphsButtonsGrid, text="<", command=self.BackImageCommand)
        self.GraphsBackImageBttn.grid(row = 0, column = 0)
        self.GraphsNextImageBttn = tk.Button(self.GraphsButtonsGrid, text=">", command=self.NextImageCommand)
        self.GraphsNextImageBttn.grid(row = 0, column = 1)
        currentRow += 1

        self.GraphCaptionLbl = tk.Label(self.GraphsGrid, textvariable=self.GraphCaptionVar)
        self.GraphCaptionLbl.grid(row = currentRow, column = currentColumn)
        currentRow += 1

        self.CurrentImage = tk.Label(self.GraphsGrid)
        self.CurrentImage.grid(row = currentRow, column = currentColumn)

        currentRow += 1
        currentColumn += 1

    def LoadImageFromPath(self, targetArea: tk.Label, ImgPath: str) -> None:
        image = Image.open(ImgPath)
        tk_image = ImageTk.PhotoImage(image)
        targetArea['image'] = tk_image
        targetArea.image = tk_image

    def OpenFileCommand(self):
        if self.controller:
            self.controller.LoadAndCalculateFile()

    def BackImageCommand(self):
        if self.controller:
            self.controller.CyclePrevFreqImage()

    def NextImageCommand(self):
        if self.controller:
            self.controller.CycleNextFreqImage()
    
    def SetController(self, controller):
        self.controller = controller

    def ShowMessage(self, message: str, type: str):
        """
        Show a message with the specified type.
        :param message: The string to show.
        :param type: A string either "msg", "err", or "warn" to determine the type.
        :return:
        """

        self.MessageLabel['text'] = message
        self.MessageLabel['foreground']  = 'red' if type == "err" else ('yellow' if type == 'warn' else 'black')
        self.MessageLabel.after(3000, self.HideMessage)

    def HideMessage(self):
        self.message_label['text'] = ''