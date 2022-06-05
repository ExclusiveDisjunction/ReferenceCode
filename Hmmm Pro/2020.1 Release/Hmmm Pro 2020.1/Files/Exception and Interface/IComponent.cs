
using Phosphor;

namespace HmPro
{
    /// <summary>
    /// This is the basis of every component inside of HmPro, and this carrys out the basic functions of these components. It does require that the parent class overrides the Type, but it allows each component to be converted to a bool (Returns Component.IsEmpty). This is abstract.
    /// </summary>
    public interface IComponent : IComponentBase
    {
        ComponentTypes ObjectType { get; }
    }
}
