using System.Xml;
using System.IO;
using System.Windows;
using System.Collections.Generic;

using Lyseria;
using HmPro.Scripting.Files;

namespace HmPro.Scripting
{
    public static class Ins
    {
        public static bool IsLoaded //Determines if CurrentSession Exists.
        {
            get
            {
                return File.Exists(CurrentSession);
            }
        }

        public static string CurrentSession //The path of the session when it is loaded into the program.
        {
            get => @"current\currentsession.teh";
        }

        private static string _Loaded = ""; //The stored Loaded path.
        public static string CurrentLoaded //The path of the session to save to. When set, if the file does not exist, it closes the session & throws the Unloaded event & sets _Loaded = "". If it does, it loads a new one & throws the Loaded event.
        {
            get => _Loaded;
            set
            {
                if (value is null || !File.Exists(value))
                {
                    if (IsLoaded)
                        File.Delete(CurrentSession);
                    _Loaded = "";
                    Unloaded?.Invoke(null, null);
                }
                else
                {
                    if (IsLoaded)
                        File.Delete(CurrentSession);
                    File.Copy(value, CurrentSession);
                    _Loaded = value;
                    Loaded?.Invoke(null, null);
                }
            } 
        } 
        public static string LastLoaded //The last loaded path in the program.
        {
            get
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                return file.DocumentElement.Attributes.Item(1).Value;
            }
            set
            {
                if (value is null)
                    value = "";

                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                file.DocumentElement.Attributes.Item(1).Value = value;
                file.Save("instance\\ins.tehs");
            }
        }
        public static bool LoadLast //Determines if the program should load up the last project on starup.
        {
            get
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                if (file.DocumentElement.Attributes.Item(3).Value == bool.TrueString)
                {
                    return true;
                }
                return false;
            }
            set
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                file.DocumentElement.Attributes.Item(3).Value = value.ToString();
                file.Save("instance\\ins.tehs");
            }
        }
        public static bool FirstTime //Determines if it is the first time that the app has been used.
        {
            get
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                if (file.DocumentElement.Attributes.Item(4).Value == bool.TrueString)
                {
                    return true;
                }
                return false;
            }
            set
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                file.DocumentElement.Attributes.Item(4).Value = value.ToString();
                file.Save("instance\\ins.tehs");
            }
        }
        public static bool ShowWelcome //Determines if the Welcome Screen should show up on startup.
        {
            get
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                if (file.DocumentElement.Attributes.Item(5).Value == bool.TrueString)
                {
                    return true;
                }
                return false;
            }
            set
            {
                XmlDocument file = new XmlDocument();
                file.Load("instance\\ins.tehs");

                file.DocumentElement.Attributes.Item(5).Value = value.ToString();
                file.Save("instance\\ins.tehs");
            }
        }
        public static bool DevMode //Determines if the app is in dev mode, used to skip startup procedures.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\ins.tehs");

                if (data.DocumentElement.Attributes.Item(7).Value == bool.TrueString)
                {
                    return true;
                }
                return false;
            }
            set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\ins.tehs");

                data.DocumentElement.Attributes.Item(7).Value = value.ToString();

                data.Save("instance\\ins.tehs");
            }
        }
        public static bool SaveMemeCreator //When this is ommited true, the Main.ResetEditor does not change the admCreator.Text property. This is to keeps things simpler for the user.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\ins.tehs");

                if (data.DocumentElement.Attributes.Item(8).Value == bool.TrueString)
                {
                    return true;
                }
                else if (data.DocumentElement.Attributes.Item(8).Value == bool.FalseString)
                {
                    return false;
                }
                else
                {
                    data.DocumentElement.Attributes.Item(8).Value = bool.FalseString;
                    return false;
                }
            }
            set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\ins.tehs");

                data.DocumentElement.Attributes.Item(8).Value = $"{value}";

                data.Save("instance\\ins.tehs");
            }
        }

        public static event System.EventHandler Loaded; //Thrown when you set the current loaded property and the file exists.
        public static event System.EventHandler Unloaded; //Thrown when you set the current loaded property and the file does not exist.

        public static List<Window> OpenWindows = new List<Window>(); //Whenever there is a window open, this is added to. When it closes, it is supposed to remove it. When the app closes, it closes all of these open windows.
        public static WindowState LastState //The window state of main, used to automaticly restore window state for main.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\ins.tehs");

                switch (data.DocumentElement.Attributes.Item(6).Value)
                {
                    case "Normal":
                        return WindowState.Normal;
                    case "Maximized":
                        return WindowState.Maximized;
                    case "Minimized":
                        return WindowState.Minimized;
                    default:
                        throw new System.Exception();
                }
            }
            set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\ins.tehs");

                data.DocumentElement.Attributes.Item(6).Value = value.ToString();

                data.Save("instance\\ins.tehs");
            }
        }

        public static string Version
        {
            get => "2019.1";
        }

        public static string BasicTemplate //Returns the path of a basic session.
        {
            get
            {
                return @"template\session\basic.teh";
            }
        }
        public static string BasicSetting(SettingTypes Type) //Returns the path of a setting file based of a type.
        {
            switch (Type)
            {
                case SettingTypes.Asset:
                    return @"template\setting\asset\basic.tehs";
                case SettingTypes.Instance:
                    return @"template\setting\instance\basic.tehs";
                case SettingTypes.Recent:
                    return @"template\setting\recent\basic.tehs";
                case SettingTypes.Text:
                    return @"template\setting\text\basic.tehst";
                default:
                    throw new System.Exception();
            }
        }

        public static string GetRecent(RecentOptions Item) //Gets a recent item from recents.
        {
            XmlDocument Recent = new XmlDocument();
            Recent.Load("instance\\recent.tehs");

            if (Recent.DocumentElement.Attributes.Item((int)Item).Value == "")
            {
                return null;
            }
            return Recent.DocumentElement.Attributes.Item((int)Item).Value;
        }
        public static void SetRecent(string Value) //Sets the value of the first recent and slides all other recents back.
        {
            XmlDocument Recent = new XmlDocument();
            Recent.Load("instance\\recent.tehs");

            XmlAttributeCollection Recents = Recent.DocumentElement.Attributes;
            
            RecentOptions? exists(string item)
            {
                if (Recents.Item(5).Value == item)
                {
                    return RecentOptions.Rec5;
                }
                else if (Recents.Item(4).Value == item)
                {
                    return RecentOptions.Rec4;
                }
                else if (Recents.Item(3).Value == item)
                {
                    return RecentOptions.Rec3;
                }
                else if (Recents.Item(2).Value == item)
                {
                    return RecentOptions.Rec2;
                }
                else if (Recents.Item(1).Value == item)
                {
                    return RecentOptions.Rec1;
                }
                return null;
            }

            RecentOptions? value = exists(Value);
            if (value == null)
            {
                Recents.Item(5).Value = Recents.Item(4).Value;
                Recents.Item(4).Value = Recents.Item(3).Value;
                Recents.Item(3).Value = Recents.Item(2).Value;
                Recents.Item(2).Value = Recents.Item(1).Value;
                Recents.Item(1).Value = Value;
            }
            else
            {
                switch (value)
                {
                    case RecentOptions.Rec1:
                        break;
                    case RecentOptions.Rec2:
                    {
                        string Item1 = Recents.Item(1).Value;
                        Recents.Item(1).Value = Recents.Item(2).Value;
                        Recents.Item(2).Value = Item1;
                        Recents.Item(3).Value = Recents.Item(4).Value;
                        Recents.Item(4).Value = Recents.Item(5).Value;
                        Recents.Item(5).Value = "";
                        break;
                    }
                    case RecentOptions.Rec3:
                    {
                        string Item1 = Recents.Item(1).Value;
                        string Item2 = Recents.Item(2).Value;

                        Recents.Item(1).Value = Recents.Item(3).Value;
                        Recents.Item(2).Value = Item1;
                        Recents.Item(3).Value = Item2;
                        Recents.Item(4).Value = Recents.Item(5).Value;
                        Recents.Item(5).Value = "";
                        break;
                    }
                    case RecentOptions.Rec4:
                    {
                        string Item1 = Recents.Item(1).Value;
                        string Item2 = Recents.Item(2).Value;
                        string Item3 = Recents.Item(3).Value;

                        Recents.Item(1).Value = Recents.Item(4).Value;
                        Recents.Item(2).Value = Item1;
                        Recents.Item(3).Value = Item2;
                        Recents.Item(4).Value = Item3;
                        Recents.Item(5).Value = "";
                        break;
                    }
                    case RecentOptions.Rec5:
                    {
                        string Item1 = Recents.Item(1).Value;
                        string Item2 = Recents.Item(2).Value;
                        string Item3 = Recents.Item(3).Value;
                        string Item4 = Recents.Item(4).Value;

                        Recents.Item(1).Value = Recents.Item(5).Value;
                        Recents.Item(2).Value = Item1;
                        Recents.Item(3).Value = Item2;
                        Recents.Item(4).Value = Item3;
                        Recents.Item(5).Value = Item4;
                        break;
                    }
                }
            }
           
            Recent.Save("instance\\recent.tehs");
        }
        public static string RecentName(RecentOptions Option) //Returns the name of a recent option.
        {
            string Path = GetRecent(Option);

            try
            {
                Session sess = new Session(Path);
                return sess.Title;
            }
            catch
            {
                return "#Missing";
            }
        }

        public static bool UserFileStartup = false; //This is set to true when startup detects that the user started up the program using a file in File Explorer, and it tells startup to load the file in.
    }

    public enum SettingTypes { Recent, Instance, Asset, Text }
}