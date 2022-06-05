using System;
using HmPro.Files;

namespace HmPro.Events
{
    /// <summary>
    /// Supplies information for Appending ofr (App.Appended)
    /// </summary>
    public sealed class AppendEventArgs : EventArgs
    {
        public Session Session //The session that was appened to.
        {
            get
            {
                if (Path == null)
                {
                    throw new ArgumentNullException("The path supplied == null.");
                }
                return new Session(Path);
            }
        }
        public string Path //The path of the appened session.
        {
            get; private set;
        }
        public bool? Sucessfull //The value of the append action.
        {
            get; private set;
        }

        /// <summary>
        /// Loads the class using a path and a response for if the appending's actions were sucessfull.
        /// </summary>
        /// <param name="Path">The path of the session saved to.</param>
        /// <param name="Sucessfull">The value of the append action (null for cancel, false for failure, true for sucessfull.)</param>
        /// <exception cref="System.IO.FileNotFoundException">The path supplied does not exist.</exception>
        /// <exception cref="ArgumentNullException">The path given is null.</exception>
        public AppendEventArgs(string Path, bool? Sucessfull)
        {
            if (Path == null)
            {
                throw new ArgumentNullException("The path supplied == null.");
            }

            if (!System.IO.File.Exists(Path))
            {
                throw new System.IO.FileNotFoundException("The path supplied does not exist on this computer.");
            }

            this.Path = Path;
            this.Sucessfull = Sucessfull;
        }
    }
}
