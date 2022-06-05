using System;
using System.Windows;
using HmPro.Scripting.Files;
using HmPro.Scripting;
using System.IO;
using HmPro.Scripting.Events;

namespace HmPro
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static Windows.Main MainIns;

        private void Application_Exit(object sender, ExitEventArgs e) //Does nothing
        {
        }
        private void Application_Startup(object sender, StartupEventArgs e) //Loads the application and uses the e.Args parameter to determine if the user requiested a file be opened.
        {
            if (e.Args.Length > 0)
            {
                Ins.CurrentLoaded = e.Args[0];
                
                if (File.Exists(Ins.CurrentSession))
                {
                    File.Delete(Ins.CurrentSession);
                }
                File.Copy(e.Args[0], Ins.CurrentSession);
            }

            MainIns = new Windows.Main();

            Windows.Interaction.Startup Startup = new Windows.Interaction.Startup();
            Startup.Show();
            return;
        }

        public static void ExitApp() //Closes down the application.
        { 
            
            if (Ins.IsLoaded)
            {
                MessageBoxResult result = MessageBox.Show("Do you want to save?", "Hmmm Pro:", MessageBoxButton.YesNo, MessageBoxImage.Question);
                if (result == MessageBoxResult.Yes)
                {
                    if (!File.Exists(Ins.CurrentLoaded))
                    {
                        MessageBox.Show("The save could not be completed. The file will be saved in crash.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);

                        Session sess = new Session();
                        if (!Directory.Exists($"crash\\{sess.Name}"))
                        {
                            Directory.CreateDirectory($"crash\\{sess.Name}");
                        }

                        if (File.Exists($"crash\\{sess.Name}\\{sess.Name}.teh"))
                        {
                            File.Delete($"crash\\{sess.Name}\\{sess.Name}.teh");
                        }
                        File.Copy(Ins.CurrentSession, $"crash\\{sess.Name}\\{sess.Name}");
                        return;
                    }
                    File.Delete(Ins.CurrentLoaded);
                    File.Copy(Ins.CurrentSession, Ins.CurrentLoaded);

                    Ins.LastLoaded = Ins.CurrentLoaded;
                    File.Delete(Ins.CurrentSession);
                    return;
                }
                if (result == MessageBoxResult.No || result == MessageBoxResult.None)
                {
                    File.Delete(Ins.CurrentSession);
                    Ins.LastLoaded = Ins.CurrentLoaded;

                    Scripting.Functions.Edit.EditorReg.Close();

                    return;
                }

                Scripting.Functions.Edit.EditorReg.Close();
            }
        }
        public static void ThrowSaved(Session Session) //Throws the Saved event.
        {
            SaveEventArgs e = new SaveEventArgs(Session.Path);
            EventHandler<SaveEventArgs> saving = Saved;
            if (saving != null)
            {
                Saved(Session, e);
            }
        }
        public static void ThrowAppended(Session Session, bool? Sucessfull) //Throws the Appended event.
        {
            AppendEventArgs e = new AppendEventArgs(Session.Path, Sucessfull);
            EventHandler<AppendEventArgs> appedn = Appended;
            if (appedn != null)
            {
                Appended(Session, e);
            }
        }

        public static event EventHandler<SaveEventArgs> Saved; //Thrown when the file is saved.
        public static event EventHandler<AppendEventArgs> Appended; //Thrown when Hmmm Pro appends.
    }
}
