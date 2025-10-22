using Phosphor;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Linq;

using HmPro.Events;
using System;
using System.Xml.Linq;
using System.Windows.Navigation;

namespace HmPro.Files
{
    /// <summary>
    /// The main class that files are read and written from. All components are split into sub classes. This class can be enumerated into collections.
    /// </summary>
    public sealed class Session : IHSession
    {
        public Session(string Path) //Initalizes the class using the specified path. If the path does not exist, then it copyies the basic template to it.
        {
            XmlDocument data = new XmlDocument();
            if (!System.IO.File.Exists(Path))
                System.IO.File.Copy("template\\session\\basic.teh", Path);
            data.Load(Path);
            _Document = data;
            this.Path = Path;
        }

        private XmlDocument _Document;
        public XmlDocument Document //The curent xmlDocument for the session.
        {
            get => _Document;
        }
        public string Path //The current path.
        {
            get; private set;
        }

        internal bool CheckSessionFormat()
        {
            try
            {
                object Temp = "";
                Temp = Name;
                Temp = Title;
                Temp = Version;

                Temp = Memes;
                Temp = Collections;
                Temp = FavoriteMemes;
                Temp = LegendaryMemes;
            }
            catch (ComponentNullException)
            {
                return false;
            }

            return true;
        }
        public void Save() //This does not save the document in the app to it's path like Common.Save() does, but it does save the current instance of the class. This only saves things within the file, such as a title change.
        {
            _Document.Save(Path);
        }
        public void Repair()
        {
            Create(Path);
        }

        public static Session Create(string Path)
        {
            XDocument Doc = new XDocument(); //Create The document
            XElement Root = new XElement("Session"); //Create the root element
            Root.Add(new XAttribute("Title", FileTools.FileName(Path)), new XAttribute("Version", "V_2020_1")); //Add the attributes
            Doc.Add(Root); //Append the root. 

            //NOTICE: All items are added the XElement Root. 

            Root.Add(
                new XElement("Session.FavoriteMemes"),
                new XElement("Session.LegendaryMemes"),
                new XElement("Session.Collections"),
                new XElement("Session.Files"),
                new XElement("Session.Bins")); //Adds the basic segment parts. 

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
            return new Session(Path);
        }

        public XmlNode ColorNode //this not part of HmmmPro 2019.1, but is part of ISession.
        {
            get => null;
        }
        public XmlNode BasicColor //this is not part of HmmmPro 2019.1, but is part of ISession.
        {
            get => null;
        }

        public string Name //Returns the session's name. ReadOnly. 
        {
            get => Title is null ? null : Fix.FixToName(Title);
        }
        public string Title //Return's the session's title. Get or Set. When setting, changes the name also.
        {
            get
            {
                XmlAttributeCollection Coll = _Document.DocumentElement.Attributes;
                XmlAttribute Attrib = null;
                foreach (XmlAttribute Thing in Coll)
                {
                    if (Thing.Name == "Title")
                        Attrib = Thing;
                }

                if (Attrib is null)
                    return null;

                return Attrib.Value;
            }
            set
            {
                if (value is null)
                    throw new NullReferenceException("The value null is not allowed to be set for title. ");

                XmlAttributeCollection Coll = _Document.DocumentElement.Attributes;
                XmlAttribute Attrib = null;
                foreach (XmlAttribute Thing in Coll)
                {
                    if (Thing.Name == "Title")
                        Attrib = Thing;
                }

                if (Attrib is null)
                    return;

                Attrib.Value = value;
                Save();
            }
        }
        public AppVersions Version //Returns the version as AppVersions. ReadOnly.
        {
            get
            {
                XmlAttributeCollection Coll = _Document.DocumentElement.Attributes;
                XmlAttribute Attrib = null;
                foreach (XmlAttribute Thing in Coll)
                {
                    if (Thing.Name == "Version")
                        Attrib = Thing;
                }

                if (Attrib is null)
                    return AppVersions.V_Unidentified;

                switch (Attrib.Value)
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
                XmlAttributeCollection Coll = _Document.DocumentElement.Attributes;
                XmlAttribute Attrib = null;
                foreach (XmlAttribute Thing in Coll)
                {
                    if (Thing.Name == "Version")
                        Attrib = Thing;
                }

                if (Attrib is null)
                    return;

                Attrib.Value = $"{value}";
                Save();
            }
        }

