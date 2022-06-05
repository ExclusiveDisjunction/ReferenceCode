using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Linq;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This a set of collections from a MasterCollection or a Session. This can enumerate into Collection.
    /// </summary>
    public sealed class CollectionList : IComponentList<Collection>
    {
        private List<Collection> Collections; //The collections.
        internal ISession _File; //The file to save to   
        internal XmlNode _Parent; //The parent node to add and remove from.
        private bool IsFromBin = false; //When true, then it does not allow add to add a type of 'Storage Collection'.
        private bool IsFromSession = false;
        private bool IsFromMSCollection = false;

        internal CollectionList(ICollectionParent Parent) //Gets all of the collections from the session, not from a defined list.
        {
            if (Parent is null || Parent.ParentFile is null || Parent.CollectionParent is null)
                throw new ArgumentNullException("The ISession Provided was null. This is not allowed.");

            this._File = Parent.ParentFile;
            this._Parent = Parent.CollectionParent;

            if (Parent is Session || Parent is Segment)
                this.IsFromSession = true;
            if (Parent is Bin)
                this.IsFromBin = true;
            if (Parent is Collection)
                this.IsFromMSCollection = true;

            Recalculate();
        }

        public Collection this[int Index] //Returns a collection based on an index. If Index >= Count || Index < 0, then it returns null when getting, and throws an IndexOutOfRangeException when setting.
        {
            get
            {
                if (Index >= Collections.Count || Index < 0)
                {
                    return null;
                }
                return Collections[Index];
            }
        }
        public int Count //Returns the number of collections in the List.
        {
            get
            {
                return Collections.Count;
            }
        }

        public ISession ParentFile => throw new NotImplementedException();

        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);
        private void Recalculate() //Saves the session and updates the values of Collections.
        {
            _File.Save();
            Collections = new List<Collection>();

            foreach (XmlNode collection in _Parent.ChildNodes)
            {
                if (collection.Name == "Collection")
                {
                    if ((IsFromMSCollection || IsFromBin) && collection.Attributes.Item(2).Value != "Master")
                        this.Collections.Add(new Collection(collection, this._File));
                    else
                        this.Collections.Add(new Collection(collection, this._File));
                }
                    
            }
            foreach (Collection item in Collections)
                item.Modifyed += ChildModifyed;
        }

        public Collection Add(CollectionTypes Type)
        {
            if (!IsFromSession && (Type == CollectionTypes.Master))
                throw new NotSupportedException("Master Collections can only be added to the Session.");

            XmlNode New = _File.TemplateNode.ChildNodes.Item(1).ChildNodes.Item((int)Type).Clone();
            New = _Parent.AppendChild(New);
            Recalculate();

            Modifyed?.Invoke(this);
            return new Collection(New, _File);
        }
        public void Remove(Collection Collection)
        {
            int Index = NodeIndexOf(Collection);
            if (Index < 0)
                return;
            XmlNode Item = _Parent.ChildNodes.Item(Index);
            _Parent.RemoveChild(Item);

            Recalculate();
            Modifyed?.Invoke(this);
        }

        public int NodeIndexOf(Collection Collection)
        {
            int i = 0;
            foreach (XmlNode item in _Parent.ChildNodes)
            {
                if (item == Collection._This)
                    return i;
                i++;
            }
            return -1;
        }
        public int IndexOf(Collection Collection) //Returns the index of a collection based of of it's name. Returns -1 if the collection does not exist.
        {
            int Index = 0;
            foreach (Collection collection in Collections)
            {
                if (collection == Collection)
                {
                    return Index;
                }
                Index++;
            }
            return -1;
        }
        public bool Contains(Collection Collection) //Evaluates if a collection specified is in the current list.
        {
            foreach (Collection collection in Collections)
            {
                if (collection == Collection)
                {
                    return true;
                }
            }
            return false;
        }

        public Phosphor.SortedSet<Collection> Sort() //Sorts the list using each collection's name.
        {
            var Result = from Item in Collections
                         orderby Item.Name ascending
                         select Item;
            return new Phosphor.SortedSet<Collection>(Result);
        }

        public IEnumerator<Collection> GetEnumerator() //Allows illiteration into collections.
        {
            return Collections.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public override string ToString()
        {
            return $"Count = {Count}";
        }
        public override bool Equals(object obj)
        {
            return obj is CollectionList && obj as CollectionList == this;
        }
        public bool Equals(IComponentList<Collection> other)
        {
            return other is CollectionList && other as CollectionList == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -141626995;
            hashCode = hashCode * -1521134295 + EqualityComparer<List<Collection>>.Default.GetHashCode(this.Collections);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._Parent);
            hashCode = hashCode * -1521134295 + this.IsFromBin.GetHashCode();
            hashCode = hashCode * -1521134295 + this.IsFromSession.GetHashCode();
            hashCode = hashCode * -1521134295 + this.IsFromMSCollection.GetHashCode();
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            return hashCode;
        }

        public static bool operator ==(CollectionList One, CollectionList Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null || One.Count != Two.Count)
                return false;
            bool Return = true;
            for (int i = 0; i < One.Count; i++)
            {
                if (Return)
                    Return = Return && One[i] == Two[i];
            }

            return Return;
        }
        public static bool operator !=(CollectionList One, CollectionList Two)
        {
            if (One is null || Two is null || One.Count != Two.Count)
                return true;
            bool Return = true;
            for (int i = 0; i < One.Count; i++)
            {
                if (Return)
                    Return = One[i] != Two[i];
            }
            return Return;
        }
    }
}
