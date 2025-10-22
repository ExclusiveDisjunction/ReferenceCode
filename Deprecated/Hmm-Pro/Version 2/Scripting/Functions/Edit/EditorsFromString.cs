namespace GaePro.Scripting.Functions.Edit
{
    public static class EditorFromString
    {
        public static Editors FromString(string Value)
        {
            switch (Value)
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
            }
            return Editors.None;
        }
    }
}
