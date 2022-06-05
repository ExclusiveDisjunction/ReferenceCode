using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Linq;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This is a rigid class that requires a single souce. This contains information over a group of Memes. This can enumerate into Meme.
    /// </summary>
    public sealed class MemeSet : IComponentList<Meme>
    {
        private List<Meme> Memes;//The memes.
        internal ISession _File; //The session to save.
        internal XmlNode _Parent; //The parent node to add or remove memes from. 

        internal MemeSet(IMemeParent Parent)
        {
            if (Parent is null || Parent.IsEmpty || Parent.ParentFile is null || Parent.MemeParent is null)
                throw new ArgumentNullException("One of the arguments was null.");

            this._File = Parent.ParentFile;
            this._Parent = Parent.MemeParent;

            Recalculate();
        }

        private void Recalculate() //Re-loads the Memes and saves the Session.
        {
            _File.Save();
            Memes = new List<Meme>();

            foreach (XmlNode child in _Parent.ChildNodes)
            {
                if (child.Name == "Meme")
                    Memes.Add(new Meme(child, _File));
            }
            foreach (Meme item in Memes)
                item.Modifyed += ChildModifyed;
        }

        public int Count //Returns the amout of memes in the list.
        {
            get
            {
                return Memes.Count;
            }
        }
        public Meme this[int Index] //Returns a meme from the list at an index. If the index >= Count || Index < 0 , it returns null.
        {
            get => Memes[Index] ?? null;
        }

        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);
        public ISession ParentFile
        {
            get => _File;
        }

        public void Remove(Meme Meme) //Remove a meme from the parent and recaluclates.
        {
            int Index = NodeIndexOf(Meme);
            if (Index < 0)
                return;
            _Parent.RemoveChild(_Parent.ChildNodes.Item(Index));
            Recalculate();
        }
        public Meme Add(MemeTypes Type) //Adds a meme to the parent and recalculates.
        {
            XmlNode New = _File.TemplateNode.ChildNodes.Item(0).ChildNodes.Item((int)Type).Clone();
            New = _Parent.AppendChild(New);
            Recalculate();

            return new Meme(New, _File);
        }

        public bool Contains(Meme Meme) //Evaluates if there is a meme in this list from a meme class.
        {
            foreach (Meme meme in Memes)
            {
                if (meme == Meme)
                {
                    return true;
                }
            }
            return false;
        }
        public int IndexOf(Meme Meme) //Returns the index of a meme from the list. Returns -1 if there is no meme in the list.
        {
            int Index = 0;
            foreach (Meme meme in Memes)
            {
                if (meme == Meme)
                {
                    return Index;
                }
                Index++;
            }
            return 0;
        }
        public int NodeIndexOf(Meme Meme)
        {
            int i = 0;
            foreach (XmlNode item in _Parent.ChildNodes)
            {
                if (item == Meme._This)
                    return i;
                i++;
            }
            return -1;
        }

        public Phosphor.SortedSet<Meme> Sort() //Sorts the set using each meme's name.
        {
            var Result = from Meme in Memes
                         orderby Meme.Name ascending
                         select Meme;
            return new Phosphor.SortedSet<Meme>(Result);
        }

        public IEnumerator<Meme> GetEnumerator() //Allows illiteration for memes.
        {
            return Memes.GetEnumerator();
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
            return obj is MemeSet && obj as MemeSet == this;
        }
        public bool Equals(IComponentList<Meme> obj)
        {
            return obj is MemeSet && obj as MemeSet == this;
        }
        public override int GetHashCode()
        {
            int hashCode = 2016380939;
            hashCode = hashCode * -1521134295 + EqualityComparer<List<Meme>>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._Parent);
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            return hashCode;
        }

        public static bool operator ==(MemeSet One, MemeSet Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null || One.Count != Two.Count || !ISessionComparer.ObjectsEqual(One._File, Two._File))
                return false;

            bool Return = true;
            for (int i = 0; i < One.Count; i++)
            {
                if (Return)
                    Return = Return && One[i] == Two[i];
            }

            return Return;
        }
        public static bool operator !=(MemeSet One, MemeSet Two)
        {
            return !(One == Two);
        }
    }
}
