using System;
using System.Windows;

using HmPro.Scripting.Files;

namespace HmPro.Windows
{
    public partial class Main
    {
        internal Component baeObj = null;
 
        public void baeExecute(Component Obj) //Loads the basic editor, and implements an object for editing.
        {
            if (Obj is Obj)
                throw new NotSupportedException("The component cannot be obj");
            baeObj = Obj;

            baeTitle.Text = Obj.Title;

            baeTitleH.Content = "Title:";
            baeTypeH.Content = "Type:";
            baeDescriptionH.Content = "Description:";
            baeCreatorH.Content = "Creator:";

            switch (Obj.Type)
            {
                case ComponentTypes.Meme: //Meme types are not allowed to be edited in this version (Comming in 2020.1). Everything else is enabled.
                {
                    Meme Meme = (Meme)Obj;
                    baeTitle.IsEnabled = true;

                    baeType.IsEnabled = false;
                    baeType.Text = Meme.ObjType == MemeTypes.Attachment ? "Attachment Meme" : Meme.ObjType == MemeTypes.Script ? "Script Meme" : "Standard Meme";

                    baeCreator.IsEnabled = true;
                    baeCreator.Text = Meme.Creator;

                    baeDescription.IsEnabled = true;
                    baeDescription.Text = Meme.Description;
                    break;
                }
                case ComponentTypes.Collection: //Collection (Standard) are allowed to have their type modified, but they do not have Creators or Descriptions in this release (Comming in 2020.1). 
                {
                    Collection Collection = (Collection)Obj;
                    baeTitle.IsEnabled = true;

                    baeType.IsEnabled = true;
                    baeType.Text = Collection.ObjType == CollectionTypes.Favorite ? "Favorite Collection" : Collection.ObjType == CollectionTypes.Legendary ? "Legendary Collection" : "Standard Collection";

                    baeCreator.IsEnabled = false;
                    baeCreator.Text = "";
                    baeCreatorH.Content = "*Not Used*";

                    baeDescription.IsEnabled = false;
                    baeDescription.Text = "";
                    baeDescriptionH.Content = "*Not Used*";
                    break;
                }
                case ComponentTypes.MasterCollection: //Master collections cannot change their type. They can only change thier title. (Conversion is comming in 2020.1)
                {
                    baeTitle.IsEnabled = true;

                    baeType.IsEnabled = false;
                    baeType.Text = "Master Collection";

                    baeCreator.IsEnabled = false;
                    baeCreator.Text = "";
                    baeCreatorH.Content = "*Not Used*";

                    baeDescription.IsEnabled = false;
                    baeDescription.Text = "";
                    baeDescriptionH.Content = "*Not Used*";
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
