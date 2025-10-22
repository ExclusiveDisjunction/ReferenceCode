using System;

using HmPro.Scripting.Files;

namespace HmPro.Scripting.Events
{
    /// <summary>
    /// This is thrown by Collection Connector, and it is thrown when the user cancels the job or picks an item.
    /// </summary>
    public sealed class ConnectEventArgs : EventArgs
    {
        /// <summary>
        /// This determines if the job is connected, and if false, the user canceled the job.
        /// </summary>
        public bool Connected
        {
            get; private set;
        }
        /// <summary>
        /// This is the object that the user connected to.
        /// </summary>
        public Component Object
        {
            get; private set;
        }
        /// <summary>
        /// This is the session that the object is from.
        /// </summary>
        public Session Source
        {
            get; private set;
        }
        /// <summary>
        /// Used for if the Object = null, but Connected = true. This is a notification that states where the object is connecting to.
        /// </summary>
        public string Notes
        {
            get; private set;
        }

        /// <summary>
        /// Loads the class uisng the class's parameters and sets them, exempt Notes.
        /// </summary>
        /// <param name="Object">The object selected. Sets it equal to null if the obj is not connected.</param>
        /// <param name="Connected">True for if the object has been picked, false for cancel job.</param>
        /// <param name="Source">The session that it is from. Set equal to null if the obj is not connected.</param>
        public ConnectEventArgs(Component Object, bool Connected, Session Source)
        {
            if (!Connected)
            {
                Object = null;
                Source = null;
            }

            this.Connected = Connected;
            this.Object = Object;
            this.Source = Source;
        }
        /// <summary>
        /// Loads the class uisng the class's parameters and sets them.
        /// </summary>
        /// <param name="Object">The object selected. Sets it equal to null if the obj is not connected.</param>
        /// <param name="Connected">True for if the object has been picked, false for cancel job.</param>
        /// <param name="Source">The session that it is from. Set equal to null if the obj is not connected.</param>
        /// <param name="Notes">The notes to add on the connection.</param>
        public ConnectEventArgs(Component Object, bool Connected, Session Source, string Notes)
        {
            if (!Connected)
            {
                Object = null;
                Source = null;
            }

            this.Connected = Connected;
            this.Object = Object;
            this.Source = Source;
            this.Notes = Notes;
        }
    }
}
