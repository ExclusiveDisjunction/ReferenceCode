namespace HmPro
{
    public enum ComponentTypes { Collection, Meme, Script, Bin, File }
    public enum MemeTypes { Standard = 0, Attachment = 1, Script = 2 }
    public enum CollectionTypes { Master = 0, Standard = 1, Favorite = 2, Legendary = 3}
    public enum UserTypes { Standard = 1, Admin = 2, Guest = 0}
    public enum ScriptPositions { Left, Right }
    public enum SessionSegments { FavoriteMemes = 0, LegendaryMemes = 1, Collections = 2, Users_Admin = 3, Users_Standard = 4, Files = 5, Bins = 6, Settings = 7}
}
