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
    public sealed class AttachmentSet : IComponentList<Attachment>
    {
        private List<Attachment> Files;
        internal ISession _File;
        internal XmlNode _Parent;

        public AttachmentSet(IFileParent Parent)
        {
            if (Parent is null || Parent.IsEmpty || Parent.ParentFile is null || Parent.FileParent is null)
                throw new ArgumentNullException("The file provided was null.");

            this._File = Parent.ParentFile;
            this._Parent = Parent.FileParent;

            Recalculate();
        }

        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);
        private void Recalculate()
        {
            _File.Save();
            Files = new List<Attachment>();

            foreach (XmlNode item in _Parent.ChildNodes)
            {
                if (item.Name == "File")
                    Files.Add(new Attachment(item, _File));
            }

            foreach (Attachment item in Files)
                item.Modifyed += ChildModifyed;
        }

        public Attachment this[int Index]
        {
            get
            {
                if (Index >= Count || Index < 0)
                    throw new IndexOutOfRangeException("The index is greater than or equal to the count, or less than 0.");
                return Files[Index];
            }
        }
        public int Count
        {
            get => Files.Count;
        }
        public ISession ParentFile
        {
            get => _File;
        }
        public Phosphor.SortedSet<Attachment> Sort()
        {
            var Return = from item in Files
                         orderby item.Path
                         select item;
            return new Phosphor.SortedSet<Attachment>(Return);
        }

        public bool Contains(Attachment Obj)
        {
            foreach (Attachment item in Files)
            {
                if (item == Obj)
                    return true;
            }
            return false;
        }
        public int NodeIndexOf(Attachment Obj)
        {
            int i = 0;
            foreach (XmlNode item in _Parent.ChildNodes)
            {
                if (item == Obj._This)
                    return i;
            }
            return -1;
        }
        public int IndexOf(Attachment Obj)
        {
            int i = 0;
            foreach (Attachment item in Files)
            {
                if (item == Obj)
                    return i;
                i++;
            }
            return -1;
        }

        public Attachment Add()
        {
            XmlNode New = _File.TemplateNode.ChildNodes.Item(3).Clone();
            New = _Parent.AppendChild(New);

            Modifyed?.Invoke(this);
            Recalculate();
            return new Attachment(New, _File);
        }
        public void Remove(Attachment Obj)
        {
            int index = NodeIndexOf(Obj);
            if (index < 0)
                return;
            _Parent.RemoveChild(_Parent.ChildNodes.Item(index));
            Recalculate();

            Modifyed?.Invoke(this);
        }

        public IEnumerator<Attachment> GetEnumerator()
        {
            return Files.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return Files.GetEnumerator();
        }

        public bool Equals(IComponentList<Attachment> other)
        {
            throw new NotImplementedException();
        }
        public override bool Equals(object obj)
        {
            return obj is AttachmentSet && obj as AttachmentSet == this;
        }
        public override string ToString()
        {
            return $"FileSet - {this.Count}";
        }
        public override int GetHashCode()
        {
            int hashCode = -1641575323;
            hashCode = hashCode * -1521134295 + EqualityComparer<List<Attachment>>.Default.GetHashCode(this.Files);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._Parent);
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            return hashCode;
        }

        public static bool operator ==(AttachmentSet One, AttachmentSet Two)
        {
            if (One is null && Two is null) return true;
            if (One.Count != Two.Count) return false;
            if (!ISessionComparer.ObjectsEqual(One._File, Two._File)) return false;

            bool Return = true;
            for (int i = 0; i < One.Count; i++)
            {
                Return = Return && One[i] == Two[i];
            }

            return Return;
        }
        public static bool operator !=(AttachmentSet One, AttachmentSet Two)
        {
            return !(One == Two);
        }
    }
}
