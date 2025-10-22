using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace HmPro.Files
{
    public interface IBinParent : IComponentParent
    {
        XmlNode BinParent { get; }
        BinGroup Bins { get; }
    }
}
