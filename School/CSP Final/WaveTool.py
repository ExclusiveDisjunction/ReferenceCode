from pydub import AudioSegment
import matplotlib.pyplot as plt
import numpy as np

import conf.Log as Log
import conf.Configuration as Config

def CleanWave(seg: AudioSegment) -> AudioSegment:
    Log.LogEvent("Began cleaning wave.")

    chann = seg.channels
    Log.LogEvent(f"Detected {chann} channel(s), acting accordiningly....", Log.Debug)

    if chann != 1:
        return seg.set_channels(1)
    else:
        return seg

def GraphWave(wave: AudioSegment) -> str:
    """
        Graphs the wave function and returns the path it outputted to.
    """
    Log.LogEvent("Began graphing wave.")

    ImgPath = "output/TotalWaveOutput.png"

    # Graph the wave function
    Log.LogEvent("Graphing wave data now.")

    # Step is FrameRate * Sample Width * 8, in Hz, so step is 1 over that number.

    framerate = wave.frame_rate
    frames = wave.raw_data
    signal = np.frombuffer(frames, dtype=np.int16)

    time = np.linspace(0, len(signal) / framerate, num=len(signal))

    plt.figure(figsize=(10,4))
    plt.plot(time, signal, color='b')
    plt.title('Total Audio Waveform')
    plt.xlabel('Time')
    plt.ylabel('Amplitude')
    plt.grid()
    plt.savefig(ImgPath)

    Log.LogEvent("Completed making data graph.") 
    return ImgPath
