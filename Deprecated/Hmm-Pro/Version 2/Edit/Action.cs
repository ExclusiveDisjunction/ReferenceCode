using System.Xml;

using GaePro.Scripting.Functions.Edit.Actions;

namespace GaePro.Scripting.Functions.Edit
{
    public sealed class Action
    {
        public string Path
        {
            get;
            set;
        }
        public XmlDocument Document { get; set; }

        public Action(string Path)
        {
            this.Path = Path;

            XmlDocument data = new XmlDocument();
            data.Load(Path);
            Document = data;
        }

        public Editors CurrentEditor
        {
            get => EditorFromString.FromString(Document.DocumentElement.Attributes.Item(1).Value);
            set
            {
                Document.DocumentElement.Attributes.Item(1).Value = value.ToString();
                Document.Save(Path);
            }
        }

        public AddMeme AddMemeValue
        {
            get => new AddMeme(Document.DocumentElement.ChildNodes.Item(0), Path);
            set
            {
                AddMeme New = new AddMeme(Document.DocumentElement.ChildNodes.Item(0), Path)
                {
                    Creator = value.Creator,
                    Description = value.Description,
                    MParent = value.MParent,
                    StrType = value.StrType,
                    Tags = value.Tags,
                    Title = value.Title
                };
                AddMemeValue = New;
            }
        }
        public AddCollection AddCollectionValue
        {
            get => new AddCollection(Document.DocumentElement.ChildNodes.Item(1), Path);
            set
            {
                AddCollection New = new AddCollection(Document.DocumentElement.ChildNodes.Item(1), Path)
                {
                    Title = value.Title,
                    IParent = value.IParent,
                    StrType = value.StrType
                };
                AddCollectionValue = New;
            }
        }
        public BasicEditor BasicEditorValue
        {
            get => new BasicEditor(Document.DocumentElement.ChildNodes.Item(2), Path);
            set
            {
                BasicEditor New = new BasicEditor(Document.DocumentElement.ChildNodes.Item(2), Path)
                {
                    Title = value.Title,
                    IParent = value.IParent,
                    ItemName = value.ItemName,
                    SessionPath = value.SessionPath,
                    Type = value.Type
                };
                BasicEditorValue = New;
            }
        }

        public void Reset()
        {
            AddMemeValue.Creator = "";
            AddMemeValue.Description = "";
            AddMemeValue.MParent = "";
            AddMemeValue.StrType = "";
            AddMemeValue.Tags = "";
            AddMemeValue.Title = "";
            
            AddCollectionValue.IParent = "";
            AddCollectionValue.StrType = "";
            AddCollectionValue.Title = "";

            BasicEditorValue.IParent = "";
            BasicEditorValue.ItemName = "";
            BasicEditorValue.SessionPath = "";
            BasicEditorValue.Title = "";
            BasicEditorValue.Type = "";

            CurrentEditor = Editors.None;
        }
    }
}
