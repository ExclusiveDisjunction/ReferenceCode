using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HmPro.Registry
{
    /// <summary>
    /// A Registry designed to link Help Topic buttons with a corrisponding help topic page. 
    /// </summary>
    public sealed class HelpReg
    {
        private Window DocWindow;

        public HelpReg(Window Documentation)
        {
            if (!(Documentation is IDocumentationWindow))
                throw new ArgumentException("The argument window is not a IDocumentationWindow.");
        }
    }

    public interface IDocumentationWindow
    {
        void HandleCommand(string path);
        IEnumerable<string> GetAllCommands { get; }
    }
}
