using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using HmPro.Files;
using HmPro.Events;
using HmPro.Edit;
using System.Linq;

namespace HmPro.Windows.Editing
{
    /// <summary>
    /// Interaction logic for Collection Connector.xaml
    /// </summary>
    public partial class ObjectSelector : Window
    {
        private ObjectSelector()
        {
            InitializeComponent();
        }

        private event EventHandler<ObjectSelectionArgs> Connected; //The event raised when the user returns a response.
        private IComponent Selected; //The selected item from the tree.        
        private string Notes = null;
        private bool MultipleInstance = false;

        private bool Collections = true; //Sets it that collections are visible.
        private bool MasterCollections = true; //Sets it that mastercollections are visible.
        private bool Memes = false; //Determines if statup should show memes.
        private bool Bins = true;

        private List<string> TitleMaster = new List<string>();

        private void Window_Loaded(object sender, RoutedEventArgs e) //Runs on startup.
        {
            Ins.OpenWindows.Add(this);

            TreeViewItem Item = SessionBridge.GetObjects(Ins.CurrentLoaded, MasterCollections, Bins, Memes, Collections, true);
            VisualTree.Items.Add(Item);
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Ins.OpenWindows.Remove(this);
        }

        private void VisualTree_Selected(object sender, RoutedEventArgs e) //Sets the information on the form using the selected item from the tree.
        {
            if (!(VisualTree.SelectedItem != null && VisualTree.SelectedItem is TreeViewItem item && item.Header is string))
            {
                ObjTitle.Text = "";
                Type.Text = "";
                this.Selected = null;
                this.Notes = "";
                MultipleInstance = false;
            }

            TreeViewItem selected = (TreeViewItem)VisualTree.SelectedItem;

            string Name = (string)selected.Header;

            if (Name == Ins.LoadedSession.Title)
            {
                ObjTitle.Text = Name;
                Type.Text = Ins.LoadedSession is Session ? "Session File" : Ins.LoadedSession is Segment ? "Segment File" : "Unreconized Project";

                this.Selected = null;
                this.Notes = null;
                MultipleInstance = false;
                return;
            }

            if (Name == "Legendary Memes" || Name == "Favorite Memes" || Name == "Memes" || Name == "Collections" || Name == "Bins" || Name == "Memes")
            {
                ObjTitle.Text = Name;
                Type.Text = "Group Header";
                this.Selected = null;
                this.Notes = Name == "Legendary Memes" || Name == "Favorite Memes " ? Name : null;

                MultipleInstance = false;
                return;
            }
            else
            {
                Session Session = new Session(Ins.CurrentLoaded);
                IComponent[] NewSelectedList = Session.GetObject(Name, Memes, Collections, MasterCollections);

                if (NewSelectedList is null || NewSelectedList.Contains(null)) 
                {
                    ObjTitle.Text = "No Object Selected";
                    Type.Text = "No Type";
                    this.Selected = null;
                    this.Notes = null;

                    MultipleInstance = false;
                    return;
                }

                string TypeString;                
                if (NewSelectedList.Length == 1)
                    TypeString = $"{NewSelectedList[0].ObjectType}";
                else
                    TypeString = "Duplicate Items, Multiple Types.";

                if (NewSelectedList.Length > 1)
                    MultipleInstance = true;
                else
                    MultipleInstance = false;

                IComponent NewSelected = NewSelectedList[0];

                Type.Text = TypeString;
                ObjTitle.Text = NewSelected.Title;

                this.Selected = NewSelected;
                this.Notes = null;
            }
        }

        private void Connect_Click(object Sender, RoutedEventArgs e) //Throws the Selected event with the selected object. and sets Waiting = false;
        {
            if (MultipleInstance)
                Selected = DuplicateSelector.Execute(Selected.Title);

            ObjectSelectionArgs E = new ObjectSelectionArgs(Selected, true, new Session(Ins.CurrentLoaded), Notes);
            this.Connected?.Invoke(this, E);

            this.Close();
        }
        private void CancelJob_Click(object sender, RoutedEventArgs e) //Throws the Selected event with a null object and sets Waiting = false;
        {
            ObjectSelectionArgs E = new ObjectSelectionArgs(null, false, null);
            this.Connected?.Invoke(this, E);

            this.Close();
        }

        /// <summary>
        /// Used to run the entire form, and returns the user's response.
        /// </summary>
        /// <returns>The eventargs of the button's click.</returns>
        public static ObjectSelectionArgs Execute(bool Memes = false, bool Collections=true, bool MasterCollections = true, bool Bins = true)
        {
            ObjectSelector This = new ObjectSelector
            {
                Collections = Collections,
                MasterCollections = MasterCollections,
                Memes = Memes,
                Bins = Bins
            };

            ObjectSelectionArgs Return = null;

            This.Connected += (s, e) =>
            {
                Return = e;
            };

            This.ShowDialog();

            return Return;
        }
    }
}
