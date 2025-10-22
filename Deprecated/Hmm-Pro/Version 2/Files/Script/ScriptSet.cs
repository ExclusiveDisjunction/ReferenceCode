using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Linq;

using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This class is used to show all of the scrips from a single meme. This can enmumerate into Script.
    /// </summary>
    public sealed class ScriptSet : IComponentList<Script>
    {
        private List<Script> Scripts; //The current Scripts.
        private ISession _File; //The Session to save to.
        private XmlNode _Parent;

        internal ScriptSet(IScriptParent Parent) //Loads the class from a meme, using it's childnodes as the Scripts, and then loads the session to save. If the meme is not a script meme, then it throws a System.FormatExecption. This also throws an error if the Session's version is below 2019.2.
        {
            if (Parent is null || Parent.IsEmpty || Parent.ParentFile is null || Parent.ScriptParent is null)
                throw new ArgumentNullException("The argument Parent is null, or one of its children are null.");

            this._Parent = Parent.ScriptParent;
            this._File = Parent.ParentFile;

            Recalculate();
        }

        public Script this[int Index] //Returns a script by an index. This is ReadOnly. If Index >= Count || Index < 0 then it returns null.
        {
            get
            {
                if (Index >= Count || Index < 0 )
                {
                    return null;
                }
                return Scripts[Index];
            }
        }
        public int Count //Returns the number of items in this list.
        {
            get
            {
                return Scripts.Count;
            }
        }
        public ISession ParentFile
        {
            get => _File;
        }
        public event BlankVoid Modifyed;

        private void Recalculate() //Saves the session and refreshes the list.
        {
            _File.Save();
            Scripts = new List<Script>();

            foreach (XmlNode item in _Parent.ChildNodes)
            {
                if (item.Name == "Script")
                    Scripts.Add(new Script(item, _File));
            }
            foreach (Script item in Scripts)
                item.Modifyed += ChildModifyed;
        }
        private void ChildModifyed(object t) => Modifyed?.Invoke(this);

        public Script Add() //Adds a script to the list and then recalculates.
        {
            XmlNode New = _File.TemplateNode.ChildNodes.Item(0).ChildNodes.Item(3).Clone();
            New = _Parent.AppendChild(New);
            Recalculate();

            return new Script(New, _File);
        }
        public void Remove(Script Script) //Removes a script from the list and then recaulculates.
        {
            int index = NodeIndexOf(Script);
            if (index < 0)
                return;
            _Parent.RemoveChild(_Parent.ChildNodes.Item(index));
            Recalculate();
        }

        public bool Contains(Script Script) //Evaluates if a script is in this list.
        {
            foreach (Script script in Scripts)
            {
                if (script == Script)
                {
                    return true;
                }
            }
            return false;
        }
        public int IndexOf(Script Script) //Gets the index of a provided item. If the item does not exist in the set, then it returns -1.
        {
            int Index = 0;
            for (int i =0; i < Count; i++)
            {
                if (this[i] == Script)
                {
                    return Index;
                }
                Index++;
            }
            return -1;
        }
        public int NodeIndexOf(Script Script)
        {
            int Index = 0;
            foreach (XmlNode item in _Parent.ChildNodes)
            {
                if (item == Script._This)
                    return Index;
                Index++;
            }
            return -1;
        }

        public Phosphor.SortedSet<Script> Sort() //Sorts the set by each script's person. Do not use.
        {
            return null;
        }

        public IEnumerator<Script> GetEnumerator() //Allows illiteration for Script.
        {
            return Scripts.GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public override string ToString()
        {
            return $"Count = {Count}";
        }
        public string ToString(bool Index)
        {
            string Return = "";
            int ind = 1;
            foreach (Script script in this)
            {
                if (Index)
                {
                    Return += $"{ind}: {script.Text}/n";
                    ind++;
                }
                else
                {
                    Return += $"{script.Person} : {script.Text}/n";
                }
            }
            return Return;
        }
        public override bool Equals(object obj)
        {
            return obj is ScriptSet && obj as ScriptSet == this;
        }
        public bool Equals(IComponentList<Script> obj)
        {
            return obj is ScriptSet && obj as ScriptSet == this;
        }
        public override int GetHashCode()
        {
            int hashCode = 659175149;
            hashCode = hashCode * -1521134295 + EqualityComparer<List<Script>>.Default.GetHashCode(this.Scripts);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._Parent);
            hashCode = hashCode * -1521134295 + this.Count.GetHashCode();
            return hashCode;
        }

        public static bool operator ==(ScriptSet One, ScriptSet Two)
        {
            if (One is null && Two is null) return true;
            if (One is null || Two is null || One.Count != Two.Count)
                return false;
            bool Return = true;
            for (int i = 0; i < One.Count; i++)
            {
                if (Return)
                    Return = One[i] == Two[i];
            }

            return Return;
        }
        public static bool operator !=(ScriptSet One, ScriptSet Two)
        {
            return !(One == Two);
        }
    }
}
