using System.Collections.Generic;
using System.IO;
using System.Windows;

using Lyseria;
using HmPro.Scripting;
using HmPro.Scripting.Files;
using HmPro.Scripting.Functions.Edit;
using HmPro.Scripting.Events;
using HmPro.Windows;
using HmPro.Windows.Editing;

namespace HmPro
{
    public static class Common
    {
        public static bool Save(bool Stop = true) //Returns true if the session was saved properly, false for any errors or cancel.
        {
            if (!File.Exists(Ins.CurrentLoaded))
            {
                if (Stop)
                {
                    MessageBoxResult result = MessageBox.Show("The location does not exist anymore. Do you want to Save As instead?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Stop);
                    switch (result)
                    {
                        case MessageBoxResult.Cancel:
                            return false;
                        case MessageBoxResult.None:
                            return false;
                        case MessageBoxResult.No:
                            return false;
                        case MessageBoxResult.Yes:
                            return SaveAs();
                    }
                }
                else
                {
                    return false;
                }
            }
            if (!File.Exists(Ins.CurrentSession))
            {
                if (Stop)
                {
                    MessageBox.Show("The session cannot be saved, as the session no longer exists. This cannot be fixed (Was it moved or deleted?). Sorry for the problem.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                return false;
            }

            File.Delete(Ins.CurrentLoaded);
            File.Copy(Ins.CurrentSession, Ins.CurrentLoaded);
            Ins.LastLoaded = Ins.CurrentLoaded;

            Session Sess = new Session();
            App.ThrowSaved(Sess);

            return true;
        }
        public static bool SaveAs() //Returns true for proper saving, false for cancel or errors.
        {
            Microsoft.Win32.SaveFileDialog open = new Microsoft.Win32.SaveFileDialog()
            {
                AddExtension = true,
                DefaultExt = ".teh",
                Filter = "Hmmm Pro Sessions | *.teh",
                RestoreDirectory = true,
                Title = "Select a location for saving the session."
            };

            open.ShowDialog();

            string path = open.FileName;

            if (path == "")
            {
                return false;
            }

            if (File.Exists(path))
            {
                File.Delete(path);
            }
            if (!File.Exists(Ins.CurrentSession))
            {
                MessageBox.Show("The session cannot be saved, as the session no longer exists. This cannot be fixed (Was it moved or deleted?). Sorry for the problem.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            File.Copy(Ins.CurrentSession, path);
            Ins.CurrentLoaded = path;

            App.MainIns.OpenFromPath(path);
            return true;
        }

        public static bool Append(Actions Action) //Returns true if the specified action was appened, false for if it is a non-appendable type (such as CurrentFile), an error occurs, it gets canceled for if the action = none.
        {
            Session File = new Session();
            Main Main = App.MainIns;

            Editors Open = Editors.None;
            switch (Action)
            {
                case Actions.Action1:
                    Open = EditorReg.Action1;
                    break;
                case Actions.Action2:
                    Open = EditorReg.Action2;
                    break;
                case Actions.Action3:
                    Open = EditorReg.Action3;
                    break;
                case Actions.Action4:
                    Open = EditorReg.Action4;
                    break;
                case Actions.Action5:
                    Open = EditorReg.Action5;
                    break;
                case Actions.Action6:
                    Open = EditorReg.Action6;
                    break;
                case Actions.Action7:
                    Open = EditorReg.Action7;
                    break;
                case Actions.None:
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
            }

            switch (Open)
            {
                case Editors.AddCollection:
                {
                    CollectionTypes Type = CollectionTypes.Standard;
                    switch (Main.adcType.Text)
                    {
                        case "Standard":
                            Type = CollectionTypes.Standard;
                            break;
                        case "Favorite":
                            Type = CollectionTypes.Favorite;
                            break;
                        case "Legendary":
                            Type = CollectionTypes.Legendary;
                            break;
                        case "Master":
                            Type = CollectionTypes.Master;
                            break;
                    }

                    if (Main.adcTitle.Text == "")
                    {
                        MessageBox.Show("The collection cannot be appended, as title cannot be blank.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return false;
                    }

                    if (Main.adcType.Text == "")
                    {
                        Main.adcType.Text = "Standard";
                    }
                    if (Main.adcLocation.Text == "")
                    {
                        Main.adcLocation.Text = "Session";
                    }

                    Retry:  
                    switch (Main.adcLocation.Text)
                    {
                        case "Session":
                        {
                            CollectionBase New;
                            if (Type == CollectionTypes.Master)
                            {
                                New = File.MasterCollections.Add();
                            }
                            else
                            {
                                New = File.Collections.Add(Type);                                
                            }

                            if (New == null)
                            {
                                MessageBox.Show("The collection given is currently in the session, and duplicates are not allowed.", "Add Collection:", MessageBoxButton.OK, MessageBoxImage.Error);
                                return false;
                            }
                            New.Title = Main.adcTitle.Text;
                            break;
                        }
                        case "<Select Master Collection>":
                        {
                            if (Type == CollectionTypes.Master)
                            {
                                MessageBoxResult Result = MessageBox.Show("A master collection cannot be appended to another collection.\nDo you want to add this to session?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                                switch (Result)
                                {
                                    case MessageBoxResult.Cancel | MessageBoxResult.None | MessageBoxResult.No:
                                        MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                        return false;
                                    case MessageBoxResult.Yes:
                                        Main.adcLocation.Text = "Session";
                                        goto Retry;
                                }
                            }
                            ConnectEventArgs args = new ObjectSelector().Execute(Memes:false, Collections:false, MasterCollections:true, Scripts:false);
                            if (!args.Connected || args.Object == null)
                            {
                                MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                Main.ResetEditor(Editors.AddCollection);
                                return false;
                            }

                            Collection New = ((MasterCollection)args.Object).Collections.Add(Type);
                            New.Title = Main.adcTitle.Text;

                            Main.ResetEditor(Editors.AddCollection);
                            break;
                        }
                    }

                    break;
                }
                case Editors.AddMeme:
                {
                    MemeTypes Type = MemeTypes.Standard;
                    switch (Main.admType.Text)
                    {
                        case "Standard":
                            Type = MemeTypes.Standard;
                            break;
                        case "Attachment":
                            Type = MemeTypes.Attachment;
                            break;
                        case "Script":
                            Type = MemeTypes.Script;
                            break;
                    }

                    if (Main.admTitle.Text == "")
                    {
                        MessageBox.Show("The meme cannot be appended, as title cannot be blank.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return false;
                    }

                    if (Main.admType.Text == "") //This fixes any parameters that have not been ommited.
                    {
                        Main.admType.Text = "Standard";
                    }
                    if (Main.admLocation.Text == "")
                    {
                        Main.admLocation.Text = "<Select Collection>";
                    }

                    /*
                     * The location is special. It is decided by ConnectedEventArgs args (defined later), and setup in a special way.
                     *  1. From Collection & Object associated; (true, CollectionBase)
                     *  2. From Favorite Memes & null; (true, null)
                     *  3. From Legendary Memes & null. (true, null)
                     *  
                     *  when Collection ommits true, then the object is added under the component's memes. Component must be CollectionBase.
                     *  when Favortie Memes ommits true, then it adds the object to the Session.FavoriteMemes list.
                     *  when Legendary Memes ommits true, then it adds the object to the Session.LegendaryMemes list.
                     */
                    DoubleList<bool, Component> Location = new DoubleList<bool, Component>();
                    if (Main.admLocation.Text == "<Select Collection>")
                    {
                        ConnectEventArgs args = new ObjectSelector().Execute(Memes:false, Collections:true, MasterCollections:true, Scripts:false);
                        if (!args.Connected)
                        {
                            MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                            Main.ResetEditor(Editors.AddMeme);
                            return false;
                        }
                        if (args.Object == null)
                        {
                            switch (args.Notes)
                            {
                                case "Favorite_Memes":
                                    Location.Add(false, null);
                                    Location.Add(true, null);
                                    Location.Add(false, null);
                                    break;
                                case "Legendary_Memes":
                                    Location.Add(false, null);
                                    Location.Add(false, null);
                                    Location.Add(true, null);
                                    break;
                            }
                        }
                        else
                        {
                            Location.Add(true, args.Object);
                            Location.Add(false, null);
                            Location.Add(false, null);
                        }
                    }
                    else if (Main.admLocation.Text == "Favorite Memes")
                    {
                        Location.Add(false, null);
                        Location.Add(true, null);
                        Location.Add(false, null);
                    }
                    else if (Main.admLocation.Text == "Legendary Memes")
                    {
                        Location.Add(false, null);
                        Location.Add(false, null);
                        Location.Add(true, null);
                    }

                    List<bool> Options = Location.Ones;
                    List<Component> Components = Location.Twos;
                    Meme meme = null;

                    if (Options[0] == true && Options[1] == false && Options[2] == false)
                    {
                        if (Components[0] == null || !(Components[0] is CollectionBase))
                        {
                            MessageBox.Show("There was an unresolvable internal error. Sorry for the inconvience.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                            return false;
                        }
                        if (Components[0].Type == ComponentTypes.MasterCollection)
                        {
                            meme = ((MasterCollection)Components[0]).Memes.Add(Type);
                        }
                        else
                        {
                            meme = ((Collection)Components[0]).Memes.Add(Type);
                        }
                    }
                    else if (Options[0] == false && Options[1] == true && Options[2] == false)
                    {
                        meme = File.FavoriteMemes.Add(Type);
                    }
                    else if (Options[0] == false && Options[1] == false && Options[2] == true)
                    {
                        meme = File.LegendaryMemes.Add(Type);
                    }
                    
                    if (meme == null)
                    {
                        MessageBox.Show("The meme could not be appended, the location was inconclusive.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                        return false;
                    }

                    meme.Title = Main.admTitle.Text;
                    meme.Creator = Main.admCreator.Text;

                    if (meme.ObjType == MemeTypes.Attachment) //Adds the attachments to the meme if the meme is Attachment.
                    {
                        meme.Attachments[0] = Main.admAtt1.Text != "" && System.IO.File.Exists(Main.admAtt1.Text) ? Main.admAtt1.Text : "";
                        meme.Attachments[1] = Main.admAtt2.Text != "" && System.IO.File.Exists(Main.admAtt2.Text) ? Main.admAtt2.Text : "";
                        meme.Attachments[2] = Main.admAtt3.Text != "" && System.IO.File.Exists(Main.admAtt3.Text) ? Main.admAtt3.Text : "";
                        meme.Attachments[3] = Main.admAtt4.Text != "" && System.IO.File.Exists(Main.admAtt4.Text) ? Main.admAtt4.Text : "";
                        meme.Attachments[4] = Main.admAtt5.Text != "" && System.IO.File.Exists(Main.admAtt5.Text) ? Main.admAtt5.Text : "";
                    }

                    Main.ResetEditor(Open);
                    break;
                }
                case Editors.AllObjects: //This is a view only thing, with pointers to other editors, such as 'Manage Memes' and 'Add Meme'. This does not contain information.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.BasicEditor:
                {
                    Component Obj = Main.baeObj;

                    switch (Obj.Type)
                    {
                        case ComponentTypes.Collection:
                        {
                            CollectionBase Collection = (CollectionBase)Obj;

                            if (Collection is MasterCollection)
                            {
                                Main.baeType.Text = "Master Collection";
                            }
                            string type = Main.baeType.Text.ToLower();

                            if (Main.baeTitle.Text == "" || (Collection is MasterCollection && type != "master collection") || (Collection is Collection && type != "standard collection" && type != "favorite collection" && type != "legendary collection"))
                            {
                                MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & a correct type.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                return false;
                            }

                            Collection.Title = Main.baeTitle.Text;
                            if (Collection is Collection nCollection)
                                nCollection.ObjType = (type == "favorite collection") ? CollectionTypes.Favorite : (type == "legendary collection") ? CollectionTypes.Legendary : CollectionTypes.Standard;
                            break;
                        }
                        case ComponentTypes.MasterCollection:
                        {
                            CollectionBase Collection = (CollectionBase)Obj;

                            if (Collection is MasterCollection)
                            {
                                Main.baeType.Text = "Master Collection";
                            }
                            string type = Main.baeType.Text.ToLower();

                            if (Main.baeTitle.Text == "" || (Collection is MasterCollection && type != "master collection") || (Collection is Collection && type != "standard collection" && type != "favorite collection" && type != "legendary collection"))
                            {
                                MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & a correct type.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                return false;
                            }

                            Collection.Title = Main.baeTitle.Text;
                            if (Collection is Collection nCollection)
                                nCollection.ObjType = (type == "favorite collection") ? CollectionTypes.Favorite : (type == "legendary collection") ? CollectionTypes.Legendary : CollectionTypes.Standard;
                            break;
                        }
                        case ComponentTypes.Meme:
                        {
                            Meme Meme = (Meme)Obj;

                            string title = Main.baeTitle.Text;
                            string type = Main.baeType.Text.ToLower();

                            if (title == "" || (type != "standard meme" && type != "script meme" && type != "attachment meme"))
                            {
                                MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & a correct type.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                return false;
                            }

                            Meme.Title = title;
                            Meme.Creator = Main.baeCreator.Text;
                            Meme.Description = Main.baeDescription.Text;
                            break;
                        }
                        case ComponentTypes.Script:
                        {
                            Script Script = (Script)Obj;

                            string title = Main.baeTitle.Text;
                            string type = Main.baeType.Text.ToLower();

                            if (title == "" || (type != "right" && type != "left"))
                            {
                                MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & make sure type is either 'Left' or 'Right'.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                            }

                            Script.Person = title;
                            Script.Position = type == "left" ? ScriptPositions.Left : ScriptPositions.Right;
                            Script.Text = Main.baeDescription.Text;
                            break;
                        }
                    }

                    Main.CloseEditor(EditorReg.AssociatedAction(Editors.BasicEditor));
                    break;
                }
                case Editors.CurrentFile: //There is nothing to append here.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.ManageCollections: //Nothing to append.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.ManageMemes: //Nothing to append.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.None: //There is nothing to append here, as it is nothing.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return false;
                }
                case Editors.ReportBug: //Nothing to append.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    break;
                }
                case Editors.Settings: //There is nothing to append here, information is appened as it is modifyied.
                {
                    MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                    break;
                }
            }

            App.ThrowAppended(File, true);
            MessageBox.Show("Append Complete.", "Append Action:", MessageBoxButton.OK, MessageBoxImage.Information);
            return true;
        }
        public static bool AppendAll() //Appends all items. Returns true if all of the editors passed and appended, false for errors or canceling.
        {
            Session File = new Session();
            Main Main = App.MainIns;

            EditorReg editors = new EditorReg();
            IEnumerable<Editors> Open = editors.GetOpenEditors();

            List<Editors> OpenT = new List<Editors>(Open);
            if (OpenT.Count == 0)
            {
                MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
            }

            foreach (Editors Current in Open)
            {
                switch (Current)
                {
                    case Editors.AddCollection:
                    {
                        CollectionTypes Type = CollectionTypes.Standard;
                        switch (Main.adcType.Text)
                        {
                            case "Standard":
                                Type = CollectionTypes.Standard;
                                break;
                            case "Favorite":
                                Type = CollectionTypes.Favorite;
                                break;
                            case "Legendary":
                                Type = CollectionTypes.Legendary;
                                break;
                            case "Master":
                                Type = CollectionTypes.Master;
                                break;
                        }

                        if (Main.adcTitle.Text == "")
                        {
                            MessageBox.Show("The collection cannot be appended, as title cannot be blank.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                            continue;
                        }

                        if (Main.adcType.Text == "")
                        {
                            Main.adcType.Text = "Standard";
                        }
                        if (Main.adcLocation.Text == "")
                        {
                            Main.adcLocation.Text = "Session";
                        }

                        Retry:
                        switch (Main.adcLocation.Text)
                        {
                            case "Session":
                            {
                                CollectionBase New;
                                if (Type == CollectionTypes.Master)
                                {
                                    New = File.MasterCollections.Add();
                                }
                                else
                                {
                                    New = File.Collections.Add(Type);
                                }

                                if (New == null)
                                {
                                    MessageBox.Show("The collection given is currently in the session, and duplicates are not allowed.", "Add Collection:", MessageBoxButton.OK, MessageBoxImage.Error);
                                    continue;
                                }
                                New.Title = Main.adcTitle.Text;
                                break;
                            }
                            case "<Select Master Collection>":
                            {
                                if (Type == CollectionTypes.Master)
                                {
                                    MessageBoxResult Result = MessageBox.Show("A master collection cannot be appended to another collection.\nDo you want to add this to session?", "Hmmm Pro:", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                                    switch (Result)
                                    {
                                        case MessageBoxResult.Cancel | MessageBoxResult.None | MessageBoxResult.No:
                                            MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                            continue;
                                        case MessageBoxResult.Yes:
                                            Main.adcLocation.Text = "Session";
                                            goto Retry;
                                    }
                                }
                                ConnectEventArgs args = new ObjectSelector().Execute(Memes:false, Collections:false, MasterCollections:true, Scripts:false);
                                if (!args.Connected || args.Object == null)
                                {
                                    MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                    Main.ResetEditor(Editors.AddCollection);
                                    continue;
                                }

                                Collection New = ((MasterCollection)args.Object).Collections.Add(Type);
                                New.Title = Main.adcTitle.Text;

                                Main.ResetEditor(Editors.AddCollection);
                                break;
                            }
                        }

                        break;
                    }
                    case Editors.AddMeme:
                    {
                        MemeTypes Type = MemeTypes.Standard;
                        switch (Main.admType.Text)
                        {
                            case "Standard":
                                Type = MemeTypes.Standard;
                                break;
                            case "Attachment":
                                Type = MemeTypes.Attachment;
                                break;
                            case "Script":
                                Type = MemeTypes.Script;
                                break;
                        }

                        if (Main.admTitle.Text == "")
                        {
                            MessageBox.Show("The meme cannot be appended, as title cannot be blank.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                            continue;
                        }

                        if (Main.admType.Text == "") //This fixes any parameters that have not been ommited.
                        {
                            Main.admType.Text = "Standard";
                        }
                        if (Main.admLocation.Text == "")
                        {
                            Main.admLocation.Text = "<Select Collection>";
                        }

                        /*
                         * The location is special. It is decided by ConnectedEventArgs args (defined later), and setup in a special way.
                         *  1. From Collection & Object associated; (true, CollectionBase)
                         *  2. From Favorite Memes & null; (true, null)
                         *  3. From Legendary Memes & null. (true, null)
                         *  
                         *  when Collection ommits true, then the object is added under the component's memes. Component must be CollectionBase.
                         *  when Favortie Memes ommits true, then it adds the object to the Session.FavoriteMemes list.
                         *  when Legendary Memes ommits true, then it adds the object to the Session.LegendaryMemes list.
                         */
                        DoubleList<bool, Component> Location = new DoubleList<bool, Component>();
                        if (Main.admLocation.Text == "<Select Collection>")
                        {
                            ConnectEventArgs args = new ObjectSelector().Execute(Memes:false, Collections:true, MasterCollections:true, Scripts:false);
                            if (!args.Connected)
                            {
                                MessageBox.Show("Job canceled.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Information);
                                Main.ResetEditor(Editors.AddMeme);
                                continue;
                            }
                            if (args.Object == null)
                            {
                                switch (args.Notes)
                                {
                                    case "Favorite_Memes":
                                        Location.Add(false, null);
                                        Location.Add(true, null);
                                        Location.Add(false, null);
                                        break;
                                    case "Legendary_Memes":
                                        Location.Add(false, null);
                                        Location.Add(false, null);
                                        Location.Add(true, null);
                                        break;
                                }
                            }
                            else
                            {
                                Location.Add(true, args.Object);
                                Location.Add(false, null);
                                Location.Add(false, null);
                            }
                        }
                        else if (Main.admLocation.Text == "Favorite Memes")
                        {
                            Location.Add(false, null);
                            Location.Add(true, null);
                            Location.Add(false, null);
                        }
                        else if (Main.admLocation.Text == "Legendary Memes")
                        {
                            Location.Add(false, null);
                            Location.Add(false, null);
                            Location.Add(true, null);
                        }

                        List<bool> Options = Location.Ones;
                        List<Component> Components = Location.Twos;
                        Meme meme = null;

                        if (Options[0] == true && Options[1] == false && Options[2] == false)
                        {
                            if (Components[0] == null || !(Components[0] is CollectionBase))
                            {
                                MessageBox.Show("There was an unresolvable internal error. Sorry for the inconvience.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                                continue;
                            }
                            if (Components[0].Type == ComponentTypes.MasterCollection)
                            {
                                meme = ((MasterCollection)Components[0]).Memes.Add(Type);
                            }
                            else
                            {
                                meme = ((Collection)Components[0]).Memes.Add(Type);
                            }
                        }
                        else if (Options[0] == false && Options[1] == true && Options[2] == false)
                        {
                            meme = File.FavoriteMemes.Add(Type);
                        }
                        else if (Options[0] == false && Options[1] == false && Options[2] == true)
                        {
                            meme = File.LegendaryMemes.Add(Type);
                        }

                        if (meme == null)
                        {
                            MessageBox.Show("The meme could not be appended, the location was inconclusive.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Error);
                            continue;
                        }

                        meme.Title = Main.admTitle.Text;
                        meme.Creator = Main.admCreator.Text;

                        if (meme.ObjType == MemeTypes.Attachment) //Adds the attachments to the meme if the meme is Attachment.
                        {
                            meme.Attachments[0] = Main.admAtt1.Text;
                            meme.Attachments[1] = Main.admAtt2.Text;
                            meme.Attachments[2] = Main.admAtt3.Text;
                            meme.Attachments[3] = Main.admAtt4.Text;
                            meme.Attachments[4] = Main.admAtt5.Text;
                        }

                        Main.ResetEditor(Editors.AddMeme);
                        break;
                    }
                    case Editors.AllObjects: //This is a view only thing, with pointers to other editors, such as 'Manage Memes' and 'Add Meme'. This does not contain information.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        continue;
                    }
                    case Editors.BasicEditor:
                    {
                        Component Obj = Main.baeObj;

                        switch (Obj.Type)
                        {
                            case ComponentTypes.Collection | ComponentTypes.MasterCollection:
                            {
                                CollectionBase Collection = (CollectionBase)Obj;

                                if (Collection is MasterCollection)
                                {
                                    Main.baeType.Text = "Master Collection";
                                }
                                string type = Main.baeType.Text.ToLower();

                                if (Main.baeTitle.Text == "" || (Collection is MasterCollection && type != "master collection") || (Collection is Collection && type != "standard collection" && type != "favorite collection" && type != "legendary collection"))
                                {
                                    MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & a correct type.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                    continue;
                                }

                                Collection.Title = Main.baeTitle.Text;
                                if (Collection is Collection nCollection)
                                    nCollection.ObjType = (type == "favorite collection") ? CollectionTypes.Favorite : (type == "legendary collection") ? CollectionTypes.Legendary : CollectionTypes.Standard;
                                break;
                            }
                            case ComponentTypes.Meme:
                            {
                                Meme Meme = (Meme)Obj;

                                string title = Main.baeTitle.Text;
                                string type = Main.baeType.Text.ToLower();

                                if (title == "" || (type != "standard meme" && type != "script meme" && type != "attachment meme"))
                                {
                                    MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & a correct type.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                    continue;
                                }

                                Meme.Title = title;
                                Meme.Creator = Main.baeCreator.Text;
                                Meme.Description = Main.baeDescription.Text;
                                break;
                            }
                            case ComponentTypes.Script:
                            {
                                Script Script = (Script)Obj;

                                string title = Main.baeTitle.Text;
                                string type = Main.baeType.Text.ToLower();

                                if (title == "" || (type != "right" && type != "left"))
                                {
                                    MessageBox.Show("The item could not be modified.\nEnsure that the object has a title & make sure type is either 'Left' or 'Right'.", "Append:", MessageBoxButton.OK, MessageBoxImage.Warning);
                                }

                                Script.Person = title;
                                Script.Position = type == "left" ? ScriptPositions.Left : ScriptPositions.Right;
                                Script.Text = Main.baeDescription.Text;
                                break;
                            }
                        }

                        Main.ResetEditor(Editors.BasicEditor);
                        break;
                    }
                    case Editors.CurrentFile: //There is nothing to append here.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        continue;
                    }
                    case Editors.ManageCollections: //Nothing to append.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        continue;
                    }
                    case Editors.ManageMemes: //Nothing to append.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        continue;
                    }
                    case Editors.None: //There is nothing to append here, as it is nothing.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        continue;
                    }
                    case Editors.ReportBug: //Nothing to append.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;
                    }
                    case Editors.Settings: //There is nothing to append here, information is appened as it is modifyied.
                    {
                        MessageBox.Show("There is nothing to append.", "Hmmm Pro:", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;
                    }
                }
            }

            App.ThrowAppended(File, true);
            MessageBox.Show("Append All Complete.", "Append Action:", MessageBoxButton.OK, MessageBoxImage.Information);
            return true;
        }
    }
}
