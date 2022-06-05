using System;

namespace HmPro.Events
{
    public sealed class ComponentNullException : Exception
    {
        public ComponentNullException() : base("The requested list/requested object does not exist in the current context.")
        {

        }
        public ComponentNullException(string Message) : base(Message)
        {

        }
        public ComponentNullException(string Message, Exception Inner) : base(Message, Inner)
        {

        }
    }
}
