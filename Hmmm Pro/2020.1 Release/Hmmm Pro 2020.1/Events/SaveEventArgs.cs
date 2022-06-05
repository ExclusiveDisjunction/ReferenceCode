using System;

using HmPro.Files;
using Phosphor;

namespace HmPro.Events
{
    public sealed class SaveEventArgs : EventArgs
    {
        public IHSession Session //The session that it was saved to.
        {
            get
            {
                if (Path == null)
                {
                    throw new ArgumentNullException("The path supplied is null. This is not valid.");
                }
                return new Session(Path);
            }
        }
        public string Path { get; private set; } //The path of the object. This can only be set by the Initlilizer.

        /// <summary>
        /// Loads the event args using a path.
        /// </summary>
        /// <param name="Path">The path that was saved to.</param>
        public SaveEventArgs(string Path)
        {
            this.Path = Path ?? throw new ArgumentNullException("The path supplied is null. This is not valid.");
        }
    }
}
