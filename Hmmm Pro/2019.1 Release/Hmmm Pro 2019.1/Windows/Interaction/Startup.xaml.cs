using System;
using System.Windows;
using System.IO;
using System.Collections.Generic;

using HmPro.Scripting;
using Lyseria;

namespace HmPro.Windows.Interaction
{
    /// <summary>
    /// Interaction logic for Windows\Startup.xaml
    /// </summary>
    public partial class Startup : Window
    {
        public Startup()
        {
            InitializeComponent();
        }

        private void Window_ContentRendered(object sender, EventArgs e) //Runs the apps true startup features, reading the Ins settings.
        {
            Ins.OpenWindows.Add(this);
            Vers.Content = Ins.Version;

            if (!CheckFiles())
            {
                MessageBox.Show("The app could not be opened.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            Main ms = App.MainIns;

            if (Ins.DevMode)
            {
                ms.Show();
                this.Close();
                return;
            }

            if (Ins.UserFileStartup)
            {
                Progress.Text = $"Loading file '{Ins.CurrentLoaded}'...";
                Timer.WaitSeconds(6);

                Timer.WaitSeconds(6);
                Progress.Text = "Loading Components...";
                Timer.WaitSeconds(3);
                Progress.Text = "Initalizing...";
                Timer.Wait(2);

                ms.OpenFromPath(Ins.CurrentLoaded);
                ms.Show();
                this.Close();
            }

            if ((Ins.LastLoaded != "" && Ins.LoadLast) && !Ins.UserFileStartup)
            {
                Progress.Text = "Loading Last Session...";
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
                        case MessageBoxResult.Yes:
                            Progress.Text = "Locate Missing File...";
                            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog()
                            {
                                Title = "Find Missing File:",
                                RestoreDirectory = true,
                                Filter = "Gae Sessions | *.teh",
                                AddExtension = true,
                                DefaultExt = ".teh",
                                Multiselect = false
                            };

                            open.ShowDialog();

                            string path = open.FileName;

                            if (path == "")
                            {
                                goto Elsewise;
                            }
                            Progress.Text = $"Loading Path '{path}'...";
                            Timer.WaitSeconds(3);
                            ms.OpenFromPath(path);
                            ms.Show();
                            this.Close();
                            return;
                    }
                }
                Timer.WaitSeconds(4);
                ms.OpenFromPath(Ins.LastLoaded);
                ms.Show();
                this.Close();
                return;
            }

            Elsewise:
            if (Ins.IsLoaded)
            {
                MessageBoxResult result = MessageBox.Show("The application may not have shut down correctly. Do you want to restore crash data?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                switch (result)
                {
                    case MessageBoxResult.Cancel | MessageBoxResult.None:
                        File.Delete(Ins.CurrentSession);
                        this.Close();
                        return;
                    case MessageBoxResult.Yes:
                        Progress.Text = "Restoring Crash Data...";
                        if (File.Exists(Ins.CurrentLoaded))
                        {
                            File.Delete(Ins.CurrentLoaded);
                            File.Copy(Ins.CurrentSession, Ins.CurrentLoaded);
                            Progress.Text = $"Loading Session '{Ins.CurrentLoaded}'...";
                            Timer.WaitSeconds(3);
                            ms.OpenFromPath(Ins.CurrentLoaded);
                        }
                        else
                        {
                            Progress.Text = "Locate Missing File...";
                            Microsoft.Win32.OpenFileDialog open = new Microsoft.Win32.OpenFileDialog()
                            {
                                Title = "Find Missing File:",
                                RestoreDirectory = true,
                                Filter = "Gae Sessions | *.teh",
                                AddExtension = true,
                                DefaultExt = ".teh",
                                Multiselect = false
                            };

                            open.ShowDialog();

                            string path = open.FileName;

                            if (path == "")
                            {
                                Progress.Text = "Removing Crash Data...";
                                Timer.WaitSeconds(3);
                                File.Delete(Ins.CurrentSession);
                            }

                            Progress.Text = $"Loading Session '{path}'...";
                            ms.OpenFromPath(path);
                        }
                        break;
                    case MessageBoxResult.No:
                        File.Delete(Ins.CurrentSession);
                        break;
                }
            }

            Timer.WaitSeconds(6);
            Progress.Text = "Loading Components...";
            Timer.WaitSeconds(3);
            Progress.Text = "Initalizing...";
            Timer.Wait(2);

            ms.Show();
            this.Close();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) //Removes this window from the Ins.OpenWindows registry.
        {
            Ins.OpenWindows.Remove(this);
        }

        public bool CheckFiles() //returns true for no errors, false for an error
        {
            Progress.Text = "Evaluating Application Files...";

            List<string> Errors = new List<string>();
            List<string> Warnings = new List<string>();
            int ErrorCount = 1;
            int WarningCount = 1;

            if (!File.Exists("instance\\dir.tehst"))
            {
                Warnings.Add($"Warning #{WarningCount}: File 'instance\\dir.tehst' does not exist. This is required, but is replaced.");
                Repair(Missing.Dir);
                WarningCount++;
            }
            if (!File.Exists("instance\\file.tehst"))
            {
                Warnings.Add($"Warning #{WarningCount}: File 'instance\\file.tehst' does not exist. This is required, but is replaced.");
                Repair(Missing.File);
            }

            foreach (string path in File.ReadAllLines("instance\\dir.tehst"))
            {
                if (!Directory.Exists(path))
                {
                    if (path == "instance" || path == "template" || path == "template\\session" || path == "template\\setting")
                    {
                        Errors.Add($"Error #{ErrorCount}: Folder '{path}' does not exist. This is required.");
                        ErrorCount++;
                    }
                    Directory.CreateDirectory(path);
                } 
            }

            foreach (string path in File.ReadAllLines("instance\\file.tehst"))
            {
                if (!File.Exists(path))
                {
                    Errors.Add($"Error #{ErrorCount}: File '{path}' does not exist. This is required.");
                }
            }

            if (Errors.Count != 0)
            {
                MessageBox.Show("The application is missing core files and/or folders. When you press OK, a dialog will prompt you to save the report.\nTo resolve these errors, please contact me at Connorhcs17@outlook.com with the subject of 'HMPROERR'.", "Startup Failure:", MessageBoxButton.OK, MessageBoxImage.Error);

                Microsoft.Win32.SaveFileDialog save = new Microsoft.Win32.SaveFileDialog()
                {
                    AddExtension = true,
                    DefaultExt = ".txt",
                    Filter = "Text Files | *.txt",
                    RestoreDirectory = true,
                    Title = "Select a location for the error messages."
                };

                save.ShowDialog();

                string path = save.FileName;

                if (path == "")
                {
                    this.Close();
                    return false;
                }

                Progress.Text = "Creating Error Report...";

                Timer.WaitSeconds(2);

                using (StreamWriter file = new StreamWriter(path))
                {
                    foreach (string line in Warnings)
                    {
                        file.WriteLine(line);
                    }
                    foreach (string line in Errors)
                    {
                        file.WriteLine(line);
                    }
                    file.Close();
                }

                this.Close();
                return false;
            }

            return true;
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
