using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using HmPro.Files;
using Phosphor;
using Phosphor.Files;

namespace HmPro
{
    public static class Recent
    {
        public static string GetRecent(RecentOptions Item) //Gets a recent item from recents.
        {
            SettingsParser Parser = new SettingsParser("instance\\recent.tehs");
            List<SettingsItem> Items = new List<SettingsItem>(Parser.GetAllItems());
            Items.Reverse();

            if ((int)Item >= Items.Count)
                return null;

            return Items[(int)Item].Value;
        }
        public static void AddRecent(string Value) //Sets the value of the first recent and slides all other recents back.
        {
            SettingsParser Parser = new SettingsParser("instance\\recent.tehs");

            List<string> Items = new List<string>(Parser.GetAllValues());
            if (Items.Contains(Value))
                return;

            Parser.Add("", SettingsItemTypes.RecentItem, Value, "");
        }
        public static string RecentName(RecentOptions Option) //Returns the name of a recent option.
        {
            string Path = GetRecent(Option);

            try
            {
                Session sess = new Session(Path);
                return sess.Title;
            }
            catch
            {
                return "#Missing";
            }
        }
        public static void DeleteRecent(int Index)
        {
            SettingsParser Parser = new SettingsParser("instance\\recent.tehs");
            List<SettingsItem> Items = new List<SettingsItem>(Parser.GetAllItems());

            if (Index >= Items.Count)
                return;

            Items.Reverse();
            SettingsItem Intrest = Items[Index];

            Parser.Remove(Intrest.Name, Intrest.Type, Intrest.Value, Intrest.FileTypeFilter);
        }
        public static void DeleteAllRecents()
        {
            new SettingsParser("instance\\recent.tehs").RemoveAll();
        }
    }
}
