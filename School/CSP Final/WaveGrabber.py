import pathlib as pl
from pathlib import Path
from pydub import AudioSegment
import os
from tkinter import messagebox
from tkinter import filedialog

from WaveData import WaveData
import conf.Log as Log

# This file will prompt the user for a file, and convert it to a .wav and then return it.
# NOTE: This file REQUIRES ffmpeg to be installed on the current computer to work correctly. The library pydub requires it to open and decompress files other than .wav.
# To download ffmpeg on windows, type 'winget install ffmpeg' in the powershell.

def GrabWaveFile(waveData: WaveData, TargetFilePath: Path | None = None) -> AudioSegment:
    """
        Ensures that a file provided will be a wave file, and either loads a file from the path provided, or will use the file browser to grab one.
    """
    Log.LogEvent("Began grabbing a wave file.")

    # Determiens the target file to load.
    FilePath = None
    if (TargetFilePath == None):
        FilePath = PromptFile()
    else:
        FilePath = TargetFilePath
    AudFile = GrabAudioSegment(FilePath)

    # If the file is none, the file could not be opened or the user decided to cancel, so return none.
    if (AudFile == None):
        return None
    
    Log.LogEvent("Wave file grabbed, begining cleaning.")

    # If the current file is not a .wav, it needs to be converted to the .wav format. 
    if (FilePath.suffix != ".wav"):
        Log.LogEvent("File loaded, but .wav is not detected. Prompting user.", Log.Warning)
        # First it will determine if this is ok by the user.
        MsgResult = messagebox.askquestion("File not .wav",
                                           "The provided file is not .wav, and it will be converted. Is this ok? A copy of the file with an extension of .wav will be created.")
        if (MsgResult != "yes"):
            Log.LogEvent("User opted to not convert, aborting.")
            return None

        # Removes the file as a wave if it exists.
        NewPath = FilePath.with_suffix(".wav")
        if os.path.exists(str(NewPath)):
            os.remove(str(NewPath))

        # Exports the current file & re-opens it as a wav.
        AudFile.export(str(NewPath), format="wav")
        AudFile = GrabAudioSegment(NewPath)
        Log.LogEvent("Wave file converted successfully.")

    # Set audio data to obj instead of returning to eventloop
    waveData.Data = AudFile
    waveData.Path = str(FilePath)
    return AudFile


def GrabAudioSegment(FilePath: Path) -> AudioSegment:
    """
        Opens an pydub.AudioSegment object from a specificed file path.
    """
    Log.LogEvent(f"Attempting to load an audio segment from path \"{FilePath}\"")
    # If the file provided is None, return None (there isnt anything to open)
    if (FilePath == None):
        return None

    # Determiens the segment.
    Suffix = FilePath.suffix
    Suffix = Suffix.removeprefix(".")

    # Attempt to open the file, but if not sucessful, then alert the user and return None.
    try:
        AudioSeg = AudioSegment.from_file(str(FilePath), format=Suffix)
    except FileNotFoundError as e:
        messagebox.showerror("Error:", f"The file path could not be found")
        Log.LogEvent("File Not Found!", Log.Error)
        return None
    except PermissionError as e:
        messagebox.showerror("Error: ", "The user does not have access to that file.")
        Log.LogEvent("Permissions not allowed!", Log.Error)
        return None

    Log.LogEvent("Audio segment loaded sucessfully.", Log.Debug)
    return AudioSeg


# We will have to first import a file.
def PromptFile() -> Path:
    """
        Asks user to select a file from the file system as either a .wav, .mp3, .wma, or .aac.
    """

    Log.LogEvent("Opening a file prompt for user...")
    FilePathRaw = filedialog.askopenfilename(filetypes=[("Audio Files", "*.mp3;*.wav;*.wma;*.aac")])

    if (FilePathRaw == ""):
        return None

    Log.LogEvent(f"File selected \"{FilePathRaw}\"", Log.Debug)
    return Path(FilePathRaw)
