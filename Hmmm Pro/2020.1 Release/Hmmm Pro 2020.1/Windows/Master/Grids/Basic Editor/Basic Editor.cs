using System;
using System.Windows;

using HmPro.Files;

namespace HmPro.Windows
{
    public partial class Main
    {
        internal IComponent baeObj = null;
 
        public void baeExecute(IComponent Obj) //Loads the basic editor, and implements an object for editing.
        {
            baeObj = Obj;

            baeTitle.Text = Obj.Title;

            baeTitleH.Content = "Title:";
            baeTypeH.Content = "Type:";
            baeDescriptionH.Content = "Description:";
            baeCreatorH.Content = "Creator:";

            switch (Obj.ObjectType)
            {
                case ComponentTypes.Meme: //Meme types are not allowed to be edited in this version (Comming in 2020.1). Everything else is enabled.
                {
                    Meme Meme = (Meme)Obj;
                    baeTitle.IsEnabled = true;

                    baeType.IsEnabled = false;
                    baeType.Text = Meme.Type == MemeTypes.Attachment ? "Attachment Meme" : Meme.Type == MemeTypes.Script ? "Script Meme" : "Standard Meme";

                    baeCreator.IsEnabled = true;
                    baeCreator.Text = Meme.Creator;

                    baeDescription.IsEnabled = true;
                    baeDescription.Text = Meme.Description;
                    break;
                }
                case ComponentTypes.Collection: //Collection (Standard) are allowed to have their type modified. 
                {
                    Collection Collection = (Collection)Obj;
                    baeTitle.IsEnabled = true;

                    baeType.IsEnabled = true;
                    baeType.Text = Collection.Type == CollectionTypes.Favorite ? "Favorite Collection" : Collection.Type == CollectionTypes.Legendary ? "Legendary Collection" : Collection.Type == CollectionTypes.Master ? "Master Collection" : "Standard Collection";

                    baeCreator.IsEnabled = true;
                    baeCreator.Text = Collection.Creator;
                    baeCreatorH.Content = "Creator";

                    baeDescription.IsEnabled = true;
                    baeDescription.Text = Collection.Description;
                    baeDescriptionH.Content = "Description";
                    break;
                }
                case ComponentTypes.Script: //This can edit all three properties, in a specific order. baeTitleH is turned to 'Person', baeTypeH is turned into 'Position', and baeDescription turns to 'Text'.
                {
                    Script Script = (Script)Obj;
                    baeTitle.IsEnabled = true;
                    baeTitleH.Content = "Person:";

                    baeType.IsEnabled = true;
                    baeType.Text = Script.Position == ScriptPositions.Left ? "Left" : "Right";
                    baeTitleH.Content = "Location:";

                    baeCreator.IsEnabled = false;
                    baeCreator.Text = "";
                    baeCreatorH.Content = "*Not Used*";

                    baeDescription.IsEnabled = true;
                    baeDescription.Text = Script.Text;
                    baeDescriptionH.Content = "Text:";
                    break;
                }
            }
            LoadEditor(Editors.BasicEditor);
        }
    }
}
