using System;
using System.Windows;

using HmPro.Files;

namespace HmPro.Windows.Editing
{
    public partial class ScriptEditor : Window
    {
        public ScriptEditor()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) //Sets up the window and then Draws it.
        {
            Ins.OpenWindows.Add(this);

            MemeTitle.Content = Obj.Title;
            ScriptIndex = 0;

            Draw();
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)  //Removes the window from Ins.OpenWindows registry.
        {
           Ins.OpenWindows.Remove(this);
        }

        public bool AppendAction() //Saves the currently edited script.
        {
            Script Current = Obj.Scripts[ScriptIndex];

            if (ScriptPerson.Text == "" || (ScriptPosition.Text.ToLower() != "left" && ScriptPosition.Text.ToLower() != "right"))
            {
                MessageBox.Show("The script cannot be saved, as the person cannot be blank and the position must be either 'Left' or 'Right' in any case.");
                return false;
            }
            if (Obj.Scripts.IndexOf(Current) != ScriptIndex)
            {
                MessageBox.Show("The script cannot be saved, as the person and text must be unique.");
                return false;
            }

            Current.Person = ScriptPerson.Text;
            Current.Position = ScriptPosition.Text.ToString() == "left" ? ScriptPositions.Left : ScriptPositions.Right;
            Current.Text = ScriptText.Text;
            return true;
        }
        private void AppendChanges_Click(object sender, RoutedEventArgs e) //Runs the AppendAction method.
        {
            AppendAction();
        }
        private void Cancel_Click(object sender, RoutedEventArgs e) //Closes the window.
        {
            ScriptIndex = 0;
            Obj = null;
            this.Close();
        }
        private void Save_Click(object sender, RoutedEventArgs e) //Saves the file.
        {
            if (AppendAction() == false) return;
            Obj.ParentFile.Save();
        }

        private void MoveNext_Click(object sender, RoutedEventArgs e) //Moves to the next script and then draws.
        {
            if (AppendAction() == false) return;

            ScriptIndex++;
            if (ScriptIndex >= Obj.Scripts.Count)
            {
                ScriptIndex--;
                MoveNext.IsEnabled = false;
            }
            Draw();
        }
        private void MoveBack_Click(object sender, RoutedEventArgs e) //Moves to the previous script and then draws.
        {
            if (AppendAction() == false) return;

            ScriptIndex--;
            if (ScriptIndex < 0)
            {
                ScriptIndex = 0;
                MoveBack.IsEnabled = false;
            }
            Draw();
        }
        
        private void AddScript_Click(object sender, RoutedEventArgs e) //Adds a script to the meme and then draws it.
        {
            if (AppendAction() == false) return;

            Script Item = AddScript();
            ScriptIndex = Obj.Scripts.IndexOf(Item);
            Draw();
        }
        private void RemoveScript_Click(object sender, RoutedEventArgs e) //Removes the script from the selected index.
        {
            Obj.Scripts.Remove(Obj.Scripts[ScriptIndex]);
            if (ScriptIndex != 0)
            {
                ScriptIndex--;
            }
            Draw();
        }

        public Script AddScript() //Adds a script to the parent meme.
        {
            Script Return = Obj.Scripts.Add();
            ScriptPerson.Text = "";
            ScriptPosition.Text = "Left";
            ScriptText.Text = "";
            return Return;
        }

        private Meme Obj; //The parent meme that information is pulled from.
        private int ScriptIndex = 0; //The index of the current script.

        private void Draw() //Display's the current scripts properties.
        {
            Script Current;
            if (Obj.Scripts.Count == 0)
            {
                Current = AddScript();
            }
            else
            {
                Current = Obj.Scripts[ScriptIndex];
            }

            ScriptPerson.Text = Current.Person;
            ScriptText.Text = Current.Text;
            ScriptPosition.Text = Current.Position == ScriptPositions.Left ? "Left" : "Right";

            if (ScriptIndex >= Obj.Scripts.Count && ScriptIndex != 0)
            {
                ScriptIndex = Obj.Scripts.Count - 1;
                MoveNext.IsEnabled = false;
                MoveBack.IsEnabled = true;
            }
            else if (ScriptIndex <= 0)
            {
                ScriptIndex = 0;
                MoveNext.IsEnabled = true;
                MoveBack.IsEnabled = false;
            }
            else
            {
                MoveNext.IsEnabled = true;
                MoveBack.IsEnabled = true;
            }

            IndexDisp.Content = $"Item {ScriptIndex + 1} of {Obj.Scripts.Count}";
        }
        public void Execute(Meme Source) //Loads the window using a source meme.
        {
            if (Source.Type != MemeTypes.Script)
                throw new InvalidOperationException("The Meme 'Source' was was not the expected type of 'MemeTypes.Script'.");

            this.Obj = Source;
            this.Show();
        }        
    }
}