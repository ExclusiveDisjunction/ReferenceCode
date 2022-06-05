using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HmPro.Files
{
    public interface IObjectFinder
    {
        IComponent[] GetObject(string Title, bool Memes, bool Collections, bool Bins);
    }
}
