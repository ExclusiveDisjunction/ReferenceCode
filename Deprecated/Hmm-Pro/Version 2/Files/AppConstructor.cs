using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Xml.Linq;
using Phosphor.Files;
using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This class is used to set up all of the files and folders in Hmmm Pro. 
    /// </summary>
    public static class AppConstructor
    {
        public static void Repair()
        {
            using (StreamWriter stream = new StreamWriter("dir.tehst"))
            {
                stream.Write("crash\n" +
                    "instance\n" +
                    "template\n" +
                    "template\\session\n" +
                    "doc\n" +
                    "doc\\2020.1");
            }
            using (StreamWriter stream = new StreamWriter("file.tehst"))
            {
                stream.Write("template\\session\\basic.teh\n" +
                    "instance\\ins.tehs\n" +
                    "instance\\editor.tehs\n" +
                    "instance\\recent.tehs\n" +
                    "Hmmm Pro.exe\n" +
                    "Hmmm Pro.exe.config\n" +
                    "Phosphor.Tools.dll\n" +
                    "doc\\2020.1\\Intro.tehst");
            }

            if (!Directory.Exists("instance"))
                Directory.CreateDirectory("instance");

            SettingsParser.Create("instance\\ins.tehs");
            SettingsParser.Create("instance\\recent.tehs");

            Session.Create("template\\session\\basic.teh");
            Segment.Create("template\\session\\basic.tehseg");
        }
        public static bool CheckFiles() //returns true for no errors, false for an error
        { 
            if (!File.Exists("dir.tehst"))
                return false;
            if (!File.Exists("file.tehst"))
                return false;

            foreach (string path in File.ReadAllLines("dir.tehst"))
            {
                if (!Directory.Exists(path))
                    return false;
            }
            foreach (string path in File.ReadAllLines("file.tehst"))
            {
                if (!File.Exists(path))
                    return false;
            }

            return true;
        }
    }
}
