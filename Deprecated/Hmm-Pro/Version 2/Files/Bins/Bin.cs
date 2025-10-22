using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

using Phosphor;

namespace HmPro.Files
{
    public sealed class Bin : IComponent, IMemeParent, IScriptParent, IFileParent, ICollectionParent
    {
        internal XmlNode _This = null;
        internal ISession _File = null;

        public Bin(XmlNode Node, ISession File)
        {
            if (!(Node is null) && Node.Name != "Bin")
                throw new ArgumentException("The Node provided was not of type Bin.");

            _This = Node;
            _File = File;

            //Objects do not have to initialized bc this is not a list. The child objects are retrived when the Individual properties are called.
            Collections.Modifyed += ChildModifyed;
            Files.Modifyed += ChildModifyed;
            Memes.Modifyed += ChildModifyed;
            Scripts.Modifyed += ChildModifyed;
        }

        public XmlNode This
        {
            get => _This;
        }
        public ISession ParentFile
        {
            get => _File;
        }
        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);
        public Phosphor.SortedSet<IComponentBase> LogicalChildren
        {
            get
            {
                List<IComponentBase> Return = new List<IComponentBase>();
                foreach (Collection item in Collections)
                {
                    Return.Add(item);
                }
                foreach (Script item in Scripts)
                {
                    Return.Add(item);
                }
                foreach (Meme item in Memes)
                {
                    Return.Add(item);
                }
                foreach (Script item in Scripts)
                {
                    Return.Add(item);
                }

                var Sorted = from item in Return
                             orderby item.Name
                             select item;
                return new Phosphor.SortedSet<IComponentBase>(Sorted);
            }
        }

        public string Name
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");

                return _This.Attributes.Item(0).Value;
            }
        }
        public string Title
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");

                return _This.Attributes.Item(1).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                if (value is null)
                    throw new ArgumentNullException("The value tring to be set is null. This is not allowed");

                _This.Attributes.Item(0).Value = Fix.FixToName(value);
                _This.Attributes.Item(1).Value = value;
                _File.Save();

                Modifyed?.Invoke(this);
            }
        }
        public string Creator
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                return _This.Attributes.Item(2).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                if (value is null)
                    throw new ArgumentNullException("The value tring to be set is null. This is not allowed");

                _This.Attributes.Item(2).Value = value;
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }
        public string Description
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");

                return _This.Attributes.Item(3).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                if (value is null)
                    throw new ArgumentNullException("The value tring to be set is null. This is not allowed");

                _This.Attributes.Item(3).Value = value;
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }

        public CollectionList Collections
        {
            get => !this ? null : new CollectionList(this);
        }
        public MemeSet Memes
        {
            get => !this ? null : new MemeSet(this);
        }
        public ScriptSet Scripts
        {
            get => !this ? null : new ScriptSet(this);
        }
        public AttachmentSet Files
        {
            get => !this ? null : new AttachmentSet(this);
        }

        public XmlNode MemeParent { get => _This; }
        public XmlNode ScriptParent { get => _This; }
        public XmlNode FileParent { get => _This; }
        public XmlNode CollectionParent { get => _This; }

        public bool Contains(IComponent Obj)
        {
            switch (Obj.ObjectType)
            {
                case ComponentTypes.Bin:
                    return false;
                case ComponentTypes.Collection:
                    return Collections.Contains(Obj as Collection);
                case ComponentTypes.File:
                    return Files.Contains(Obj as Attachment);
                case ComponentTypes.Meme:
                    return Memes.Contains(Obj as Meme);
                case ComponentTypes.Script:
                    return Scripts.Contains(Obj as Script);
                default:
                    return false;
            }
        }
        public int IndexOf(IComponent Obj)
        {
            switch (Obj.ObjectType)
            {
                case ComponentTypes.Collection:
                    return Collections.IndexOf(Obj as Collection);
                case ComponentTypes.File:
                    return Files.IndexOf(Obj as Attachment);
                case ComponentTypes.Meme:
                    return Memes.IndexOf(Obj as Meme);
                case ComponentTypes.Script:
                    return Scripts.IndexOf(Obj as Script);
                default:
                    return -1;
            }
        }

        public override bool Equals(object obj)
        {
            return obj is Bin && obj as Bin == this;
        }
        public override int GetHashCode()
        {
            int hashCode = 1124555222;
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            hashCode = hashCode * -1521134295 + EqualityComparer<Phosphor.SortedSet<IComponentBase>>.Default.GetHashCode(this.LogicalChildren);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Creator);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Description);
            hashCode = hashCode * -1521134295 + EqualityComparer<CollectionList>.Default.GetHashCode(this.Collections);
            hashCode = hashCode * -1521134295 + EqualityComparer<MemeSet>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + EqualityComparer<ScriptSet>.Default.GetHashCode(this.Scripts);
            hashCode = hashCode * -1521134295 + EqualityComparer<AttachmentSet>.Default.GetHashCode(this.Files);
            hashCode = hashCode * -1521134295 + this.IsEmpty.GetHashCode();
            hashCode = hashCode * -1521134295 + this.ObjectType.GetHashCode();
            return hashCode;
        }
        public override string ToString()
        {
            return $"Bin - {this.Title}";
        }

        public bool IsEmpty
        {
            get => _This is null;
        }
        public ComponentTypes ObjectType
        {
            get => ComponentTypes.Bin;
        }
        public static implicit operator bool(Bin Obj)
        {
            return !Obj.IsEmpty;
        }
        public static bool operator ==(Bin One, Bin Two)
        {
            if (One is null && Two is null) return true;
            if (!One && !Two) return true;
            if (One is null || Two is null || !One || !Two) return false;

            return ISessionComparer.ObjectsEqual(One.ParentFile, Two.ParentFile) &&
                One.Collections == Two.Collections &&
                One.Creator == Two.Creator &&
                One.Description == Two.Description &&
                One.Files == Two.Files &&
                One.Memes == Two.Memes &&
                One.Name == Two.Name &&
                One.Scripts == Two.Scripts &&
                One.This == Two.This &&
                One.Title == Two.Title &&
                One.ObjectType == Two.ObjectType;

        }
        public static bool operator !=(Bin One, Bin Two)
        {
            return !(One == Two);
        }
    }
}
