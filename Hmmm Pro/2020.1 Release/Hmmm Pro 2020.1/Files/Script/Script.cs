using System;
using System.Collections.Generic;
using System.Xml;
using Phosphor;

namespace HmPro.Files
{
    /// <summary>
    /// This represents a single Script from a meme. 
    /// </summary>
    public sealed class Script : IComponent
    {
        public const string Left = "Left"; //The text for left.
        public const string Right = "Right"; //The text for right.

        internal XmlNode _This;
        internal ISession _File;

        internal Script(XmlNode Script, ISession File) //Loads the class using an XmlNode and a session for saving. If the session's version is below 2019.2, then it throws an error.
        {
            if (File is null)
                throw new ArgumentNullException("One of the arguments are null. This is not allowed.");

            _This = Script;
            _File = File;
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
        public Phosphor.SortedSet<IComponentBase> LogicalChildren //Returns null, scrips cannot have logical children.
        {
            get => null;
        }

        public string Name //Returns null.
        {
            get => null;
        }
        public string Title //Returns null.
        {
            get => null;
            set => Modifyed?.Invoke(this);
        }
        public string Person //Returns the person from the script. Get or Set.
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This instance is null.");
                return _This.Attributes.Item(0).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This instance is null.");
                _This.Attributes.Item(0).Value = value;
                _File.Save();

                Modifyed?.Invoke(this);
            }
        }
        public ScriptPositions Position //Returns the position as a ScriptPosition (Left, Right). Get or Set.
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This instance is null.");

                if (This.Attributes.Item(1).Value == Left)
                    return ScriptPositions.Left;

                return ScriptPositions.Right;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This is null.");

                This.Attributes.Item(1).Value = $"{value}";
                _File.Save();

                Modifyed?.Invoke(this);
            }
        }
        public string Text //Returns the text from the script. Get or Set.
        {
            get
            {
                if (!this)
                    throw new NullReferenceException("This instance is null.");
                return _This.Attributes.Item(2).Value;
            }
            set
            {
                if (!this)
                    throw new NullReferenceException("This instance is null.");
                _This.Attributes.Item(2).Value = value;

                _File.Save();
                Modifyed?.Invoke(this);
            }
        }

        public ComponentTypes ObjectType //Returns ComponentTypes.Script.
        {
            get => ComponentTypes.Script;
        }
        public bool IsEmpty
        {
            get => _This is null;
        }        

        public static implicit operator bool(Script Obj)
        {
            return !Obj.IsEmpty;
        }
        public static bool operator ==(Script One, Script Two)
        {
            if (One is null && Two is null) return true;
            if (!One && !Two) return true;
            if (One is null || Two is null || !One || !Two) return false;

            return ISessionComparer.ObjectsEqual(One._File, Two._File) && 
                One.Name == Two.Name &&
                One.Person == Two.Person &&
                One.Position == Two.Position &&
                One.Text == Two.Text &&
                One.This == Two.This &&
                One.Title == Two.Title &&
                One.ObjectType == Two.ObjectType;
        }
        public static bool operator !=(Script One, Script Two)
        {
            return !(One == Two);
        }
        public override string ToString()
        {
            return Person + ", " + Text;
        }
        public override bool Equals(object obj)
        {
            return obj is Script && obj as Script == this;
        }
        public override int GetHashCode()
        {
            int hashCode = -1790686031;
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this._This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this._File);
            hashCode = hashCode * -1521134295 + EqualityComparer<XmlNode>.Default.GetHashCode(this.This);
            hashCode = hashCode * -1521134295 + EqualityComparer<ISession>.Default.GetHashCode(this.ParentFile);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Name);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Title);
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Person);
            hashCode = hashCode * -1521134295 + this.Position.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<string>.Default.GetHashCode(this.Text);
            hashCode = hashCode * -1521134295 + this.ObjectType.GetHashCode();
            hashCode = hashCode * -1521134295 + this.IsEmpty.GetHashCode();
            hashCode = hashCode * -1521134295 + EqualityComparer<Phosphor.SortedSet<IComponentBase>>.Default.GetHashCode(this.LogicalChildren);
            return hashCode;
        }
    }
}
