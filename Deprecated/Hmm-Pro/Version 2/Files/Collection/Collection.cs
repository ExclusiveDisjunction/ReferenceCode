using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This represents a single collection in a set. This can read or write from a collection, and can enumerate into Meme.
    /// </summary>
    public sealed class Collection : IComponent, ICollectionParent, IMemeParent
    {
        internal ISession _File = null;
        internal XmlNode _This = null;

        internal Collection(XmlNode Collection, ISession File) //Initializes the class, reqesting a collection and a session.
        {
            if (File is null)
                throw new ArgumentNullException("The file provided was null.");
            if (!(Collection is null) && Collection.Name != "Collection")
                throw new InvalidOperationException("The XmlNode provided was not of type Collection.");

            _This = Collection;
            _File = File;

            if (this)
            {
                if (this.Type == CollectionTypes.Master)
                    Collections.Modifyed += ChildModifyed;
                Memes.Modifyed += ChildModifyed;
            }
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
                _This.Attributes.Item(0).Value = Fix.FixToName(value);
                _This.Attributes.Item(1).Value = value;

                _File.Save();
                Modifyed?.Invoke(this);
            }
        }
        public CollectionTypes Type //Returns the type from CollectionTypes.
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                switch (This.Attributes.Item(2).Value)
                {
                    case "Master":
                        return CollectionTypes.Master;
                    case "Legendary":
                        return CollectionTypes.Legendary;
                    case "Favorite":
                        return CollectionTypes.Favorite;
                    case "Standard":
                        return CollectionTypes.Standard;
                }
                return CollectionTypes.Standard;
            }
            set
            {
                CollectionTypes OldType = Type;

                if (!this)
                    throw new NullReferenceException("This is null.");
                This.Attributes.Item(2).Value = $"{value}";
                
                switch (OldType) //If the types are Master or Storage, it has to be able to convert them to the others by removing invalid children.
                {
                    case CollectionTypes.Master:
                    {
                        if (value != CollectionTypes.Master) //It has to remove all of the collections under it if the type to convert to is not a master or a storage collection.
                        {
                            foreach (XmlNode Item in _This.ChildNodes)
                            {
                                if (Item.Name != "Meme")
                                    _This.RemoveChild(Item);
                            }
                        }
                        break;
                    }
                }

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
                return _This.Attributes.Item(3).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                if (value is null)
                    throw new ArgumentNullException("The value tring to be set is null.");

                _This.Attributes.Item(3).Value = value;
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
                return _This.Attributes.Item(4).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");
                if (value is null)
                    throw new ArgumentNullException("The value tring to be set is null.");

                _This.Attributes.Item(4).Value = value;
                _File.Save();
                Modifyed?.Invoke(this);
            }
        }
        
        public MemeSet Memes //Returns the memes of the session, automaticly adusting between a master collection and every other type. All Collections can store memes.
        {
            get => !this ? null : new MemeSet(this);
        }
        public CollectionList Collections //Returns the collections of this collection. Only Master Collections store collections. Returns null if is not a master collection.
        {
            get => !this ? null : (this.Type == CollectionTypes.Master ? new CollectionList(this) : null);
        }
        public Phosphor.SortedSet<IComponentBase> LogicalChildren //Returns all of the children of this set ordered by their title.
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This is null.");

                List<IComponentBase> Return = new List<IComponentBase>();
                foreach (Meme item in Memes)
                    Return.Add(item);
                if (Type == CollectionTypes.Master)
                {
                    foreach (Collection Item in Collections)
                        Return.Add(Item);

                }

                var Sorted = from item in Return
                             orderby item.Title
                             select item;
                return new Phosphor.SortedSet<IComponentBase>(Sorted);
            }
        }

        public XmlNode CollectionParent { get => !this ? null : (Type == CollectionTypes.Master ? _This : null); }
        public XmlNode MemeParent { get => _This; }

        public override string ToString()
        {
            return $"Collection - {this.Title}";
        }
        public override bool Equals(object obj)
        {
            return obj is Collection && obj as Collection == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -1499194937;
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._This);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + this.Type.GetHashCode();
            hashCode = hashCode * -1521134295 + this.ObjectType.GetHashCode();
            hashCode = hashCode * -1521134295 + this.IsEmpty.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<MemeSet>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + EqualityComparer<CollectionList>.Default.GetHashCode(this.Collections);
            hashCode = hashCode * -1521134295 + EqualityComparer<Phosphor.SortedSet<IComponentBase>>.Default.GetHashCode(this.LogicalChildren);
            return hashCode;
        }

        public ComponentTypes ObjectType //Returns ComponentTypes.Collection
        {
            get => ComponentTypes.Collection;
        }
        public bool IsEmpty
        {
            get => _This is null;
        }
        public static implicit operator bool(Collection Obj)
        {
            return !Obj.IsEmpty;
        }
        public static bool operator ==(Collection One, Collection Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null)
                return false;
            bool Return = One.IsEmpty == Two.IsEmpty &&
                One.Memes == Two.Memes &&
                One.Name == Two.Name &&
                One.Type == Two.Type &&
                One.This == Two.This &&
                One.Title == Two.Title &&
                One.ObjectType == Two.ObjectType &&
                ISessionComparer.ObjectsEqual(One._File, Two._File);

            if (Return)
            {
                if (One.Type == CollectionTypes.Master)
                    Return = Return && One.Collections == Two.Collections;
            }

            return Return;
        }
        public static bool operator !=(Collection One, Collection Two)
        {
            return !(One == Two);
        }
    }
}
