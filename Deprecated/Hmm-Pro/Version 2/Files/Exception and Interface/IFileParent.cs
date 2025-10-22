using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace HmPro.Files
{
    public interface IFileParent : IComponentParent
    {
        XmlNode FileParent { get; }
        AttachmentSet Files { get; }
    }
}