        public LooseMemesList Memes //Returns the sesson's memes from each collection. 
        {
            get
            {
                try
                {
                    List<Meme> Memes = new List<Meme>();
                    foreach (Collection collection in Collections)
                    {
                        foreach (Meme meme in collection.Memes)
                        {
                            Memes.Add(meme);
                        }
                    }
                    foreach (Meme meme in FavoriteMemes)
                    {
                        Memes.Add(meme);
                    }
                    foreach (Meme meme in LegendaryMemes)
                    {
                        Memes.Add(meme);
                    }
                    return new LooseMemesList(Memes);
                }
                catch
                {
                    throw new ComponentNullException();
                }
            }
        }
        public FavoriteMemes FavoriteMemes //Returns the favorite memes.
        {
            get
            {
                XmlNodeList Nodes = _Document.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Session.FavoriteMemes")
                        Target = Node;
                }

                if (Target is null)
                    return null;
                else
                    return new FavoriteMemes(this);
            }
        }
        public LegendaryMemes LegendaryMemes //Returns the legendary memes.
        {
            get
            {
                XmlNodeList Nodes = _Document.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Session.LegendaryMemes")
                        Target = Node;
                }

                if (Target is null)
                    return null;
                else
                    return new LegendaryMemes(this); 
            }
        }
        public CollectionList Collections //Returns the session's collections.
        {
            get => CollectionParent is null ? null : new CollectionList(this);
        }
        public AttachmentSet Files
        {
            get => FileParent is null ? null : new AttachmentSet(this);
        }
        public BinGroup Bins
        {
            get => BinParent is null ? null : new BinGroup(this);
        }

        public ISession ParentFile => this;
        public bool IsEmpty => false;

        public XmlNode CollectionParent
        {
            get
            {
                XmlNodeList Nodes = _Document.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Session.Collections")
                        Target = Node;
                }

                if (Target is null)
                    return null;
                else
                    return Target;
            }
        }
        public XmlNode FileParent
        {
            get
            {
                XmlNodeList Nodes = _Document.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Session.Files")
                        Target = Node;
                }

                if (Target is null)
                    return null;
                else
                    return Target;
            }
        }
        public XmlNode BinParent
        {
            get
            {
                XmlNodeList Nodes = _Document.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Session.Bins")
                        Target = Node;
                }

                if (Target is null)
                    return null;
                else
                    return Target;
            }
        }

        public XmlNode TemplateNode
        {
            get
            {
                XmlNodeList Nodes = _Document.DocumentElement.ChildNodes;
                XmlNode Target = null;
                foreach (XmlNode Node in Nodes)
                {
                    if (Node.Name == "Session.Templates")
                        Target = Node;
                }

                if (Target is null)
                    throw new FormatException("The session lacks a template node. This is required. Run Reconstruction.");
                else
                    return Target;
            }
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
            return 0;
        }

        public Meme[] GetMeme(string Title) //Gets a meme from the session by a name.
        {
            List<Meme> Return = new List<Meme>();
            foreach (Meme meme in this.Memes)
            {
                if (meme.Name == Title)
                    Return.Add(meme);
            }
            return Return.ToArray();
        }
        public Collection[] GetCollection(string Title) //Gets a collection from the session by a name.
        {
            List<Collection> Return = new List<Collection>();
            foreach (Collection collection in Collections)
            {
                if (collection.Name == Title)
                    Return.Add(collection);
            }
            return Return.ToArray();
        }
        public Bin[] GetBin(string Title)
        {
            List<Bin> Return = new List<Bin>();
            foreach (Bin item in Bins)
            {
                if (item.Name == Title)
                    Return.Add(item);
            }
            return Return.ToArray();
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

        public override string ToString()
        {
            return $"Name = '{Name}'";
        }
        public override bool Equals(object other)
        {
            return other != null && other is Session Other && this == Other;
        }
        public bool Equals(ISession other)
        {
            return other != null && other == this;
        }
        public override int GetHashCode()
        {
            var hashCode = -795803423;
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlDocument>.Default.GetHashCode(this._Document);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlDocument>.Default.GetHashCode(this.Document);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Path);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.ColorNode);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.BasicColor);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + this.Version.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<LooseMemesList>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + EqualityComparer<CollectionList>.Default.GetHashCode(this.Collections);
            hashCode = hashCode * -1521134295 + EqualityComparer<FavoriteMemes>.Default.GetHashCode(this.FavoriteMemes);
            hashCode = hashCode * -1521134295 + EqualityComparer<LegendaryMemes>.Default.GetHashCode(this.LegendaryMemes);
            hashCode = hashCode * -1521134295 + EqualityComparer<AttachmentSet>.Default.GetHashCode(this.Files);
            hashCode = hashCode * -1521134295 + EqualityComparer<BinGroup>.Default.GetHashCode(this.Bins);
            return hashCode;
        }        

        public static bool operator ==(Session One, Session Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null) return false;

            return One.Document == Two.Document &&
                Two.Path == One.Path &&
                Two.Name == One.Name &&
                Two.Title == One.Title &&
                Two.Version == One.Version;
        }
        public static bool operator !=(Session One, Session Two)
        {
            return !(One == Two);
        }
        public static bool operator ==(Session One, ISession Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null || !(Two is Session))
                return false;
            Session Oppose = (Session)Two;
            return EqualityComparer<XmlDocument>.Default.Equals(One.Document, Oppose.Document) &&
                Oppose.Path == One.Path &&
                Oppose.Name == One.Name &&
                Oppose.Title == One.Title;
        }
        public static bool operator !=(Session One, ISession Two)
        {
            return !(One == Two);
        }
        public static bool operator ==(ISession One, Session Two)
        {
            return Two == One;
        }
        public static bool operator !=(ISession One, Session Two)
        {
            return Two != One;
        }
    }
}
