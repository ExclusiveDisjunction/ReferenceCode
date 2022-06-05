using System.Collections;
using System.Collections.Generic;
using System.Linq;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This represents a non-rigid list of Memes. This can be set with a list of memes, but does not offer adding and removing memes. This can enumerate into Meme.
    /// </summary>
    public sealed class LooseMemesList : IReadOnlySet<Meme>
    {
        private List<Meme> Memes { get; set; } //The current memes.

        internal LooseMemesList(IEnumerable<Meme> Memes) //Loads the class using an IEnumerable<Meme> item to get information from. (Ex: List<Meme>)
        {
            this.Memes = new List<Meme>(Memes);
        }

        public Meme this[int Index] //Gets an item from an index. If Index >= Count || Index < 0 then it returns null.
        {
            get
            {
                if (Index >= Count || Index < 0)
                {
                    return null;
                }
                return Memes[Index];
            }
        }
        public int Count //Returns the amount of items in the List.
        {
            get => Memes.Count;
        }

        public bool Contains(Meme Meme) //Determines if an item exists from a meme.
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
        public int IndexOf(Meme Meme) //Determines the index of an item provided. If the item does exist in the list, then it returns -1;
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
            return -1;
        }

        public Phosphor.SortedSet<Meme> Sort() //Sorts the list using each meme's name.
        {
            var Result = from Item in Memes
                         orderby Item.Name ascending
                         select Item;
            return new Phosphor.SortedSet<Meme>(Result);
        }

        public IEnumerator<Meme> GetEnumerator() //Allows illiteration to memes.
        {
            return Memes.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return Memes.GetEnumerator();
        }

        public override string ToString()
        {
            return $"Count = {Count}";
        }
        public override bool Equals(object obj)
        {
            return obj is LooseMemesList && obj as LooseMemesList == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -1635563494;
            hashCode = hashCode * -1521134295 + EqualityComparer<List<Meme>>.Default.GetHashCode(this.Memes);
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            return hashCode;
        }

        public static bool operator ==(LooseMemesList One, LooseMemesList Two)
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
        public static bool operator !=(LooseMemesList One, LooseMemesList Two)
        {
            if ((object)One == null || (object)Two == null || One.Count != Two.Count)
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
