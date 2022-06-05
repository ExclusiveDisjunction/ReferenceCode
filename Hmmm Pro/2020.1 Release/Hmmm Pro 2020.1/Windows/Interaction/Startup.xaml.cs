using System;
using System.Windows;
using System.IO;
using System.Collections.Generic;

using Phosphor;
using System.Runtime.CompilerServices;

namespace HmPro.Windows.Interaction
{
    /// <summary>
    /// Interaction logic for Windows\Startup.xaml
    /// </summary>
    public partial class Startup : Window
    {
        private Startup()
        {
            InitializeComponent();
        }

        private bool Return = true;

        private void Window_ContentRendered(object sender, EventArgs e) //Runs the apps true startup features, reading the Ins settings.
        {
            Ins.OpenWindows.Add(this);
            Vers.Content = Ins.Version;

            if (Ins.FirstTime)
            {
                Files.AppConstructor.Repair();
                Ins.FirstTime = false;
            }

            if (!Files.AppConstructor.CheckFiles())
            {
                MessageBoxResult Result = MessageBox.Show("The app could not be opened.\n\nDo you want to repair? Notice: Reparing will reset ALL recents and user settings.", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Error);

                switch (Result)
                {
                    case MessageBoxResult.Yes:
                        Files.AppConstructor.Repair();
                        break;
                    case MessageBoxResult.No:
                    case MessageBoxResult.Cancel:
                    case MessageBoxResult.None:
                        Return = false;
                        this.Close();
                        return;
                }                
            }

            string NewText = "";
            if (Ins.UserFileStartup) NewText += " | User Opened Path";
            NewText += Ins.LoadLast ? " | Load Last Session: ON" : " | Load Last Session: OFF";
            NewText += Ins.DevMode ? " | Developer Mode: ON" : " | Developer Mode: OFF";
            NewText += Ins.ShowWelcome ? " | Show Welcome: ON" : " | Show Welcome: OFF";
            NewText += $" | Last Window State: {Ins.LastState}";

            ConfigText.Text += NewText;

            if (Ins.UserFileStartup)
            {
                if (Ins.DevMode)
                {
                    this.Close();
                    return;
                }

                Timer.Wait(4);

                this.Close();
            }

            if (Ins.DevMode)
            {
                this.Close();
                return;
            }

            if ((Ins.LastLoaded != "" && Ins.LoadLast) && !Ins.UserFileStartup)
            {
                Timer.WaitSeconds(5);
                if (!File.Exists(Ins.LastLoaded))
                {
                    MessageBoxResult result = MessageBox.Show($"The path '{Ins.LastLoaded}' does not exist on your computer. Do you want to find it?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Warning);
                    switch (result)
                    {
                        case MessageBoxResult.No:
                            goto Elsewise;
                        case MessageBoxResult.Cancel:
                            this.Close();
                            return;
                        case MessageBoxResult.None:
                            this.Close();
                            return;
                        case MessageBoxResult.Yes:
                            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog()
                            {
                                Title = "Find Missing File:",
                                RestoreDirectory = true,
                                Filter = "Hmmm Pro Sessions | *.teh",
                                AddExtension = true,
                                DefaultExt = ".teh",
                                Multiselect = false
                            };

                            open.ShowDialog();

                            string path = open.FileName;

                            if (path == "")
                                goto Elsewise;

                            Ins.CurrentLoaded = path;

                            this.Close();
                            return;
                    }
                }

                SessionControl.OpenFile(Ins.LastLoaded);

                this.Close();
                return;
            }

            Elsewise:

            Timer.Wait(6);

            this.Close();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) //Removes this window from the Ins.OpenWindows registry.
        {
            Ins.OpenWindows.Remove(this);
        }

        public static bool Execute()
        {
            Startup st = new Startup();
            st.ShowDialog();
            return st.Return;
        }

        

        private void Repair(Missing MissingFile) //Repairs a missing file from the Missing Enum.
        {
            if (MissingFile == Missing.Dir)
            {
                using (StreamWriter file = new StreamWriter("instance\\dir.tehst"))
                {
                    file.WriteLine("assets");
                    file.WriteLine("crash");
                    file.WriteLine("current");
                    file.WriteLine("instance");
                    file.WriteLine("template");
                    file.WriteLine("template\\session");
                    file.WriteLine("template\\setting");
                    file.Close();
                }
    }
            else
            {
                using (StreamWriter file = new StreamWriter("instance\\file.tehst"))
                {
                    file.WriteLine("template\\session\\basic.teh");
                    file.WriteLine("template\\setting\\instance.tehs");
                    file.WriteLine("template\\setting\\recent.tehs");
                    file.WriteLine("template\\setting\\text.tehst");
                    file.WriteLine("instance\\dir.tehst");
                    file.WriteLine("instance\\editor.tehs");
                    file.WriteLine("instance\\file.tehst");
                    file.WriteLine("instance\\instance.tehs");
                    file.WriteLine("instance\\recent.tehs");
                    file.Close();
                }
            }
        }

        private enum Missing { File, Dir} //Used to identifiy which key file is missing from Hmmm Pro.
    }
}
