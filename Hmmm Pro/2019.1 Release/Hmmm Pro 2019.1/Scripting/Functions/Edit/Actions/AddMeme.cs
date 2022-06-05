using System;
using System.Xml;
using GaePro.Scripting.Information;

namespace GaePro.Scripting.Functions.Edit.Actions
{
    public sealed class AddMeme
    {
        private XmlNode Parent { get; set; }
        private XmlDocument Action { get; set; }
        private string Path { get; set; }

        public AddMeme(XmlNode ParentSet, string ActionPath)
        {
            if (ParentSet.ChildNodes.Count != 6)
            {
                throw new NotSupportedException("The amout of items in this index is not supported for 'AddMeme'.");
            }
            Parent = ParentSet;

            XmlDocument data = new XmlDocument();
            data.Load(ActionPath);
            Action = data;
            Path = ActionPath;
        }

        public string Title
        {
            get => Parent.ChildNodes.Item(0).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(0).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }

        public MemeTypes Type
        {
            get
            {
                switch (Parent.ChildNodes.Item(1).Attributes.Item(1).Value)
                {
                    case "StandardMeme":
                        return MemeTypes.StandardMeme;
                    case "AttachmentMeme":
                        return MemeTypes.AttachmentMeme;
                    case "ScriptMeme":
                        return MemeTypes.ScriptMeme;
                }
                return MemeTypes.StandardMeme;
            }
        }
        public string StrType
        {
            get => Parent.ChildNodes.Item(1).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(1).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }

        public string Tags
        {
            get => Parent.ChildNodes.Item(2).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(2).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
        public string Creator
        {
            get => Parent.ChildNodes.Item(3).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(3).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
        public string Description
        {
            get => Parent.ChildNodes.Item(4).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(4).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
        public string MParent
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
