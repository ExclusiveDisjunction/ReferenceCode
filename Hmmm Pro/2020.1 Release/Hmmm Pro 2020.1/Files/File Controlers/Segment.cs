using Phosphor;
using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Linq;

namespace HmPro.Files
{
    /// <summary>
    /// Represents the file type (.tehseg) as its parser. It contains every type of object in a non-organized form. 
    /// </summary>
    public sealed class Segment : IHSession, IMemeParent, IScriptParent
    {
        private XmlDocument _Doc;
        private string _Path;

        public Segment(string Path)
        {
            XmlDocument data = new XmlDocument();
            if (!System.IO.File.Exists(Path))
                System.IO.File.Copy("template\\session\\basic.teh", Path);
            data.Load(Path);
            _Doc = data;
            this._Path = Path;
        }

        public void Save()
        {
            _Doc.Save(_Path);
        }
        public void Repair()
        {
            Create(_Path);
        }

        public static Segment Create(string Path)
        {
            XDocument Doc = new XDocument(); //Create The document
            XElement Root = new XElement("Session"); //Create the root element
            Root.Add(new XAttribute("Title", FileTools.FileName(Path)), new XAttribute("Version", "V_2020_1")); //Add the attributes
            Doc.Add(Root); //Append the root. 

            //NOTICE: All items are added the XElement Root. 

            //Begin Templates
            XElement Template = new XElement("Session.Templates");
            XAttribute[] Attrib = { new XAttribute("Name", ""), new XAttribute("Title", ""), new XAttribute("Creator", ""), new XAttribute("Desc", "") };

            XElement MemesHeader = new XElement("Templates.Memes");
            XElement CollectionsHeader = new XElement("Templates.Collections");

            //Meme Templates
            XElement[] Memes = { new XElement("Meme"), new XElement("Meme"), new XElement("Meme") };

            Memes[0].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Standard"), Attrib[2], Attrib[3]);
            Memes[1].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Attachment"), Attrib[2], Attrib[3]);
            Memes[2].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Script"), Attrib[2], Attrib[3]);

            MemesHeader.Add(Memes[0], Memes[1], Memes[2]);

            XElement ScriptTemplate = new XElement("Script");
            ScriptTemplate.Add(new XAttribute("Person", ""), new XAttribute("Position", ""), new XAttribute("Text", ""));
            MemesHeader.Add(ScriptTemplate);

            Template.Add(MemesHeader);
            //End Memes

            //Begin Collections
            XElement[] Collections = { new XElement("Collection"), new XElement("Collection"), new XElement("Collection"), new XElement("Collection") };

            Collections[0].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Master"), Attrib[2], Attrib[3]);
            Collections[1].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Standard"), Attrib[2], Attrib[3]);
            Collections[2].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Favorite"), Attrib[2], Attrib[3]);
            Collections[3].Add(Attrib[0], Attrib[1], new XAttribute("Type", "Legendary"), Attrib[2], Attrib[3]);

            CollectionsHeader.Add(Collections[0], Collections[1], Collections[2], Collections[3]);

            Template.Add(CollectionsHeader);
            //End Collections

            XElement BinTemplate = new XElement("Bin");
            BinTemplate.Add(Attrib[0], Attrib[1], Attrib[2], Attrib[3]);
            XElement FileTemplate = new XElement("File");
            FileTemplate.Add(new XAttribute("Path", ""));

            Template.Add(BinTemplate, FileTemplate);

            Root.Add(Template);
            //End Templates

            Doc.Save(Path);
            return new Segment(Path);
        }

        public string Name
        {
            get => Title is null ? null : Fix.FixToName(Title);
        }
        public string Title
        {
            get
            {
                XmlAttributeCollection Attrib = _Doc.DocumentElement.Attributes;
                XmlAttribute Target = null;
                foreach (XmlAttribute thing in Attrib)
                {
                    if (thing.Name == "Title")
                        Target = thing;
                }

                if (Target is null)
                    return null;
                else
                    return Target.Value;
            }
            set
            {
                if (value is null)
                    throw new NullReferenceException("The value trying to be set, null, is not an allowed value.");

                XmlAttributeCollection Attrib = _Doc.DocumentElement.Attributes;
                XmlAttribute Target = null;
                foreach (XmlAttribute thing in Attrib)
                {
                    if (thing.Name == "Title")
                        Target = thing;
                }

                if (Target is null)
                    return;
                else
                    Target.Value = value;
            }
        }
        public AppVersions Version
        {
            get
            {
                XmlAttributeCollection Attrib = _Doc.DocumentElement.Attributes;
                XmlAttribute Target = null;
                foreach (XmlAttribute thing in Attrib)
                {
                    if (thing.Name == "Title")
                        Target = thing;
                }

                if (Target is null)
                    return AppVersions.V_Unidentified;

                switch (Target.Value)
                {
                    case "V_2019_1":
                        return AppVersions.V_2019_1;
                    case "V_2019_2":
                        return AppVersions.V_2019_2;
                    case "V_2020_1":
                        return AppVersions.V_2020_1;
                    default:
                        return AppVersions.V_Unidentified;
                }
            }
            set
            {
                XmlAttributeCollection Attrib = _Doc.DocumentElement.Attributes;
                XmlAttribute Target = null;
                foreach (XmlAttribute thing in Attrib)
                {
                    if (thing.Name == "Title")
                        Target = thing;
                }

                if (Target is null)
                    return;
                else
                    Target.Value = $"{value}";
            }
        }

