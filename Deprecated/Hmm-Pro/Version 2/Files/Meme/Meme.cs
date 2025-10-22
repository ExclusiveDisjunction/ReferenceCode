using System.Collections.Generic;
using System.Linq;
using System.Xml;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This represents a single meme from a session.
    /// </summary>
    public sealed class Meme : IComponent, IScriptParent, IFileParent
    {
        internal ISession _File;
        internal XmlNode _This;

        internal Meme(XmlNode Meme, ISession File) //Loads the class using a XmlNode to read/wrte from. The session is for savnig.
        {
            _This = Meme;
            _File = File;

            if (this.Type == MemeTypes.Attachment)
            {
                foreach (Attachment item in Files)
                    item.Modifyed += ChildModifyed;
            }
            if (this.Type == MemeTypes.Script)
            {
                foreach (Attachment item in Files)
                    item.Modifyed += ChildModifyed;
            }
        }

        public XmlNode This //Retuns the node of the current item. This is _This.
        {
            get => _This;
        }
        public ISession ParentFile //Returns the owner document. This is _File.
        {
            get => _File;
        }
        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);

        public string Name //Returns the name of the meme. ReadOnly. To change the value, change title. 
        {
            get
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                return _This.Attributes.Item(0).Value;
            }
        }
        public string Title //Gets or sets the title of the meme. When set, it changes the Name of the meme in response.
        {
            get
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                return _This.Attributes.Item(1).Value;
            }
            set
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                _This.Attributes.Item(0).Value = Fix.FixToName(value);
                _This.Attributes.Item(1).Value = value;

                _File.Save();
            }
        }
        public MemeTypes Type //Return's the meme's type as MemeTypes. This is ReadOnly.
        {
            get
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");

                switch (This.Attributes.Item(2).Value)
                {
                    case "Attachment":
                        return MemeTypes.Attachment;
                    case "Standard":
                        return MemeTypes.Standard;
                    case "Script":
                        return MemeTypes.Script;
                }
                return MemeTypes.Standard;
            }
            set
            {
                if (!this)
                    throw new System.NullReferenceException("This is null.");

                if (Type != value)
                {
                    foreach (XmlNode item in _This.ChildNodes)
                        _This.RemoveChild(item);
                }

                This.Attributes.Item(2).Value = $"{value}";
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }
        public string Creator //Retruns the meme's creator. Get or Set.
        {
            get 
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                return _This.Attributes.Item(4).Value; 
            }
            set
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                _This.Attributes.Item(4).Value = value;
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }
        public string Description //Returns the meme's Descrption. Get or Set.
        {
            get
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                return _This.Attributes.Item(5).Value; 
            }
            set
            {
                if (!this)
                    throw new System.NullReferenceException("This instance is null.");
                _This.Attributes.Item(5).Value = value;
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }

        public XmlNode ScriptParent { get => _This; }
        public ScriptSet Scripts //Returns the scripts if the meme is a script meme. If this is not loaded, or not an attachment meme, it returns null.
        {
            get => !this ? null : (Type == MemeTypes.Script ? new ScriptSet(this) : null);
        }
        public XmlNode FileParent { get => _This; }
        public AttachmentSet Files //Returns the attachments if the meme is an attachment meme. If this is not loaded, or not an attachment meme, it returns null.
        {
            get => !this ? null : (Type == MemeTypes.Attachment ? new AttachmentSet(this) : null);
        }
        public Phosphor.SortedSet<IComponentBase> LogicalChildren //Returns the scripts as a sorted set, not intended for use. (Required by Phosphor.ComponentBase)
        {
            get => null;
        }

        public ComponentTypes ObjectType
        {
            get => ComponentTypes.Meme;
        }
        public bool IsEmpty
        {
            get => _This is null;
        }

        public static implicit operator bool(Meme Obj)
        {
            return !Obj.IsEmpty;
        }
        public static bool operator ==(Meme One, Meme Two)
        {
            if (One is null && Two is null) return true;
            if (!One && !Two) return true;
            if (One is null || Two is null || !One || !Two) return false;
            return One.Creator == Two.Creator &&
                One.Description == Two.Description &&
                One.Name == Two.Name &&
                One.Type == Two.Type &&
                One.This == Two.This &&
                One.Title == Two.Title &&
                One.Files == Two.Files &&
                One.Scripts == Two.Scripts &&
                ISessionComparer.ObjectsEqual(One._File, Two._File);
        }
        public static bool operator !=(Meme One, Meme Two)
        {
            return !(One == Two);
        }

        public override bool Equals(object obj)
        {
            return obj is Meme && obj as Meme == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -282658440;
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._This);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + this.Type.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Creator);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Description);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.ScriptParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<ScriptSet>.Default.GetHashCode(this.Scripts);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.FileParent);
            hashCode = hashCode * -1521134295 + EqualityComparer<AttachmentSet>.Default.GetHashCode(this.Files);
            hashCode = hashCode * -1521134295 + EqualityComparer<Phosphor.SortedSet<IComponentBase>>.Default.GetHashCode(this.LogicalChildren);
            hashCode = hashCode * -1521134295 + this.ObjectType.GetHashCode();
            hashCode = hashCode * -1521134295 + this.IsEmpty.GetHashCode();
            return hashCode;
        }
        public override string ToString()
        {
            return $"Meme - {Title}";
        }
    }

    public enum ScriptLocations { Favorite, Legendary }
}