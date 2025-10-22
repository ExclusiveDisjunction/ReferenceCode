import WaveData

import numpy as np
import matplotlib.pyplot as plt

from scipy.io import wavfile
from scipy.signal import welch

import conf.Log as Log
import conf.Configuration as Conf


# Call this only after RT60 values have been calculated!
def getRT60Difference() -> float:
    file_path = "conf/rt60data.txt"
    with open(file_path, 'r') as file:
        lines = file.readlines()

    rt60Arr = lines[3].split(",")
    avgRT60 = (float(rt60Arr[0]) + float(rt60Arr[1]) + float(rt60Arr[2])) / 3.0
    return (avgRT60 - 0.5)


def ComputeHighestResonance(waveData: WaveData):
    sample_rate, data = wavfile.read(waveData.Path)
    frequencies, power = welch(data, sample_rate, nperseg=4096)
    dominant_frequency = frequencies[np.argmax(power)]
    return dominant_frequency


# Call this function to get the rt60 data after already calculated!
def getRT60(freqType):
    file_path = "conf/rt60data.txt"
    with open(file_path, 'r') as file:
        lines = file.readlines()

    rt60Arr = lines[3].split(",")
    if freqType == "low":
        return rt60Arr[0]
    elif freqType == "mid":
        return rt60Arr[1]
    elif freqType == "high":
        return rt60Arr[2]
    else:
        # something went wrong!
        Log.LogEvent("Invalid frequency type! '" + freqType + "'", Log.Error)
        return None


def generateRT60Plot():
    freqTypes = ["low", "mid", "high"]
    values = [getRT60("low"), getRT60("mid"), getRT60("high")]
    plt.bar(freqTypes, values, color='skyblue')  # Create the bar chart with specified data and color
    plt.xlabel('Frequency Types')  # Set the label for the x-axis
    plt.ylabel('RT60 values')  # Set the label for the y-axis
    plt.title('RT60 Values at different frequencies')  # Set the title of the bar chart
    plt.tight_layout()  # Adjust layout to prevent clipping of labels
    plt.savefig("output/rt60bar.png")


def getFreqRange(freqType: str):
    if freqType == "low":
        return [60, 250]
    elif freqType == "mid":
        return [0, 1000]
    elif freqType == "high":
        return [5, 10000]
    else:
        # something went wrong!
        Log.LogEvent("Invalid frequency type! '" + freqType + "'", Log.Error)
        return None


# Call this function to calculate and store the rt60 data of a given frequency!
def calculate_rt60(WaveData: WaveData.WaveData):
    data_in_db_all = []
    indexes = []

    for freqType in ["low", "mid", "high"]:
        freq_range = getFreqRange(freqType)

        def find_target_frequency(freqs):
            freq_min = freq_range[0]
            freq_max = freq_range[1]

            for x in freqs:
                if x > freq_max:
                    break
            return x

        def frequency_check():
            global target_frequency
            target_frequency = find_target_frequency(freqs)
            index_of_frequency = np.where(freqs == target_frequency)[0][0]
            data_for_frequency = spectrum[index_of_frequency]
            data_in_db_fun = 10 * np.log10(data_for_frequency)
            return data_in_db_fun

        sample_rate, data = wavfile.read(WaveData.Path)
        spectrum, freqs, t, im = plt.specgram(data, Fs=sample_rate, NFFT=1024, cmap=plt.get_cmap("autumn_r"))

        data_in_db = frequency_check()
        plt.figure()

        plt.plot(t, data_in_db, linewidth=1, alpha=0.7, color="#004bc6")
        data_in_db_all.append(data_in_db)

        plt.xlabel("Time (s)")
        plt.ylabel("Power (dB)")

        index_of_max = np.argmax(data_in_db)
        value_of_max = data_in_db[index_of_max]
        plt.plot(t[index_of_max], data_in_db[index_of_max], 'go')
        indexes.append(index_of_max)

        sliced_array = data_in_db[index_of_max:]
        value_of_max_less_5 = value_of_max - 5

        def find_nearest_value(array, value):
            array = np.asarray(array)
            idx = (np.abs(array - value)).argmin()
            return array[idx]

        value_of_max_less_5 = find_nearest_value(sliced_array, value_of_max_less_5)
        index_of_max_less_5 = np.where(data_in_db == value_of_max_less_5)
        plt.plot(t[index_of_max_less_5], data_in_db[index_of_max_less_5], 'yo')
        indexes.append(index_of_max_less_5)

        value_of_max_less_25 = value_of_max - 25
        value_of_max_less_25 = find_nearest_value(sliced_array, value_of_max_less_25)
        index_of_max_less_25 = np.where(data_in_db == value_of_max_less_25)
        plt.plot(t[index_of_max_less_25], data_in_db[index_of_max_less_25], 'ro')
        indexes.append(index_of_max_less_25)

        rt20 = (t[index_of_max_less_5] - t[index_of_max_less_25])[0]
        rt60 = 3 * rt20
        plt.grid()

        save_img_path = Conf.Configuration.RetriveConfiguration("rt60plot").split(".")
        # Removed low range
        # save_img_path = save_img_path[0] + str(freq_range[0]) + "-" + str(freq_range[1]) + "Hz"
        save_img_path = save_img_path[0] + "-" + str(freq_range[1]) + "Hz"

        rt60 = round(abs(rt60), 2)

        def writeRT60(file_path):
            with open(file_path, 'r') as file:
                lines = file.readlines()

            rt60Arr = lines[3].split(",")
            if freqType == "low":
                rt60Arr[0] = rt60
            elif freqType == "mid":
                rt60Arr[1] = rt60
            elif freqType == "high":
                rt60Arr[2] = rt60
            else:
                Log.LogEvent("Invalid frequency type! '" + freqType + "'", Log.Error)

            lines[3] = str(rt60Arr[0]) + ',' + str(rt60Arr[1]) + ',' + str(rt60Arr[2])

            with open(file_path, 'w') as file:
                file.writelines(lines)

        writeRT60("conf/rt60data.txt")

        plt.savefig(save_img_path)
        print([int(target_frequency), rt60])
    # Do not read the data from here, read it from getRT60

    plt.clf()
    plt.xlabel("Time (s)")
    plt.ylabel("Power (dB)")
    plt.plot(t, data_in_db_all[0], linewidth=1, alpha=0.7, color="#004bc6")
    plt.plot(t, data_in_db_all[1], linewidth=1, alpha=0.7, color="#c60021")
    plt.plot(t, data_in_db_all[2], linewidth=1, alpha=0.7, color="#38c600")
    for DATA_IN_DB in data_in_db_all:
        plt.plot(t[indexes[0]], DATA_IN_DB[indexes[0]], 'go')
        plt.plot(t[indexes[1]], DATA_IN_DB[indexes[1]], 'yo')
        plt.plot(t[indexes[2]], DATA_IN_DB[indexes[2]], 'ro')

    # save_img_path = Conf.Configuration.RetriveConfiguration("frequencyGraphPath")
    plt.savefig("output/all.png")
    plt.clf()
    generateRT60Plot()
    return None


if __name__ == "__main__":
    Conf.Configuration.Init("setup.cfg")
    Log.InitLog()

    waveData = WaveData.WaveData()
    WaveData.GrabWaveFile(waveData, None)

    if waveData.getData() is not None:
        calculate_rt60(waveData)
