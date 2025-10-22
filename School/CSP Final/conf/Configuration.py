"""
    This module will read a file that ends with .conf and construct a dictionary of its contents.
"""

from conf import Log


class Configuration:
    __path = ""
    __configurations = {}

    @staticmethod
    def Init(path: str):
        Configuration.__path = path
        Log.LogEvent(f"Opening configuration file \"{path}\"")
        Configuration.ReadFile()

    @staticmethod
    def ReadFile() -> None:
        Configuration.__configurations = {}

        with open(Configuration.__path, "r") as file:
            chunk = file.read()
            lines = chunk.splitlines()
            if (len(lines) == 0):
                Log.LogEvent("Empty configuration file!", Log.Warning)
                return

            for line in lines:
                line = None if line == None else line.strip()
                Log.LogEvent(f"Reading line \"{line}\"", Log.Debug)

                if (line == None or len(line) == 0):  # blank line
                    continue

                if (line[0] == "[" and line[len(line) - 1] == "]"):  # Title Block
                    Log.LogEvent(f"Begining block \"{line[1:len(line) - 1]}\"", Log.Debug)
                    continue

                # Split the string by '='
                Splits = line.split("=")
                if (len(Splits) < 2):  # No = was found, not proper format
                    Log.LogEvent(f"The line \"{line}\" is not in proper format, to be ignored....", Log.Warning)
                    continue

                Name = Splits[0]
                Value = Splits[1]
                if (len(Splits) > 2):  # There was another = after the first one, and string re-combination must occur.
                    for i in range(2, len(Splits) - 1):  # Go through each other element and add it back together.
                        Value += "=" + Splits[i]

                Name = Name.strip().lower()  # For best searches.
                Value = Value.strip()  # For most accurate results.
                if (Configuration.__configurations.get(Name) != None):
                    Log.LogEvent(f"The configuration with name \"{Name}\" is a duplicate, only the first item is kept.",
                                 Log.Warning)
                    continue

                Configuration.__configurations.update({Name: Value})
                Log.LogEvent(f"Configuration file property \"{Name}\" was added sucessfully.", Log.Debug)

            Log.LogEvent(f"Configuration file read, {len(Configuration.__configurations)} configuration(s) were found.")

    @staticmethod
    def RetriveConfiguration(name: str) -> str:
        name = name.strip()
        Value = Configuration.__configurations.get(name)
        if (Value == None):
            Log.LogEvent(f"A configuration of name \"{name}\" was not found.", Log.Warning)
            return None

        return Value

    @staticmethod
    def SetConfiguration(name: str, value: str) -> None:
        name = name.strip()
        Value = Configuration.__configurations.get(name)
        if (Value == None):
            Log.LogEvent(f"A configuration of name \"{name}\" was not found.", Log.Warning)
            raise NameError("The name could not be found.")

        Configuration.__configurations[name] = value

    @staticmethod
    def OutputAllConfigurations() -> None:
        for element in Configuration.__configurations:
            print("\t", element)


if __name__ == "__main__":
    Log.InitLog("configuration_run.log", Log.Debug)
    Configuration.Init(input("Configuration to test path? "))
    print("All configurations found:")
    Configuration.OutputAllConfigurations()
