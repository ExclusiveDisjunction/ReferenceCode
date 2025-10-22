using System.Xml;
using System.IO;
using System.Windows;
using System.Collections.Generic;

using Phosphor;
using Phosphor.Files;
using HmPro.Files;

/*
    Ins - Complies to the Instance (Ins) Standards 2.0.0.0
 */

namespace HmPro
{
    public static class Ins
    {
        public static bool IsLoaded //Determines if CurrentSession Exists.
        {
            get => File.Exists(_Loaded);
        }

        public static IHSession LoadedSession
        {
            get
            {
                if (!IsLoaded)
                    return null;
                else if (FileTools.FileExt(_Loaded, false) == "teh")
                    return new Session(_Loaded);
                else if (FileTools.FileExt(_Loaded, false) == "tehseg")
                    return new Segment(_Loaded);
                else
                    return null;
            }
        }
        private static string _Loaded = ""; //The stored Loaded path.
        public static string CurrentLoaded //The path of the session to save to. When set, if the file does not exist, it closes the session & throws the Unloaded event & sets _Loaded = "". If it does, it loads a new one & throws the Loaded event.
        {
            get => _Loaded;
            set
            {
                if (value is null || !File.Exists(value))
                {
                    _Loaded = "";
                    Unloaded?.Invoke(null, null);
                }
                else
                {
                    _Loaded = value;
                    Loaded?.Invoke(null, null);
                }
            } 
        } 

        public static string LastLoaded //The last loaded path in the program.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "LastLoaded"];
                if (Item is null)
                {
                    Parser.Add("LastLoaded", SettingsItemTypes.SettingItem);
                    return "";
                }

                return Item.Value;
            }
            set
            {
                if (value is null)
                    value = "";

                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "LastLoaded"];
                if (Item is null)
                {
                    Parser.Add("LastLoaded", SettingsItemTypes.SettingItem, value);
                    return;
                }

                Item.Value = value;
            }
        }
        public static bool LoadLast //Determines if the program should load up the last project on starup.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "LoadLast"];
                if (Item is null)
                {
                    Parser.Add("LoadLast", SettingsItemTypes.SettingItem, "False");
                    return false;
                }

                return Item.Value == "True" ? true : false;
            }
            set
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "LoadLast"];
                if (Item is null)
                {
                    Parser.Add("LastLoaded", SettingsItemTypes.SettingItem, $"{value}");
                    return;
                }

                Item.Value = $"{value}";
            }
        }
        public static bool FirstTime //Determines if it is the first time that the app has been used.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "FirstTime"];
                if (Item is null)
                {
                    Parser.Add("FirstTime", SettingsItemTypes.SettingItem, "False");
                    return false;
                }

                return Item.Value == "True" ? true : false;
            }
            set
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "FirstTime"];
                if (Item is null)
                {
                    Parser.Add("FirstTime", SettingsItemTypes.SettingItem, $"{value}");
                    return;
                }

                Item.Value = $"{value}";
            }
        }
        public static bool ShowWelcome //Determines if the Welcome Screen should show up on startup.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "ShowWelcome"];
                if (Item is null)
                {
                    Parser.Add("ShowWelcome", SettingsItemTypes.SettingItem, "False");
                    return false;
                }

                return Item.Value == "True" ? true : false;
            }
            set
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "ShowWelcome"];
                if (Item is null)
                {
                    Parser.Add("ShowWelcome", SettingsItemTypes.SettingItem, $"{value}");
                    return;
                }

                Item.Value = $"{value}";
            }
        }
        public static bool DevMode //Determines if the app is in dev mode, used to skip startup procedures.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "DevMode"];
                if (Item is null)
                {
                    Parser.Add("DevMode", SettingsItemTypes.SettingItem, "False");
                    return false;
                }

                return Item.Value == "True" ? true : false;
            }
            set
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "DevMode"];
                if (Item is null)
                {
                    Parser.Add("DevMode", SettingsItemTypes.SettingItem, $"{value}");
                    return;
                }

                Item.Value = $"{value}";
            }
        }
        public static bool SaveMemeCreator //When this is ommited true, the Main.ResetEditor does not change the admCreator.Text property. This is to keeps things simpler for the user.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "SaveMemeCreator"];
                if (Item is null)
                {
                    Parser.Add("SaveMemeCreator", SettingsItemTypes.SettingItem, "False");
                    return false;
                }

                return Item.Value == "True" ? true : false;
            }
            set
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "SaveMemeCreator"];
                if (Item is null)
                {
                    Parser.Add("SaveMemeCreator", SettingsItemTypes.SettingItem, $"{value}");
                    return;
                }

                Item.Value = $"{value}";
            }
        }
        public static WindowState LastState //The window state of main, used to automaticly restore window state for main.
        {
            get
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "LastState"];
                if (Item is null)
                {
                    Parser.Add("LastState", SettingsItemTypes.SettingItem, "Normal");
                    return WindowState.Normal;
                }

                switch (Item.Value)
                {
                    case "Normal":
                        return WindowState.Normal;
                    case "Maximized":
                        return WindowState.Maximized;
                    case "Minimized":
                        return WindowState.Minimized;
                    default:
                        Item.Value = "Normal";
                        return WindowState.Normal;
                }
            }
            set
            {
                SettingsParser Parser = new SettingsParser("instance\\ins.tehs");

                SettingsItem Item = Parser[SettingsItemTypes.SettingItem, "LastState"];
                if (Item is null)
                {
                    Parser.Add("LastState", SettingsItemTypes.SettingItem, $"{value}");
                    return;
                }

                Item.Value = $"{value}";
            }
        }

        public static event System.EventHandler Loaded; //Thrown when you set the current loaded property and the file exists.
        public static event System.EventHandler Unloaded; //Thrown when you set the current loaded property and the file does not exist.

        public static List<Window> OpenWindows = new List<Window>(); //Whenever there is a window open, this is added to. When it closes, it is supposed to remove it. When the app closes, it closes all of these open windows.

        public const string Version = "2020.1 BETA";

        public const string BasicSession = "template\\session\\basic.teh"; //Returns the path of a basic session.
        public const string BasicSegment = "template\\session\\basic.tehseg";

        public static void Repair()
        {
            SettingsParser.Create("instance\\ins.tehs");
            SettingsParser.Create("instance\\recent.tehs");
        }

        public static bool UserFileStartup = false; //This is set to true when startup detects that the user started up the program using a file in File Explorer, and it tells startup to load the file in.
    }

    public enum SettingTypes { Recent, Instance, Asset, Text }
}