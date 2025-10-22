using Phosphor;

namespace HmPro.Files
{
    public static class ISessionComparer
    {
        public static bool ObjectsEqual(ISession Left, ISession Right)
        {
            if (Left is null && Right is null) return true;
            if (Left is null || Right is null) return false;
            if ((Left is Session && Right is Segment) || (Left is Segment && Right is Session)) return false;

            if (Left is Session Session1 && Right is Session Session2) return Session1 == Session2;
            else if (Left is Segment Segment1 && Right is Segment Segment2) return Segment1 == Segment2;
            else return false;                
        }
        public static bool TypesEqual(ISession Left, ISession Right)
        {
            if (Left is Session && Right is Session) return true;
            else if (Left is Segment && Right is Segment) return true;
            else return false;
        }
    }
}
