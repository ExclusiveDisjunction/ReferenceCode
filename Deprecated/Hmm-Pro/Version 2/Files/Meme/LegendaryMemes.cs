using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Linq;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// Another type of MemeList, but this only focusus on LegendaryMemes from the session. This can enumerate into Meme.
    /// </summary>
    public sealed class LegendaryMemes : IComponentList<Meme>
    {
        private List<Meme> Memes; //The Memes.
        private ISession File; //The session to save.
        private XmlNode Parent; //The parent node, in this case, Session.SetDocument.DocumentElement.ChildNodes.Item(1).

        internal LegendaryMemes(Session File) //Loads the class, using the session to save from, pull memes, and set the parent from.
        {
            this.File = File;
            this.Parent = File.Document.DocumentElement.ChildNodes.Item(1);

            Recalculate();
        } 

        private void Recalculate() //Saves the session and updates the memes.
        {
            File.Save();
            Memes = new List<Meme>();

            foreach (XmlNode meme in Parent.ChildNodes)
            {
                if (meme.Name == "Meme")
                    Memes.Add(new Meme(meme, File));
            }
            foreach (Meme Item in Memes)
                Item.Modifyed += ChildModifyed;
        }

        public event BlankVoid Modifyed;
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);
        public int Count //Returns the number of items in the list.
        {
            get
            {
                return Memes.Count;
            }
        }
        public Meme this[int Index] //Returns a meme from an Index. If the Index >= Count || Index < 0, then it returns null. Zero Based.
        {
            get
            {
                if (Index >= Memes.Count || Index < 0)
                {
                    return null;
                }
                return Memes[Index];
            }
            set
            {

            }
        }
        public ISession ParentFile
        {
            get => File;
        }

        public bool Contains(Meme Meme) //Determines if there is a meme in the list that matches the one specified.
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
        public int IndexOf(Meme Meme) //Determines the index of a single meme from the set. If the meme does not exist in the set, then it returns -1.
        {
            int Index = 0;
            foreach (Meme meme in Memes)
            {
                if (meme == Meme)
                {
                    return Index;
                }
            }
            return -1;
        }
        public int NodeIndexOf(Meme Meme)
        {
            int Index = 0;
            foreach (XmlNode item in Parent.ChildNodes)
            {
                if (item == Meme._This)
                    return Index;
                Index++;
            }
            return -1;
        }

        public Meme Add(MemeTypes Type) //Adds a meme to the list and recalculates.
        {
            XmlNode New = File.TemplateNode.ChildNodes.Item(0).ChildNodes.Item((int)Type).Clone();
            New = Parent.AppendChild(New);

            Recalculate();
            return new Meme(New, File);
        }
        public void Remove(Meme Meme) //Removes a meme from the list and recalculates.
        {
            int Index = NodeIndexOf(Meme);
            if (Index < 0)
                return;
            Parent.RemoveChild(Parent.ChildNodes.Item(Index));
            Recalculate();           
        }
        public Phosphor.SortedSet<Meme> Sort() //Sorts the set by the meme's names.
        {
            var Result = from Item in Memes
                         orderby Item.Name ascending
                         select Item;
            return new Phosphor.SortedSet<Meme>(Result);
        }

        public IEnumerator<Meme> GetEnumerator() //Supports illiteration over memes.
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
            return obj is LegendaryMemes && obj as LegendaryMemes == this;
        }
        public bool Equals(IComponentList<Meme> other)
        {
            return other is LegendaryMemes && other as LegendaryMemes == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -1492134606;
            hashCode = hashCode * -1521134295 + EqualityComparer<List<Meme>>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.Parent);
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            return hashCode;
        }

        public static bool operator ==(LegendaryMemes One, LegendaryMemes Two)
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
        public static bool operator !=(LegendaryMemes One, LegendaryMemes Two)
        {
            return !(One == Two);
        }
    }
}