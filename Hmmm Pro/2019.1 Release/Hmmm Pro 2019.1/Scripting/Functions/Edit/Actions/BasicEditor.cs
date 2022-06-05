using System.Xml;

using GaePro.Scripting.Information;

namespace GaePro.Scripting.Functions.Edit.Actions
{
    public sealed class BasicEditor
    {
        private XmlNode Parent { get; set; }
        private XmlDocument Action { get; set; }
        private string Path { get; set; }

        public BasicEditor(XmlNode ParentSet, string Path)
        {
            Parent = ParentSet;

            XmlDocument data = new XmlDocument();
            data.Load(Path);
            Action = data;
            this.Path = Path;
        }

        public string Title
        {
            get => Parent.ChildNodes.Item(1).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(1).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
        public string Type
        {
            get => Parent.ChildNodes.Item(2).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(2).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
        public string IParent
        {
            get => Parent.ChildNodes.Item(3).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(3).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }

        public string ItemName
        {
            get => Parent.ChildNodes.Item(0).Attributes.Item(0).Value;
            set
            {
                Parent.ChildNodes.Item(0).Attributes.Item(0).Value = value;
                Action.Save(Path);
            }
        }

        public Session Session
        {
            get => new Session(Parent.ChildNodes.Item(0).Attributes.Item(1).Value);
        }
        public string SessionPath
        {
            get => Parent.ChildNodes.Item(0).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(0).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
    }
}
