using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

using Phosphor;

namespace HmPro.Files
{
    public sealed class Attachment : IComponent
    {
        internal XmlNode _This;
        internal ISession _File;

        public Attachment(XmlNode Node, ISession File)
        {
            if (File is null)
                throw new ArgumentNullException("The file is null.");

            _File = File;
            _This = Node;
        }
        
        public ISession ParentFile
        {
            get => _File;
        }
        public XmlNode This
        {
            get => _This;
        }
        public Phosphor.SortedSet<IComponentBase> LogicalChildren
        {
            get => null;
        }
        public event BlankVoid Modifyed;

        public string Name
        {
            get => null;
        }
        public string Title
        {
            get => null;
            set => Modifyed?.Invoke(this);
        }
        public string Path
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");

                return _This.Attributes.Item(0).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                if (value is null)
                    throw new ArgumentNullException("The value tring to be set is null. This is not allowed.");

                _This.Attributes.Item(0).Value = value;
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }

        public ComponentTypes ObjectType
        {
            get => ComponentTypes.File;
        }
        public bool IsEmpty
        {
            get => _This is null;
        }
        public static implicit operator bool(Attachment Obj)
        {
            return !Obj.IsEmpty;
        }
        public static bool operator ==(Attachment One, Attachment Two)
        {
            if (One is null && Two is null) return true;
            if (!One && !Two) return true;
            if (One is null || !One || Two is null || !Two) return false;

            return ISessionComparer.ObjectsEqual(One._File, Two._File) &&
                One.Path == Two.Path &&
                One._This == Two._File;
        }
        public static bool operator !=(Attachment One, Attachment Two)
        {
            return !(One == Two);
        }

        public override bool Equals(object obj)
        {
            return obj is Attachment && obj as Attachment == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -9666542;
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.This);
            hashCode = hashCode * -1521134295 + EqualityComparer<Phosphor.SortedSet<IComponentBase>>.Default.GetHashCode(this.LogicalChildren);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Path);
            hashCode = hashCode * -1521134295 + this.ObjectType.GetHashCode();
            hashCode = hashCode * -1521134295 + this.IsEmpty.GetHashCode();
            return hashCode;
        }
        public override string ToString()
        {
            return $"File - {this.Path}";
        }
    }
}
