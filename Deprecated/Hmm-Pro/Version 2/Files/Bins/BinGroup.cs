using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Xml;

using Phosphor;

namespace HmPro.Files
{
    public sealed class BinGroup : IComponentList<Bin>
    {
        private List<Bin> Bins;
        internal ISession _File;
        internal XmlNode _Parent;

        internal BinGroup(IBinParent Parent)
        {
            if (Parent is null || Parent.IsEmpty || Parent.ParentFile is null || Parent.BinParent is null)
                throw new InvalidOperationException("The File is null, or the file is not a Session or Segemnt for Hmmm Pro.");

            this._File = Parent.ParentFile;
            this._Parent = Parent.BinParent;

            Recalculate();
        }

        public Bin this[int Index]
        {
            get
            {
                if (Index >= Count || Index < 0)
                    throw new IndexOutOfRangeException("The index provided is greater than or equal to Count, or less than 0.");
                return Bins[Index];
            }
        }
        public int Count
        {
            get => Bins.Count;
        }

        private void Recalculate()
        {
            _File.Save();
            Bins = new List<Bin>();

            foreach (XmlNode Item in _Parent.ChildNodes)
            {
                if (Item.Name == "Bin")
                    Bins.Add(new Bin(Item, _File));
            }
            foreach (Bin item in Bins)
                item.Modifyed += ChildModifyed;
        }

        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);
        public ISession ParentFile
        {
            get => _File;
        }

        public Bin Add()
        {
            XmlNode New = _File.TemplateNode.ChildNodes.Item(2).Clone();
            New = _Parent.AppendChild(New);
            Recalculate();

            Modifyed?.Invoke(this);
            return new Bin(New, _File);
        }
        public void Remove(Bin Obj)
        {
            int Index = NodeIndexOf(Obj);
            if (Index < 0)
                return;
            _Parent.RemoveChild(_Parent.ChildNodes.Item(Index));

            Modifyed?.Invoke(this);
            Recalculate();
        }

        public bool Contains(Bin Obj)
        {
            foreach (Bin Thing in Bins)
            {
                if (Thing == Obj)
                    return true;
            }
            return false;
        }
        public int IndexOf(Bin Obj)
        {
            for (int i = 0; i < Count; i++)
            {
                if (this[i] == Obj)
                {
                    return i;
                }
            }
            return -1;
        }
        public int NodeIndexOf(Bin Obj)
        {
            int i = 0;
            foreach (XmlNode Item in _Parent.ChildNodes)
            {
                if (Item == Obj._This)
                    return i;
                i++;
            }
            return -1;
        }

        public Phosphor.SortedSet<Bin> Sort()
        {
            var result = from item in Bins
                         orderby item.Name
                         select item;
            return new Phosphor.SortedSet<Bin>(result);
        }

        public IEnumerator<Bin> GetEnumerator()
        {
            return Bins.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return Bins.GetEnumerator();
        }

        public override string ToString()
        {
            return $"Count = {Count}";
        }
        public override bool Equals(object obj)
        {
            return obj is BinGroup Group && this == Group;
        }
        public bool Equals(IComponentList<Bin> obj)
        {
            return obj is BinGroup && this == obj as BinGroup;
        }
        public override int GetHashCode()
        {
            var hashCode = -793847765;
            hashCode = hashCode * -1521134295 + this.Bins.GetHashCode();
            hashCode = hashCode * -1521134295 + _File.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._Parent);
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            return hashCode;
        }

        public static bool operator ==(BinGroup One, BinGroup Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null || !ISessionComparer.ObjectsEqual(One._File, Two._File) || One.Count != Two.Count) return false;

            bool Return = true;
            for (int i = 0; i < One.Count; i++)
            {
                Return = Return && One[i] == Two[i];
            }
            return Return;
        }
        public static bool operator !=(BinGroup One, BinGroup Two)
        {
            return !(One == Two);
        }
    }
}
