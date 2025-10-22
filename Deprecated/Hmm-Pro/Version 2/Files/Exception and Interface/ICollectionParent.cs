using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace HmPro.Files
{
    public interface ICollectionParent : IComponentParent
    {
        XmlNode CollectionParent { get; }
        CollectionList Collections { get; }
    }
}
