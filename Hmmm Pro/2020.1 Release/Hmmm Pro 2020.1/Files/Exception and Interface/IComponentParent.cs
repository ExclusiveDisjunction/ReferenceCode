using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Phosphor;

namespace HmPro.Files
{
    public interface IComponentParent
    {
        ISession ParentFile { get; }
        bool IsEmpty { get; }
    }
}
