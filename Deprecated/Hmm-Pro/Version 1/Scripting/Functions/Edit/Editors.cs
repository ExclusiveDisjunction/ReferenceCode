using System.Xml;
using System.Collections.Generic;
using System.Collections;
using Lyseria;

namespace HmPro.Scripting.Functions.Edit
{
    public class EditorReg : IEnumerable<Editors>
    {
        public static bool EditorRunning //Determines if any editor is running.
        {
            get
            {
                return (Action1 != Editors.None) || 
                    (Action2 != Editors.None) || 
                    (Action3 != Editors.None) || 
                    (Action4 != Editors.None) || 
                    (Action5 != Editors.None) || 
                    (Action6 != Editors.None) || 
                    (Action7 != Editors.None);
            }
        }
        public static bool AppendableEditorRunning //Determines if any appendable editor is running
        {
            get
            {
                return (Action1 == Editors.AddCollection || Action1 == Editors.AddMeme || Action1 == Editors.BasicEditor) ||
                    (Action2 == Editors.AddCollection || Action2 == Editors.AddMeme || Action2 == Editors.BasicEditor) ||
                    (Action3 == Editors.AddCollection || Action3 == Editors.AddMeme || Action3 == Editors.BasicEditor) ||
                    (Action4 == Editors.AddCollection || Action4 == Editors.AddMeme || Action4 == Editors.BasicEditor) ||
                    (Action5 == Editors.AddCollection || Action5 == Editors.AddMeme || Action5 == Editors.BasicEditor) ||
                    (Action6 == Editors.AddCollection || Action6 == Editors.AddMeme || Action6 == Editors.BasicEditor) ||
                    (Action7 == Editors.AddCollection || Action7 == Editors.AddMeme || Action7 == Editors.BasicEditor);
            }
        }
        public static bool SpecEditorRuning(Editors Editor) //Determines if a single editor is running.
        {
            return Action1 == Editor || Action2 == Editor || Action3 == Editor || Action4 == Editor || Action5 == Editor || Action6 == Editor || Action7 == Editor;
        }
        public static bool ActionRunning(Actions Action)
        {
            switch (Action)
            {
                case Actions.Action1:
                    if (Action1 != Editors.None)
                    {
                        return true;
                    }
                    break;
                case Actions.Action2:
                    if (Action2 != Editors.None)
                    {
                        return true;
                    }
                    break;
                case Actions.Action3:
                    if (Action3 != Editors.None)
                    {
                        return true;
                    }
                    break;
                case Actions.Action4:
                    if (Action4 != Editors.None)
                    {
                        return true;
                    }
                    break;
                case Actions.Action5:
                    if (Action5 != Editors.None)
                    {
                        return true;
                    }
                    break;
                case Actions.Action6:
                    if (Action6 != Editors.None)
                    {
                        return true;
                    }
                    break;
                case Actions.Action7:
                    if (Action7 != Editors.None)
                    {
                        return true;
                    }
                    break;
            }
            return false;
        }

        public static Actions LastEmpty //Determines the last empty action in the list, as in the first editor that is equal to Editors.None.
        {
            get
            {
                if (Action1 == Editors.None)
                {
                    return Actions.Action1;
                }
                else if (Action2 == Editors.None)
                {
                    return Actions.Action2;
                }
                else if (Action3 == Editors.None)
                {
                    return Actions.Action3;
                }
                else if (Action4 == Editors.None)
                {
                    return Actions.Action4;
                }
                else if (Action5 == Editors.None)
                {
                    return Actions.Action5;
                }
                else if (Action6 == Editors.None)
                {
                    return Actions.Action6;
                }
                else if (Action7 == Editors.None)
                {
                    return Actions.Action7;
                }
                else
                {
                    return Actions.None;
                }
            }
        }              
        public static Editors Assign(Actions OrigItem, Editors To) //Sets 'OrigItem's value to the value in 'To'.
        {
            switch (OrigItem)
            {
                case Actions.Action1:
                    Action1 = To;
                    break;
                case Actions.Action2:
                    Action2 = To;
                    break;
                case Actions.Action3:
                    Action3 = To;
                    break;
                case Actions.Action4:
                    Action4 = To;
                    break;
                case Actions.Action5:
                    Action5 = To;
                    break;
                case Actions.Action6:
                    Action6 = To;
                    break;
                case Actions.Action7:
                    Action7 = To;
                    break;
                case Actions.None:
                    return Editors.None;
            }
            return To;
        }
        public static void Assign(Editors QueryItem, Editors To) //Sets the value of an action in the class by finding the Actions of QueryItem.
        {
            switch (AssociatedAction(QueryItem))
            {
                case Actions.Action1:
                    Action1 = To;
                    break;
                case Actions.Action2:
                    Action2 = To;
                    break;
                case Actions.Action3:
                    Action3 = To;
                    break;
                case Actions.Action4:
                    Action4 = To;
                    break;
                case Actions.Action5:
                    Action5 = To;
                    break;
                case Actions.Action6:
                    Action6 = To;
                    break;
                case Actions.Action7:
                    Action7 = To;
                    break;
                case Actions.None:
                    return;
            }
        }

