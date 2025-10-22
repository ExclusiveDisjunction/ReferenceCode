using Phosphor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HmPro.Files
{
    public interface IHSession : ISession, ICollectionParent, IBinParent, IFileParent, ICountable<CollectionTypes>, ICountable<MemeTypes>, IObjectFinder 
    {
    }
}
