using System;
using System.Windows;

using HmPro.Files;
using HmPro.Events;
using Phosphor;
using HmPro.Registry;

namespace HmPro
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static Windows.Main MainIns
        {
            get; private set;
        }

        private void Application_Exit(object sender, ExitEventArgs e)
        {
        }
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            if (e.Args.Length > 0)
            {
                Ins.CurrentLoaded = e.Args[0];
                Ins.UserFileStartup = true;
            }

            BeginExit += BeginExit_Run;

            MainIns = new Windows.Main();
            if (!HmPro.Windows.Interaction.Startup.Execute()) this.Shutdown();

            if (Ins.ShowWelcome && !Ins.IsLoaded)
            {
                Windows.Interaction.Welcome Wel = new Windows.Interaction.Welcome();
                Wel.ShowDialog();
                if (Wel.ShutDown) this.Shutdown();
            }

            MainIns.ShowDialog();
            return;
        }

        public static void ExitApp()
        {
            if (Ins.IsLoaded)
            {
                Ins.CurrentLoaded = "";
                EditorReg.Close();

                foreach (Window item in Ins.OpenWindows)
                    item.Close();
            }
        }
        public static void ThrowSaved(Session Session)
        {
            SaveEventArgs e = new SaveEventArgs(Session.Path);
            EventHandler<SaveEventArgs> saving = Saved;
            if (saving != null)
            {
                Saved(Session, e);
            }
        }
        public static void ThrowAppended(IHSession Session, bool? Sucessfull)
        {
            AppendEventArgs e = new AppendEventArgs(Session.Path, Sucessfull);
            Appended?.Invoke(Session, e);
        }
        public static void ThrowBeginExit(object sender)
        {
            BeginExit?.Invoke(sender);
        }

        private void BeginExit_Run(object sender)
        {
            this.Shutdown();
        }

        public static event EventHandler<SaveEventArgs> Saved;
        public static event EventHandler<AppendEventArgs> Appended;
        public static event BlankVoid BeginExit;
    }
}
