using System;
using System.Xml;

using GaePro.Scripting.Information;

namespace GaePro.Scripting.Functions.Edit.Actions
{
    public sealed class AddCollection
    {
        private XmlNode Parent { get; set; }
        private XmlDocument Action { get; set; }
        private string Path { get; set; }

        public AddCollection(XmlNode Parent, string Path)
        {
            if (Parent.ChildNodes.Count != 3)
            {
                throw new NotSupportedException("This item does not have the proper amount of children for 'AddCollection'");
            }

            this.Parent = Parent;

            XmlDocument data = new XmlDocument();
            data.Load(Path);
            Action = data;
            this.Path = Path;
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

        public CollectionTypes Type
        {
            get
            {
                switch (Parent.ChildNodes.Item(1).Attributes.Item(1).Value)
                {
                    case DataTypes.MasterCollection:
                        return CollectionTypes.MasterCollection;
                    case DataTypes.StandardCollection:
                        return CollectionTypes.StandardCollection;
                    case DataTypes.FavoriteCollection:
                        return CollectionTypes.FavoriteCollection;
                    case DataTypes.LegendaryCollection:
                        return CollectionTypes.LegendaryCollection;
                }
                return CollectionTypes.StandardCollection;
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

        public string IParent
        {
            get => Parent.ChildNodes.Item(2).Attributes.Item(1).Value;
            set
            {
                Parent.ChildNodes.Item(2).Attributes.Item(1).Value = value;
                Action.Save(Path);
            }
        }
    }
}