        public static Actions AssociatedAction(Editors Editor) //Returns the action associated with a running editor. If the editor is not running, it returns Actions.None
        {
            if (Action1 == Editor)
            {
                return Actions.Action1;
            }
            if (Action2 == Editor)
            {
                return Actions.Action2;
            }
            if (Action3 == Editor)
            {
                return Actions.Action3;
            }
            if (Action4 == Editor)
            {
                return Actions.Action4;
            }
            if (Action5 == Editor)
            {
                return Actions.Action5;
            }
            if (Action6 == Editor)
            {
                return Actions.Action6;
            }
            if (Action7 == Editor)
            {
                return Actions.Action7;
            }
            return Actions.None;
        }
        public static Editors AssociatedEditor(Actions Action) //Returns the editor associated with an action. If the action is Actions.None, then it returns Editors.None.
        {
            switch (Action)
            {
                case Actions.Action1:
                    return Action1;
                case Actions.Action2:
                    return Action2;
                case Actions.Action3:
                    return Action3;
                case Actions.Action4:
                    return Action4;
                case Actions.Action5:
                    return Action5;
                case Actions.Action6:
                    return Action6;
                case Actions.Action7:
                    return Action7;
                default:
                    return Editors.None;
            }
        }

        public static Editors Action1 //The running editor in Action1.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(1).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(1).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        } 
        public static Editors Action2 //The running editor in Action2.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(2).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(2).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        }
        public static Editors Action3 //The running editor in Action3.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(3).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(3).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        }
        public static Editors Action4 //The running editor in Action4.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(4).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(4).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        }
        public static Editors Action5 //The running editor in Action5.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(5).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(5).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        }
        public static Editors Action6 //The running editor in Action6.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(6).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(6).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        }
        public static Editors Action7 //The running editor in Action7.
        {
            get
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                switch (data.DocumentElement.Attributes.Item(7).Value)
                {
                    case "CurrentFile":
                        return Editors.CurrentFile;
                    case "AddMeme":
                        return Editors.AddMeme;
                    case "AddCollection":
                        return Editors.AddCollection;
                    case "ManageMemes":
                        return Editors.ManageMemes;
                    case "ManageCollections":
                        return Editors.ManageCollections;
                    case "AllObjects":
                        return Editors.AllObjects;
                    case "BasicEditor":
                        return Editors.BasicEditor;
                    case "Settings":
                        return Editors.Settings;
                    case "ReportBug":
                        return Editors.ReportBug;
                    default:
                        return Editors.None;
                }
            }
            private set
            {
                XmlDocument data = new XmlDocument();
                data.Load("instance\\editor.tehs");

                data.DocumentElement.Attributes.Item(7).Value = $"{value}";

                data.Save("instance\\editor.tehs");
            }
        }

        public static void Close() //Resets all actions for the next time that the application is loaded, to stop the app from thinking that there is an editor running.
        {
            Action1 = Editors.None;
            Action2 = Editors.None;
            Action3 = Editors.None;
            Action4 = Editors.None;
            Action5 = Editors.None;
            Action6 = Editors.None;
            Action7 = Editors.None;
        }
        public static void CloseItem(Actions Action) //Sets a specific action to Editors.None.
        {
            switch (Action)
            {
                case Actions.Action1:
                    Action1 = Editors.None;
                    break;
                case Actions.Action2:
                    Action2 = Editors.None;
                    break;
                case Actions.Action3:
                    Action3 = Editors.None;
                    break;
                case Actions.Action4:
                    Action4 = Editors.None;
                    break;
                case Actions.Action5:
                    Action5 = Editors.None;
                    break;
                case Actions.Action6:
                    Action6 = Editors.None;
                    break;
                case Actions.Action7:
                    Action7 = Editors.None;
                    break;
                case Actions.None:
                    return;
            }
        }
        public static void CloseItem(Editors Editor) //Sets a specific action to Editors.None from the assocated action of 'Editor'.
        {
            switch (AssociatedAction(Editor))
            {
                case Actions.Action1:
                    Action1 = Editors.None;
                    break;
                case Actions.Action2:
                    Action2 = Editors.None;
                    break;
                case Actions.Action3:
                    Action3 = Editors.None;
                    break;
                case Actions.Action4:
                    Action4 = Editors.None;
                    break;
                case Actions.Action5:
                    Action5 = Editors.None;
                    break;
                case Actions.Action6:
                    Action6 = Editors.None;
                    break;
                case Actions.Action7:
                    Action7 = Editors.None;
                    break;
                case Actions.None:
                    return;
            }
        }

        public IEnumerator<Editors> GetEnumerator() //Allows illiteration of all 7 actions.
        {
            List<Editors> Value = new List<Editors>
            {
                Action1, Action2, Action3, Action4, Action5, Action6, Action7
            };

            return Value.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            List<Editors> Value = new List<Editors>
            {
                Action1, Action2, Action3, Action4, Action5, Action6, Action7
            };

            return Value.GetEnumerator();
        }

        public IEnumerable<Editors> GetOpenEditors() //Returns an array of editors that is only contains editors that are appendable.
        {
            List<Editors> editors = new List<Editors>();

            foreach (Editors Item in this)
            {
                if (Item != Editors.AddCollection && Item != Editors.AddMeme && Item != Editors.BasicEditor)
                    editors.Add(Item);
            }

            return editors;
        }
    }
}   