        public XmlNode ColorNode
        {
            get => null;
        }
        public XmlNode BasicColor
        {
            get => null;
        }

        public string Path
        {
            get => _Path;
        }
        public XmlDocument Document
        {
            get => _Doc;
        }

        public ISession ParentFile => this;
        public bool IsEmpty => false;

        public XmlNode MemeParent { get => _Doc.DocumentElement; }
        public XmlNode CollectionParent { get => _Doc.DocumentElement; }
        public XmlNode BinParent { get => _Doc.DocumentElement; }
        public XmlNode FileParent { get => _Doc.DocumentElement; }
        public XmlNode ScriptParent { get => _Doc.DocumentElement; }

        public XmlNode TemplateNode
        {
            get
            {
                XmlNodeList Nodes = _Doc.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Segment.Templates")
                        Target = Node;
                }

                if (Target is null)
                    throw new FormatException("The segment lacks a template node. This is required. Run Reconstruction.");
                else
                    return Target;
            }
        }

        public CollectionList Collections
        {
            get => new CollectionList(this);
        }
        public MemeSet Memes
        {
            get => new MemeSet(this);
        }
        public BinGroup Bins
        {
            get => new BinGroup(this);
        }
        public ScriptSet Scripts
        {
            get => new ScriptSet(this);
        }
        public AttachmentSet Files
        {
            get => new AttachmentSet(this);
        }

        public int CountOf(CollectionTypes Type) //Returns the number of collections by type. 
        {
            int Return = 0;
            foreach (Collection item in Collections)
            {
                if (item.Type == Type)
                    Return++;
            }

            return Return;
        }
        public int CountOf(MemeTypes Type) //Returns the number of memes by type.
        {
            int Return = 0;
            foreach (Meme item in Memes)
            {
                if (item.Type == Type)
                    Return++;
            }

            return Return;
        }
        public int CountOf(Type Type)
        {
            if (Type == typeof(Bin))
                return Bins is null ? 0 : Bins.Count;
            if (Type == typeof(Collection))
                return Collections is null ? 0 : Collections.Count;
            if (Type == typeof(Meme))
                return Memes is null ? 0 : Memes.Count;
            if (Type == typeof(Attachment))
                return Files is null ? 0 : Files.Count;
            if (Type == typeof(Script))
                return Scripts is null ? 0 : Memes.Count;
            return 0;
        }

        public IComponent[] GetObject(string Title, bool Memes, bool Collections, bool Bins) //Gets an object based off of a filter.
        {
            List<IComponent> Return = new List<IComponent>();

            if (Memes)
            {
                foreach (Meme item in this.Memes)
                {
                    if (item.Title == Title)
                        Return.Add(item);
                }
            }
            if (Collections)
            {
                foreach (Collection item in this.Collections)
                {
                    if (item.Title == Title)
                        Return.Add(item);
                }
            }
            if (Bins)
            {
                foreach (Bin item in this.Bins)
                {
                    if (item.Title == Title)
                        Return.Add(item);
                }
            }

            return Return.ToArray();
        }

        public override bool Equals(object obj)
        {
            return obj is Segment && obj as Segment == this;
        }
        public bool Equals(ISession other)
        {
            throw new NotImplementedException();
        }
        public override int GetHashCode()
        {
            int hashCode = -2038185851;
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlDocument>.Default.GetHashCode(this._Doc);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this._Path);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + this.Version.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.ColorNode);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.BasicColor);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Path);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlDocument>.Default.GetHashCode(this.Document);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            hashCode = hashCode * -1521134295 + this.IsEmpty.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.MemeParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.CollectionParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.BinParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.FileParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.ScriptParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<CollectionList>.Default.GetHashCode(this.Collections);
            hashCode = hashCode * -1521134295 + EqualityComparer<MemeSet>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + EqualityComparer<BinGroup>.Default.GetHashCode(this.Bins);
            hashCode = hashCode * -1521134295 + EqualityComparer<ScriptSet>.Default.GetHashCode(this.Scripts);
            hashCode = hashCode * -1521134295 + EqualityComparer<AttachmentSet>.Default.GetHashCode(this.Files);
            return hashCode;
        }
        public override string ToString()
        {
            return $"Segment - {Title}";
        }

        public static bool operator ==(Segment One, Segment Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null) return false;

            return One.Document == Two.Document &&
                Two.Path == One.Path &&
                Two.Name == One.Name &&
                Two.Title == One.Title &&
                Two.Version == One.Version;
        }
        public static bool operator !=(Segment One, Segment Two)
        {
            return !(One == Two);
        }
        public static bool operator ==(Segment One, ISession Two)
        {
            return Two is Segment && One == Two as Segment;
        }
        public static bool operator !=(Segment One, ISession Two)
        {
            return !(One == Two);
        }
        public static bool operator ==(ISession One, Segment Two)
        {
            return One is Segment && One as Segment == Two;
        }
        public static bool operator !=(ISession One, Segment Two)
        {
            return !(One == Two);
        }
    }
}
